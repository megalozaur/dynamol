#include "nowy_krysztal.h"

/* ------------------------------------
   wypełnia tablicę położeń 
   - tworzy kryształ

   uzywa:
   r[w][i], n
--------------------------------------- */
int stworz_krysztal(){
	int i, j, k;
	float x, y, z;
	int nx, ny, nz;

	for(i=0;i<n; i++){
		nx=n*n*i;
		for(j=0; j<n; j++){
			ny=n*j;
			for(k=0; k<n; k++){
				x=a*(i-n/2)	+	a*(j-n/2)/2.0		+	a*(k-n/2)/2.0;
				y=			a*(j-n/2)*1.732/2.0	+	a*(k-n/2)*1.732/6.0;
				z=							a*(k-n/2)*1.414/1.732;
		/*		x=a*(i-n/2);
				y=a*(j-n/2);
				z=a*(k-n/2);*/


				/*printf("%d. [%f, %f, %f]\n",i*n*n+j*n+k,x,y,z);*/
				r[0][k+nx+ny]=x;
				r[1][k+nx+ny]=y;
				r[2][k+nx+ny]=z;
				}
			}
		}
	
	return 0;
	}


/* ------------------------------------
   losuje energie z rozkładu Maxwella
   i oblicza pedy, tak że <p>=0

   uzywa: E[w][i], p[w][i], r[w][i],
          n, kT
--------------------------------------- */

int ozyw_krysztal(){
	int i, w;
	float u;

	float P_sr;		//sredni ped na ilosc atomow
	float E_sr;
	/* LOSOWANIE ENERGII */
	/*	E = p**2/2m = 1/2kT ln U
		U nalezy (0,1]:
	*/
	srand(time(NULL));

	for(w=0;w<_WYMIAR;w++)
		for(i=0;i<n*n*n;i++){
			u=(float) ( RAND_MAX-rand() ) / RAND_MAX;
			E[w][i]=-kT/2.*log(u);
			}

	/* RENORMALIZACJA ENERGII */
	for(w=0;w<_WYMIAR;w++){
		/* obliczenie sredniej */
		E_sr=0;
		for(i=0;i<n*n*n;i++)
			E_sr+=E[w][i];
		E_sr/=n*n*n;
		
		//printf("E_sr_%d=%f",w,E_sr);

		/* renormalizacja */
		if(E_sr!=0)
			for(i=0;i<n*n*n;i++)
				E[w][i]*=kT/2./E_sr;
		
		/* spr. */
		E_sr=0;
		for(i=0;i<n*n*n;i++)
			E_sr+=E[w][i];
		E_sr/=n*n*n;
		//printf(" ---> %f\n",E_sr);
		}
		

	/* OBLICZENIE PEDOW */
	printf("-------------------------\n");
	for(w=0;w<_WYMIAR;w++)
		for(i=0;i<n*n*n;i++){
			if(rand()>RAND_MAX/2)
				p[w][i]=sqrt(E[w][i] * 2 * m);
				else
				p[w][i]=-sqrt(E[w][i] * 2 * m);
		}

	/* USREDNIENIE PEDOW */
	for(w=0;w<_WYMIAR;w++){
		/* obliczenie sredniej */
		P_sr=0;
		for(i=0;i<n*n*n;i++)
			P_sr+=p[w][i];
		P_sr/=n*n*n;
		//printf("P_sr_%d=%f",w,P_sr);

		/* odjecie sredniej od kazdego pedu*/
		for(i=0;i<n*n*n;i++)
			p[w][i]-=P_sr;

		/* spr. */
		P_sr=0;
		for(i=0;i<n*n*n;i++)
			P_sr+=p[w][i];
		P_sr/=n*n*n;
		//printf(" --> %f\n",P_sr);
		}
	return 0;
	}

