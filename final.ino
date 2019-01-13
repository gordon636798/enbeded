#include "pitches.h"
#include<SoftwareSerial.h>
#include<Wire.h>
byte speakerPin = A0;
byte ledPin[8] = {4,5,6,7,8,9,10,11};
short baseTone[8] = {NOTE_C4,NOTE_D4,NOTE_E4,NOTE_F4,NOTE_G4,
NOTE_A4,NOTE_B4,NOTE_C4}; //基本do re mi fa so la si do7
SoftwareSerial BTSerial(12,13);
short *tempTone; //暫存樂譜
byte *tempDuration; //暫存拍子
void Select(int R) //選擇樂譜 將樂譜放入暫存樂譜
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
void press(byte pin) //按下keyboard上的動作 按下後一直響直到放開
{
  digitalWrite(pin,1);
  while(1)  //使用while卡住 直到收到放開訊號
  {
    if(BTSerial.read() == 'S') 
      break;    
  }
  digitalWrite(pin,0);
}

void game() //讀取樂譜根據樂譜亮出提示LED的燈 按下對應key後繼續下一段
{
    
    int n=0;
    while(bee[n]>0)
    {   
        byte t = bee[n];
        digitalWrite(t+3,1);
        char k;
        while (1) //使用while卡住 直到收到放開訊號
        {
           k = BTSerial.read();
          if (k-'0'==t) //使用0~7代表 Do~Do7
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
          if(k == 'q') //收到停止訊號後停止
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

bool start = false; //音樂盒開始狀態
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
    for(int i=0;i<8;i++) //音樂盒 音樂響起開始閃爍
      digitalWrite(ledPin[i],random(0,2));
   R= BTSerial.read(); //藍芽收訊
   /* Serial.print(tempTone[thisNote]);
    Serial.println(tempDuration[thisNote]);*/
    if(R=='q') //收到停止訊號停止
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
