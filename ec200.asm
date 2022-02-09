
_Init_EC200:

;ec200.c,5 :: 		void Init_EC200(){
;ec200.c,6 :: 		unsigned char tamListInitComands = 0, i=0;
	CLRF        Init_EC200_tamListInitComands_L0+0 
	CLRF        Init_EC200_i_L0+0 
;ec200.c,17 :: 		};
	MOVLW       ?lstr1_ec200+0
	MOVWF       Init_EC200_listInitComands_L0+0 
	MOVLW       hi_addr(?lstr1_ec200+0)
	MOVWF       Init_EC200_listInitComands_L0+1 
	MOVLW       ?lstr2_ec200+0
	MOVWF       Init_EC200_listInitComands_L0+2 
	MOVLW       hi_addr(?lstr2_ec200+0)
	MOVWF       Init_EC200_listInitComands_L0+3 
	MOVLW       ?lstr3_ec200+0
	MOVWF       Init_EC200_listInitComands_L0+4 
	MOVLW       hi_addr(?lstr3_ec200+0)
	MOVWF       Init_EC200_listInitComands_L0+5 
	MOVLW       ?lstr4_ec200+0
	MOVWF       Init_EC200_listInitComands_L0+6 
	MOVLW       hi_addr(?lstr4_ec200+0)
	MOVWF       Init_EC200_listInitComands_L0+7 
	MOVLW       ?lstr5_ec200+0
	MOVWF       Init_EC200_listInitComands_L0+8 
	MOVLW       hi_addr(?lstr5_ec200+0)
	MOVWF       Init_EC200_listInitComands_L0+9 
	MOVLW       ?lstr6_ec200+0
	MOVWF       Init_EC200_listInitComands_L0+10 
	MOVLW       hi_addr(?lstr6_ec200+0)
	MOVWF       Init_EC200_listInitComands_L0+11 
	MOVLW       ?lstr7_ec200+0
	MOVWF       Init_EC200_listInitComands_L0+12 
	MOVLW       hi_addr(?lstr7_ec200+0)
	MOVWF       Init_EC200_listInitComands_L0+13 
	MOVLW       ?lstr8_ec200+0
	MOVWF       Init_EC200_listInitComands_L0+14 
	MOVLW       hi_addr(?lstr8_ec200+0)
	MOVWF       Init_EC200_listInitComands_L0+15 
	MOVLW       ?lstr9_ec200+0
	MOVWF       Init_EC200_listInitComands_L0+16 
	MOVLW       hi_addr(?lstr9_ec200+0)
	MOVWF       Init_EC200_listInitComands_L0+17 
;ec200.c,19 :: 		tamListInitComands = sizeof(listInitComands)/sizeof(listInitComands[0]);
	MOVLW       9
	MOVWF       Init_EC200_tamListInitComands_L0+0 
;ec200.c,20 :: 		for(i=0; i<tamListInitComands; i++){
	CLRF        Init_EC200_i_L0+0 
L_Init_EC2000:
	MOVF        Init_EC200_tamListInitComands_L0+0, 0 
	SUBWF       Init_EC200_i_L0+0, 0 
	BTFSC       STATUS+0, 0 
	GOTO        L_Init_EC2001
;ec200.c,21 :: 		UART1_Write_Text(listInitComands[i]);
	MOVF        Init_EC200_i_L0+0, 0 
	MOVWF       R0 
	MOVLW       0
	MOVWF       R1 
	RLCF        R0, 1 
	BCF         R0, 0 
	RLCF        R1, 1 
	MOVLW       Init_EC200_listInitComands_L0+0
	ADDWF       R0, 0 
	MOVWF       FSR0L+0 
	MOVLW       hi_addr(Init_EC200_listInitComands_L0+0)
	ADDWFC      R1, 0 
	MOVWF       FSR0L+1 
	MOVF        POSTINC0+0, 0 
	MOVWF       FARG_UART1_Write_Text_uart_text+0 
	MOVF        POSTINC0+0, 0 
	MOVWF       FARG_UART1_Write_Text_uart_text+1 
	CALL        _UART1_Write_Text+0, 0
;ec200.c,22 :: 		delay_ms(200);
	MOVLW       2
	MOVWF       R11, 0
	MOVLW       4
	MOVWF       R12, 0
	MOVLW       186
	MOVWF       R13, 0
L_Init_EC2003:
	DECFSZ      R13, 1, 1
	BRA         L_Init_EC2003
	DECFSZ      R12, 1, 1
	BRA         L_Init_EC2003
	DECFSZ      R11, 1, 1
	BRA         L_Init_EC2003
	NOP
;ec200.c,23 :: 		cleanBufferRx();
	CALL        _cleanBufferRx+0, 0
;ec200.c,20 :: 		for(i=0; i<tamListInitComands; i++){
	INCF        Init_EC200_i_L0+0, 1 
;ec200.c,24 :: 		}
	GOTO        L_Init_EC2000
L_Init_EC2001:
;ec200.c,25 :: 		}
L_end_Init_EC200:
	RETURN      0
