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


// Variáveis Globais
BlackADC PotSon1(AIN2);
BlackADC PotSon2(AIN6);
BlackGPIO LedSon1(GPIO_30,output);
BlackGPIO LedSon2(GPIO_31,output);
