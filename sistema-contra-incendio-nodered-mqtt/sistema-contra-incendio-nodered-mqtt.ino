
#include <WiFi.h>                               //biblioteca para esp32
#include <PubSubClient.h>                      //biblioteca para funções do protocolo Pub-Sub
const char* ssid = "nome_wifi";             //string com nome da rede onde o esp vai se conectar
const char* password =  "senha123";        //string com a senha da rede. Deixar em branco caso a rede seja aberta
const char* mqttServer = "broker.mqttdashboard.com";   //string com a url do Broker que vai receber e transmitir as mensagens
const int mqttPort = 1883;                      //inteiro com o número da porta utilizada na conexão
const char* mqttUser = "";                      //string com nome de usuário no Broker, quando for necessário
const char* mqttPassword = "";                  //string com senha do Broker, quando necessário
const char* topic_url_temperatura = "esp/temperatura/bernardash";
const char* topic_url_umidade = "esp/umidade/bernardash";
const char* topic_url_presenca = "esp/presenca/bernardash";
WiFiClient espClient;                     //criação do objeto espClient do tipo WiFiClient
PubSubClient client(espClient);           //biblioteca pubsubclient
#include "DHT.h"
//here we use 14 of ESP32 to read data
#define DHTPIN A13
//our sensor is DHT11 type
#define DHTTYPE DHT11
//create an instance of DHT sensor
DHT dht(22, DHTTYPE);
int buzzer = 4; 
int pinPIR = 27;
void setup()
{
  Serial.begin(115200);
  Serial.println("DHT11 sensor!");
  //call begin to start sensor
  pinMode(buzzer,OUTPUT);
  pinMode(pinPIR,INPUT);

  dht.begin();
  WiFi.begin(ssid, password);             //inicia a conexão WiFi com a rede

  while (WiFi.status() != WL_CONNECTED)   //verifica se a conexão ainda não estabizou
  {
    delay(500);                           //espera 500 ms
    Serial.println("Iniciando conexao com a rede WiFi.."); //"printa" na tela a frase entre aspas
  }

  Serial.println("Conectado na rede WiFi!");  //quando sai do laço, print a frase entre aspas
  client.setCallback(callback);
}
 
void loop() {
    //Conecta com o broker, caso não esteja conectado.
  if(!client.connected()){
    reconectabroker();                      //chama função de conexão ao Broker
  }
  //Para checar a rotina de callback
  client.loop();
 bool valorPIR = digitalRead(pinPIR);
 if(valorPIR){
    Serial.println("DETECTADO:");
  }
  else{
    Serial.println("----");
    }
//use the functions which are supplied by library.
float h = dht.readHumidity();
// Read temperature as Celsius (the default)
float t = dht.readTemperature();
// Check if any reads failed and exit early (to try again).
if (isnan(h) || isnan(t)) {
Serial.println("Failed to read from DHT sensor!");
return;
}
// print the result to Terminal

Serial.print("Humidity: ");
Serial.print(h);
Serial.print(" %\t");
Serial.print("Temperature: ");
Serial.print(t);
Serial.println(" *C ");
if(t >= 30){
  digitalWrite(buzzer, HIGH);
}
else{
  digitalWrite(buzzer, LOW);
}
client.publish(topic_url_temperatura, String(t).c_str(),true);
client.publish(topic_url_umidade, String(h).c_str(),true);

client.publish(topic_url_presenca, String(valorPIR).c_str(),true);

Serial.println("Sinal enviado com sucesso...");

//we delay a little bit for next read
delay(2000);
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
      client.subscribe(topic_url_temperatura);               //assina o esp32 no tópico topico_teste definido no inicio do programa
      client.subscribe(topic_url_umidade);
      client.subscribe(topic_url_presenca);
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
}
