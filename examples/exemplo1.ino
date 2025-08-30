
//
// EXEMPLO n.1  de utilização da biblioteca X28BYJ48.h
// apenas com as funções básicas, sem utilização de WiFi
// ---------------------------------------------------------------
// utilização das funções presentes na biblioteca X28BYJ48.h,
// para controle do motor, bee, led e timer nas placas de circuito
// impresso modelo CTRL-28BYJ48 versão 1.1 e posteriores
// 

#include <Arduino.h>
#include <X28BYJ48.h>

X28BYJ48 x(2);                 //indica motor de passo no modo 2 em CN1

bool sent=false;
int  vel =0;

void setup() {
  x.begin();
  Serial.begin(115200);
  x.led(20, 100, 100);         //pisca o LED 20 vezes com 100ms aceso e 100ms apagado
  x.setms(5000);               //espera 5 segundos (apenas para exemplo)
  while(x.getms()>0){}         //durante a espera, pode-se fazer qualquer coisa
  }

void loop() {
    
  while (x.where()!=0){}       //espera o motor chegar ao seu último destino (está parado)

  vel=random(4,17); sent=!sent;//gera velocidade entre 4 e 16 e sentido aleatórios 
  x.runStep(4096, vel, sent);  //move o motor 4096 passos (2 voltas), velocidade de 4 a 16 RPM, sentido horário 
    
  x.setms(100);                //espera 100ms (apenas para exemplo)
  while(x.getms()>0){}         //durante a espera, pode-se fazer qualquer coisa
  x.led(3, 50, 50);            //pisca o LED 3 vezes com 50ms aceso e 50ms apagado
  x.beep(2, 200, 2000, 100);   //emite 2 beep de 200ms cada, 2000Hz, intervalo entre eles de 100ms 
}

