#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include <ctype.h>

#include "../include/huffman.h"
#include "../include/htables.h"
/* On utilise un tampon: un vecteur de 8 cases pour stocker les bits de flux dès qu'il 
   collecte un octet il écrit l'octet dans le fichier outfile*/
/*On vide le tampon*/   
void ecriture_tampon(FILE * file, char * tampon) 
    {
    uint8_t valeur = 0;    
    for (int i = 0; i < 8; i++) 
        {
        if (tampon[i] == '1') 
            {
            valeur += (1 << (7 - i)); 
            }
            tampon[i] = '0';  
        }
    /* O n écrit valeur dans le fichier*/    
    fwrite(&valeur, sizeof(uint8_t), 1, file);
    if (valeur == 0xFF) 
        {
        fwrite("\x00", 1, 1, file);
        }
    /*On ajoute un zéro: byte stuffing*/
    }

/*Fonction qui prend une chaine et la stocke bit par bit dans le tampon si on collecte 
  un octet on vide le tampon et on continue a remplir */
int remplissage_tampon(FILE * file, char * tampon, char * chaine, int * indice) 
    {
    for (int i = 0; i < (int)strlen(chaine); i++) 
        {
        tampon[*indice] = chaine[i];
        (* indice)++;
        if ((*indice) == 8) 
            {
            ecriture_tampon(file, tampon);
            *indice = 0;
            }
        }
    return *indice;
    }
/* fonction qui retourne true si on a que des zéro à lire dans le vecteur à partir de cet indice  */
bool verify_end_of_block(int16_t* vecteur,int indice)
    {
    bool test=false ;   
    for (int i=indice+1; i<64;i++)
        {
        if (vecteur[i]!=0)
            {
            return(test);
            }
        }
    return(true);
    }

/* fonction qui gère tout le codage d'un vecteur donné 
elle commence avec le codage dc,puis le codage ac
 en utilisant le tampon et elle écrit octet par octet dans le fichier outfile*/
void codage(FILE * file, int16_t * vecteur, int valeur_prec, char ** pointeur_tampon, int * indice_pointeur, int j)   
    {  
    int16_t classe, indice;
    int16_t * resultat_magnitude = magnitude(vecteur[0] - valeur_prec);    
    classe = resultat_magnitude[0];
    indice = resultat_magnitude[1];
    char * chaine_codee_DC = code_huffman(classe, htables_nb_symb_per_lengths[0][j], htables_symbols[0][j]);
    /* codage huffman de la classe de magnitude dc*/
    *(indice_pointeur) = remplissage_tampon(file, *pointeur_tampon, chaine_codee_DC ,indice_pointeur);
    if (classe > 0)
        {
        char * chaine_codee_indice_DC = conversion_binaire(indice,classe);      
        /* codage de l'indice*/    
        *(indice_pointeur) = remplissage_tampon(file, *pointeur_tampon, chaine_codee_indice_DC, (indice_pointeur));
        free(chaine_codee_indice_DC);
        free(chaine_codee_DC);
        }
    free(resultat_magnitude);   
    int nb_de_zeros = 0;
    for (int i = 1; i < 64; i++)
        {
        if (vecteur[i] == 0)
            {
            if(verify_end_of_block(vecteur,i))
                {   
                char * chaine_codee_AC = code_huffman(0, htables_nb_symb_per_lengths[1][j], htables_symbols[1][j]);
                /*codage du end of block*/
                *(indice_pointeur)=remplissage_tampon(file,*pointeur_tampon,chaine_codee_AC,(indice_pointeur));
                return;
                free(chaine_codee_AC);      
                } 
            else 
                {   
                nb_de_zeros += 1;
                if (nb_de_zeros == 16)
                    {   
                    /*codage du saut de 15 zéros*/
                    char * chaine_codee_AC = code_huffman(240, htables_nb_symb_per_lengths[1][j], htables_symbols[1][j]);
                    *(indice_pointeur)=remplissage_tampon(file,*pointeur_tampon,chaine_codee_AC,(indice_pointeur));
                    nb_de_zeros = 0;
                    free(chaine_codee_AC);      
                    }
                }
            }
        else
            { 
            int poids_fort = nb_de_zeros;
            classe = magnitude(vecteur[i])[0];
            indice = magnitude(vecteur[i])[1];
            int poids_faible = classe; 
            int16_t nombre_a_coder = poids_faible + (poids_fort * 16);
            /* codage huffman de la classe de magnitude ac*/
            char * chaine_codee_AC = code_huffman(nombre_a_coder, htables_nb_symb_per_lengths[1][j], htables_symbols[1][j]);
            *(indice_pointeur) = remplissage_tampon(file, *pointeur_tampon, chaine_codee_AC, (indice_pointeur));
            if (classe > 0)
                {
                /* codage de l'indice*/  
                char * chaine_codee_indice_AC = conversion_binaire(indice,classe);
                *(indice_pointeur) = remplissage_tampon(file, *pointeur_tampon, chaine_codee_indice_AC, (indice_pointeur));
                free(chaine_codee_indice_AC);
                }
            nb_de_zeros = 0;
            free(chaine_codee_AC);
            free(magnitude(vecteur[i]));
            }
        } 
    }
