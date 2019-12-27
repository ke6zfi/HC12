
#define RF_POWER_UP 0x02, 0x01, 0x00, 0x01, 0xC9, 0xC3, 0x80
#define GPIO_CFG 0x13, 0x60, 0x48, 0x57, 0x56, 0x5A, 0x4B


#define BUFFER_LEN        64
#define DATA_LEN          BUFFER_LEN - 3




//-------------------- PIN CONSTANTS --------------

#define SDN_PORT PORTD
#define SDN_PIN  PIN4

#define CSN_PORT PORTD
#define CSN_PIN  PIN2

#define GPIO0_PORT PORTB
#define GPIO0_PIN  PIN4

#define GPIO1_PORT PORTC
#define GPIO1_PIN  PIN3

#define NIRQ_PORT PORTC
#define NIRQ_PIN  PIN4

#define SCK_PORT PORTC
#define SCK_PIN	PIN5

//MOSI
#define SI_PORT	PORTC 
#define SI_PIN	PIN6

//MISO
#define SO_PORT	PORTC
#define SO_PIN	PIN7


void si4463_init();
void si4463_select();
void si4463_unSelect();
void si4463_reset();
uint8_t* si4463_partInfo();
int16_t si4463_getTemp();
void si446x_read(const uint8_t *txData, uint8_t byteCntTx, uint8_t *rxData, uint8_t byteCntRx);

