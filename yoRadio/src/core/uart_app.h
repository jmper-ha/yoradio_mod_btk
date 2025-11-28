#ifndef __UART_APP_H__
#define __UART_APP_H__

enum {
    APP_UART_IDLE,
    APP_UART_EIR_DATA,
    APP_UART_BT_STATUS,
    APP_UART_DRV_READY,
    APP_UART_BATTERY,
    APP_UART_ASK_MODE,
    APP_UART_BT_TITLE,
    APP_UART_BT_NUM_CFM,
    APP_UART_BT_NAME,
    APP_UART_BT_A2DP,
    APP_UART_VOLUME,
    APP_UART_PLAYBACK,
    APP_UART_BT_VOLUM,
    APP_UART_PT_CMD,
    APP_UART_ART_IMG,
    APP_UART_ART_IMG_DATA,
};
enum {
    APP_UART_SEND_IDLE,
    APP_UART_SEND_GET_STATUS,
    APP_UART_SEND_START_DISCOVERY,
    APP_UART_SEND_STOP_DISCOVERY,
    APP_UART_SEND_CONNECT,
    APP_UART_SEND_DISCONNECT,
    APP_UART_SEND_FORGET_SINK,
    APP_UART_SEND_PIN_SINK,
    APP_UART_SEND_I2S_SAMPLE_RATE,
    APP_UART_SEND_GET_INIT,
    APP_UART_SEND_REINIT_CMD,
    APP_UART_SEND_VOLUME,
    APP_UART_SEND_PT_CMD,
    APP_UART_SEND_ONLY_FORGET,
};
enum {
    APP_MODE_IDLE,
    APP_MODE_SOURCE,
    APP_MODE_SINK,
};
enum {
    ESP_AVRC_PT_CMD_PLAY           =   0x44,    /*!< play */
    ESP_AVRC_PT_CMD_STOP           =   0x45,    /*!< stop */
    ESP_AVRC_PT_CMD_PAUSE          =   0x46,    /*!< pause */
    ESP_AVRC_PT_CMD_FORWARD        =   0x4B,    /*!< forward */
    ESP_AVRC_PT_CMD_BACKWARD       =   0x4C,    /*!< backward */
}pt_cmd_t;

void uart_init(void);
extern int uart_send_data(const char* data);
extern void uart_send_param(u_int8_t param, uint16_t data);
export void uart_send_param_init(uint16_t data,char* name);

#endif /* __UART_APP_H__ */