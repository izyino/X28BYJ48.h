
//
// EXEMPLO n.3 de utilização da biblioteca X28BYJ48.h
// com WiFi no modo station, para manter acesso a internet
// ------------------------------------------------------------------
// Pressupõe um motor de passo 28byj48 conectado a CN1
// usado para despejar uma dose de ração a cada 45graus girados
// Acessar via browser o IP informado no monitor serial. Fazer então a
// programação da alimentação, de até 4 vezes ao dia, informando para
// cada refeição: hora, minuto e qtde de doses (uma dose=45graus)
//
// IMPORTANTE: Deve-se rodar antes, pelo menos uma vez, o exemplo n.2
// para que o nome da rede WiFi a ser utilizada, juntamente com a sua
// senha fiquem armazenadas na memória flash do ESP32 C3 supermini
// ---------------------------------------------------------------
//

#include <WiFi.h>
#include <EEPROM.h>
#include <WebServer.h>
#include <WiFiClient.h>
#include <TimeLib.h>  
#include <WiFiAP.h>
#include <X28BYJ48.h>

//#include "html_unico.h"

WebServer server(80);

X28BYJ48 x(2);

String ssid,pass;

String hora;
byte   versao[4]={1,1,7,24};            //versão 1.1, julho, 2024
byte   serviu[4]={0,0,0,0};
bool   chntp=false,chwifi=false;
byte   h,m,s,parx[128];
int    n,i,k;

const char* ntpServer = "pool.ntp.org"; //servidor NTP
const long  gmtOffset_sec = -10800;     //gmt-3 (em segundos
const int   daylightOffset_sec = 0;     //horário de verão

