#include "bsp_uart1.h"
#include "usart.h"

#include <stddef.h>
#include <string.h>
#include <limits.h>

#define BSP_UART1_HANDLE             huart1
#define BSP_UART1_TX_TIMEOUT_MS      100U

/*
 * HAL stores each received byte here.
 */
static uint8_t uart1RxByte;

/*
 * Application callback called from UART interrupt context.
 */
static BSP_UART1_RxCallback_t uart1RxCallback = NULL;


bool BSP_UART1_StartReceive(void)
{
    return HAL_UART_Receive_IT(
               &BSP_UART1_HANDLE,
               &uart1RxByte,
               1U
           ) == HAL_OK;
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


bool BSP_UART1_SendData(
    const uint8_t *data,
    uint16_t length
)
{
    if ((data == NULL) || (length == 0U))
    {
        return false;
    }

    return HAL_UART_Transmit(
               &BSP_UART1_HANDLE,
               (uint8_t *)data,
               length,
               BSP_UART1_TX_TIMEOUT_MS
           ) == HAL_OK;
}


void BSP_UART1_RegisterRxCallback(
    BSP_UART1_RxCallback_t callback
)
{
    uart1RxCallback = callback;
}


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART1)
    {
        /*
         * Send the received byte to the registered application callback.
         * This function executes in interrupt context.
         */
        if (uart1RxCallback != NULL)
        {
            uart1RxCallback(uart1RxByte);
        }

        /*
         * Arm UART1 reception for the next byte.
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
         * Clear UART overrun error.
         */
        __HAL_UART_CLEAR_OREFLAG(huart);

        /*
         * Restart reception after an error.
         */
        (void)HAL_UART_Receive_IT(
            &BSP_UART1_HANDLE,
            &uart1RxByte,
            1U
        );
    }
}
