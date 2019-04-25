/* Laboratório 11 - PWM
 * Aluna: Sara Sousa de Oliveira (205733)
 * main.c*/

#include "lcd.h"
#include "delay.h"
#include "derivative.h" /* include peripheral declarations */
#include <stdio.h>
#define SET_BIT(reg, idx)	reg = (reg | (1 << idx))		// Macro que seta o bit idx do registrador reg

static int duty_cycle = 0;

void init_PORT(void){
	SET_BIT(SIM_SCGC5, 9); // Ativo clock da PORTA.

	GPIOA_PDDR = 0; // Configuracao das portas A como entradas.
	// Para BOTAO S1:
	PORTA_PCR4 &= 0xFFF0F8FF;
	SET_BIT(PORTA_PCR4, 8); // Coloco alternativa 1, que relaciona a PTA4 com o botao (GPIO).
	SET_BIT(PORTA_PCR4, 17); // Interrupcao na borda de descida (secao IRQC no reg PORTA_PCR4).
	SET_BIT(PORTA_PCR4, 19);

	// Para BOTAO S2:
	PORTA_PCR5 &= 0xFFF0F8FF;
	SET_BIT(PORTA_PCR5, 8); // Coloco alternativa 1, que relaciona a PTA5 com o botao (GPIO).
	SET_BIT(PORTA_PCR5, 17); // Interrupcao na borda de descida (secao IRQC no reg PORTA_PCR5).
	SET_BIT(PORTA_PCR5, 19);

	// Para botao S3:
	PORTA_PCR12 &= 0xFFF0F8FF;
	SET_BIT(PORTA_PCR12, 8); // Coloco alternativa 1, que relaciona a PTA12 com o botao (GPIO).
	SET_BIT(PORTA_PCR12, 17); // Interrupcao na borda de descida (secao IRQC no reg PORTA_PCR12).
	SET_BIT(PORTA_PCR12, 19);

	// Interrupcoes:
	NVIC_IPR7 &= 0xFF3FFFFF; // Limpar prioridades.
	//SET_BIT(PORTA_PCR4, 24); // Limpar eventuais pedidos de interrupcoes que estejam pendentes.
	//SET_BIT(PORTA_PCR5, 24);
	//SET_BIT(PORTA_PCR12, 24);
	SET_BIT(NVIC_IPR7, 22); // Definir prioridade no NVIC (NVIC_IPR7). Coloquei 1 pq so uso essa interupcao NVIC.
	SET_BIT(NVIC_ISER, 30); // Habilitar interrupção que estou usando (30), com NVIC_ISER.
}

void init_TPM(){
	SET_BIT(SIM_SCGC6, 25); // Ativo clock do TPM1.
	SET_BIT(SIM_SCGC5, 10); // Ativo clock da PORTB.

	PORTB_PCR0 &= 0xFFFFF8FF;
	SET_BIT(PORTB_PCR0, 8); // Coloco alternativa 3, que relaciona PTB0 com TPM1_CH0.
	SET_BIT(PORTB_PCR0, 9);

	SIM_SOPT2 &= 0xFCFFFFFF; // Zera bit 25 e seta bit 24 para configurar o clock.
	SET_BIT(SIM_SOPT2,24);

	TPM1_SC = 0; // Zero tudo antes de fazer alguma alteração.
	TPM1_MOD = 0x3333;   // Valor de MOD = 13107
	TPM1_SC = 0b000101100; // Ativa CPWMS (clock do PWM), coloca o LPTPM counter para incrementar em cada
							// clock do LPTPM e faz prescaler = 16.
	TPM1_C0V = 13107; // Valor Match do canal usado, o canal 0.
	TPM1_C0SC = 0b01101100; // Faz o canal 0 ficar com centro alinhado (PWM).

}

void imprime_lcd(){
	char impressao[10];
	sprintf(impressao, "%d%c   %c", duty_cycle, '%', '\0');
	puts_LCD(impressao);
	setpos_LCD(1, 1);
}

void PORTA_IRQHandler(){ // O que a interrupcao, relativa aos botoes S3 e S2, vai fazer.
	int a, pta4, pta5, pta12;
	a = GPIOA_PDIR;       // Faz a leitura no PORTA (16 bits)
	pta4  = a & (1<<4); // S1
	pta5  = a & (1<<5); // S2
	pta12 = a & (1<<12); // S3

	if(pta4==0){ // botao S1 apertado -> duty_cycle zera.
		duty_cycle = 0;
		TPM1_C0V =  13107 *( 1 - (duty_cycle/100) );
		SET_BIT(PORTA_PCR4, 24); // Limpa pedido de interrupcao.
	}
	else if(pta5==0){ // botao S2 apertado -> diminuicao do duty_cycle.
		if(duty_cycle>0){
			duty_cycle = duty_cycle - 5;
			TPM1_C0V =  TPM1_C0V + 655;
		}
		SET_BIT(PORTA_PCR5, 24); // Limpa pedido de interrupcao.
	}
	else if(pta12==0){ // botao S3 apertado -> aumento do duty_cycle.
		if(duty_cycle<100){
			duty_cycle = duty_cycle + 5;
			TPM1_C0V =  TPM1_C0V - 655;
		}
		SET_BIT(PORTA_PCR12, 24); // Limpa pedido de interrupcao.
	}
}

int main(void)
{
	init_GPIO_LCD();
	init_LCD();
	init_PORT();
	init_TPM();

	for(;;){
		if(duty_cycle<=100 && duty_cycle>=0){
			imprime_lcd();
		}
	}
	return 0;
}
