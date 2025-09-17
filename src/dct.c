#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include <ctype.h>

void decalage(int16_t ** matrice)
    {
    /* la fonction qui fait le offset de 128 avant la DCT */
    for (int i=0;i<8;i++)
        {
        for (int j = 0; j <8; j++) 
            {
            matrice[i][j] = matrice[i][j]-128;
            }
        }
    }

void fonction_dct(int16_t ** mcu)
    {
    /* la fonction qui calcul la DCT */

    /* faire le offset */
    decalage(mcu);

    /* stocker les tableaux des cosinus */
    float cosinus_tab[8][8];
    float phi[8][8];
    for (int i = 0; i < 8; i++)
        {
        for (int j = 0; j <8; j++) 
            {
            cosinus_tab[i][j] = cos((float)((float)((2.0 * i + 1.0) * M_PI * j) / (float)(2.0 * 8.0)));
            }
        }

    /* appliquer la formule de DCT pour chaque case */
    for (int i = 0; i < 8; i++)
        {
        for (int j = 0; j < 8; j++) 
            {
            float sum = 0.0;
            for (int x = 0; x < 8; x++) 
                {
                for (int y = 0; y < 8; y++) 
                    {
                    sum += mcu[x][y] * cosinus_tab[x][i] * cosinus_tab[y][j];
                    } 
                }
            if (i == 0) 
                {
                sum = sum / (sqrt(2));
                }
            if (j == 0) 
                {
                sum = sum / (sqrt(2));
                }
            phi[i][j] = sum / 4.0;
            }        
        }

    /* stocker les valeurs finales */
    for (int i=0; i < 8; i++)
        {
        for (int j = 0; j < 8; j++) 
            {
            mcu[i][j]=ceil(phi[i][j]);
            }
        }
    }
    