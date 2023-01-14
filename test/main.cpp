#include <Arduino.h>
using namespace std;

int main() {
    String readBuff = "pin_D0_0_1000";
    int index = readBuff.lastIndexOf("_");
    String pin_and_value = readBuff.substring(6, index);
    String touch_value = readBuff.substring(index + 1);
    index = pin_and_value.lastIndexOf("_");
    String pin = pin_and_value.substring(0, index);
    String value = pin_and_value.substring(index + 1);
    printf("pin: %s, value: %s, touch_value: %s", pin.c_str(), value.c_str(), touch_value.c_str());
    return 0;
}