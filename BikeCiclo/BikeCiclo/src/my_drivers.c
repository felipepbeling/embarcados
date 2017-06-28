/*
 * my_drivers.c
 *
 * Created: 23/06/2017 15:57:18
 *  Author: Carlos
 */ 
#include <my_drivers.h>




/** \brief Realiza a configura��o da comunica��o serial
 *		entre a placa e o PC, por exemplo.
 *		Pad�o: 9600bps,  8 bits
 *		Ap�s chamar init_usart().
 *
 * \param none
 * \return none
 *
 */ 
void config_usarts(void){
	usart_get_config_defaults(&usart_conf);
	usart_conf.baudrate    = 9600;
	usart_conf.mux_setting = EDBG_CDC_SERCOM_MUX_SETTING;
	usart_conf.pinmux_pad0 = EDBG_CDC_SERCOM_PINMUX_PAD0;
	usart_conf.pinmux_pad1 = EDBG_CDC_SERCOM_PINMUX_PAD1;
	usart_conf.pinmux_pad2 = EDBG_CDC_SERCOM_PINMUX_PAD2;
	usart_conf.pinmux_pad3 = EDBG_CDC_SERCOM_PINMUX_PAD3;
}

/** \brief Inicializa a comunica��o serial
 *		e aguarda uma conex�o.
 *		Necess�rio chamarconfig_usarts() antes.
 *
 * \param none
 * \return none
 */
void init_usart(void){
	stdio_serial_init(&usart_instance, EDBG_CDC_MODULE, &usart_conf);
	usart_enable(&usart_instance);
}


/** \brief Calcula o comprimento da 
 *		circunferencia da roda
 *
 * \param roda tipo uint8_t Di�metro da roda em polegadas
 * \param struct ciclometro guarda o valor calculado da circunferencia
 * \return none
 *
 */ 
void getCircunferencia(uint8_t roda, ciclometro *ciclo){
	float circunferencia = (roda*2.54)*PI/100;
	ciclo->tamRoda = circunferencia;
}

/** \brief Inicializa a a estrutura ciclometro
 *		que persiste as informa��es do deslocamento
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
	ciclo->contador = 0;
	ciclo->lastTime = 0;
	ciclo->situacao = READY;
}


/** \brief Calcula as velocidades  e dist�ncia percorrida
 *
 * \param start_time tipo uint32_t instante do pulso anterior em ms 
 * \param final_time tipo uint32_t instante do pulso atual em ms
 * \param *ciclo tipo ciclometro struct que persiste os dados
 * \return none
 *
 */
void getSpeed(uint32_t final_time, ciclometro *ciclo, uint16_t contador){
		//tamanho da circunfer�ncia da roda multiplicado pelo n�mero de pulsos
		//e pelo tempo decorrido em milissegundos
		uint32_t intervalo = final_time - ciclo->lastTime; /**<intervalo de tempo de uma volta completa da roda. */
		float speed = (((ciclo->tamRoda*(contador-ciclo->contador))*intervalo/10)*36000)/2; /**< em km/h*/
		ciclo->contador = contador;
		ciclo->lastTime = final_time;
		/**< Atualiza a velocidade m�xima. */
		if(ciclo->maxSpeed < speed){
			ciclo->maxSpeed = speed;
		}
		/**< Atualiza a dist�ncia percorrida. */
		ciclo->travelled +=(ciclo->tamRoda/1000); /**< em km*/
		ciclo->currentSpeed = speed;
		
		/**< Atualiza a velocidade m�dia. */
		float med=0;
		intervalo = final_time - ciclo->startTime; /**< Intervalo total da se��o atual. */
		med = (ciclo->travelled/intervalo)*3600000; /**< 3600*1000 (segundo em uma hora)*1000 - de ms para s. */
		ciclo->medSpeed = med;	
}

/** \brief  envia os dados pela serial
 *		atrav�s de um printf para serem apresentados ao usu�rio
 *
 * \param ciclo tipo ciclometro 
 * \return none
 *
 */
void sendValues(ciclometro *ciclo){
	//uint8_t decSpeed = 2, decTrav = 3;
	//uint8_t buffer[] =  ""+(uint16_t) (ciclo->currentSpeed*100)+","+(uint16_t) (ciclo->maxSpeed*100)+","+
	//(uint16_t) (ciclo->medSpeed*100)+","+(uint16_t) (ciclo->travelled*1000)+","+ciclo->situacao;//
	//uint8_t buffer[] = ("%d,%d,%d,%d,%d\r\n", (uint16_t) (ciclo->currentSpeed*100), (uint16_t) (ciclo->maxSpeed*100),
	//(uint16_t) (ciclo->medSpeed*100),(uint16_t) (ciclo->travelled*1000), ciclo->situacao);
	
	//usart_write_buffer_wait(&usart_instance, buffer, sizeof(buffer));
	
	printf("%d,%d,%d,%d,%d\r\n", (uint16_t) (ciclo->currentSpeed*100), (uint16_t) (ciclo->maxSpeed*100),
		(uint16_t) (ciclo->medSpeed*100),(uint16_t) (ciclo->travelled*1000), ciclo->situacao);
}

