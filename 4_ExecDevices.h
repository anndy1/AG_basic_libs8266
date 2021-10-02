// ver.2.0 2020-02-24

//-------------------  Класс "Реле" -------------------

class Relay {
  public:
    Relay(uint8_t pin, bool active);
    //входные параметры --
    bool   modeAuto;            // 1 - Авто вкл. , 0 - ручной режим
    bool   cmd;                 // 1 - вкл. ,      0 - выкл. Команда <- MQTT
    bool   state;               // 1 - вкл. ,      0 - выкл. Реальное состояние вывода контроллера -> MQTT
    char  stateChar[2]={'0','\0'};
    char  cmdChar[2] = {'0','\0'};
    //--------------------
    uint8_t getPwm();
    bool   stateChanged();
    bool   cmdChanged();
    bool   pwmChanged();
    bool   getState();
    void   on();
    void   off();
    void   run();
    void   runAuto();
    void   setPwm(uint8_t val);
    void   runPwm();
  private:
    uint8_t _pin;
    bool   _active;
    bool   _on, _off;
    bool   _stateOld;
    bool   _cmdOld;
    bool   _pwmOld;
    bool   _pwmChanged;
    bool   _cmdChanged;
    bool   _stateChanged;
    uint32_t _timeLast = 0;
    uint8_t  _pwmValue = 0;  
    

};

// конструктор: на входе pin - номер вывода, 
//              active - логический уровень включения: 
//              1 - (1-вкл., 0-выкл.), 0 - (1-выкл., 0-вкл.) 
Relay:: Relay(uint8_t pin, bool active) {
  // stateChar = "2";
  modeAuto = 1;
  _pin =    pin;
  _active=  active;
  if (_active) {
    _on =   1;
    _off =  0; }
  else {
    _on =   0;
    _off =  1; 
  }
  _pwmChanged = true;
  pinMode(_pin,OUTPUT);
  off();
  delay(100);
  _stateOld = getState();
  cmdChanged();
  setPwm(0);
}

///---------------------------------------------------------------///

void Relay:: on(){
  digitalWrite(_pin, _on);
}

void Relay:: off(){
  digitalWrite(_pin, _off);
}

bool Relay:: getState() {
  if (_active) {
    if (digitalRead(_pin) != 0)  {state=1; dtostrf((state), 1, 0, stateChar);}
    else                         {state=0; dtostrf((state), 1, 0, stateChar);}
    }
  else {
    if (digitalRead(_pin) != 0) {state=0; dtostrf((state), 1, 0, stateChar);}
    else                        {state=1; dtostrf((state), 1, 0, stateChar);}
  }
  return state;    
}

bool Relay:: cmdChanged(){
  if (_cmdOld != cmd){
    _cmdOld=cmd;
    _cmdChanged = true;
    dtostrf((cmd), 1, 0, cmdChar);
  }
  else _cmdChanged = false;
  return _cmdChanged;
}

// можно использовать вместо getState() //
bool Relay:: stateChanged(){ 
  bool statetemp = getState();
  if  (statetemp != _stateOld) {
    _stateOld = statetemp;
    _stateChanged = true;
  }
  else _stateChanged = false;
  return _stateChanged;
}

void Relay:: runAuto(){
  if (modeAuto){
    if (cmd == 1) {
      }
    else {
    }
  }
  else{
  }
  
};

void Relay:: run() {
  if (cmd) {
    on();
  }
  else {
    off();
  }
}

void Relay:: setPwm(uint8_t val){
  _pwmValue  = val;
  if (_pwmValue != _pwmOld){
    _pwmOld = _pwmValue;
    _pwmChanged = true;
  } 
  else {
    _pwmChanged = false;
  }
}

uint8_t Relay:: getPwm(){
  return _pwmValue;
}

bool Relay:: pwmChanged(){
  if (_pwmChanged) return true;
  else return false;
}


void Relay:: runPwm() {
  uint16_t pwmMap;
  if (_active){
    pwmMap = map(_pwmValue, 0, 20, 0, 1023);
  }
  else {
    pwmMap = map(_pwmValue, 0, 20, 1023, 0);
  }
    if (cmd) {
    analogWrite(_pin, pwmMap);
  }
  else {
    off();
  }
}


