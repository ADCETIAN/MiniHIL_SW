/*
 * bsp_uart1.c
 *
 *  Created on: 19-Jul-2026
 *      Author: Emagix
 */

#include "bsp_uart1.h"
#include "usart.h"

#include <stddef.h>
#include <string.h>
#include <limits.h>

#define BSP_UART1_HANDLE             huart1
#define BSP_UART1_TX_TIMEOUT_MS      100U

/*
 * HAL_UART_Receive_IT() receives one byte into this variable.
 */
static uint8_t uart1RxByte;

/*
 * Application receive callback.
 */
static BSP_UART1_RxCallback_t uart1RxCallback = NULL;


bool BSP_UART1_StartReceive(void)
{
    HAL_StatusTypeDef status;

    status = HAL_UART_Receive_IT(
        &BSP_UART1_HANDLE,
        &uart1RxByte,
        1U
    );

    return (status == HAL_OK);
}


bool BSP_UART1_Send(const char *message)
{
    size_t length;

    if (message == NULL)
    {
        return false;
    }

    length = strlen(message);

    if (length == 0U)
    {
        return true;
    }

    if (length > UINT16_MAX)
    {
        return false;
    }

    return BSP_UART1_SendData(
        (const uint8_t *)message,
        (uint16_t)length
    );
}


bool BSP_UART1_SendData(const uint8_t *data, uint16_t length)
{
    HAL_StatusTypeDef status;

    if ((data == NULL) || (length == 0U))
    {
        return false;
    }

    status = HAL_UART_Transmit(
        &BSP_UART1_HANDLE,
        (uint8_t *)data,
        length,
        BSP_UART1_TX_TIMEOUT_MS
    );

    return (status == HAL_OK);
}


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART1)
    {
        /*
         * Inform application about the received byte.
         */
        if (uart1RxCallback != NULL)
        {
            uart1RxCallback(uart1RxByte);
        }

        /*
         * Restart UART reception for the next byte.
         */
        (void)HAL_UART_Receive_IT(
            &BSP_UART1_HANDLE,
            &uart1RxByte,
            1U
        );
    }
}
void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART1)
    {
        /*
         * Clear UART overrun condition.
         */
        __HAL_UART_CLEAR_OREFLAG(huart);

        /*
         * Restart reception.
         */
        (void)HAL_UART_Receive_IT(
            &BSP_UART1_HANDLE,
            &uart1RxByte,
            1U
        );
    }
}
