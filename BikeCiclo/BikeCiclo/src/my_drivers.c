/*
 * my_drivers.c
 *
 * Created: 23/06/2017 15:57:18
 *  Author: Carlos
 */ 
#include <my_drivers.h>




/** \brief Realiza a configuração da comunicação serial
 *		entre a placa e o PC, por exemplo.
 *		Padão: 9600bps,  8 bits
 *		Após chamar init_usart().
 *
 * \param none
 * \return none
 *
 */  
void config_usarts(){
	usart_get_config_defaults(&usart_conf);
	usart_conf.baudrate    = 9600;
	usart_conf.mux_setting = EDBG_CDC_SERCOM_MUX_SETTING;
	usart_conf.pinmux_pad0 = EDBG_CDC_SERCOM_PINMUX_PAD0;
	usart_conf.pinmux_pad1 = EDBG_CDC_SERCOM_PINMUX_PAD1;
	usart_conf.pinmux_pad2 = EDBG_CDC_SERCOM_PINMUX_PAD2;
	usart_conf.pinmux_pad3 = EDBG_CDC_SERCOM_PINMUX_PAD3;
}

/** \brief Inicializa a comunicação serial
 *		e aguarda uma conexão.
 *		Necessário chamarconfig_usarts() antes.
 *
 * \param none
 * \return none
 *
 */ 
void init_usart(){
	stdio_serial_init(&usart_instance, EDBG_CDC_MODULE, &usart_conf);
	usart_enable(&usart_instance);
}

/** \brief Calcula o comprimento da 
 *		circunferencia da roda
 *
 * \param roda tipo uint8_t Diâmetro da roda em polegadas 
 * \return float comprimento da circunferência em metros
 *
 */ 
float getCircunferencia(uint8_t roda){
	float circunferencia = (roda*2.54)*PI/100;
	return circunferencia;
}


/** \brief Calcula o comprimento da 
 *		circunferencia da roda
 *
 * \param roda tipo uint8_t Diâmetro da roda em polegadas 
 * \return float comprimento da circunferência em metros
 *
 */
uint16_t getSpeed(uint32_t start_time, uint32_t final_time, float circunferencia){

			//tamanho da circunferência da roda multiplicado pelo número de pulsos
			//e pelo tempo decorrido em milissegundos
			aux = relogio - lastT;
			pAux = pulsos;
			pulsos = 0;
			lastT = relogio;
			speed = ((circunferencia*pAux*aux/10)*36000)/2;
			lastT = relogio;
			distancia += circunferencia*pAux/10000;//em km
			/*Envia a velocidade por socket*/
			//TO DO......
			//speed=(XX,X), distancia=XX,XXX km
		}
		wait_period (&info);
	}
	
}