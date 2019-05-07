//Tomas de Camino Beck
//www.funcostarica.org

#include "Chromosome.h"
#include <Adafruit_CircuitPlayground.h>

#define N 10

//crea la poblacion
Population pop(N);

byte mejorChromosoma;
int generacionesSinCambios = 0;

//prende los pixles dependiendo de los bits en el cromosoma
void setPixels(int a) {
  for (int i = 0; i < 8; i++) {
    if (bitRead(pop.chromosome[a], i)) {
      CircuitPlayground.setPixelColor(i + 2, CircuitPlayground.strip.Color(255, 255, 255));
    } else {
      CircuitPlayground.setPixelColor(i + 2, CircuitPlayground.strip.Color(0, 0, 0));
    }
  }
}

//Calcula el fitnes basado en la cantidad de luz (maximiza)
//y la cantidad de leds (minimiza)
void fitness(int a) {
  pop.fitness[a] = 0.7 * float(CircuitPlayground.lightSensor()) - 0.3 * float(pop.countBits(a));
}

//Evalua cada cromosoma
void evaluate() {
  for (int i = 0; i < pop.n; i++) {
    setPixels(i);
    //gives LED time to turn on
    delay(30);
    fitness(i);
  }
}


void mejorEncontrado(){
  for (int i = 0; i < 8; i++) {
    if (bitRead(pop.chromosome[0], i)) {
      CircuitPlayground.setPixelColor(i + 2, CircuitPlayground.strip.Color(0, 200, 0));
    } else {
      CircuitPlayground.setPixelColor(i + 2, CircuitPlayground.strip.Color(125, 0, 0));
    }
  }
}

void setup() {
  randomSeed(analogRead(A9));
  Serial.begin(115200);
  CircuitPlayground.begin();
  CircuitPlayground.clearPixels();
  //inicia con una poblaci�n aleatoria
  pop.mutateChromosomes(0.5,0);
  mejorChromosoma=pop.chromosome[0];
}

void loop() {
 
  //aplica recombinaci�n con cromosoma 0 a 2 y los copia en una poblaci�n
  pop.copyCrossover(2);

  //genera mutaciones en cada gen con
  //probabilidad 0.1
  pop.mutateChromosomes(0.01, 1);

  //evalua
  evaluate();

  //Ordena los cromosomas de mayr a menor fitness
  pop.sort();

  //prende los leds que correspoden al mejor fitness
  setPixels(0);
  
  if (mejorChromosoma==pop.chromosome[0]){
    generacionesSinCambios++;
    Serial.println(generacionesSinCambios);
  }
  else{
    generacionesSinCambios=0;
    mejorChromosoma=pop.chromosome[0];
  }
  if(generacionesSinCambios>25){
    Serial.println(millis()/1000);
    mejorEncontrado();
    while(1){
      delay(1000);
    }
  }
  //espera
  delay(10);
}
