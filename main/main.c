#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
<<<<<<< HEAD
#include "freertos/queue.h"
=======
#include "freertos/queue.h"    // Adicionado explicitamente para evitar erros de tipo
>>>>>>> cfcc3d8 (feat: setup interrupt lab with FreeRTOS queue for ESP-IDF v6.0)
#include "driver/gpio.h"
#include "esp_driver_gpio.h"  // <--- ESSA LINHA É A CHAVE PARA O IDF v6.0
#include "esp_log.h"

#define LED_PIN    2
#define BUTTON_PIN 0  
static const char *TAG = "INTERRUPT_LAB";

static QueueHandle_t gpio_evt_queue = NULL;

// ISR: O mais rápida possível
static void IRAM_ATTR gpio_isr_handler(void* arg) {
    uint32_t gpio_num = (uint32_t) arg;
    xQueueSendFromISR(gpio_evt_queue, &gpio_num, NULL);
}

void button_task(void *pvParameter) {
    uint32_t io_num;
    bool led_state = false;
    TickType_t last_button_time = 0; // Para o debounce manual

    while(1) {
        if(xQueueReceive(gpio_evt_queue, &io_num, portMAX_DELAY)) {
            // DEBOUNCE: Ignora sinais num intervalo menor que 250ms
            TickType_t current_time = xTaskGetTickCount();
            if (current_time - last_button_time > pdMS_TO_TICKS(250)) {
                led_state = !led_state;
                gpio_set_level(LED_PIN, led_state);
                
                // %lu é o padrão correto para uint32_t no compilador do ESP32
                ESP_LOGI(TAG, "Botao GPIO[%lu] apertado! LED: %s", io_num, led_state ? "ON" : "OFF");
                
                last_button_time = current_time;
            }
        }
    }
}

void app_main(void) {
    // 1. Configurar LED
    gpio_reset_pin(LED_PIN);
    gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT);

    // 2. Configurar Botão (Importante: Habilitar Pull-up interno se o botão for para o GND)
    gpio_reset_pin(BUTTON_PIN);
    gpio_set_direction(BUTTON_PIN, GPIO_MODE_INPUT);
    gpio_set_pull_mode(BUTTON_PIN, GPIO_PULLUP_ONLY); // Garante nível HIGH quando solto
    gpio_set_intr_type(BUTTON_PIN, GPIO_INTR_NEGEDGE); 

    // 3. Recursos do FreeRTOS
    gpio_evt_queue = xQueueCreate(10, sizeof(uint32_t));
    xTaskCreate(button_task, "button_task", 2048, NULL, 10, NULL);

    // 4. Instalar ISR
    gpio_install_isr_service(0);
    gpio_isr_handler_add(BUTTON_PIN, gpio_isr_handler, (void*) BUTTON_PIN);

    ESP_LOGI(TAG, "Sistema v6.0 Pronto. Aperte o botao BOOT (GPIO 0)!");
}