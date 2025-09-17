#ifndef _JPEG_H_
#define _JPEG_H_

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include <ctype.h>

#include "../include/commande.h"
#include "../include/lecture.h"
/* fonction permettant d'écrire un nombre sur 2 bits dans le fichier*/
extern void ecriture_nombre(FILE * file, int * nombre);
/* fonction qui écrit l'entete du fichier outfile: la partie SOI,DQT,SOFO,DHT,et SOS sans le code brut des données de l'image*/
extern void remplissage_entete_fichier_jpeg(FILE * file, struct image_fichier image_fichier_en_lecture);
/* marqueur EOI */
extern void ecriture_fin_image(FILE * file);

#endif
