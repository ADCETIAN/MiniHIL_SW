#ifndef BSP_LED_H
#define BSP_LED_H

#include <stdbool.h>

typedef enum
{
    BSP_LED_STATUS = 0,
    BSP_LED_ACTIVITY,

    BSP_LED_COUNT
} BSP_LED_Id_t;

void BSP_LED_Init(void);

bool BSP_LED_On(BSP_LED_Id_t led);
bool BSP_LED_Off(BSP_LED_Id_t led);
bool BSP_LED_Toggle(BSP_LED_Id_t led);
bool BSP_LED_Set(BSP_LED_Id_t led, bool state);

bool BSP_LED_GetState(BSP_LED_Id_t led);

#endif /* BSP_LED_H */