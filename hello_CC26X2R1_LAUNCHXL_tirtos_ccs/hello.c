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
/* BIOS Module Headers */
#include <ti/sysbios/BIOS.h>

#include <ti/drivers/Board.h>
#include <ti/drivers/apps/LED.h>
#include <ti/drivers/apps/Button.h>
#include <ti/drivers/Timer.h>

//#include<ti/drivers/GPIO.h>
//#include<ti/drivers/PWM.h>
#include <ti/drivers/UART.h>

#include"ti_drivers_config.h"
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Semaphore.h>
#include <ti/sysbios/knl/Event.h>

//#include DeviceFamily_constructPath(driverlib/timer.h)

/*
 *  ======== main ========
 */

UART_Handle uartHandle;
UART_Params uartParams;

Task_Params taskParams;
Task_Handle taskHandle;

Event_Handle eventHandle;
Event_Params eventParams;
UInt events;
#define UART_EVENT_MASK 0x01
//#define GPIO_EVENT_MASK 0x02

LED_Handle ledHandle;
LED_Params ledParams;
uint8_t currentBrightness = 100;
uint8_t finalBrightness = 100;

Button_Handle buttonHandle;
Button_Params buttonParams;

Timer_Handle timerHandle;
Timer_Params timerParams;
#define TIMER_TRIGGERED 0x02

char taskStack[512];


void setLEDBrightness(uint8_t inputIp){
    int_fast16_t status = Timer_start(timerHandle);
    if(status == Timer_STATUS_ERROR){
        printf("Timer start error!\n");
    }
}

void uartCallBack(UART_Handle handle, void *buf, size_t count){
    finalBrightness = *((uint8_t *)buf);
    Event_post(eventHandle, UART_EVENT_MASK);
}

//global gpioState
void buttonCallback(Button_Handle buttonHandle,
                    Button_EventMask buttonEvents){
    Event_post(eventHandle, buttonEvents);
}

void timerCallBack(Timer_Handle handle, int_fast16_t status){
    if(currentBrightness == finalBrightness){
        Timer_stop(timerHandle);
        return;
    }
    if(currentBrightness > finalBrightness) currentBrightness--;
    else currentBrightness++;
    LED_setBrightnessLevel(ledHandle, currentBrightness);
}


void UARTinit(){
    UART_init();
    UART_Params_init(&uartParams);
    uartParams.baudRate = 115200;
    uartParams.readDataMode = UART_DATA_BINARY;
    uartParams.writeDataMode = UART_DATA_BINARY;
    uartParams.readReturnMode = UART_RETURN_FULL;
    uartParams.readMode = UART_MODE_CALLBACK;
    uartParams.readCallback = uartCallBack;

    uartHandle = UART_open(CONFIG_UART_0, &uartParams);
    if(uartHandle == NULL){
        printf("Uart open failed!\n");
        while(1);
    }
}

void LEDInit(){
    LED_init();
    LED_Params_init(&ledParams);
    ledParams.pwmPeriod = 1000;
    ledHandle = LED_open(LED0, &ledParams);
    if(ledHandle == NULL){
        printf("Failed to initialise LED!\n");
        while(1){}
    }
    LED_setBrightnessLevel(ledHandle, finalBrightness);
}

void ButtonInit(){
    Button_init();
    Button_Params_init(&buttonParams);
    buttonParams.buttonEventMask = Button_EV_CLICKED | Button_EV_DOUBLECLICKED;

    buttonHandle = Button_open(BUTTON0, buttonCallback, &buttonParams);
    if(buttonHandle == NULL){
        printf("Failed to open button.\n");
        while(1){}
    }
}

void TimerInit(){
    Timer_init();
    Timer_Params_init(&timerParams);
    timerParams.periodUnits = Timer_PERIOD_US;
    timerParams.period = 100000;
    timerParams.timerCallback = &timerCallBack;
    timerParams.timerMode = Timer_CONTINUOUS_CALLBACK;

    timerHandle = Timer_open(timer0, &timerParams);
    if(timerHandle == NULL){
        printf("Error opening timer\n");
        while(1){}
    }
}

void myTaskFxn(xdc_UArg arg1, xdc_UArg arg2){
    LEDInit();
    UARTinit();
    ButtonInit();
    TimerInit();

    Event_Params_init(&eventParams);
    eventHandle = Event_create(&eventParams, NULL);
    if(eventHandle == NULL){
        printf("Failed to create event!\n");
        while(1){}
    }
    //create an eventMask with different bits for different action
   /*
    * 0x01 -> read uart
    * 0x02 -> write uart
    * 0x04 -> update pwm
    * 0x08 -> gpio toggle
    * */
    while(1){

        //pend on semaphore
        UART_read(uartHandle, &finalBrightness, 1);
        events = Event_pend(eventHandle,Event_Id_NONE, (UART_EVENT_MASK |
                Button_EV_CLICKED | Button_EV_DOUBLECLICKED), BIOS_WAIT_FOREVER);

        if(events & UART_EVENT_MASK){
            setLEDBrightness(finalBrightness);
        }
        else if(events & Button_EV_CLICKED){ //if single clicked
                //SET gpio brightness to 0
                finalBrightness = 0;
                setLEDBrightness(finalBrightness);
            }
        else if(events & Button_EV_DOUBLECLICKED){ //if zero, then restore previous value
            //set LED brightness to MAX
            finalBrightness = 100;
            setLEDBrightness(finalBrightness);
        }
    }
}

int main()
{
    /* Call driver init functions */
    Board_init();

    Task_Params_init(&taskParams);
    taskParams.stack = (void *)taskStack;
    taskParams.stackSize = sizeof(taskStack);
    taskHandle = Task_create(&myTaskFxn, &taskParams, NULL);

    BIOS_start();
    return(0);
}
