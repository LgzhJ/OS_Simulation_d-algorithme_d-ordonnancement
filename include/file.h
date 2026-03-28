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

#endif /* FILE_H */
