#ifndef STARTER_REMOTE_H
#define STARTER_REMOTE_H

#include "Arduino.h"

#define RC5_ADR_PROGRAMMING     0x0B
#define RC5_ADR_START_STOP      0x07

class StarterRemote
{
    public:
        StarterRemote(int pin);
        int pin;
        void sendStartCommand(int address);
        void sendStopCommand(int address);
        void sendReprogramCommand(int newAddress);
    private:
        void sendCommandToModule(int dataToSend);
        void sendTickIn38Khz();
};

#endif