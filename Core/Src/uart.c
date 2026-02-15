//UART Baremetal Source File

/*
Notes: Virtual Com Port on Nucleo H5A3ZI-Q is connected to USART3. 
- TX is on PD8 and RX is on PD9 (AF7) check usermanual 109/277
- AHB2 Clock (check Block diagram in datasheet, page 19/277)
- Target Baudrate: 115200

The higher sampling rate, the better to reduce for noise rejection and avoid deviation from sampling of bit state, but require higher clock frequency.
*/

//Includes
#include "uart.h"

void UART_Init(void) {

    //Initlize the Peripheral Clock
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIODEN; //Enable GPIOD Clock
    RCC->APB1ENR |= RCC_APB1ENR_USART3EN; //Enable USART3 Clock

    //GPIO Configuration for USART3
    //GPIO Mode to Alternate Function
    GPIOD->MODER &= ~((3U << (2 * 8)) | (3U << (2 * 9))); //Clear GPIOD MODER bits
    GPIOD->MODER |= ((2U << (2 * 8)) | (2U << (2 * 9))); //AF Mode

    //Setting to Alternative Function 7 AF7  
    GPIOD->AFRH &= ~((16U << (4 * 0) | (16U << (4 * 1)))); //Clear ARFH bits
    GPIOD->AFRH |= ((7U << (4 * 0)) | (7U << (4 * 1))); //AF7

    //UART3 Configuration
    //Clear Bits 
    USART3->CR1 &= ~((3U << 30) //RXFIFO and TXFIFO Full Interupt Disable
                    |(USART_CR1_M1) //1 start bit, 8 data bits, n stop bits
                    |(3U << 26) //End of block interrupt disable
                    |(USART_CR1_OVER8) // Oversampling by 16
                    |(USART_CR1_CMIE) //Character match interrupt disable
                    |(USART_CR1_MME) //Mute Mode Disable
                    |(USART_CR1_M0) //Word Length 
                    |(USART_CR1_PCE) //Parity Control Disable
                    |(USART_CR1_PEIE) //PE Interrupt Disable
                    |(USART_CR1_TXFNFIE) //TXFIFO Not Full interrupt Disable
                    |(USART_CR1_TCIE) //Transmission complete interrupt disable
                    |(USART_CR1_RXFNFIE) //RXFIFO Not Full interrupt Disable
                    |(USART_CR1_IDLEIE) //IDLE Interrupt Enable
                    |(USART_CR1_TE) | (USART_CR1_RX) //Disable receiver and transmitter
                    |(USART_CR1_UE)); //Disable UART3

    //Enable UART3
    USART3->CR1 |= (1U << 0);
}