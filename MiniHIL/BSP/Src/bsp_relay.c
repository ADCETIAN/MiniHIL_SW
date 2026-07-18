/*
 * bsp_relay.c
 *
 *  Created on: Jul 18, 2026
 *      Author: Emagix
 */

#include "bsp_relay.h"
#include "main.h"
#include <stdint.h>

typedef struct
{
    GPIO_TypeDef *port;
    uint16_t pin;
    GPIO_PinState activeState;
} BSP_RelayConfig_t;

static const BSP_RelayConfig_t relayConfig[BSP_RELAY_COUNT] =
{
    [BSP_RELAY_1] =
    {
        .port = RELAY_1_GPIO_Port,
        .pin = RELAY_1_Pin,
        .activeState = GPIO_PIN_SET
    },

    [BSP_RELAY_2] =
    {
        .port = RELAY_2_GPIO_Port,
        .pin = RELAY_2_Pin,
        .activeState = GPIO_PIN_SET
    },

    [BSP_RELAY_3] =
    {
        .port = RELAY_3_GPIO_Port,
        .pin = RELAY_3_Pin,
        .activeState = GPIO_PIN_SET
    },

    [BSP_RELAY_4] =
    {
        .port = RELAY_4_GPIO_Port,
        .pin = RELAY_4_Pin,
        .activeState = GPIO_PIN_SET
    }
};

/*
 * false = relay OFF
 * true  = relay ON
 */
static bool relayState[BSP_RELAY_COUNT] =
{
    false,
    false,
    false,
    false
};

static bool BSP_Relay_IsValid(BSP_RelayId_t relay)
{
    return ((uint32_t)relay < (uint32_t)BSP_RELAY_COUNT);
}

static GPIO_PinState BSP_Relay_GetPinState(
    BSP_RelayId_t relay,
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

void BSP_Relay_Init(void)
{
    BSP_Relay_AllOff();
}

bool BSP_Relay_Set(BSP_RelayId_t relay, bool state)
{
    if (!BSP_Relay_IsValid(relay))
    {
        return false;
    }

    HAL_GPIO_WritePin(
        relayConfig[relay].port,
        relayConfig[relay].pin,
        BSP_Relay_GetPinState(relay, state)
    );

    relayState[relay] = state;

    return true;
}

bool BSP_Relay_On(BSP_RelayId_t relay)
{
    return BSP_Relay_Set(relay, true);
}

bool BSP_Relay_Off(BSP_RelayId_t relay)
{
    return BSP_Relay_Set(relay, false);
}

bool BSP_Relay_Toggle(BSP_RelayId_t relay)
{
    if (!BSP_Relay_IsValid(relay))
    {
        return false;
    }

    return BSP_Relay_Set(
        relay,
        !relayState[relay]
    );
}

bool BSP_Relay_GetState(BSP_RelayId_t relay)
{
    if (!BSP_Relay_IsValid(relay))
    {
        return false;
    }

    return relayState[relay];
}

bool BSP_Relay_GetPhysicalState(BSP_RelayId_t relay)
{
    bool pinIsHigh;

    if (!BSP_Relay_IsValid(relay))
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

void BSP_Relay_AllOff(void)
{
    for (uint32_t index = 0U;
         index < (uint32_t)BSP_RELAY_COUNT;
         index++)
    {
        (void)BSP_Relay_Off((BSP_RelayId_t)index);
    }
}
