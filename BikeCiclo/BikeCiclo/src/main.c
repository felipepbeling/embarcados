/**
 * \file
 *
 * \brief Empty user application template
 *
 */

/**
 * \mainpage User Application template doxygen documentation
 *
 * \par Empty user application template
 *
 * This is a bare minimum user application template.
 *
 * For documentation of the board, go \ref group_common_boards "here" for a link
 * to the board-specific documentation.
 *
 * \par Content
 *
 * -# Include the ASF header files (through asf.h)
 * -# Minimal main function that starts with a call to system_init()
 * -# Basic usage of on-board LED and button
 * -# "Insert application code here" comment
 *
 */

/*
 * Include header files for all drivers that have been imported from
 * Atmel Software Framework (ASF).
 */
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */
#include <asf.h>
#include <stdio.h>
#include <my_drivers.h>
#include <my_drivers.h>

struct usart_module usart_instance;
struct usart_config usart_conf;


//!********************INICIO DA CONFIUGURACAO DA INTERRUPCAO ********************

void configure_extint_channel(void);
void configure_extint_callbacks(void);
void extint_detection_callback(void);

//! [setup]
void configure_extint_channel(void)
{
	//! [setup_1]
	struct extint_chan_conf config_extint_chan;
	//! [setup_1]
	//! [setup_2]
	extint_chan_get_config_defaults(&config_extint_chan);
	//! [setup_2]

	//! [setup_3]
	config_extint_chan.gpio_pin           = BUTTON_0_EIC_PIN;
	config_extint_chan.gpio_pin_mux       = BUTTON_0_EIC_MUX;
	config_extint_chan.gpio_pin_pull      = EXTINT_PULL_UP;
	config_extint_chan.detection_criteria = EXTINT_DETECT_BOTH;
	//! [setup_3]
	//! [setup_4]
	extint_chan_set_config(BUTTON_0_EIC_LINE, &config_extint_chan);
	//! [setup_4]
}

void configure_extint_callbacks(void)
{
	//! [setup_5]
	extint_register_callback(extint_detection_callback,
	BUTTON_0_EIC_LINE,
	EXTINT_CALLBACK_TYPE_DETECT);
	//! [setup_5]
	//! [setup_6]
	extint_chan_enable_callback(BUTTON_0_EIC_LINE,
	EXTINT_CALLBACK_TYPE_DETECT);
	//! [setup_6]
}

//! [setup_7]
void extint_detection_callback(void)
{
	bool pin_state = port_pin_get_input_level(BUTTON_0_PIN);
	port_pin_set_output_level(LED_0_PIN, pin_state);
}
//! [setup_7]

//!********************FIM DA CONFIUGURACAO DA INTERRUPCAO ********************

int main (void)
{
	system_init();

	configure_extint_channel();         
	configure_extint_callbacks();           //configuracao da interrupcao
	system_interrupt_enable_global();

	/************************************************************************/
	/* Prepara a comunicação serial                                         */
	/************************************************************************/
	/** \brief Realiza a configuração da comunicação serial
	 *		entre a placa e o PC, por exemplo.
	 *		Padão: 9600bps,  8 bits
	 */
	usart_get_config_defaults(&usart_conf);
	usart_conf.baudrate    = 9600;
	usart_conf.mux_setting = EDBG_CDC_SERCOM_MUX_SETTING;
	usart_conf.pinmux_pad0 = EDBG_CDC_SERCOM_PINMUX_PAD0;
	usart_conf.pinmux_pad1 = EDBG_CDC_SERCOM_PINMUX_PAD1;
	usart_conf.pinmux_pad2 = EDBG_CDC_SERCOM_PINMUX_PAD2;
	usart_conf.pinmux_pad3 = EDBG_CDC_SERCOM_PINMUX_PAD3;
	stdio_serial_init(&usart_instance, EDBG_CDC_MODULE, &usart_conf);

	usart_enable(&usart_instance);

	printf("hello");
	ciclometro ciclo;
	init_ciclo(&ciclo);
	
	/* This skeleton code simply sets the LED to the state of the button. */
	getCircunferencia(DIAMETRO_RODA, &ciclo);
	
	while (1) {
		/* Is button pressed? */
		if (port_pin_get_input_level(BUTTON_0_PIN) == BUTTON_0_ACTIVE) {
			/* Yes, so turn LED on. */
			port_pin_set_output_level(LED_0_PIN, LED_0_ACTIVE);
			sendValues(&ciclo);
			} else {
			/* No, so turn LED off. */
			port_pin_set_output_level(LED_0_PIN, !LED_0_ACTIVE);
			//printf("LED OFF\r\n");
		}
	}
}
