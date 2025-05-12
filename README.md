# 🌱 AgroSmart - Monitoramento de Umidade do Solo com Webserver (Pico W)

Este projeto foi desenvolvido como parte do tópico **Comunicação em IoT - 37M3SE** (Residência Embarcatech), orientado pelo Prof. Dr. Ricardo Menezes Prates e pelo Prof. Auerê Vasconcelos Veras.

## 📌 Objetivo

Desenvolver um sistema de monitoramento de umidade do solo utilizando o **Raspberry Pi Pico W** com o módulo Wi-Fi **CYW43439**, capaz de exibir os dados via Webserver e emitir alertas visuais e sonoros quando a umidade estiver abaixo de um limite crítico.

## 🧠 Funcionalidades Implementadas

- ✅ Leitura de umidade simulada via **potenciômetro do joystick (GPIO26 / ADC0)**.
- ✅ Exibição do valor de umidade atual em uma **interface HTML interativa**.
- ✅ Geração de **alerta visual (LED vermelho)** e **alarme sonoro (buzzer)** em caso de baixa umidade.
- ✅ Comportamento do buzzer configurado como **alarme com bipes** por 10 segundos.
- ✅ Botão “Resetar Alerta” na interface Web para desativar os alarmes.
- ✅ Código em linguagem **C puro**, com uso do **Pico SDK** e **LwIP TCP/IP stack**.

## 🌐 Interface Web

A página HTML é servida localmente e apresenta:

- Valor atual da umidade (%)
- Mensagem de status (`Nível dentro do ideal` ou `Alerta: Umidade muito baixa!`)
- Botão para resetar o alerta

## 💻 Tecnologias Utilizadas

- Raspberry Pi Pico W (RP2040 + CYW43439)
- ADC do RP2040
- Stack LwIP (protocolo TCP/IP)
- GPIOs, buzzer, LED
- Servidor TCP simples na porta 80
- HTML embutido no firmware

## 🛠️ Requisitos

- [Pico SDK](https://github.com/raspberrypi/pico-sdk) corretamente configurado
- Ferramentas de build: `cmake`, `ninja` ou `make`
- Editor: VS Code com extensão CMake Tools (recomendado)
- Exclusões no antivírus para permitir compilação de `.elf` e `.uf2`