#define SW4_EIC_LINE              5
void configure_extint_channel(void){
	//! [setup_1]
	struct extint_chan_conf config_extint_chan;
	
	struct extint_chan_conf config_extint_chan2;

	extint_chan_get_config_defaults(&config_extint_chan);
	extint_chan_get_config_defaults(&config_extint_chan2);

	
	/**< para o bot�o SW0. */
	config_extint_chan.gpio_pin           = BUTTON_0_EIC_PIN;	 //BUTTON_0_EIC_PIN;
	config_extint_chan.gpio_pin_mux       = BUTTON_0_EIC_MUX;   //BUTTON_0_EIC_MUX;
	config_extint_chan.gpio_pin_pull      = EXTINT_PULL_UP;
	config_extint_chan.detection_criteria = EXTINT_DETECT_RISING;

	extint_chan_set_config(BUTTON_0_EIC_LINE, &config_extint_chan);
	
	/**< Para o sensor. */
	/**< Configura a GPIO pin*/
	struct port_config pin_conf2;
	port_get_config_defaults(&pin_conf2);
	pin_conf.direction  = PORT_PIN_DIR_INPUT;
	pin_conf.input_pull = PORT_PIN_PULL_DOWN;
	port_pin_set_config(EIC_IN_PIN, &pin_conf2);
	
	config_extint_chan2.gpio_pin           = EIC_IN_PIN;	 //BUTTON_0_EIC_PIN;
	config_extint_chan2.gpio_pin_mux       = EIC_IN_PIN_MUX;   //BUTTON_0_EIC_MUX;
	config_extint_chan2.gpio_pin_pull      = EXTINT_PULL_UP;
	config_extint_chan.detection_criteria = EXTINT_DETECT_RISING;

	extint_chan_set_config(EIC_IN_CHANNEL, &config_extint_chan2);

}

/** \brief  evento disparado quando ocorre uma borda de subida 
 *		no pino de entrada, neste caso a vari�vel pulso � incrementada
 *
 * \param none
 * \return none
 *
 */
void extint_detection_callback2(void){
	/**< callback para o sensor. */
	bool pin_state = port_pin_get_input_level(EIC_IN_PIN);
	if(pin_state == true){
		if(ciclom.situacao == RUNNING){
			if(pulsos==0)
				relogio=0;
			pulsos++;
			printf("Pino alto\r\n");
			//sendValues(&ciclom);
		}
	}
}
	
/** \brief  evento disparado quando o bot�o SW0 � precionado
 *		ao ser pressionado o bot�o altera o estado do sistema para:
 *		RUNNING se READY
 *		STOPPED se RUNNING
 *		READY se STOPPED
 *	
 * \param none
 * \return none
 *
 */
void extint_detection_callback(void){
	/**< callback para o bot�o. */
	bool pin_state2 = port_pin_get_input_level(BUTTON_0_PIN);
	if(pin_state2 == true){
		switch(ciclom.situacao){
			case READY:
				pulsos=0;
				ciclom.situacao = RUNNING;
				sendValues(&ciclom);
			break;
			case RUNNING:
				ciclom.situacao = STOPPED;
				sendValues(&ciclom);
			break;
			case STOPPED:
				init_ciclo(&ciclom);
				pulsos = 0;
				sendValues(&ciclom);
			break;
		}
		port_pin_toggle_output_level(LED_0_PIN);
	}
}

void configure_extint_callbacks(void){
	/**< Para o bot�o. */
	extint_register_callback(extint_detection_callback, BUTTON_0_EIC_LINE, EXTINT_CALLBACK_TYPE_DETECT);
	extint_chan_enable_callback(BUTTON_0_EIC_LINE,EXTINT_CALLBACK_TYPE_DETECT);
	
	/**< Para o Sensor. */
	extint_register_callback(extint_detection_callback2, EIC_IN_CHANNEL, EXTINT_CALLBACK_TYPE_DETECT);
	extint_chan_enable_callback(EIC_IN_CHANNEL,EXTINT_CALLBACK_TYPE_DETECT);

}

/**< Defini��es para o timer.*/
/** \brief  evento disparado quando o timer contar 1ms
 *		aa cada disparo � feito o incremento equivalente � 
 *		1ms
 *	
 * \param tcc_module 
 * \return none
 *
 */
void tcc_callback_to_relogio1ms( struct tcc_module *const module_inst){
	relogio++;
}


/** \brief  Evento disparado quando o timer contar 500ms
 *		a cada disparo � feita a atualiza��o das medi��es.
 *	cal - sendValues(Ciclometro *ciclo);
 *	
 * \param tcc_module 
 * \return none
 *
 */
void tcc_callback_to_sendValues( struct tcc_module *const module_inst){
	sendValues(&ciclom);
}

