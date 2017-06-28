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
#include <pt.h>
#include <my_drivers.h>


static struct pt movimento_pt;

static PT_THREAD(ptmovimento(struct pt *m_pt)){
	PT_BEGIN(m_pt);
	while(1){
		PT_WAIT_UNTIL(m_pt, pulsos >1);
		getSpeed(relogio, &ciclom, pulsos);
		gravaMemDados(&ciclom);
	}
	PT_END(m_pt);
}


int main (void)
{
	system_init();
	/** \brief Realiza a configuração da comunicação serial
	 *		entre a placa e o PC, por exemplo.
	 *		Padão: 9600bps,  8 bits
	 */
	config_usarts();
	init_usart();
	configure_nvm();

	init_ciclo(&ciclom);
	ciclom.situacao = STOPPED;
	lerMemDados(&ciclom);
	
	/**<inicializa o módulo TCC para o contador. */
	configure_tcc();
	configure_tcc_callbacks();
	
	/**< Processos para detecção do movimento da roda. E manipulação
	*	do botão SW0. */
	configure_extint_channel();
	configure_extint_callbacks();
	system_interrupt_enable_global();
	
	PT_INIT(&movimento_pt);
	while (1) {
		
		ptmovimento(&movimento_pt);

	}
}