; end of _Init_EC200

_sendSMS:

;ec200.c,27 :: 		void sendSMS(unsigned char numero[15], unsigned char msj[30]){
;ec200.c,29 :: 		UART1_Write_Text("AT+CMGS=");
	MOVLW       ?lstr10_ec200+0
	MOVWF       FARG_UART1_Write_Text_uart_text+0 
	MOVLW       hi_addr(?lstr10_ec200+0)
	MOVWF       FARG_UART1_Write_Text_uart_text+1 
	CALL        _UART1_Write_Text+0, 0
;ec200.c,30 :: 		UART1_Write_Text(numero);
	MOVF        FARG_sendSMS_numero+0, 0 
	MOVWF       FARG_UART1_Write_Text_uart_text+0 
	MOVF        FARG_sendSMS_numero+1, 0 
	MOVWF       FARG_UART1_Write_Text_uart_text+1 
	CALL        _UART1_Write_Text+0, 0
;ec200.c,31 :: 		UART1_Write_Text("\r\n");
	MOVLW       ?lstr11_ec200+0
	MOVWF       FARG_UART1_Write_Text_uart_text+0 
	MOVLW       hi_addr(?lstr11_ec200+0)
	MOVWF       FARG_UART1_Write_Text_uart_text+1 
	CALL        _UART1_Write_Text+0, 0
;ec200.c,32 :: 		delay_ms(500);
	MOVLW       3
	MOVWF       R11, 0
	MOVLW       138
	MOVWF       R12, 0
	MOVLW       85
	MOVWF       R13, 0
L_sendSMS4:
	DECFSZ      R13, 1, 1
	BRA         L_sendSMS4
	DECFSZ      R12, 1, 1
	BRA         L_sendSMS4
	DECFSZ      R11, 1, 1
	BRA         L_sendSMS4
	NOP
	NOP
;ec200.c,33 :: 		cleanBufferRx();
	CALL        _cleanBufferRx+0, 0
;ec200.c,34 :: 		UART1_Write_Text(msj);
	MOVF        FARG_sendSMS_msj+0, 0 
	MOVWF       FARG_UART1_Write_Text_uart_text+0 
	MOVF        FARG_sendSMS_msj+1, 0 
	MOVWF       FARG_UART1_Write_Text_uart_text+1 
	CALL        _UART1_Write_Text+0, 0
;ec200.c,35 :: 		UART1_Write_Text("\r\n");
	MOVLW       ?lstr12_ec200+0
	MOVWF       FARG_UART1_Write_Text_uart_text+0 
	MOVLW       hi_addr(?lstr12_ec200+0)
	MOVWF       FARG_UART1_Write_Text_uart_text+1 
	CALL        _UART1_Write_Text+0, 0
;ec200.c,36 :: 		UART1_Write((char)26);  //Ctrl + Z   para enviar el mensaje
	MOVLW       26
	MOVWF       FARG_UART1_Write_data_+0 
	CALL        _UART1_Write+0, 0
;ec200.c,37 :: 		delay_ms(100);
	MOVLW       130
	MOVWF       R12, 0
	MOVLW       221
	MOVWF       R13, 0
L_sendSMS5:
	DECFSZ      R13, 1, 1
	BRA         L_sendSMS5
	DECFSZ      R12, 1, 1
	BRA         L_sendSMS5
	NOP
	NOP
;ec200.c,38 :: 		cleanBufferRx();
	CALL        _cleanBufferRx+0, 0
;ec200.c,43 :: 		}
L_end_sendSMS:
	RETURN      0
; end of _sendSMS
