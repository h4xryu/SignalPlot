#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define CLCD_RS 0b00100000
#define CLCD_RW 0b01000000
#define CLCD_EBIT 0b01000000
#define AT24CxxPORT PORTD
#define AT24CxxDDR DDRD
#define AT24CxxPIN PIND
#define AT24Cxx_SCL 0x04
#define AT24Cxx_SDA 0x08

#define SLAVE1_ADDR 0x27
#define SLAVE2_ADDR 0xC0
#define AT24Cxx_DELAY 5
#define ACK 0x00 // ACK LOW
#define NACK 0x01 // NACK HIGH
#define EEP_WRITE_WAIT 10 // write wait time

#define AT24Cxx_SDA_HIGH() (AT24CxxPORT |= AT24Cxx_SDA)
#define AT24Cxx_SDA_LOW() (AT24CxxPORT &= ~AT24Cxx_SDA)
#define AT24Cxx_SCL_HIGH() (AT24CxxPORT |= AT24Cxx_SCL)
#define AT24Cxx_SCL_LOW() (AT24CxxPORT &= ~(AT24Cxx_SCL))
#define AT24Cxx_SDA_IN() (AT24CxxDDR &= ~AT24Cxx_SDA)
#define AT24Cxx_SDA_OUT() (AT24CxxDDR |= AT24Cxx_SDA)

unsigned char ASCII_Table[16] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};

void USART0_init(void);
void TX0_char(unsigned char Data);
void AT24Cxx_i2c_init(void);
void AT24Cxx_i2c_start(void);
void AT24Cxx_ACK_send(unsigned char ack_data);
void AT24Cxx_i2c_stop(void);
void write_AT24Cxx_i2c_byte(unsigned char byte);
void write_AT24Cxx_i2c_LCDAddressing(unsigned char byte);
unsigned char read_AT24Cxx_i2c_byte(void);
unsigned char AT24Cxx_Read(unsigned char address, unsigned char sub_addr1, unsigned char sub_addr2);
void AT24Cxx_Write(unsigned char address, unsigned char sub_addr1, unsigned char sub_addr2, unsigned char data);
void write_AT24Cxx_i2c_LCDPrint(unsigned char line, unsigned char locaton, unsigned char data);

void Port_init();
void Timer_init();
void ADC_init();
void CLCD_init();
void CLCD_print_char(unsigned char line, unsigned char location, unsigned char c_data);
void CLCD_send_control(unsigned char control);
void CLCD_send_char(unsigned char c_data);
unsigned char ADC_read(unsigned char ch);
void Hex2Dec(unsigned char data);
unsigned char Digit2ASCII(unsigned char data);

volatile unsigned char Wait_time = 0;
volatile unsigned char digit[3];
unsigned int sub_addr = 0x20;
unsigned char save_data = 0, read_data = 0;
unsigned char temp_1, temp_2;
unsigned char ADC_buff;
void setup(){
  
	Port_init();
	Timer_init();
	ADC_init();
	USART0_init();
	
}
 void loop(){
	
  save_data = 0xAA;
	temp_1 = (unsigned char)(sub_addr >> 8); temp_2 = (unsigned char)(sub_addr & 0xFF);
	AT24Cxx_Write(SLAVE1_ADDR, temp_1, temp_2, save_data);
	read_data = AT24Cxx_Read(SLAVE1_ADDR, (unsigned char)(sub_addr >> 8), (unsigned char)(sub_addr & 0xFF));
	TX0_char(ASCII_Table[read_data >> 4]); TX0_char(ASCII_Table[read_data & 0x0F]); TX0_char(' ');
		
		//AT24Cxx_Write(SLAVE2_ADDR, temp_1, temp_2, save_data);
		//read_data = AT24Cxx_Read(SLAVE2_ADDR, (unsigned char)(sub_addr >> 8), (unsigned char)(sub_addr & 0xFF));
		//TX0_char(ASCII_Table[read_data>>4]);TX0_char(ASCII_Table[read_data & 0x0F]);
		//TX0_char('\r');TX0_char('a');
		
	sub_addr ++;
	sub_addr &= 0x1FFF;
	save_data ++;
	_delay_ms(100);
}

void Port_init(void){
	PORTD = 0x0C; // PD1 = 1
	DDRD = 0x0F; 
	DDRB = 0x00;
	
}

void Timer_init(void){
	TCCR1A = (0 << COM1A1) | (0 << COM1A0) | (0 << COM1B1) | (0 << COM1B0) |
	 (0 << WGM11) | (0 << WGM10);
	TCCR1B = (0 << WGM13) | (1 << WGM12) | (0 << CS12) | (1 << CS11) | (1 << CS10);
	TCCR1C = 0;
	OCR1A = 0x09C4;
	TIMSK0 = (1 << OCIE1A); 
}

void ADC_init(void){
	ADMUX = 0x60;
	ADCSRA = 0x87;
}

unsigned char ADC_read(unsigned char Ch){
	Ch &= 0x01;
	
	ADMUX |= (Ch & 0b00011111);
	ADCSRA |= 0x40;
	
}



void Hex2Dec(unsigned char data){
	digit[2] = data % 100;
	data = data - digit[2]*100;
	digit[1] = data%10;
	digit[0] = data - digit[1]*10;
}

unsigned char Digit2ASCII(unsigned char data){
	data &= 0x0F;
	if (data < 10) return (data + '0');
	return (data + 'A');
}

void USART0_init(void){
	UCSR0A = 0x20;
	UCSR0B = 0x18;
	UCSR0C = 0x06;
	UBRR0H = 0;
	UBRR0L = 103;
}

void TX0_char(unsigned char Data){
	while(!(UCSR0A & (1 << UDRE0)));
	UDR0 = Data;
}