const char uni_page[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
  <head>
    <title>Configuração ALIMENTADOR</title>
    <meta charset="utf-8" />
    <meta name="viewport" content="width=device-width, initial-scale=1.0" />
    <style>
      input {
        caret-color: red;
      }

      body {
        margin: 10px;
        width: 100vw;
        background: #ecf0f3;
        display: flex;
        align-items: center;
        text-align: center;
        justify-content: center;
        place-items: center;
        overflow: hidden;
        font-family: Arial, Helvetica, sans-serif;
        overflow-y: visible;
      }

    h1 {
      font-size: 18px;
      font-weight: 600;
      text-align: center;
      color: #3594ff;
    }


    h2 {
      font-size: 1.2rem;
      text-align: center;
      color: #1da1f2;
    }

    h3 {
      font-size: 1.2rem;
      text-align: center;
      line-height: 36px;
      color: #1da1f2;
    }

    p {
    font-size: 12px;
    text-indent: 10px;
    text-align: justify;
    }

      .painel {
        margin-top:5px;
        margin-left:10px;
        margin-button:10px;
        margin-right:10px;
        position: relative;
        width: 300px;
        height: 550px;
        border-radius: 20px;
        padding: 20px;
        box-sizing: border-box;
        background: #ecf0f3;
        box-shadow: 14px 14px 20px #cbced1, -14px -14px 20px white;
      }

      .inputs {
        text-align: center;
        margin-top: 30px;
        margin-bottom: 5px;
        width: 100%;
      }

      label,
      input,
      button {
        text-align: center;
        display: block;
        width:100%;
        padding: 0;
        border: none;
        outline: none;
        box-sizing: border-box;
      }

      label {
        margin-bottom: 0px;
      }

      label:nth-of-type(2) {
        margin-top: 5px;
      }

      input::placeholder {
        color: gray;
      }


      .titulo {
        font-size: 14px;
        text-align: center;
        font-weight: 600;
        padding: 2px;
        border: none;
        outline: none;
        color: black;
      }

      input {
        background: #ecf0f3;
        padding: 5px;
        padding-left: 20px;;
        margin-top: 5px;
        height: 50px;
        font-size: 30px;
        border-radius: 50px;
        box-shadow: inset 6px 6px 6px #cbced1, inset -6px -6px 6px white;
      }
      .i200 {
        height: 36px;
        width: 72px;    //260px;
      }

      button {
        color: #707070;
        margin-top: 13px;
        background: #9dc9ff;
        height: 70px;
        width: 100%;
        padding-top: 0px;
        border-radius: 25px;
        cursor: pointer;
        font-weight: 900;
        font-size: 20px;
        box-shadow: 6px 6px 6px #cbced1, -6px -6px 6px white;
        transition: 0.5s;
      }
    
      button:hover {
        box-shadow: none;
      }

      .knob {
        color: white;
        font-size: 1.8rem;
        align-items: center;
        background: #9dc9ff;
        line-height: 50px;
        height: 50px;
        width: 50px;
        margin: 5px;
        cursor: pointer;
        font-weight: 900;
        box-shadow: 6px 6px 6px #cbced1, -6px -6px 6px white;
        transition: 0.5s;
      }

      .knob:hover {
        box-shadow: none;
      }

      .knob1 {
        margin: 0;  
        color: white;
        font-size: 1.8rem;
        text-align: center;
        background: #9dc9ff;
        line-height: 50px; 
        height: 50px;
        width: 50px;
        margin: 5px;
        cursor: pointer;
        font-weight: 900;
        box-shadow: 6px 6px 6px #cbced1, -6px -6px 6px white;
        transition: 0.5s;
      }

      .knob1:hover {
        box-shadow: none;
      }

     
    .ib {
      display:flex;
      flex-direction: row;
      align-items: center;
      text-align: center;
      justify-content: center;
      place-items: center;
    }

      a {
        position: absolute;
        font-size: 8px;
        text-align: center;
        bottom: 4px;
        right: 4px;
        padding: 6px;
        text-decoration: none;
        color: black;
        border-radius: 5px;
      }

      h2 {
        font-size: 8px;
        text-align: center;
        bottom: 4px;
        right: 4px;
        padding: 6px;
        text-decoration: none;
        color: black;
      }
    </style>
  
  </head>
  
  <body onload="lepref()"> <!- Lê dados da EEPROM assim que carregar o HTML ->
  <main>
  
    <section id="inicial" class="painel" style="display:block">
    <h1>ALIMENTADOR<br>Rev. 1.1 - by ART&JCSG<br></h1>
      <div class="button">
        <button title="Define o programa de alimentação - Tela 1" onclick="mudapag('inicial','pref1')" class="button">Programa<br>de alimentação<label class=titulo></label></button>
        <button title="Altera as configurações e as preferências - Tela 2" onclick="mudapag('inicial','pref2')" class="button">Configurações<br>e preferências<label class=titulo></label></button>
        <button title="" onclick="" class="button"><label class=titulo><br></label></button>
        <button title="" onclick="" class="button"><label class=titulo><br></label></button>
        <button title="" onclick="" class="button"><label class=titulo><br></label></button>        
      </div>
      <h2><br>ALIMENTADOR - Rev. <span id="vers0">x.y</span></h2>
    </section>

    
    <section id="pref1" class="painel" style="display:none">
      <table border="0" width="100%" id="table1" cellspacing="0" cellpadding="0">
        <tr>
          <td><h3>TELA 1<h3></td>
          <td class="ib">
            <button type="button" title="Confirma a programação abaixo" class="knob" onclick="grava()" class="button">&#9658;</button>
            <button type="button" title="Não altera nada e volta para a tela inicial" class="knob" onclick="mudapag('pref1','inicial')" class="button">&#8617;</button>
          </td>
        </tr>
      </table>
      <form class="inputs" >
        <label class=titulo>1ª REFEIÇÃO<br>&nbsp;hora&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;minuto&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;dose&nbsp;</label><div class="ib"><input class="i200" type="number" id="hora1" step="1" min="0" max="23" /><input class="i200" type="number" id="min1" step="1" min="0" max="59" />&nbsp;&nbsp;&nbsp;&nbsp;<input class="i200" type="number" id="dose1" step="1" min="1" max="20" /></div>
        <label class=titulo><br>2ª REFEIÇÃO<br>&nbsp;hora&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;minuto&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;dose&nbsp;</label><div class="ib"><input class="i200" type="number" id="hora2" step="1" min="0" max="23" /><input class="i200" type="number" id="min2" step="1" min="0" max="59" />&nbsp;&nbsp;&nbsp;&nbsp;<input class="i200" type="number" id="dose2" step="1" min="1" max="20" /></div>
        <label class=titulo><br>3ª REFEIÇÃO<br>&nbsp;hora&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;minuto&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;dose&nbsp;</label><div class="ib"><input class="i200" type="number" id="hora3" step="1" min="0" max="23" /><input class="i200" type="number" id="min3" step="1" min="0" max="59" />&nbsp;&nbsp;&nbsp;&nbsp;<input class="i200" type="number" id="dose3" step="1" min="1" max="20" /></div>
        <label class=titulo><br>4ª REFEIÇÃO<br>&nbsp;hora&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;minuto&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;dose&nbsp;</label><div class="ib"><input class="i200" type="number" id="hora4" step="1" min="0" max="23" /><input class="i200" type="number" id="min4" step="1" min="0" max="59" />&nbsp;&nbsp;&nbsp;&nbsp;<input class="i200" type="number" id="dose4" step="1" min="1" max="20" /></div>
      </form>
      <h2><br>ALIMENTADOR - Rev. <span id="vers1">x.y</span></h2>
    </section>


    <section id="pref2" class="painel" style="display:none">
      <table border="0" width="100%" id="table2" cellspacing="0" cellpadding="0">
        <tr>
          <td><h3>TELA 2<h3></td>
          <td class="ib">
            <button type="button" title="Confirma os valores abaixo" class="knob" onclick="grava()" class="button">&#9658;</button>
            <button type="button" title="Não altera nada e volta para a tela inicial" class="knob" onclick="mudapag('pref2','inicial')" class="button">&#8617;</button>
          </td>
        </tr>
      </table>
      <form class="inputs" >
        <label class=titulo>PARÂMETRO 1<br>xxxxxxxxxxxxxx</label><div class="ib"><input class="i200" type="number" id="parm1" step="1" min="0" max="255" /></div>
        <label class=titulo>PARÂMETRO 2<br>xxxxxxxxxxxxxx</label><div class="ib"><input class="i200" type="number" id="parm2" step="1" min="0" max="255" /></div>
        <label class=titulo>PARÂMETRO 3<br>xxxxxxxxxxxxxx</label><div class="ib"><input class="i200" type="number" id="parm3" step="1" min="0" max="255" /></div>
        <label class=titulo>PARÂMETRO 4<br>xxxxxxxxxxxxxx</label><div class="ib"><input class="i200" type="number" id="parm4" step="1" min="0" max="255" /></div>        
       </form>
    <h2><br><br><br><br>NOMENOME - Rev. <span id="vers2">x.y</span></h2>
    </section>


    <script> 
    // ===================== leitura das preferências atuais ===========================================
      function lepref() {
        var eeprom;
        var meses="Janeiro;Fevereiro;Março;Abril;Maio;Junho;Julho;Agosto;Setembro;Outubro;Novembro;Dezembro";
        meses=meses.split(";");
        var xhttp = new XMLHttpRequest();
        xhttp.onreadystatechange = function () {
          if (this.readyState == 4 && this.status == 200) {
            eeprom = this.responseText.split("&");

            document.getElementById("parm1").value=eeprom[76];
            document.getElementById("parm2").value=eeprom[77];
            document.getElementById("parm3").value=eeprom[78];
            document.getElementById("parm4").value=eeprom[79];
            
            document.getElementById("hora1").value=eeprom[80];
            document.getElementById("min1").value=eeprom[81];
            document.getElementById("dose1").value=eeprom[82];
            document.getElementById("hora2").value=eeprom[83];
            document.getElementById("min2").value=eeprom[84];
            document.getElementById("dose2").value=eeprom[85];
            document.getElementById("hora3").value=eeprom[86];
            document.getElementById("min3").value=eeprom[87];
            document.getElementById("dose3").value=eeprom[88];
            document.getElementById("hora4").value=eeprom[89];
            document.getElementById("min4").value=eeprom[90];
            document.getElementById("dose4").value=eeprom[91];

            var vers=eeprom[92] + "." + eeprom[93] + " - by ART&JCSG - " + meses[eeprom[94]] + "/20" + eeprom[95];
            
            document.getElementById("vers0").innerHTML=vers;
            document.getElementById("vers1").innerHTML=vers;
            document.getElementById("vers2").innerHTML=vers;

          };  
        };
        xhttp.open("GET", "lepar", true);
        xhttp.send();
      };


    // ===================== muda página (da página que vem, para a página que vai) ===================
      function mudapag(xvem, xvai) {
        var vemx = document.getElementById(xvem);
        var vaix = document.getElementById(xvai);
        vaix.style.display = "block";
        vemx.style.display = "none";
      };


    // ===================== grava preferências ===========================================
    function grava() {
      var msg ="";

      var parm1 = document.getElementById("parm1").value;
      var parm2 = document.getElementById("parm2").value;
      var parm3 = document.getElementById("parm3").value;
      var parm4 = document.getElementById("parm4").value;
      msg = "0="+parm1+"&1="+parm2+"&2="+parm3+"&3="+parm4;
      
      var hora1 = document.getElementById("hora1").value;
      var min1 = document.getElementById("min1").value;
      var dose1 = document.getElementById("dose1").value;
      var hora2 = document.getElementById("hora2").value;
      var min2 = document.getElementById("min2").value;
      var dose2 = document.getElementById("dose2").value;
      var hora3 = document.getElementById("hora3").value;
      var min3 = document.getElementById("min3").value;
      var dose3 = document.getElementById("dose3").value;
      var hora4 = document.getElementById("hora4").value;
      var min4 = document.getElementById("min4").value;
      var dose4 = document.getElementById("dose4").value;
      msg = msg + "&4="+hora1+"&5="+min1+"&6="+dose1+"&7="+hora2+"&8="+min2+"&9="+dose2+"&10="+hora3+"&11="+min3+"&12="+dose3+"&13="+hora4+"&14="+min4+"&15="+dose4;

        
      var xhttp = new XMLHttpRequest();
      xhttp.onreadystatechange = function() { // The readystatechange event is fired whenever the readyState property of the XMLHttpRequest changes.
        if (this.readyState == 4 && this.status == 200) { // 4 = The operation is complete.
          document.getElementById("mensagem").innerHTML = this.responseText;
        };
      };
      xhttp.open("GET", "gravapar?"+msg, true);
      xhttp.send();
    };

    </script>

  </body>
  
</html>
)=====";


