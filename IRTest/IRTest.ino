#include <TinyIRremote.h>
#include <TinyIRremoteInt.h>

IRsend sender;

void setup() {
  // put your setup code here, to run once:
}

void loop() {
  // put your main code here, to run repeatedly:
  sender.sendNEC(0x20DF6897, 32);
  delay(10000);
}
