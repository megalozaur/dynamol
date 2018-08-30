#include"druk.h"

/* ------------------------------------
   drukuje stan ukladu

   uzywa:
   krok, E_p, E_k, H, T, cisnienie
--------------------------------------- */
inline int drukuj_stan(FILE *out){
	
	if( out==NULL ) return 1;

	fprintf(out, "%d\t",krok);
	fprintf(out, "%f\t",E_p);
	fprintf(out, "%f\t",E_k);
	fprintf(out, "%f\t",H);
	fprintf(out, "%f\t",T);
	fprintf(out, "%f\n",cisnienie);
	return 0;
	}

inline int drukuj_stan_opis(FILE *out){
	
	if( out==NULL ) return 1;

	fprintf(out, "krok   \tE_p     \tE_k      \tH       \tT       \tcisnienie\n");
	return 0;
	}


/* ------------------------------------
   drukuje stan usredniony

   uzywa:
   E_p_sr, E_k_sr, H_sr, T_sr, cisnienie_sr
--------------------------------------- */
inline int drukuj_stan_sr(FILE *out){
	
	if( out==NULL ) { out=stdout; }

	fprintf(out,"\n%f\t",E_p_sr);
	fprintf(out,"%f\t",E_k_sr);
	fprintf(out,"%f\t",H_sr);
	fprintf(out,"%f\t",T_sr);
	fprintf(out,"%f",cisnienie_sr);

	return 0;
	}

inline int drukuj_stan_sr_opis(FILE *out){
	fprintf(out,"<E_p>     \t<E_k>      \t<H>       \t<T>       \t<cisnienie>\n");

	return 0;
	}

/* ------------------------------------
   drukuje polozenia

   uzywa:
   r[w][i]
--------------------------------------- */
inline int drukuj_pol(FILE *out){
	int i, w;
	if( out==NULL ) { out=stdout; }
	
	/*ilosc atomow*/
	fprintf(out, "%d\n", n*n*n);

	/*komentarz*/
	fprintf(out, "AB\n");

	for(i=0;i<n*n*n;i++){
		//fprintf(out,"%s%d\t",symbol,i);
		fprintf(out,"%s\t",symbol);
		for(w=0;w<_WYMIAR;w++)
			fprintf(out,"%f\t",r[w][i]);
		fprintf(out, "\n");

	}


	return 0;
	}


/* ------------------------------------
   drukuje parametry

   uzywa:
   symbol, n, m, epsilon, R, a, L, f, 
   kT, k_termalizaji, k_usredniania,
   k_zapisu_x, k_zapisu_p
--------------------------------------- */
int drukuj_param(FILE *out){

	if( out==NULL ) { out=stdout; }

	fprintf( out, "symbol=%s\n",symbol);
	fprintf( out, "n=%d\n",n);
	fprintf( out, "m=%f\n",m);
	fprintf( out, "epsilon=%f\n",epsilon);
	fprintf( out, "R=%f\n",R);
	fprintf( out, "a=%f\n",a);
	if (L_wej == TRUE ) fprintf( out, "L=%f\n",L);
		else fprintf( out, "L_obl=%f\n",L);
	fprintf( out, "f=%f\n",f);
//	fprintf( out, "kT_0=%f\n",kT);
	fprintf( out, "T_0=%f\n",kT/k_B);
	fprintf( out, "k_termalizacji=%d\n",k_termalizacji);
	fprintf( out, "k_usredniania=%d\n",k_usredniania);
	fprintf( out, "k_zapisu_x=%d\n",k_zapisu_x);
	fprintf( out, "k_zapisu_p=%d\n",k_zapisu_p);
	fprintf( out, "dt=%f\n",dt);

	return 0;
	}


/* ------------------------------------
   zapisuje pozycje atomow do pliku
   w formacie odpowiednim dla programu
   Jmol

   uzywa:
   r[w][i], n
--------------------------------------- */
int zapisz_krysztal(char *nazwa_pliku ){
	FILE *fpol;
	int i;

	fpol=fopen(nazwa_pliku,"w");

	if(fpol==NULL) {printf("[BLAD] Nie mozna stworzyc pliku wyjsciowego\n"); return 1;}


	/*ilosc atomow*/
	fprintf(fpol, "%d\n", n*n*n);

	/*komentarz*/
	fprintf(fpol, "AB\n");
	for(i=0;i<n*n*n;i++)
		fprintf(fpol, "%s %f %f %f \n",symbol, r[0][i], r[1][i], r[2][i]);

	fprintf(fpol, "\n");
	fclose(fpol);

	return 0;
	}


/* ------------------------------------
   zapisuje pedy do pliku, w formacie
   px, py, pz
--------------------------------------- */
zapisz_pedy(char *nazwa_pliku){
	FILE *fped;
	int i;

	printf("ZAPISUJE PĘDY\n");

	fped=fopen(nazwa_pliku,"w");

	if(fped==NULL) {printf("[BLAD] Nie mozna stworzyc pliku wyjsciowego na p\n"); return 1;}

	for(i=0;i<n*n*n;i++)
		fprintf(fped, "%f %f %f\n", p[0][i], p[1][i], p[2][i]);
	fprintf(fped, "\n");
	fclose(fped);

	return 0;
	}
//A.B.
