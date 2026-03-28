/**
 * @file file.h
 * @brief Interface publique des fonctions de gestion de la file de processus.
 *
 * Ce fichier déclare toutes les fonctions permettant de manipuler une
 * file (FIFO) de processus représentée par la structure FileTQ.
 *
 * @authors NOM1 (xx%), NOM2 (xx%), NOM3 (xx%)
 */

#ifndef FILE_H
#define FILE_H

#include "process.h"

/* -----------------------------------------------------------------------
 * Initialisation et libération
 * ----------------------------------------------------------------------- */

/** @brief Initialise une file vide. */
void file_init(FileTQ *f);

/** @brief Vide la file et libère toute la mémoire allouée. */
void file_vider(FileTQ *f);

/* -----------------------------------------------------------------------
 * Consultation
 * ----------------------------------------------------------------------- */

/** @brief Indique si la file est vide (1 = vide, 0 = non vide). */
int file_est_vide(const FileTQ *f);

/** @brief Retourne le nombre de processus dans la file. */
int file_taille(const FileTQ *f);

/**
 * @brief Consulte le processus en tête sans le retirer.
 * @return 1 si succès, 0 si la file est vide.
 */
int file_tete(const FileTQ *f, Process *dest);

/* -----------------------------------------------------------------------
 * Modification
 * ----------------------------------------------------------------------- */

/** @brief Ajoute un processus en queue de file. */
void file_enfiler(FileTQ *f, Process p);

/** @brief Retire et retourne le processus en tête de file. */
void file_defiler(FileTQ *f, Process *dest);

/* -----------------------------------------------------------------------
 * Affichage
 * ----------------------------------------------------------------------- */

/** @brief Affiche le contenu de la file sur la sortie standard. */
void file_afficher(const FileTQ *f);

/* -----------------------------------------------------------------------
 * Structures de données
 * ----------------------------------------------------------------------- */

/**
 * @brief Maillon d'une file chaînée de processus.
 *
 * Chaque cellule contient un processus et un pointeur vers la cellule
 * suivante. Ce type est un pointeur : @c NULL représente la fin de chaîne.
 */
typedef struct cellule {
    Process process;         /**< Processus stocké dans ce maillon */
    struct cellule *suivant; /**< Pointeur vers le maillon suivant (NULL si fin) */
} *File;

/**
 * @brief File d'attente de processus avec accès direct en tête et en queue.
 *
 * Maintenir un pointeur sur la queue permet d'enfiler en O(1) sans
 * parcourir toute la liste. Les deux champs valent @c NULL quand la
 * file est vide.
 */
typedef struct {
    File tete;  /**< Pointeur vers le premier élément (prochain à défiler) */
    File queue; /**< Pointeur vers le dernier élément (dernier enfilé) */
} FileTQ;

#endif /* FILE_H */
