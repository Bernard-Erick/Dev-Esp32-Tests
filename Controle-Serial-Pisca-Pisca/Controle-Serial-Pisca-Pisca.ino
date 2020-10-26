//variaveis representando as LEDS
int ledAmarelo = 4;
int ledVermelho = 5;
//variavel para a guardar o codigo da letra
int qtdBytes = 0;
//variaveis para saber se é para ligar a ESQUERDA ou DIREITA
bool esquerda = false;
bool direita = false;

void setup() {
  Serial.begin(19200);
  //D4,D5 E GND
  pinMode(ledAmarelo,OUTPUT);
  pinMode(ledVermelho,OUTPUT);
}

void loop() {
  //verificar se a quantidade de bytes é maior que zero
  if(Serial.available() > 0){
    //armazenar o numero de bytes na variavel
    qtdBytes = Serial.read();
    //verificar letra 'D'
    if(qtdBytes == 68){
        direita = true;
        esquerda = false;
     }
     //verificar letra 'E'
    if(qtdBytes == 69){
        esquerda = true;
        direita = false;
    }
    //verificar letra 'P'
    if(qtdBytes == 80){
        esquerda = false;
        direita = false;
    }
  }
  //se for pra ligar a esquerda
  if(esquerda){
    piscaLED(ledVermelho);
  }
  //se for pra ligar a direita
  if(direita){
    piscaLED(ledAmarelo);
  }
  
}
//metodo para piscar qualquer led
void piscaLED(int led){
    digitalWrite(led,HIGH);
    delay(500);
    digitalWrite(led,LOW);
    delay(500);
}
//metodo para desligar qualquer led
void desligaLED(int led){
    digitalWrite(led,LOW);
}
