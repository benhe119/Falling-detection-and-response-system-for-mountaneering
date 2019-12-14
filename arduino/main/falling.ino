#define GET_LOCATION_TRYING_TIME 6000 //in ms
void activateAirbag(){
/*SIMULATION. The airbag would be triggered by one electrical signal coming from Arduino.
 * As long as we have not been able to obtain the airbag, the triggering cannot be done.
 * Instead, this function blinks the LEDs as a simulation*/
  int i=0;
  for (i=0;i<30;i++){
    digitalWrite(LEDgreen,HIGH);
    digitalWrite(LEDred,HIGH);
    delay(100);
    digitalWrite(LEDgreen,LOW);
    digitalWrite(LEDred,LOW);
    delay(100);
  }
}

        
void falling(){
   activateAirbag();
   
   Serial.println("****Activating falling reaction...*******"); 
   Serial2.println(String(FALLING_DETECTED)+";"); 
   Serial.println("Falling reaction activated");
   Serial.println("Sent FALLING_DETECTED (code "+String(FALLING_DETECTED)+").");        
    
   Serial.println("***Activating brunzer. If the user is OK should stop the emergency by;");
   Serial.println("     -Pressing the OK button in the App.");
   Serial.println("     -Pressing the 2 physical buttons in the bag*");
   Serial.println("Otherwise, emergency rescue will be called.***");
   
   bool stopped = alarm();

   if (stopped){
      Serial.println("You are OK. Good to here that");
   } else{
      fallingReaction();
   }

   resetSystem();
   setLEDs();
                 
}


void fallingReaction(){
   Serial.println("");
   Serial.println("");
   Serial.println("***You are not OK, emergency will be called***");

   //Try to obtain location. 
   if (isLocationAvailable(GET_LOCATION_TRYING_TIME)){  //Location succesfully obtained
       String message = ": Please, send an emergency rescue team in the following location.   Location=" + getLocation();
       message +=       "Personal information: Name=" + namE + ", Blood Type=" + bloodType;
       
       calling(message);  //that's the calling, with location
      
       afterFalling();   //endless
       
   } else{  //Location not obtained
      String message = ": Please, send an emergency rescue team. Not able to send my current location. I will send it when possible";
      message +=       "Personal information: Name=" + namE + ", Blood Type=" + bloodType;
      
      calling(message); //that's the calling, without location

      while(!buttonsPressed()){
          Serial.print("Attempting to get location...");
          if(isLocationAvailable(5000)){fallingReaction();};  //check GPS during 20 seconds
          Serial.println("failed");
          Serial.println("Activating brunzer 10 secs");
          if(brunzerCycle(10000)){break;};                     //activate brunzer 10 sec. If user is OK, finish emeergency
      }

   }

   //end of the falling reaction process. Back to the main loop, listening the App and the accelerometer
  
}



void calling(String message){
     Serial.println("CALLING TO " + emNumber + message);  
     String sms = String(SMS_SIMULATION) + ";Calling " + emNumber + message + ";stop";
     Serial2.println(sms);
}



void afterFalling(){
  //Brunzer and LEDs blinking endless, unless 2 buttons or "I'm OK" from the app
  while(!brunzerCycle(300)){} 
}
