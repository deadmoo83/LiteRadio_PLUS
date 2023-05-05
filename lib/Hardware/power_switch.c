#include "power_switch.h"
#include "key.h"
#include "rgb.h"
#include "buzzer.h"
#include "radiolink.h"

EventGroupHandle_t powerEventHandle = NULL;
TaskHandle_t powerTaskHandle;

void powerswitchTask(void* param)
{
	EventBits_t powerEvent;
	while(1)
	{
		vTaskDelay(100);
		powerEvent= xEventGroupWaitBits( powerEventHandle,
                                         RC_POWER_ON|RC_POWER_OFF,
		                                 pdTRUE,
	                                     pdFALSE,
		                                 0);
		if((powerEvent & RC_POWER_ON) == RC_POWER_ON)
		{

            RGB_BreathUp(BLUE);
            taskENTER_CRITICAL();	/*�����ٽ�*/
            
            vTaskSuspend(powerTaskHandle);

            taskEXIT_CRITICAL();   /*�˳��ٽ�*/
		}
		if((powerEvent & RC_POWER_OFF) == RC_POWER_OFF)
		{
            RGB_Set(BLACK,BRIGHTNESS_MAX);
            xEventGroupSetBits( buzzerEventHandle, POWER_OFF_RING);
           // RGB_Breath_Down(RED);
            POWER_PIN_HOLD_DOWN();
		}        
	}
}



