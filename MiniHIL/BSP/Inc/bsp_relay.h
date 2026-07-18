#ifndef BSP_RELAY_H
#define BSP_RELAY_H

#include <stdbool.h>

typedef enum
{
    BSP_RELAY_1 = 0,
    BSP_RELAY_2,
    BSP_RELAY_3,
    BSP_RELAY_4,

    BSP_RELAY_COUNT
} BSP_RelayId_t;

void BSP_Relay_Init(void);

bool BSP_Relay_On(BSP_RelayId_t relay);
bool BSP_Relay_Off(BSP_RelayId_t relay);
bool BSP_Relay_Toggle(BSP_RelayId_t relay);
bool BSP_Relay_Set(BSP_RelayId_t relay, bool state);

bool BSP_Relay_GetState(BSP_RelayId_t relay);
bool BSP_Relay_GetPhysicalState(BSP_RelayId_t relay);

void BSP_Relay_AllOff(void);

#endif /* BSP_RELAY_H */
