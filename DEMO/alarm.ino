
bool emergStoppedViaAPP(){
  //check if received the "i'm ok" from the app
   while (Serial2.available()) { 
      char message[500]="";
      Serial2.readBytes(message,500); 
      char *ptr = NULL;
      ptr = strtok(message, ";");  
      String first = ptr;
      int meaning = first.toInt();

      if (meaning == STOP_EMERGENCY){
        Serial.println("***Emergency stopped via the app***");
        return true;
      }
   }   
  return false;
    
}


bool buttonsPressed(){
  //check if both buttons are pressed at the same time
    int button1=digitalRead(BUTTON1);
    int button2=digitalRead(BUTTON2);
    
    if (button1==HIGH and button2==HIGH){
        Serial.println("***Emergency stopped because 2 buttons pressed***");
        return true;
    }else{
        return false;
    }
}

bool doesEmergStopped(){
  return buttonsPressed() or emergStoppedViaAPP();
}


boolean alarm(){
/*Activate the brunzer.
* - returns true if emergency has been stopped 
* - returns false if it has not, emergency should be called */

  bool emergStopped = false;
  
   int i=10; //i=42. 42 seconds. i=10= 10 secs
   while(!emergStopped and i>0 ){
      emergStopped = brunzerCycle(500);
      i--;
   }

  /*
   i=1; //i=3  18 seconds
   while(!emergStopped and i>0 ){
      emergStopped = brunzerCycle(3000);
      i--;
   }
  */
  return emergStopped;
   
}



bool brunzerCycle(int interval){
/*Returns true if emergency has stopped. Returns false if emergency has not stopped */
  bool stopped = false;
  digitalWrite(BRUNZ,HIGH);
  digitalWrite(LEDgreen,HIGH);
  digitalWrite(LEDred,HIGH);
  stopped = cuttedDelay (interval);   //maybe here check the buttons and the serial port.
  
  digitalWrite(BRUNZ,LOW);
  digitalWrite(LEDgreen,LOW);
  digitalWrite(LEDred,LOW);
  if (!stopped){ stopped = cuttedDelay (interval); }  //and here  
  return stopped;
}


bool cuttedDelay(int delaY){
/*does a delay, but during the delay checks if the emergency is stopped
WHY? explained in the Report (Arduino doesn't allow multithreading)*/
    int littleDelay = delaY/100;
    int i=0;
    bool stopped=false;
    
    while(i<100 and !stopped){  
      delay(littleDelay); 
      stopped = doesEmergStopped();
      i++;      
    } 
    return stopped;
}
