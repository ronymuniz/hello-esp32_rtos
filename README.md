# Hello ESP32 RTOS - Interrupt & Queues

Este projeto demonstra o uso de interrupções de hardware (GPIO) e filas (Queues) do FreeRTOS para controle de um LED.

## 🛠️ Especificações Técnicas
* **Chip:** ESP32-D0WD-V3 (Revision v3.1)
* **Arquitetura:** Dual Core + LP Core (240MHz)
* **Recursos:** Wi-Fi, Bluetooth, 40MHz Crystal
* **Framework:** ESP-IDF v6.0 (Padrão C gnu23)

## 🚀 Funcionalidade
O código configura o pino GPIO 0 (botão BOOT) para gerar uma interrupção na borda de descida. 
1. A **ISR** (Interrupt Service Routine) captura o evento.
2. O evento é enviado para uma **FreeRTOS Queue**.
3. Uma **Task** dedicada consome a fila e inverte o estado do LED (GPIO 2).

## 🔧 Como compilar
```zsh
idf.py build
idf.py -p /dev/cu.usbserial-0001 flash monitor