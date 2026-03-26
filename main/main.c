#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h" // Fila para comunicação entre ISR e Task
#include "driver/gpio.h"
#include "esp_log.h"

#define LED_PIN    2
#define BUTTON_PIN 0  // O botão 'BOOT' do ESP32 geralmente está no GPIO 0
static const char *TAG = "INTERRUPT_LAB";

// Fila para enviar o evento do botão da interrupção para a Task
static QueueHandle_t gpio_evt_queue = NULL;

// Função que roda quando o botão é apertado (Interrupção - ISR)
// Deve ser rápida e não pode usar printf/vTaskDelay
static void IRAM_ATTR gpio_isr_handler(void* arg) {
    uint32_t gpio_num = (uint32_t) arg;
    xQueueSendFromISR(gpio_evt_queue, &gpio_num, NULL);
}

// Task que espera o evento da fila para agir
void button_task(void *pvParameter) {
    uint32_t io_num;
    bool led_state = false;
    while(1) {
        // Fica bloqueado aqui sem gastar CPU até chegar algo na fila
        if(xQueueReceive(gpio_evt_queue, &io_num, portMAX_DELAY)) {
            led_state = !led_state; // Inverte o estado
            gpio_set_level(LED_PIN, led_state);
            ESP_LOGI(TAG, "Botão GPIO[%ld] apertado! LED: %s", io_num, led_state ? "ON" : "OFF");
        }
    }
}

void app_main(void) {
    // 1. Configurar LED como saída
    gpio_reset_pin(LED_PIN);
    gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT);

    // 2. Configurar Botão com Interrupção na descida (Aperto)
    gpio_reset_pin(BUTTON_PIN);
    gpio_set_direction(BUTTON_PIN, GPIO_MODE_INPUT);
    gpio_set_intr_type(BUTTON_PIN, GPIO_INTR_NEGEDGE); // Gatilho na borda de descida

    // 3. Criar a fila e a Task
    gpio_evt_queue = xQueueCreate(10, sizeof(uint32_t));
    xTaskCreate(button_task, "button_task", 2048, NULL, 10, NULL);

    // 4. Instalar o serviço de interrupção e adicionar o handler
    gpio_install_isr_service(0);
    gpio_add_isr_handler(BUTTON_PIN, gpio_isr_handler, (void*) BUTTON_PIN);

    ESP_LOGI(TAG, "Sistema Pronto. Aperte o botão BOOT!");
}
