
#include <ESP_EEPROM.h>

struct MyEEPROMStruct {
  uint8_t tMin;
  uint8_t tMax;
  uint8_t tTres;
  uint8_t tCritical;
} initValues;

void printInitValues();

void initSettings(){
  EEPROM.begin(sizeof(MyEEPROMStruct));
  EEPROM.get(0, initValues);
    // Check if the EEPROM contains valid data from another run
    // If so, overwrite the 'default' values set up in our struct
    if(EEPROM.percentUsed()>=0) {
      #ifdef serialOut
        Serial.println(F("EEPROM has data from a previous run."));
        Serial.print  (EEPROM.percentUsed());
        Serial.println  (F("% of ESP flash space currently used"));
      #endif
      printInitValues();
    } 
    else {
      Serial.println(F("EEPROM size changed - EEPROM data zeroed - commit() to make permanent"));    
    }
  if (INITSTATE) {
    initValues.tMin      = TMIN;
    initValues.tMax      = TMAX;
    initValues.tTres     = TTRES;
    initValues.tCritical = TEMPCRITICAL;
    EEPROM.put(0, initValues);
    boolean ok1 = EEPROM.commit();
    EEPROM.get(0, initValues);
    Serial.println(F(" ---------------- Перезапись исходнх данных. ----------------"));
    Serial.println((ok1) ? "Данные перезаписаны" : "Данные в EEPROM уже актуальны");
    printInitValues();
  }
}

void printInitValues(){
  #ifdef serialOut
    Serial.print  (F(  "tMin      = "));
    Serial.print  (initValues.tMin);
    Serial.print  (F("\ttMax      = "));
    Serial.print  (initValues.tMax);
    Serial.print  (F("\ttTres     = "));
    Serial.print  (initValues.tTres);
    Serial.print  (F("\ttCritical = "));
    Serial.println(initValues.tCritical);
  #endif
}