#include "BluetoothSerial.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)

#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it

#endif

BluetoothSerial SerialBT;
int ledAmarelo = 4;
int qtdBytes = 0;

void setup() {

  Serial.begin(115200);

  SerialBT.begin("e-s-p-3-2"); //Bluetooth device name

  Serial.println("The device started, now you can pair it with bluetooth!");

  pinMode(ledAmarelo,OUTPUT);
}



void loop() {
  
  if (Serial.available() > 0) {
    SerialBT.write(Serial.read());
    
  }

  if (SerialBT.available() > 0) {
    qtdBytes = SerialBT.read();
    if(qtdBytes == 65){
      digitalWrite(ledAmarelo,HIGH);
    }
    if(qtdBytes == 66){
      digitalWrite(ledAmarelo,LOW);
    }
    Serial.write(SerialBT.read());
  }

  delay(20);

}
