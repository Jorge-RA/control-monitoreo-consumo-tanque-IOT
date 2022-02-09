#include "lm35.h"
#define resolucion 0.0048
float temperatura(){
  float voltaje = 0;
  float temperatura = 0;
  ADCON1 = 0b10000100; //AN0,AN1,AN3 analogo, los demas digitales, voltaje de referencia = VDD
  ADCON0 = 0b10000001; //ADCON.ADON =1 ADC, channel 0, activado, ADCON0.GODONE = 0 <- en uno es para empezar la conversion
  delay_ms(500);
  ADCON0.F2 = 1; //GO/DONE = inicia la lectura y conversion
  while(ADCON0.F2){
   voltaje = (((ADRESH&0x03) << 8) + ADRESL)* resolucion;
   temperatura = voltaje * 100;
  }
  return temperatura;

}