// https://www.bakke.online/index.php/2017/06/02/self-updating-ota-firmware-for-esp8266/

/*
  String getMAC()
  {
  uint8_t mac[6];
  char result[14];

  snprintf( result, sizeof( result ), "%02x%02x%02x%02x%02x%02x", mac[ 0 ], mac[ 1 ], mac[ 2 ], mac[ 3 ], mac[ 4 ], mac[ 5 ] );

  return String( result );
  }*/

void checkForUpdates() {
  String mac = config->_esp_name;
  String fwURL = ("http://" + String (config->_mqtt_server) + "/fota/");
  fwURL.concat( mac );
  String fwVersionURL = fwURL;
  fwVersionURL.concat( ".version" );
  #ifdef serialOut
  Serial.print( F("Device name: ") );
  Serial.println( mac );
  Serial.print( F("Current firmware version: ") );
  Serial.println( FW_VERSION );
  Serial.println( F("Checking for firmware updates.") );
  Serial.print( F("Firmware version URL: ") );
  Serial.println( fwVersionURL );
  #endif

  HTTPClient httpClient;
  httpClient.begin( fwVersionURL );
  int httpCode = httpClient.GET();
  if ( httpCode == 200 ) {
    String newFWVersion = httpClient.getString();
    uint32_t newVersion = newFWVersion.toInt();
    #ifdef serialOut
    Serial.print( F("Current firmware version: ") );
    Serial.println( FW_VERSION );
    Serial.print( F("Available firmware version: ") );
    Serial.println( newVersion );
    #endif


    if ( newVersion > FW_VERSION ) { 
      String fwImageURL = fwURL;
      fwImageURL += ("_" + String(newVersion));
      fwImageURL.concat( ".bin" );
      #ifdef serialOut
      Serial.println( F("Preparing to update") );
      Serial.println("Waiting for " + fwImageURL);                    //**************************************
      #endif
      t_httpUpdate_return ret = ESPhttpUpdate.update( fwImageURL );
      
      if (ret == HTTP_UPDATE_FAILED) 
        { 
        #ifdef serialOut
        Serial.printf("HTTP_UPDATE_FAILD Error (%d): %s", ESPhttpUpdate.getLastError(), ESPhttpUpdate.getLastErrorString().c_str());
        #endif 
        }
      if (ret == HTTP_UPDATE_NO_UPDATES) 
        { 
        #ifdef serialOut 
        Serial.println(F("HTTP_UPDATE_NO_UPDATES"));
        #endif 
        }
    }
    else 
      {
        #ifdef serialOut
        Serial.println( F("Already on latest version") ); 
        #endif
      }
  }
  else {
    #ifdef serialOut
    Serial.print( F("Firmware version check failed, got HTTP response code ") );
    Serial.println( httpCode );
    #endif
  }
  httpClient.end();
  
}
