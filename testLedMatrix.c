#include "../pre_emptive_os/api/osapi.h"
#include "../pre_emptive_os/api/general.h"
#include <printf_P.h>
#include <lpc2xxx.h>
#include <consol.h>
#include <config.h>
#include "testLedMatrix.h"

#define CRYSTAL_FREQUENCY FOSC
#define PLL_FACTOR        PLL_MUL
#define VPBDIV_FACTOR     PBSD
#define  SPI_CS   0x00008000  //<= new board, old board = 0x00800000


const tU8 eaText[] = {
	0x18,0x14,0xF2,0x81,0x81,0xF2,0x14,0x18, // W prawo ------ u nas w  góre
	0x18,0x28,0x4F,0x81,0x81,0x4F,0x28,0x18, // W lewo ------- u nas w dół
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,//Czyszczenie
	0x3C,0x5A,0x99,0x99,0x99,0x99,0x5A,0x3C //stop

};

tU8 pattern[8] = {0,0,0,0,0,0,0,0};

/*!
void startTimer1 przy pomocy tej funkcji ustawiamy TIMER1

skutkiem wywolania funckji jest inicjalizacja i włączenie TIMER1 z odswiezaniem co 1ms
*/

void startTimer1(){
  //initialize VIC for Timer1 interrupts
  VICIntSelect &= ~0x20;           //Timer1 interrupt is assigned to IRQ (not FIQ)
  VICVectAddr5  = (tU32)ledMatrix; //register ISR address
  VICVectCntl5  = 0x25;            //enable vector interrupt for timer1
  VICIntEnable  = 0x20;            //enable timer1 interrupt

  //initialize and start Timer #0
  T1TCR = 0x00000002;                           //disable and reset Timer1
  T1PC  = 0x00000000;                           //no prescale of clock
  T1MR0 = ((CRYSTAL_FREQUENCY * PLL_FACTOR) / (1000 * VPBDIV_FACTOR));
  T1IR  = 0x000000ff;                           //reset all flags before enable IRQs
  T1MCR = 0x00000003;                           //reset counter and generate IRQ on MR0 match
  T1TCR = 0x00000001;                           //start Timer1
}

/*!
void matrixStrzalka funkcja odpowiedzialna jest za zayswietlenie na LedMatrix strzalki w odpowiednim kierunku poruszania sie dzwigu
ma w sobie parametry:
kierunek - ktora przechowyje informacje którą wartość cntA przyjąć
eaText - przechowuje 8 liczb jednobajtowych ktore beda przypozadkowywane do kolejnych wierszy matrycy

skutkiem wywolania funckji jest wyswietlenie na LED Matrix strzalki w odpowiednim kierunku lub znaku stop (wygladajacy jak zakaz wjazdu)
*/

void matrixStrzalka(tU8 kierunek)
{
 	tU8 cntA = 0;

  PINSEL0 |= 0x00001500 ;  //Initiering av SPI
  SPI_SPCCR = 0x08;
  SPI_SPCR  = 0x60;
  IODIR0 |= SPI_CS;


  if (kierunek == 'g'){
	  cntA = 0;
  }else if (kierunek == 'd'){
	  cntA = 8;
  }else if (kierunek == 'n'){
	  cntA = 16;
  }else if (kierunek == 's'){
	  cntA = 24;
  }

	pattern[0] = eaText[cntA+0];
	pattern[1] = eaText[cntA+1];
	pattern[2] = eaText[cntA+2];
	pattern[3] = eaText[cntA+3];
	pattern[4] = eaText[cntA+4];
	pattern[5] = eaText[cntA+5];
	pattern[6] = eaText[cntA+6];
	pattern[7] = eaText[cntA+7];


}
