#line 1 "D:/Jorge/Codigos en Mikro C/Proyecto_Diplomado_Iot/ec200.c"
#line 1 "d:/jorge/codigos en mikro c/proyecto_diplomado_iot/ec200.h"

 void Init_EC200();
 void sendSMS(unsigned char numero[15], unsigned char msj[30]);
#line 1 "d:/jorge/codigos en mikro c/proyecto_diplomado_iot/main.h"
extern unsigned char bufferRx[80]={0};
extern unsigned char indexBufferRx = 0;
extern unsigned char errorPZEMFlag = 0;
void cleanBufferRx();
#line 4 "D:/Jorge/Codigos en Mikro C/Proyecto_Diplomado_Iot/ec200.c"
void cleanBufferRx();
void Init_EC200(){
 unsigned char tamListInitComands = 0, i=0;
 unsigned char *listInitComands[] = {
 "ATE0\r\n",
 "AT+CGDCONT=1,\"IP\",\"internet.comcel.com.co\"\r\n",
 "AT+CPIN?\r\n",
 "AT+CMEE=2\r\n",
 "AT+CSQ\r\n",
 "AT+CREG?\r\n",
 "AT+CMGF=1\r\n",
 "AT+CNMI=2,2,0,0,0\r\n",
 "AT+QCFG=\"urc/cache\",1\r\n",
 };

 tamListInitComands = sizeof(listInitComands)/sizeof(listInitComands[0]);
 for(i=0; i<tamListInitComands; i++){
 UART1_Write_Text(listInitComands[i]);
 delay_ms(200);
 cleanBufferRx();
 }
}

void sendSMS(unsigned char numero[15], unsigned char msj[30]){

 UART1_Write_Text("AT+CMGS=");
 UART1_Write_Text(numero);
 UART1_Write_Text("\r\n");
 delay_ms(500);
 cleanBufferRx();
 UART1_Write_Text(msj);
 UART1_Write_Text("\r\n");
 UART1_Write((char)26);
 delay_ms(100);
 cleanBufferRx();




}
