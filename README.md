# Hello ESP32 - FreeRTOS Blinky

Primeiro projeto profissional de firmware utilizando o **ESP32 DevKitV1** e o framework **ESP-IDF v6.0**.

## 🛠️ Tecnologias e Ferramentas
- **Linguagem:** C
- **RTOS:** FreeRTOS (Multitarefa)
- **Framework:** ESP-IDF v6.0
- **Hardware:** ESP32 (Dual Core Xtensa)

## 📋 Descrição do Projeto
Este projeto demonstra o uso de **Tasks** do FreeRTOS para controlar periféricos de hardware (GPIO). Ao contrário do modelo de loop do Arduino, este código utiliza um escalonador de tarefas, permitindo que a CPU entre em estados de baixo consumo enquanto aguarda o tempo de delay.

## 🚀 Como Compilar
1. Configure o ambiente do ESP-IDF.
2. Selecione o target: `idf.py set-target esp32`.
3. Compile e grave: `idf.py build flash monitor`.
