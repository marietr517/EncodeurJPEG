#ifndef _CODAGE_AC_DC_H_
#define _CODAGE_AC_DC_H_
/* On utilise un tampon: un vecteur de 8 cases pour stocker les bits de flux dès qu'il 
   collecte un octet il écrit l'octet dans le fichier outfile*/
/*On vide le tampon on calcule la valeur et on l'écrit dans le fichier */   
extern void ecriture_tampon(FILE * file, char * tampon);
/*Fonction qui prend une chaine et la stocke bit par bit dans le tampon si on collecte 
  un octet on vide le tampon et on continue a remplir */
extern int remplissage_tampon(FILE * file, char * tampon, char * chaine, int * indice);
/* fonction qui retourne true si on a que des zéro à lire dans le vecteur à partir de cet indice  */
extern bool verify_end_of_block(int16_t * vecteur, int indice);
/* fonction qui gère tout le codage d'un vecteur donné 
elle commence avec le codage dc,puis le codage ac
 en utilisant le tampon et elle écrit octet par octet dans le fichier outfile*/
extern void codage(FILE * file, int16_t * vecteur, int valeur_prec, char ** pointeur_tampon, int * indice_pointeur, int j);

#endif
