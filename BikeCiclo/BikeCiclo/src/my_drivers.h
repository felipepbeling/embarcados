/*
 * my_drivers.h
 *
 * Created: 23/06/2017 15:52:51
 *  Author: Carlos
 */ 


#ifndef MY_DRIVERS_H_
#define MY_DRIVERS_H_

#define PI 3.14159265358979323846
#define DIAMETRO_RODA 29 //tamanho da roda em polegadas

struct usart_module usart_instance;
struct usart_config usart_conf;

/**
*	Estrutura das grandezas medidas
*	Todas as grandezas s�o do tipo float.
*	As grandezas maxSpeed e travelled ser�o resetadas
*	apenas quando o bot�o sw0 for precionado duas vezes, sendo suas
*	informa��es persistidas mesmo quando o sistema for desligado.
*	Quando o bot�o sw0 for precionado pela primeira vez
*	ser� encerrada a sess�o de deslocamento.
*/
struct{
	uint32_t startTime; /**< Momento de in�cio do movimento. */
	float maxSpeed;	/**< Velocidade m�xima atingida no percurso atual, em km/h. */
	float currentSpeed; /**< Velocidade instant�nea, em km/h. */
	float medSpeed; /**< Velocidade m�dia, em km/h. */
	float travelled; /**< Dist�ncia percorrida no percurso atual, em km */
	uint32_t endTime; /**< Momento de encerramento da se��o, quando o bot�o sw0 for precionado. */
	}ciclometro;

/** \brief Realiza a configura��o da comunica��o serial
 *		entre a placa e o PC, por exemplo.
 *		Pad�o: 9600bps,  8 bits
 *		Ap�s chamar init_usart().
 *
 * \param none
 * \return none
 *
 */  
void config_usarts();

/** \brief Inicializa a comunica��o serial
 *		e aguarda uma conex�o.
 *		Necess�rio chamarconfig_usarts() antes.
 *
 * \param none
 * \return none
 *
 */ 
void init_usart();




#endif /* MY_DRIVERS_H_ */