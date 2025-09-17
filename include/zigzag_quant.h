#ifndef _ZIGZAG_QUANT_H_
#define _ZIGZAG_QUANT_H_

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include <ctype.h>

extern void zig_zag( int16_t ** matrice_mcu, int16_t tab[]);
    /* la fonction qui donne le vecteur zig zag d'une matrice  */

extern void quantification(int16_t vecteur[64], uint8_t * tab_quant);
    /* fonction qui calcul les valeurs du vecteur après le quantifier */

extern void affichage_quantification(int16_t * vecteur);
    /* fonction pour afficher le vecteur quantifié */

#endif