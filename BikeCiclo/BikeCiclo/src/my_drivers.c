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
 * 
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
 *
void init_usart(){
	stdio_serial_init(&usart_instance, EDBG_CDC_MODULE, &usart_conf);
	usart_enable(&usart_instance);
}

/** \brief Calcula o comprimento da 
 *		circunferencia da roda
 *
 * \param roda tipo uint8_t Diâmetro da roda em polegadas
 * \param struct ciclometro guarda o valor calculado da circunferencia
 * \return none
 *
 */ 

void getCircunferencia(uint8_t roda, ciclometro *ciclo){
	float circunferencia = (roda*2.54)*PI/100;
	ciclo->tamRoda = circunferencia;
}

/** \brief Inicializa a a estrutura ciclometro
 *		que persiste as informações do deslocamento
 *
 * \param &ciclo tipo ciclometro
 * \return none
 *
 */
void init_ciclo(ciclometro *ciclo){
	ciclo->currentSpeed = 0;
	ciclo->endTime = 0;
	ciclo->maxSpeed = 0;
	ciclo->startTime = 0;
	ciclo->tamRoda = 0;
	ciclo->travelled = 0;
	ciclo->situacao = STOPPED;
}


/** \brief Calcula o comprimento da 
 *		circunferencia da roda
 *
 * \param start_time tipo uint32_t instante do pulso anterior em ms 
 * \param final_time tipo uint32_t instante do pulso atual em ms
 * \param *ciclo tipo ciclometro struct que persiste os dados
 * \return none
 *
 */
void getSpeed(uint32_t start_time, uint32_t final_time, ciclometro *ciclo){
		//tamanho da circunferência da roda multiplicado pelo número de pulsos
		//e pelo tempo decorrido em milissegundos
		uint32_t intervalo = final_time - start_time; /**<intervalo de tempo de uma volta completa da roda. */
		float speed = ((ciclo->tamRoda*intervalo/10)*36000)/2; /**< em km/h*/
		/**< Atualiza a velocidade máxima. */
		if(ciclo->maxSpeed < speed){
			ciclo->maxSpeed = speed;
		}
		/**< Atualiza a distância percorrida. */
		ciclo->travelled +=(ciclo->tamRoda/1000); /**< em km*/
		ciclo->currentSpeed = speed;
		
		/**< Atualiza a velocidade média. */
		float med=0;
		intervalo = final_time - ciclo->startTime; /**< Intervalo total da seção atual. */
		med = (ciclo->travelled/intervalo)*3600000; /**< 3600*1000 (segundo em uma hora)*1000 - de ms para s. */
		ciclo->medSpeed = med;	
}

/** \brief  envia os dados pela serial
 *		através de um printf para serem apresentados ao usuário
 *
 * \param ciclo tipo ciclometro 
 * \return none
 *
 */
void sendValues(ciclometro *ciclo){
	uint8_t decSpeed = 2, decTrav = 3;
	printf("%d,%d,%d,%d\r\n", (uint16_t) (ciclo->currentSpeed*100), (uint16_t) (ciclo->maxSpeed*100),
		(uint16_t) (ciclo->medSpeed*100),(uint16_t) (ciclo->travelled*1000));
}
