/**
 * 
 * SCHm10
 * 
 * SCHm10 Configuration helper
 * 
 * Version :
 *    - 0.0.2
 *      Replace loop fonction with sendConfiguration in setup
 *    - 0.0.1
 *      Creation
 * 
 */

#ifndef SCHm10_h
#define SCHm10_h

#include <Arduino.h>

#define SCHm10_MASTER "1"
#define SCHm10_SLAVE "0"

#define SCHm10_BAUD2400 "6"
#define SCHm10_BAUD4800 "5"
#define SCHm10_BAUD9600 "0"
#define SCHm10_BAUD19200 "1"
#define SCHm10_BAUD38400 "2"
#define SCHm10_BAUD57600 "3"

#define SCHm10_POWER_LEVEL_1 "0" //-23db
#define SCHm10_POWER_LEVEL_2 "1" //-6db
#define SCHm10_POWER_LEVEL_3 "2" //0db
#define SCHm10_POWER_LEVEL_4 "3" //6db

enum SCHm10State {
  SCHm10StateReady,
  SCHm10StateWaitingAcknowledge,
};

struct Buffer {
  static const int maxLen = 10;
  String stringBuffer[maxLen];
  int head, tail;
};

class SCHm10 {

  public:
    SCHm10(Stream &comms);

    void checkSerialData(String data);
    
    boolean setName(const char * deviceName);
    boolean setBaud(const char * baudRate);
    boolean setPassword(int password);
    boolean setRole(const char * role);
    boolean setModulePower(const char * power);

    boolean reset();
    boolean sleep();
    boolean start();

    void sendConfiguration();

  private:
    Stream *comm;

    SCHm10State state;
    
    boolean addToBuffer(String command);
    boolean sendCommand(String command);

    
    Buffer sendBuffer;
    void pushString(String command);
    String popString();
};

#endif
