
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
  Serial.begin(9600);
  delay(100);
}
 
int value = 0;
 
void loop() {
  for (int i = 0; i<10; i++){
    // Limpiamos el trigger
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    // Seteamos el trigger en HIGH por 10 microSegundos
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    // Leemos el echo, retorna el tiempo que duro la onda de sonido en microsegundos
    duration = pulseIn(echoPin, HIGH);
    // Calcula la distancia
    distance= duration*0.034/2;
    Serial.print("Distancia: ");
    Serial.println(distance);
    // Imprime la distancia
    lista[i]=distance;
    total = total + distance;
    delay(500);
  }
  double promedioFinal = (total/10);
  int promedioFinalAux = (int)promedioFinal;
  Serial.print("Promedio de Distancia: ");
  Serial.println(promedioFinalAux);
  total = 0;
}
