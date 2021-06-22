/*
 * Copyright (c) 2015-2019, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 *  ======== hello.c ========
 */

#include <stdio.h>
/* XDC Module Headers */
#include <xdc/std.h>
#include <xdc/runtime/System.h>
#include <xdc/runtime/Error.h>
#include <ti/sysbios/hal/Hwi.h>
/* BIOS Module Headers */
#include <ti/sysbios/BIOS.h>

#include <ti/drivers/Board.h>
#include<ti/drivers/Timer.h>
#include<ti/drivers/GPIO.h>
#include"ti_drivers_config.h"

#include DeviceFamily_constructPath(driverlib/timer.h)

/*
 *  ======== main ========
 */

#define  _K_TIMER_ALL_INTS  (TIMER_TIMB_DMA | \
                             TIMER_TIMB_MATCH |   \
                             TIMER_CAPB_EVENT |   \
                             TIMER_CAPB_MATCH |   \
                             TIMER_TIMB_TIMEOUT | \
                             TIMER_TIMA_DMA | \
                             TIMER_TIMA_MATCH |   \
                             TIMER_CAPA_EVENT |   \
                             TIMER_CAPA_MATCH |   \
                             TIMER_TIMA_TIMEOUT)

void timerISR(UArg arg){
    //printf("ISR called!\n");
    TimerIntClear(GPT1_BASE, _K_TIMER_ALL_INTS);
    Hwi_clearInterrupt(33);
    IntPendClear(33);
    GPIO_toggle(CONFIG_GPIO_LED1);
}

void timerISR2(Timer_Handle tHandle, int_fast16_t status){

    //printf("ISR 2 called\n");
    GPIO_toggle(CONFIG_GPIO_1);
}

int main()
{
    /* Call driver init functions */
    Board_init();

    Hwi_Handle myHwi;
    Hwi_Params hParams;
    Error_Block eb;

    Timer_Handle tHandle;
    Timer_Handle t2Handle;
    Timer_Params tParams;
    int_fast16_t tStatus;
    int_fast16_t t2Status;

    Error_init(&eb);
    Hwi_Params_init(&hParams);
    hParams.enableInt = false;
    hParams.priority = 0;
    myHwi = Hwi_create(33, timerISR, &hParams, &eb);

    if (myHwi == NULL) {
    System_abort("Hwi create failed");
    }

    Hwi_enable();
    Hwi_enableInterrupt(33);

    Timer_Params_init(&tParams);
    tParams.timerMode = Timer_CONTINUOUS_CALLBACK;
    tParams.periodUnits = Timer_PERIOD_US;
    tParams.period = 300;
    tParams.timerCallback = timerISR2;

    tHandle = Timer_open(timer0,&tParams);
    if(tHandle == NULL){
        printf("Error opening timer instance. \n");
        while(1){}
    }

    t2Handle = Timer_open(timer1, &tParams);
    if(t2Handle == NULL){
        printf("Error opening timer instance. \n");
        while(1){}
    }

    tStatus = Timer_start(tHandle);
    if(tStatus == Timer_STATUS_ERROR){
        printf("Error starting timer\n");
    }

    t2Status = Timer_start(t2Handle);
    if(t2Status == Timer_STATUS_ERROR){
        printf("Error starting timer\n");
    }
    /*
     *  normal BIOS programs, would call BIOS_start() to enable interrupts
     *  and start the scheduler and kick BIOS into gear.  But, this program
     *  is a simple sanity test and calls BIOS_exit() instead.
     */
    BIOS_start();
    return(0);
}
