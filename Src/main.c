#include <stdint.h>
#include <stdio.h>
#include <math.h>
// Base addresses for the peripherals
#define RCC_BASE        0x40023800
#define GPIOA_BASE      0x40020000
#define GPIOD_BASE      0x40020C00
#define GPIOE_BASE      0x40021000
#define ADC1_BASE       0x40012000

// Offsets for the specific registers
#define RCC_AHB1ENR     *(volatile uint32_t *)(RCC_BASE + 0x30)
#define RCC_APB2ENR     *(volatile uint32_t *)(RCC_BASE + 0x44)

#define GPIOA_MODER     *(volatile uint32_t *)(GPIOA_BASE + 0x00)
#define GPIOD_MODER     *(volatile uint32_t *)(GPIOD_BASE + 0x00)
#define GPIOE_MODER     *(volatile uint32_t *)(GPIOE_BASE + 0x00)
#define GPIOD_OUTPUT     *(volatile uint32_t *)(GPIOD_BASE + 0x14)
#define GPIOE_OUTPUT     *(volatile uint32_t *)(GPIOE_BASE + 0x14)

#define ADC1_SR         *(volatile uint32_t *)(ADC1_BASE + 0x00)
#define ADC1_CR1        *(volatile uint32_t *)(ADC1_BASE + 0x04)
#define ADC1_CR2        *(volatile uint32_t *)(ADC1_BASE + 0x08)
#define ADC1_SMPR2      *(volatile uint32_t *)(ADC1_BASE + 0x10)
#define ADC1_SQR1       *(volatile uint32_t *)(ADC1_BASE + 0x2C)
#define ADC1_SQR3       *(volatile uint32_t *)(ADC1_BASE + 0x34)
#define ADC1_DR         *(volatile uint32_t *)(ADC1_BASE + 0x4C)


void GPIO_Config(void);
void ADC_Config(void);
uint32_t ADC_Read(void);
void delay();

uint8_t segment_hex[10] = {
		0xC0,
		0xF9,
		0xA4,
		0xB0,
		0x99,
		0x92,
		0x82,
		0xF8,
		0x80,
		0x90
};

int main(void) {
    uint32_t adc_value;
    int temperature;
    int msb, lsb;

    GPIO_Config();
    ADC_Config();

    while (1) {

        adc_value = ADC_Read();
        temperature = (adc_value * 3.3 / 4096.0) / 0.01;
        msb = temperature / 10;
        lsb = temperature % 10;
        GPIOD_OUTPUT = segment_hex[msb];
        GPIOE_OUTPUT = 0x01;
        delay();
        GPIOD_OUTPUT = segment_hex[lsb];
        GPIOE_OUTPUT = 0x02;
        delay();


    }
}

void GPIO_Config(void) {

    RCC_AHB1ENR |= 0x19;


    GPIOA_MODER |= (3 << (0 * 2));
    GPIOD_MODER |= 0x5555;
    GPIOE_MODER |= 0x05;
}

void ADC_Config(void) {

    RCC_APB2ENR |= (1 << 8);
    ADC1_CR2 = 0;
    ADC1_SQR1 = 0;
    ADC1_SQR3 |= (0 << 0);
    ADC1_SMPR2 |= (7 << 0);
    ADC1_CR2 |= (1 << 0);
}

uint32_t ADC_Read(void) {

    ADC1_CR2 |= (1 << 30);
    while (!(ADC1_SR & (1 << 1)));
    return ADC1_DR;
}

void delay()
{
	for(int i = 0; i<10000; i++);
}


