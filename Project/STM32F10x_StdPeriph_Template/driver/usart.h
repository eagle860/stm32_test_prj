#ifndef USART_H
#define USART_H
#include "public.h"


#define RX_BUF_LEN      200
#define TX_BUF_LEN      200


extern uint8_t usart_rx_buf[RX_BUF_LEN];
extern uint8_t *usart_tx_buf;
extern uint16_t usart_rx_len;
extern uint16_t usart_tx_len;
extern bool flag_tx_complete;


void usart_init(void);
bool usart_send(uint8_t *buf, uint16_t len);


#endif //~USART_H
