#ifndef BEC_UART_CTRL_PROTOCOL
#define BEC_UART_CTRL_PROTOCOL
#include <zephyr/device.h>
#include <zephyr/drivers/uart.h>

// commands
enum {
    IDLE,
    DELETE_ASSET,
    IDENTIFY_ASSET,
    GET_NODE,
    GET_LOCATION,
    SET_LOCATION
};

// Msg Positon
enum {
    OPCODE,
    UUIDMSB,
    UUIDLSB,
    XPOSMSB,
    XPOSLSB,
    YPOSMSB,
    YPOSLSB,
};

#define UARTMSGSIZE 16
#define STOPCHAR 254
#define STARTCHAR 253

extern uint8_t rx_buf[];
extern struct k_sem uart_sem;
/* Desc:    Initiates UART peripherals and semaphore used for handling. */
void initUART(void);

/* Desc:    Detects command received by uart and reacts accordingly. 
   Input:   rxbuf - buffer when uart data is received
   Output:  Error code  */
int parseCommand(uint8_t *rxbuf);

void send_ack_uart(uint8_t opcode, uint16_t *uuid, size_t bufSize, uint8_t err);

#endif