#include <stdio.h>
#include <string.h>
#include "lib/stm8s.h"
#include "lib/delay.h"
#include "lib/gpio.h"
#include "lib/uart.h"
#include "lib/interrupt.h"
#include "lib/spi.h"
#include "lib/timer.h"
#include "lib/eeprom.h"
#include "lib/si4463.h"

#define SETIO_PORT PORTB
#define SETIO_PIN PIN5

#define RX_PORT PORTD
#define RX_PIN PIN6

#define TX_PORT PORTD
#define TX_PIN	PIN5

#define FALSE 0
#define TRUE 1

uint8_t syncWord[2] = {199, 10};
volatile uint8_t packetWaiting;

unsigned long lastSend = 0;
unsigned int sendDelay = 50000;


ISR_PORTD(handlerD) {
	packetWaiting=TRUE;
}

#define BUFFER_LEN 64

void main() {
    uint8_t length;
    char packet[BUFFER_LEN];
    unsigned long now = 0;

// setup
    uart_init();

    si4463_init();
    printf("SI4463_init() done");


    si4463_partInfo();
	printf("\r\nSI4463_partInfo() done");
	
    printf("SI4463 radio initialized.");

    // set up interrupt
    //   PinInputInterruptEnable(GD0_PORT,GD0_PIN);
    // Enable PORTD interrupts
    // PortDIntSens(FALLING_ONLY);
    enable_interrupts();
// loop
    while(1) {    
        now++;
        if (now > lastSend + sendDelay) {
    	   // PinInputInterruptDisable(GD0_PORT,GD0_PIN);
            lastSend = now;
            const char *message = "hello world HC12";
            // We also need to include the 0 byte at the end of the string
            length = strlen(message)  + 1;
            strncpy((char *) packet, message, length);
            //printf("\r\nGetting temp\r\n");
	    int16_t t=si4463_getTemp();
            printf("\r\nTemp is: %d",t);

           //PinInputInterruptEnable(GD0_PORT,GD0_PIN);
        }
    }
}
