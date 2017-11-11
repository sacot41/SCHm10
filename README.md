# SCHm10
Helper class to configure HM-10 with arduino

SCHm10 is a help class to avoid having to constantly refer to the HM-10 datasheet. It implements functions to modify the basic parameters of the Bluetooth module HM-10, such as its name, the baud rate, the password, etc.

# Install

You can install through the Arduino Library Manager.

# Example

``` arduino
#include "SCHm10.h"

SCHm10 hm10(Serial);

void setup() {
  hm10.setModulePower(HM10_POWER_LEVEL_1);
  hm10.setBaud(HM10_BAUD38400);
  hm10.setName("SCIOT_DOOR");
  hm10.reset();
  hm10.sendConfiguration();
}
```