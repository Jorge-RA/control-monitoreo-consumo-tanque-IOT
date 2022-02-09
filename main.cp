#line 1 "D:/Jorge/Codigos en Mikro C/Proyecto_Diplomado_Iot/main.c"


void cleanBufferRx();
#line 1 "d:/jorge/codigos en mikro c/proyecto_diplomado_iot/ec200.h"

 void Init_EC200();
 void sendSMS(unsigned char numero[15], unsigned char msj[30]);
#line 1 "d:/jorge/codigos en mikro c/proyecto_diplomado_iot/mqtt.h"
void sendMQTT(unsigned char mensaje[15], unsigned char topic[10]);
void Init_MQT(unsigned char ip[20], unsigned char clientID[20]);
void subMQTT(unsigned char topic[10]);
void closeMQTT();
#line 1 "d:/jorge/codigos en mikro c/proyecto_diplomado_iot/rf_module.h"
void Init_RF();
unsigned char pending_data_RF();
extern unsigned char bufferRF[10];
#line 1 "d:/jorge/codigos en mikro c/proyecto_diplomado_iot/pzem004t.h"
void PZEM_Read_All();
extern float voltajePZEM;
extern float corrientePZEM;
extern float potenciaPZEM;
#line 9 "D:/Jorge/Codigos en Mikro C/Proyecto_Diplomado_Iot/main.c"
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
 int waterLevel = 30;

 void Interrupt(){
 INTCON.GIE = 0;

 if(PIR1.RCIF){
 do{
 bufferRx[indexBufferRx] = UART1_Read();
 indexBufferRx++;
 }while( UART1_Data_Ready() );


 PIR1.RCIF = 0;
 }
 if(INTCON.TMR0IF){
 INTCON.TMR0IF = 0;
 contCurrentFlag++;
 if(contCurrentFlag == 625){
 currentFlag = 1;
 contCurrentFlag = 0;
 }
 TMR0L = 6;
 }

 if(PIR1.TMR1IF){
 PIR1.TMR1IF = 0;
 contTimer1++;
 if(contTimer1 >= 4){
 T1CON.TMR1ON = 0;
 contTimer1=0;
 Soft_UART_Break();
 errorPZEMFlag = 1;
 }
 TMR1H = 0b00001011;
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
 Init_MQT("40.71.125.21", "GrupoIoTClient");
 subMQTT("motobomba");
 delay_ms(500);


 INTCON = 0b11100000;
 PIE1 = 0b00100001;
 PIR1 = 0b00000000;

 T0CON = 0b01000111;
 T1CON = 0b00110000;
 TMR0L = 6;
 TMR1H = 0b00001011;
 TMR1L = 0b11011100;



 TRISC.F3 = 0;
 PORTC.F3 = 1;
 TRISC.F2 = 0;
 PORTC.F2 = 1;

 TRISC.F4=1;


 cleanBufferRx();
 delay_ms(500);

 T0CON.TMR0ON = 1;
 while(1){

 if(pending_data_RF()){
 bufferRF[4]='\0';
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
 T0CON.TMR0ON = 1;
 }

 if(strstr(bufferRx,"change") ||  PORTC.F4 ){
  PORTC.F2  = ~ PORTC.F2 ;
 if( PORTC.F2 ==0){
 sendMQTT("1", "motobomba");
 delay_ms(100);
 sendSMS("3008157431","Motobomba encendida");
 delay_ms(100);
 }else{
 sendMQTT("0", "motobomba");
 delay_ms(100);
 sendSMS("3008157431","Motobomba apagada");
 delay_ms(100);
 }
 delay_ms(100);

 }


 if( waterLevel < 10 || waterLevel > 45 ){


 if(waterLevel > 45 &&  PORTC.F2 ==1){
  PORTC.F2  = 0;
 sendMQTT("1", "motobomba");
 delay_ms(100);
 sendSMS("3008157431","Motobomba encendida");
 delay_ms(100);
 }
 else if(waterLevel < 10 &&  PORTC.F2 ==0){
  PORTC.F2  = 1;
 sendMQTT("0", "motobomba");
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
