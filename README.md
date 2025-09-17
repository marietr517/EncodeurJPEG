# Projet C - Encodeur JPEG

Ce projet consiste à créer un encodeur JPEG simplifié en langage C. Il lit une image au format **PPM (Portable PixMap)** et génère un fichier compressé en suivant les grandes étapes d’un encodage JPEG, incluant la transformation couleur, la transformation en cosinus discrète (DCT), la quantification et l'encodage entropique.

## Objectifs pédagogiques

- Manipuler les bases de la compression d’images (JPEG) ;
- Structurer un projet C modulaire ;
- Travailler avec des formats binaires (PPM, JPEG simplifié) ;
- Appliquer des transformations mathématiques (DCT, quantification) ;

---

## Organisation des fichiers

| Fichier              | Rôle                                                                 |
|----------------------|----------------------------------------------------------------------|
| `ppm2jpeg.c`         | Point d’entrée du programme (`main`). Gère l'assemblage des modules. |
| `lecture.c`          | Lecture et chargement des images PPM.                                |
| `dct.c`              | Application de la Transformée en Cosinus Discrète (DCT).             |
| `zigzag_quant.c`     | Réalise le parcours en zigzag et la quantification.                  |
| `codage_blocs.c`     | Découpe l’image en blocs 8x8 pour traitement JPEG.                   |
| `codage_ac_dc.c`     | Codage différentiel DC + codage AC selon JPEG.                       |
| `htables.c`          | Génération des tables de Huffman (DC/AC) et quantification.          |
| `huffman.c`          | Codage des données avec les arbres de Huffman.                       |
| `jpeg.c`             | Création de l’en-tête JPEG + structure de fichier.                   |
| `commande.c`         | Gestion des paramètres ligne de commande (si utilisé).               |

---

## Compilation

Assurez-vous d’avoir un environnement C avec GCC installé.

```bash
make
```

Cela génère un exécutable nommé `ppm2jpeg` à la racine du projet.

---

## Utilisation

```bash
./ppm2jpeg image.ppm
```

Cela génère un fichier `image.jpeg` compressé.

---

## Nettoyage

```bash
make clean
```

Supprime l’exécutable et les fichiers objets.

---

