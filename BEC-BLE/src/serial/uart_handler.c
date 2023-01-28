#include "serial_protocol.h"
#include "BEC_Element.h"

uint8_t rx_buf[UARTMSGSIZE];
static uint8_t rx_buf_pos;

struct k_sem uart_sem;
static const struct device *uart1;


static void uart_callback(const struct device *dev, void *user_data)
{
    uint8_t c;
    if (!uart_irq_update(dev)) {
        return;
    }

    while (uart_irq_rx_ready(dev)) {
		uart_fifo_read(dev, &c, sizeof(uint8_t));
        if (c == STOPCHAR && rx_buf_pos > 0) {
            k_sem_give(&uart_sem);
            rx_buf_pos = 0;// reset the buffer (it was copied to the msgq)
            return;
        } else if (rx_buf_pos < (sizeof(rx_buf) - 1)) {
            if (c != STARTCHAR)
            {
                rx_buf[rx_buf_pos++] = c;
            }
        }
    }
}

void initUART(void)
{
    uart1 = DEVICE_DT_GET(DT_NODELABEL(arduino_serial));

    if(!device_is_ready(uart1))
	{
		printk("UART1 device not ready\n");
        return;
	}
    uart_irq_callback_user_data_set(uart1, uart_callback, (void *)uart1);
	uart_irq_rx_enable(uart1);
    k_sem_init(&uart_sem, 0, 1);
}

void send_ack_uart(uint8_t opcode, uint16_t *uuid, size_t bufSize, uint8_t err)
{ // make uuid an array to send all conf nodes at once
    uint8_t tx_buf[bufSize*2];
    char tx_uuid[bufSize*3];

    uart_poll_out(uart1, STARTCHAR);    
    uart_poll_out(uart1, opcode);

    for (size_t i = 0, j = 0; i < bufSize; i++, j++)
    {
        tx_buf[j] = (uuid[i] & 0xff00) >> 8;
        hex2char(tx_buf[j], &tx_uuid[j]);
        uart_poll_out(uart1, tx_uuid[j]);
        j++;
        tx_buf[j] = uuid[i] & 0xff;
        hex2char(tx_buf[j], &tx_uuid[j]);
        uart_poll_out(uart1, tx_uuid[j]);
    }
    
	uart_poll_out(uart1, err);
    uart_poll_out(uart1, STOPCHAR);
}

int parseCommand(uint8_t *rxbuf)
{
    uint16_t addr = 0;
    printk("Uart payload: 0x%02x\n",rx_buf[OPCODE]);
    struct bt_mesh_loc_local loc;

    switch (rx_buf[OPCODE])
    {
    case IDLE:
        break;
    case DELETE_ASSET:
        addr = ((rx_buf[UUIDMSB] << 8) | rx_buf[UUIDLSB]);
        delete(addr);
        break;
    case IDENTIFY_ASSET:
        break;
    case GET_NODE:
        print_cdb();
        break;
    case GET_LOCATION:
        addr = ((rx_buf[UUIDMSB] << 8) | rx_buf[UUIDLSB]);
        get_location(addr);
        break;
    case SET_LOCATION:
        addr = ((rx_buf[UUIDMSB] << 8) | rx_buf[UUIDLSB]);
        loc.north = ((rx_buf[XPOSMSB] << 8) | rx_buf[XPOSLSB]); // fix ypos msb | lsb
        loc.east = ((rx_buf[YPOSMSB] << 8) | rx_buf[YPOSLSB]); // x too
        set_location(addr, loc);
        break;
    default:
        break;
    }

    return -1;
}