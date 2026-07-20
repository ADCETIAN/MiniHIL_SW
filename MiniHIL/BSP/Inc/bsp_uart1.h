/*
 * bsp_uart1.h
 *
 *  Created on: 19-Jul-2026
 *      Author: Emagix
 */

#ifndef INC_BSP_UART1_H_
#define INC_BSP_UART1_H_

#include <stdbool.h>
#include <stdint.h>

/*
 * Callback function type.
 * This callback will execute when one UART1 byte is received.
 */
typedef void (*BSP_UART1_RxCallback_t)(uint8_t byte);

/*
 * Start UART1 interrupt reception.
 * UART1 receives one byte at a time.
 */
bool BSP_UART1_StartReceive(void);

/*
 * Send a null-terminated string through UART1.
 */
bool BSP_UART1_Send(const char *message);

/*
 * Send raw data through UART1.
 */
bool BSP_UART1_SendData(const uint8_t *data, uint16_t length);

/*
 * Register application receive callback.
 */
void BSP_UART1_RegisterRxCallback(BSP_UART1_RxCallback_t callback);

#endif /* INC_BSP_UART1_H_ */
