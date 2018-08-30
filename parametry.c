#include "parametry.h"
/* ------------------------------------
   wczytuje parametry początkowe symulacji

   uzywa:
   symbol, n, m, epsilon, R, a, L, f, 
   kT, k_termalizaji, k_usredniania,
   k_zapisu_x, k_zapisu_p
--------------------------------------- */

int wczytaj_param(char* nazwa_pliku){
	FILE *fparam;
	char linia[80];
	
	printf("PARAMETRY: ");

	fparam=fopen(nazwa_pliku,"r");

	if(fparam==NULL) {printf("[BLAD] Brak pliku z parametrami\n"); return 1;}

	while(  fscanf(fparam,"%s",linia)!=-1  ){
		/*printf("%s\n",linia);*/
		/* Argumenty ----------------------------------------------------- */

		if ( strncmp(linia,"symbol=",7) == 0  ) {
			strcpy(symbol, 1+strchr(linia,'='));
			//printf("symbol=%s\n",symbol);
			printf("symbol");
			}
		else if ( strncmp(linia,"n=",2) == 0  ) { 
			n=atoi(strchr(linia,'=')+1);
			//printf("n=%d\n",n);
			printf("n");
			}
		else if ( strncmp(linia,"m=",2) == 0  ) {
			m=atof(strchr(linia,'=')+1);
			//printf("m=%f\n",m);
			printf("m");
			}
		else if ( strncmp(linia,"epsilon=",8) == 0  ) {
			epsilon=atof(strchr(linia,'=')+1);
			//printf("epsilon=%f\n",epsilon);
			printf("epsilon");
			}
		else if ( strncmp(linia,"R=",2) == 0  ) {
			R=atof(strchr(linia,'=')+1);
			//printf("R=%f\n",R);
			printf("R");
			}
		else if ( strncmp(linia,"a=",2) == 0  ) {
			a=atof(strchr(linia,'=')+1);
			//printf("a=%f\n",a);
			printf("a");
			}
		else if ( strncmp(linia,"L=",2) == 0  ) {
			L=atof(strchr(linia,'=')+1);
			//printf("L=%f\n",L);
			printf("L");
			L_wej=TRUE;
			}
		else if ( strncmp(linia,"f=",2) == 0  ) {
			f=atof(strchr(linia,'=')+1);
			//printf("f=%f\n",f);
			printf("f");
			}
		else if ( strncmp(linia,"T_0=",3) == 0  ) {
			kT=k_B * atof(strchr(linia,'=')+1);
			//printf("kT=%f\n",kT);
			printf("T_0");
			}
		else if ( strncmp(linia,"k_termalizacji=",15) == 0  ) {
			k_termalizacji=atof(strchr(linia,'=')+1);
			//printf("k_termalizacji=%d\n",k_termalizacji);
			printf("k_termalizacji");
			}
		else if ( strncmp(linia,"k_usredniania=",14) == 0  ) {
			k_usredniania=atof(strchr(linia,'=')+1);
			//printf("k_usredniania=%d\n",k_usredniania);
			printf("k_usredniania, ");
			}
		else if ( strncmp(linia,"k_zapisu_x=",10) == 0  ) {
			k_zapisu_x=atof(strchr(linia,'=')+1);
			//printf("k_zapisu_x=%d",k_zapisu_x);
			printf("k_zapisu_x");
			}
		else if ( strncmp(linia,"k_zapisu_p=",10) == 0  ) {
			k_zapisu_p=atof(strchr(linia,'=')+1);
			//printf("k_zapisu_p=%d",k_zapisu_p);
			printf("k_zapisu_p");
			}
		else if ( strncmp(linia,"dt=",3) == 0  ) {
			dt=atof(strchr(linia,'=')+1);
			//printf("dt=%f\n", dt);
			printf("dt");
			}
		else { printf("Nierozpoznana zmienna '%s' ",linia); }
		
		printf(", ");
		}
	fclose(fparam);
	printf("|\n");

	if(L==0) {
		L = 1.2 * 0.5 * sqty(6) * a * ( n-1 );
		printf("Obliczono: L = %f\n",L);
		}

	return 0;
	}

