#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include <ctype.h>

int16_t * magnitude(int16_t nombre)
    {
    int16_t indice;
    int16_t classe;
    if (nombre == 0)
        {
        classe = 0;    
        }
    else 
        {
        classe = (int16_t)(log2(abs(nombre))) + 1;
        }  
    if (nombre >= 0)
        {
        indice = nombre;
        }
    else
        {
        indice = pow(2, classe) - 1 + nombre;
        }
    int16_t * tab = malloc(2 * sizeof(int16_t));
    tab[0] = classe;
    tab[1] = indice;
    return tab;
    }

char * conversion_binaire(int32_t nombre, int taille)
    {
    char * chaine_binaire = (char *)malloc(taille + 1);
    chaine_binaire[taille] = '\0';
    int i = taille - 1;
    while (i >= 0)
        {
        if ( nombre % 2 == 0)
            {
            chaine_binaire[i] = '0';
            }
        else
            {
            chaine_binaire[i] = '1';
            }
        nombre = nombre / 2;
        i--;
        }
    return chaine_binaire;
    }

char* code_huffman(uint8_t nombre, uint8_t* htables_lengths, uint8_t* htables_symbols)
    {
    bool test = false;
    int indice = 1;
    int indice_de_symboles = 0;
    int32_t code = 0;
    int nb_de_valeurs = 0;

    while (!(test))
        {
        if (htables_lengths[indice] == 0)
            {
            indice++;
            code = code * 2; 
            }
        else 
            { 
            nb_de_valeurs++; 
            test = htables_symbols[indice_de_symboles] == nombre;  
            if (!(test))
                {
                indice_de_symboles++;
                if (nb_de_valeurs >= htables_lengths[indice])
                    {
                    code = (code + 1) * 2;
                    nb_de_valeurs = 0;
                    indice++;
                    }
                else 
                    {
                    code = (code + 1);
                    }
                }
            }
        }
    indice += 1;
    char * code_huffman = conversion_binaire(code, indice);
    return(code_huffman);
    }  