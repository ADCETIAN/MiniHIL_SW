#include "app_tasks.h"
#include "app_config.h"
#include "bsp_led.h"
#include "bsp_relay.h"
#include "cmsis_os.h"
#include "bsp_uart2.h"



void AppHeartbeatTask(void *argument)
{
    (void)argument;

    BSP_LED_Init();
    int adcValue = 10;

    for (;;)
    {
        BSP_LED_Toggle(BSP_LED_STATUS);
        BSP_LED_Toggle(BSP_LED_ACTIVITY);
        /*BSP_Relay_Toggle(BSP_RELAY_2);
        BSP_Relay_Toggle(BSP_RELAY_4);*/
        Debug_UART_Send("Heartbeat task started\r\n");
        Debug_UART_Printf("ADC = %d\r\n", adcValue);

        osDelay(APP_HEARTBEAT_PERIOD_MS);
    }
}
