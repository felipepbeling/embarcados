/*
 * my_drivers.h
 *
 * Created: 23/06/2017 15:52:51
 *  Author: Carlos
 */ 


#ifndef MY_DRIVERS_H_
#define MY_DRIVERS_H_

#include <asf.h>
#include <stdio.h>
#include <stdint.h>

#define PI				3.14159265358979323846
#define DIAMETRO_RODA	29						//tamanho da roda em polegadas

enum movimento {STOPPED, RUNNING};
	
uint32_t relogio;
/**
*	Estrutura das grandezas medidas
*	Todas as grandezas s�o do tipo float.
*	As grandezas maxSpeed, medSpeed e travelled ser�o resetadas
*	apenas quando o bot�o sw0 for precionado duas vezes, sendo suas
*	informa��es persistidas mesmo quando o sistema for desligado.
*	Quando o bot�o sw0 for precionado pela primeira vez
*	ser� encerrada a sess�o de deslocamento.
*/
typedef struct{
	float tamRoda; /**<comprimento da circunfer�ncia da roda em metros. */
	uint32_t startTime; /**< Momento de in�cio do movimento. */
	float maxSpeed;	/**< Velocidade m�xima atingida no percurso atual, em km/h. */
	float currentSpeed; /**< Velocidade instant�nea, em km/h. */
	float medSpeed; /**< Velocidade m�dia, em km/h. */
	float travelled; /**< Dist�ncia percorrida no percurso atual, em km */
	uint32_t endTime; /**< Momento de encerramento da se��o, quando o bot�o sw0 for precionado. */
	movimento situacao; /**<identifica a condi��o atual da se��o */
	}ciclometro;


/** \brief Inicializa a a estrutura ciclometro
 *		que persiste as informa��es do deslocamento
 *
 * \param &ciclo tipo ciclometro
 * \return none
 *
 */
void init_ciclo(ciclometro *ciclo);

/** \brief Calcula o comprimento da 
 *		circunferencia da roda
 *
 * \param start_time tipo uint32_t instante do pulso anterior em ms 
 * \param final_time tipo uint32_t instante do pulso atual em ms
 * \param *ciclo tipo ciclometro struct que persiste os dados
 * \return none
 *
 */
void getSpeed(uint32_t start_time, uint32_t final_time, ciclometro *ciclo);

/** \brief Calcula o comprimento da 
 *		circunferencia da roda
 *
 * \param roda tipo uint8_t Di�metro da roda em polegadas
 * \param struct ciclometro guarda o valor calculado da circunferencia
 * \return none
 *
 */
void getCircunferencia(uint8_t roda, ciclometro *ciclo);

/** \brief  envia os dados pela serial
 *		atrav�s de um printf para serem apresentados ao usu�rio
 *
 * \param ciclo tipo ciclometro 
 * \return none
 *
 */
void sendValues(ciclometro *ciclo);


#endif /* MY_DRIVERS_H_ */