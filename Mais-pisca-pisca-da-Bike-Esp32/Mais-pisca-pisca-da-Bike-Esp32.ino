//variaveis representando as LEDS
//
int ledAmarelo = 4;
int ledVermelho = 5;
//variaveis para saber se é para ligar a ESQUERDA ou DIREITA
bool esquerda = false;
bool direita = false;
//botoes
int botaoAmarelo = 16;
int botaoVermelho = 17;
//variaveis de controle
bool statusBotaoAmarelo = false;
bool statusBotaoVermelho = false;
void setup() {
  Serial.begin(115200);
  //D4,D5 E GND
  pinMode(ledAmarelo,OUTPUT);
  pinMode(ledVermelho,OUTPUT);
  pinMode(botaoAmarelo,INPUT_PULLUP);
  pinMode(botaoVermelho,INPUT_PULLUP);
}

void loop() {
  statusBotaoAmarelo = digitalRead(botaoAmarelo);
  statusBotaoVermelho = digitalRead(botaoVermelho);
  if(!statusBotaoAmarelo){
    esquerda = false;
    direita = true;
  }
  if(!statusBotaoVermelho){
    esquerda = true;
    direita = false;
  }
   if(!statusBotaoVermelho && !statusBotaoAmarelo){
    esquerda = false;
    direita = false;
  }

  //verificar se a quantidade de bytes é maior que zero
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
    delay(200);
    digitalWrite(led,LOW);
    delay(200);
}
//metodo para desligar qualquer led
void desligaLED(int led){
    digitalWrite(led,LOW);
}
