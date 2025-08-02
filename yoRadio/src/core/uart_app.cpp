
#include "driver/uart.h"
#include "driver/gpio.h"
#include "string.h"
#include "uart_app.h"
#include "options.h"
#include "display.h"

extern void uart_sent_net();
static void uart_hdl_evt(char* d);
static const int RX_BUF_SIZE = 256;

#define BAUD_RATE   115200

char* data;

static void rx_task(void *arg);

void uart_init(void)
{
    const uart_config_t uart_config = {
        .baud_rate  = BAUD_RATE,
        .data_bits  = UART_DATA_8_BITS,
        .parity     = UART_PARITY_DISABLE,
        .stop_bits  = UART_STOP_BITS_1,
        .flow_ctrl  = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_DEFAULT,
    };
    uart_driver_install(UART_NUM_1, RX_BUF_SIZE * 2, 0, 0, NULL, 0);
    uart_param_config(UART_NUM_1, &uart_config);
    uart_set_pin(UART_NUM_1, UART_TX, UART_RX, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
    xTaskCreate(rx_task, "uart_rx_task", 3072, NULL, configMAX_PRIORITIES - 1, NULL);
}

int uart_send_data(const char* data)
{
    const int len = strlen(data);
    const int txBytes = uart_write_bytes(UART_NUM_1, data, len);
    return txBytes;
}

char* get_uart_rx_data(){
    return data;
}

static void rx_task(void *arg)
{
    uint8_t* uart_rx_data = (uint8_t*) malloc(RX_BUF_SIZE + 1);
    while (1) {
        const int rxBytes = uart_read_bytes(UART_NUM_1, uart_rx_data, RX_BUF_SIZE,150 / portTICK_PERIOD_MS);// portMAX_DELAY);//
        if (rxBytes > 0) {
            uart_rx_data[rxBytes] = 0;
            uart_hdl_evt((char*)uart_rx_data);
        }
    }
    free(uart_rx_data);
}

static void uart_hdl_evt(char* d){
    uint8_t comm = (uint8_t)strtol((char*)d, &data, 10);
    data++;

    switch (comm)
    {
    case APP_UART_EIR_DATA:
        uart_sent_net();
        break;
    case APP_UART_BT_CONNECTED:
    case APP_UART_BT_DISCONNECTED:
        display.putRequest(SHOWBT, APP_UART_BT_CONNECTED - comm);
        break;
    case APP_UART_BATTERY:{
        uint16_t volts = (uint16_t)strtol(data, &data, 10);
        data++;
        uint16_t prt = (uint16_t)strtol(data, &data, 10);
    #ifdef BATTERY_SHOW_ON
        display.putRequest(BATTERY,prt);
//        Serial.printf("Volts: %3.2f Persent: %d%%\n",(float)volts/1000,prt);
    #endif
    }
        break;
    default:
        break;
    }

}