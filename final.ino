#include "pitches.h"
#include<SoftwareSerial.h>
#include<Wire.h>
byte speakerPin = A0;
byte ledPin[8] = {4,5,6,7,8,9,10,11};
short baseTone[8] = {NOTE_C4,NOTE_D4,NOTE_E4,NOTE_F4,NOTE_G4,
NOTE_A4,NOTE_B4,NOTE_C4};
SoftwareSerial BTSerial(12,13);
short *tempTone;
byte *tempDuration;
void Select(int R)
{
   if(R == 'a')
   {
   // Serial.println(R);
     tempTone= CanonTone;
     tempDuration= noteDurations;
   }
   if(R == 's')
   {
    //Serial.println(R);
     tempTone= sakura;
     tempDuration= Sd;
   }
}
void press(byte pin)
{
  digitalWrite(pin,1);
  while(1) 
  {
    if(BTSerial.read() == 'S') 
      break;    
  }
  digitalWrite(pin,0);
}

void game()
{
    
    int n=0;
    while(bee[n]>0)
    {   
        byte t = bee[n];
        digitalWrite(t+3,1);
        char k;
        while (1)
        {
           k = BTSerial.read();
          if (k-'0'==t)
          {
            digitalWrite(t+3,0);
            tone(A0,baseTone[t]);
            while(1) 
            {
              if(BTSerial.read() == 'S') 
                break;    
            }  
            noTone(A0);
            break;
          }
          if(k == 'q')
          {
            digitalWrite(t+3,0); 
            return;
          }
        }
        //digitalWrite(t+3,0);
       //{ tone(A0,baseTone[t]); press();  noTone(A0);}
        n++;

    }
}
void setup() {  
    pinMode(speakerPin, OUTPUT);
    Serial.begin(9600);
    BTSerial.begin(9600);
    for(byte i = 0 ; i < 6; i++)
      pinMode(ledPin[i],OUTPUT);

  }  

bool start = false;
void loop() {  
   bool start = false;
   int thisNote = 0;  // 在此使用While 而不用 for
   char R ;
   R=BTSerial.read();
   //Serial.println(R);
   if(R == '1') { tone(A0,NOTE_C4); press(ledPin[0]);  noTone(A0);}
   if(R == '2') { tone(A0,NOTE_D4); press(ledPin[1]);  noTone(A0);}
   if(R == '3') { tone(A0,NOTE_E4); press(ledPin[2]);  noTone(A0);}
   if(R == '4') { tone(A0,NOTE_F4); press(ledPin[3]);  noTone(A0);}
   if(R == '5') { tone(A0,NOTE_G4); press(ledPin[4]);  noTone(A0);}
   if(R == '6') { tone(A0,NOTE_A4); press(ledPin[5]);  noTone(A0);}
   if(R == '7') { tone(A0,NOTE_B4); press(ledPin[6]);  noTone(A0);}
   if(R == '8') { tone(A0,NOTE_C5); press(ledPin[7]);  noTone(A0);}
   if(R == 'a') {Select(R); start = true;}
   if(R == 's') {Select(R); start = true;}
   if(R == 'd') game();
   if(start)
   while(tempTone[thisNote] >0){  // 當CanonTone 內的值 > 0 時 則
    // toneSpeed / 4 當四分音符
    // / 8 當 八分音符  
    // .  
    for(int i=0;i<8;i++)
      digitalWrite(ledPin[i],random(0,2));
   R= BTSerial.read();
   /* Serial.print(tempTone[thisNote]);
    Serial.println(tempDuration[thisNote]);*/
    if(R=='q')
      {   
        start = false;
        break;
      }
    int noteDuration = toneSpeed/int(tempDuration[thisNote]);  
    tone(speakerPin, tempTone[thisNote],noteDuration);  
    // delay 1.3 偣的音長
    // 
    int pauseBetweenNotes = noteDuration *1.30;  
    delay(pauseBetweenNotes);  
    // 準備下一個音調 
    thisNote++;  
   }  
   for(int i=0;i<8;i++)
      digitalWrite(ledPin[i],0);
   noTone(9);  // 播完後靜音
   start = false;

}  
