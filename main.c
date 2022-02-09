#define RELE PORTC.F2
#define Button_RELE PORTC.F4
void cleanBufferRx();
#include "ec200.h"
#include "mqtt.h"
#include "rf_module.h"
#include "pzem004t.h"
 
 unsigned char bufferRx[80]={0};
 unsigned char indexBufferRx = 0;
 unsigned char msj[10];
 unsigned char *dataRF;
 unsigned int contCurrentFlag = 0;
 unsigned int contTimer1 = 0;
 unsigned char currentFlag = 0;
 unsigned char motobombaFlag=0;
 float currentRMS = 0;
 unsigned char k=0;
 unsigned char errorPZEMFlag=0;
 int waterLevel = 30; //valor para que no se tenga un tanque vacio

 void Interrupt(){
  INTCON.GIE = 0;

  if(PIR1.RCIF){ //Interrupcion por UART
   do{
    bufferRx[indexBufferRx] = UART1_Read();
    indexBufferRx++;
   }while( UART1_Data_Ready() );

    
   PIR1.RCIF = 0;
  }
  if(INTCON.TMR0IF){ //Interrupcion por TIMER 0
   INTCON.TMR0IF = 0;
   contCurrentFlag++;
   if(contCurrentFlag == 625){  // (16mS)(625) = 10segs, enviar datos PZEM004t
    currentFlag = 1;
    contCurrentFlag = 0;
   }
   TMR0L = 6;
  }
  
  if(PIR1.TMR1IF){ //Interrupcion por TIMER 1
     PIR1.TMR1IF = 0;
     contTimer1++;
     if(contTimer1 >= 4){  // 125mS*(1) = 0.5 segundos
      T1CON.TMR1ON = 0; //para el conteo del TIMER1
      contTimer1=0;
      Soft_UART_Break();// Si el Soft_UART_Read() del PZEM004t queda atascado por 1miliseg, que haga un break para seguir el flujo del programa
      errorPZEMFlag = 1;
     }
     TMR1H = 0b00001011;//Timer1 de 16bits comienza en 3036
     TMR1L = 0b11011100;
  }

  INTCON.GIE = 1;
 }

void main() {
 delay_ms(30000);
 UART1_Init(115200);
 Init_EC200();
 Init_RF();
 delay_ms(500);
 Init_MQT("40.71.125.21", "GrupoIoTClient");//"40.71.125.21"
 subMQTT("motobomba");
 delay_ms(500);
 
 /*Configuracion para interrupcion por UART, TIMER0, TIMER1*/
 INTCON = 0b11100000;//Global Interrupt, interrupciones por perisfericos habilitadas,Overflow por Timer0(INTCON.GIE=1, PEIE=1, TMR0IE=1)
 PIE1   = 0b00100001; // Flag PIE1.RCIE (bit5), para habilitar las interrupciones por Rx UART - TMR1 Overflow Interrupt Enable bit(bit0)
 PIR1   = 0b00000000; // el(PIR1.RCIF) nos indica si llega algo al Rx UART, el (PIR1.TMR1IF)desbordamiento de timer1
 //IPR1   = 0b00100000; // USART RX Interrupt Priority bit  (bit5)
 T0CON = 0b01000111; // Fosc/4 -> 16Mhz/4 = 4Mhz,     prescaler 1:256
 T1CON = 0b00110000; //Fosc/4, preescaler 1:8
 TMR0L = 6;
 TMR1H = 0b00001011;//Timer1 de 16bits comienza en 3036
 TMR1L = 0b11011100;
 /*********************/
 
 /***Configuracion de Puertos***/
 TRISC.F3 = 0; //Salida LED
 PORTC.F3 = 1; //LED indicador ON
 TRISC.F2 = 0;
 PORTC.F2 = 1;//RELÉ para encendido/apagado de motobomba. El modulo relé se enciende con un 0
 
 TRISC.F4=1; //Boton motobomba ON/OFF
 /**********/

 cleanBufferRx();
 delay_ms(500);
 
 T0CON.TMR0ON = 1; //Comienza a contar el timer0
 while(1){

  if(pending_data_RF()){
   bufferRF[4]='\0'; //Cambiando formato de #123# a solo enviar el dato 123
   sendMQTT(&bufferRF[1],"water");
   waterLevel = StrToInt(&bufferRF[1]);
   for (k=0; k<5;k++)
    bufferRF[k] = '\0';
   delay_ms(100);
  }


  if(currentFlag){
   currentFlag = 0;
   PZEM_Read_All();
   delay_ms(10);
   FloatToStr(voltajePZEM/10.0, msj);
   sendMQTT(msj,"voltaje");
   FloatToStr(corrientePZEM, msj);
   sendMQTT(msj,"corriente");
   FloatToStr(potenciaPZEM/10.0, msj);
   sendMQTT(msj,"potencia");
   T0CON.TMR0ON = 1;//Reincia el conteo
  }
  
  if(strstr(bufferRx,"change") || Button_RELE){
    RELE = ~RELE;
    if(RELE==0){
      sendMQTT("1", "motobomba"); //Motobomba encendida
      delay_ms(100);
      sendSMS("3008157431","Motobomba encendida");
      delay_ms(100);
     }else{
      sendMQTT("0", "motobomba"); //Motobomba apagada
      delay_ms(100);
      sendSMS("3008157431","Motobomba apagada");
      delay_ms(100);
     }
     delay_ms(100);
  
  }
  

   if( waterLevel < 10 || waterLevel > 45 ){ //90% de 50cms
    //Realizar tarea si recibe "change" por RX-UART o el tanque está lleno/vacio
    //RELE = ~RELE;
    if(waterLevel > 45  &&  RELE==1){ //tanque vacío y rele apagado
      RELE = 0;
      sendMQTT("1", "motobomba"); //Motobomba encendida
      delay_ms(100);
      sendSMS("3008157431","Motobomba encendida");
      delay_ms(100);
     }
     else if(waterLevel < 10  &&  RELE==0){ //tanque lleno y rele está encendido, entonces apagalo
      RELE = 1;
      sendMQTT("0", "motobomba"); //Motobomba apagada
      delay_ms(100);
      sendSMS("3008157431","Motobomba apagada");
      delay_ms(100);
     }
    cleanBufferRx();
   }
   else if(strstr(bufferRx,"CloseMQTT")){
    delay_ms(100);
    cleanBufferRx();
    closeMQTT();
    delay_ms(100);
    cleanBufferRx();
   }
   else{
    cleanBufferRx();
    delay_ms(100);
   }
   
 }

}

void cleanBufferRx(){
 unsigned char i =0;
 for(i=0; i<50; i++)
  bufferRx[i] = '\0';
  indexBufferRx=0;
}