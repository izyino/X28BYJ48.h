
//
//  Exemplo de utilização da biblioteca Z28BYJ48.h na PCI CTRL-28BYJ48
//  ------------------------------------------------------------------
//  Por A.R.Testa, Agosto/2025
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
}

void loop() {
    
  while (x.where()!=0){}       //espera o motor chegar ao seu último destino (está parado)

  vel=random(4,17); sent=!sent;//gera velocidade entre 4 e 16 e sentido aleatórios 
  x.runStep(4096, vel, sent);  //move o motor 4096 passos (2 voltas), velocidade 10 RPM, sentido horário 
    
  x.setms(100);
  while(x.getms()>0){}         //espera 100ms (apenas para exemplo)
  x.led(3, 50, 50);            //pisca o LED 3 vezes com 50ms aceso e 50ms apagado
  x.beep(2, 200, 2000, 100);   //emite 2 beep de 200ms cada, 2000Hz, intervalo entre eles de 100ms 
}

