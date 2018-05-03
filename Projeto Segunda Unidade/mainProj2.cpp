// Universidade Federal do Rio Grande do Norte
// Graduação em Engenharia de Computação
// DCA0125 - Sistemas de Tempo Real
// Alunos: Renan Lucas Ribeiro Bertoldo e Jairo Boaz Cavalcanti dos Santos Louzarth

#include <time.h>
#include <unistd.h>
#include <stdlib.h>
#include <iostream>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/types.h>
#include "BlackGPIO/BlackGPIO.h"
#include "BlackADC/BlackADC.h"
#include "ADC/Adc.h"

using namespace BlackLib;
using namespace std;


//Variaveis Globais
BlackADC PotSon1(AIN0);
BlackADC PotSon2(AIN6);
BlackGPIO LedSon1(GPIO_30,output);
BlackGPIO LedSon2(GPIO_31,output);

bool parar = false;
int HighPrio = 3;
int MidPrio = 10;
int LowPrio = 18;
int valorCarga = 10000;
int showLed = 0;
float vSon1, vSon2;

void carga(int k){
	float f = 0.999999;
	for(int i=0; i<k; i++){
		f = f*f*f*f*f;
		for(int j=0; j<k; j++){
			f = sin(f)*sin(f)*f*f*f;
		}
	}
}

int main(){
	int res;
	pid_t son1, son2;

	son1 = fork();   // dividindo o processo em dois
  switch(son1)
    {
      case -1:       // erro na abertura do processo filho
        exit(1);
      case 0:        // Parte a ser executada pelo processo Filho1
				while(!parar){
					carga(valorCarga);
					if(showLed==0){
						LedSon1.setValeu(high);
						LedSon2.setValeu(low);
						showLed++;
					}
				}
				break;
      default:       // parte a ser executada pelo processo Pai
				son2 = fork();   // dividindo o processo em dois
				switch(son2)
					{
						case -1:       // erro na abertura do processo filho
							exit(1);
						case 0:        // Parte a ser executada pelo processo Filho2
							while(!parar){
								carga(valorCarga);
								if(showLed==1){
									LedSon1.setValeu(low);
									LedSon2.setValeu(high);
									showLed--;
								}
							}
							break;
						default:       // parte a ser executada pelo processo Pai
							setpriority(PRIO_PROCESS, getpid(), HighPrio);
							while(!parar){
								vSon1 = PotSon1.getNumericValue();
								vSon2 = PotSon2.getNumericValue();
								if(vSon1>=1.0){
									setpriority(PRIO_PROCESS,son1,MidPrio);
								}
								if(vSon2>=1.0){
									setpriority(PRIO_PROCESS,son2,MidPrio);
								}
								if(vSon1<1.0){
									setpriority(PRIO_PROCESS,son1,LowPrio);
								}
								if (vSon2<1.0) {
									setpriority(PRIO_PROCESS,son2,LowPrio);
								}
							}
				 }
				 break;
   }
	return(0);
}
