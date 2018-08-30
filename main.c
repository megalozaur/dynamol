#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "druk.h"
#include "parametry.h"
#include "nowy_krysztal.h"

FILE *fpol;
FILE *fstan;

/* FUNKCJE:
UZYWAJA ZMIENNYCH GLOBALNYCH */

/* ------------------------------------
   oblicza odległość między atomami 

   uzywa:
   r[w][i]
--------------------------------------- */
inline float cR(int i, int j){
	float R=0;
	int w;
	for(w=0;w<3;w++)
		R+=(r[w][i]-r[w][j])*(r[w][i]-r[w][j]);
	R=sqrt(R);						
	return R;
	}	

/* ------------------------------------
   oblicza odległość i-tego atomu od 
   środka kryształu.

   używa:
   r[w][i]
--------------------------------------- */
inline float cR_0(int i){
	int w;
	float R=0;
	for(w=0;w<3;w++)					\
		R+=(r[w][i])*(r[w][i]);
	R=sqrt(R);
	return R;
	}

/* ------------------------------------
   oblicza potęgi x^k.
--------------------------------------- */
inline float pot(float x, int k){
	float wynik=1;
	for(;k>0;k--)
		wynik*=x;
	return wynik;
}


/* ------------------------------------
   oblicza siły

   uzywa:
   F[w][i], p[w][i], r[w][i]
--------------------------------------- */

inline int oblicz_sily(){
	float R_0;
	float RnaR12_ij, RnaR6_ij;
	float RnaR_ij;
	float R_ij;
	float F_i_w;
	float mod_F_i;
	int i, j, w;

	/* zerowanie */
	for(w=0;w<_WYMIAR;w++)
		for(i=0;i<n*n*n;i++)
			F[w][i]=0;

	/* obliczenie sił między cząstekami */ 
	for(i=0; i<n*n*n; i++)
		for(j=i+1; j<n*n*n; j++){
			// zmienne pomocnicze
			R_ij=cR(i,j);	
			RnaR_ij=R/R_ij;
			
			RnaR12_ij = pot(RnaR_ij, 12);
			RnaR6_ij  = pot(RnaR_ij, 6);

			// moduł siły
			mod_F_i= 12 * epsilon * ( RnaR12_ij  - RnaR6_ij) / (R_ij*R_ij);

			for(w=0; w<_WYMIAR; w++){
				F_i_w=( r[w][i] - r[w][j] ) * mod_F_i;
				
				F[w][i]+=F_i_w;
				F[w][j]-=F_i_w;
				}
			}


	/* sił od bariery potencjału*/
	for(i=0;i<n*n*n;i++){
		R_0=cR_0(i);
		if ( R_0 > L ) {
			mod_F_i= f * ( R_0 - L);
			for(w=0;w<_WYMIAR;w++)
				F[w][i] += - mod_F_i * r[w][i]/R_0;
			}
		}
	return 0;
	}



/* ------------------------------------
   oblicza potencjal E_p, siły itd. 

   uzywa:
   F[w][i], p[w][i], r[w][i], E_p, E_k,
   T, H, cisnienie
--------------------------------------- */
inline int oblicz_stan_i_sily(){
	float R_0;
	float RnaR12_ij, RnaR6_ij;
	float RnaR_ij;
	float R_ij;
	float F_i_w;
	float mod_F_i;

	int i, j, w;

	for(w=0;w<_WYMIAR;w++)
		for(i=0;i<n*n*n;i++)
			F[w][i]=0;

	E_p=0;
	/* obliczenie potencjału od cząstek i sił między cząstekami */
	for(i=0; i<n*n*n; i++)
		for(j=i+1; j<n*n*n; j++){
			// zmienne pomocnicze
			R_ij=cR(i,j);	
			RnaR_ij=R/R_ij;
			RnaR12_ij = pot(RnaR_ij, 12);
			RnaR6_ij  = pot(RnaR_ij, 6);

			// potencjał
			E_p+=epsilon * ( RnaR12_ij - 2 * RnaR6_ij );

			// siły
			// F_i[w]=12 * epsilon * ( RnaR12_ij - 2 * RnaR6_ij ) ( r[w][i] - r[w][j] ) / (R_ij*R_ij);

			// moduł siły
			mod_F_i= 12 * epsilon * ( RnaR12_ij  - RnaR6_ij) / (R_ij*R_ij);

			for(w=0; w<_WYMIAR; w++){
				F_i_w=( r[w][i] - r[w][j] ) * mod_F_i;
				
				F[w][i]+=F_i_w;
				F[w][j]-=F_i_w;
				}
			}
	
	/* obliczenie potencjału i sił od bariery potencjału i cisnienia */
	cisnienie=0;
	for(i=0;i<n*n*n;i++){
		R_0=cR_0(i);
		if ( R_0 > L ) {
			E_p += f/2. * (R_0-L) * (R_0-L);

			mod_F_i= f * ( R_0 - L);
			for(w=0;w<_WYMIAR;w++)
				F[w][i] += - mod_F_i * r[w][i]/R_0;
			cisnienie+=mod_F_i;
			}
		}

	/* obliczenie energii kinetycznej*/
	E_k=0;
	for(i=0;i<n*n*n;i++)
		for(w=0;w<_WYMIAR;w++)
			E_k+=p[w][i]*p[w][i];
	E_k/=2*m;
	T=2./3./ k_B  /(n*n*n) * E_k;
	H= E_p + E_k;

	return 0;
	}


