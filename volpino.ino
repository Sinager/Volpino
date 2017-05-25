/*
## VOLPINO Arduino + DDS ARDF transmitter
## version 1.0 - Sep 2016
## (c) Stefano "Sinager" Sinagra IZ0MJE
## homnepage  www.tarapippo.net
## released under GNU/GPL license
## http://www.gnu.org/licenses/gpl.txt
Includes code stolen from:
--- Radio Artisan
http://blog.radioartisan.com/analog-devices-dds-arduino-library/
and
--- Simple CW Beacon by Charlie Davy M0PZT  www.m0pzt.com
http://www.m0pzt.com/arduino_beacon.txt
*/

#include <stdio.h>
#include <dds.h>

int pinPTT = 4;
int MorseLEDPin = 13;
int MorseTonePin = 5;
int MorseToneFreq = 800;
int MorseWPM = 16;

// Declare variables...
#define strBeacondID "MOE"
long frequency = 3580000LL;
long offset = 51000000LL;

// Bung Morse into an array
char* MorseCodeCharacters[] = {
"A", ".-", 
"B","-...", 
"C","-.-.", 
"D","-..", 
"E",".", 
"F","..-.", 
"G","--.", 
"H","....", 
"I","..", 
"J",".---", 
"K","-.-", 
"L",".-..", 
"M","--", 
"N","-.", 
"O","---", 
"P",".--.", 
"Q","--.-", 
"R",".-.", 
"S","...", 
"T","-", 
"U","..-", 
"V","...-", 
"W",".--", 
"X","-..-", 
"Y","-.--",
"Z","--..",
"0","-----", 
"1",".----", 
"2","..---",
"3","...--",
"4","....-",
"5",".....",
"6","-....",
"7","--...",
"8","---..",
"9","----.",
"/","-..-.",
};

// chip, data_pin, load_pin, clock_pin, clock_hz
dds ddschip(DDS9850, 10, 11, 12, 125000000LL);

void setup()
{
 
  ddschip.calibrate(0.0000495);
  pinMode(pinPTT, OUTPUT);
  pinMode(MorseLEDPin, OUTPUT);
  pinMode(MorseTonePin, OUTPUT);

}


void loop()
{
  ddschip.setfrequency(3580000LL);
  digitalWrite(pinPTT, HIGH);
  TransmitMorse(strBeacondID, MorseWPM, MorseToneFreq); 
  digitalWrite(pinPTT, LOW);
  delay(2000);

}





void TransmitMorse(String MorseString, int MorseWPM,int MorseToneFreq) {   
  int CWdot = 1200 / MorseWPM;
  int CWdash = (1200 / MorseWPM)*3;
  int istr;
  for (istr=0; istr < MorseString.length(); istr++) {
    String currentchar = MorseString.substring(istr,istr+1);
    int ichar = 0;
    while(ichar < sizeof(MorseCodeCharacters)) {
      String currentletter = MorseCodeCharacters[ichar];   // letter
      String currentcode = MorseCodeCharacters[ichar+1];   // corresponding morse code
      if (currentletter.equalsIgnoreCase(currentchar)) {
        int icp=0;
        for (icp=0;icp < currentcode.length(); icp++) {
          // Transmit Dit
          if (currentcode.substring(icp,icp+1).equalsIgnoreCase(".")) { 
            ddschip.setfrequency(frequency);
            digitalWrite(MorseLEDPin, HIGH);
            tone(MorseTonePin, MorseToneFreq);
            delay(CWdot);
            ddschip.setfrequency(offset);
            digitalWrite(MorseLEDPin, LOW);
            noTone(MorseTonePin);
            delay(CWdot);
          }
          // Transmit Dah
          else if (currentcode.substring(icp,icp+1).equalsIgnoreCase("-")) {
            ddschip.setfrequency(frequency);
            digitalWrite(MorseLEDPin, HIGH);
            tone(MorseTonePin, MorseToneFreq);
            delay(CWdash);
            ddschip.setfrequency(offset);
            digitalWrite(MorseLEDPin, LOW);
            noTone(MorseTonePin);
            delay(CWdot); 
          }
          else if (currentcode.substring(icp,icp+1).equalsIgnoreCase(" ")) {
            delay(CWdot*3);
          };
          }
        }
        ichar=ichar+2;
     }
      delay(CWdot*3);
      }    
    delay(CWdot*7);
  }


