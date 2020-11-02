#include <WiFi.h>                               //biblioteca para esp32
#include <PubSubClient.h>                      //biblioteca para funções do protocolo Pub-Sub
const char* ssid = "wifiname";             //string com nome da rede onde o esp vai se conectar
const char* password =  "senha123";        //string com a senha da rede. Deixar em branco caso a rede seja aberta
const char* mqttServer = "broker.mqttdashboard.com";   //string com a url do Broker que vai receber e transmitir as mensagens
const int mqttPort = 1883;                      //inteiro com o número da porta utilizada na conexão
const char* mqttUser = "";                      //string com nome de usuário no Broker, quando for necessário
const char* mqttPassword = "";                  //string com senha do Broker, quando necessário
const char* topic_url_led = "esp/led/bernardash";
const char* topic_url_button = "esp/button/bernardash";

WiFiClient espClient;                     //criação do objeto espClient do tipo WiFiClient
PubSubClient client(espClient);           //biblioteca pubsubclient

int ledPin = 2;                           //define a váriavel led com o pino
int buttonPin = 16;
                       
boolean buttonStatus = false;             //variavel para armazenar o status do botao (true ou false)

void setup()
{
  pinMode(ledPin, OUTPUT);                //define o pino do led como saída
  pinMode(buttonPin,INPUT_PULLUP); 
  Serial.begin(115200);                   //define o baudrate da comunicação serial como 115200 kbps
  WiFi.begin(ssid, password);             //inicia a conexão WiFi com a rede

  while (WiFi.status() != WL_CONNECTED)   //verifica se a conexão ainda não estabizou
  {
    delay(500);                           //espera 500 ms
    Serial.println("Iniciando conexao com a rede WiFi.."); //"printa" na tela a frase entre aspas
  }

  Serial.println("Conectado na rede WiFi!");  //quando sai do laço, print a frase entre aspas
  client.setCallback(callback);               //configura o Callback do cliente a partir do callback passado como parâmetro
}

void loop()       //laço principal
{
  //Conecta com o broker, caso não esteja conectado.
  if(!client.connected()){
    reconectabroker();                      //chama função de conexão ao Broker
  }
  //Para checar a rotina de callback
  client.loop();
  //Vai ler o status do botao
  buttonStatus = digitalRead(buttonPin);
  //vai verificar se foi pressionado
  if(!buttonStatus){
    //Envia a mensagem ao broker
    client.publish(topic_url_button, "Botão pressionado");
    Serial.println("Sinal enviado com sucesso...");
    delay(1000);
  }

}

void reconectabroker()
{
  //Conexao ao broker MQTT
  client.setServer(mqttServer, mqttPort);
  while (!client.connected())
  {
    Serial.println("Conectando ao broker MQTT...");
    if (client.connect("", mqttUser, mqttPassword ))
    {
      Serial.println("Conectado ao broker!");
      client.subscribe(topic_url_led);               //assina o esp32 no tópico topico_teste definido no inicio do programa
    }
    else
    {
      Serial.print("Falha na conexao ao broker - Estado: ");
      Serial.print(client.state());
      delay(2000);
    }
  }
}
//definição da funçao de callback.
void callback(char* topic, byte* message, unsigned int length) {
  Serial.println("Chegou mensagem no topico: ");
  Serial.print(topic);
  Serial.print(". Mensagem: ");
  String messageTemp;
  
  for (int i = 0; i < length; i++) {  //escreve mensagem caractere por caractere
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();

  if (String(topic) == topic_url_led) {
    Serial.print("Trocando o estado do LED para: ");
    if(messageTemp == "on"){
      Serial.println("Ligado");
      digitalWrite(ledPin, HIGH);
    }
    else if(messageTemp == "off"){
      Serial.println("Desligado");
      digitalWrite(ledPin, LOW);
    }
  }
}
