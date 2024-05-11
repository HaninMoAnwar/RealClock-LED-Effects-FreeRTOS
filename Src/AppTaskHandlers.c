#include "main.h"

#include "App_Interface.h"
#include "App_config.h"
#include "App_private.h"

extern TaskHandle_t CmdTask, DisplayTask, MainMenuTask, LEDsEffectTask, RTCTask;

extern QueueHandle_t Q_Send, Q_Receive;

extern RTC_HandleTypeDef hrtc;

extern UART_HandleTypeDef huart2;

extern State_T CurrentState;

extern TimerHandle_t HandleRTCReportTimer;

const char* InvalidMsgs[2] = {
		"////////The Input Exceeded The Valid Length////////\n",
		"////////The Input Is Invalid Option////////\n"};


LEDMenu_T LED_EffectState = none;

void ProcessCommand(Cmd_T *Copy_Command)
{
	/*Extract command */
	ExtractCommand(Copy_Command);

	/* Notify the desired task */
	switch(CurrentState)
	{
	case MainMenuState:
		/* Notify Main-Menu Task by giving the address of the command*/
		xTaskNotify(MainMenuTask,(uint32_t)Copy_Command,eSetValueWithOverwrite);
		break;

	case LedMenuState:
		/* Notify Led Task by giving the address of the command*/
		xTaskNotify(LEDsEffectTask,(uint32_t)Copy_Command,eSetValueWithOverwrite);
		break;

	case RTCMenuState:
		/* Notify RTC Task by giving the address of the command*/
		xTaskNotify(RTCTask,(uint32_t)Copy_Command,eSetValueWithOverwrite);
		break;


	}
}
void ExtractCommand(Cmd_T *Copy_Command)
{
	uint8_t Local_u8Counter;

	/*Get length of the message inside Q-Receive*/
	BaseType_t Local_MsgLen = uxQueueMessagesWaiting(Q_Receive);

	for(Local_u8Counter = 0; Local_u8Counter < Local_MsgLen; Local_u8Counter++)
	{
		/* Extract the command from Q-reveive*/
		xQueueReceive(Q_Receive, (void *)&Copy_Command->Payload[Local_u8Counter], 0);

		if((char)Copy_Command->Payload[Local_u8Counter] == CMD_LINE_FINISH_CHAR)
		{
			/* Command is finished*/
			break;
		}
	}

	/* Put NULL character at the end of command to convert it into string*/
	Copy_Command->Payload[Local_u8Counter] = '\0';

	/*Save the command length*/
	Copy_Command->Len = Local_u8Counter;


}

void AppDisplayHandler(void * Paramters)
{
	uint32_t *Local_pu32MsgAddr = NULL;

	while(1)
	{
		/*Wait for any data is sent to Q_Send queue*/
		xQueueReceive(Q_Send, (void *)&Local_pu32MsgAddr, portMAX_DELAY);

		/* Transmit the messages through UART*/
		HAL_UART_Transmit(&huart2, (uint8_t *)Local_pu32MsgAddr, strlen((char*)Local_pu32MsgAddr),HAL_MAX_DELAY);

	}
}

void AppCmdHandler(void * Paramters)
{
	BaseType_t Local_ReturnVal;

	Cmd_T Command;

	while(1)
	{
		/* Wait for UART notification */
		Local_ReturnVal = xTaskNotifyWait(0,0,NULL,portMAX_DELAY);

		if(Local_ReturnVal == pdTRUE)
		{
			ProcessCommand(&Command);
		}
		else
		{
			/*Do nothing*/
		}
	}
}

