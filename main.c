#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include <CoOS.h>                                            /*!< CoOS header file */
#include "trend_func_lib.h"

#define STACK_SIZE_DEFAULT 128								/*! Same as MCU flash size*/

OS_STK task1_stk[STACK_SIZE_DEFAULT];
OS_STK task2_stk[STACK_SIZE_DEFAULT];

void initializeBoard(){

        GPIO_InitTypeDef GPIO_InitStructure_Led;
        GPIO_InitTypeDef GPIO_InitStructure_Button;

        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);//Enables the clock for the LED's connected to the GPIO in the C bank
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//Enables the clock for the buttons connected to the GPIO in the A bank

        GPIO_InitStructure_Led.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9; //Configure the two LED's
        GPIO_InitStructure_Led.GPIO_Mode = GPIO_Mode_Out_PP;
        GPIO_InitStructure_Led.GPIO_Speed = GPIO_Speed_50MHz;

        GPIO_InitStructure_Button.GPIO_Pin = GPIO_Pin_0; //Configure the user button
        GPIO_InitStructure_Button.GPIO_Mode = GPIO_Mode_IN_FLOATING;
        GPIO_InitStructure_Button.GPIO_Speed = GPIO_Speed_50MHz;

        GPIO_Init(GPIOC,&GPIO_InitStructure_Led);
        GPIO_Init(GPIOA,&GPIO_InitStructure_Button);

}

/*Global Variables*/
static int ledval = 0; //Global LED integer
double s1R;

/*RTOS Task 1*/
/*Flashes the two LED's in an alternating pattern*/
void task1 (void* pdata){
        while(1){
        		GPIO_WriteBit(GPIOC, GPIO_Pin_8, (ledval) ? Bit_SET : Bit_RESET);
        		ledval = 1 - ledval;
        		GPIO_WriteBit(GPIOC, GPIO_Pin_9, (ledval) ? Bit_SET : Bit_RESET);
                CoTimeDelay(0,0,5,0); //Each LED turns on for 3 seconds
        		}
}

/*RTOS Task 2*/
void task2 (void* pdata){
        while(1){
        		//mystep1();
        		//mystep2();
        		}
}

/*Main Program Loop*/
int main(void)
{
        initializeBoard();
        CoInitOS();
        CoCreateTask(task1,0,0,&task1_stk[STACK_SIZE_DEFAULT-1],STACK_SIZE_DEFAULT);
        CoCreateTask(task2,0,1,&task2_stk[STACK_SIZE_DEFAULT-1],STACK_SIZE_DEFAULT);
        CoStartOS();
    while(1);
}
