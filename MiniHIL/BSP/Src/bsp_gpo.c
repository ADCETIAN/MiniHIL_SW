/*
 * bsp_gpo.c
 *
 *  Created on: 26-Jul-2026
 *      Author: Emagix
 */

#include "bsp_gpo.h"
#include "main.h"

#include <stdint.h>

typedef struct
{
    GPIO_TypeDef *port;
    uint16_t pin;

} BSP_GpoConfig_t;


/*
 * Hardware behavior:
 *
 * MCU pin HIGH:
 *     Optocoupler ON
 *     BB output LOW
 *     Logical GPO state = ON
 *
 * MCU pin LOW:
 *     Optocoupler OFF
 *     BB output HIGH through pull-up
 *     Logical GPO state = OFF
 */
static const BSP_GpoConfig_t gpoConfig[BSP_GPO_COUNT] =
{
    [BSP_GPO_1] =
    {
        .port = GPO1_GPIO_Port,
        .pin  = GPO1_Pin
    },

    [BSP_GPO_2] =
    {
        .port = GPO2_GPIO_Port,
        .pin  = GPO2_Pin
    },

    [BSP_GPO_3] =
    {
        .port = GPO3_GPIO_Port,
        .pin  = GPO3_Pin
    },

    [BSP_GPO_4] =
    {
        .port = GPO4_GPIO_Port,
        .pin  = GPO4_Pin
    },

    [BSP_GPO_5] =
    {
        .port = GPO5_GPIO_Port,
        .pin  = GPO5_Pin
    },

    [BSP_GPO_6] =
    {
        .port = GPO6_GPIO_Port,
        .pin  = GPO6_Pin
    },

    [BSP_GPO_7] =
    {
        .port = GPO7_GPIO_Port,
        .pin  = GPO7_Pin
    },

    [BSP_GPO_8] =
    {
        .port = GPO8_GPIO_Port,
        .pin  = GPO8_Pin
    }
};


/*
 * Stores logical state, not BB output voltage.
 *
 * true:
 *     Logical ON
 *     MCU pin HIGH
 *     BB output LOW
 *
 * false:
 *     Logical OFF
 *     MCU pin LOW
 *     BB output HIGH
 */
static bool gpoState[BSP_GPO_COUNT] =
{
    false
};


static bool BSP_GPO_IsValid(BSP_GpoId_t gpo)
{
    return ((uint32_t)gpo < (uint32_t)BSP_GPO_COUNT);
}


void BSP_GPO_Init(void)
{
    /*
     * Required default:
     * All GPOs logically ON.
     * Therefore all BB outputs are LOW.
     */
    BSP_GPO_AllOn();
}


bool BSP_GPO_Set(
    BSP_GpoId_t gpo,
    bool logicalState)
{
    GPIO_PinState mcuPinState;

    if (!BSP_GPO_IsValid(gpo))
    {
        return false;
    }

    /*
     * Logical ON  → MCU HIGH → external BB output LOW
     * Logical OFF → MCU LOW  → external BB output HIGH
     */
    mcuPinState = logicalState
                    ? GPIO_PIN_SET
                    : GPIO_PIN_RESET;

    HAL_GPIO_WritePin(
        gpoConfig[gpo].port,
        gpoConfig[gpo].pin,
        mcuPinState
    );

    gpoState[gpo] = logicalState;

    return true;
}


bool BSP_GPO_On(BSP_GpoId_t gpo)
{
    return BSP_GPO_Set(gpo, true);
}


bool BSP_GPO_Off(BSP_GpoId_t gpo)
{
    return BSP_GPO_Set(gpo, false);
}


bool BSP_GPO_Toggle(BSP_GpoId_t gpo)
{
    if (!BSP_GPO_IsValid(gpo))
    {
        return false;
    }

    return BSP_GPO_Set(
        gpo,
        !gpoState[gpo]
    );
}


bool BSP_GPO_GetState(BSP_GpoId_t gpo)
{
    if (!BSP_GPO_IsValid(gpo))
    {
        return false;
    }

    /*
     * Returns logical protocol state:
     *
     * 1 = GPO ON, BB output LOW
     * 0 = GPO OFF, BB output HIGH
     */
    return gpoState[gpo];
}


bool BSP_GPO_GetPhysicalState(BSP_GpoId_t gpo)
{
    GPIO_PinState pinState;

    if (!BSP_GPO_IsValid(gpo))
    {
        return false;
    }

    pinState = HAL_GPIO_ReadPin(
        gpoConfig[gpo].port,
        gpoConfig[gpo].pin
    );

    /*
     * MCU pin HIGH means logical GPO ON.
     */
    return (pinState == GPIO_PIN_SET);
}


void BSP_GPO_AllOn(void)
{
    uint32_t index;

    for (index = 0U;
         index < (uint32_t)BSP_GPO_COUNT;
         index++)
    {
        (void)BSP_GPO_On(
            (BSP_GpoId_t)index
        );
    }
}


void BSP_GPO_AllOff(void)
{
    uint32_t index;

    for (index = 0U;
         index < (uint32_t)BSP_GPO_COUNT;
         index++)
    {
        (void)BSP_GPO_Off(
            (BSP_GpoId_t)index
        );
    }
}