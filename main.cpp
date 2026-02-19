//=====[Libraries]=============================================================

#include "mbed.h"
#include "arm_book_lib.h"
#include <cstring>

//=====[Declaration and initialization of public global objects]===============

DigitalIn enterButton(BUTTON1);
DigitalIn gasDetector(D2);
DigitalIn sendgaspc(D3);
DigitalIn sendtemppc(D4);
DigitalIn overTempDetector(D5);
DigitalIn alarmreset(D6);
DigitalIn startmonotoring(D7);

UnbufferedSerial uartUsb(USBTX, USBRX, 115200);

//=====[Declaration and initialization of public global variables]=============

bool gasalarmState = OFF;
bool overtempalarmState = OFF;
int numberOfIncorrectCodes = 0;

//=====[Declarations (prototypes) of public functions]=========================

void inputsInit();
void detectprompt();
void alarmActivationUpdate();
void alarmDeactivationUpdate();
void uartTask();
void availableCommands();

//=====[Main function, the program entry point after power on or reset]========

int main()
{
    inputsInit();
    while (true) {
        alarmActivationUpdate();
        alarmDeactivationUpdate();
        detectprompt();
        uartTask();
    }
}

//=====[Implementations of public functions]===================================

void inputsInit()
{
    gasDetector.mode(PullDown);
    overTempDetector.mode(PullDown);
    sendgaspc.mode(PullDown);
    sendtemppc.mode(PullDown);
    alarmreset.mode(PullDown);
    startmonotoring.mode(PullDown);
}


void alarmActivationUpdate()
{
    if ( gasDetector) {
        ThisThread::sleep_for(300ms);
        gasalarmState = ON;
        uartUsb.write("The gas alarm has been activated\r\n",
                              strlen("The gas alarm has been activated\r\n"));

    }

    if(overTempDetector){
        ThisThread::sleep_for(300ms);
        overtempalarmState = ON;
        uartUsb.write("The over temp alarm has been activated\r\n",
                              strlen("The over temp alarm has been activated\r\n"));
    }


}

void alarmDeactivationUpdate()
{
if (alarmreset){
    ThisThread::sleep_for(300ms);
    gasalarmState = OFF;
    overtempalarmState = OFF;
    uartUsb.write("The alarms have been reset\r\n",
                              strlen("The alarms have been reset\r\n"));
}
}

void detectprompt(){
    if (sendgaspc && gasalarmState){
        ThisThread::sleep_for(300ms);
        uartUsb.write("The gas alarm is activated\r\n",
                              strlen("The gas alarm is activated\r\n"));                       
    }
    if (sendgaspc && !gasalarmState){
        ThisThread::sleep_for(300ms);
        uartUsb.write("The gas alarm is not activated\r\n",
                              strlen("The gas alarm is not activated\r\n"));
    }
    if (sendtemppc && overtempalarmState){
        ThisThread::sleep_for(300ms);
        uartUsb.write("The overtemp alarm is activated\r\n",
                              strlen("The overtemp alarm is activated\r\n"));                       
    }
    if (sendtemppc && !overtempalarmState){
        ThisThread::sleep_for(300ms);
        uartUsb.write("The overtemp alarm is not activated\r\n",
                              strlen("The overtemp alarm is not activated\r\n"));
    }
}

void uartTask()
{
    char receivedChar = '\0';

    if (uartUsb.readable()) {
        uartUsb.read(&receivedChar, 1);

        if (receivedChar == '1') {

            if (gasalarmState) {
                uartUsb.write("The gas alarm is activated\r\n",
                              strlen("The gas alarm is activated\r\n"));
            } else {
                uartUsb.write("The gas alarm is not activated\r\n",
                              strlen("The gas alarm is not activated\r\n"));
            }

            if (overtempalarmState) {
                uartUsb.write("The overtemp alarm is activated\r\n",
                              strlen("The overtemp alarm is activated\r\n"));
            } else {
                uartUsb.write("The overtemp alarm is not activated\r\n",
                              strlen("The overtemp alarm is not activated\r\n"));
            }

        } else {
            availableCommands();
        }
    }
}
void availableCommands()
{
    uartUsb.write( "Available commands:\r\n", 21 );
    uartUsb.write( "Press '1' to get the alarm state\r\n\r\n", 36 );
}