//******************************************************************************************************************************

//-------------------  Класс "Термостат" -------------------

class Thermostat {
  public:
    Thermostat(uint8_t pin, bool active);
    Thermostat(uint8_t pin, bool active, bool _cooler);
    Thermostat(uint8_t pin, bool active, int8_t min, int8_t max, int8_t thres);
    Thermostat(uint8_t pin, bool active, int8_t min, int8_t max, int8_t thres, bool _cooler);
    //входные параметры --
    int8_t min;
    int8_t max;
    int8_t thres;
    int8_t critical;
    float  paramCurrent;
    bool   modeAuto;            // 1 - Авто вкл. , 0 - ручной режим
    bool   cmd;                 // 1 - вкл. ,      0 - выкл. Команда <- MQTT
    bool   state;               // 1 - вкл. ,      0 - выкл. Реальное состояние вывода контроллера -> MQTT
    bool   cooler;
    char  stateChar[2]={'2','\0'};
    char  cmdChar[2] = {'2','\0'};
    //--------------------
    bool   stateChanged();
    bool   cmdChanged();
    bool   getState();
    void   on();
    void   off();
    void   runMin();
    void   runMax();
    void   runMinMax();
    void   runAuto();
  private:
    uint8_t _pin;
    bool   _active;
    bool   _on, _off;
    bool   _stateOld;
    bool   _cmdOld;
    bool   _cmdChanged;
    bool   _stateChanged;
    uint32_t _timeLast = 0;

};



// конструктор: на входе pin - номер вывода, 
//              active - логический уровень включения: 
//              1 - (1-вкл., 0-выкл.), 0 - (1-выкл., 0-вкл.) 
Thermostat:: Thermostat(uint8_t pin, bool active) {
  //stateChar = "2";
  modeAuto = 1;
  _pin =    pin;
  _active=  active;
  cooler = 0;
  if (_active) {
    _on =   1;
    _off =  0; }
  else {
    _on =   0;
    _off =  1; 
  }
  paramCurrent = 777.0;  
  pinMode(_pin,OUTPUT);
  off();
  delay(100);
  _stateOld = getState();
  cmdChanged();
}

// конструктор: на входе pin - номер вывода, 
//              active - логический уровень включения: 
//              1 - (1-вкл., 0-выкл.), 0 - (1-выкл., 0-вкл.) 
//              _cooler - 1 кондиционер, 0 - нагреватель
Thermostat:: Thermostat(uint8_t pin, bool active, bool _cooler) {
  //stateChar = "2";
  modeAuto = 1;
  _pin    = pin;
  _active = active;
  cooler = _cooler;
  if (_active) {
    _on =   1;
    _off =  0; }
  else {
    _on =   0;
    _off =  1; 
  }
  paramCurrent = 777.0;  
  pinMode(_pin,OUTPUT);
  off();
  delay(100);
  _stateOld = getState();
  cmdChanged();
}

// конструктор: на входе pin - номер вывода, 
//              active - логический уровень включения: 
//              1 - (1-вкл., 0-выкл.), 0 - (1-выкл., 0-вкл.)
//              _min, _max, _thres - минимум значения (температуры), максимум, гистерезис 
Thermostat:: Thermostat(uint8_t pin, bool active, int8_t _min, int8_t _max, int8_t _thres) {
  //stateChar = "2";
  modeAuto = 1;
  _pin =    pin;
  _active=  active;
  cooler = 0;
  if (_active) {
    _on =   1;
    _off =  0; }
  else {
    _on =   0;
    _off =  1; 
  }
  min = _min;
  max = _max;
  thres = _thres;
  paramCurrent = 777.0;  
  pinMode(_pin,OUTPUT);
  off();
  delay(100);
  _stateOld = getState();
  cmdChanged();
}

