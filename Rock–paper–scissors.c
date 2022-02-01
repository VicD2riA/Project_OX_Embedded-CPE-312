/*Base register address header file*/
#include "stm32l1xx.h"

/*Library related header files*/
#include "stm32l1xx_ll_gpio.h"
#include "stm32l1xx_ll_pwr.h"
#include "stm32l1xx_ll_rcc.h"
#include "stm32l1xx_ll_bus.h"
#include "stm32l1xx_ll_utils.h"
#include "stm32l1xx_ll_system.h"

//Show LCD
#include "stm32l1xx_ll_lcd.h"
#include "stm32l152_glass_lcd.h"

//for random
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void SystemClock_Config(void); 
uint8_t CheckWin(uint8_t p1, uint8_t p2); // Function Check win

uint8_t usr_button;
uint8_t state_led; 

char disp_str[9];
char disp_WIN[]  = " *WIN*";
char disp_LOSE[] = " LOSE";
char disp_DRAW[] = " DRAW";

uint8_t  player1_rand = 0; 
uint8_t  player2;

int main()
{
	int i = 0; //Declare i for random
	uint8_t usr_button;  //Declare
	LL_GPIO_InitTypeDef GPIO_InitStruct; //Declare struct for GPIO config
	
    //////////LCD//////////
	SystemClock_Config();   //Max-performance configure
	LCD_GLASS_Init();	    //LCD low-level init
	
	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA); //Enable GPIOA clock
	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB); //Enable GPIOB clock
	
	///////////DAC////////////
	RCC->APB1ENR |= (1<<29); //eneble clock DAC at bit 29
	GPIOA->MODER |= (3<<8);  // PA4 analog mode
	DAC->CR &= ~(1<<0);      //DAC channel 1 disenable
	DAC->DHR12R1 = 0x0FFF;	
	
	////////OUTPUT////////////
	GPIO_InitStruct.Mode  = LL_GPIO_MODE_OUTPUT;
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	GPIO_InitStruct.Pull  = LL_GPIO_PULL_NO;
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;	
	
	GPIO_InitStruct.Pin = LL_GPIO_PIN_6;
	LL_GPIO_Init(GPIOB, &GPIO_InitStruct);//write configuration to GPIOB registor
	GPIO_InitStruct.Pin = LL_GPIO_PIN_7;
	LL_GPIO_Init(GPIOB, &GPIO_InitStruct);//configuration to GPIOB for pin 7
	
	/////////INPUT//////////
	GPIO_InitStruct.Mode  = LL_GPIO_MODE_INPUT;
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	GPIO_InitStruct.Pull  = LL_GPIO_PULL_NO;
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
	
	GPIO_InitStruct.Pin = LL_GPIO_PIN_0;  // USER button
	LL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	GPIO_InitStruct.Pin = LL_GPIO_PIN_11; // PA11 button
	LL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	GPIO_InitStruct.Pin = LL_GPIO_PIN_12; // PA12 button
 	LL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	
	//////*START GAME*///////
	/////////////////////////////////////////////////////////
	sprintf(disp_str, " Start");
	LCD_GLASS_DisplayString((uint8_t*)disp_str);
	
	while(1)
	{		
		srand(i++); //for random 
		state_led =  LL_GPIO_IsOutputPinSet(GPIOB, LL_GPIO_PIN_6);
		usr_button = LL_GPIO_IsInputPinSet (GPIOA,LL_GPIO_PIN_0);
								
		if(usr_button == 1)
		 {
			LL_GPIO_TogglePin(GPIOB, LL_GPIO_PIN_6);
		    LL_GPIO_TogglePin(GPIOB, LL_GPIO_PIN_7);
			 
			if(state_led == 0) 
			{	
				DAC->CR |= (1<<0);  //DAC channel 1 enable
				sprintf(disp_str, " PLAYER 1 ");
				LCD_GLASS_ScrollSentence((uint8_t*) disp_str , 2 , 100);
							
			}		
			while(usr_button != 0){		
				player1_rand = rand() % 3 ; // random value is 0 is scissor , 1 is rock , 2 is paper
				usr_button = LL_GPIO_IsInputPinSet(GPIOA,LL_GPIO_PIN_0);			    
			}
			
			LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_6); // Close LED PIN6/PIN7
		    LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_7);
			
			sprintf(disp_str, "UR TURN");
			LCD_GLASS_DisplayString((uint8_t*)disp_str);	
			DAC->CR &= ~(1<<0);			
		 }
		 
		 // 01 is scissor , 10 is rock , 00 is paper
		 if( LL_GPIO_IsInputPinSet(GPIOA,LL_GPIO_PIN_11) == 0 || LL_GPIO_IsInputPinSet(GPIOA,LL_GPIO_PIN_12) == 0 )
		 {
				LL_mDelay(300); 
				if (LL_GPIO_IsInputPinSet(GPIOA,LL_GPIO_PIN_11) == 0 && LL_GPIO_IsInputPinSet(GPIOA,LL_GPIO_PIN_12) == 1)
					{
						player2 = 0; //scissor
						DAC->CR |= (1<<0);				
					}	
				else if(LL_GPIO_IsInputPinSet(GPIOA,LL_GPIO_PIN_11) == 1 && LL_GPIO_IsInputPinSet(GPIOA,LL_GPIO_PIN_12) == 0)
					{
						player2 = 1; //rock
						DAC->CR |= (1<<0);				
					}
				else if(LL_GPIO_IsInputPinSet(GPIOA,LL_GPIO_PIN_11) == 0 && LL_GPIO_IsInputPinSet(GPIOA,LL_GPIO_PIN_12) == 0)
					{
						player2 = 2; //paper
						DAC->CR |= (1<<0);				
					}	
				LL_mDelay(50);
				DAC->CR &= ~(1<<0);	
			
				// 0 is scissor , 1 is rock , 2 is paper			
				if(CheckWin(player1_rand,player2) == 0)     //Check DRAW
					{	
						LCD_GLASS_Clear();
						LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_6); // Close LED PIN6/PIN7
						LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_7);
						LCD_GLASS_DisplayString((uint8_t*)disp_DRAW);
					
					}
				else if (CheckWin(player1_rand,player2) == 1) //Check  LOSE
					{
						LCD_GLASS_Clear();
						LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_6); // Close LED PIN6/PIN7
						LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_7);
						LCD_GLASS_DisplayString((uint8_t*)disp_LOSE);
				
					}					
				else if(CheckWin(player1_rand,player2) == 2)   //Check WIN
					{				
						LCD_GLASS_Clear();				
						LCD_GLASS_DisplayString((uint8_t*)disp_WIN);	
						LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_6); // Open LED PIN6/PIN7
						LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_7); 

						DAC->CR |= (1<<0);	//If win sound is up
						LCD_GLASS_BlinkConfig(LCD_BLINKMODE_ALLSEG_ALLCOM, LCD_BLINKFREQUENCY_DIV512);
						LL_mDelay(2000);
						LCD_GLASS_BlinkConfig(LCD_BLINKMODE_OFF, LCD_BLINKFREQUENCY_DIV512);
						DAC->CR &= ~(1<<0);	
					}		
		} 		 	 	 
	}

}
void SystemClock_Config(void)
{
  /* Enable ACC64 access and set FLASH latency */ 
  LL_FLASH_Enable64bitAccess();; 
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_1);

  /* Set Voltage scale1 as MCU will run at 32MHz */
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);
  LL_PWR_SetRegulVoltageScaling(LL_PWR_REGU_VOLTAGE_SCALE1);
  
  /* Poll VOSF bit of in PWR_CSR. Wait until it is reset to 0 */
  while (LL_PWR_IsActiveFlag_VOSF() != 0)
  {
  };
  
  /* Enable HSI if not already activated*/
  if (LL_RCC_HSI_IsReady() == 0)
  {
    /* HSI configuration and activation */
    LL_RCC_HSI_Enable();
    while(LL_RCC_HSI_IsReady() != 1)
    {
    };
  }
  
	
  /* Main PLL configuration and activation */
  LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSI, LL_RCC_PLL_MUL_6, LL_RCC_PLL_DIV_3);

  LL_RCC_PLL_Enable();
  while(LL_RCC_PLL_IsReady() != 1)
  {
  };
  
  /* Sysclk activation on the main PLL */
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL)
  {
  };
  
  /* Set APB1 & APB2 prescaler*/
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
  LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);

  /* Set systick to 1ms in using frequency set to 32MHz                             */
  /* This frequency can be calculated through LL RCC macro                          */
  /* ex: __LL_RCC_CALC_PLLCLK_FREQ (HSI_VALUE, LL_RCC_PLL_MUL_6, LL_RCC_PLL_DIV_3); */
  LL_Init1msTick(32000000);
  
  /* Update CMSIS variable (which can be updated also through SystemCoreClockUpdate function) */
  LL_SetSystemCoreClock(32000000);
}
uint8_t CheckWin(uint8_t p1, uint8_t p2)
{
	uint8_t result;
	// 0 is scissor , 1 is rock , 2 is paper
	// 0 is draw , 1 is lose , 2 is win 
	
	if(p2 == 0) //player 2 is scissor
	{
		if(p1 == 0) 
			result = 0;
		else if(p1 == 1)
			result = 1;
		else if(p1 == 2)
			result = 2;
	}
	if(p2 == 1) //player 2 is rock
	{
		if(p1 == 0) 
			result = 2; 
		else if(p1 == 1)
			result = 0;
		else if(p1 == 2)
			result = 1;
	}
	if(p2 == 2) //player 2 is paper
	{
		if(p1 == 0)
			result = 1;
		else if(p1 == 1)
			result = 2;
		else if(p1 == 2)
			result = 0;
	}	
	return(result);
}


