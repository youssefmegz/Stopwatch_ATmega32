/***************************************************************************************************
 * 	mini_project2.c
 *
 *  Created on: Sep 11, 2024
 *  Author: Youssef Maged Tadros
 ***************************************************************************************************/
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/delay.h>
/***************************************************************************************************/
signed char seconds=0,minutes=0,hours=0;		/*Time counters*/
unsigned char pause_flag=0;						/*Flag for increment and decrement buttons*/
unsigned char button_flag[6]={0};				/*Flag to ensure single increment or decrement*/
unsigned char mode_flag=0;						/*Flag to select count mode*/
unsigned char mode_button_flag=0;				/*Flag to ensure one change in mode*/

/***************************************************************************************************/
void INT0_Init()				/*Reset Interrupt*/
{
	SREG|=(1<<7);				/*Global interrupt enable*/
	MCUCR |= (1<<ISC01);		/*Interrupt with falling edge*/
	GICR |= (1<<INT0);			/*Module interrupt enable*/
}
/***************************************************************************************************/
void INT1_Init()				/*Pause Interrupt*/
{
	SREG|=(1<<7);				/*Global interrupt enable*/
	MCUCR |= (1<<ISC10) | (1<<ISC11);		/*Interrupt with rising edge*/
	GICR |= (1<<INT1);			/*Module interrupt enable*/
}
/***************************************************************************************************/
void INT2_Init()				/*Resume Interrupt*/
{
	SREG|=(1<<7);				/*Global interrupt enable*/
	MCUCSR&=~(1<<ISC2);			/*Interrupt with falling edge*/
	GICR |= (1<<INT2);			/*Module interrupt enable*/
}
/***************************************************************************************************/
void Timer1_Init()				/*Timer control using Timer1*/
{

	SREG|= (1<<7);				/*Global interrupt enable*/
	TCCR1A= (1<<FOC1A);			/*Non PWM*/
	TCCR1B= (1<<WGM12) | (1<<CS12) | (1<<CS10);		/*1024 Prescaler*/
	TCNT1=0;					/*Start count from 0*/
	OCR1A=15625;				/*Number of ticks to interrupt every one second */
	TIMSK= (1<<OCIE1A);			/*Module interrupt enable*/
}
/***************************************************************************************************/
ISR(INT0_vect)					/*Reset behavior*/
{
	seconds=0;
	minutes=0;
	hours=0;
}
/***************************************************************************************************/
ISR(INT1_vect)					/*Pause behavior*/
{
TCCR1B &=~ ((1<<CS12)|(1<<CS10));			/*Stop timer clock*/
pause_flag=1;					/*Flag to enable increment and  decrement buttons*/
}
/***************************************************************************************************/
ISR(INT2_vect)					/*Resume behavior*/
{
TCCR1B |= (1<<CS12)|(1<<CS10);	/*Start timer clock again*/
pause_flag=0;					/*Flag to disable increment and  decrement buttons*/
}
/***************************************************************************************************/
ISR(TIMER1_COMPA_vect)			/*Handling stopwatch limits in both modes*/
{
if(PORTD & (1<<PD5))			/*If the yellow LED is on Count down*/
{
	seconds--;
		if((seconds==-1)&&((minutes!=0)||(hours!=0)))
		{
			seconds=59;
			minutes--;
			if(minutes==-1)
			{
				seconds=59;
				minutes=59;
				hours--;
			}

			if(hours==-1)
			{
					hours=0;
			}
		}

	if(seconds==-1 && minutes==0 && hours==0)
	{	seconds=0;
		PORTD|=(1<<PD0);
		TCCR1B &=~ ((1<<CS12)|(1<<CS10));
	}
}

else						/*If the RED LED is on Count up*/
{
	seconds++;
	if(seconds==60)
	{
		seconds=0;
		minutes++;
		if(minutes==60)
		{
			seconds=0;
			minutes=0;
			hours++;
		}

		if(hours==100)
		{
				seconds=0;
				minutes=0;
				hours=0;
		}
	}
}
}
/***************************************************************************************************/

