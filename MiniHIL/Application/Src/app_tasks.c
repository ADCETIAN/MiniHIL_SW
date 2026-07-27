#include "app_tasks.h"
#include "app_config.h"
#include "bsp_led.h"
#include "bsp_relay.h"
#include "bsp_gpo.h"
#include "bsp_dcrelay.h"
#include "cmsis_os2.h"
#include "bsp_uart2.h"
#include "bsp_uart1.h"
//#include "usart.h"
#include <string.h>
#include <stdio.h>
#include "hil_protocol.h"

/* Queue handle — declared by CubeMX in freertos.c, we extern it */
uint8_t uart1RxByte;
extern osMessageQueueId_t uart1RxQueueHandle;

static void APP_UART1_RxCallback(uint8_t byte)
{
    osStatus_t status;

    if (uart1RxQueueHandle == NULL)
    {
        return;
    }

    /*
     * This function executes from UART interrupt context.
     *
     * Timeout must always be zero when osMessageQueuePut()
     * is called from an interrupt.
     */
    status = osMessageQueuePut(
        uart1RxQueueHandle,
        &byte,
        0U,
        0U
    );

    if (status != osOK)
    {
        /*
         * Queue is probably full.
         * Do not print or block inside the interrupt.
         * An overflow counter can be added later.
         */
    }
}

/* ── Protocol task ──────────────────────────────────────────────
   Blocks on queue — zero CPU usage while waiting.
   Wakes instantly when ISR posts a byte.
──────────────────────────────────────────────────────────────── */
void vProtocolTask(void *pvParameters)
{
    const char *uart1Message = "UART1 working\r\n";
    uint8_t receivedByte;
    HIL_Packet_t receivedPacket;

    (void)pvParameters;
    HIL_Protocol_Init();

    Debug_UART_Send("UART1 task started\r\n");

    /*
     * Register callback before starting UART reception.
     */
    BSP_UART1_RegisterRxCallback(APP_UART1_RxCallback);

    /*
     * Start UART1 reception only once.
     */
    if (BSP_UART1_StartReceive() == false)
    {
        Debug_UART_Send("UART1 RX interrupt start failed\r\n");
    }
    else
    {
        Debug_UART_Send("UART1 RX interrupt started\r\n");
    }

    /*
     * Test UART1 transmission.
     */
    if (BSP_UART1_Send(uart1Message) == false)
    {
        Debug_UART_Send("UART1 TX failed\r\n");
    }

    for (;;)
    {
        if (osMessageQueueGet(
                uart1RxQueueHandle,
                &receivedByte,
                NULL,
                osWaitForever
            ) == osOK)
        {
            if (HIL_Protocol_ProcessByte(
                    receivedByte,
                    &receivedPacket
                ))
            {
                Debug_UART_Printf(
                    "RX CMD=0x%02X LEN=%u\r\n",
                    receivedPacket.cmd,
                    receivedPacket.len
                );

                HIL_Protocol_Execute(
                    &receivedPacket
                );
            }
        }
    }
}


void AppHeartbeatTask(void *argument)
{
    (void)argument;
    //uint8_t receivedByte;
    //osStatus_t status;

    BSP_LED_Init();
    BSP_Relay_Init();
    BSP_GPO_Init();
    BSP_DC_Relay_Init();
    //int adcValue = 10;
    Debug_UART_Send("Heartbeat task started\r\n");
    for (;;)
    {
    	osDelay(1);
    	/*BSP_LED_Toggle(BSP_LED_STATUS);
        BSP_LED_Toggle(BSP_LED_ACTIVITY);
        BSP_Relay_Toggle(BSP_RELAY_2);
        BSP_Relay_Toggle(BSP_RELAY_4);*/

        /*status = osMessageQueueGet(
            uart1RxQueueHandle,
            &receivedByte,
            NULL,
            osWaitForever
        );

        if (status == osOK)
        {
            Debug_UART_SendData(
                &receivedByte,
                1U
            );

            BSP_LED_Toggle(BSP_LED_ACTIVITY);
        }*/
    }
}

