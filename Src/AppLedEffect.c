#include "main.h"
#include "App_Interface.h"
#include "App_config.h"
#include "App_private.h"

extern LEDMenu_T LED_EffectState;
extern TimerHandle_t HandleLedTimer;

GPIO_TypeDef *LED_Ports[NUM_LEDS] = {LED1_PORT, LED2_PORT, LED3_PORT, LED4_PORT};

uint16_t LED_Pins[NUM_LEDS] = {LED1_PIN, LED2_PIN, LED3_PIN, LED4_PIN};


void AppLedEffectCallBackFunc(TimerHandle_t xTimer)
{

	/*Check the desired Led effect*/
	switch(LED_EffectState)
	{
	case effect1: LedEffectAction1();break;
	case effect2: LedEffectAction2();break;
	case effect3: LedEffectAction3();break;
	case effect4: LedEffectAction4();break;
	case none 	: LedEffectStop();break;
	}
}
void AppLedEffectStart(void)
{
	uint8_t Local_u8Counter;

	/*Turn off all Leds*/
	for(Local_u8Counter = 0; Local_u8Counter < NUM_LEDS; Local_u8Counter++)
	{
		HAL_GPIO_WritePin(LED_Ports[Local_u8Counter], LED_Pins[Local_u8Counter],GPIO_PIN_RESET);
	}

	xTimerStart(HandleLedTimer,portMAX_DELAY);
}

void LedEffectAction1(void)
{
	uint8_t Local_u8Counter;

	/*Toggle all LEDS*/
	for(Local_u8Counter = 0; Local_u8Counter < NUM_LEDS; Local_u8Counter++)
	{
		HAL_GPIO_TogglePin(LED_Ports[Local_u8Counter], LED_Pins[Local_u8Counter]);
	}

}
void LedEffectAction2(void)
{
	ToggleEvenOddLeds();

}
void LedEffectAction3(void)
{
	ShiftLeft();
}
void LedEffectAction4(void)
{
	ShiftRight();
}

void LedEffectStop(void)
{
	uint8_t Local_u8Counter;

	/*Turn off all Leds*/
	for(Local_u8Counter = 0; Local_u8Counter < NUM_LEDS; Local_u8Counter++)
	{
		HAL_GPIO_WritePin(LED_Ports[Local_u8Counter], LED_Pins[Local_u8Counter],GPIO_PIN_RESET);
	}

	/*Stop the Timer*/
	xTimerStop(HandleLedTimer,portMAX_DELAY);

}
void ToggleEvenOddLeds(void)
{
	static uint8_t Local_u8Flag = 0;
	uint8_t Local_u8Counter;

	if(Local_u8Flag == 0)
	{
		/*Toggle Even LEDs*/
		HAL_GPIO_WritePin(LED1_PORT, LED1_PIN,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(LED2_PORT, LED2_PIN,GPIO_PIN_SET);
		HAL_GPIO_WritePin(LED3_PORT, LED3_PIN,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(LED4_PORT, LED4_PIN,GPIO_PIN_SET);


	}
	else
	{
		/*Toggle Odd LEDs*/
		HAL_GPIO_WritePin(LED1_PORT, LED1_PIN,GPIO_PIN_SET);
		HAL_GPIO_WritePin(LED2_PORT, LED2_PIN,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(LED3_PORT, LED3_PIN,GPIO_PIN_SET);
		HAL_GPIO_WritePin(LED4_PORT, LED4_PIN,GPIO_PIN_RESET);

	}

	/*Toggle the flag*/
	Local_u8Flag ^= 1;

}
void ShiftRight(void)
{
	static uint8_t Local_u8PinNum = 0;
	uint8_t Local_u8Counter ;
	uint8_t Local_u8ShiftedLed = 1 << Local_u8PinNum;

	for(Local_u8Counter = 0; Local_u8Counter < NUM_LEDS; Local_u8Counter++)
	{
		HAL_GPIO_WritePin(LED_Ports[Local_u8Counter], LED_Pins[Local_u8Counter], ((Local_u8ShiftedLed)>>Local_u8Counter)&1);
	}
	Local_u8PinNum++;
	Local_u8PinNum %= 4;
}

void ShiftLeft(void)
{
	static uint8_t Local_u8PinNum = 0;
	uint8_t Local_u8Counter ;
	uint8_t Local_u8ShiftedLed = 8 >> Local_u8PinNum;

	for(Local_u8Counter = 0; Local_u8Counter < NUM_LEDS; Local_u8Counter++)
	{
		HAL_GPIO_WritePin(LED_Ports[Local_u8Counter], LED_Pins[Local_u8Counter], ((Local_u8ShiftedLed)>>Local_u8Counter)&1);
	}
	Local_u8PinNum++;
	Local_u8PinNum %= 4;

}
