//UART Baremetal Source File

/*
Notes: Virtual Com Port on Nucleo H5A3ZI-Q is connected to USART3. 
- TX is on PD8 and RX is on PD9 (AF7) check usermanual 109/277
- AHB2 Clock (check Block diagram in datasheet, page 19/277)
- Target Baudrate: 115200

The higher sampling rate, the better to reduce for noise rejection and avoid deviation from sampling of bit state, but require higher clock frequency.
*/

//Includes
#include "stm32h5xx.h"
#include <uart.h>

void UART_Init(void) {

    //Initlize the Peripheral Clock
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIODEN; //Enable GPIOD Clock
    RCC->APB1HENR |= RCC_APB1LENR_USART3EN; //Enable USART3 Clock

    //GPIO Configuration for USART3
    //GPIO Mode to Alternate Function
    GPIOD->MODER &= ~((3U << (2 * 8)) | (3U << (2 * 9))); //Clear GPIOD MODER bits
    GPIOD->MODER |= ((2U << (2 * 8)) | (2U << (2 * 9))); //AF Mode

    //Setting to Alternative Function 7 AF7  
    GPIOD->AFR[1] &= ~((16U << (4 * 0) | (16U << (4 * 1)))); //Clear ARFH bits
    GPIOD->AFR[1] |= ((7U << (4 * 0)) | (7U << (4 * 1))); //AF7

    //UART3 Configuration
    //Control Register 1
    USART3->CR1 &= ~((3U << 30) //RXFIFO and TXFIFO Full Interupt Disable
                    |(USART_CR1_M1) //1 start bit, 8 data bits, n stop bits
                    |(3U << 26) //End of block interrupt disable
                    |(USART_CR1_OVER8) // Oversampling by 16
                    |(USART_CR1_CMIE) //Character match interrupt disable
                    |(USART_CR1_MME) //Mute Mode Disable
                    |(USART_CR1_M0) //Word Length 
                    |(USART_CR1_PCE) //Parity Control Disable
                    |(USART_CR1_PEIE) //PE Interrupt Disable
                    |(1U << 7) //TXFIFO Not Full interrupt Disable
                    |(USART_CR1_TCIE) //Transmission complete interrupt disable
                    |(1U << 5) //RXFIFO Not Full interrupt Disable
                    |(USART_CR1_IDLEIE) //IDLE Interrupt Enable
                    |(USART_CR1_TE) | (USART_CR1_RE) //Disable receiver and transmitter
                    |(USART_CR1_UE)); //Disable UART3

    //Control Register 2
    USART3->CR2 &=  ~((USART_CR2_RTOEN) //Receiver timeout disable
                    |(USART_CR2_ABREN) //Auto baud rate detection disable
                    |(USART_CR2_MSBFIRST) //Transmit with LSB
                    |(USART_CR2_TXINV) | (USART_CR2_RXINV) //Using standard logic levels
                    |(USART_CR2_SWAP) //Standard pinout , no swapping TX and RX
                    |(USART_CR2_LINEN) //LIN mode disable
                    |(USART_CR2_STOP) //1 stop bit
                    |(USART_CR2_CLKEN) //Clock Disable
                    |(USART_CR2_LBDIE)); //LIN break detection interrupt disable
         
    //Control Register 3
    USART3->CR3 &= ~((USART_CR3_DEM) //Driver Enable Disable
                    |(USART_CR3_RXFTIE) | (USART_CR3_TXFTIE)//RXFIFO and TXFIFO Threshold interrupt disable
                    |(USART_CR3_TCBGTIE) //Transmission complete before guardtime, interrupt disable
                    |(USART_CR3_WUFIE) //Wake-up form low-power mode interrupt disable
                    |(USART_CR3_CTSIE) 
                    |(USART_CR3_CTSE) | (USART_CR3_RTSE) //CTS and RTS enable
                    |(USART_CR3_HDSEL) //Half Duplex is not selected
                    |(USART_CR3_IREN) //IrDA mode Disable
                    |(USART_CR3_EIE)); //Error interupt disable 
    
    USART3->CR3 |=  ((USART_CR3_OVRDIS) //Overun flag disable
                    |(USART_CR3_ONEBIT) //One sample bit method as non-noisy environment
                    |(USART_CR3_DMAT) | (USART_CR3_DMAR)); //DMA receiver and transmitter disable
    
    //Enable UART3
    USART3->CR1 |= (1U << 0);
}