// конструктор: на входе pin - номер вывода, 
//              active - логический уровень включения: 
//              1 - (1-вкл., 0-выкл.), 0 - (1-выкл., 0-вкл.)
//              _min, _max, _thres - минимум значения (температуры), максимум, гистерезис 
//              _cooler - 1 кондиционер, 0 - нагреватель
Thermostat:: Thermostat(uint8_t pin, bool active, int8_t _min, int8_t _max, int8_t _thres, bool _cooler) {
  //stateChar = "2";
  modeAuto = 1;
  _pin    = pin;
  _active = active;
  cooler = _cooler;
  if (_active) {
    _on =   1;
    _off =  0; }
  else {
    _on =   0;
    _off =  1; 
  }
  min = _min;
  max = _max;
  thres = _thres;
  paramCurrent = 777.0;  
  pinMode(_pin,OUTPUT);
  off();
  delay(100);
  _stateOld = getState();
  cmdChanged();
}

///---------------------------------------------------------------///

void Thermostat:: on(){
  digitalWrite(_pin, _on);
}

void Thermostat:: off(){
  digitalWrite(_pin, _off);
}

bool Thermostat:: getState() {
  if (_active) {
    if (digitalRead(_pin) != 0)  {state=1; dtostrf((state), 1, 0, stateChar);}
    else                         {state=0; dtostrf((state), 1, 0, stateChar);}
    }
  else {
    if (digitalRead(_pin) != 0) {state=0; dtostrf((state), 1, 0, stateChar);}
    else                        {state=1; dtostrf((state), 1, 0, stateChar);}
  }
  return state;    
}

bool Thermostat:: cmdChanged(){
  if (_cmdOld != cmd){
    _cmdOld=cmd;
    _cmdChanged = true;
  }
  else _cmdChanged = false;
  return _cmdChanged;
}

// можно использовать вместо getState()
bool Thermostat:: stateChanged(){ 
  bool i = getState(); 
  if (i != _stateOld) {
    _stateOld = i;
    _stateChanged = true;
  }
  else _stateChanged = false;
  return _stateChanged;
}

// для нагревателя использовать обе функции: runMin(); runMax(); либо только runMin() для "Анти-замерзания"
// runMinMax() для нагревателя включает "Анти-замерзание" свм по себе. 

// Только для нагревателя. Включ если темп < min, min+thres - отключается 
void Thermostat:: runMin(){       
  if (cmd != 1) {
    if (paramCurrent <= (float)(min)) on();
    if (paramCurrent >= (float)(min+thres)) off(); 
  }
};

// нагрев:      включ. если темп <= max; отключ. темп >= max-thres
// кондиционер: включ. если темп >= max; отключ. темп <= max-thres
void Thermostat:: runMax(){       
  if (cmd == 1) {
    if (!cooler){
      if (paramCurrent <= (float)(max-thres)) on();
      if (paramCurrent >= (float)(max)) off(); 
    }
    else {
      if (paramCurrent <= (float)(max-thres)) off();
      if (paramCurrent >= (float)(max)) on();      
    }
  }
};

// используется, в основном, для кондиционера и вентилятора
// нагрев:      от min до max
// кондиционер: включ. если темп >= max; отключ. темп <= min
void Thermostat:: runMinMax(){
  if (cmd == 1) {
    if (!cooler){
      if (paramCurrent >= (float)max) off();
      if (paramCurrent <= (float)min) on();  
    }
    else {
      if (paramCurrent >= (float)max) on();
      if (paramCurrent <= (float)min) off(); 
    }
  }
  else {
    if (!cooler) runMin();
    else off();
  }
};

void Thermostat:: runAuto(){
  if (modeAuto){
    if (cmd == 1) {
      if (!cooler){
        if (paramCurrent >= (float)max) off();
        if (paramCurrent <= (float)min) on();  
      }
      else {
        if (paramCurrent >= (float)max) on();
        if (paramCurrent <= (float)min) off(); 
      }
    }
    else {
      if (!cooler) runMin();
      else off();
    }
  }
  else{
  }
};

// ********************************************************************************************************************* //
// класс для внешних пинов и расширителей: на входе _pin - active - логический уровень включения:                         //
// lib_deps =                                                                                                            //
// ArduinoJson                                                                                                           //
// https://github.com/WereCatf/PCF8574_ESP                                                                               //
// ********************************************************************************************************************* //

#include <PCF8574_esp.h>

