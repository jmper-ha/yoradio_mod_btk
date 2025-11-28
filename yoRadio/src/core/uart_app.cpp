
#include "driver/uart.h"
#include "driver/gpio.h"
#include "string.h"
#include "uart_app.h"
#include "options.h"
#include "Arduino.h"
#include "display.h"
#include "config.h"
#include "player.h"
#include "netserver.h"


//extern void uart_sent_net(char* d);
static void uart_hdl_evt(char* d,int data_len);
static const int RX_BUF_SIZE = 256*2;

#define BAUD_RATE   460800 //115200 //230400 //

char* uart_data;

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
    xTaskCreate(rx_task, "uart_rx_task", 512*5, NULL, configMAX_PRIORITIES - 1, NULL);
}

int uart_send_data(const char* data){
    const int len = strlen(data);
    Serial.printf("***UART Sent data: %s len: %d\n",data,len);
    const int txBytes = uart_write_bytes(UART_NUM_1, data, len);
    return txBytes;
}

void uart_send_param(u_int8_t param, uint16_t data){
    char* buff = (char*)malloc(255);
    sprintf(buff,"%d:%d",param,data);
    uart_send_data(buff);
    free(buff);
}

void uart_send_param_init(uint16_t data,char* name){
    char* buff = (char*)malloc(255);
    sprintf(buff,"%d:%d:%s\n",APP_UART_SEND_GET_INIT,data,name);
    uart_send_data(buff);
    free(buff);
}

static void rx_task(void *arg)
{
    uint8_t* uart_rx_data = (uint8_t*) malloc(RX_BUF_SIZE + 1);
    while (1) {
        const int rxBytes = uart_read_bytes(UART_NUM_1, uart_rx_data, RX_BUF_SIZE,150 / portTICK_PERIOD_MS);// portMAX_DELAY);//
        if (rxBytes > 0) {
            uart_rx_data[rxBytes] = 0;
            Serial.printf("***UART Reseive data: %s len: %d\n",uart_rx_data,rxBytes);
            uart_hdl_evt((char*)uart_rx_data,rxBytes);
        }
    }
    free(uart_rx_data);
}

static void uart_hdl_evt(char* d,int data_len){
    uint16_t volts;
    uint16_t prt;
    bool connect;
    uint8_t comm = (uint8_t)strtol((char*)d, &uart_data, 10);
    uart_data++;
    switch (comm){
    case APP_UART_EIR_DATA: netserver.uart_sent_net(uart_data); break;
    case APP_UART_BT_STATUS: 
        config.bt_spkr_connected = (bool)strtol(uart_data, &uart_data, 10);
        display.putRequest(SHOWBT, (uint16_t)config.bt_spkr_connected); 
        if(config.bt_spkr_connected) uart_send_param(APP_UART_SEND_VOLUME,config.store.volume);
        else config.bt_volume_control = false;
        break;
    case APP_UART_DRV_READY: config.reinit(); break;
    case APP_UART_BATTERY:
        volts = (uint16_t)strtol(uart_data, &uart_data, 10);
        uart_data++;
        prt = (uint16_t)strtol(uart_data, &uart_data, 10);
#if defined BATTERY_SHOW_ON && defined ISUART
        display.putRequest(BATTERY,prt);
#endif
        break;
    case APP_UART_ASK_MODE: config.get_init(); break;
    case APP_UART_BT_TITLE:
        uart_data[strlen(uart_data)-1] = 0;
        if(strlen(uart_data)>1) config.setStation(uart_data);
        else config.setStation(const_BTSink);
        break;
    case APP_UART_BT_NAME:
        uart_data[strlen(uart_data)-1] = 0;
        config.setTitle(uart_data);
        break;
    case APP_UART_BT_NUM_CFM: display.putRequest(SHOWFCFMNUM,(uint32_t)strtol(uart_data, &uart_data, 10)); break;
    case APP_UART_BT_A2DP:
        connect = (bool)strtol(uart_data, &uart_data, 10);
        player.setVU_Only();
        player.set_run(connect);
        display.putRequest(connect?PSTART:PSTOP);
        if((config.getMode()==PM_BTSINK)&&(connect)){
            uart_send_param(APP_UART_SEND_VOLUME,config.store.volume);
            config.setStation(const_BTSink);
            config.station.bitrate=441;
            config.setBitrateFormat(BF_SBC);
            display.putRequest(DBITRATE);
            display.putRequest(NEWSTATION);            
        }
        break;
    case APP_UART_VOLUME: player.setVolremote((uint8_t)strtol(uart_data, &uart_data, 10)); break;
    case APP_UART_PLAYBACK: player.set_status(strtol(uart_data, &uart_data, 10)); break;
    case APP_UART_BT_VOLUM: config.bt_volume_control = true; break;
    case APP_UART_PT_CMD:
        switch ((uint16_t)strtol(uart_data, &uart_data, 10)){
        case ESP_AVRC_PT_CMD_PLAY:
        case ESP_AVRC_PT_CMD_PAUSE: player.toggle();  break;
        case ESP_AVRC_PT_CMD_FORWARD: player.next();  break;
        case ESP_AVRC_PT_CMD_BACKWARD: player.prev(); break;        
        default: break;
        }
        break;
    case APP_UART_ART_IMG:
        netserver.set_art_image(strtol(uart_data, &uart_data, 10));
        break;
    case APP_UART_ART_IMG_DATA:

        break;
    default:
//        uart_flush_input(UART_NUM_1);
        break;
    }
}