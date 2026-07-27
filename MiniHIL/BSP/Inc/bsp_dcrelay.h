/*
 * bsp_dcrelay.h
 *
 *  Created on: 26-Jul-2026
 *      Author: Emagix
 */

#ifndef INC_BSP_DCRELAY_H_
#define INC_BSP_DCRELAY_H_
#include <stdbool.h>

typedef enum
{
    BSP_DC_RELAY_1 = 0,
    BSP_DC_RELAY_2,
    BSP_DC_RELAY_3,
    BSP_DC_RELAY_4,

    BSP_DC_RELAY_COUNT
} BSP_DC_RelayId_t;

void BSP_DC_Relay_Init(void);

bool BSP_DC_Relay_On(BSP_DC_RelayId_t relay);
bool BSP_DC_Relay_Off(BSP_DC_RelayId_t relay);
bool BSP_DC_Relay_Toggle(BSP_DC_RelayId_t relay);
bool BSP_DC_Relay_Set(BSP_DC_RelayId_t relay, bool state);

bool BSP_DC_Relay_GetState(BSP_DC_RelayId_t relay);
bool BSP_DC_Relay_GetPhysicalState(BSP_DC_RelayId_t relay);

void BSP_DC_Relay_AllOff(void);


#endif /* INC_BSP_DCRELAY_H_ */
