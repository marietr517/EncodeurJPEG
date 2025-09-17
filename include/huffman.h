#ifndef _HUFFMAN_H_
#define _HUFFMAN_H_

#include <stdint.h>
/* fonction qui prend un nombre et retourne la classe et l'indice de magnitude auxquels elle appartient*/
extern int16_t * magnitude(int16_t nombre);
/* fonction qui prend un entier n et retourne sa chaine de conversion binaire sur x bits */
extern char * conversion_binaire(int32_t nombre, int taille);
/* fonction qui pour un entier donné construit à partir de tableau symboles et longeur_per_symbole  la chaine du code huffman correspondante */
extern char * code_huffman(uint8_t nombre, uint8_t * htables_lengths, uint8_t * htables_symbols);

#endif
