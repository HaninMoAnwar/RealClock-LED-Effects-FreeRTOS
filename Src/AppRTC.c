#include "main.h"
#include "App_Interface.h"
#include "App_config.h"
#include "App_private.h"

extern RTC_HandleTypeDef hrtc;

extern UART_HandleTypeDef huart2;

extern  QueueHandle_t Q_Send;

const char *RTCSuccessMsgs[4] = {
		"\n===============		Time Is Set Successfully	===============",
		"\n===============	Couldn't Set The Time, Please Try Again ===============",
		"\n===============		Date Is Set Successfully	===============",
		"\n===============	Couldn't Set The Date, Please Try Again ===============",
};
uint8_t AppRTC_u8TimeValidation(RTC_Time_Elements Copy_TimeElement, uint8_t Copy_u8Num)
{
	uint8_t Local_u8State;

	switch(Copy_TimeElement)
	{
	case RTC_HOURS:
		if((Copy_u8Num < 1) || (Copy_u8Num > 12))
		{
			/*Invalid Option*/
			Local_u8State = StateInvalid;
		}
		else
		{
			/*valid Option*/
			Local_u8State = StateValid;
		}
		break;
	case RTC_MINUTES:
	case RTC_SECONDS:
		if((Copy_u8Num < 0) || (Copy_u8Num > 59))
		{
			/*Invalid Option*/
			Local_u8State = StateInvalid;
		}
		else
		{
			/*valid Option*/
			Local_u8State = StateValid;
		}
		break;
	case RTC_AM_PM:
		if(Copy_u8Num > 1)
		{
			/*Invalid Option*/
			Local_u8State = StateInvalid;
		}
		else
		{
			/*valid Option*/
			Local_u8State = StateValid;
		}
		break;

	default:
		/*Invalid Option*/
		Local_u8State = StateInvalid;
		break;
	}

	return Local_u8State;
}
uint8_t AppRTC_u8DateValidation(RTC_Date_Elements Copy_Datelement, uint8_t Copy_u8Num)
{
	uint8_t Local_u8State;

	switch(Copy_Datelement)
	{
	case RTC_DATE:
		if((Copy_u8Num < 1) || (Copy_u8Num > 31))
		{
			/*Invalid Option*/
			Local_u8State = StateInvalid;
		}
		else
		{
			/*valid Option*/
			Local_u8State = StateValid;
		}
		break;
	case RTC_MONTH:
		if((Copy_u8Num < 1) || (Copy_u8Num > 12))
		{
			/*Invalid Option*/
			Local_u8State = StateInvalid;
		}
		else
		{
			/*valid Option*/
			Local_u8State = StateValid;
		}
		break;
	case RTC_YEAR:
		if((Copy_u8Num < 0) || (Copy_u8Num > 99))
		{
			/*Invalid Option*/
			Local_u8State = StateInvalid;
		}
		else
		{
			/*valid Option*/
			Local_u8State = StateValid;
		}
		break;
	case RTC_DAY:
		if((Copy_u8Num < 1) || (Copy_u8Num > 7))
		{
			/*Invalid Option*/
			Local_u8State = StateInvalid;
		}
		else
		{
			/*valid Option*/
			Local_u8State = StateValid;
		}
		break;

	default:
		/*Invalid Option*/
		Local_u8State = StateInvalid;

		break;
	}

	return Local_u8State;

}
void AppRTC_vStringToNumber(Cmd_T * Copy_Command,uint8_t *Copy_pu8Num)
{
	if(Copy_Command->Len == 2)
	{
		*Copy_pu8Num = (Copy_Command->Payload[0] - '0')*10 + Copy_Command->Payload[1] - '0';

	}
	else
	{
		*Copy_pu8Num = Copy_Command->Payload[0] - '0' ;

	}
}
void AppRTC_vShowTimeDate(void)
{
	RTC_TimeTypeDef RTC_CurrentTime;
	RTC_DateTypeDef RTC_CurrentDate;
	char *TimeFormat;
	static char DisplayTime[50];
	static char DisplayDate[50];

	char * MsgCurrentTime = DisplayTime;
	char * MsgCurrentDate  = DisplayDate;

	HAL_RTC_GetTime(&hrtc, &RTC_CurrentTime, RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&hrtc, &RTC_CurrentDate, RTC_FORMAT_BIN);

	if(RTC_CurrentTime.TimeFormat == RTC_HOURFORMAT12_AM)
	{
		TimeFormat = "AM";
	}
	else
	{
		TimeFormat = "PM";
	}

	sprintf((char *)DisplayTime,"%s  %02d : %02d : %02d[%s]",
			"\nCurrent Time & Date",RTC_CurrentTime.Hours,RTC_CurrentTime.Minutes,RTC_CurrentTime.Seconds,TimeFormat
	);

	sprintf((char *)DisplayDate,"\t%02d / %02d / %02d",
			RTC_CurrentDate.Date,RTC_CurrentDate.Month,(2000+RTC_CurrentDate.Year));

	xQueueSend(Q_Send,(void *)&MsgCurrentTime,portMAX_DELAY);
	xQueueSend(Q_Send,(void *)&MsgCurrentDate,portMAX_DELAY);
}
void AppRTC_vSetTime(uint8_t * Copy_pu8TimeCfg)
{
	RTC_TimeTypeDef Local_RTC_Time ;
	HAL_StatusTypeDef Local_RTCState;

	Local_RTC_Time.Hours = Copy_pu8TimeCfg[RTC_HOURS];
	Local_RTC_Time.Minutes = Copy_pu8TimeCfg[RTC_MINUTES];
	Local_RTC_Time.Seconds = Copy_pu8TimeCfg[RTC_SECONDS];
	Local_RTC_Time.TimeFormat = Copy_pu8TimeCfg[RTC_AM_PM];

	Local_RTCState = HAL_RTC_SetTime(&hrtc, &Local_RTC_Time, RTC_FORMAT_BIN);

	if(Local_RTCState == HAL_OK)
	{
		/* Time is set */
		xQueueSend(Q_Send, (void *)&RTCSuccessMsgs[TimeIsSet], portMAX_DELAY);
	}
	else
	{
		/* Time failed */
		xQueueSend(Q_Send, (void *)&RTCSuccessMsgs[TimeFailed], portMAX_DELAY);
	}

}
void AppRTC_vSetDate(uint8_t * Copy_pu8DateCfg)
{
	RTC_DateTypeDef Local_RTC_Date ;
	HAL_StatusTypeDef Local_RTCState;

	Local_RTC_Date.Date= Copy_pu8DateCfg[RTC_DATE];
	Local_RTC_Date.Month = Copy_pu8DateCfg[RTC_MONTH];
	Local_RTC_Date.Year = Copy_pu8DateCfg[RTC_YEAR];
	Local_RTC_Date.WeekDay = Copy_pu8DateCfg[RTC_DAY];

	Local_RTCState = HAL_RTC_SetDate(&hrtc, &Local_RTC_Date, RTC_FORMAT_BIN);

	if(Local_RTCState == HAL_OK)
	{
		/* Time is set */
		xQueueSend(Q_Send, (void *)&RTCSuccessMsgs[DateIsSet], portMAX_DELAY);
	}
	else
	{
		/* Time failed */
		xQueueSend(Q_Send, (void *)&RTCSuccessMsgs[DateFailed], portMAX_DELAY);
	}
}

void AppRTCReportCallBackFunc(TimerHandle_t xTimer)
{
	AppRTC_vShowReportTimeDate();
}
void AppRTC_vShowReportTimeDate(void)
{
	RTC_TimeTypeDef RTC_CurrentTime;
		RTC_DateTypeDef RTC_CurrentDate;

		char *TimeFormat;

		HAL_RTC_GetTime(&hrtc, &RTC_CurrentTime, RTC_FORMAT_BIN);
		HAL_RTC_GetDate(&hrtc, &RTC_CurrentDate, RTC_FORMAT_BIN);

		if(RTC_CurrentTime.TimeFormat == RTC_HOURFORMAT12_AM)
		{
			TimeFormat = "AM";
		}
		else
		{
			TimeFormat = "PM";
		}

		printf("\nCurrent Time & Date  %02d : %02d : %02d[%s]",RTC_CurrentTime.Hours,RTC_CurrentTime.Minutes,RTC_CurrentTime.Seconds,TimeFormat);

		printf("\t%02d / %02d / %02d",RTC_CurrentDate.Date,RTC_CurrentDate.Month,(2000+RTC_CurrentDate.Year));


}
