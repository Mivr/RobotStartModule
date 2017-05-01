#include "StarterRemote.h"
#include "Arduino.h"

StarterRemote::StarterRemote(int pin)
{
    this->pin = pin;
}

void StarterRemote::sendStartCommand(int address)
{
    this->sendCommandToModule((RC5_ADR_START_STOP << 6) | (address + 1));
}

void StarterRemote::sendStopCommand(int address)
{
    this->sendCommandToModule((RC5_ADR_START_STOP << 6) | address);
}

void StarterRemote::sendReprogramCommand(int newAddress)
{
    this->sendCommandToModule((RC5_ADR_PROGRAMMING << 6) | newAddress);
}

void StarterRemote::sendCommandToModule(int dataToSend)
{
    // send the start sequence first
    delayMicroseconds(889);
    for(int i =0; i<32; i++)sendTickIn38Khz();
    delayMicroseconds(889);
    for(int i =0; i<32; i++)sendTickIn38Khz();
    for(int i =0; i<32; i++)sendTickIn38Khz();
    delayMicroseconds(889);

    // send the data and address next
    for(int i = 10; i >= 0; i--)
    {
        // check on whtere 1 or 0 should be send next
        if((dataToSend >> i) & 0x01)
        {
            // 1 is defined as LOW to HIGH transition
            
            // LOW part we wait for 889 microseconds (as to RC-5 protocol)
            delayMicroseconds(889);
            
            // HIGH part -> we send ticks (32 as to specification of RC-5)
            for(int i =0; i<32; i++)sendTickIn38Khz();
        }
        else
        {
            // 0 is defined as HIGH to LOW transition

            // HIGH part -> we send ticks (32 as to specification of RC-5)
            for(int i =0; i<32; i++)sendTickIn38Khz();

            // LOW part we wait for 889 microseconds (as to RC-5 protocol)
            delayMicroseconds(889);
        }
    }
}

// a simple function for sending a single tick on 38KHz frequency utilizing IO control and delay microseconds commands
void StarterRemote::sendTickIn38Khz()
{
  // 30% fill on 26us impulses
  digitalWrite(this->pin, HIGH);
  delayMicroseconds(8);
  digitalWrite(this->pin, LOW);
  delayMicroseconds(15);
}