 //UART0_CONFIGURATION_1
#define THRE 5
#define RDR  0
//UART0_DECLARATION_2
void UART_TX(unsigned char);
unsigned char UART_RX(void);

void UART0_CONFIG(void);

void UART_RXSTRING(char*);
void UART_TXSTRING(char*);


  
//uart0
void UART_RXSTRING(char* p)
{
  unsigned int i;
  for(i=0;i<12;++i)
  p[i]=UART_RX();
}

void UART_TXSTRING(char* p)
{
 unsigned int i;
  for(i=0;i<12;++i)
   UART_TX(p[i]);
} 

void UART0_CONFIG()
{
 PINSEL0 |= 0X00000005;
 U0LCR   = 0X83;
 U0DLL   = 97;
 U0LCR   = 0X03;
}

unsigned char UART_RX()
{
 while(((U0LSR>>RDR)&1)==0);
 return U0RBR;
}

void UART_TX(unsigned char rx_byte)
{
 U0THR=rx_byte;
 while(((U0LSR>>THRE)&1)==0);
}
