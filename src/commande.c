#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#include <../include/commande.h>

/* message affiché pour l'option --h ou --help */
void print_help() 
    {
    fprintf(stderr,"Usage: ppm2jpeg <input_file.ppm> [options]\n");
    fprintf(stderr,"Options:\n");
    fprintf(stderr," --help affiche ce message\n");
    fprintf(stderr," --outfile=<output_file.jpg> permet de spécifier le nom du fichier sortie\n");
    fprintf(stderr," --sample=h1xv1,h2xv2,h3xv3 permet de spécifier les facteurs d'échantillonnage (par défaut: 1x1,1x1,1x1)\n");
    }
/* fonction qui permet de traiter les paramètres optionnels sur la ligne de commande
   et retourne les arguments stockés dans struct arguments */
struct arguments traitement_des_options(int argc, char* argv[]) 
    {
    /* l'option  help */
    if (argc == 2)
        {
        if ((strcmp(argv[1], "--help") == 0)||(strcmp(argv[1], "--h") == 0))
                {
                print_help();
                exit(EXIT_FAILURE);    
                } 
        }
    
    struct arguments arguments;
    /*valeurs par défaut*/
    arguments.facteurs_echantillonnage = "1x1,1x1,1x1";
    arguments.fichier_en_ecriture = NULL;
    arguments.fichier_en_lecture = NULL;
    for (int i = 1; i < argc; i++) 
        {
        /* Nom du fichier sortie*/
        if (strncmp(argv[i], "--outfile=", 10) == 0) 
            {
            arguments.fichier_en_ecriture = malloc((strlen(argv[i] + 10) + 1) * sizeof(char));
            strcpy(arguments.fichier_en_ecriture, argv[i] + 10);
            } 
        /* définition des facteurs d'échantillonnage */    
        else if (strncmp(argv[i], "--sample=", 9) == 0) 
            {   
            arguments.facteurs_echantillonnage = malloc(12 * sizeof(char));
            for (int j = 0; j < 12; j++)
                {
                arguments.facteurs_echantillonnage[j] = argv[i][j + 9];
                }
            arguments.facteurs_echantillonnage[11] = '\0';
            strcpy(arguments.facteurs_echantillonnage, argv[i] + 9);
            }
        /* Nom du fichier entré en lecture */    
        else if ((argv[i][0]) != '-')
            {
            arguments.fichier_en_lecture = malloc((strlen(argv[i]) + 1) * sizeof(char));
            strcpy(arguments.fichier_en_lecture, argv[i]);
            fprintf(stderr,"nom du fichier %s\n", arguments.fichier_en_lecture);
            }
         /* error */   
        else 
            {
            printf("une option non connue %s\n", argv[i]);
            print_help();
            exit(EXIT_FAILURE);
            }
        }

    if (arguments.fichier_en_lecture == NULL) 
        {
        fprintf(stderr, "Tu dois donner un fichier en entrée.\n");
        exit(EXIT_FAILURE);
        }
    /* Nom du fichier sortie définit par défaut*/
    if (arguments.fichier_en_ecriture == NULL) 
        {
        char * name = arguments.fichier_en_lecture;
        int longueur = strlen(name);
        char * file_out = malloc((longueur + 1) * sizeof(char));
        for (int i = 0; i < longueur - 2; i++)
            {
            file_out[i] = name[i];
            }
        char * type = "jpg";
        for (int i = 0; i < 4; i++)
            {
            file_out[i + longueur - 3] = type[i];
            }
        arguments.fichier_en_ecriture = file_out;
        }
    
    return arguments;
    }   
/* Fonction permettant de vérifier la validité du nom fichier en lecture passé sur la ligne commande 
   et des facteurs d'échantillonnage*/
bool verification_validite(struct arguments arg) 
    {
    char * chaine = arg.fichier_en_lecture;
    bool test_extension;
    /*extention ppm ou pgm*/
    if (((chaine[strlen(chaine) - 4] == '.') && (chaine[strlen(chaine) - 3] == 'p') && (chaine[strlen(chaine) - 2] == 'p') && (chaine[strlen(chaine) - 1] == 'm')) || ((chaine[strlen(chaine) - 4] == '.') && (chaine[strlen(chaine) - 3] == 'p') && (chaine[strlen(chaine) - 2] == 'g') && (chaine[strlen(chaine) - 1] == 'm')))
        {
        test_extension = true;
        }
    else 
        {
        test_extension = false;
        fprintf(stderr, "Erreur: L'extention du fichier devra finir avec .ppm ou .pgm'.\n");
        exit(1);
        }  
    
    bool test_facteurs = true;
    char * chaine_facteurs = arg.facteurs_echantillonnage;
    int len = (int)strlen(chaine_facteurs);
    
    for (int i = 0; i < len; i++) 
        {
        if (i % 4 == 1) 
            {
            if (chaine_facteurs[i] != 'x') 
                {
                test_facteurs = false;
                break;
                }
            } 
        else if (i % 4 == 3) 
            {
            if (chaine_facteurs[i] != ',' && i != len - 1) 
                {
                test_facteurs = false;
                break;
                }
            } 
        else 
            {
            if (!isdigit(chaine_facteurs[i])) 
                {
                test_facteurs = false;
                break;
                }
            }
        }

    if (!test_facteurs) 
        {
        fprintf(stderr, "Erreur: Les facteurs d'échantillonnage doivent être au format h1xv1,h2xv2,h3xv3.\n");
        exit(1);
        }

        int h1 = atoi(&chaine_facteurs[0]);
        int v1 = atoi(&chaine_facteurs[2]);
        int h2 = atoi(&chaine_facteurs[4]);
        int v2 = atoi(&chaine_facteurs[6]);
        int h3 = atoi(&chaine_facteurs[8]);
        int v3 = atoi(&chaine_facteurs[10]);

        if ((1 <= h1 && h1 <= 4) && (1 <= v1 && v1 <= 4) &&
            (1 <= h2 && h2 <= 4) && (1 <= v2 && v2 <= 4) &&
            (1 <= h3 && h3 <= 4) && (1 <= v3 && v3 <= 4)) 
            {
            int sum = h1 * v1 + h2 * v2 + h3 * v3;
            if (sum > 10) 
                {
                fprintf(stderr, "Erreur: La somme des produits h*v doit être inférieure ou égale à 10.\n");
                exit(1);
                }
            if (h1 % h2 != 0 || h1 % h3 != 0 || v1 % v2 != 0 || v1 % v3 != 0) 
                {
                fprintf(stderr, "Erreur: Les facteurs d'échantillonnage des chrominances doivent diviser parfaitement ceux de la luminance.\n");
                exit(1);
                }
            test_facteurs = true;
            } 
        else 
            {
            fprintf(stderr, "Erreur: Chaque facteur h ou v doit être compris entre 1 et 4.\n");
            exit(1);
            }
    
    return test_extension && test_facteurs;
    }
