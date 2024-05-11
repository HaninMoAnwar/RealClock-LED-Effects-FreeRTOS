# RealClock FreeRTOS LED Effects

This project showcases a RealClock system with LED effects implemented using FreeRTOS. The Nucleo-f446re microcontroller is utilized, along with a user interface program using UART.

## Project Features
- Choose from four different LED effects for four LEDs.
- Set a new time or date.
- Enable time and date reporting, sent to SWV (Serial Wire Viewer) every 1 second.

## Project FlowChart 
![SystemFlowChart](https://github.com/HaninMoAnwar/RealClock-LED-Effects-FreeRTOS/assets/169531536/cfed724c-bc6a-4a58-8390-c36070bc441e)

## FreeRTOS Task Design
### Tasks:
- **Command Handle Task**: Responsible for receiving user input over UART.
- **Display Task**: Responsible for displaying messages or menus to the user over UART.
- **Main-menu Task**: Allows the user to choose system functionalities such as LED Effects and RTC configuration.
- **LED Effect Task**: Starts the selected LED effect.
- **RTC Task**: Handles setting the new time and date and enabling reporting.

### Queues
- **Q_Send**: Sends data to UART.
- **Q_Receive**: Receives data from UART.

### Tasks And Interrupt FlowChart
![TasksAndINTFlow](https://github.com/HaninMoAnwar/RealClock-LED-Effects-FreeRTOS/assets/169531536/bafb9c40-2a3e-4138-bcb4-10ea7d846ef6)

### Tasks State Machine Diagrams
#### Command Handle Task
![CommandTaskSM](https://github.com/HaninMoAnwar/RealClock-LED-Effects-FreeRTOS/assets/169531536/fbfd7c2f-196f-43a3-af75-514da6e40233)

#### Display Task
![DisplayTaskSM](https://github.com/HaninMoAnwar/RealClock-LED-Effects-FreeRTOS/assets/169531536/73cdb36c-f2ef-4b08-b486-10ed1028e6ad)

#### Main-Menu Task
![MainMenuTaskSM](https://github.com/HaninMoAnwar/RealClock-LED-Effects-FreeRTOS/assets/169531536/10dd2e2e-0dc7-4b0d-85b9-44756c9712c9)

#### LED Effect Task
![LEDsTaskSM](https://github.com/HaninMoAnwar/RealClock-LED-Effects-FreeRTOS/assets/169531536/582bde7c-1025-4447-b9a1-3472b5afaca7)

#### RTC Task
![RTCTaskSM](https://github.com/HaninMoAnwar/RealClock-LED-Effects-FreeRTOS/assets/169531536/fdb00477-bc82-4785-8f91-3fa15d106444)

## Used Peripherals
The project is based on CubeMX HAL Drivers and utilizes the following peripherals:
- UART
= RTC
- GPIO
- Systick (FreeRTOS clock base)

## Future Improvements
- Set alarms.
-  Contribute to the project with SEGGER tool.
