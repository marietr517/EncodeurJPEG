#ifndef _CODAGE_BLOCS_H_
#define _CODAGE_BLOCS_H_

#include <stdint.h>
#include <../include/lecture.h>
/* structure bloc nécessaire pour l'encodage du flux dans le cas ou 
   les mcus ne sont pas de taille 8*8: il s'agit d'un tableau 8*8 */
struct bloc 
    {
    int16_t ** matrice_bloc;
    };

extern void creation_bloc(struct bloc * bloc);

extern void free_bloc(struct bloc * bloc);
/* applique sur un bloc 8*8 de la matrice _Y la dct ,la fonction zig_zag, la quantification, et apres on l'encode dans le fichier outfile*/
extern int codage_bloc_Y(struct bloc * bloc, struct mcu * mcu, int valeur_prec_DC_Y, FILE * file_out, int * indice_tampon, char ** tampon);
/* applique sur un bloc 8*8 de la matrice _CB la dct ,la fonction zig_zag, la quantification, et apres on l'encode dans le fichier outfile*/
extern int codage_bloc_CB(struct bloc * bloc, struct mcu * mcu, int valeur_prec_DC_CB, FILE * file_out, int * indice_tampon, char ** tampon);
/* applique sur un bloc 8*8 de la matrice _CR la dct ,la fonction zig_zag, la quantification, et apres on l'encode dans le fichier outfile*/
extern int codage_bloc_CR(struct bloc * bloc,struct mcu * mcu, int valeur_prec_DC_CR, FILE * file_out, int * indice_tampon, char ** tampon);

#endif
