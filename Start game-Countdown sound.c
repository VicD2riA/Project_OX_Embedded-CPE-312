//Sheet note count down 
uint16_t sheetnote_count[] = {D_4,D_5,G_5,E_6};
uint16_t cur_count = 0;

//declare variable num count down 
int num = 3;

 //ClearFlag for any point timeline a something happen flag
LL_TIM_ClearFlag_UPDATE(TIM2);    

//Show NUM 3	
sprintf(disp_str, "   %d" , num);    
LCD_GLASS_DisplayString((uint8_t*)disp_str);	
TIM_OC_Config(ARR_CALCULATE(sheetnote_count[cur_count]));

if(LL_TIM_IsActiveFlag_UPDATE(TIM2) == SET && num != 0)
{
			LL_TIM_ClearFlag_UPDATE(TIM2); //Present timeline flag is high ClearFlag for normal situation		
			num--;
			cur_count++;
			LL_LCD_Clear();
			
			sprintf(disp_str, "   %d" , num);
			LCD_GLASS_DisplayString((uint8_t*)disp_str);
			

            if(num == 0){		
				sprintf(disp_str, " Start");
				LCD_GLASS_DisplayString((uint8_t*)disp_str);
				TIM_OC_Config(ARR_CALCULATE(sheetnote_count[cur_count]));
				
                //LCD display blink 
                LCD_Blink();
				
                //This code Stop! count here.
				LL_TIM_DisableCounter(TIM2); 
				LL_TIM_SetAutoReload(TIM4, MUTE);
				LL_TIM_SetCounter(TIM2, 0);
			}
            else if (num == 1){
				TIM_OC_Config(ARR_CALCULATE(sheetnote_count[cur_count]));
			}	
            else if (num == 2){
				TIM_OC_Config(ARR_CALCULATE(sheetnote_count[cur_count]));
			}			
}