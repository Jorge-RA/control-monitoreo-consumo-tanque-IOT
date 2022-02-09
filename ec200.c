#include "ec200.h"
#include "main.h"

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
  UART1_Write((char)26);  //Ctrl + Z   para enviar el mensaje
   delay_ms(100);
   cleanBufferRx();

   


}