// ======================================== T E R M O S T A T . E X P A N D E R ======================================== //
  class ThermostatExpander : public PCF857x {
    public:
      ThermostatExpander(uint8_t address);   // address - адрес расширителя, active  - битовая маска уровней вкл/выкл устройств; cooler - кондей/вентилятор, для реле = 0
      //входные параметры --
      int8_t    min[8];
      int8_t    max[8];
      int8_t    thres[8];
      int8_t    critical[8];
      float     paramCurrent[8];
      bool      modeAuto[8];            // 1 - Авто вкл. , 0 - ручной режим
      bool      cmd[8];                 // 1 - вкл. ,      0 - выкл. Команда <- MQTT
      bool      state[8];               // 1 - вкл. ,      0 - выкл. Реальное состояние вывода контроллера -> MQTT
      char      stateChar[8][2] = {{'2','\0'}, {'3','\0'}, {'4','\0'}, {'5','\0'}, {'6','\0'}, {'7','\0'}, {'8','\0'}};
      char      cmdChar[8][2]   = {{'8','\0'}, {'7','\0'}, {'6','\0'}, {'5','\0'}, {'4','\0'}, {'3','\0'}, {'2','\0'}};
      //--------------------
      void      init(uint8_t active, uint8_t cooler);
      void      on(uint8_t _pin);
      void      off(uint8_t _pin);
      bool      getState(uint8_t _pin);
      bool      stateChanged(uint8_t _pin);
      bool      cmdChanged(uint8_t _pin);
      void      run(uint8_t _pin);
      void      runAuto(uint8_t _pin);
      void      runMin(uint8_t _pin);
      void      runMax(uint8_t _pin);
      void      runMinMax(uint8_t _pin);
      void      setParamCurrent(uint8_t _pin, float _param);
    private:
      bool      _active[8];
      bool      _cooler[8];              // 1 - Нагреватель 0 - охлаждатель (кондей)
      bool      _on[8], _off[8];
      bool      _stateOld[8];
      bool      _cmdOld[8];
      bool      _cmdChanged[8];
      bool      _stateChanged[8];
      uint32_t  _timeLast[8];
  };
  ///---------------------------------------------------------------///  
    

    // конструктор: на входе _pin - номер вывода, 
    //              active - логический уровень включения: 
    //              1 - (1-вкл., 0-выкл.), 0 - (1-выкл., 0-вкл.) 
    //              cooler - 1 кондиционер, 0 - нагреватель
    ThermostatExpander:: ThermostatExpander(uint8_t address) : PCF857x(address, &Wire) {
      PCF857x :: begin();
    }
    ///---------------------------------------------------------------///

    void ThermostatExpander:: init(uint8_t active, uint8_t cooler){
      for (uint8_t idx=0; idx<=7; idx++){
       // if ((active & (0b00000001<<idx))) _active[idx] = true;
       // else _active[idx] = false;
        
       _active[idx] =  (active & (0b00000001<<idx));
       if (_active[idx]) {
           _on[idx] =   true;
           _off[idx] =  false; }
        else {
            _off[idx] =  true; 
            _on[idx] =   false;
       }
       _cooler[idx] = (cooler & (0b00000001<<idx));
       modeAuto[idx] = true;
       paramCurrent[idx] = 777.0;  
       off(idx);
       delay(100);
       _stateOld[idx] = getState(idx);
       cmdChanged(idx);
      }
    }

    void ThermostatExpander:: on(uint8_t _pin){
     PCF857x :: write(_pin, _on[_pin]);
    }

    void ThermostatExpander:: off(uint8_t _pin){
     PCF857x :: write(_pin, _off[_pin]);
    }

    bool ThermostatExpander:: getState(uint8_t _pin) {
     if (_active[_pin]) {
        if (PCF857x :: read(_pin) != 0)  {state[_pin]=1; dtostrf((state[_pin]), 1, 0, stateChar[_pin]);}
       else                              {state[_pin]=0; dtostrf((state[_pin]), 1, 0, stateChar[_pin]);}
        }
      else {
       if (PCF857x :: read(_pin) != 0) {state[_pin]=0; dtostrf((state[_pin]), 1, 0, stateChar[_pin]);}
        else                           {state[_pin]=1; dtostrf((state[_pin]), 1, 0, stateChar[_pin]);}
     }
      return state[_pin];    
    }

    bool ThermostatExpander:: cmdChanged(uint8_t _pin){
      if (_cmdOld[_pin] != cmd[_pin]){
        _cmdOld[_pin]=cmd[_pin];
       _cmdChanged[_pin] = true;
     }
     else _cmdChanged[_pin] = false;
     return _cmdChanged[_pin];
    }

    // можно использовать вместо getState()
    bool ThermostatExpander:: stateChanged(uint8_t _pin){ 
    bool __state = getState(_pin); 
    if (__state != _stateOld[_pin]) {
        _stateOld[_pin] = __state;
        _stateChanged[_pin] = true;
    }
    else _stateChanged[_pin] = false;
    return _stateChanged[_pin];
    }

    // для нагревателя использовать обе функции: runMin(); runMax(); либо только runMin() для "Анти-замерзания"
    // runMinMax() для нагревателя включает "Анти-замерзание" сам по себе. 

    // run() - для реле
    void ThermostatExpander:: run(uint8_t _pin) {
      if (cmd[_pin]) {
        on(_pin);
      }
      else {
        off(_pin);
      }
    }

    void ThermostatExpander:: runAuto(uint8_t _pin){
      if (modeAuto[_pin]){
        if (cmd[_pin] == 1) {
            if (!_cooler[_pin]){
                if (paramCurrent[_pin] >= (float)max[_pin]) off(_pin);
                if (paramCurrent[_pin] <= (float)min[_pin]) on(_pin);  
            }
            else {
            if (paramCurrent[_pin] >= (float)max[_pin]) on(_pin);
            if (paramCurrent[_pin] <= (float)min[_pin]) off(_pin); 
            }
        }
        else {
            if (!_cooler[_pin]) runMin(_pin);
            else off(_pin);
        }
      }
      else{ }
    };

    // Только для нагревателя. Включ если темп < min, min+thres - отключается 
    void ThermostatExpander:: runMin(uint8_t _pin){       
      if (cmd[_pin] != 1) {
        if (paramCurrent[_pin] <= (float)(min[_pin])) on(_pin);
          if (paramCurrent[_pin] >= (float)(min[_pin]+thres[_pin])) off(_pin); 
      }
    };

  // нагрев:      включ. если темп <= max; отключ. темп >= max-thres
  // кондиционер: включ. если темп >= max; отключ. темп <= max-thres
    void ThermostatExpander:: runMax(uint8_t _pin){       
      if (cmd[_pin] == 1) {
        if (!_cooler[_pin]){
          if (paramCurrent[_pin] <= (float)(max[_pin]-thres[_pin])) on(_pin);
          if (paramCurrent[_pin] >= (float)(max[_pin])) off(_pin); 
        }
       else {
          if (paramCurrent[_pin] <= (float)(max[_pin]-thres[_pin])) off(_pin);
          if (paramCurrent[_pin] >= (float)(max[_pin])) on(_pin);      
        }
      }
    };

    // используется, в основном, для кондиционера и вентилятора
    // нагрев:      от min до max
    // кондиционер: включ. если темп >= max; отключ. темп <= min
    void ThermostatExpander:: runMinMax(uint8_t _pin){
      if (cmd[_pin] == 1) {
          if (!_cooler){
          if (paramCurrent[_pin] >= (float)max[_pin]) off(_pin);
          if (paramCurrent[_pin] <= (float)min[_pin]) on(_pin);  
        }
      else {
          if (paramCurrent[_pin] >= (float)max[_pin]) on(_pin);
          if (paramCurrent[_pin] <= (float)min[_pin]) off(_pin); 
        }
      }
      else {
        if (!_cooler[_pin]) runMin(_pin);
        else off(_pin);
      }
    };

    void ThermostatExpander:: setParamCurrent(uint8_t _pin, float _param){
        // перевод цифрового входного пераметра в символьный вид
    }


// ===================================== E N D T E R M O S T A T . E X P A N D E R ===================================== //