#include "SCHm10.h"
#include <Arduino.h>

#define Nullptr(type) (type *)0

SCHm10::SCHm10(Stream &comms) {
  comm = &comms;
  state = SCHm10StateReady;

  sendBuffer.head = 0;
  sendBuffer.tail = 0;
}

void SCHm10::sendConfiguration() {
  checkSerialData("");
  while(sendBuffer.head != sendBuffer.tail) if(comm->available()) checkSerialData(comm->readString());
}

void SCHm10::pushString(String command) {
  int next = sendBuffer.head + 1;
  if (next > sendBuffer.maxLen) next = 0;
  if (next == sendBuffer.tail) return;

  sendBuffer.stringBuffer[sendBuffer.head] = command;
  sendBuffer.head = next;
}

String SCHm10::popString(){
  if (sendBuffer.head == sendBuffer.tail) return String();
  String command = sendBuffer.stringBuffer[sendBuffer.tail];

  int next = sendBuffer.tail + 1;
  if (next >= sendBuffer.maxLen) next = 0;
  sendBuffer.tail = next;

  return command;
}

void SCHm10::checkSerialData(String data) {
  switch(state) {
    case SCHm10StateReady:
      {
        String command = popString();
        if (command && command.length() > 0)  sendCommand(command);
      }
      break;
    case SCHm10StateWaitingAcknowledge:
        {
          if (data.length() > 0) {
             if (data.indexOf('OK') > 0) {
                state = SCHm10StateReady;
                String command = popString();
                if (command && command.length() > 0) sendCommand(command);
             }
          }
        }
        break;
  }
}

boolean SCHm10::setName(const char * deviceName) {
  if (strlen(deviceName) >= 12) return false;
  String sendValue = "NAME" + String(deviceName);
  return addToBuffer(sendValue);
}

boolean SCHm10::setBaud(const char * baudRate) {
  String sendValue = "BAUD" + String(baudRate);
  return addToBuffer(sendValue);
}

boolean SCHm10::setPassword(int password) {
  String sendValue = "PIN" + String(password);
  return addToBuffer(sendValue);
}

boolean SCHm10::setRole(const char * role) {
  String sendValue = "ROLE" + String(role);
  return addToBuffer(sendValue);
}

boolean SCHm10::setModulePower(const char * power) {
  String sendValue = "POWE" + String(power);
  return addToBuffer(sendValue);
}

boolean SCHm10::reset() {
  return addToBuffer(F("RESET"));
}

boolean SCHm10::sleep() {
  return addToBuffer(F("SLEEP"));
}

boolean SCHm10::start() {
  return addToBuffer(F("START"));
}

boolean SCHm10::addToBuffer(String command) {
  pushString(command);
  return true;
}

boolean SCHm10::sendCommand(String command){
  if (!comm || state == SCHm10StateWaitingAcknowledge) return false;
  state = SCHm10StateWaitingAcknowledge;
  command = "AT+" + command;
  char* buf = (char*) malloc(sizeof(char)*command.length()+1);
  command.toCharArray(buf, command.length()+1);
  comm->write(buf);
  free(buf);
}

