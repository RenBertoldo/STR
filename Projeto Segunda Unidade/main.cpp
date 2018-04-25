// Universidade Federal do Rio Grande do Norte
// Graduação em Engenharia de Computação
// DCA0125 - Sistemas de Tempo Real
// Alunos: Renan Lucas Ribeiro Bertoldo e Jairo Boaz Cavalcanti dos Santos Louzarth

#include <time.h>
#include <unistd.h>
#include <thread>
#include <stdlib.h>
#include <iostream>
#include <pthread.h>
#include "BlackGPIO/BlackGPIO.h"
#include "ADC/Adc.h"

using namespace BlackLib;
using namespace std;

//Variaveis Globais
BlackADC PotSon1(AIN0);
BlackADC PotSon2(AIN6);
BlackGPIO LedSon1(GPIO_30,output);
BlackGPIO LedSon2(GPIO_31,output);
bool parar = false;
void carga(int k){
	float f = 0.999999;
	for(int i=0; i<k; i++){
		f = f*f*f*f*f;
		for(int j=0; j<k; j++){
			f = sin(f)*sin(f)*f*f*f;
		}
	}
}
void readPot(BlackADC *pot){
  float var = pot.getNumericValue();
  while(!parar){
    

  }
 }
