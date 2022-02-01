/*Base register address header file*/
#include "stm32l1xx.h"

/*Library related header files*/
#include "stm32l1xx_ll_gpio.h"
#include "stm32l1xx_ll_pwr.h"
#include "stm32l1xx_ll_rcc.h"
#include "stm32l1xx_ll_bus.h"
#include "stm32l1xx_ll_utils.h"
#include "stm32l1xx_ll_system.h"
#include "stm32l1xx_ll_tim.h"

//Show LCD
#include "stm32l1xx_ll_lcd.h"
#include "stm32l152_glass_lcd.h"

//for random
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define B_3                 (uint16_t)245
#define Db_4                (uint16_t)277
#define D_4                 (uint16_t)293
#define E_4                 (uint16_t)329
#define Gb_4                (uint16_t)370
#define G_4                 (uint16_t)392
#define Ab_4                (uint16_t)415
#define A_4                 (uint16_t)440
#define B_4                 (uint16_t)494
#define Db_5                (uint16_t)554
#define D_5                 (uint16_t)587
#define E_5                 (uint16_t)659
#define F_5                 (uint16_t)698
#define Gb_5                (uint16_t)740
#define G_5                 (uint16_t)784
#define Ab_5                (uint16_t)830
#define A_5                 (uint16_t)880
#define Bb_5                (uint16_t)932
#define B_5                 (uint16_t)988
#define C_6                 (uint16_t)1046
#define Db_6                (uint16_t)1109
#define D_6                 (uint16_t)1174
#define Eb_6                (uint16_t)1244
#define E_6                 (uint16_t)1318
#define MUTE                (uint16_t)1

/*for 10ms update event*/
#define TIMx_PSC			      2 

/*Macro function for ARR calculation*/
#define ARR_CALCULATE(N) ((32000000) / ((TIMx_PSC) * (N)))

uint16_t sheetnote[] = {D_5,MUTE, D_5,D_5,B_4,B_4,D_5,D_5,D_5,E_5,  G_5,G_5,G_5,G_5,A_5,A_5,A_5,G_5,
								  E_5,E_5,E_5,E_5,D_5,D_5,MUTE,D_5, B_4,B_4,B_4,B_4,D_5,D_5,E_5,E_5,
								  G_5,G_5,G_5,G_5,E_5,E_5,G_5,G_5,  MUTE,MUTE,D_5,D_5,E_5,E_5,G_5,G_5,
								  A_5,A_5,G_5,G_5,E_5,E_5,E_5,D_5,  D_5,D_5,D_5,D_5,D_5,D_5,D_5,MUTE};

																	 
void SystemClock_Config(void);
void TIM_BASE_Config(uint16_t);
void TIM_OC_GPIO_Config(void);
void TIM_OC_Config(uint16_t);
void TIM_BASE_DurationConfig(void);
									
void GPIO_Config_Input (void);
void GPIO_LED_Config (void);
uint8_t CheckWin(uint8_t p1, uint8_t p2); // Function Check win

uint8_t usr_button;
uint8_t state_led;
uint16_t cur = 0;									

char disp_str[9];
char disp_WIN[]  = " *WIN*";
char disp_LOSE[] = " LOSE";
char disp_DRAW[] = " DRAW";

uint8_t  player1_rand = 0; 
uint8_t  player2;

