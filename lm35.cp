#line 1 "D:/Jorge/Codigos en Mikro C/Proyecto_Diplomado_Iot/lm35.c"
#line 1 "d:/jorge/codigos en mikro c/proyecto_diplomado_iot/lm35.h"
float temperatura();
#line 3 "D:/Jorge/Codigos en Mikro C/Proyecto_Diplomado_Iot/lm35.c"
float temperatura(){
 float voltaje = 0;
 float temperatura = 0;
 ADCON1 = 0b10000100;
 ADCON0 = 0b10000001;
 delay_ms(500);
 ADCON0.F2 = 1;
 while(ADCON0.F2){
 voltaje = (((ADRESH&0x03) << 8) + ADRESL)*  0.0048 ;
 temperatura = voltaje * 100;
 }
 return temperatura;

}
