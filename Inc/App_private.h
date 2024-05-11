#ifndef INC_APP_PRIVATE_H_
#define INC_APP_PRIVATE_H_


/************************	Command Private Functions Prototypes	************************/

void ProcessCommand(Cmd_T *Copy_Command);
void ExtractCommand(Cmd_T *Copy_Command);

/************************	LEDs Private Functions Prototypes	************************/

void LedEffectStop(void);

void LedEffectAction1(void);
void LedEffectAction2(void);
void LedEffectAction3(void);
void LedEffectAction4(void);

void ToggleEvenOddLeds(void);
void ShiftRight(void);
void ShiftLeft(void);

/************************	Private Enums	************************/

typedef enum
{
	TimeIsSet,
	TimeFailed,
	DateIsSet,
	DateFailed,


}SuccessMsgIndex_T;

typedef enum
{
	MainMenuLen = 1,
	LedLen		= 7,
	RTCMenuLen	= 1,
	RTCTimeLen	= 2,
	RTCDateLen  = 2

}InputLen_T;

typedef enum
{
	InvalidLen,
	InvalidOptn,

}InvalidMsgIndex_T;

#endif /* INC_APP_PRIVATE_H_ */
