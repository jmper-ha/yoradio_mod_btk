#ifndef __UART_APP_H__
#define __UART_APP_H__

#if (UART_TX!=255 && UART_RX!=255)
#define ISUART
#endif

enum {
    APP_UART_IDLE,
    APP_UART_EIR_DATA,
    APP_UART_BT_DISCONNECTED,
    APP_UART_BT_CONNECTED,
    APP_UART_BATTERY,
};

#define APP_UART_GET_STATUS "1"

void uart_init(void);
int uart_send_data(const char* data);
char* get_uart_rx_data(void);

#endif /* __UART_APP_H__ */