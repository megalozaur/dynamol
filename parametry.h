#ifndef DRUK_H
#define DRUK_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define _WYMIAR 3
#define k_B 8.31e-3

#define TRUE 1
#define FALSE 0

/*       TABLICE:
ALOKOWANE W MAIN */

/* polożenie w-ta wspórzędna i-tej => r[w][i] */
float *r[_WYMIAR];
/* pędy => p[w][i] */
float *p[_WYMIAR];
/* energie => E[w][i] */
float *E[_WYMIAR];
/* siły */
float *F[_WYMIAR];
/* potencjał i-tej cząstki = V[i] */
float *V;
/* ciśnienie na ścianki */
float cisnienie, cisnienie_sr;
/* energia */
float H, H_sr, E_k, E_k_sr, E_p, E_p_sr;
/* temperatura */
float T, T_sr;

/*     ZMIENNE:
TRZEBA PODAĆ WARTOŚCI
DOMYŚLNE, NA WYPADEK BRAKU
WPISU W PLIKU */

/* --- parametry modelu --- */
/* symbol pierwiastka */
char symbol[4]="Ar";

/* ilość atomów w rzędzie
ilość atomów = n^3 */
int n=4;
/* masa */
float m=1;
/* epsilon oddziaływania między atomami */
float epsilon=1;
/* parametr równania Van der Wallsa */
float R=0.38;
/* stała sieci */
float a=0.38;
/* L */
float L=0;
/* f stała potencjału dpychania od ścianek */
float f=1e4;
/* kT - wczytywania temperatura */
float kT=k_B *1000;

/* --- parametry symulacji --- */	

/* ilość kroków termalizacji */
int k_termalizacji=100;
/* ilość kroków/ uśredniania */
int k_usredniania=2000;
/* krok, w którym zostaną zapisane położenia */
int k_zapisu_x=10;
/* -"- pędy */
int k_zapisu_p=10;

/* "czas" */
int krok;

/* krok czasowy */
float dt=2e-3;

/* czy wczytano parametr L z pliku */
char L_wej = FALSE;

int wczytaj_param(char *nazwa_pliku);

#endif

