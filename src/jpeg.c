#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include <ctype.h>

#include <../include/commande.h>
#include <../include/qtables.h>
#include <../include/htables.h>
/* fonction permettant d'écrire un nombre sur 2 bits dans le fichier*/
void ecriture_nombre(FILE * file, int * nombre)
    {
    uint8_t octet_1 = *nombre % (16*16);
    uint8_t octet_2 = *nombre / (16*16);
    fwrite(&octet_2, sizeof(uint8_t), 1, file);
    fwrite(&octet_1, sizeof(uint8_t), 1, file);
    }
/* fonction qui écrit l'entete du fichier outfile: la partie SOI,DQT,SOFO,DHT,et SOS sans le code brut des données de l'image*/
void remplissage_entete_fichier_jpeg(FILE * file, struct image_fichier image_fichier_en_lecture)
    {
    /* marqueur SOI début image */
    fputc(0xFF, file);
    fputc(0xD8, file);
    /* marqueur APPO */
    fputc(0xFF, file);
    fputc(0xE0, file);
    /* la longueur */
    int longueur = 16;
    ecriture_nombre(file, &longueur);
    /* JFIF */
    char str[] = {'J', 'F', 'I', 'F', '\0'};
    fwrite(str, sizeof(char), sizeof(str), file);
    /* version 1.1 */
    fputc(0x01, file);
    fputc(0x01, file);
    /* Données non traitées mises à 0 */
    fputc(0x00, file);
    fputc(0x00, file);
    fputc(0x00, file);
    fputc(0x00, file);
    fputc(0x00, file);
    fputc(0x00, file);
    fputc(0x00, file);
    /* section tableaux de quantification */
    /* tableau de quantification pour Y */
    /* marqueur DQT */
    fputc(0xFF, file);
    fputc(0xDB, file);
    /* longueur */
    fputc(0x00, file);
    fputc(0x43, file);
    /* precision et indice */
    fputc(0x00, file);
    fwrite(quantification_table_Y, sizeof(unsigned char), sizeof(quantification_table_Y), file);
    if (strcmp(image_fichier_en_lecture.version, "couleurs") == 0)
        {
        /* tableau de quantification pour CB CR */
        /* marqueur DQT */
        fputc(0xFF, file);
        fputc(0xDB, file);
        /* longueur */
        fputc(0x00, file);
        fputc(0x43, file);
        /* precision et indice */
        fputc(0x01, file);
        fwrite(quantification_table_CbCr, sizeof(unsigned char), sizeof(quantification_table_CbCr), file);
        }
    /* section start of frame */
    /* marqueur SOF0 */
    fputc(0xFF, file);
    fputc(0xC0, file);
    /* longueur */
    fputc(0x00, file);
    if (strcmp(image_fichier_en_lecture.version, "couleurs") == 0)
        {
        fputc(0x11, file);
        }
    else
        {
        fputc(0x0B, file);
        }
    /* precision */
    fputc(0x08, file);
    /* hauteur et largeur */
    ecriture_nombre(file, &image_fichier_en_lecture.hauteur);
    ecriture_nombre(file, &image_fichier_en_lecture.largeur);
    /* nombre de composantes */
    if (strcmp(image_fichier_en_lecture.version, "couleurs") == 0)
        {
        fputc(0x03, file);
        }
    else
        {
        fputc(0x01, file);
        }
    if (strcmp(image_fichier_en_lecture.version, "couleurs") == 0)
        {
        /* identifiant */
        fputc(0x01, file);
        /* facteur d'echantillonnage horizontal et vertical */
        uint8_t valeur = image_fichier_en_lecture.coeff_echantillonnage[1] + image_fichier_en_lecture.coeff_echantillonnage[0] * 16;
        fwrite(&valeur, sizeof(uint8_t), 1, file);
        /* indice de table de quantification */
        fputc(0x00, file);
        /* identifiant */
        fputc(0x02, file);
        /* facteur d'echantillonnage horizontal et vertical */
        valeur = image_fichier_en_lecture.coeff_echantillonnage[3] + image_fichier_en_lecture.coeff_echantillonnage[2] * 16;
        fwrite(&valeur, sizeof(uint8_t), 1, file);
        /* indice de table de quantification */
        fputc(0x01, file);
        /* identifiant */
        fputc(0x03, file);
        /* facteur d'echantillonnage horizontal et vertical */
        valeur = image_fichier_en_lecture.coeff_echantillonnage[5] + image_fichier_en_lecture.coeff_echantillonnage[4] * 16;
        fwrite(&valeur, sizeof(uint8_t), 1, file);
        /* indice de table de quantification */
        fputc(0x01, file);
        }
    else
        {
        /* identifiant */
        fputc(0x01, file);
        /* facteur d'echantillonnage horizontal et vertical */
        fputc(0x11, file);
        /* indice de table de quantification */
        fputc(0x00, file);
        }
            
    /* tab DHT */
    /* 1er tableau (DC) de Y */
    /* marqueur DHT */
    fputc(0xFF, file);
    fputc(0xC4, file);
    /* longueur */
    fputc(0x00, file);
    fputc(0x1F, file);
    /* information */
    fputc(0x00, file);
    /* nombre de symboles */
    int sum = 0;
    for(int i = 0; i < 16; i++) 
        {
        sum += htables_nb_symb_per_lengths[0][0][i];    
        fwrite(&htables_nb_symb_per_lengths[0][0][i], 1, 1, file);
        }
    for(int i = 0; i < sum; i++) 
        {
       fwrite(&htables_symbols[0][0][i], 1, 1, file);
        }
        
    /* 2eme tableau (AC) de Y */
    /* marqueur DHT */
    fputc(0xFF, file);
    fputc(0xC4, file);
    /* longueur */
    fputc(0x00, file);
    fputc(0xB5, file);
    /* information */
    fputc(0x10, file);
    /* nombre de symboles */
    sum = 0;
    for(int i = 0; i < 16; i++) 
        {
        fwrite(&htables_nb_symb_per_lengths[1][0][i], 1, 1, file);
        sum += htables_nb_symb_per_lengths[1][0][i];
        }
    /* les symboles */
    for(int i = 0; i < sum; i++) 
        {
            fwrite(&htables_symbols[1][0][i], 1, 1, file);
        }
    if (strcmp(image_fichier_en_lecture.version, "couleurs") == 0)
        {
        /* 1er tableau (DC) de CB ET CR */
        /* marqueur DHT */
        fputc(0xFF, file);
        fputc(0xC4, file);
        /* longueur */
        fputc(0x00, file);
        fputc(0x1F, file);
        /* information */
        fputc(0x01, file); 
        /* nombre de symboles */
        sum = 0;
        for (int i = 0; i < 16; i++) 
            {
            sum += htables_nb_symb_per_lengths[0][1][i];    
            fwrite(&htables_nb_symb_per_lengths[0][1][i], 1, 1, file);
            }
        for (int i = 0; i < sum ; i++) 
            {
            fwrite(&htables_symbols[0][1][i], 1, 1, file);
            }

        /* 2eme tableau (AC) de CB ET CR */
        /* marqueur DHT */
        fputc(0xFF, file);
        fputc(0xC4, file);
        /* longueur */
        fputc(0x00, file);
        fputc(0xB5, file);
        /* information */
        fputc(0x11, file); 
        /* nombre de symboles */
        sum=0;
        for(int i = 0; i < 16; i++) 
            {
            fwrite(&htables_nb_symb_per_lengths[1][1][i], 1, 1, file);
            sum += htables_nb_symb_per_lengths[1][1][i];
            }
        /* les symboles */
        for(int i = 0; i <sum ; i++) 
            {
            fwrite(&htables_symbols[1][1][i], 1, 1, file);
            }
        }
        
    /*start of scan*/
    /* marqueur SOS */
    fputc(0xFF, file);
    fputc(0xDA, file);
    /* longueur et nombre de composantes */
    if (strcmp(image_fichier_en_lecture.version, "couleurs") == 0)
        {
        fputc(0x00, file);
        fputc(0x0C, file);
        fputc(0x03, file);
        } 
    else 
        {
        fputc(0x00, file);
        fputc(0x08, file);
        fputc(0x01, file);
        }
    fputc(0x01, file);
    fputc(0x00, file);
    if (strcmp(image_fichier_en_lecture.version, "couleurs") == 0)
        {
        fputc(0x02, file);
        fputc(0x11, file);
        fputc(0x03, file);
        fputc(0x11, file);
        } 
    fputc(0x00, file);
    fputc(0x3F, file);
    fputc(0x00, file);
    }

void ecriture_fin_image(FILE * file)
    {
    /* marqueur EOI */
    fputc(0xFF, file);
    fputc(0xD9, file);
    }
