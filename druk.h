#ifndef DRUK_H
#define DRUK_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include"parametry_extern.h"

inline int drukuj_stan(FILE *out);
inline int drukuj_stan_opis(FILE *out);
inline int drukuj_stan_sr(FILE *out);
inline int drukuj_pol(FILE *out);
int drukuj_param(FILE *out);
int zapisz_krysztal(char *nazwa_pliku );
int zapisz_pedy(char *nazwa_pliku);

#endif

