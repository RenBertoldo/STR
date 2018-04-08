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
int tLed = 800000;
int *seq_jogo;
int *User_Seq;
int PosInput = 0;

time_t TempoDesligar = 5;

bool restart = false;
bool btCollect = false;
bool fimJogo = false;
bool erro = false;
//Declara as saídas
BlackGPIO ledR(GPIO_60,output);
BlackGPIO ledB(GPIO_51,output);
BlackGPIO ledG(GPIO_31,output);
BlackGPIO ledY(GPIO_30,output);
vector<BlackGPIO*> vetorLeds;

BlackGPIO dispA(GPIO_48,output);
BlackGPIO dispB(GPIO_2,output);
BlackGPIO dispC(GPIO_3,output);
BlackGPIO dispD(GPIO_20,output);
BlackGPIO dispE(GPIO_49,output);
BlackGPIO dispF(GPIO_4,output);
BlackGPIO dispG(GPIO_5,output);
vector<BlackGPIO*> vetorDisp;

//Declara Entradas
BlackGPIO btR(GPIO_69,input);
BlackGPIO btG(GPIO_67,input);
BlackGPIO btB(GPIO_66,input);
BlackGPIO btY(GPIO_68,input);
BlackGPIO btReset(GPIO_44,input);



void showLeds(int cor){
	vetorLeds[cor]->setValue(low);
	usleep(300000);
	vetorLeds[cor]->setValue(high);
	usleep(500000);
	vetorLeds[cor]->setValue(low);
	usleep(300000);
}

// Mostra no Display de 7 segmentos
void NumDisplay(vector<BlackGPIO*> array, char n ){
	for (int i=0; i<7; i++) array[i]->setValue(high);
	switch (n){
		case '0':
			array[0]->setValue(low);
			array[1]->setValue(low);
			array[2]->setValue(low);
			array[3]->setValue(low);
			array[4]->setValue(low);
			array[5]->setValue(low);
			break;
		case '1':
			array[1]->setValue(low);
			array[2]->setValue(low);
			break;
		case '2':
			array[0]->setValue(low);
			array[1]->setValue(low);
			array[3]->setValue(low);
			array[4]->setValue(low);
			array[6]->setValue(low);
			break;
		case '3':
			array[0]->setValue(low);
			array[1]->setValue(low);
			array[2]->setValue(low);
			array[3]->setValue(low);
			array[6]->setValue(low);
			break;
		case '4':
			array[1]->setValue(low);
			array[2]->setValue(low);
			array[5]->setValue(low);
			array[6]->setValue(low);
			break;
		case '5':
			array[0]->setValue(low);
			array[2]->setValue(low);
			array[3]->setValue(low);
			array[5]->setValue(low);
			array[6]->setValue(low);
			break;
		case '6':
			array[0]->setValue(low);
			array[2]->setValue(low);
			array[3]->setValue(low);
			array[4]->setValue(low);
			array[5]->setValue(low);
			array[6]->setValue(low);
			break;
		case '7':
			array[0]->setValue(low);
			array[1]->setValue(low);
			array[2]->setValue(low);
			break;
		case '8':
			array[0]->setValue(low);
			array[1]->setValue(low);
			array[2]->setValue(low);
			array[3]->setValue(low);
			array[4]->setValue(low);
			array[5]->setValue(low);
			array[6]->setValue(low);
			break;
		case '9':
			array[0]->setValue(low);
			array[1]->setValue(low);
			array[2]->setValue(low);
			array[3]->setValue(low);
			array[5]->setValue(low);
			array[6]->setValue(low);
			break;
		case 'a'://A
			array[0]->setValue(low);
			array[1]->setValue(low);
			array[2]->setValue(low);
			array[4]->setValue(low);
			array[5]->setValue(low);
			array[6]->setValue(low);
			break;
		case 'e'://E
			array[0]->setValue(low);
			array[3]->setValue(low);
			array[4]->setValue(low);
			array[5]->setValue(low);
			array[6]->setValue(low);
			break;
		case 'q':
			array[0]->setValue(high);
			array[1]->setValue(high);
			array[2]->setValue(high);
			array[3]->setValue(high);
			array[4]->setValue(high);
			array[5]->setValue(high);
			array[6]->setValue(high);
			break;
	}
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
			if(User_Seq[PosInput]!=seq_jogo[PosInput]){
				erro = true;
				NumDisplay(vetorDisp,'a');
				break;
			}
			PosInput++;
		}
	}
}

void ReInic(BlackGPIO* bt){
	string i;
	while(!fimJogo){
		i = bt->getValue();
		if (i == "1"){
			restart = true;
			NumDisplay(vetorDisp,'a');
			while( i=="1" )
				i = bt->getValue();
			}
			if (erro) {
				break;
			}

		}
	}

int main(int argc, char * argv[]){
	//Carrega vetor de leds
	vetorLeds.push_back(&ledB);
	vetorLeds.push_back(&ledG);
	vetorLeds.push_back(&ledY);
	vetorLeds.push_back(&ledR);
	//Carrega vetor com Leds do Display
	vetorDisp.push_back(&dispA);
	vetorDisp.push_back(&dispB);
	vetorDisp.push_back(&dispC);
	vetorDisp.push_back(&dispD);
	vetorDisp.push_back(&dispE);
	vetorDisp.push_back(&dispF);
	vetorDisp.push_back(&dispG);
	//inicia a thread dos botoes
	thread red(readBT,&btR,3);
	thread green(readBT,&btG,1);
	thread blue(readBT,&btB,0);
	thread yellow(readBT,&btY,2);
	thread reset(ReInic,&btReset);


	lvl = 1;
	tLed = 800000;
	seq_jogo = new int[nLvl];
	//int erro = 0;
	//bool resp;
	bool fimTempo = false;
	time_t LimTime, InicTime, CurrentTime, VarTime;

	NumDisplay(vetorDisp,'8');


	while(!fimJogo && lvl<nLvl){

		NumDisplay(vetorDisp,lvl+48);
		User_Seq = new int[lvl];

		for(int k=0; k<lvl;k++){
			User_Seq[k]=10;
		}
		PosInput = 0;

		//cria a sequencia
		srand((unsigned)(time)(NULL));
		seq_jogo[lvl-1]=rand()%4;

		for(int i = 0; i<lvl; i++){
			showLeds(seq_jogo[i]);
		}

		LimTime = 2*(lvl)*tLed/1000000;
		fimTempo = false;

		btCollect = true;

		time(&InicTime);
		VarTime = InicTime;
		while( PosInput< lvl && !fimJogo && !fimTempo){
			time(&CurrentTime);
			if( CurrentTime - VarTime > LimTime){
				fimTempo = true;
			}
		}

		btCollect=false;
		if(fimTempo){
			for(int k=0;k<lvl;k++){
				if(User_Seq[k] == 10){
					erro = true;
					break;
				}
				if(User_Seq[k]-seq_jogo[k]!=0){
					erro = true;
				}
			}
		}
		lvl++;
		fimTempo = false;

		delete[] User_Seq;

		if(erro || lvl == 10){
			time(&InicTime);
			time(&CurrentTime);
			while(CurrentTime - InicTime <= 6){
				NumDisplay(vetorDisp,'a');
				time(&CurrentTime);
				if(restart){
					erro = false;
					lvl = 1;
					delete[] seq_jogo;
					seq_jogo = new int[nLvl];
				}
				else{
					fimJogo = true;
					NumDisplay(vetorDisp,'q');
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