void AppMainMenuHandler(void * Paramters)
{
	uint32_t Local_u32CommandAddr;
	Cmd_T *Local_Command;

	const char *MsgMainMenu =
			"\n------------------------------"
			"\n------------------------------"
			"\n---- Welcome To Main Menu ----"
			"\n------------------------------"
			"\n------------------------------"
			"\nLED Effect---> 0"
			"\nDate&Time----> 1"
			"\nExit---------> 2"
			"\nEnter Your Choice-->";

	while(1)
	{
		/*Display the Main-Menu*/
		xQueueSend(Q_Send,(void *)&MsgMainMenu,portMAX_DELAY);

		/*Wait for receiving the command address*/
		xTaskNotifyWait(0,0,&Local_u32CommandAddr,portMAX_DELAY);

		/* Extract the command from the address*/
		Local_Command = (Cmd_T *)Local_u32CommandAddr;

		/* Validate command length
		 * The input shouldn't exceed 1 */
		if(Local_Command->Len == MainMenuLen)
		{
			/* Valid Length*/

			switch(Local_Command->Payload[0])
			{

			/* User chose LED Effects */
			case LED_Effect: CurrentState = LedMenuState; xTaskNotify(LEDsEffectTask,0,eNoAction);break;

			/* User chose Time & Date */
			case Time_Date: CurrentState = RTCMenuState; xTaskNotify(RTCTask,0,eNoAction);break;

			/* User chose to exit */
			case Exit_From_App:	break;

			default:/* Invalid Option*/

				CurrentState = MainMenuState;
				xQueueSend(Q_Send,(void*)&InvalidMsgs[InvalidOptn],portMAX_DELAY);
			}

		}
		else
		{
			/* Invalid Length*/
			CurrentState = MainMenuState;
			xQueueSend(Q_Send,(void*)&InvalidMsgs[InvalidLen],portMAX_DELAY);
		}

		/*Wait for any task finishes its work */
		xTaskNotifyWait(0,0,NULL,portMAX_DELAY);

	}
}
void AppLEDsEffectHandler(void * Paramters)
{
	uint32_t Local_u32CommandAddr;
	Cmd_T *Local_Command;

	const char *MsgLED =
			"\n------------------------------"
			"\n------------------------------"
			"\n------- LED Effect Menu ------"
			"\n------------------------------"
			"\n------------------------------"
			"\n(effect1, effect2, effect3, effect4, none)"
			"\nEnter Your Choice-->";


	while(1)
	{

		/*Wait for Main-Menu notification*/
		xTaskNotifyWait(0,0,NULL,portMAX_DELAY);

		/*Display LED-Effect Menu*/
		xQueueSend(Q_Send,(void *)&MsgLED,portMAX_DELAY);

		/*Wait for receiving command address*/
		xTaskNotifyWait(0,0,&Local_u32CommandAddr,portMAX_DELAY);

		/*Extract the command*/
		Local_Command = (Cmd_T*)Local_u32CommandAddr;

		/*Validate the Length*/
		if(Local_Command->Len <= LedLen)
		{
			/*Valid Length*/

			if((strcmp((char *)Local_Command->Payload,(char *)"effect1")) == 0)
			{
				/*Set the Led State by :  effect1*/
				LED_EffectState = effect1;
			}
			else if((strcmp((char *)Local_Command->Payload,(char *)"effect2")) == 0)
			{
				/*Set the Led State by :  effect2*/
				LED_EffectState = effect2;

			}
			else if((strcmp((char *)Local_Command->Payload,(char *)"effect3")) == 0)
			{
				/*Set the Led State by :  effect3*/
				LED_EffectState = effect3;

			}
			else if((strcmp((char *)Local_Command->Payload,(char *)"effect4")) == 0)
			{
				/*Set the Led State by :  effect4*/
				LED_EffectState = effect4;
			}
			else if((strcmp((char *)Local_Command->Payload,(char *)"none")) == 0)
			{
				/*Set the LedState by :  none(Stop all effects)*/
				LED_EffectState = none;

			}
			else
			{
				/* Invalid Option*/
				CurrentState = MainMenuState;
				xQueueSend(Q_Send,(void*)&InvalidMsgs[InvalidOptn],portMAX_DELAY);

			}

			/*Start the Effect*/
			AppLedEffectStart();

		}
		else
		{
			/*Invalid Length*/
			xQueueSend(Q_Send,(void*)&InvalidMsgs[InvalidLen],portMAX_DELAY);

		}

		/* Exchange the current State to Main-Menu */
		CurrentState = MainMenuState;

		/* Notify Main-Menu Task*/
		xTaskNotify(MainMenuTask,0,eNoAction);
	}
}
void AppRTCTaskHandler(void * Paramters)
{
	const char *MsgRTCMenu =
	{
			"\n------------------------------"
			"\n------------------------------"
			"\n---------- RTC Menu ----------"
			"\n------------------------------"
			"\n------------------------------"
	};
	const char *MsgRTCCfg ={
			"\nConfigure Time------> 0"
			"\nConfigure Date------> 1"
			"\nEnable Reporting----> 2"
			"\nExit----------------> 3"
			"\nEnter Your Choice---> "
	};

	const char *MsgDateCfgArr[] =
	{
			"\nEnter Date (1 -> 31):",
			"\nEnter Month (1 -> 12):",
			"\nEnter WeekDay (1 -> 7)(Sun:1):",
			"\nEnter Year (0 -> 99):"
	};

	const char *MsgTimeCfgArr[]=
	{
			"\nEnter Hour (1 -> 12):",
			"\nEnter Minutes (0 -> 59):",
			"\nEnter Seconds (0 -> 59):",
			"\nEnter Hour-Format (AM-->0, PM-->1):"
	};

	uint32_t Local_u32CommandAddr;

	RTC_Time_Elements Local_TimeCounter;
	RTC_Date_Elements Local_DateCounter;


	uint8_t Local_u8TimeCfgState,Local_u8DateCfgState,Local_u8Num;
	uint8_t Local_pTimeCfg[RTC_MAX_TIME_ELEM];
	uint8_t Local_pDateCfg[RTC_MAX_DATE_ELEM];

	Cmd_T *Local_Command;


	while(1)
	{
		/*Wait for Main-Menu Notification*/
		xTaskNotifyWait(0,0,NULL,portMAX_DELAY);

		/*Display RTC Menu*/
		xQueueSend(Q_Send,(void *)&MsgRTCMenu,portMAX_DELAY);

		/*Display the current time & date*/
		AppRTC_vShowTimeDate();

		/*Display RTC Options*/
		xQueueSend(Q_Send,(void *)&MsgRTCCfg,portMAX_DELAY);

		/* Wait for receiving the command address*/
		xTaskNotifyWait(0,0, (void *)&Local_u32CommandAddr, portMAX_DELAY);
		Local_Command = (Cmd_T *) Local_u32CommandAddr;

		/*Validate the input length*/
		if(Local_Command->Len == RTCMenuLen)
		{
			/*Valid Length*/

			switch(Local_Command->Payload[0])
			{
			case Configure_Time:

				/* Get Time Configurations */
				for(Local_TimeCounter = 0; Local_TimeCounter < RTC_MAX_TIME_ELEM; Local_TimeCounter++)
				{
					/*Display configuration options*/
					xQueueSend(Q_Send,(void *)&MsgTimeCfgArr[Local_TimeCounter],portMAX_DELAY);

					/* Wait for the command*/
					xTaskNotifyWait(0,0, (void *)&Local_u32CommandAddr,portMAX_DELAY);
					Local_Command = (Cmd_T *)Local_u32CommandAddr;

					/*Validate the input length*/
					if(Local_Command->Len <= RTCTimeLen)
					{
						/*Valid Length*/

						/* Convert input from string to number format*/
						AppRTC_vStringToNumber(Local_Command,&Local_u8Num);

						/*Validate the input range*/
						Local_u8TimeCfgState = AppRTC_u8TimeValidation(Local_TimeCounter,Local_u8Num);

						if(Local_u8TimeCfgState == StateValid)
						{
							/*Valid Option*/
							Local_pTimeCfg[Local_TimeCounter] = Local_u8Num;
						}
						else{
							/*Invalid Option*/
							CurrentState = MainMenuState;
							xQueueSend(Q_Send,(void*)&InvalidMsgs[InvalidOptn],portMAX_DELAY);
							break;
						}
					}
					else
					{
						/*Invalid Length*/
						Local_u8TimeCfgState = StateInvalid;

						CurrentState = MainMenuState;
						xQueueSend(Q_Send,(void*)&InvalidMsgs[InvalidLen],portMAX_DELAY);
						break;
					}
				}
				if(Local_u8TimeCfgState == StateValid)
				{
					/*All configurations are valid*/
					/* Set the time */
					AppRTC_vSetTime(Local_pTimeCfg);

					/*Display the current time & date*/
					AppRTC_vShowTimeDate();

				}
				break;

			case Configure_Date:

				/* Get Date Configurations */
				for(Local_DateCounter = 0; Local_DateCounter < RTC_MAX_DATE_ELEM; Local_DateCounter++)
				{
					/*Display configuration options*/
					xQueueSend(Q_Send,(void *)&MsgDateCfgArr[Local_DateCounter],portMAX_DELAY);

					/* Wait for the command*/
					xTaskNotifyWait(0,0, (void *)&Local_u32CommandAddr,portMAX_DELAY);
					Local_Command = (Cmd_T *)Local_u32CommandAddr;

					/*Validate the input length*/
					if(Local_Command->Len <= RTCDateLen)
					{
						/*Valid Length*/

						/* Convert input from string to number format*/
						AppRTC_vStringToNumber(Local_Command,&Local_u8Num);

						/*Validate the input range*/
						Local_u8DateCfgState = AppRTC_u8DateValidation(Local_DateCounter,Local_u8Num);

						if(Local_u8DateCfgState == StateValid)
						{
							/*Valid Option*/
							Local_pDateCfg[Local_DateCounter] = Local_u8Num;
						}
						else{
							/*Invalid Option*/
							CurrentState = MainMenuState;
							xQueueSend(Q_Send,(void*)&InvalidMsgs[InvalidOptn],portMAX_DELAY);
							break;
						}
					}
					else
					{
						/*Invalid Length*/
						Local_u8DateCfgState = StateInvalid;

						CurrentState = MainMenuState;
						xQueueSend(Q_Send,(void*)&InvalidMsgs[InvalidLen],portMAX_DELAY);
						break;
					}
				}
				if(Local_u8DateCfgState == StateValid)
				{
					/*All configurations are valid*/
					/* Set the time */
					AppRTC_vSetDate(Local_pDateCfg);

					/*Display the current time & date*/
					AppRTC_vShowTimeDate();

				}
				break;

			case Enable_Reporting:

				/*Start Reporting Timer*/
				(void)xTimerStart(HandleRTCReportTimer,portMAX_DELAY);

				break;
			case Exit_From_RTC:	break;
			default:
				/*Invalid Option*/
				CurrentState = MainMenuState;
				xQueueSend(Q_Send,(void*)&InvalidMsgs[InvalidOptn],portMAX_DELAY);

				break;
			}
		}
		else
		{
			/*Invalid Length*/
			Local_u8TimeCfgState = StateInvalid;

			CurrentState = MainMenuState;
			xQueueSend(Q_Send,(void*)&InvalidMsgs[InvalidLen],portMAX_DELAY);

		}

		/*Exchange the CurrentState to Main-Menu */
		CurrentState = MainMenuState;

		/*Notify Main-Menu Task*/
		xTaskNotify(MainMenuTask,0,eNoAction);

	}
}
