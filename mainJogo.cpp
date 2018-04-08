// Universidade Federal do Rio Grande do Norte
// DCA0125 - Sistemas de Tempo Real
// Alunos: Renan Lucas Ribeiro Bertoldo e Pablo Favero de Siqueira.
// Projeto jogo Genius.

#include <vector>
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
int nCores = 4;
int nLvl = 10;
int lvl;
int tLed = 1200000;
int *seq_jogo;
int *User_Seq;
int PosInput = 0;

time_t TempoDesligar = 5;
bool btCollect = false;
bool start = false;
bool fimJogo = false;
bool restart = false;

//Declara as saídas
BlackGPIO ledR(GPIO_38,output);
BlackGPIO ledG(GPIO_34,output);
BlackGPIO ledB(GPIO_66,output); 
BlackGPIO ledY(GPIO_69,output);
vector<BlackGPIO*> vetorLeds;

BlackGPIO dispA(GPIO_30,output);
BlackGPIO dispB(GPIO_31,output);
BlackGPIO dispC(GPIO_48,output);
BlackGPIO dispD(GPIO_5,output);
BlackGPIO dispE(GPIO_3,output);
BlackGPIO dispF(GPIO_49,output);
BlackGPIO dispG(GPIO_117,output);
vector<BlackGPIO*> vetorDisp;

// Declara as entradas
BlackGPIO btR(GPIO_51,input);
BlackGPIO btG(GPIO_2,input);
BlackGPIO btB(GPIO_14,input);
BlackGPIO btY(GPIO_20,input);
BlackGPIO btReset(GPIO_7,input);

void showLeds(int cor){
	vetorLeds[cor]->setValue(high);
	usleep(tLed);
	vetorLeds[cor]->setValue(low);
	usleep(tLed);
}



void readBT(BlackGPIO* bt, int cor){
	string i ;
	while(!fimJogo){
		i = bt->getValue();
		if(i == "1" && btCollect){
			vetorLeds[cor]->setValue(high);
			while(i == "1") i = bt->getValue();
			vetorLeds[cor]->setValue(low);
			User_Seq[PosInput] = cor;
			if (User_Seq[PosInput]!=seq_jogo[PosInput]){
				fimJogo = true;
				break;
			}
			PosInput++;	
		}
	}
}


// Mostra no Display de 7 segmentos
void NumDisplay(vector<BlackGPIO*> array, char n ){
	for (int i=0; i<7; i++) array[i]->setValue(low);
	switch (n){
		case '0':
			array[0]->setValue(high);
			array[1]->setValue(high);
			array[2]->setValue(high);
			array[3]->setValue(high);
			array[4]->setValue(high);
			array[5]->setValue(high);
			break;
		case '1':
			array[1]->setValue(high);
			array[2]->setValue(high);			
			break;
		case '2':
			array[0]->setValue(high);
			array[1]->setValue(high);
			array[3]->setValue(high);
			array[4]->setValue(high);
			array[6]->setValue(high);
			break;
		case '3':
			array[0]->setValue(high);
			array[1]->setValue(high);
			array[2]->setValue(high);
			array[3]->setValue(high);
			array[6]->setValue(high);
			break;
		case '4':
			array[1]->setValue(high);
			array[2]->setValue(high);
			array[5]->setValue(high);
			array[6]->setValue(high);
			break;
		case '5':
			array[0]->setValue(high);
			array[2]->setValue(high);
			array[3]->setValue(high);
			array[5]->setValue(high);
			array[6]->setValue(high);
			break;
		case '6':
			array[0]->setValue(high);
			array[2]->setValue(high);
			array[3]->setValue(high);
			array[4]->setValue(high);
			array[5]->setValue(high);
			array[6]->setValue(high);
			break;
		case '7':
			array[0]->setValue(high);
			array[1]->setValue(high);
			array[2]->setValue(high);
			break;
		case '8':
			array[0]->setValue(high);
			array[1]->setValue(high);
			array[2]->setValue(high);
			array[3]->setValue(high);
			array[4]->setValue(high);
			array[5]->setValue(high);
			array[6]->setValue(high);
			break;
		case '9':
			array[0]->setValue(high);
			array[1]->setValue(high);
			array[2]->setValue(high);
			array[3]->setValue(high);
			array[5]->setValue(high);
			array[6]->setValue(high);
			break;
		case 'a'://A
			array[0]->setValue(high);
			array[1]->setValue(high);
			array[2]->setValue(high);
			array[4]->setValue(high);
			array[5]->setValue(high);
			array[6]->setValue(high);
			break;
		case 'e'://E
			array[0]->setValue(high);
			array[3]->setValue(high);
			array[4]->setValue(high);
			array[5]->setValue(high);
			array[6]->setValue(high);
			break;
	}
}

