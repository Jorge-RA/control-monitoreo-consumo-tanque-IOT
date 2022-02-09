
_temperatura:

;lm35.c,3 :: 		float temperatura(){
;lm35.c,4 :: 		float voltaje = 0;
;lm35.c,5 :: 		float temperatura = 0;
	CLRF        temperatura_temperatura_L0+0 
	CLRF        temperatura_temperatura_L0+1 
	CLRF        temperatura_temperatura_L0+2 
	CLRF        temperatura_temperatura_L0+3 
;lm35.c,6 :: 		ADCON1 = 0b10000100; //AN0,AN1,AN3 analogo, los demas digitales, voltaje de referencia = VDD
	MOVLW       132
	MOVWF       ADCON1+0 
;lm35.c,7 :: 		ADCON0 = 0b10000001; //ADCON.ADON =1 ADC, channel 0, activado, ADCON0.GODONE = 0 <- en uno es para empezar la conversion
	MOVLW       129
	MOVWF       ADCON0+0 
;lm35.c,8 :: 		delay_ms(500);
	MOVLW       3
	MOVWF       R11, 0
	MOVLW       138
	MOVWF       R12, 0
	MOVLW       85
	MOVWF       R13, 0
L_temperatura0:
	DECFSZ      R13, 1, 1
	BRA         L_temperatura0
	DECFSZ      R12, 1, 1
	BRA         L_temperatura0
	DECFSZ      R11, 1, 1
	BRA         L_temperatura0
	NOP
	NOP
;lm35.c,9 :: 		ADCON0.F2 = 1; //GO/DONE = inicia la lectura y conversion
	BSF         ADCON0+0, 2 
;lm35.c,10 :: 		while(ADCON0.F2){
L_temperatura1:
	BTFSS       ADCON0+0, 2 
	GOTO        L_temperatura2
;lm35.c,11 :: 		voltaje = (((ADRESH&0x03) << 8) + ADRESL)* resolucion;
	MOVLW       3
	ANDWF       ADRESH+0, 0 
	MOVWF       R3 
	MOVF        R3, 0 
	MOVWF       R1 
	CLRF        R0 
	MOVF        ADRESL+0, 0 
	ADDWF       R0, 1 
	MOVLW       0
	ADDWFC      R1, 1 
	CALL        _word2double+0, 0
	MOVLW       82
	MOVWF       R4 
	MOVLW       73
	MOVWF       R5 
	MOVLW       29
	MOVWF       R6 
	MOVLW       119
	MOVWF       R7 
	CALL        _Mul_32x32_FP+0, 0
;lm35.c,12 :: 		temperatura = voltaje * 100;
	MOVLW       0
	MOVWF       R4 
	MOVLW       0
	MOVWF       R5 
	MOVLW       72
	MOVWF       R6 
	MOVLW       133
	MOVWF       R7 
	CALL        _Mul_32x32_FP+0, 0
	MOVF        R0, 0 
	MOVWF       temperatura_temperatura_L0+0 
	MOVF        R1, 0 
	MOVWF       temperatura_temperatura_L0+1 
	MOVF        R2, 0 
	MOVWF       temperatura_temperatura_L0+2 
	MOVF        R3, 0 
	MOVWF       temperatura_temperatura_L0+3 
;lm35.c,13 :: 		}
	GOTO        L_temperatura1
L_temperatura2:
;lm35.c,14 :: 		return temperatura;
	MOVF        temperatura_temperatura_L0+0, 0 
	MOVWF       R0 
	MOVF        temperatura_temperatura_L0+1, 0 
	MOVWF       R1 
	MOVF        temperatura_temperatura_L0+2, 0 
	MOVWF       R2 
	MOVF        temperatura_temperatura_L0+3, 0 
	MOVWF       R3 
;lm35.c,16 :: 		}
L_end_temperatura:
	RETURN      0
; end of _temperatura
