#ifndef TIMER_H
#define TIMER_H

#include "stdbool.h"
#include "stdint.h"

#define TIMER_START_VALUE 0
#define TIMER_SCALER_VALUE 60

typedef enum
{
    Timer_Id_0 = 0,
    Timer_Id_1 = 1,
    Timer_Id_2 = 2,
    Timer_Id_3 = 3,
    Timer_Id_4 = 4,
    Timer_Id_Max = 5,
} Timer_Id;

typedef struct
{
    int id;
} Timer_Apbctrl1;

typedef struct
{
    bool isInterruptEnabled;
    bool isEnabled;
    bool isAutoReloaded;
    bool isChained;
    uint32_t reloadValue;
} Timer_Config;

typedef void (*Timer_InterruptCallback)(volatile void* arg);

typedef struct
{
    Timer_InterruptCallback callback;
    volatile void* arg;
} Timer_InterruptHandler;

extern Timer_InterruptHandler defaultInterruptHandler;

void Timer_Apbctrl1_init(Timer_Id id, Timer_Apbctrl1 *const timer, const Timer_InterruptHandler handler){}

void Timer_Apbctrl1_setBaseScalerReloadValue(Timer_Apbctrl1 *const timer, uint16_t scalerReloadValue){}

void Timer_Apbctrl1_setConfigRegisters(Timer_Apbctrl1 *const timer, const Timer_Config *const config){}

void Timer_Apbctrl1_start(Timer_Apbctrl1 *const timer){}

uint32_t Timer_Apbctrl1_getCounterValue(const Timer_Apbctrl1 *const timer){}

#endif