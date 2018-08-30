#ifndef ZMEINNE_GLOBALNE_H
#define ZMIENNE_GLOBALNE_H
#define _WYMIAR 3
#define k_B 8.31e-3

#define TRUE 1
#define FALSE 0

/*       TABLICE: ALOKOWANE W MAIN */

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
extern char symbol[];

/* ilość atomów w rzędzie
ilość atomów = n^3 */
extern int n;
/* masa */
extern float m;
/* epsilon oddziaływania między atomami */
extern float epsilon;
/* parametr równania Van der Wallsa */
extern float R;
/* stała sieci */
extern float a;
/* L */
extern float L;
/* f stała potencjału dpychania od ścianek*/
extern float f;
/* kT - wczytywania temperatura */
extern float kT;

/* --- parametry symulacji --- */	

/* ilość kroków termalizacji */
extern int k_termalizacji;
/* ilość kroków/ uśredniania */
extern int k_usredniania;
/* krok, w którym zostaną zapisane położenia */
extern int k_zapisu_x;
/* -"- pędy */
extern int k_zapisu_p;

/* "czas" */
extern int krok;

/* krok czasowy */
extern float dt;

/* czy wczytano parametr L z pliku */
char L_wej;
#endif

