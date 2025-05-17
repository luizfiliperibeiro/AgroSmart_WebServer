# 🌱 AgroSmart – Estação de Monitoramento de Solo com Webserver

Este projeto foi desenvolvido como parte do tópico **Comunicação em IoT - 37M3SE (Residência Embarcatech)**, com o objetivo de criar uma estação inteligente de monitoramento de solo embarcada, acessível via interface Web.

---

## 🧠 Objetivo

Simular um sistema embarcado de agricultura de precisão que monitora:
- Umidade do solo (simulada via joystick vertical)
- Temperatura (simulada via joystick horizontal)

Com suporte a:
- Interface Web com atualização em tempo real
- Alertas visuais e sonoros
- Controle manual de irrigação

---

## 🛠 Tecnologias e Recursos

- ✅ **Placa BitDogLab (RP2040 + CYW43439)**
- ✅ **Wi-Fi via CYW43 com LwIP (modo polling)**
- ✅ **Servidor Web embarcado na porta 80**
- ✅ **Display OLED SSD1306 (I2C)**
- ✅ **Joystick analógico (ADC0 e ADC1)**
- ✅ **LEDs e buzzer para alertas**
- ✅ **Botões Web para reset e irrigação**

---

## 💻 Interface Web

Acessível via navegador (Wi-Fi local). Apresenta:

- Umidade do solo (em %)
- Status do sistema: alerta, irrigação ativa
- Botões para:
  - 🔄 Atualizar os dados
  - 🚫 Resetar o alerta
  - 🚿 Iniciar irrigação
  - 🛑 Parar irrigação

---

## 📟 Interface OLED

O display exibe:
- Título do projeto
- Umidade
- Temperatura
- Status de irrigação

---

## 🔧 Como compilar

Pré-requisitos:
- Pico SDK instalado e configurado
- VS Code com CMake Tools **ou** uso do terminal com `cmake` e `make`

### Passos:

```bash
git clone https://github.com/seu-usuario/agrosmart_webserver.git
cd agrosmart_webserver
mkdir build
cd build
cmake ..
make
```
O arquivo .uf2 será gerado e pode ser gravado na placa via modo bootloader.

---

## 👨‍💻 Autor

Luiz Filipe Ribeiro de Jesus

Comunicação em IoT (37M3SE)

Polo: Vitória da Conquista

Professor: Dr. Ricardo Menezes Prates

Mentor: Auerê Vasconcelos Veras

Data: Maio de 2025

---

## 🎥 Demonstração em Vídeo

📹 [Clique aqui para assistir à demonstração do projeto](https://drive.google.com/file/d/1f3V-rScIezGIDB3gVNh9bIogvQNJ-jGM/view?usp=drive_link)