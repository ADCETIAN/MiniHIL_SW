#include "bsp_led.h"
#include "main.h"

typedef struct
{
    GPIO_TypeDef *port;
    uint16_t pin;
    GPIO_PinState activeState;
} BSP_LED_Config_t;

static const BSP_LED_Config_t ledConfig[BSP_LED_COUNT] =
{
    [BSP_LED_STATUS] =
    {
        .port = LED_1_GPIO_Port,
        .pin = LED_1_Pin,
        .activeState = GPIO_PIN_SET
    },

    [BSP_LED_ACTIVITY] =
    {
        .port = LED_2_GPIO_Port,
        .pin = LED_2_Pin,
        .activeState = GPIO_PIN_SET
    }
};

/*
 * Software state:
 * false = LED OFF
 * true  = LED ON
 */
static bool ledState[BSP_LED_COUNT] =
{
    false,
    false
};

static bool BSP_LED_IsValid(BSP_LED_Id_t led)
{
    return ((uint32_t)led < (uint32_t)BSP_LED_COUNT);
}

static GPIO_PinState BSP_LED_GetOutputState(
    BSP_LED_Id_t led,
    bool requestedState)
{
    if (requestedState)
    {
        return ledConfig[led].activeState;
    }

    return (ledConfig[led].activeState == GPIO_PIN_SET)
               ? GPIO_PIN_RESET
               : GPIO_PIN_SET;
}

void BSP_LED_Init(void)
{
    for (uint32_t index = 0U;
         index < (uint32_t)BSP_LED_COUNT;
         index++)
    {
        ledState[index] = false;

        HAL_GPIO_WritePin(
            ledConfig[index].port,
            ledConfig[index].pin,
            BSP_LED_GetOutputState(
                (BSP_LED_Id_t)index,
                false
            )
        );
    }
}

bool BSP_LED_Set(BSP_LED_Id_t led, bool state)
{
    if (!BSP_LED_IsValid(led))
    {
        return false;
    }

    HAL_GPIO_WritePin(
        ledConfig[led].port,
        ledConfig[led].pin,
        BSP_LED_GetOutputState(led, state)
    );

    ledState[led] = state;

    return true;
}

bool BSP_LED_On(BSP_LED_Id_t led)
{
    return BSP_LED_Set(led, true);
}

bool BSP_LED_Off(BSP_LED_Id_t led)
{
    return BSP_LED_Set(led, false);
}

bool BSP_LED_Toggle(BSP_LED_Id_t led)
{
    if (!BSP_LED_IsValid(led))
    {
        return false;
    }

    return BSP_LED_Set(
        led,
        !ledState[led]
    );
}

bool BSP_LED_GetState(BSP_LED_Id_t led)
{
    if (!BSP_LED_IsValid(led))
    {
        return false;
    }

    return ledState[led];
}