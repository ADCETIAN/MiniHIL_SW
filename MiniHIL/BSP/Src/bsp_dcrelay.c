/*
 * bsp_dcrelay.c
 *
 *  Created on: 26-Jul-2026
 *      Author: Emagix
 */


/*
 * bsp_dcrelay.c
 *
 *  Created on: 26-Jul-2026
 *      Author: Emagix
 */

#include "bsp_dcrelay.h"
#include "main.h"
#include <stdint.h>

typedef struct
{
    GPIO_TypeDef *port;
    uint16_t pin;
    GPIO_PinState activeState;
} BSP_DC_RelayConfig_t;

static const BSP_DC_RelayConfig_t relayConfig[BSP_DC_RELAY_COUNT] =
{
    [BSP_DC_RELAY_1] =
    {
        .port = DC_Relay_1_GPIO_Port,
        .pin = DC_Relay_1_Pin,
        .activeState = GPIO_PIN_SET
    },

    [BSP_DC_RELAY_2] =
    {
        .port = DC_Relay_2_GPIO_Port,
        .pin = DC_Relay_2_Pin,
        .activeState = GPIO_PIN_SET
    }
};

/*
 * false = relay OFF
 * true  = relay ON
 */
static bool DC_relayState[BSP_DC_RELAY_COUNT] =
{
    false,
    false,
    false,
    false
};

static bool BSP_DC_Relay_IsValid(BSP_DC_RelayId_t relay)
{
    return ((uint32_t)relay < (uint32_t)BSP_DC_RELAY_COUNT);
}

static GPIO_PinState BSP_DC_Relay_GetPinState(
    BSP_DC_RelayId_t relay,
    bool requestedState)
{
    if (requestedState)
    {
        return relayConfig[relay].activeState;
    }

    return (relayConfig[relay].activeState == GPIO_PIN_SET)
               ? GPIO_PIN_RESET
               : GPIO_PIN_SET;
}

void BSP_DC_Relay_Init(void)
{
    BSP_DC_Relay_AllOff();
}

bool BSP_DC_Relay_Set(BSP_DC_RelayId_t relay, bool state)
{
    if (!BSP_DC_Relay_IsValid(relay))
    {
        return false;
    }

    HAL_GPIO_WritePin(
        relayConfig[relay].port,
        relayConfig[relay].pin,
        BSP_DC_Relay_GetPinState(relay, state)
    );

    DC_relayState[relay] = state;

    return true;
}

bool BSP_DC_Relay_On(BSP_DC_RelayId_t relay)
{
    return BSP_DC_Relay_Set(relay, true);
}

bool BSP_DC_Relay_Off(BSP_DC_RelayId_t relay)
{
    return BSP_DC_Relay_Set(relay, false);
}

bool BSP_DC_Relay_Toggle(BSP_DC_RelayId_t relay)
{
    if (!BSP_DC_Relay_IsValid(relay))
    {
        return false;
    }

    return BSP_DC_Relay_Set(
        relay,
        !DC_relayState[relay]
    );
}

bool BSP_DC_Relay_GetState(BSP_DC_RelayId_t relay)
{
    if (!BSP_DC_Relay_IsValid(relay))
    {
        return false;
    }

    return DC_relayState[relay];
}

bool BSP_DC_Relay_GetPhysicalState(BSP_DC_RelayId_t relay)
{
    bool pinIsHigh;

    if (!BSP_DC_Relay_IsValid(relay))
    {
        return false;
    }

    pinIsHigh =
        ((relayConfig[relay].port->ODR &
          relayConfig[relay].pin) != 0U);

    if (relayConfig[relay].activeState == GPIO_PIN_SET)
    {
        return pinIsHigh;
    }

    return !pinIsHigh;
}

void BSP_DC_Relay_AllOff(void)
{
    for (uint32_t index = 0U;
         index < (uint32_t)BSP_DC_RELAY_COUNT;
         index++)
    {
        (void)BSP_DC_Relay_Off((BSP_DC_RelayId_t)index);
    }
}
