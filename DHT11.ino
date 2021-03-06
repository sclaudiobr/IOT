// Claudio Silva
// 30/08/2019
//claumao@gmail.com
//--->>>> IMPORTACAO DE BIBLIOTECAS
#include <ESP8266WiFi.h>     //esp8266
#include <FirebaseArduino.h> //bibliotecas 
#include <DHT.h>            // dht11 temperatura e umidade sensor

//----->> CONEXÃO COM FIREBASE E INTERNET WIFI
#define FIREBASE_HOST "arduinofirebase-53da1.firebaseio.com"
#define FIREBASE_AUTH "XXXXXXXxxxXXXxxXXXXXXXXXXXXXXXXXxxX"
#define WIFI_SSID "XXXXXXXXX"
#define WIFI_PASSWORD "xxxxxxxxxxxxxx"

 //--> CONFIGURAÇÃO DE PINOS DE COMUNICAÇÃO

#define DHTPIN D4                // pino de conexão do sensor
#define DHTTYPE DHT11           // seleciona o modelo do sensor (DHT 11 or DHT22)
DHT dht(DHTPIN, DHTTYPE);                                                     

//--> INÍCIALIZAÇÃO DE PROCESSOS 
void setup() {
  Serial.begin(9600);
  delay(1000);                
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);    
  Serial.print("Connecting to ");
  Serial.print(WIFI_SSID);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("Connected to ");
  Serial.println(WIFI_SSID);
  Serial.print("IP Address is : ");
  Serial.println(WiFi.localIP());                  //imprime  endereço do IP Local
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);    // conecta ao firebase
  dht.begin();                                    //inicia a leitura dht sensor
}

// --> PROGRAMA DE CICLOS
void loop() { 
  float h = dht.readHumidity();       // Lê temperatura ou umidade 
  float t = dht.readTemperature();   // Lê temperatura em Celsius (padrão)

    //--> Testa se são valores válidos
  if (isnan(h) || isnan(t)) {  // verifica se está sendo realizada a leitura e tenta novamente.
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }
    
    // - > Imprime as leituras na porta serial e converte os dados float lidos na porta 4
  Serial.print("Humidity: ");  Serial.print(h);
  String fireHumid = String(h) + String("%");    //converte inteiro para texto 
  Serial.print("%  Temperature: ");  Serial.print(t);  Serial.println("°C ");
  String fireTemp = String(t) + String("°C"); //converte de inteiro para texto
  delay(5000); // intervalo entre as medições
  
    // --> Registra no Firebase através de uma chave única "pushString"
  Firebase.pushString("/ESTACAO/Umidade", fireHumid);         //pasta de trabalho
  Firebase.pushString("/ESTACAO/Temperatura", fireTemp);        //pasta de trabalho
   
}
