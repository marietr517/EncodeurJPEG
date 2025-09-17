#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include <ctype.h>

#include <../include/commande.h>
#include <../include/lecture.h>
#include <../include/jpeg.h>
#include <../include/codage_blocs.h>
#include <../include/dct.h>
#include <../include/zigzag_quant.h>
#include <../include/qtables.h>
#include <../include/codage_ac_dc.h>

int main(int argc, char* argv[]) 
    {

    /* traiter les cas triviaux pour le traitement de la commande en entrée sur terminal */
    if (argc == 1) 
        {
        fprintf(stderr, "L'entrée doit être de la manière suivante : ./ppm2jpeg (option) nom-du-fichier-à-encoder.\n");
        exit(EXIT_FAILURE);
        }

    /* traitement des options et nom du fichier à lire */
    struct image_fichier image_fichier_en_lecture;
    struct arguments arg = traitement_des_options(argc, argv);
    bool test = verification_validite(arg);

    FILE * file = fopen(arg.fichier_en_lecture, "rb");
    if (file == NULL) 
        {
        fprintf(stderr, "Il est impossible d'ouvrir ce fichier en lecture. Vérifiez qu'il existe bien dans le répertoire.\n");
        exit(EXIT_FAILURE);
        }

    if (!test) 
        {
        fprintf(stderr, "Vous avez une entête invalide.\n");
        exit(EXIT_FAILURE);
        }

    /* stocker les valeurs d'echantillonnage */
    image_fichier_en_lecture.coeff_echantillonnage = malloc(6 * sizeof(uint8_t));
    image_fichier_en_lecture.coeff_echantillonnage[0] = atoi(&arg.facteurs_echantillonnage[0]);
    image_fichier_en_lecture.coeff_echantillonnage[1] = atoi(&arg.facteurs_echantillonnage[2]);
    image_fichier_en_lecture.coeff_echantillonnage[2] = atoi(&arg.facteurs_echantillonnage[4]);
    image_fichier_en_lecture.coeff_echantillonnage[3] = atoi(&arg.facteurs_echantillonnage[6]);
    image_fichier_en_lecture.coeff_echantillonnage[4] = atoi(&arg.facteurs_echantillonnage[8]);
    image_fichier_en_lecture.coeff_echantillonnage[5] = atoi(&arg.facteurs_echantillonnage[10]);

    /* affichage du fichier resultat */
    fprintf(stderr, "votre fichier jpg généré %s\n", arg.fichier_en_ecriture);

    /* lecture et test du fichier en entrée */
    lecture_fichier(file, &image_fichier_en_lecture);
    test_remplissage_image(image_fichier_en_lecture);

    /* commencer le flux du fichier sortie */
    FILE * file_out = fopen(arg.fichier_en_ecriture, "wb");
    remplissage_entete_fichier_jpeg(file_out, image_fichier_en_lecture);
    
    /* se localiser dans la bonne position dans le fichier à lire */
    long position = ftell(file);
    int indice_mcu = 0;

    /*preparation du tampon */
    int valeur_prec_DC_Y = 0;
    int valeur_prec_DC_CB = 0;
    int valeur_prec_DC_CR = 0;
    char * tampon = malloc(8 * sizeof(char));
    for (int i = 0; i < 8; i++)
        {
        tampon[i] = '0';
        }
    int indice_tampon = 0;

    /* préparation du bloc 8x8 */
    struct bloc bloc_8;
    creation_bloc(&bloc_8);
    
    /* parcourir le fichier à lire */
    for (int i = 0; i < (int)image_fichier_en_lecture.hauteur_apres_dup; i += 8 * image_fichier_en_lecture.coeff_echantillonnage[1])
        {   
        for (int j = 0; j < (int)image_fichier_en_lecture.largeur_apres_dup; j += 8 * image_fichier_en_lecture.coeff_echantillonnage[0])
            { 

            /* partie pour remplir la mcu */
            struct mcu mcu;
            if (strcmp(image_fichier_en_lecture.version, "gris") == 0)
                {
                remplissage_mcu_grise(&mcu, image_fichier_en_lecture, position, file, i, j);
                indice_mcu++;
                } 
            else 
                {
                remplissage_mcu_couleur(&mcu, image_fichier_en_lecture, position, file, i, j);
                affichage_mcu_couleur(&mcu);
                indice_mcu++;
                }

            /* deux cas : si on a echantillonnage et sinon */    
            bool test = verif_echantillonnage(image_fichier_en_lecture);

            if (test)
                {
                /* on a pas d echantillonnage */

                /* traitement de matrice Y */
                fonction_dct(mcu.matrice_mcu_Y);
                int16_t vecteur_Y[64];
                zig_zag(mcu.matrice_mcu_Y, vecteur_Y);
                quantification(vecteur_Y, quantification_table_Y);
                codage(file_out, vecteur_Y, valeur_prec_DC_Y, &tampon, &indice_tampon, 0);
                valeur_prec_DC_Y = vecteur_Y[0];

                if (strcmp(image_fichier_en_lecture.version, "couleurs") == 0)
                    {
                    /*traitement de matrice CB*/
                    fonction_dct(mcu.matrice_mcu_CB);
                    int16_t vecteur_CB[64] ;
                    zig_zag(mcu.matrice_mcu_CB, vecteur_CB);
                    quantification(vecteur_CB, quantification_table_CbCr);
                    codage(file_out, vecteur_CB, valeur_prec_DC_CB, &tampon, &indice_tampon, 1);
                    valeur_prec_DC_CB = vecteur_CB[0];

                    /*traitement de matrice CR*/
                    fonction_dct(mcu.matrice_mcu_CR);
                    int16_t  vecteur_CR[64] ;
                    zig_zag(mcu.matrice_mcu_CR, vecteur_CR);
                    quantification(vecteur_CR, quantification_table_CbCr);
                    codage(file_out, vecteur_CR, valeur_prec_DC_CR, &tampon, &indice_tampon, 2);
                    valeur_prec_DC_CR = vecteur_CR[0];                
                    }
                }
            else 
                {   
                /* on a echantillonnage */
                valeur_prec_DC_Y = codage_bloc_Y(&bloc_8, &mcu, valeur_prec_DC_Y, file_out, &indice_tampon, &tampon);
                valeur_prec_DC_CB = codage_bloc_CB(&bloc_8, &mcu, valeur_prec_DC_CB, file_out, &indice_tampon, &tampon);
                valeur_prec_DC_CR = codage_bloc_CR(&bloc_8, &mcu, valeur_prec_DC_CR, file_out, &indice_tampon, &tampon);    
                }
            free_mcu(&mcu);
            }
        } 

    /* vider le tampon */
    int8_t valeur = 0;    
    for (int i = 0; i < indice_tampon; i++)
        {
        valeur = valeur + pow(2, indice_tampon - i);
        } 
    fwrite(&valeur, sizeof(int8_t), 1, file_out);
    free(tampon);

    /* vider l'espace mémoire */
    free_bloc(&bloc_8);
    free(image_fichier_en_lecture.coeff_echantillonnage);
    ecriture_fin_image(file_out);
    free(arg.fichier_en_ecriture);

    /* fermer les fichiers */
    fclose(file);
    fclose(file_out);

    /* c'est fini ! */
    return EXIT_SUCCESS;
    }

