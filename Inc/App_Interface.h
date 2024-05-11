#ifndef INC_APP_INTERFACE_H_
#define INC_APP_INTERFACE_H_

#define CMD_LINE_FINISH_CHAR		'\r'

#define NUM_LEDS		4


typedef struct
{
	uint8_t Payload[10];
	uint8_t Len;
}Cmd_T;

typedef enum
{
	MainMenuState,
	LedMenuState,
	RTCMenuState,
}State_T;

typedef enum
{
	LED_Effect 			= '0',
	Time_Date  			= '1',
	Exit_From_App       = '2',
	INVILD_MainMenu		= '3'
}MainMenu_T;

typedef enum
{
	effect1 ,
	effect2 ,
	effect3 ,
	effect4 ,
	none

}LEDMenu_T;

typedef enum
{
	Configure_Time	= '0',
	Configure_Date	= '1',
	Enable_Reporting= '2',
	Exit_From_RTC	= '3',
	INVILD_Config  	= '4'
}RTCMenu_T;



typedef enum
{
	RTC_HOURS,
	RTC_MINUTES,
	RTC_SECONDS,
	RTC_AM_PM,
	RTC_MAX_TIME_ELEM,

}RTC_Time_Elements;
typedef enum
{
	RTC_DATE,
	RTC_MONTH,
	RTC_DAY,
	RTC_YEAR,
	RTC_MAX_DATE_ELEM
}RTC_Date_Elements;


typedef enum
{
	StateValid,
	StateInvalid

}ValidState_T;

/************************	Tasks Handlers Prototypes	************************/

void AppCmdHandler(void * Paramters);
void AppDisplayHandler(void * Paramters);
void AppMainMenuHandler(void * Paramters);
void AppLEDsEffectHandler(void * Paramters);
void AppRTCTaskHandler(void * Paramters);


void AppLedEffectStart(void);

void AppLedEffectCallBackFunc(TimerHandle_t xTimer);

uint8_t AppRTC_u8TimeValidation(RTC_Time_Elements Copy_TimeElement, uint8_t Copy_u8Num);
uint8_t AppRTC_u8DateValidation(RTC_Date_Elements Copy_Datelement, uint8_t Copy_u8Num);

void AppRTC_vStringToNumber(Cmd_T * Copy_Command,uint8_t *Copy_pu8Num);
void AppRTC_vShowTimeDate(void);
void AppRTC_vSetTime(uint8_t * Copy_pu8TimeCfg);
void AppRTC_vSetDate(uint8_t * Copy_pu8DateCfg);

void AppRTCReportCallBackFunc(TimerHandle_t xTimer);
void AppRTC_vShowReportTimeDate(void);


#endif /* INC_APP_INTERFACE_H_ */
