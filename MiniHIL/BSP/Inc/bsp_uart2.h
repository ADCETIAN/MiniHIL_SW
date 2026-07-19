/*
 * bsp_uart2.h
 *
 *  Created on: Jul 18, 2026
 *      Author: Emagix
 */

#ifndef INC_BSP_UART2_H_
#define INC_BSP_UART2_H_


#include <stdbool.h>
#include <stdint.h>

bool Debug_UART_Send(const char *message);
bool Debug_UART_SendData(const uint8_t *data, uint16_t length);
bool Debug_UART_Printf(const char *fmt, ...);


#endif /* INC_BSP_UART2_H_ */