void setup() {
  Serial.begin(115200);
  EEPROM.begin(128);
  x.begin();
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  x.beep(1, 200, 2000, 0);  //emite 1 beep de 200ms cada, 2000Hz
  x.led(10, 50, 25);        //pisca o LED 10 vezes com 50ms aceso e 25ms apagado

  EEPROM.write(92, versao[0]);EEPROM.write(93, versao[1]);
  EEPROM.write(94, versao[2]);EEPROM.write(95, versao[3]);
  EEPROM.commit();

  delay(5000);
  Serial.println("");
  Serial.println("Alimentador de animais - Programa teste n.3");
  Serial.println("-------------------------------------------");
  Serial.println("");

  dumpflash();

  initudo();
  pegahora();
  Serial.print("Hora de início: ");Serial.println(hora);
}

void loop() {
  server.handleClient();
  if ((millis()%1000)==0){           //a cada segundo verifica se deve servir...
    pegahora();
    for (k=0; k<4; k++){
      if ((h==parx[80+k*3])&&(m==parx[81+k*3])&&(serviu[k]==0)&&(x.where()==0)){
        x.beep(3, 1000, 2000, 1000); //beep e pisca
        x.led(10, 50, 25);           //para avisar que vai servir 
        uint32_t nsteps=map(parx[82+k*3]*45,0,360,0,2048);  
        x.runStep(nsteps, 5, 1);  //roda motor 45 graus * doses a 5RPM
        while (x.where()!=0){}      //espera rodar 
        serviu[k]=1;
        if (k>0){serviu[k-1]=0;}else{serviu[3]=0;}
        Serial.print("Serviu: ");Serial.print(parx[82+k*3]);
        Serial.print(" doses, as ");Serial.println(hora);
      }
    }
  }
}

