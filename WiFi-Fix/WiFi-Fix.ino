#include <nvs_flash.h>
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  nvs_flash_erase();
  nvs_flash_init();
  Serial.println("Suscess!");
}

void loop() {
  // put your main code here, to run repeatedly:

}