int main(void){
	DDRA =0x3F;			/*Screen Enables (0b00111111)*/
	PORTA=0;
	DDRB=0;				/*Push Buttons*/
	PORTB=0xFF;			/*0b11111111*/
	DDRC=0x0F;			/*Screen Numbers (0b00001111)*/
	PORTC=0;
	DDRD=0x31;			/*LEDS and alarm (0b00110001)*/
	PORTD=0x14;			/*(0b00010100)*/

	Timer1_Init();
	INT0_Init();
	INT1_Init();
	INT2_Init();

	while(1)
	{

		if(!(PINB & (1<<PB7)))					/*if mode button pressed convert between count down and count up*/
		{
			_delay_ms(30);
		if(!(PINB & (1<<PB7)))
		{
			if(mode_button_flag==0)
			{
			if(mode_flag==0)
			{
			PORTD &=~ (1<<PD4);					/*Count up LED OFF*/
			PORTD |= (1<<PD5);					/*Count down LED ON*/
			mode_flag=1;
			}
			else
			{
				PORTD |= (1<<PD4);					/*Count up LED ON*/
				PORTD &=~ (1<<PD5);					/*Count down LED OFF*/
				PORTD&=~(1<<PD0);					/*Stop the alarm*/
				mode_flag=0;
			}
			mode_button_flag=1;
			}
			}
		}
		else mode_button_flag=0;

/*Increment and decrement buttons*/

		if(pause_flag==1){

			if(!(PINB & (1<<PB1)))
			{
				_delay_ms(30);
			if(!(PINB & (1<<PB1)))
			{
				if(button_flag[0]==0)
				{
				if(hours!=99)
				hours++;
				button_flag[0]=1;
				}
			}
			}
			else button_flag[0]=0;

			if(!(PINB & (1<<PB0)))
			{
				_delay_ms(30);
			if(!(PINB & (1<<PB0)))
			{
				if(button_flag[1]==0)
				{
				if(hours!=0)
				hours--;
				button_flag[1]=1;
				}
			}
			}
			else button_flag[1]=0;

			if(!(PINB & (1<<PB4)))
			{
				_delay_ms(30);
			if(!(PINB & (1<<PB4)))
			{
				if(button_flag[2]==0)
				{
				if(minutes!=59)
				minutes++;
				button_flag[2]=1;
				}
			}
			}
			else button_flag[2]=0;

			if(!(PINB & (1<<PB3)))
			{
				_delay_ms(30);
			if(!(PINB & (1<<PB3)))
			{
				if(button_flag[3]==0)
				{
				if(minutes!=0)
				minutes--;
				button_flag[3]=1;
				}
			}
			}
			else button_flag[3]=0;

			if(!(PINB & (1<<PB6)))
			{
				_delay_ms(30);
			if(!(PINB & (1<<PB6)))
			{
				if(button_flag[4]==0)
				{
				if(seconds!=59)
				seconds++;
				button_flag[4]=1;
				}
			}
			}
			else button_flag[4]=0;

			if(!(PINB & (1<<PB5)))
			{
				_delay_ms(30);
			if(!(PINB & (1<<PB5)))
			{
				if(button_flag[5]==0)
				{
				if(seconds!=0)
				seconds--;
				button_flag[5]=1;
				}
			}
			}
			else button_flag[5]=0;
		}

		PORTA=0b00000001;					/*Light first hours screen*/
		PORTC=hours/10;
		_delay_ms(2);
		PORTA=0b00000010;					/*Light second hours screen*/
		PORTC=hours%10;
		_delay_ms(2);
		PORTA=0b00000100;					/*Light first minutes screen*/
		PORTC=minutes/10;
		_delay_ms(2);
		PORTA=0b00001000;					/*Light second minutes screen*/
		PORTC=minutes%10;
		_delay_ms(2);
		PORTA=0b00010000;					/*Light first seconds screen*/
		PORTC=seconds/10;
		_delay_ms(2);
		PORTA=0b00100000;					/*Light second minutes screen*/
		PORTC=seconds%10;
		_delay_ms(2);

	}
	return 0;
}