/* ------------------------------------
   oblicza stan w następnej chwili z zapisem

   uzywa:
   F[w][i], r[w][i], p[w][i]
   ------------------------------------ */

inline int Sdt_zapis(){
	int i, w;
		//18a, 18b//
		for(w=0;w<_WYMIAR;w++)
			for(i=0;i<n*n*n;i++){
				// p(t+0.5dt) = p(t) + 0.5 F(t) dt | p(t) ---> p(t+0.5dt)
				p[w][i]=p[w][i]+ 0.5 * F[w][i] * dt;
				// r(t+dt) = r(t) + 1/m p(t+0.5dt)dt
				r[w][i]=r[w][i] + p[w][i]/m * dt;
				}

		// F(t) ---> F(t+dt)
		if ( krok%k_zapisu_p == 0 ) { oblicz_stan_i_sily(); drukuj_stan(fstan); }
			else oblicz_sily();  
		if ( krok%k_zapisu_x == 0 && fpol!=NULL) {drukuj_pol(fpol);}
		
		//18c//
		for(w=0;w<_WYMIAR;w++)
			for(i=0;i<n*n*n;i++){
				//p(t+dt)=p(t+0.5dt) + 0.5 F(t+dt) dt
				p[w][i]=p[w][i]+ 0.5 * F[w][i] * dt;
				}

		}



/* ------------------------------------
   oblicza stan w następnej chwili

   uzywa:
   F[w][i], r[w][i], p[w][i]
   ------------------------------------ */

inline int Sdt(){
	int i, w;
		//18a, 18b//
		for(w=0;w<_WYMIAR;w++)
			for(i=0;i<n*n*n;i++){
				// p(t+0.5dt) = p(t) + 0.5 F(t) dt | p(t) ---> p(t+0.5dt)
				p[w][i]=p[w][i]+ 0.5 * F[w][i] * dt;
				// r(t+dt) = r(t) + 1/m p(t+0.5dt)dt
				r[w][i]=r[w][i] + p[w][i]/m * dt;
				}

		// F(t) ---> F(t+dt)
		oblicz_sily();  
		
		//18c//
		for(w=0;w<_WYMIAR;w++)
			for(i=0;i<n*n*n;i++){
				//p(t+dt)=p(t+0.5dt) + 0.5 F(t+dt) dt
				p[w][i]=p[w][i]+ 0.5 * F[w][i] * dt;
				}

		}
