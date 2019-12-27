#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "stm8s.h"
#include "delay.h"
#include "gpio.h"
#include "si4463.h"
#include "spi.h"

const uint8_t RF_POWER_UP_DATA[] =                        {RF_POWER_UP};
const uint8_t GPIO_CFG_DATA[] =                           {GPIO_CFG};

void si4463_init() {

	// set CSN_PIN as output pin 
	PinOutput(CSN_PORT,CSN_PIN);
	PinOutputPullUp(CSN_PORT,CSN_PIN);
	PinOutputSpeed2MHZ(CSN_PORT,CSN_PIN);

	//Setup SDN and turn on chip
	PinOutput(SDN_PORT,SDN_PIN);
	PinOutputPullUp(SDN_PORT,SDN_PIN);
	PinOutputSpeed2MHZ(SDN_PORT,SDN_PIN);

	PinInput(GPIO0_PORT,GPIO0_PIN);
	PinInputFloat(GPIO0_PORT,GPIO0_PIN);

	PinInput(GPIO1_PORT,GPIO1_PIN);
	PinInputFloat(GPIO1_PORT,GPIO1_PIN);

	//NIRQ
	PinInput(NIRQ_PORT,NIRQ_PIN);
	PinInputFloat(NIRQ_PORT,NIRQ_PIN);

	PinInput(SO_PORT,SO_PIN);	//MISO
	PinInputFloat(SO_PORT,SO_PIN);

	PinOutput(SI_PORT,SI_PIN);	//MOSI
	PinOutputPullUp(SI_PORT,SI_PIN);
	PinOutputSpeed2MHZ(SI_PORT,SI_PIN);

	SPI_init();		                      
	si4463_reset();
}

void si4463_select() {
	uint8_t rv=0;
	do {
		PinHigh(CSN_PORT,CSN_PIN);
		_delay_us(5); //TODO: Choose better value
		PinLow(CSN_PORT,CSN_PIN);
		SPI_write(0x44);
		rv = SPI_write(0x00); //Read CTS
		printf("{%X}",rv);
	} while(rv!=0xFF);
}

void si4463_unSelect() {
	PinHigh(CSN_PORT,CSN_PIN);
}

void si446x_read(const uint8_t *txData, uint8_t byteCntTx, uint8_t *rxData, uint8_t byteCntRx) {
    
    /* There was a bug in A1 hardware that will not handle 1 byte commands. 
       It was supposedly fixed in B0 but the fix didn't make it at the last minute, so here we go again */
    if (byteCntTx == 1)
        byteCntTx++;
    
    PinLow(CSN_PORT, CSN_PIN);
      
    for (int j = 0; j < byteCntTx; j++) {
      SPI_write(txData[j]);  
    } 
    
    PinHigh(CSN_PORT, CSN_PIN);

    _delay_us(20);

    PinLow(CSN_PORT, CSN_PIN);
    
    uint8_t reply = 0x00;
    while (reply != 0xFF) {       
       SPI_write(0x44);
       reply = SPI_write(0x00);
       if (reply != 0xFF) {
	 PinHigh(CSN_PORT, CSN_PIN);
         _delay_us(20);
         PinLow(CSN_PORT, CSN_PIN);
       }
    }
    
    for (int k = 0; k < byteCntRx; k++) {
	rxData[k]=SPI_write(0x00);
    }
       
    PinHigh(CSN_PORT, CSN_PIN);
    _delay_ms(50);   //make sure spi communication has finished
}

int16_t si4463_getTemp() {
  const uint8_t txData[] = {0x14, 0x10};
  uint8_t rxData[6];
  si446x_read(txData, sizeof(txData), rxData, sizeof(rxData));
  uint16_t adc16 = rxData[5] | ((rxData[4] & 0x7) << 8);
  int16_t temp = (89900 * adc16) / 4096 - 29300;
  return temp;
}


uint8_t* si4463_partInfo() {
	printf("Reading Part Info: ");
        const char PART_INFO_command[] = {0x01}; // Part Info
        uint8_t rxData[8];
        si446x_read(PART_INFO_command, 1, rxData, 8);
        for(int i=0;i<8;i++) {
		printf(":%X",rxData[i]);
	}

	return rxData;
}

void si4463_powerup() {
	uint8_t i;
	printf("Power Up\r\n");
	si446x_read(RF_POWER_UP_DATA,sizeof(RF_POWER_UP_DATA),NULL,0);
}

void si4463_gpiocfg() {
	uint8_t i,r;
	printf("GPIO CFG\r\n");
	si4463_select();
  	for (i = 0; i< 7; i++)
		SPI_write(GPIO_CFG_DATA[i]);  
	si4463_unSelect();

	si4463_select();
  	for (i = 0; i< 7; i++) {
		r=SPI_write(0x00); 
		printf(":%X",r);
	} 
	si4463_unSelect();
	printf("\r\n");
}

void si4463_reset() {
	PinHigh(SDN_PORT,SDN_PIN);
	_delay_ms(4);
	PinLow(SDN_PORT,SDN_PIN);	//Wake up
	_delay_ms(20);			//Wait

	PinHigh(CSN_PORT,CSN_PIN);
   	si4463_powerup();
	_delay_ms(40);
	si4463_gpiocfg();
}



