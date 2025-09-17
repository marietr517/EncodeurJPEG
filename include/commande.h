#ifndef _COMMANDE_H_
#define _COMMANDE_H_

#include <stdint.h>

struct image_fichier 
    {
    uint32_t largeur_apres_dup;     
    uint32_t hauteur_apres_dup;
    uint32_t hauteur;
    uint32_t largeur; 
    char * version;
    int nombre_mcu;
    uint8_t * coeff_echantillonnage;
    };
/* arguments passés sur la ligne de commande*/
struct arguments 
    {
    char * fichier_en_lecture;
    char * fichier_en_ecriture;
    char * facteurs_echantillonnage;
    };  
/* message affiché pour l'option --h ou --help */
extern void print_help();
/* fonction qui permet de traiter les paramètres optionnels sur la ligne de commande
   et retourne les arguments stockés dans struct arguments */
extern struct arguments traitement_des_options(int argc, char * argv[]);
/* Fonction permettant de vérifier la validité du nom fichier en lecture passé sur la ligne commande 
   et des facteurs d'échantillonnage*/
extern bool verification_validite(struct arguments arg);

#endif