int main(int argc, char *argv[]) {
	int  w, i;
	int l;
	char *plik_pedy=NULL;
	char *plik_param=NULL;
	char *plik_polozenia=NULL;
	char *plik_jmol=NULL;
	char *plik_stan=NULL;
	char *plik_sr=NULL;

	FILE *fsr=NULL;
	FILE *fparam=NULL;

	/* WCZYTYWANIE PARAMETROW WEJSCIOWYCH */
	for (i=1; i<argc; i++){
		if ( strncmp( argv[i], "-par", 4 ) == 0 ){
			printf("-par ");
			if ( ++i < argc ) {printf("'%s'\n",argv[i]); wczytaj_param( argv[i] ); plik_param=argv[i]; }
			}
		else if ( strncmp( argv[i], "-ped", 4 ) == 0 ){
			printf("-ped ");
			if ( ++i < argc ) {printf("'%s'\n",argv[i]); plik_pedy = argv[i] ;}
			}
		else if ( strncmp( argv[i], "-pol", 4 ) == 0 ){
			printf("-pol ");
			if ( ++i < argc ) {printf("'%s'\n",argv[i]); plik_polozenia = argv[i] ;}
			}
		else if ( strncmp( argv[i], "-stan", 4 ) == 0 ){
			printf("-stan ");
			if ( ++i < argc ) {printf("'%s'\n",argv[i]); plik_stan = argv[i] ;}
			}
		else if ( strncmp( argv[i], "-sr", 3 ) == 0 ){
			printf("-sr ");
			if ( ++i < argc ) {printf("'%s'\n",argv[i]); plik_sr = argv[i] ;}
			}
		else if ( strncmp( argv[i], "-Jmol", 5 ) == 0 ){
			printf("-Jmol ");
			if ( ++i < argc ) {printf("'%s'\n",argv[i]); plik_jmol = argv[i] ;}
			}
		else {printf("Nie wiem co to '%s'\n", argv[i]); return 0;}	
		}



	/* polozenia */
	if ( plik_polozenia != NULL ){
		fpol=fopen(plik_polozenia, "w");
		}
	

	/* stan */
	if ( plik_stan != NULL )
		fstan=fopen(plik_stan, "w");

	/* parametry */
	if ( plik_param != NULL ){
		fparam=fopen(plik_param, "w");
		drukuj_param(fparam);
		fclose(fparam);
		}
		
		drukuj_param(stdout);
		printf ("----------------------\n");
		

	/* ALOKACJA TABLICY POLOZEN I PEDOW*/
	for(w=0; w<_WYMIAR; w++){
		r[w] = (float*) malloc( sizeof(float) * n*n*n  );
		p[w] = (float*) malloc( sizeof(float) * n*n*n  );
		E[w] = (float*) malloc( sizeof(float) * n*n*n  );
		F[w] = (float*) malloc( sizeof(float) * n*n*n  );
		}
		
	/* STWORZENIE KRYSZTALU */
	stworz_krysztal();


	/* PRZYPISZ POCZĄTKOWE PĘDY I ENERGIE */
	ozyw_krysztal();

	/* ZAPIS PEDOW DO PLIKU */
	if( plik_pedy != NULL )
		zapisz_pedy(plik_pedy);

	drukuj_stan_opis(fstan);
	oblicz_stan_i_sily();
	drukuj_stan(fstan);
	
	/* TERMALIZACJA */
	if(k_termalizacji !=0) {
		fprintf(stderr,"TERMALIZACJA:\n");
		fprintf(stderr, "|");
		for(l=0;l < k_termalizacji/(int)ceil(k_termalizacji/80.)-2 ; l++ ) fprintf(stderr, "-");
		fprintf(stderr, "|\n");
		for( krok=0; krok < k_termalizacji; krok++ ){
			if( krok%(int)ceil(k_termalizacji/80.) ==0 ) putc('*',stderr);
			Sdt();
			//oblicz_stan_i_sily();
			}

		fprintf(stderr,"\n");
		}
	
	/* UŚREDNIANIE */
	if(k_usredniania !=0) {
		E_k_sr=0; E_p_sr=0; H_sr=0; T_sr; cisnienie_sr=0;
		fprintf(stderr,"USREDNIANIE:\n");
		fprintf(stderr, "|");
		for(l=0;l < k_usredniania/(int)ceil(k_usredniania/80.)-2 ; l++ ) fprintf(stderr, "-");
		fprintf(stderr, "|\n");
		for(; krok < k_termalizacji+k_usredniania; krok++ ){
			if( (krok-k_termalizacji)%(int)ceil(k_usredniania/80.) ==0 ) putc('*', stderr);
			E_k_sr+=E_k;
			E_p_sr+=E_p;
			T_sr+=T;
			cisnienie_sr+=cisnienie;

			Sdt_zapis();
	//		Sdt();
	//		if ( krok%k_zapisu_x == 0 && fpol!=NULL) {drukuj_pol(fpol);}
	//		if ( krok%k_zapisu_p == 0 ) {oblicz_stan_i_sily(); drukuj_stan(fstan);	}
			}
	
		fprintf(stderr,"\n");
	}

	/* WARTOSCI USREDNIONE */
	E_k_sr/=k_usredniania;
	E_p_sr/=k_usredniania;
	T_sr/=k_usredniania;
	cisnienie_sr/=k_usredniania;;
	H_sr=E_k_sr+E_p_sr;	

	if ( plik_sr != NULL ){
		fsr=fopen(plik_sr, "r");
		if (fsr!=NULL) {
			fclose(fsr);
			fsr=fopen(plik_sr,"a");
			}
		else {
			fsr=fopen(plik_sr,"w");
			drukuj_stan_sr_opis(fsr);
			}
		drukuj_stan_sr(fsr);
		}
	

		drukuj_stan_sr_opis(stdout);
		drukuj_stan_sr(stdout);
		printf("\n");
		

	/* ZAMYKANIE PLIKOW */
	if (fpol != NULL ) fclose(fpol);
	if (fstan != NULL ) fclose(fstan);
	if (fsr != stdout ) fclose(fsr);

	/* ZWOLNIENIE PAMIĘCI */
	for(w=0; w<_WYMIAR; w++){
		free(r[w]);
		free(p[w]);
		free(E[w]);
		free(F[w]);
		}

	return 0 ;
	}