void ReInic(BlackGPIO* bt){
	string i;
	time_t InicT, FimT;
	if(fimJogo == true){
		fimJogo=false;
	}
	while(!fimJogo){
		i = bt->getValue();
		if (i == "1"){
			lvl = 1;
			restart = true;
			start = true;
			NumDisplay(vetorDisp,'8');
			usleep(tLed*3);
			NumDisplay(vetorDisp,'q');
			time(&InicT);
			while( i=="1" ){
				i = bt->getValue();
				time(&FimT);
				if( FimT-InicT >= TempoDesligar){
					restart = false;
				}
			}
		}
	}
}

int main(int argc, char * argv[]){
//Carrega vetor de leds
	vetorLeds.push_back(&ledR);
	vetorLeds.push_back(&ledG);
	vetorLeds.push_back(&ledB);
	vetorLeds.push_back(&ledY);
	//Carrega vetor com Leds do Display
	vetorDisp.push_back(&dispA);
	vetorDisp.push_back(&dispB);
	vetorDisp.push_back(&dispC);
	vetorDisp.push_back(&dispD);
	vetorDisp.push_back(&dispE);
	vetorDisp.push_back(&dispF);
	vetorDisp.push_back(&dispG);
	//inicia a thread dos botoes
	thread red(readBT,&btR,0);
	thread green(readBT,&btG,1);
	thread blue(readBT,&btB,2);
	thread yellow(readBT,&btY,3);
	thread reset(ReInic,&btReset);
	
	
	int lvl = 0;
	tLed = 1200000;
	seq_jogo = new int[nLvl];
	
	bool start = false;
	bool fimTempo = false;
	time_t LimTime, InicTime, CurrentTime, VarTime;
	
	NumDisplay(vetorDisp,'8');

	while(!fimJogo && lvl<nLvl && !restart && start){
		NumDisplay(vetorDisp,lvl+48);
		User_Seq = new int[nLvl];
		
		for(int i = 0; i<lvl; i++){
			showLeds(seq_jogo[i]);
		}

		LimTime = 4*lvl*tLed/1000000;
		PosInput = 0;
		btCollect = true;
		
		time(&InicTime);
		VarTime = InicTime;	
		while( PosInput<= lvl && !fimJogo && !fimTempo && !restart){
			time(&CurrentTime);
			if( CurrentTime - VarTime > LimTime){
				fimTempo = true;
			}
		}
		btCollect=false;		
		lvl++;
		seq_jogo[lvl]=rand()%4;
		delete[] User_Seq;

		if(fimJogo || lvl == 10){
			time(&InicTime);
			while(fimJogo && !fimTempo){
				NumDisplay(vetorDisp,'a');
				time(&CurrentTime);
					if( CurrentTime - InicTime >= TempoDesligar){
					fimTempo = true;
					NumDisplay(vetorDisp,'e');
					break;
				}
			}
		}
	}
	red.join();
	green.join();
	blue.join();
	yellow.join();
	reset.join();

	delete[] User_Seq;
	delete[] seq_jogo;
	
	return 0;
}