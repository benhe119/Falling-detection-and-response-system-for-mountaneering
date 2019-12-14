/*
 * Arduino Board PINOUT:
 * 
 * Bluetooth:
 *  -3'3V2
 *  -PIN 14 TX3: 3rd PIN Bluetooth (green)
 *  -PIN 15 RX3: 2nd PIN Bluetooth (yellow)
 * 
 * GPS:
 * -5V (5v to the breadboard
 *  -PIN 18 TX1: PIN RX GPS
 *  -PIN 19 RX1: PIN TX GPS
 * 
 * SIM800L:
 * 
 * 
 * Accelerometer ADXL345:
 * GND-->GND
 * VCC-->3.3v
 * CS--> 3.3V
 * SDA-->PIN 20 (SDA)
 * SCL--> PIN 21 (SCL)
 * http://adam-meyer.com/arduino/ADXL345
 * 
 * 
 * Brunzer:
 * -PIN 8
 * 
 * Buttons:
 * PINS 9 i 10
 * 
 * LEDs:
 * Green: 12
 * Red: 13
 * 
 */
 

/*LIBRARIES*/
#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <Wire.h>
#include <ADXL345.h>


/*Free falling calibration*/
#define FREEFALL_THRESHOLD 7 //(5 - 9) recommended - 62.5mg per increment
#define FREEFALL_DURATION 4 //(20 - 70) recommended - 5ms per increment


/*****************COMMUNICATION ARDUINO BOARD <----> APP  Protocol***************/
/*Phone to Board*/
#define NAME 1
#define EMER_NUMBER 2
#define BLOOD_TYPE 3
#define SIMULACRE_BUTTON 10
#define STOP_EMERGENCY 15
#define DETECT_FALLING_ON 20
#define DETECT_FALLING_OFF 30
#define SMS_SIMULATION 500

/*Board to Phone*/
#define FALLING_DETECTED 404
#define I_WILL_CALL_X 100 //what's this??
#define NAME_OK 201 
#define EMER_NUMBER_OK 202 
#define BLOOD_TYPE_OK 203 
#define NAME_ERROR 301 
#define EMER_NUMBER_ERROR 302 
#define BLOOD_TYPE_ERROR 303 


String namE;
String emNumber="112";
String bloodType;

#define LEDred 8
#define LEDgreen 9
#define BUTTON1 10
#define BUTTON2 11
#define BRUNZ 12

TinyGPSPlus gps;  //GPS object
ADXL345 adxl; //Accelerometer object
bool accelerWorking;


void setup()
{
    pinMode(BRUNZ,OUTPUT);
    pinMode(BUTTON1,INPUT);
    pinMode(BUTTON2,INPUT);
    pinMode(LEDgreen,OUTPUT);
    pinMode(LEDred,OUTPUT);

    Serial2.begin(115200);  //Bluetooth   CANVIAR A SERIAL2!!!!!!!!!!
    Serial1.begin(115200); //GPS   must be 9600, I think
    Serial.begin(115200); //Serial Monitor

    //accelerometerSetUp();
    adxl.setInterrupt(ADXL345_FREE_FALL,false);

    accelerWorking = false; //false
    //accelerometerSetUp();
    digitalWrite(LEDred,HIGH);
}


void loop() {
     if (accelerWorking){ listenAcceler(1000); }
     listenApp(20);   
     
}


void listenAcceler(int msec){
      Serial.println("Listening accelerometer");
      uint32_t period = msec;     
      for( uint32_t tStart = millis();  (millis()-tStart) < period;  ){
          byte interrupts;
        //  interrupts = adxl.getInterruptSource(ADXL345_INT_FREE_FALL_BIT); //is going crazy here??
          interrupts = adxl.getInterruptSource(); //is going crazy here??

          
          // freefall
          if(adxl.triggered(interrupts, ADXL345_FREE_FALL)){
              Serial.println("falling");             
              falling();
          } 
          
         //tap
          if(adxl.triggered(interrupts, ADXL345_SINGLE_TAP)){
           //   Serial.println("tap");
             // falling();                      
      }           
   }
}

