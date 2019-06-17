#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <Servo.h>

// Update these with values suitable for your network.

const char* ssid = "Bjornphone";
const char* password = "gonzalo11";
const char* channel_name = "topic_2";
const char* mqtt_server = "192.168.43.203";
const char* http_server = "192.168.43.203";
const char* http_server_port = "8090";
String clientId;
int peso=0;
int hora=0;
int pesoLeido=0;
Servo servo;
const int fsrPin = A0;
int fsrReading = 0;

const long utcOffsetInSeconds = 3600*2;

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
long lastMsgRest = 0;
char msg[50];
int value = 0;

// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "hora.rediris.es", utcOffsetInSeconds);
// Conexión a la red WiFi
void setup_wifi() {

  delay(10);

  // Fijamos la semilla para la generación de número aleatorios. Nos hará falta
  // más adelante para generar ids de clientes aleatorios
  randomSeed(micros());

  Serial.println();
  Serial.print("Conectando a la red WiFi ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  // Mientras que no estemos conectados a la red, seguimos leyendo el estado
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  // En este punto el ESP se encontrará registro en la red WiFi indicada, por
  // lo que es posible obtener su dirección IP
  Serial.println("");
  Serial.println("WiFi conectado");
  Serial.println("Dirección IP registrada: ");



  Serial.println(WiFi.localIP());

}


// Función para la reconexión con el servidor MQTT y la suscripción al canal
// necesario. También se fija el identificador del cliente
void reconnect() {
  // Esperamos a que el cliente se conecte al servidor
  while (!client.connected()) {
    Serial.print("Conectando al servidor MQTT...");
    // Creamos un identificador de cliente aleatorio. Cuidado, esto debe
    // estar previamente definido en un entorno real, ya que debemos
    // identificar al cliente de manera unívoca en la mayoría de las ocasiones
    clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Intentamos la conexión del cliente
    if (client.connect(clientId.c_str())) {
      String printLine = "   Cliente " + clientId + " conectado al servidor" + mqtt_server;
      Serial.println(printLine);
      // Publicamos un mensaje en el canal indicando que el cliente se ha
      // conectado. Esto avisaría al resto de clientes que hay un nuevo
      // dispositivo conectado al canal. Puede ser interesante en algunos casos.
      String body = "Dispositivo con ID = ";
      body += clientId;
      body += " conectado al canal ";
      body += channel_name;
      client.publish(channel_name, "");
      // Y, por último, suscribimos el cliente al canal para que pueda
      // recibir los mensajes publicados por otros dispositivos suscritos.
      client.subscribe(channel_name);
    } else {
      Serial.print("Error al conectar al canal, rc=");
      Serial.print(client.state());
      Serial.println(". Intentando de nuevo en 5 segundos.");
      delay(5000);
    }
  }
}


// Método para hacer una petición GET al servidor REST
void makeGetRequest(){
    HTTPClient http;
    // Abrimos la conexión con el servidor REST y definimos la URL del recurso
    String url = "http://";
    url += http_server;
    url += ":";
    url += http_server_port;
    url += "/comidaProxima/";
    String message = "Enviando petición GET al servidor REST. ";
    message += url;

    Serial.println(message);

    http.begin(url);
    // Realizamos la petición y obtenemos el código de estado de la respuesta
    int httpCode = http.GET();
    Serial.printf("\nRespuesta servidor REST %d\n", httpCode);


    if (httpCode > 0)
    {
     // Si el código devuelto es > 0, significa que tenemos respuesta, aunque
     // no necesariamente va a ser positivo (podría ser un código 400).
     // Obtenemos el cuerpo de la respuesta y lo imprimimos por el puerto serie
     String payload = http.getString();
     Serial.println("payload: " + payload);

     const size_t bufferSize = JSON_OBJECT_SIZE(1) + 370;
     DynamicJsonDocument root(bufferSize);
     deserializeJson(root, payload);
     const char* Nombre= root["nombre"];
     hora = root[0]["horas"];
     peso = root[0]["pesosPlato"];

     Serial.println(root.as<String>());

  }
  if(hora==0 && peso==0){
    String url = "http://";
    url += http_server;
    url += ":";
    url += http_server_port;
    url += "/PrimeraComida/";
    String message = "Enviando petición GET al servidor REST. ";
    message += url;

    Serial.println(message);

    http.begin(url);
    // Realizamos la petición y obtenemos el código de estado de la respuesta
    int httpCode = http.GET();
    Serial.printf("\nRespuesta servidor REST %d\n", httpCode);


    if (httpCode > 0)
    {
     // Si el código devuelto es > 0, significa que tenemos respuesta, aunque
     // no necesariamente va a ser positivo (podría ser un código 400).
     // Obtenemos el cuerpo de la respuesta y lo imprimimos por el puerto serie
     String payload = http.getString();
     Serial.println("payload: " + payload);

     const size_t bufferSize = JSON_OBJECT_SIZE(1) + 370;
     DynamicJsonDocument root(bufferSize);
     deserializeJson(root, payload);
     const char* Nombre= root["nombre"];
     hora = root[0]["horas"];
     peso = root[0]["pesosPlato"];

     Serial.println(root.as<String>());

  }
  }


    // Cerramos la conexión con el servidor REST
    http.end();
}

// Método llamado por el cliente MQTT cuando se recibe un mensaje en un canal
// al que se encuentra suscrito. Los parámetros indican el canal (topic),
// el contenido del mensaje (payload) y su tamaño en bytes (length)

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Mensaje recibido [canal: ");
  Serial.print(topic);
  Serial.print("] ");
  // Leemos la información del cuerpo del mensaje. Para ello no sólo necesitamos
  // el puntero al mensaje, si no su tamaño.
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();


  DynamicJsonDocument doc(length);
  deserializeJson(doc, payload, length);
  const char* action = doc["action"];
  Serial.printf("Acción %s\n", action);
  // Encendemos un posible switch digital (un diodo led por ejemplo) si el
  // contenido del cuerpo es 'on'
  if (strcmp(action, "actualiza") == 0) {
    makeGetRequest();
    Serial.println("Actualizacion.");
  } else{
    Serial.println("Acción no reconocida");
  }
}
void dispensar( int peso){
  fsrReading = analogRead (fsrPin);
  //double pesoL= (fsrReading*10000.0)/1023.0;
  while (fsrReading<peso){
    fsrReading = analogRead (fsrPin);
    //double pesoL= (fsrReading*10000.0)/1023.0;
    Serial.printf("Peso leido: %d",fsrReading );
    Serial.printf("Peso total: %d ", peso);
    servo.write(90);
    delay(100);
    servo.write(0);
    delay(1000);


  }

    // Construimos un objeto JSON con el contenido del mensaje a publicar
    // en el canal.
    StaticJsonDocument<200> doc;
    doc["clientId"] = clientId;
    doc["message"] = "Comida servida";

    String output;
    serializeJson(doc, output);
    Serial.print("Mensaje publicado: ");
    Serial.println(output);
    client.publish(channel_name, output.c_str());

  delay(60000);




}


// Método de inicialización de la lógica
void setup() {


  Serial.begin(115200); // activamos la conexión serial
  servo.attach(2);//D4
  servo.write(0);
  // Nos conectamos a la red WiFi
  setup_wifi();

  // Indicamos la dirección y el puerto del servidor donde se encuentra el
  // servidor MQTT
  client.setServer(mqtt_server, 1883);
  // Fijamos la función de callback que se ejecutará cada vez que se publique
  // un mensaje por parte de otro dispositivo en un canal al que el cliente
  // actual se encuentre suscrito
  client.setCallback(callback);
  makeGetRequest();
}

void loop() {


  // Nos conectamos al servidor MQTT en caso de no estar conectado previamente
  if (!client.connected()) {
    reconnect();
  }
  // Esperamos (de manera figurada) a que algún cliente suscrito al canal
  // publique un mensaje que será recibido por el dispositivo actual
  client.loop();


  timeClient.update();

  Serial.println(timeClient.getFormattedTime());
  if((timeClient.getHours())*100+(timeClient.getMinutes()) == hora){
    dispensar(peso);
    makeGetRequest();
    Serial.printf("Peso nuevo: %d",  peso);
  }else{

    Serial.printf("%d\n", peso);
    Serial.printf("%d\n", hora);
    Serial.println("Todavia no");
  }

  delay(6000);

}
