#include <avr/sleep.h>
#include <avr/power.h>
#include <avr/wdt.h>
#include <SoftwareSerial.h>

#define LED_PIN (13)
#define MOTOR_PIN_1 (12)
#define MOTOR_PIN_2 (8)
#define XB_TX (3)
#define XB_RX (4)
#define SLEEP_PIN (7)
#define MOTOR_TIME (200)

volatile int f_wdt = 1;
SoftwareSerial XBee(XB_TX, XB_RX);
int currentstate = -1;


void setup() {
  // put your setup code here, to run once:

  //Serial.begin(9600);
  //Serial.println("Power On...");
  //delay(100); 
  
  XBee.begin(9600);
  
  //setup pins
  pinMode(MOTOR_PIN_1, OUTPUT);
  pinMode(MOTOR_PIN_2, OUTPUT);
  //pinMode(SLEEP_PIN, OUTPUT);
  
  //commands to enable watchdog timer and power-down mode for 8 seconds
  //MCUSR &= ~(1 << WDRF);
  //WDTCSR |= (1 << WDCE) | (1 << WDE);
  //WDTCSR = 1 << WDP0 | 1 << WDP3;
  //WDTCSR |= _BV(WDIE);
  //Serial.println("Initialization Complete");
  //delay(100); //Allow for serial print to complete.

  //commands to enable power mode for xbee controller
  //digitalWrite(SLEEP_PIN, LOW);   // deassert to keep radio awake when sleep mode selected
  /* 
   if(atCommand("D7", 1) | atCommand("SM", 1)) {
     Serial.println("Unable to configure Xbee with power saving profile");
   }
   */

}

void motorcontrol(int dir, int time)
{
  switch (dir)
  {
    case 0:
      digitalWrite(MOTOR_PIN_1, LOW);
      digitalWrite(MOTOR_PIN_2, LOW);
      break;
    case 1:
      digitalWrite(MOTOR_PIN_1, HIGH);
      digitalWrite(MOTOR_PIN_2, LOW);
      delay (time);
      digitalWrite(MOTOR_PIN_1, LOW);
      digitalWrite(MOTOR_PIN_2, LOW);
      break;
    case 2:
      digitalWrite(MOTOR_PIN_1, LOW);
      digitalWrite(MOTOR_PIN_2, HIGH);
      delay (time);
      digitalWrite(MOTOR_PIN_1, LOW);
      digitalWrite(MOTOR_PIN_2, LOW);
      break;
  }

}

ISR(WDT_vect)
{
  if (f_wdt == 0)
  {
    f_wdt = 1;
  }
  else
  {
    // Serial.println("WDT Overrun!!!");
    //delay(100); //Allow for serial print to complete.
  }
}


void loop() {
  //sleep_disable(); 
  //power_all_enable();
  //digitalWrite(SLEEP_PIN, LOW);
  
  //Serial.println("Waking up...");
  //Serial.println(currentstate);
  //delay(10000); 

  if (XBee.available())
  {
    String data;
    char character;
    while (XBee.available())
    {
      character=XBee.read();
      data.concat(character); 
    }
    
    //if (data != "") {
      //Serial.print("Received:");
      //Serial.println(data);
      //delay(100);
    //}
           
    data.trim();
    if (data.equals("open")){   
      if (currentstate==0){ 
        //Serial.println("opening blinds");
        //delay(100);
        motorcontrol(1,MOTOR_TIME);
        currentstate=1;
      }
      else{
          if (currentstate==-1){
            //Serial.println("opening blinds");
            //delay(100);
            motorcontrol(1,MOTOR_TIME);
            currentstate=1; 
          }
          else{
            //Serial.println("Blinds are already open");
            //delay(100);
          }
      }   
    }
    
    if (data.equals("closed")){
      if (currentstate==1){
        //Serial.println("closing blinds");
        //delay(100);
        motorcontrol(2,MOTOR_TIME); 
        currentstate=0;
      }
      else{
          if (currentstate==-1){
           //Serial.println("closing blinds");
           //delay(100);
           motorcontrol(2,MOTOR_TIME); 
           currentstate=0;
          }
          else{
            //Serial.println("Blinds are already closed");
            //delay(100);
          }
      }
    }
   }  
  
  //digitalWrite(SLEEP_PIN, HIGH);
  //set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  //sleep_enable();
  //Serial.println("Sleeping...");
  //delay(100); 
  
  // sleep for a total of 240 seconds
  //int i;
  //for (i = 0; i <=10; i++)
  //{  
  //  sleep_mode();  // 8 seconds
  //}
  
}
