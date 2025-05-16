#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hardware/gpio.h"
#include "hardware/i2c.h"
#include "pico/cyw43_arch.h"

#include "lwip/tcp.h"
#include "lwip/netif.h"

#include "pico/time.h"

// Biblioteca SSD1306
#include "lib/ssd1306.h"
#include "lib/font.h"

#define ALARME_DURACAO_MS 10000
#define BUZZER_BEEP_INTERVAL_MS 500

bool alarm_active = false;
absolute_time_t alarm_start;
absolute_time_t last_beep_time;
bool buzzer_on = false;

// ======== CONFIGURAÇÃO WI-FI ========
#define WIFI_SSID "Filipe"
#define WIFI_PASSWORD "23052148"

// ======== DEFINIÇÃO DE GPIOs ========
#define BUZZER_PIN 21
#define LED_ALERTA_PIN 13         // LED vermelho
#define JOYSTICK_ADC_GPIO 26      // GPIO do potenciômetro (ADC0)

// I2C para OLED
#define I2C_PORT i2c1
#define I2C_SDA  14
#define I2C_SCL  15
#define OLED_ADDR 0x3C

bool alerta = false;

// Ponteiro do display
ssd1306_t oled;

// ======== INICIALIZAÇÃO DE HARDWARE ========
void inicializa_hardware(void) {
    gpio_init(BUZZER_PIN);
    gpio_set_dir(BUZZER_PIN, GPIO_OUT);
    gpio_put(BUZZER_PIN, 0);

    gpio_init(LED_ALERTA_PIN);
    gpio_set_dir(LED_ALERTA_PIN, GPIO_OUT);
    gpio_put(LED_ALERTA_PIN, 0);

    adc_init();
    adc_gpio_init(JOYSTICK_ADC_GPIO); // GPIO26 = ADC0
    adc_select_input(0); // ADC0

    // I2C e SSD1306
    i2c_init(I2C_PORT, 400 * 1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);

    ssd1306_init(&oled, WIDTH, HEIGHT, false, OLED_ADDR, I2C_PORT);
    ssd1306_config(&oled);
    ssd1306_fill(&oled, false);
    ssd1306_send_data(&oled);
}

// ======== LEITURA DO JOYSTICK COMO UMIDADE ========
float ler_umidade(void) {
    adc_select_input(0); // ADC0
    uint16_t raw = adc_read(); // Valor entre 0 e 4095
    float umidade_percent = 100.0f - ((raw * 100.0f) / 4095.0f);
    printf("ADC: %d | Umidade: %.2f%%\n", raw, umidade_percent);
    return umidade_percent;
}

// ======== DISPLAY OLED ========
void atualizar_oled(float umidade) {
    char buf[32];
    ssd1306_fill(&oled, false);

    ssd1306_draw_string(&oled, "AgroSmart", 25, 0);
    ssd1306_draw_string(&oled, "Umidade:", 0, 20);
    snprintf(buf, sizeof(buf), "%.1f%%", umidade);
    ssd1306_draw_string(&oled, buf, 72, 20);

    if (alerta) {
        ssd1306_draw_string(&oled, "ALERTA BAIXO!", 0, 40);
    } else {
        ssd1306_draw_string(&oled, "Nivel OK", 0, 40);
    }

    ssd1306_send_data(&oled);
}

// ======== ALERTA DE BAIXA UMIDADE ========
void verificar_alerta(float umidade) {
    if (umidade < 30.0f && !alarm_active) {
        alerta = true;
        gpio_put(LED_ALERTA_PIN, 1);
        alarm_active = true;
        alarm_start = get_absolute_time();
        last_beep_time = get_absolute_time();
        buzzer_on = false;
    }
}

void resetar_alerta(void) {
    alerta = false;
    gpio_put(LED_ALERTA_PIN, 0);
    gpio_put(BUZZER_PIN, 0);
}

// ======== TRATAMENTO DAS REQUISIÇÕES HTML ========
void tratar_requisicao(char *req) {
    if (strstr(req, "GET /resetar") != NULL) {
        resetar_alerta();
    }
}

