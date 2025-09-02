Para facilitar o desenvolvimento de aplicações a serem hospedadas na placa CTRL-28BYJ48 foi criada uma biblioteca de nome X28BYJ48.h, disponível em https://github.com/izyino/X28BYJ48.h, a qual dispõe das seguintes funções:
--------------------------------------------------------------------------------------------------------

#include <X28BYJ48.h>

para incluir a biblioteca ao programa. Dependendo de onde a biblioteca estiver gravada, pode-se usar alternativamente o formato #include “X28BYJ48.h”


--------------------------------------------------------------------------------------------------------
28BYJ48 x(t);

comando construtor que deve ser informado logo após o include, sendo t uma variável do tipo uint8_t que define o tipo de motor conectado a CN1, sendo possível os seguintes valores:

1 – Para motor 28byj-48, 2048 passos por volta, baixo torque, baixo consumo<br>
2 – Para motor 28byj-48, 2048 passos por volta, alto torque, alto consumo<br>
3 – Para motor 28byj-48, 4096 passos por volta, médio torque, médio consumo<br>

--------------------------------------------------------------------------------------------------------
x.begin();

inicializa as diversas funções da biblioteca. Deve ser colocado na sessão de setup de todos os programas que se utilizem da biblioteca 

--------------------------------------------------------------------------------------------------------
x.runStep(steps, velstep, cwstep);

comando que ativa o motor de passo, de forma automática e assíncrona, conforme as seguintes variáveis:

steps – variável uint32_t contendo o número de passos a movimentar

velstep – variável unint8_t que define a velocidade da movimentação, em rotações por minuto (RPM). Valores entre 1 e 16 RPM costumam ser aceitáveis

cwstep – variável booleana que define o sentido da movimentação, sendo “true” para sentido horário e “false” para sentido anti-horário 

--------------------------------------------------------------------------------------------------------
x.where();

esta função retorna no formato uint32_t o número de passos ainda restantes para que o motor chegue ao seu destino. Zero significa que o motor já chegou ao seu último destino e já encontra-se parado. Antes de comandar qualquer movimentação deve-se consultar esta função para ter certeza que o motor encontra-se parado

--------------------------------------------------------------------------------------------------------
x.beep(bnum, bdur, bfreq, binter);

comando que ativa a emissão de beeps sonoros, de forma automática e assíncrona, conforme as seguintes variáveis:

bnum – variável inteira que especifica o número de beeps a serem emitidos

bdur – variável inteira que especifica a duração de cada beep, em milisegundos 

bfreq – variável inteira que especifica a freqüência dos beeps, em Hertz (Hz)

binter – variável inteira que especifica a duração da pausa entre os beeps, em milisegundos 

--------------------------------------------------------------------------------------------------------
x.led(lnum, ldur, linter);

comando que ativa piscadas do Led, de forma automática e assíncrona, conforme as seguintes variáveis:

lnum – variável inteira que especifica o número de piscadas a serem emitidas

ldur – variável inteira que especifica a duração do Led acesso em cada piscada, em milisegundos 

linter – variável inteira que especifica a duração do Led apagado em cada piscada, em milisegundos 

--------------------------------------------------------------------------------------------------------
x.setms(yms);

comando para inicializar o contador de milisegundos com o valor informado pela variável yms do tipo uint32_t. Imediatamente após inicializado, o contador começa ser subtraído de 1 a cada milisegundo

--------------------------------------------------------------------------------------------------------
x.getms();

esta função retorna no formato uint32_t o estado antes do contador de milisegundos previamente inicializado pelo comando x.setms. Serve como alternativa para a função delay(), de forma assíncrona

--------------------------------------------------------------------------------------------------------
x.stopStep();

esta função interrompe o movimento do motor de passo

--------------------------------------------------------------------------------------------------------
x.stopBeep();

esta função interrompe a emissão de beeps sonoros

--------------------------------------------------------------------------------------------------------
x.stopLed();

esta função interrompe as piscadas do Led<br><br>

#######################################################################################<br>
<b><font size=6>Exemplos de utilização da biblioteca</font></b>

No início do programa:<br>
#include <X28BYJ48.h><br>
X28BYJ48 x(2);

na sessão do setup:<br>
x.begin();