void initudo(){
  ssid="";pass="";
  for (k=0; k<EEPROM.read(124); k++){ssid=ssid+char(EEPROM.read(k));}
  for (k=0; k<EEPROM.read(125); k++){pass=pass+char(EEPROM.read(k+32));}
  for (k=0; k<128; k++){parx[k]=EEPROM.read(k);}
  Serial.println("Memoria EEPROM ok. Parametros recuperados com sucesso...");
  conectawifi();
  server.begin();
  server.on("/", raiz);
  server.on("/lepar", Lepar);
  server.on("/gravapar", Gravaparm);
}

void raiz(){
  String enviapg = uni_page;
  server.send(200, "text/html", enviapg);
}

void Lepar() {
  String parmValues="";
  for (int k=0;  k<128; k++){parmValues += String(parx[k]) + "&";}
  server.send(200, "text/plane", parmValues);
};


void Gravaparm() {
  x.bdur=200;x.bfreq=2000;x.binter=0;x.bnum=1;
  x.ldur=50;x.linter=25;x.lnum=10;
  for (k=76; k<92; k++){
    parx[k] = server.arg(String(k-76)).toInt();
    EEPROM.write(k, parx[k]);
  }
  EEPROM.commit();
//  dumpflash();
  server.send(200, "text/plane", "ok");
}


