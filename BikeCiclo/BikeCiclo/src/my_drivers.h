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
*	Todas as grandezas são do tipo float.
*	As grandezas maxSpeed e travelled serão resetadas
*	apenas quando o botão sw0 for precionado duas vezes, sendo suas
*	informações persistidas mesmo quando o sistema for desligado.
*	Quando o botão sw0 for precionado pela primeira vez
*	será encerrada a sessão de deslocamento.
*/
struct{
	uint32_t startTime; /**< Momento de início do movimento. */
	float maxSpeed;	/**< Velocidade máxima atingida no percurso atual, em km/h. */
	float currentSpeed; /**< Velocidade instantânea, em km/h. */
	float medSpeed; /**< Velocidade média, em km/h. */
	float travelled; /**< Distância percorrida no percurso atual, em km */
	uint32_t endTime; /**< Momento de encerramento da seção, quando o botão sw0 for precionado. */
	}ciclometro;

/** \brief Realiza a configuração da comunicação serial
 *		entre a placa e o PC, por exemplo.
 *		Padão: 9600bps,  8 bits
 *		Após chamar init_usart().
 *
 * \param none
 * \return none
 *
 */  
void config_usarts();

/** \brief Inicializa a comunicação serial
 *		e aguarda uma conexão.
 *		Necessário chamarconfig_usarts() antes.
 *
 * \param none
 * \return none
 *
 */ 
void init_usart();




#endif /* MY_DRIVERS_H_ */