--------------------------------------------------------------------------------------------------------
//movimenta o motor de passo tipo 28BYJ-48, 
//velocidade 3, sentido horário, 2048 passos:

//função principal:<br>
x.runStep(2048, 3, true);<br>
//o motor começa a se movimentar imediatamente após a função runStep ser chamada

//para saber se o motor de passo já chegou ao destino, fazer<br>
if (x.where()>0) {ainda não chegou ao destino. Está em movimento...};

//a qualquer momento o movimento do motor de passo pode ser interrompido<br>
x.stopStep();


--------------------------------------------------------------------------------------------------------
//emite 10 beeps de 2KHz de 0,5s com pausa interbeeps de 0,25s:

//função principal:<br>
x.beep(10, 500, 2000, 250);<br>
//os beeps começam a ser emitidos imediatamente após a função beep ser chamada

//a qualquer momento a emissão dos beeps sonoros pode ser interrompida<br>
x.stopBeep();


--------------------------------------------------------------------------------------------------------
//pisca o Led 50 vezes com 0,25s aceso seguido de 0,10s apagado: 

//função principal:<br>
x.led(50, 250, 100);<br>
//o led começa a piscarimediatamente após a função led ser chamada

//a qualquer momento as piscadas do Led podem ser interrompidas<br>
x.stopLed();


--------------------------------------------------------------------------------------------------------
//contagem de 4 segundos, de forma assíncrona:

//função principal:<br>
x.setms(4000);<br>
while (x.getms()>0){enquanto espera 4s, pode fazer coisas…}<br>
//a variável x.xms começa a ser decrementada a cada um milisegundo imediatamente após a função setms ser chamada

--------------------------------------------------------------------------------------------------------

Programas contidos no diretório “examples” da biblioteca


Exemplo n.1:

utilização das funções principais presentes na biblioteca X28BYJ48.h, para controle do motor, beep, led e timer nas placas de circuito impresso modelo CTRL-28BYJ48 versão 1.1 e posteriores. Pressupõe um motor de passo 28byj48 conectado a CN1. O programa faz movimentos repetitivos do motor, dando duas voltas a cada repetição, emitindo a cada ciclo dois beeps e três piscadas rápidas do led azul.





Exemplo n.2:

exemplo de utilização da placa de circuito impresso modelo CTRL-28BYJ48 versão 1.1 e posteriores, como servidor web na modalidade “access point”. Pressupõe um motor de passo 28byj48 conectado a CN1. 



Exemplo n.3:

utilização das funções principais presentes na biblioteca X28BYJ48.h, associadas a comunicação WiFi, incluindo um servidor web, em placas de circuito impresso modelo CTRL-28BYJ48 versão 1.1 e posteriores. Pressupõe um motor de passo 28byj48 conectado a CN1.

Na primeira execução deve-se conectar via access point na rede X28BYJ48 com a senha 00000000. Acessar então o IP 192.168.4.1. Em resposta, o programa exibe uma tela contendo a lista de redes WiFi ao alcance, para que o usuário informe qual rede será utilizada, juntamente com a senha correspondente. Dependendo do número de redes ao alcance, pode demorar um certo tempo até que a lista seja exibida.

Informar então o número da rede escolhida e a sua senha, seguido por um clique no botão "submit". Feito isso, basta apontar o browser para o IP fixo normalmente igual a 192.168.1.99 ou outro, conforme informado pelo monitor serial.



Exemplo n.4:

utilização das funções principais presentes na biblioteca X28BYJ48.h, associadas a comunicação WiFi, incluindo um servidor web, em placas de circuito impresso modelo CTRL-28BYJ48 versão 1.1 e posteriores.

Esse exemplo mostra o esboço de um programa para controle de alimentação de animais via WiFi. Pressupõe um motor de passo 28byj48 conectado a CN1, supostamente usado para despejar uma dose de ração a cada 45 graus girados.
 
Acessar via browser o IP informado no monitor serial. Fazer então a programação da alimentação, de até 4 vezes ao dia, informando para cada refeição: hora, minuto e a quantidade de doses (uma dose=45graus).

IMPORTANTE: Deve-se rodar antes, pelo menos uma vez, o programa exemplo n.3  para que o nome da rede WiFi a ser utilizada, juntamente com a sua senha fiquem armazenadas na memória flash do ESP32 C3 supermini

--------------------------------------------------------------------------------------------------------

















