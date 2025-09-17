#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include <ctype.h>

#include <../include/lecture.h>

int extension(int dimension, uint8_t valeur) 
    {
    /* fonction qui calcule les nouveaux dimensions de l'image après duplication */
    int r = dimension % valeur;
    return ((r == 0) ? dimension : dimension + (valeur - r));
    }

void lecture_fichier(FILE* file, struct image_fichier * image_fichier )
    {
    /* fonction qui traite l'image en question en tirant les caractéristiques 
    comme la version (gris ou couleurs), largeur et hauteur, etc ... */
    unsigned char entete[2];
    fscanf(file, "%s", entete);
    if (entete[0] != 'P' || (entete[1] != '5' && entete[1] != '6')) 
        {
        fprintf(stderr,"Ce n'est pas un fichier PGM ou PPM valide l'entete du fichier commence avec P5 ou P6.\n");
        exit(1);
        }
    if (entete[1]=='5')
        {
        image_fichier->version="gris";
        }
    else 
        {
        image_fichier->version="couleurs"; 
        }
    uint32_t largeur, hauteur, max_intensite;
    fscanf(file, "%d %d ", &largeur, &hauteur);
    fscanf(file, "%d ", &max_intensite); 
    image_fichier->largeur = largeur;
    image_fichier->hauteur = hauteur;
    image_fichier->largeur_apres_dup = extension(largeur,image_fichier->coeff_echantillonnage[0] * 8);
    image_fichier->hauteur_apres_dup = extension(hauteur,image_fichier->coeff_echantillonnage[1] * 8);
    image_fichier->nombre_mcu = ((image_fichier->largeur_apres_dup) / (image_fichier->coeff_echantillonnage[0] * 8)) * ((image_fichier->hauteur_apres_dup) / (image_fichier->coeff_echantillonnage[1] * 8));
    }

void test_remplissage_image(struct image_fichier image_fichier)
    {
    /* tester que les caractéristiques sont justes */
    printf("version de l'image: %s\n",image_fichier.version);
    printf("hauteur de l image: %d\n",image_fichier.hauteur);
    printf("largeur de l'image: %d\n",image_fichier.largeur);
    printf("hauteur apres duplication de l'image: %d\n",image_fichier.hauteur_apres_dup);
    printf("largeur apres duplication de l'image: %d\n",image_fichier.largeur_apres_dup);
    printf("nombre de mcu: %d\n",image_fichier.nombre_mcu);
    }

bool verif_echantillonnage(struct image_fichier image_fichier)
    {
    /*fonction qui verifie si on va travailler sur des blocs 8*8*/
    bool test = false; 
    bool test1 = false; 
    bool test2 = false; 
    test1 = (image_fichier.coeff_echantillonnage[0] == 1) && (image_fichier.coeff_echantillonnage[1] == 1) && (image_fichier.coeff_echantillonnage[2] == 1);
    test2 = (image_fichier.coeff_echantillonnage[3] == 1) && (image_fichier.coeff_echantillonnage[4] == 1) && (image_fichier.coeff_echantillonnage[5] == 1);
    test = test1 && test2;
    return(test);
    }

void remplissage_mcu_grise(struct mcu * mcu, struct image_fichier image_fichier, long position, FILE * file_in, int i, int j)
    {
    /* fonction qui lit un mcu d'une image grise */
    mcu->matrice_mcu_CB = NULL;
    mcu->matrice_mcu_CR = NULL;
    mcu->hauteur_mcu_Y = 8;
    mcu->largeur_mcu_Y = 8;
    /* Remplissage matrice_mcu_Y */
    mcu->matrice_mcu_Y = malloc(8 * sizeof(int16_t *));
    for(int h = 0; h < 8; h++)
        {   
        mcu->matrice_mcu_Y[h] = malloc(8 * sizeof(int16_t));
        if (i + h >= (int)image_fichier.hauteur)
            {
            for(int l=0; l < 8; l++)
                {
                if (j + l >= (int)image_fichier.largeur)
                    {
                    mcu->matrice_mcu_Y[h][l] = mcu->matrice_mcu_Y[(image_fichier.hauteur -1) % 8][(image_fichier.largeur -1) % 8];
                    } 
                else
                    {
                    mcu->matrice_mcu_Y[h][l] = mcu->matrice_mcu_Y[(image_fichier.hauteur - 1) % 8][l];
                    }
                }
            }
        else
            {
            for(int l=0; l < 8; l++)
                {
                if (j + l >= (int)image_fichier.largeur)
                    {
                    mcu->matrice_mcu_Y[h][l] = mcu->matrice_mcu_Y[h][(image_fichier.largeur - 1) % 8];
                    } 
                else
                    {
                    fseek(file_in, position + ((i+h) * image_fichier.largeur) + (j+l), SEEK_SET);
                    mcu->matrice_mcu_Y[h][l] = fgetc(file_in);
                    }
                }
            }
        }
    }

