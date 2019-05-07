#include <ESP8266WiFi.h>

/*
 * Variables de Red
 */
const char* ssid     = "SSID"; //EDIT
const char* password = "PASSWORD";  //EDIT
//#const char* host = "pagina.com";
/*----------------------------------------------------------*/


/*
 * Variables de Ultrasonico
 */
const int trigPin = 4;
const int echoPin = 5;
int lista[10] = {0,0,0,0,0,0,0,0,0,0} ;
double total = 0;
unsigned long duration;
int distance;
/*----------------------------------------------------------*/

 
void setup() {
  
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  Serial.begin(115200);
  delay(100);
 
  // We start by connecting to a WiFi network
 
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
 
  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}
 
int value = 0;
 
void loop() {
  for (int i = 0; i<10; i++){
    // Clears the trigPin
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    // Sets the trigPin on HIGH state for 10 micro seconds
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    // Reads the echoPin, returns the sound wave travel time in microseconds
    duration = pulseIn(echoPin, HIGH);
    // Calculating the distance
    distance= duration*0.034/2;
    Serial.print("Distancia: ");
    Serial.println(distance);
    // Prints the distance on the Serial Monitor
    lista[i]=distance;
    total = total + distance;
    delay(5000);
  }
  double promedioFinal = (total/10);
  int promedioFinalAux = (int)promedioFinal;
  Serial.print("Promedio de Distancia: ");
  Serial.println(promedioFinalAux);
  String resultado="";
  while(resultado.length()<1){
    resultado = getHttp(String(promedioFinalAux));
    Serial.println(resultado);
    if(resultado.length()>0){
      Serial.println("Si se subio");
    }
    else{
      Serial.println("No se subio");
    }
  }
  total = 0;
}

String getHttp(String num){
  String textoFinal = "";
  ++value; 
  Serial.print("connecting to ");
  Serial.println(host);  
  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return "";
  }  
  // We now create a URI for the request
  String url = "/actualizar_alt_momen.php?alt="+num;
  Serial.print("Requesting URL: ");
  Serial.println(url);  
  // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");
  //delay(1000);
  int timeout = millis()+5000;
  while(client.available() == 0){
    if( (timeout - millis())<=0 || (timeout - millis())>5000 ){
      Serial.print(">>> Client Timeout!");
      client.stop();
      return "";
    }
  }
  // Read all the lines of the reply from server and print them to Serial
  while(client.available()){
    String line = client.readStringUntil('\r');
    textoFinal = line;
    Serial.print(line);
  }
  Serial.println();
  Serial.println("closing connection");
  return textoFinal;
}