void listenApp(int msec){
  //Serial.print(". ");
  uint32_t period = msec;     
    for( uint32_t tStart = millis();  (millis()-tStart) < period;  ){
       if (Serial2.available()){///////////////
          while (Serial2.available()) {////////////
          char message[500]="";
          Serial2.readBytes(message,500);///////////
          String myString = String(message);
          Serial.println(message);
          decodeString(message);
          }      
       }
    }
}


void decodeString(char message[]){
    char *ptr = NULL;
    ptr = strtok(message, ";");  // takes a list of delimiters
    String first = ptr;
    int meaning = first.toInt();
    
    switch(meaning){
      case NAME:
        setName(ptr);      
        break;
        
      case EMER_NUMBER:
        setEmNumber(ptr);
        break;
        
      case BLOOD_TYPE:
        setBloodType(ptr);
        break;

      case SIMULACRE_BUTTON:
        Serial.println("You pressed the simulacre button in the App");
        falling();
        break;
        
      case STOP_EMERGENCY: 
        Serial.println("There is no emergency right now. The message will be ignored");
        break;

      case DETECT_FALLING_ON:
        digitalWrite(LEDred,LOW);
        digitalWrite(LEDgreen,HIGH);
        accelerometerSetUp();
        break;

      case DETECT_FALLING_OFF:
        accelerWorking = false;
        digitalWrite(LEDred,HIGH);
        digitalWrite(LEDgreen,LOW);
        Serial.println("Falling detector disconnected");
        //stopping the power supplying
        break;
       
      default:
        Serial.println("Unrecognizable message. It will be ignored.");
        break;
    }
    
}

void resetSystem(){
   delay(2000);
   Serial.print("Reseting system...");
   delay(2000);
   Serial.print("READY");

}


void setLEDs(){
   if(accelerWorking){
      digitalWrite(LEDred,LOW);
      digitalWrite(LEDgreen,HIGH);
   }else{
      digitalWrite(LEDred,HIGH);
      digitalWrite(LEDgreen,LOW);
   }
}


/*NAME, EMERGENCYNUMBER & BLOODTYPE setters*/
void setName(char *ptr){
    if (ptr != NULL) {
        ptr = strtok(NULL, ";");  // takes a list of delimiters
        namE = ptr;
        Serial2.println(String(NAME_OK)+";");
        Serial.println("Your new name is "+namE);
        Serial.println("Sent NAME_OK (code "+String(NAME_OK)+").");        
    }
    else{
        Serial2.println(String(NAME_ERROR)+";");
        Serial.println("Error setting your name");
        Serial.println("Sent NAME_ERROR (code "+String(NAME_ERROR)+").");   
      
    }
}


void setEmNumber(char *ptr){
    if (ptr != NULL)
    {
        ptr = strtok(NULL, ";");  // takes a list of delimiters
        emNumber = ptr;
        Serial2.println(String(EMER_NUMBER_OK)+";"); 
        Serial.println("Your new emergency number is "+emNumber);
        Serial.println("Sent EMER_NUMBER_OK (code "+String(EMER_NUMBER_OK)+").");   
    }
    else{
        Serial2.println(String(EMER_NUMBER_ERROR)+";"); 
        Serial.println("Error setting your emergency number");
        Serial.println("Sent EMER_NUMBER_ERROR (code "+String(EMER_NUMBER_ERROR)+").");  
        emNumber="112";
    }
}


void setBloodType(char *ptr){
    if (ptr != NULL)
    {
        ptr = strtok(NULL, ";");  // takes a list of delimiters
        bloodType = ptr;
        Serial2.println(String(BLOOD_TYPE_OK)+";"); 
        Serial.println("Your blood type is "+bloodType);
        Serial.println("Sent BLOOD_TYPE_OK (code "+ String(BLOOD_TYPE_OK)+").");   
        
    }
    else{
        Serial2.println(String(BLOOD_TYPE_ERROR)+";"); 
        Serial.println("Error setting your blood type");
        Serial.println("Sent BLOOD_TYPE_ERROR (code "+String(BLOOD_TYPE_ERROR)+")."); 
    }
}