void affichage_mcu_grise(struct mcu * mcu)
    {
    /* afficher les mcus des images grises */
    printf(" affichage de la mcu grise \n");
    for (int i = 0; i < 8; i++)
        {
        for (int j =0; j < 8; j++)
            {
            printf("%02x ", mcu->matrice_mcu_Y[i][j]);
            }
        printf("\n");
        }
    }

void affichage_mcu_couleur(struct mcu * mcu)
    {
    /* affichage les mcus des images couleurs */
    printf(" affichage de la mcu Y \n");
    for (int i = 0; i < (int)mcu->hauteur_mcu_Y; i++)
        {
        for (int j =0; j < (int)mcu->largeur_mcu_Y; j++)
            {
            printf("%02x ", mcu->matrice_mcu_Y[i][j]);
            }
        printf("\n");
        }
    printf("\n");

    printf(" affichage de la mcu CB \n");
    for (int i = 0; i < (int)mcu->hauteur_mcu_CB; i++)
        {
        for (int j =0; j < (int)mcu->largeur_mcu_CB; j++)
            {
            printf("%02x ", mcu->matrice_mcu_CB[i][j]);
            }
        printf("\n");
        }
    printf("\n");  

    printf(" affichage de la mcu CR  \n");
    for (int i = 0; i < (int)mcu->hauteur_mcu_CR; i++)
        {
        for (int j =0; j < (int)mcu->largeur_mcu_CR; j++)
            {
            printf("%02x ", mcu->matrice_mcu_CR[i][j]);
            }
        printf("\n");
        }
    }

void initialisation(int32_t * vecteur, int longueur)
    {
    /* initialiser les matrices des mcus à 0 */
    for (int i = 0; i < longueur; i++)
        {
        vecteur[i] = 0;
        }
    }

void moyenne(int32_t ** matrice, int hauteur, int largeur, int facteur)
    {
    /* fonction utilisée par le remplissage des images couleurs. 
    Elle fait la moyenne de chaque case */
    for (int i = 0; i < hauteur; i++)
        {
        for (int j = 0; j < largeur; j++)
            {
            matrice[i][j] = roundf(matrice[i][j] / facteur);
            if (matrice[i][j] > 255)
                {
                matrice[i][j] = 255;
                }
            if (matrice[i][j] < 0)
                {
                matrice[i][j] = 0;
                }
            }
        }
    }

int corrige_valeur(int valeur)
    {
    /* fonction pour ajuster les valeurs et éviter leurs débordements */
    if (valeur > 255)
        {
        return 255;
        }
    else if (valeur < 0)
        {
        return 0;
        }
    else 
        {
        return valeur;
        }
    }

