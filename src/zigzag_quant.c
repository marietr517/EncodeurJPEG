#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include <ctype.h>


void zig_zag( int16_t ** matrice_mcu, int16_t tab[])
    {  
    /* la fonction qui donne le vecteur zig zag d'une matrice  */
    int i = 0;
    int j = 0;
    int pente = 0;
    int k = 0;
    while (i <= 7 && j <= 7)
        {
        tab[k] = matrice_mcu[i][j];
        if (i == 0 || i == 7) 
            {
            if (j == 7) 
                {
                pente =- 1;
                }
            j = j + 1;
            k++;
            tab[k] = matrice_mcu[i][j];
            }
        else if (j == 0 || (j == 7)) 
            {
            i = i + 1;
            k++;
            tab[k]=matrice_mcu[i][j];
            }
        else if (i==0 && j==7)
            {
            pente = -1;
            }
        if (i == 0 || j == 7) 
            { 
            pente = -1;
            }
        if (j == 0 || i == 7)  
            {
            pente = 1;
            }
        if (pente == 1) 
            {
            i = i - 1;
            j = j + 1;
            k++;
            }
        else if (pente == -1)
            {
            i = i + 1;
            j = j - 1;
            k++;
            }
        }
    }

void quantification(int16_t vecteur[64], uint8_t * tab_quant)
    {
    /* fonction qui calcul les valeurs du vecteur après le quantifier */
    for(int indice = 0; indice < 64; indice++)
        {
        vecteur[indice] = vecteur[indice] / tab_quant[indice];
        }
    }

void affichage_quantification(int16_t * vecteur)
    {
    /* fonction pour afficher le vecteur quantifié */
    printf("affiche apres quantification \n");
    for(int i = 0; i < 64; i++)
        {
        printf("%04x ", vecteur[i]);
        }
    printf("\n");
    }