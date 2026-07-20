#ifndef INC_BSP_UART1_H_
#define INC_BSP_UART1_H_

#include <stdbool.h>
#include <stdint.h>

/*
 * Callback executed whenever UART1 receives one byte.
 */
typedef void (*BSP_UART1_RxCallback_t)(uint8_t byte);

/*
 * Start UART1 interrupt reception.
 */
bool BSP_UART1_StartReceive(void);

/*
 * Send a null-terminated string using UART1.
 */
bool BSP_UART1_Send(const char *message);

/*
 * Send raw bytes using UART1.
 */
bool BSP_UART1_SendData(
    const uint8_t *data,
    uint16_t length
);

/*
 * Register application callback for received bytes.
 */
void BSP_UART1_RegisterRxCallback(
    BSP_UART1_RxCallback_t callback
);

#endif /* INC_BSP_UART1_H_ */