/** \brief  Configura��o do m�dulo TCC
 *		Utilizado 32kHz, prescaler 16
  *		Cada ciclo representa 0,5ms
 * \param none 
 * \return none
 *
 */
void configure_tcc(void){

	struct tcc_config config_tcc;

	tcc_get_config_defaults(&config_tcc, TCC0);

	config_tcc.counter.clock_source = GCLK_GENERATOR_1;
	config_tcc.counter.clock_prescaler = TCC_CLOCK_PRESCALER_DIV64;
	config_tcc.counter.period =   2000;
	config_tcc.compare.match[0] =  200; /**< para disparar o relogio. */
	config_tcc.compare.match[1] =  1000; /**< para disparar o envio das medi��es. */

	tcc_init(&tcc_instance, TCC0, &config_tcc);
	
	tcc_enable(&tcc_instance);/**< Ativa o m�dulo TCC. */
}

/** \brief  Configura��o das chamadas quando o intervalo de tempo 
 *		especificado for concluido
 *
 * \param none 
 * \return none
 *
 */
void configure_tcc_callbacks(void){

	tcc_register_callback(&tcc_instance, tcc_callback_to_relogio1ms, TCC_CALLBACK_CHANNEL_0);
	tcc_register_callback(&tcc_instance, tcc_callback_to_sendValues, TCC_CALLBACK_CHANNEL_1);
	
	tcc_enable_callback(&tcc_instance, TCC_CALLBACK_CHANNEL_0); /**< Ativa a chamada do canal 0. */
	tcc_enable_callback(&tcc_instance, TCC_CALLBACK_CHANNEL_1); /**< Ativa a chamada do canal 1. */
}

/** \brief  Leitura dos dados salvos em mem�ria
 *	maxSpeed, medSpeed, tamRoda, travelled,
 *	startTime, lastTime, situacao, contadore situacao.
 * \param ciclometro *ciclo
 * \return none
 *
 */
void lerMemDados(ciclometro *ciclo){
	do{
		error_code = nvm_read_buffer(
		100 * NVMCTRL_ROW_PAGES * NVMCTRL_PAGE_SIZE,
		page_buffer, NVMCTRL_PAGE_SIZE);
	} while (error_code == STATUS_BUSY);
	
	for (uint32_t i = 0; i < NVMCTRL_PAGE_SIZE; i++) {
		if(page_buffer[i] == 255)
			page_buffer[i] = 0;
	}
	uint8_t i = 0;
	ciclo->maxSpeed = (float)page_buffer[i++]/100;
	ciclo->medSpeed = (float)page_buffer[i++]/100;
	ciclo->tamRoda = (float)page_buffer[i++]/10000;
	ciclo->travelled = (float)page_buffer[i++]/1000;
	ciclo->startTime = page_buffer[i++];
	ciclo->lastTime = page_buffer[i++];
	ciclo->situacao = page_buffer[i++];
	ciclo->contador = 0;
	ciclo->currentSpeed = 0;
	if(ciclo->tamRoda <1){
		/**< Calcula e atualiza o tamanho da roda. */
		getCircunferencia(DIAMETRO_RODA, &ciclo);
	}
}

/** \brief  Grava os dados da se��o em mem�ria
 *	maxSpeed, medSpeed, tamRoda, travelled,
 *	startTime, lastTime, situacao, contadore situacao.
 * \param ciclometro *ciclo
 * \return none
 *
 */
void gravaMemDados(ciclometro *ciclo){
	/**< Limpa a mem�ria antes de gravar. */
	do{
		error_code = nvm_erase_row(
		100 * NVMCTRL_ROW_PAGES * NVMCTRL_PAGE_SIZE);
	} while (error_code == STATUS_BUSY);

	/**< Prepara o vetor buffer para a grava��o. */
	for (uint32_t i = 0; i < NVMCTRL_PAGE_SIZE; i++) {
		page_buffer[i] = 0;
	}
	uint8_t i = 0;
	page_buffer[i++] = (uint16_t)ciclo->maxSpeed*100;
	page_buffer[i++] = (uint16_t)ciclo->medSpeed*100;
	page_buffer[i++] = (uint16_t)ciclo->tamRoda*10000;
	page_buffer[i++] = (uint16_t)ciclo->travelled*1000;
	page_buffer[i++] = ciclo->startTime;
	page_buffer[i++] = ciclo->lastTime;
	page_buffer[i++] = ciclo->situacao;
	
	/**<Grava os dados. */
	do{
		error_code = nvm_write_buffer(
		100 * NVMCTRL_ROW_PAGES * NVMCTRL_PAGE_SIZE,
		page_buffer, NVMCTRL_PAGE_SIZE);
	} while (error_code == STATUS_BUSY);
}

/** \brief  Configura��o do m�dulo NVM para leitura e grava��o na EEPROM
 *
 * \return none
 *
 */
void configure_nvm(void){
	struct nvm_config config_nvm;

	nvm_get_config_defaults(&config_nvm);
	config_nvm.manual_page_write = false;
	nvm_set_config(&config_nvm);
}

