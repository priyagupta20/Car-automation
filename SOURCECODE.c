#include<reg51.h>
sbit motor_pin_1 = P1^0;
sbit motor_pin_2 = P1^1;
sbit motor_pin_3 = P1^2;
sbit motor_pin_4 = P1^3;
sbit rs = P0^0;  
sbit e =  P0^1;  
unsigned char number[]="7838039260";
int x;
sbit trig=P2^1;
unsigned int target_range=0,timer_val;
char button;
void delay(unsigned int);
void GSM_init()            // serial port initialization 
{
    TMOD=0x20;            // Timer 1 selected, Mode 2(8-bit auto-reload mode)
    TH1=0xfd;            // 9600 baudrate
    SCON=0x50;            // Mode 1(8-bit UART), receiving enabled
    TR1=1;                // Start timer
}
void lcd_cmd(unsigned char command)  //Function to send command instruction to LCD
{
    P2 = command;
    rs= 0;
    e=1;
    delay(1);
    e=0;
}

void lcd_data(unsigned char disp_data)  //Function to send display data to LCD
{
    P2 = disp_data;
    rs= 1;
    e=1;
    delay(1);
    e=0;
}
void lcd_init()    //Function to prepare the LCD  and get it ready
{
    lcd_cmd(0x38);  // for using 2 lines and 5X7 matrix of LCD
    delay(10);
    lcd_cmd(0x0F);  // turn display ON, cursor blinking
    delay(10);
    lcd_cmd(0x01);  //clear screen
    delay(10);
    lcd_cmd(0x80);  // bring cursor to beginning of first line
    delay(10);
}        
void lcd_string(unsigned char *str)    // Function to display string on LCD
{
    int i=0;
    while(str[i]!='\0')
    {
       lcd_data(str[i]);                  
       i++;
       delay(10);
      if(i==15) lcd_cmd(0xc2);                                          
       }
    return; 
}
void GSM_write(unsigned char ch)    // Function to send commands to GSM
{
    SBUF=ch;        // Put byte in SBUF to send to GSM
    while(TI==0);        //wait until the byte trasmission
    TI=0;            //clear TI to send next byte.      
}
void usinterrupt() interrupt 1
{ TR0=0;
	P3|=0x04;		//P3.2 as INPUT
		TMOD=0x21;
		TH0=0xFF;TL0=0xF5;
		trig=1;
		TR0=1;
		while(!TF0);
		TR0=0;
		TF0=0;
		TMOD=0x29;		//timer0 in 16 bit mode with gate enable
		TH0=0x00; TL0=0x00;
		TR0=1;		//timer0 run enabled
		trig=0;
		while(!INT0);
		while (INT0);
		TR0=0;
		timer_val=TH0<<8|TL0;
		if(timer_val<35000)
			target_range=timer_val*0.01860775;
		else
			target_range=0; // indicates that there is no obstacle in front of the sensor
		if(target_range>10 && target_range<20 )
		       {motor_pin_1 = 0;
						motor_pin_2 = 1;
					  motor_pin_3 = 0;
					  motor_pin_4 = 1;}
		else if(target_range<10 && target_range>2 )
		       {motor_pin_1 = 0;
						motor_pin_2 = 0;
					  motor_pin_3 = 0;
					  motor_pin_4 = 0;}
					 else if(target_range<2 && target_range!=0)
					 { 
    lcd_init();                            
    GSM_init();                
    delay(200);
    lcd_string("Interfacing GSM with 8051");
    delay(200);
    lcd_cmd(0x01);            //    Clear LCD screen
    delay(10); 
    GSM_write('A');              // Sending 'A' to GSM module
    lcd_data('A');                 
    delay(1);
    GSM_write('T');            // Sending 'T' to GSM module
    lcd_data('T'); 
		delay(1);	
		GSM_write('D');            // Sending 'T' to GSM module
    lcd_data('D'); 
		delay(1);	
		for(x=0;x<10;x++)
		{
		GSM_write(number[x]);
			lcd_data(number[x]); 
			delay(1);
		}
		delay(1);	
		GSM_write(';');            // Sending 'T' to GSM module
    lcd_data(';'); 
		delay(1);		
		GSM_write(0x0d);          // Sending carriage return to GSM module  	
    delay(500);
		lcd_cmd(0x01);            //    Clear LCD screen
	  lcd_string("Calling.....");
					 }
	 TMOD=0x22; 
		TR0=1;
TH0=0x00; TL0=0x00;
}

	
void main ()
{
motor_pin_1=0;
motor_pin_2=0;
motor_pin_3=0;
motor_pin_4=0;
	IE=0x82;
TMOD=0x22; //set mode
TH1=0xFD;  //baud rate
SCON=0x50; //serial communication
TR1=1; TR0=1;
TH0=0x00; TL0=0x00;
while(1) //infinte loop
{ while(!RI);
	button=SBUF;
	RI=0;
if(button=='f') 
{
delay(10);
//P3=0x05;
motor_pin_1 = 1;
motor_pin_2 = 0;
motor_pin_3 = 0;
motor_pin_4 = 1;
}
else if(button=='b')
{
delay(10);
//P3=0x0a;
motor_pin_1 = 0;
motor_pin_2 = 1;
motor_pin_3 = 1;
motor_pin_4 = 0;
}
else if (button=='s')
{
delay(5);
motor_pin_1 = 0;
motor_pin_2 = 0;
motor_pin_3 = 0;
motor_pin_4 = 0;
}
if (button=='l')
{
delay(5);
motor_pin_1 = 0;
motor_pin_2 = 1;
motor_pin_3 = 0;
motor_pin_4 = 1;
}
if (button=='r')
{
delay(5);
motor_pin_1 = 1;
motor_pin_2 = 0;
motor_pin_3 = 1;
motor_pin_4 = 0;
}
}
}
void delay (unsigned int z) //delay function
{
unsigned int i,j;
for(i=0;i<z;i++)
for(j=0;j<1275;j++);
}
