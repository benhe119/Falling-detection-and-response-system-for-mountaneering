
bool isLocationAvailable(int msec){
    uint32_t period = msec;     
    for( uint32_t tStart = millis();  (millis()-tStart) < period;  ){
        Serial.print("Checking GPS data...");
        if (gps.location.isValid() && gps.altitude.isValid()){
           Serial.println("Location data correct!");
           return true;
        }
        smartDelay(2000);
        Serial.println("Unable to get location data.");
    }
    return false; 
}


String getLocation(){
    float latitude, longitude, altitud;
    String message;
    bool found = false;
  
    while(!found){
        if (gps.location.isValid()){
            latitude = gps.location.lat();
            longitude = gps.location.lng();
            altitud = gps.altitude.meters();
            found = true;
            message = " Latitude: " + String(latitude,6) + " Longitude: " + String(longitude,6) + " Altitude: " + String(altitud,2);
            Serial.println(message);
        }
    }
    return message; 
    
}


/* This custom version of delay() ensures that the gps object is being "fed". */
static void smartDelay(unsigned long ms) {
   unsigned long start = millis();
   do
   {
     while (Serial1.available())
       gps.encode(Serial1.read());
   } while (millis() - start < ms);
}
