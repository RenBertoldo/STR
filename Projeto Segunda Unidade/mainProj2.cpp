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
int HighPrio = 3;
int MidPrio = 10;
int LowPrio = 18;
int valorCarga = 10000;
int showLed = 0;


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
  while(!parar){
		if(showLed == 0){
			LedSon1.setValue(high);
			LedSon2.setValue(low);
			carga(valorCarga);
			showLed++;
		}else{
			LedSon1.setValue(low);
			LedSon2.setValue(high);
			carga(valorCarga);
			showLed--;
		}
	}
}

<<<<<<< HEAD
int main(){
=======
 int main(){
>>>>>>> f85c65e7324cc65cdbf578c1f24108253dbff268
	 int res;
	 pthread_t Pot1, Pot2;
	 void *thread_result;
	 pthread_attr_t thread_attr1,thread_attr2;
	 sched_param scheduling_value;
	 int  mid_priority, min_priority;
	 float vPot1,vPot2;

	 res = pthread_attr_init(&thread_attr1);
	 if (res != 0) {
			 perror("Falha na criação de Atributo");
			 exit(EXIT_FAILURE);
	 }
	 res = pthread_attr_init(&thread_attr2);
	 if (res != 0) {
			 perror("Falha na criação de Atributo");
			 exit(EXIT_FAILURE);
	 }

	 res = pthread_attr_setschedpolicy(&thread_attr1, SCHED_RR);
	 if (res != 0) {
			 perror("Falha na configuração de política de escalonamento");
			 exit(EXIT_FAILURE);
	 }
	 res = pthread_attr_setschedpolicy(&thread_attr2, SCHED_RR);
	 if (res != 0) {
			 perror("Falha na configuração de política de escalonamento");
			 exit(EXIT_FAILURE);
	 }
<<<<<<< HEAD

	 res = pthread_attr_setdetachstate(&thread_attr1, PTHREAD_CREATE_DETACHED);
	 if (res != 0) {
			 perror("Falha na configuração de atributo detached");
			 exit(EXIT_FAILURE);
	 }
	 res = pthread_attr_setdetachstate(&thread_attr2, PTHREAD_CREATE_DETACHED);
	 if (res != 0) {
			 perror("Falha na configuração de atributo detached");
			 exit(EXIT_FAILURE);
	 }

	 mid_priority = sched_get_priority_min(SCHED_RR);
	 mid_priority = mid_priority/2;
	 min_priority = sched_get_priority_min(SCHED_RR);

	 res = pthread_create(&Pot1, &thread_attr, readPot, NULL);
	 if (res != 0) {
			 perror("Falha na criação da thread");
			 exit(EXIT_FAILURE);
	 }

	 res = pthread_create(&Pot2, &thread_attr, readPot, NULL);
	 if (res != 0) {
			 perror("Falha na criação da thread");
			 exit(EXIT_FAILURE);
	 }


	 while(!parar){
		 vPot1 = PotSon1.getNumericValue();
		 vPot2 = PotSon2.getNumericValue();
		 if(vPot1 > 1.0){
			 scheduling_value.sched_priority = mid_priority;
			 res = pthread_attr_setschedparam(&thread_attr1, &scheduling_value);
	     if (res != 0) {
	         perror("Falha na configuração da pol�tica de escalonamento");
	         exit(EXIT_FAILURE);
	     }
		 }if(vPot2 > 1.0){
				scheduling_value.sched_priority = mid_priority;
				res = pthread_attr_setschedparam(&thread_attr2, &scheduling_value);
				if (res != 0) {
				   perror("Falha na configuração da pol�tica de escalonamento");
				   exit(EXIT_FAILURE);
				 }
			 }
			if(vPot1 < 1.0){
				scheduling_value.sched_priority = min_priority;
				res = pthread_attr_setschedparam(&thread_attr2, &scheduling_value);
				if (res != 0) {
					perror("Falha na configuração da pol�tica de escalonamento");
	        exit(EXIT_FAILURE);
	     }
		 }else{
			 scheduling_value.sched_priority = min_priority;
			 res = pthread_attr_setschedparam(&thread_attr2, &scheduling_value);
			 if (res != 0) {
				 perror("Falha na configuração da pol�tica de escalonamento");
				 exit(EXIT_FAILURE);
			 }

		 }
	 }
	 res = pthread_join(Pot1, &thread_result);
	 if (res != 0) {
		 perror("Thread join failed");
		 exit(EXIT_FAILURE);
		}
		res = pthread_join(Pot2, &thread_result);
		if (res != 0) {
			perror("Thread join failed");
			exit(EXIT_FAILURE);
		 }


	 return(0);
}
=======

	 res = pthread_attr_setdetachstate(&thread_attr1, PTHREAD_CREATE_DETACHED);
	 if (res != 0) {
			 perror("Falha na configuração de atributo detached");
			 exit(EXIT_FAILURE);
	 }
	 res = pthread_attr_setdetachstate(&thread_attr2, PTHREAD_CREATE_DETACHED);
	 if (res != 0) {
			 perror("Falha na configuração de atributo detached");
			 exit(EXIT_FAILURE);
	 }

	 mid_priority = sched_get_priority_min(SCHED_RR);
	 mid_priority = mid_priority/2;
	 min_priority = sched_get_priority_min(SCHED_RR);

	 res = pthread_create(&Pot1, &thread_attr, readPot, NULL);
	 if (res != 0) {
			 perror("Falha na criação da thread");
			 exit(EXIT_FAILURE);
	 }

	 res = pthread_create(&Pot2, &thread_attr, readPot, NULL);
	 if (res != 0) {
			 perror("Falha na criação da thread");
			 exit(EXIT_FAILURE);
	 }


	 while(!parar){
		 vPot1 = PotSon1.getNumericValue();
		 vPot2 = PotSon2.getNumericValue();
		 if(vPot1 > 1.0){
			 scheduling_value.sched_priority = mid_priority;
			 res = pthread_attr_setschedparam(&thread_attr1, &scheduling_value);
	     if (res != 0) {
	         perror("Falha na configuração da pol�tica de escalonamento");
	         exit(EXIT_FAILURE);
	     }
		 }if(vPot2 > 1.0){
			 scheduling_value.sched_priority = mid_priority;
			 res = pthread_attr_setschedparam(&thread_attr2, &scheduling_value);
	     if (res != 0) {
	         perror("Falha na configuração da pol�tica de escalonamento");
	         exit(EXIT_FAILURE);
	     }
		 }if(vPot1 < 1.0){
			 scheduling_value.sched_priority = min_priority;
			 res = pthread_attr_setschedparam(&thread_attr2, &scheduling_value);
	     if (res != 0) {
	         perror("Falha na configuração da pol�tica de escalonamento");
	         exit(EXIT_FAILURE);
	     }
		 }else{
			 scheduling_value.sched_priority = min_priority;
			 res = pthread_attr_setschedparam(&thread_attr2, &scheduling_value);
	     if (res != 0) {
	         perror("Falha na configuração da pol�tica de escalonamento");
	         exit(EXIT_FAILURE);
		 }

	 }
	 res = pthread_join(Pot1, &thread_result);
	 if (res != 0) {
			 perror("Thread join failed");
			 exit(EXIT_FAILURE);
	 }
	 res = pthread_join(Pot2, &thread_result);
	 if (res != 0) {
			 perror("Thread join failed");
			 exit(EXIT_FAILURE);
	 }


	 return(0);
 }
>>>>>>> f85c65e7324cc65cdbf578c1f24108253dbff268