void conectawifi()
{
  Serial.print("Tentando se conectar na rede ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);

  chwifi=false;
  uint32_t milisatu=millis();
  while (WiFi.status()!=WL_CONNECTED){
    Serial.print(".");delay(250);
    if ((milisatu+5000)<millis()){Serial.println("");Serial.println("Rede WiFi NÃO conectada");
      WiFi.disconnect();
      chwifi=false;break;
    }
  }

  if (WiFi.status() == WL_CONNECTED){
    Serial.println("");Serial.print("Rede WiFi conectada. IP: ");
    Serial.println(WiFi.localIP());
    chwifi=true;
  }
}


void pegahora(){
  struct tm timeinfo;
  bool chntp=false;
  k=0;
  uint32_t milisatu=millis();
  while (!getLocalTime(&timeinfo)){
    if ((k>10)||(milisatu+1000)<millis()){
      chntp=false;break;
    }  
  }

  if (getLocalTime(&timeinfo)){chntp=true;}

  hora="??:??:??";
  
  if (chntp){
    h=timeinfo.tm_hour;
    m=timeinfo.tm_min;
    s=timeinfo.tm_sec;
    hora="";

    if (h<10){hora="0"+String(h);}else{hora=hora+String(h);}
    hora=hora+":";
    if (m<10){hora=hora+"0"+String(m);}else{hora=hora+String(m);}
    hora=hora+":";
    if (s<10){hora=hora+"0"+String(s);}else{hora=hora+String(s);}
  }
} 


void dumpflash()
{
  uint8_t xbyte;
  uint16_t addr, k; char hexVal[5];
  Serial.println("");
  for (addr=0; addr<127; addr=addr+16){
    sprintf(hexVal, "%04d", addr);
    Serial.print(hexVal);Serial.print("  ");  
    sprintf(hexVal, "%04X", addr);
    Serial.print(hexVal);Serial.print("  ");  
    for (k=0; k<16; k=k+1){
      xbyte=EEPROM.read(addr+k);
      sprintf(hexVal, "%02X", (xbyte));Serial.print(hexVal);Serial.print(" ");
    }
    Serial.print(" ");
    for (k=0; k<16; k=k+1){
      xbyte=EEPROM.read(addr+k);         
      Serial.write(xbyte>31 && xbyte != 127 ? char (xbyte) : '.');
    }
    Serial.println("");
  }
  Serial.println("");
}  


//////////////////////////////////// fim ///////////////////////////////////////////////
