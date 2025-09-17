#ifndef _LECTURE_H_
#define _LECTURE_H_

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include <ctype.h>

#include "../include/commande.h"

struct mcu  
    {
    int16_t ** matrice_mcu_Y;
    int16_t ** matrice_mcu_CB;
    int16_t ** matrice_mcu_CR;
    int hauteur_mcu_Y;
    int largeur_mcu_Y;
    int hauteur_mcu_CB;
    int largeur_mcu_CB;
    int hauteur_mcu_CR;
    int largeur_mcu_CR;
    };

extern int extension(int dimension, uint8_t valeur);
    /* fonction qui calcule les nouveaux dimensions de l'image après duplication */

extern void lecture_fichier(FILE * file, struct image_fichier * image_fichier);
    /* fonction qui traite l'image en question en tirant les caractéristiques 
    comme la version (gris ou couleurs), largeur et hauteur, etc ... */

extern void test_remplissage_image(struct image_fichier image_fichier);
    /* tester que les caractéristiques sont justes */

extern bool verif_echantillonnage(struct image_fichier image_fichier);
    /*fonction qui verifie si on va travailler sur des blocs 8*8*/

extern void remplissage_mcu_grise(struct mcu * mcu, struct image_fichier image_fichier, long position, FILE * file_in, int i, int j);
    /* fonction qui lit un mcu d'une image grise */

extern void affichage_mcu_grise(struct mcu * mcu);
    /* afficher les mcus des images grises */

extern void affichage_mcu_couleur(struct mcu * mcu);
    /* affichage les mcus des images couleurs */

extern void initialisation(int32_t * vecteur, int longueur);
    /* initialiser les matrices des mcus à 0 */

extern void moyenne(int32_t ** matrice, int hauteur, int largeur, int facteur);
    /* fonction utilisée par le remplissage des images couleurs. 
    Elle fait la moyenne de chaque case */

extern int corrige_valeur(int valeur);
    /* fonction pour ajuster les valeurs et éviter leurs débordements */

extern void remplissage_mcu_couleur(struct mcu * mcu, struct image_fichier image_fichier, long position, FILE * file_in, int i, int j);
    /* fonction qui lit un mcu d'une image en couleurs */

extern void free_mcu(struct mcu * mcu);
    /* fonction pour libérer l'espace mémoire réservé à un mcu */

#endif
