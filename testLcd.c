/***********************
 *  funkcje:
 *  1. HD44xxx Hitachi
 *  2. LED Martix
 *  3. SPI do led matrix
 *  4. IRQ od czasu do LED matrix
 *  5. Timer doIRQ
 *  6. GPIO joy
 *  7. MOtor
 *  8. PWM do motoru
 *  9. PRM do buzzera
 *  10. konstrukcja mechaniczna
 */

#include "../pre_emptive_os/api/general.h"
#include <lpc2xxx.h>
#include <stdio.h>
#include <string.h>
#include "lcd.c"
#include "testLcd.h"

#define KEYPIN_CENTER  0x00010000
#define KEYPIN_UP      0x00020000
#define KEYPIN_RIGHT   0x00040000
#define KEYPIN_LEFT    0x00080000
#define KEYPIN_DOWN    0x00100000

#define LCD_DATA      0x00ff0000  //P1.16-P1.23
#define LCD_E         0x02000000  //P1.25
#define LCD_RW        0x00400000  //P0.22
#define LCD_RS        0x01000000  //P1.24
#define LCD_BACKLIGHT 0x40000000  //P0.30


extern tU32 czasTimer;

tU8 buzz = 'n';

tU8 wybor = 'n'; //g - up, p - right, d - down, l - left

/*!
void ruch to funkcja w ktorej sprawdzamy w jakim polozeniu znajduje sie aktualnie joystick
ma w sobie parametr:
wybor - w ktorym przechowywana jest informacja o tym w jakim aktualnie polozeniu znajduje sie joystick

skutkiem wywolania funckji jest zapisanie do parametru w jakim polozeniu znajduje sie joystick
*/

void ruch(tU16 liczba){
	tU16 i = 0;
	while (i < liczba){
		if (((IOPIN & KEYPIN_UP) == 0)){
			wybor = 'g';
		}

		if (((IOPIN & KEYPIN_UP && IOPIN & KEYPIN_DOWN) != 0)){
				wybor = 'n';

		}

		if (((IOPIN & KEYPIN_RIGHT) == 0)){
						wybor = 'r';

		}

		if (((IOPIN & KEYPIN_LEFT) == 0)){
						wybor = 'l';

		}

		if (((IOPIN & KEYPIN_DOWN) == 0)){
			wybor = 'd';

		}

		if (((IOPIN & KEYPIN_CENTER) == 0)){
			wybor = 's';
		}

		//osSleep(10); // bylo 10
		i++;
	}
}

/*!
void startMenu jest glowna petla na ktorej oparty jest program to w niej podejmowane sa decyzje w ktora strone
bedzie obracal sie silnik, z jaka predkoscia oraz kiedy bedzie wlaczaby buzzer
ma w sobie parametry:
wybor - na jej podstawie okreslane jest w ktora strone bedzie krecil sie silnik PWM, czy buzzer bedzie wydawal dzwiek,
kierunek strzalki na Matrix'e oraz czy predkosc ulegnie zmianie
speed - ktory przyjmuje aktualna wartosc jednoski predkosci silnika PWM

mo퓄iwymi skutkami wywolania funckji jest: zmiana predkosci silnika PWM, wlaczenie/wylaczenie wilnika, wlaczanie/wylaczenie
buzzera, ciagi znakow wyswietlane na wyswietlaczu LCD
*/

void startMenu(){



tU16 speed=0;
initializemotor();
enablemotor();
rysujMenu(speed);



	while(wybor != 's'){

		ruch(1);

		if(wybor=='r'){

			speed=motorfaster(speed);
			rysujpredkosc();
			rysujMenu(speed);
		}

		if(wybor=='l'){
			if(speed!=0){
			speed=motorslower(speed);
			}
			rysujpredkosc();
			rysujMenu(speed);
			osSleep(10);
				}

		while(wybor =='n'){

			matrixStrzalka(wybor);
			ruch(1);

		}

		while(wybor =='g'){

			matrixStrzalka(wybor);
			enablemotorright(speed);
			buzzing();

			ruch(1);

				}



		while(wybor =='d'){
			matrixStrzalka(wybor);
			//enablemotorright(speed);
			enablemotorleft(speed);
			buzzing();
			ruch(1);

			}


		disablemotor();
		disablebuzz();
	}

	if (wybor == 's'){
		//rgb('c');
		matrixStrzalka(wybor);
		rysujAwaria();
		disablemotor();
		disablebuzz();
		osSleep(1000000000);


	}


}

