/*
 * bsp_gpo.h
Protocol state 1 → GPO logically ON → MCU pin HIGH → optocoupler ON → BB output LOW
Protocol state 0 → GPO logically OFF → MCU pin LOW → optocoupler OFF → BB output HIGH
 *  Created on: 26-Jul-2026
 *      Author: Emagix
 */

#ifndef INC_BSP_GPO_H_
#define INC_BSP_GPO_H_

#include <stdbool.h>

typedef enum
{
    BSP_GPO_1 = 0,
    BSP_GPO_2,
    BSP_GPO_3,
    BSP_GPO_4,
    BSP_GPO_5,
    BSP_GPO_6,
    BSP_GPO_7,
    BSP_GPO_8,

    BSP_GPO_COUNT

} BSP_GpoId_t;

void BSP_GPO_Init(void);

bool BSP_GPO_Set(BSP_GpoId_t gpo, bool logicalState);
bool BSP_GPO_On(BSP_GpoId_t gpo);
bool BSP_GPO_Off(BSP_GpoId_t gpo);
bool BSP_GPO_Toggle(BSP_GpoId_t gpo);

bool BSP_GPO_GetState(BSP_GpoId_t gpo);
bool BSP_GPO_GetPhysicalState(BSP_GpoId_t gpo);

void BSP_GPO_AllOn(void);
void BSP_GPO_AllOff(void);

#endif /* INC_BSP_GPO_H_ */