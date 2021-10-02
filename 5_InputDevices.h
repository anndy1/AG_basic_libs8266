
// ver.01 2019-02-14

//-------------------  Класс "MyBtn" -------------------

class MyBtn {
  public:
    MyBtn(uint8_t pin, bool active);
    //входные параметры --
    bool    modeAuto;            // 1 - Авто вкл. , 0 - ручной режим
    bool    cmd;                 // 1 - вкл. ,      0 - выкл. Команда <- MQTT
    bool    clickLong;           // 1 - вкл. ,      0 - выкл. Команда <- MQTT
    bool    state{0};               // 1 - вкл. ,      0 - выкл. Реальное состояние вывода контроллера -> MQTT
    bool    clickLongChanged;
    char    stateChar[2] =     {'0','\0'};
    char    clickLongChar[2] = {'0','\0'};
    char    cmdChar[2]   = {'0','\0'};
    //--------------------
    bool    stateChanged();
    bool    stateChangedIrq();
    bool    cmdChanged();
    bool    getState();
    uint8_t pin();
  private:
    uint8_t _pin;
    bool    _active;
    bool    _on, _off;
    bool    _stateOld;
    bool    _cmdOld;
    bool    _cmdChanged;
    bool    _stateChanged;
    uint32_t _timeLast = 0;

};

// конструктор: на входе pin - номер вывода, 
//              active - логический уровень включения: 
//              1 - (1-вкл., 0-выкл.), 0 - (1-выкл., 0-вкл.) 
MyBtn:: MyBtn(uint8_t pin, bool active) {
  clickLongChanged = 0;
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
  delay(10);
  _stateOld = getState();
  cmdChanged();
}

///---------------------------------------------------------------///

bool MyBtn:: getState() {
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

// можно использовать вместо getState()
bool MyBtn:: stateChanged(){ 
  bool i = getState(); 
  if (i != _stateOld) {
    _stateOld = i;
    _stateChanged = true;
  }
  else _stateChanged = false;
  return _stateChanged;
}

bool MyBtn:: stateChangedIrq(){ 
  if (state != _stateOld) {
    _stateOld = state;
    _stateChanged = true;
  }
  else _stateChanged = false;
  return _stateChanged;
}

bool MyBtn:: cmdChanged(){
  if (_cmdOld != cmd){
    _cmdOld=cmd;
    _cmdChanged = true;
    dtostrf((cmd), 1, 0, cmdChar);
  }
  else _cmdChanged = false;
  return _cmdChanged;
}

uint8_t MyBtn:: pin() {
  return _pin;
}
//******************************************************************************************************************************