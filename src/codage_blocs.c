#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include <ctype.h>

#include <../include/codage_blocs.h>
#include <../include/dct.h>
#include <../include/zigzag_quant.h>
#include <../include/qtables.h>
#include <../include/codage_ac_dc.h>

/* permet de créer un tableau 8*8 pour l'encoder*/
void creation_bloc(struct bloc * bloc)
    {    
    bloc->matrice_bloc = malloc(8 * sizeof(int16_t *));
    for (int k = 0; k < 8; k++)
        {
        bloc->matrice_bloc[k] = malloc(8 * sizeof(int16_t));
        }
    }

void free_bloc(struct bloc * bloc)
    {
    for (int k = 0; k < 8; k++)
        {
        free(bloc->matrice_bloc[k]);
        }
    free(bloc->matrice_bloc);    
    }
/* applique sur un bloc 8*8 de la matrice _Y la dct ,la fonction zig_zag, la quantification, et apres on l'encode dans le fichier outfile*/
int codage_bloc_Y(struct bloc * bloc, struct mcu * mcu, int valeur_prec_DC_Y, FILE * file_out, int * indice_tampon, char ** tampon)
    {
    int16_t  vecteur_Y[64];
    for (int i = 0; i < mcu->hauteur_mcu_Y; i += 8)
        {
        for (int j = 0; j< mcu->largeur_mcu_Y; j += 8)
            {
            /* remplissage du bloc */    
            for (int h = 0; h < 8; h++)
                {
                for (int k = 0; k < 8; k++)
                    {
                    bloc->matrice_bloc[h][k] = mcu->matrice_mcu_Y[h + i][k + j];   
                    }
                }
            /*codage du bloc remplis*/
            fonction_dct(bloc->matrice_bloc);
            zig_zag(bloc->matrice_bloc, vecteur_Y);
            quantification(vecteur_Y, quantification_table_Y);
            codage(file_out, vecteur_Y, valeur_prec_DC_Y, tampon,indice_tampon, 0);
            /* conservation de la composante dc pour le bloc Y prochain*/
            valeur_prec_DC_Y = vecteur_Y[0]; 
            }
        }
    return(valeur_prec_DC_Y);
    }
/* applique sur un bloc 8*8 de la matrice _CB la dct ,la fonction zig_zag, la quantification, et apres on l'encode dans le fichier outfile*/
int codage_bloc_CB(struct bloc * bloc, struct mcu * mcu, int valeur_prec_DC_CB, FILE * file_out, int * indice_tampon, char ** tampon)
    {
    int16_t  vecteur_CB[64];
    for (int i = 0; i< mcu->hauteur_mcu_CB ; i += 8)
        {
        for (int j = 0; j< mcu->largeur_mcu_CB; j += 8)
            {
            /*remplissage du bloc*/    
            for (int h = 0; h < 8; h++)
                {
                for (int k = 0; k < 8; k++)
                    {
                    bloc->matrice_bloc[h][k] = mcu->matrice_mcu_CB[h + i][k + j];   
                    }
                }
            /*codage du bloc remplis*/
            fonction_dct(bloc->matrice_bloc);
            zig_zag(bloc->matrice_bloc, vecteur_CB);
            quantification(vecteur_CB, quantification_table_CbCr);
            codage(file_out, vecteur_CB, valeur_prec_DC_CB, tampon,indice_tampon, 1);
             /* conservation de la composante dc pour le bloc CB prochain*/
            valeur_prec_DC_CB = vecteur_CB[0]; 
            }

        }
    return(valeur_prec_DC_CB);
    }
/* applique sur un bloc 8*8 de la matrice _CR la dct ,la fonction zig_zag, la quantification, et apres on l'encode dans le fichier outfile*/
int codage_bloc_CR(struct bloc * bloc, struct mcu * mcu, int valeur_prec_DC_CR, FILE * file_out, int * indice_tampon, char ** tampon)
    {
    int16_t  vecteur_CR[64];
    for (int i = 0; i < mcu->hauteur_mcu_CR ; i += 8)
        {
        for (int j = 0; j < mcu->largeur_mcu_CR; j += 8)
            {
            /*remplissage du bloc*/    
            for (int h = 0; h < 8; h++)
                {
                for (int k = 0; k < 8; k++)
                    {
                    bloc->matrice_bloc[h][k] = mcu->matrice_mcu_CR[h + i][k + j];   
                    }
                }
            /*codage du bloc remplis*/
            fonction_dct(bloc->matrice_bloc);
            zig_zag(bloc->matrice_bloc, vecteur_CR);
            quantification(vecteur_CR, quantification_table_CbCr);
            codage(file_out, vecteur_CR, valeur_prec_DC_CR, tampon,indice_tampon, 2);
             /* conservation de la composante dc pour le bloc CR prochain*/
            valeur_prec_DC_CR = vecteur_CR[0]; 
            }
        }
    return(valeur_prec_DC_CR);
    }