/*!
void rysujMenu informuje u퓓tkownika o nazwie projektu oraz aktualnej predkosci silnika PWM na wyswietlaczu LCD,
wywoluje sie przez nacisniecie joysticka
ma w sobie parametry:
wiadomosc - beda to znaki wypisywane w pierwszym wierszu wyswietlacza LCD
wiadomosc2 - beda to znaki wypisywane w drugi wierszu wswietlacza LCD

skutkiem wywolania funckji jest wypisanie dwoch wierszy znakow na wyswietlaczu LCD
informujacym o nazwie projektu oraz aktualnej predkosci silnika PWM
*/

void rysujMenu(int speed){

	int jazda = speed/10;
	tU8 wiadomosc[] = {"Projekt Dzwig"};
	tU8 wiadomosc2[16];
	snprintf((char*)wiadomosc2, sizeof(wiadomosc2), "Predkosc %d", jazda);
	initLCD(); //inicjalizacja LCD
	lcdBacklight(TRUE); //podswietlenie
	clearLCD(); //czyszczenie ekranu
	printStringLCD(wiadomosc); //wypisz na ekranie chara
	secondRowLCD(); //przejscie do drugiej linii
	printStringLCD(wiadomosc2);

}

/*!
void rysujAwaria informuje u퓓tkownika o awaryjnym zatrzymaniu przez uzytkownika na wyswietlaczu LCD,
wywoluje sie przez nacisniecie joysticka
ma w sobie parametry:
wiadomosc - beda to znaki wypisywane w pierwszym wierszu wyswietlacza LCD
wiadomosc2 - beda to znaki wypisywane w drugi wierszu wswietlacza LCD

skutkiem wywolania funckji jest wypisanie dwoch wierszy znakow na wyswietlaczu LCD
informujacym o awaryjnym zatrzymaniu programu
*/

void rysujAwaria(){

	tU8 wiadomosc[] = {"Awaryjne zatrzym"};
	tU8 wiadomosc2[] = {"Nacisnij reset"};
	initLCD(); //inicjalizacja LCD
	lcdBacklight(TRUE); //podswietlenie
	clearLCD(); //czyszczenie ekranu
	printStringLCD(wiadomosc); //wypisz na ekranie chara
	secondRowLCD(); //przejscie do drugiej linii
	printStringLCD(wiadomosc2);

}

/*!
void rysujpredkosc informuje u퓓tkownika o zmianie predkosci o 20 jednoste na wyswietlaczu LCD,
wywoluje sie przez skierowanie joysticka w lewa lub prawa strone
ma w sobie parametry:
wiadomosc - beda to znaki wypisywane w pierwszym wierszu wyswietlacza LCD
wiadomosc2 - beda to znaki wypisywane w drugi wierszu wswietlacza LCD

skutkiem wywolania funckji jest wypisanie dwoch wierszy znakow na wyswietlaczu LCD
informujacym o zmianie prednosci pracy silnika PVM
*/

void rysujpredkosc(){
	tU8 wiadomosc[] = {"Zmiana predkosci"};
		tU8 wiadomosc2[] = {"0 20 jednostek"};
		initLCD(); //inicjalizacja LCD
		lcdBacklight(TRUE); //podswietlenie
		clearLCD(); //czyszczenie ekranu
		printStringLCD(wiadomosc); //wypisz na ekranie chara
		secondRowLCD(); //przejscie do drugiej linii
		printStringLCD(wiadomosc2);
		osSleep(60);

}


