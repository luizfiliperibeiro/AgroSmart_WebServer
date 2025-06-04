# 🌱 AgroSmart MQTT – Estação Inteligente com Publicação e Controle via Broker

Este projeto integra sensores simulados de umidade e temperatura com um sistema embarcado conectado a um **broker MQTT**, permitindo o **monitoramento remoto em tempo real**, além de **controle manual da irrigação e reset de alertas** via tópicos MQTT.

---

## 🎯 Objetivo

Simular um sistema embarcado de **agricultura de precisão** com:

- Monitoramento de **umidade do solo** e **temperatura ambiente** (via joystick)
- **Publicação contínua** dos dados via MQTT
- **Alertas automáticos** com LED e buzzer
- **Controle remoto** da irrigação via MQTT
- **Interface visual local** com OLED SSD1306

---

## 🛠️ Tecnologias e Recursos

- ✅ **Placa BitDogLab RP2040 + CYW43439** (Wi-Fi)
- ✅ MQTT via **LWIP embutido no Pico SDK**
- ✅ **Broker local** (Mosquitto) com autenticação
- ✅ **Display OLED SSD1306** com I2C
- ✅ **Joystick analógico** (GPIO26 e GPIO27) para simulação de sensores
- ✅ LEDs e buzzer para alertas
- ✅ **Controle por tópicos MQTT** usando ferramentas como:
  - `mosquitto_pub` / `mosquitto_sub`
  - **MQTT Explorer**
  - **IoT MQTT Panel (Android)**

---

## 🔌 Ligações

| Componente       | GPIO          |
|------------------|---------------|
| Joystick Umidade | ADC0 (GPIO26) |
| Joystick Temp.   | ADC1 (GPIO27) |
| LED Alerta       | GPIO13        |
| LED Irrigação    | GPIO11        |
| Buzzer           | GPIO21        |
| I2C SDA (OLED)   | GPIO14        |
| I2C SCL (OLED)   | GPIO15        |

---

## 🔄 Tópicos MQTT

| Tópico                 | Direção      | Função                              |
|------------------------|--------------|-------------------------------------|
| `/temperatura`         | 📨 Publish   | Publica valor da temperatura (°C)   |
| `/umidade`             | 📨 Publish   | Publica valor da umidade (%)        |
| `/irrigacao/comando`   | 📥 Subscribe | `"on"` ou `"off"` → ativa irrigação |
| `/irrigacao/status`    | 📨 Publish   | `"ativada"` ou `"parada"`           |
| `/alerta/resetar`      | 📥 Subscribe | `"reset"` → reseta alerta/buzzer    |

---

## 🖥️ Display OLED

Exibe em tempo real:

```bash
🌱 AgroSmart
Umid: 52.3 %
Temp: 28.7 C
Nivel OK ← ou: UMID. BAIXA!
Irrigando... ← somente se ativa
```

---

## 🔔 Alerta Automático

- Ativado se umidade < 30%
- LED vermelho acende
- Buzzer emite bipes por 10 segundos
- Pode ser resetado via MQTT (`/alerta/resetar`)

---

## 📦 Compilação

Projeto feito com **Pico SDK + LWIP + MQTT + FreeRTOS (opcional)**. Para compilar:

```bash
cd agrosmart_mqtt
mkdir build && cd build
cmake ..
ninja
```

---

## 📡 Broker MQTT Recomendado

- 🐧 Linux/Termux: mosquitto -c mosquitto.conf -v
- 🪟 Windows: Baixe Mosquitto Installer com mosquitto_pub/mosquitto_sub
- 🛠 Configure arquivo mosquitto.conf com listener 1883, allow_anonymous false, etc.

---

## 📱 Testes com MQTT Explorer

- Broker: 192.168.X.Y (IP do PC ou celular)
- Porta: 1883
- Username/Password: conforme definido

---

## 📲 Publicação de comandos via terminal

```bash
mosquitto_pub -h 192.168.1.38 -t "/irrigacao/comando" -m "on"
mosquitto_pub -h 192.168.1.38 -t "/irrigacao/comando" -m "off"
mosquitto_pub -h 192.168.1.38 -t "/alerta/resetar" -m "reset"
```

---

## 👨‍💻 Autor

Projeto desenvolvido por Luiz Filipe Ribeiro de Jesus para o programa Embarcatech, com base no exemplo didático do Prof. Ricardo Prates.