void remplissage_mcu_couleur(struct mcu * mcu, struct image_fichier image_fichier, long position, FILE * file_in, int i, int j)
    {
    /* fonction qui lit un mcu d'une image en couleurs */

    /* définition des paramètres d'echantillonnage */
    mcu->hauteur_mcu_Y = image_fichier.coeff_echantillonnage[1] * 8;
    mcu->largeur_mcu_Y = image_fichier.coeff_echantillonnage[0] * 8;
    mcu->hauteur_mcu_CB = image_fichier.coeff_echantillonnage[3] * 8;
    mcu->largeur_mcu_CB = image_fichier.coeff_echantillonnage[2] * 8;
    mcu->hauteur_mcu_CR = image_fichier.coeff_echantillonnage[5] * 8;
    mcu->largeur_mcu_CR = image_fichier.coeff_echantillonnage[4] * 8;

    int facteur_echantillonage_vertic_cb = image_fichier.coeff_echantillonnage[1] / image_fichier.coeff_echantillonnage[3];
    int facteur_echantillonage_horiz_cb = image_fichier.coeff_echantillonnage[0] / image_fichier.coeff_echantillonnage[2];
    int facteur_echantillonage_vertic_cr = image_fichier.coeff_echantillonnage[1] / image_fichier.coeff_echantillonnage[5];
    int facteur_echantillonage_horiz_cr = image_fichier.coeff_echantillonnage[0] / image_fichier.coeff_echantillonnage[4];

    /* définition de matrices à utiliser en se basant sur l'echantiollonnage */
    mcu->matrice_mcu_Y = malloc( mcu->hauteur_mcu_Y * sizeof(int16_t *));
    int32_t ** matrice_CB;
    int32_t ** matrice_CR;
    matrice_CB = malloc( mcu->hauteur_mcu_CB * sizeof(int32_t *));
    for(int h = 0; h < mcu->hauteur_mcu_CB; h++)
        {
        matrice_CB[h] = malloc( mcu->largeur_mcu_CB * sizeof(int32_t));
        initialisation(matrice_CB[h], mcu->largeur_mcu_CB);
        }
    matrice_CR = malloc( mcu->hauteur_mcu_CR * sizeof(int32_t *));
    for(int h = 0; h < mcu->hauteur_mcu_CR; h++)
        {
        matrice_CR[h] = malloc( mcu->largeur_mcu_CR * sizeof(int32_t)); 
        initialisation(matrice_CR[h], mcu->largeur_mcu_CR); 
        }

    /* début lecture des mcus */
    int16_t R,B,G;
    for(int h = 0; h < mcu->hauteur_mcu_Y; h++)
        {  
        mcu->matrice_mcu_Y[h] = malloc( mcu->largeur_mcu_Y * sizeof(int16_t));   
        if (i + h >= (int)image_fichier.hauteur)
            {
            for(int l = 0; l < mcu->largeur_mcu_Y; l++)
                {
                if (j + l >= (int)image_fichier.largeur)
                    { 
                    fseek(file_in, position + (((int)image_fichier.hauteur -1) * image_fichier.largeur) * 3 + ((int)image_fichier.largeur -1) * 3, SEEK_SET);
                    R = fgetc(file_in);
                    G = fgetc(file_in);
                    B = fgetc(file_in);
                    mcu->matrice_mcu_Y[h][l] = (int16_t)corrige_valeur(roundf(0.299 * R + 0.587 * G +0.114 * B));
                    matrice_CB[h / facteur_echantillonage_vertic_cb][l / facteur_echantillonage_horiz_cb] += roundf(-0.1687 * R -0.3313 * G + 0.5 * B +128);
                    matrice_CR[h / facteur_echantillonage_vertic_cr][l / facteur_echantillonage_horiz_cr] += roundf(0.5 * R -0.4187 * G -0.0813 * B +128);
                    } 
                else
                    {
                    fseek(file_in, position + (((int)image_fichier.hauteur -1) * image_fichier.largeur) * 3 + (j + l) * 3, SEEK_SET);
                    R = fgetc(file_in);
                    G = fgetc(file_in);
                    B = fgetc(file_in);
                    mcu->matrice_mcu_Y[h][l] = (int16_t)corrige_valeur(roundf(0.299 * R +0.587 * G +0.114 * B));
                    matrice_CB[h / facteur_echantillonage_vertic_cb][l / facteur_echantillonage_horiz_cb] += roundf(-0.1687 * R -0.3313 * G + 0.5 * B +128);
                    matrice_CR[h / facteur_echantillonage_vertic_cr][l / facteur_echantillonage_horiz_cr] += roundf(0.5 * R -0.4187 * G -0.0813 * B +128);
                    }
                }
            }
        else
            {
            for(int l = 0; l < mcu->largeur_mcu_Y; l++)
                {
                if (j + l >= (int)image_fichier.largeur)
                    {
                    fseek(file_in, position + ((i + h) * image_fichier.largeur) * 3 + ((int)image_fichier.largeur -1) * 3, SEEK_SET);
                    R = fgetc(file_in);
                    G = fgetc(file_in);
                    B = fgetc(file_in);
                    mcu->matrice_mcu_Y[h][l] = (int16_t)corrige_valeur(roundf(0.299 * R + 0.587 * G + 0.114 * B));
                    matrice_CB[h / facteur_echantillonage_vertic_cb][l / facteur_echantillonage_horiz_cb] += roundf(-0.1687 * R -0.3313 * G +0.5 * B +128);
                    matrice_CR[h / facteur_echantillonage_vertic_cr][l / facteur_echantillonage_horiz_cr] += roundf(0.5 * R -0.4187 * G -0.0813 * B +128);
                    } 
                else
                    {
                    fseek(file_in, position + ((i + h) * image_fichier.largeur) * 3 + (j + l) * 3, SEEK_SET);
                    R = fgetc(file_in);
                    G = fgetc(file_in);
                    B = fgetc(file_in);
                    mcu->matrice_mcu_Y[h][l] = (int16_t)corrige_valeur(roundf(0.299 * R + 0.587 * G + 0.114 * B));
                    matrice_CB[h / facteur_echantillonage_vertic_cb][l / facteur_echantillonage_horiz_cb] += roundf( -0.1687 * R -0.3313 * G +0.5 * B + 128);
                    matrice_CR[h / facteur_echantillonage_vertic_cr][l / facteur_echantillonage_horiz_cr] += roundf(0.5 * R -0.4187 * G -0.0813 * B + 128);
                    }
                }
            }
        }
    
    /* faire la moyenne des cases */
    moyenne(matrice_CB, mcu->hauteur_mcu_CB, mcu->largeur_mcu_CB, facteur_echantillonage_horiz_cb * facteur_echantillonage_vertic_cb);
    moyenne(matrice_CR, mcu->hauteur_mcu_CR, mcu->largeur_mcu_CR, facteur_echantillonage_horiz_cr * facteur_echantillonage_vertic_cr);
    
    /* mettre les valeurs dans le matrices du mcu */
    mcu->matrice_mcu_CB = malloc( mcu->hauteur_mcu_CB * sizeof(int16_t *));
    for (int h = 0; h < mcu->hauteur_mcu_CB; h++)
        {
        mcu->matrice_mcu_CB[h] = malloc( mcu->largeur_mcu_CB * sizeof(int16_t));
        for (int l = 0; l < mcu->largeur_mcu_CB; l++)
            {
            mcu->matrice_mcu_CB[h][l] = (int16_t)(matrice_CB[h][l]);
            }
        free(matrice_CB[h]);
        }
    free(matrice_CB);

    mcu->matrice_mcu_CR = malloc( mcu->hauteur_mcu_CR * sizeof(int16_t *));
    for (int h = 0; h < mcu->hauteur_mcu_CR; h++)
        {
        mcu->matrice_mcu_CR[h] = malloc( mcu->largeur_mcu_CR * sizeof(int16_t));
        for (int l = 0; l < mcu->largeur_mcu_CR; l++)
            {
            mcu->matrice_mcu_CR[h][l] = (int16_t)(matrice_CR[h][l]);
            }
        free(matrice_CR[h]);
        }
    free(matrice_CR);
    }

void free_mcu(struct mcu * mcu)
    {
    /* fonction pour libérer l'espace mémoire réservé à un mcu */
    for (int i = 0; i < (int)mcu->hauteur_mcu_Y; i++)
        {
        free(mcu->matrice_mcu_Y[i]);
        }    
    if (mcu->matrice_mcu_CB != NULL)
        {
        for (int i = 0; i < (int)mcu->hauteur_mcu_CB; i++)
            {
            free(mcu->matrice_mcu_CB[i]);
            }      
        }
    if (mcu->matrice_mcu_CR!=NULL)
        {
        for (int i=0; i < (int)mcu->hauteur_mcu_CR; i++)
            {
            free(mcu->matrice_mcu_CR[i]);
            }        
        }
    free(mcu->matrice_mcu_Y);
    free(mcu->matrice_mcu_CB);
    free(mcu->matrice_mcu_CR);
    }