int main()
{
	//Max-performance configure
	SystemClock_Config(); 
	
	//Declare i for random
	int i = 0; 
	
	TIM_OC_Config(ARR_CALCULATE(sheetnote[cur]));
	TIM_BASE_DurationConfig();
 
	//Declare user button
	uint8_t usr_button;  
	
  //////////LCD//////////
	LCD_GLASS_Init();	      
	
	/////////INPUT//////////
	GPIO_Config_Input();
	
	/////////OUTPUT LED/////
	GPIO_LED_Config();
	
	//////*START GAME*//////
	sprintf(disp_str, " Start");
	LCD_GLASS_DisplayString((uint8_t*)disp_str);
	
	while(1)
	{		
		//for random 
		srand(i++); 
		state_led =  LL_GPIO_IsOutputPinSet(GPIOB, LL_GPIO_PIN_6);
		usr_button = LL_GPIO_IsInputPinSet (GPIOA,LL_GPIO_PIN_0);
								
				
		if(usr_button == 1)
		 {
			LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_6);
		  LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_7);
			 
			if(state_led == 0) 
			{	
				
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

void GPIO_Config_Input (void)
{
	//Declare struct for GPIO config
	LL_GPIO_InitTypeDef GPIO_InitStruct;
	
	//Enable GPIOA clock
	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA); 
	
	/////////INPUT/////////
	GPIO_InitStruct.Mode  = LL_GPIO_MODE_INPUT;
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	GPIO_InitStruct.Pull  = LL_GPIO_PULL_NO;
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
	
	// USER button PIN 0
	// PA11 button PIN 11
	// PA12 button PIN 12
	GPIO_InitStruct.Pin = LL_GPIO_PIN_0 | LL_GPIO_PIN_11 | LL_GPIO_PIN_12;  
	
	//write configuration to GPIOA registor
	LL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

void GPIO_LED_Config (void)
{
	//Declare struct for GPIO config
	LL_GPIO_InitTypeDef GPIO_InitStruct; 
	
	//Enable GPIOB clock
	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB); 
	
	/////////OUTPUT/////////
	GPIO_InitStruct.Mode  = LL_GPIO_MODE_OUTPUT;
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	GPIO_InitStruct.Pull  = LL_GPIO_PULL_NO;
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;	
	
	GPIO_InitStruct.Pin = LL_GPIO_PIN_6 | LL_GPIO_PIN_7;
	
	//write configuration to GPIOB registor
	LL_GPIO_Init(GPIOB, &GPIO_InitStruct);	
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

void TIM_BASE_DurationConfig(void)
{
	LL_TIM_InitTypeDef timbase_initstructure;
	
	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM2);
	//Time-base configure
	timbase_initstructure.ClockDivision = LL_TIM_CLOCKDIVISION_DIV1;
	timbase_initstructure.CounterMode = LL_TIM_COUNTERMODE_UP;
	timbase_initstructure.Autoreload = 150 - 1;
	timbase_initstructure.Prescaler =  32000 - 1;
	LL_TIM_Init(TIM2, &timbase_initstructure);
	
	LL_TIM_EnableCounter(TIM2); 
	LL_TIM_ClearFlag_UPDATE(TIM2); //Force clear update flag
}

void TIM_BASE_Config(uint16_t ARR)
{
	LL_TIM_InitTypeDef timbase_initstructure;
	
	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM4);
	//Time-base configure
	timbase_initstructure.ClockDivision = LL_TIM_CLOCKDIVISION_DIV1;
	timbase_initstructure.CounterMode = LL_TIM_COUNTERMODE_UP;
	timbase_initstructure.Autoreload = ARR - 1;
	timbase_initstructure.Prescaler =  TIMx_PSC- 1;
	LL_TIM_Init(TIM4, &timbase_initstructure);
	
	LL_TIM_EnableCounter(TIM4); 
}


void TIM_OC_GPIO_Config(void)
{
	LL_GPIO_InitTypeDef gpio_initstructure;
	
	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB);
	
	gpio_initstructure.Mode = LL_GPIO_MODE_ALTERNATE;
	gpio_initstructure.Alternate = LL_GPIO_AF_2;
	gpio_initstructure.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	gpio_initstructure.Pin = LL_GPIO_PIN_6;
	gpio_initstructure.Pull = LL_GPIO_PULL_NO;
	gpio_initstructure.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
	LL_GPIO_Init(GPIOB, &gpio_initstructure);
}

void TIM_OC_Config(uint16_t note)
{
	LL_TIM_OC_InitTypeDef tim_oc_initstructure;
	
	TIM_OC_GPIO_Config();
	TIM_BASE_Config(note);
	
	tim_oc_initstructure.OCState = LL_TIM_OCSTATE_DISABLE;
	tim_oc_initstructure.OCMode = LL_TIM_OCMODE_PWM1;
	tim_oc_initstructure.OCPolarity = LL_TIM_OCPOLARITY_HIGH;
	tim_oc_initstructure.CompareValue = LL_TIM_GetAutoReload(TIM4) / 2;
	LL_TIM_OC_Init(TIM4, LL_TIM_CHANNEL_CH1, &tim_oc_initstructure);
	
	/*Interrupt Configure*/
	NVIC_SetPriority(TIM4_IRQn, 1);
	NVIC_EnableIRQ(TIM4_IRQn);
	LL_TIM_EnableIT_CC1(TIM4);
	
	/*Start Output Compare in PWM Mode*/
	LL_TIM_CC_EnableChannel(TIM4, LL_TIM_CHANNEL_CH1);
	LL_TIM_EnableCounter(TIM4);
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
