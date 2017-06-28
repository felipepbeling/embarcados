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

#define PINO_ENTRADA PIN_PA07

typedef enum movimento_situacao {STOPPED=1, RUNNING, READY} movimento;
	
struct usart_module usart_instance; /**< Estrutura de controle da comunica��o serial. */
struct usart_config usart_conf; /**< Estrutura de configura��o da comunica��o serial. */
//struct nvm_config config_nvm; /**< Estrutura de configura��o do acesso � eeprom. */

struct tcc_module tcc_instance; /**< Instancia da estrutura do m�dulo TCC. */


uint8_t pulsos;
	
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
	uint32_t lastTime; /**< momento do inicio da volta da roda, utilizado para c�lculo da velocidade, etc .*/
	uint16_t contador; /**< contador de pulsos, este representa as voltas j� executadas. */
	}ciclometro;

ciclometro ciclom;

enum status_code error_code;/**C�digos de erro para leitura e grava��o na mem�ria. */
uint8_t page_buffer[NVMCTRL_PAGE_SIZE]; /**< vetor para leitura e grava��o em mem�ria. */

void config_usarts(void);
void init_usart(void);

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
void getSpeed(uint32_t final_time, ciclometro *ciclo, uint16_t contador);

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

void configure_extint_channel(void);
void configure_extint_callbacks(void);

/** \brief  Configura��o do m�dulo TCC
 *		Utilizado 32kHz, prescaler 16
  *		Cada ciclo representa 0,5ms
 * \param none 
 * \return none
 *
 */
void configure_tcc(void);

/** \brief  Configura��o das chamadas quando o intervalo de tempo 
 *		especificado for concluido
 *
 * \param none 
 * \return none
 *
 */
void configure_tcc_callbacks(void);

/** \brief  Configura��o do m�dulo NVM para leitura e grava��o na EEPROM
 *
 * \return none
 *
 */
void configure_nvm(void);

/** \brief  Grava os dados da se��o em mem�ria
 *	maxSpeed, medSpeed, tamRoda, travelled,
 *	startTime, lastTime, situacao, contadore situacao.
 * \param ciclometro *ciclo
 * \return none
 *
 */
void gravaMemDados(ciclometro *ciclo);

/** \brief  Leitura dos dados salvos em mem�ria
 *	maxSpeed, medSpeed, tamRoda, travelled,
 *	startTime, lastTime, situacao, contadore situacao.
 * \param ciclometro *ciclo
 * \return none
 *
 */
void lerMemDados(ciclometro *ciclo);


#endif /* MY_DRIVERS_H_ */