void AT24Cxx_i2c_init(void){
	
	AT24CxxPORT |= (AT24Cxx_SDA + AT24Cxx_SCL);
	AT24CxxDDR |= (AT24Cxx_SDA + AT24Cxx_SCL);
	_delay_us(AT24Cxx_DELAY);
	AT24Cxx_SCL_LOW();
	_delay_us(AT24Cxx_DELAY);
}

void AT24Cxx_i2c_start(void) { //start condition
	AT24Cxx_i2c_init();
	
	AT24Cxx_SDA_LOW(); //clear SDA
	_delay_us(AT24Cxx_DELAY);
	AT24Cxx_SCL_LOW();
	_delay_us(AT24Cxx_DELAY);
}


void AT24Cxx_ACK_send(unsigned char ack_data){
	if(ack_data) AT24Cxx_SDA_HIGH(); //NACK
	else AT24Cxx_SDA_LOW(); //ACK
	AT24Cxx_SDA_OUT(); //set SDA to Output
	
	AT24Cxx_SCL_HIGH(); //9th clk rising edge
	_delay_us(AT24Cxx_DELAY);
	AT24Cxx_SCL_LOW();
	_delay_us(AT24Cxx_DELAY);
}

void AT24Cxx_i2c_stop(void){
	AT24Cxx_SDA_OUT(); //set SDA to output
	AT24Cxx_SDA_LOW(); //clear SDA
	AT24Cxx_SCL_LOW(); //clear SCL
	
	_delay_us(AT24Cxx_DELAY);
	
	AT24Cxx_SCL_HIGH(); // set SCL High
	_delay_us(AT24Cxx_DELAY);
	AT24Cxx_SDA_HIGH(); // set SDA High
}

void write_AT24Cxx_i2c_LCDAddressing(unsigned char byte){//SDA의 바이트를 읽어노는 과정
	
	for(int i = 0; i <8; i++){
		if(byte & 0x80) AT24Cxx_SDA_HIGH(); //MSB
		else AT24Cxx_SDA_LOW(); //Clear
		
		AT24Cxx_SCL_HIGH(); //clock data
		byte = byte << 1; //shift data in buffer right
		_delay_us(5);
		AT24Cxx_SCL_LOW();
		_delay_us(AT24Cxx_DELAY);
		 
	}
	AT24Cxx_SDA_LOW();
	AT24Cxx_SDA_IN(); //get ACK
	_delay_us(AT24Cxx_DELAY);
	
	AT24Cxx_SCL_HIGH();
	while(!(AT24CxxPIN & AT24Cxx_SDA)); // wait for ack
  AT24Cxx_SDA_OUT(); //set to output
  _delay_us(AT24Cxx_DELAY);
	AT24Cxx_SCL_LOW();
}

void write_AT24Cxx_i2c_LCDPrint(unsigned char line, unsigned char locaton, unsigned char data){//SDA의 바이트를 읽어노는 과정
	
	for(int i = 0; i <8; i++){
		if(data & 0x80) AT24Cxx_SDA_HIGH(); //MSB
		else AT24Cxx_SDA_LOW(); //Clear
		
		AT24Cxx_SCL_HIGH(); //clock data
		byte = byte << 1; //shift data in buffer right
		_delay_us(5);
		AT24Cxx_SCL_LOW();
		_delay_us(AT24Cxx_DELAY);
		 
	}
	AT24Cxx_SDA_LOW();
	AT24Cxx_SDA_IN(); //get ACK
	_delay_us(AT24Cxx_DELAY);
	
	AT24Cxx_SCL_HIGH();
	while(!(AT24CxxPIN & AT24Cxx_SDA)); // wait for ack
  AT24Cxx_SDA_OUT(); //set to output
  _delay_us(AT24Cxx_DELAY);
	AT24Cxx_SCL_LOW();
}

unsigned char read_AT24Cxx_i2c_byte(unsigned char byte){ 
	unsigned char i, buff=0;
	
	for(i=0; i<8; i++){
		buff <<= 1;
		
		AT24Cxx_SCL_HIGH();
		_delay_us(AT24Cxx_DELAY);
		
		if ((AT24CxxPIN & AT24Cxx_SDA)) buff |= 0x01;
		else buff &= (~0x01);
		
		AT24Cxx_SCL_LOW();
		_delay_us(AT24Cxx_DELAY);
	}
	return buff;
}

unsigned char AT24Cxx_Read(unsigned char address, unsigned char sub_addr1, unsigned char sub_addr2){
	unsigned char buffer;
	
	AT24Cxx_i2c_start();
	write_AT24Cxx_i2c_byte(address + 0x00);
	write_AT24Cxx_i2c_byte(sub_addr1);
	write_AT24Cxx_i2c_byte(sub_addr2);
	AT24Cxx_i2c_stop();
	
	_delay_us(AT24Cxx_DELAY);
	
	AT24Cxx_i2c_start();
	write_AT24Cxx_i2c_byte(address + 0x01);
	buffer = read_AT24Cxx_i2c_byte(0);
	
	AT24Cxx_ACK_send(NACK);
	
	AT24Cxx_i2c_stop();
	
	return buffer;
}

void AT24Cxx_Write(unsigned char address, unsigned char line, unsigned char location, unsigned char data){
	AT24Cxx_i2c_start();
	write_AT24Cxx_i2c_LCDAddressing((address));
	write_AT24Cxx_i2c_byte(line);
	write_AT24Cxx_i2c_byte(location);
	write_AT24Cxx_i2c_byte(data);
	AT24Cxx_i2c_stop();
	_delay_ms(EEP_WRITE_WAIT);
}


ISR(TIMER1_COMPA_vect)
{
	if(Wait_time) Wait_time --;
}