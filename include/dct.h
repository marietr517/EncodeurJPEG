#ifndef _DCT_H_
#define _DCT_H_

#include <stdint.h>

extern void decalage(int16_t ** matrice);
    /* la fonction qui fait le offset de 128 avant la DCT */

extern void fonction_dct(int16_t ** mcu);
    /* la fonction qui calcul la DCT */
    
#endif