// ======== CALLBACK: RECEBIMENTO DE DADOS ========
err_t receber_requisicao(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err) {
    if (!p) {
        tcp_close(tpcb);
        return ERR_OK;
    }

    char *req = malloc(p->len + 1);
    memcpy(req, p->payload, p->len);
    req[p->len] = '\0';

    printf("Requisição recebida:\n%s\n", req);
    tratar_requisicao(req);

    float umidade = ler_umidade();
    if (!alerta) verificar_alerta(umidade);

    // Atualiza display
    atualizar_oled(umidade);

    // Monta HTML
    char html[1024];
    snprintf(html, sizeof(html),
    "HTTP/1.1 200 OK\r\n"
    "Content-Type: text/html\r\n\r\n"
    "<!DOCTYPE html><html><head><title>AgroSmart</title>"
    "<style>"
      "body{font-family:sans-serif;text-align:center;}"
      "h1{margin-top:30px;}"
      "button{font-size:20px;padding:10px;margin:5px;}"
    "</style>"
    "</head><body>"
      "<h1>AgroSmart</h1>"
      "<p>Umidade: %.1f%%</p>"
      "%s"
      "<form action='/'><button type='submit'>Atualizar</button></form>"
      "<form action='/resetar'><button type='submit'>Resetar Alerta</button></form>"
    "</body></html>",
    umidade,
    alerta 
      ? "<p style='color:red;'>Alerta: Umidade baixa!</p>"
      : "<p style='color:green;'>Nivel OK</p>"
    );

    tcp_write(tpcb, html, strlen(html), TCP_WRITE_FLAG_COPY);
    tcp_output(tpcb);

    free(req);
    pbuf_free(p);
    return ERR_OK;
}

// ======== CALLBACK: CONEXÃO ACEITA ========
static err_t aceitar_conexao(void *arg, struct tcp_pcb *newpcb, err_t err) {
    tcp_recv(newpcb, receber_requisicao);
    return ERR_OK;
}

// ======== MAIN ========
int main() {
    stdio_init_all();
    inicializa_hardware();

    if (cyw43_arch_init()) {
        printf("Erro ao iniciar CYW43\n");
        return -1;
    }

    cyw43_arch_enable_sta_mode();

    printf("Conectando ao Wi-Fi...\n");
    while (cyw43_arch_wifi_connect_timeout_ms(WIFI_SSID, WIFI_PASSWORD, CYW43_AUTH_WPA2_AES_PSK, 30000)) {
        printf("Falha ao conectar. Tentando novamente...\n");
        sleep_ms(3000);
    }

    printf("Conectado!\n");
    if (netif_default)
        printf("IP: %s\n", ipaddr_ntoa(&netif_default->ip_addr));

    struct tcp_pcb *server = tcp_new();
    if (!server || tcp_bind(server, IP_ADDR_ANY, 80) != ERR_OK) {
        printf("Erro ao iniciar servidor TCP\n");
        return -1;
    }

    server = tcp_listen(server);
    tcp_accept(server, aceitar_conexao);
    printf("Servidor ouvindo na porta 80...\n");

    while (true) {
        cyw43_arch_poll();
        sleep_ms(100);
        if (alarm_active) {
    // Verifica se já passou o tempo do alarme
    if (absolute_time_diff_us(alarm_start, get_absolute_time()) > ALARME_DURACAO_MS * 1000) {
        alarm_active = false;
        gpio_put(BUZZER_PIN, 0);  // Garante que desliga ao final
        buzzer_on = false;
    } else {
        // Alterna o buzzer a cada BUZZER_BEEP_INTERVAL_MS
        if (absolute_time_diff_us(last_beep_time, get_absolute_time()) > BUZZER_BEEP_INTERVAL_MS * 1000) {
            buzzer_on = !buzzer_on;
            gpio_put(BUZZER_PIN, buzzer_on);
            last_beep_time = get_absolute_time();
        }
    }
}

    }

    cyw43_arch_deinit();
    return 0;
}