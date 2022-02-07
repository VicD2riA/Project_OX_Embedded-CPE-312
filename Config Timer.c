//GPIO and LCD
void GPIO_Config_Input(void);
void GPIO_LED_Config (void);
void LCD_Blink(void);

//Timer
void TIM2_Base_Config(void);
void TIM3_Base_Config(void);
void TIM_OC_GPIO_Config(void);
void TIM_BASE_Config(uint16_t ARR);
void TIM_OC_Config(uint16_t note);

void LCD_Blink(void)
{
	LCD_GLASS_BlinkConfig(LCD_BLINKMODE_ALLSEG_ALLCOM, LCD_BLINKFREQUENCY_DIV512);
	LL_mDelay(500);
	LCD_GLASS_BlinkConfig(LCD_BLINKMODE_OFF, LCD_BLINKFREQUENCY_DIV512);
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
	
	GPIO_InitStruct.Pin =  LL_GPIO_PIN_7;
	
	//write configuration to GPIOB registor
	LL_GPIO_Init(GPIOB, &GPIO_InitStruct);	
}

void TIM2_Base_Config(void)
{
	LL_TIM_InitTypeDef timbase_initstructure;
	
	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM2);
	
	timbase_initstructure.ClockDivision = LL_TIM_CLOCKDIVISION_DIV1;
	timbase_initstructure.CounterMode = LL_TIM_COUNTERMODE_UP;
	timbase_initstructure.Autoreload = 1000-1;
	timbase_initstructure.Prescaler = 32000-1;
	
	LL_TIM_Init(TIM2 , &timbase_initstructure);
	LL_TIM_EnableCounter(TIM2);  //This code Open count here
	
}

void TIM3_Base_Config(void)
{
	LL_TIM_InitTypeDef timbase_initstructure;
	
	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM3);
	
	timbase_initstructure.ClockDivision = LL_TIM_CLOCKDIVISION_DIV1;
	timbase_initstructure.CounterMode = LL_TIM_COUNTERMODE_UP;
	timbase_initstructure.Autoreload = 150-1;
	timbase_initstructure.Prescaler = 32000-1;
	
	LL_TIM_Init(TIM3 , &timbase_initstructure);
	LL_TIM_EnableCounter(TIM3);  //This code Open count here
	
}

void TIM_BASE_Config(uint16_t ARR)
{
	LL_TIM_InitTypeDef timbase_initstructure;
	
	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM4);
	
	//Time-base confingure
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
  tim_oc_initstructure.CompareValue = LL_TIM_GetAutoReload(TIM4) / 2; //50% duty
	LL_TIM_OC_Init(TIM4, LL_TIM_CHANNEL_CH1, &tim_oc_initstructure);
	
	/*Start Output Compare in PWM Mode*/
	LL_TIM_CC_EnableChannel(TIM4, LL_TIM_CHANNEL_CH1);
	LL_TIM_EnableCounter(TIM4);
}