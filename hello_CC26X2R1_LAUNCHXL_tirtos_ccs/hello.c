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
#include<ti/drivers/GPIO.h>
#include<ti/drivers/PWM.h>
#include <ti/drivers/UART.h>

#include"ti_drivers_config.h"
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Semaphore.h>
//#include DeviceFamily_constructPath(driverlib/timer.h)

/*
 *  ======== main ========
 */


PWM_Handle pwmHandle;
PWM_Params pwmParams;

UART_Handle uartHandle;
UART_Params uartParams;

Task_Params taskParams;
Task_Handle taskHandle;

Semaphore_Handle semHandle;
Semaphore_Params semParams;

char taskStack[512];
uint8_t input = 0;
uint8_t pwmPrevVal = 0;

void uartWriteCallBack(UART_Handle handle, void *buf, size_t count){
    UART_read(uartHandle, &input, 1);
}
void uartCallBack(UART_Handle handle, void *buf, size_t count){

    printf("Callback called\n");
//    const char echoBuffer[] = "Hello from callback";
//    UART_write(handle, echoBuffer, sizeof(echoBuffer));
    UART_write(handle, buf, 1);
//    uint8_t val = *((uint8_t *)buf);
//    size_t bytesWritten;
//    printf("received value: %hu\n", val);
//    PWM_setDuty(userArg, val);
//
//    UART2_write(handle, buf, count, &bytesWritten);
}

//global gpioState
void gpioCallBackFxn(uint_least8_t index){
    printf("GPIO callback!\n");
    Semaphore_post(semHandle);

    // gpioState

}
void UARTinit(){
    UART_init();
    UART_Params_init(&uartParams);
    uartParams.baudRate = 115200;
    uartParams.readDataMode = UART_DATA_BINARY;
    uartParams.writeDataMode = UART_DATA_BINARY;
    uartParams.readReturnMode = UART_RETURN_FULL;

//    uartParams.readMode = UART_MODE_BLOCKING;

    uartHandle = UART_open(CONFIG_UART_0, &uartParams);
    if(uartHandle == NULL){
        printf("Uart open failed!\n");
        while(1);
    }
}

void PWMinit(){
    PWM_init();
    PWM_Params_init(&pwmParams);
    pwmParams.dutyValue = 0;
    pwmParams.dutyUnits = PWM_DUTY_FRACTION;
    pwmParams.periodUnits = PWM_PERIOD_US;
    pwmParams.periodValue = 30000;
    pwmParams.idleLevel = PWM_IDLE_LOW;

    pwmHandle = PWM_open(CONFIG_PWM_0, &pwmParams);
    if(pwmHandle == NULL){
        printf("Error opening PWM o/p\n");
        while(1){}
    }
    PWM_start(pwmHandle);
}

void GPIOinit(){
    GPIO_init();
    GPIO_setCallback(BUTTON0,gpioCallBackFxn);
    GPIO_enableInt(BUTTON0);

}
void myTaskFxn(xdc_UArg arg1, xdc_UArg arg2){
    printf("Task started\n");
    PWMinit();
    UARTinit();
    GPIOinit();

    //create semaphore counting

    Semaphore_Params_init(&semParams);
    semParams.mode = ti_sysbios_knl_Semaphore_Mode_COUNTING;
    semParams.event = NULL;

    semHandle = Semaphore_create(0, &semParams, NULL);

//    appSemHandle = Semaphore_handle(&appSem);

//    appServiceTaskId = OsalPort_registerTask(Task_self(), appSemHandle,
//                                            &appServiceTaskEvents);

//    Semaphore_create(1, &semParams, NULL);

    //create an eventMask with different bits for different action
   /*
    * 0x01 -> read uart
    * 0x02 -> write uart
    * 0x04 -> update pwm
    * 0x08 -> gpio toggle
    * */

    input = 80;
    PWM_setDuty(pwmHandle, (uint32_t) (((uint64_t) PWM_DUTY_FRACTION_MAX * input / 100)));

    while(1){

        //pend on semaphore

        //if(events & 0x01)
        // readuart
        //

//        UART_read(uartHandle, &input, 1);
//        UART_write(uartHandle, &input, 1);

        Semaphore_pend(semHandle, BIOS_WAIT_FOREVER);
        if(input!=0){ //if pem!= 0, set to zero
                pwmPrevVal = input;

                UART_write(uartHandle, &input, 1);
        //        UART_write(uartHandle, '\n', 1);
                UART_write(uartHandle, &pwmPrevVal, 1);

                input = 0;
                PWM_setDuty(pwmHandle, (uint32_t) (((uint64_t) PWM_DUTY_FRACTION_MAX * input / 100)));
            }
            else{ //if zero, then restore previous value
                UART_write(uartHandle, &input, 1);
        //        UART_write(uartHandle, '\n', 1);
                UART_write(uartHandle, &pwmPrevVal, 1);

                input = pwmPrevVal;
                PWM_setDuty(pwmHandle, (uint32_t) (((uint64_t) PWM_DUTY_FRACTION_MAX * input / 100)));
            }
//        Semaphore_post(semHandle);
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
