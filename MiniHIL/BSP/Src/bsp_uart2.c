/*
 * bsp_uart2.c
 *
 *  Created on: Jul 18, 2026
 *      Author: Emagix
 */
#include "bsp_uart2.h"
#include "usart.h"

#include <stddef.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>

#define DEBUG_UART_HANDLE       huart2
#define DEBUG_UART_TIMEOUT_MS   100U
#define DEBUG_UART_BUFFER_SIZE       128U

bool Debug_UART_Send(const char *message)
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

    return HAL_UART_Transmit(
               &DEBUG_UART_HANDLE,
               (const uint8_t *)message,
               (uint16_t)length,
               DEBUG_UART_TIMEOUT_MS
           ) == HAL_OK;
}

bool Debug_UART_SendData(const uint8_t *data, uint16_t length)
{
    if ((data == NULL) || (length == 0U))
    {
        return false;
    }

    return HAL_UART_Transmit(
               &DEBUG_UART_HANDLE,
               data,
               length,
               DEBUG_UART_TIMEOUT_MS
           ) == HAL_OK;
}
bool Debug_UART_Printf(const char *fmt, ...)
{
    char buffer[DEBUG_UART_BUFFER_SIZE];
    va_list args;
    int length;

    if (fmt == NULL)
    {
        return false;
    }

    va_start(args, fmt);

    length = vsnprintf(
        buffer,
        sizeof(buffer),
        fmt,
        args
    );

    va_end(args);

    if (length < 0)
    {
        return false;
    }

    /*
     * vsnprintf returns the number of characters that would have been
     * written, even when the buffer is too small.
     */
    if ((size_t)length >= sizeof(buffer))
    {
        length = (int)(sizeof(buffer) - 1U);
    }

    return Debug_UART_SendData(
        (const uint8_t *)buffer,
        (uint16_t)length
    );
}


