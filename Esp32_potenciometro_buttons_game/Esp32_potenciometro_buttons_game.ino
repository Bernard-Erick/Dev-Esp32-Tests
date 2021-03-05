int POT = 4;
int potAtual= 0;
int buttonUp = 18;
int buttonDown = 19;
int flag = 0;

void setup() {
  Serial.begin(9600);
  pinMode(POT,INPUT);
  pinMode(buttonUp,INPUT_PULLUP);
  pinMode(buttonDown,INPUT_PULLUP);
  potAtual = map(analogRead(POT),0,4095,0,20);
}

void loop() {
  // logica potenciometro Player 2
  int potTempReal = map(analogRead(POT),0,4095,0,20);
  if(potAtual != potTempReal){
    potAtual = potTempReal;
        Serial.print("Player 2: ");
        Serial.println(potAtual);
  }
   
  // logica botoes Player 1 - subindo
  if(digitalRead(buttonUp) == LOW && flag == 0){
    Serial.print("Player 1: ");
    Serial.println("subiu");
    flag = 1;
    delay(3);
  }
    // logica botoes Player 1 - descendo
  if(digitalRead(buttonDown) == LOW && flag == 0){
    Serial.print("Player 1: ");
    Serial.println("desceu");
    flag = 1;
    delay(3);
  }
  if(digitalRead(buttonUp) == HIGH && digitalRead(buttonDown) == HIGH ){
    flag = 0;
    delay(3);
  }
}
