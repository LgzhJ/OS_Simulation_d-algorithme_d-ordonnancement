/**
 * @file file.c
 * @brief Implémentation des fonctions de gestion de la file de processus.
 *
 * Ce fichier fournit toutes les opérations nécessaires pour manipuler
 * une file (FIFO) de processus représentée par la structure FileTQ,
 * elle-même composée de cellules chaînées (File).
 *
 * @authors NOM1 (xx%), NOM2 (xx%), NOM3 (xx%)
 */

#include <stdio.h>
#include <stdlib.h>
#include "process.h"

/* -----------------------------------------------------------------------
 * Initialisation
 * ----------------------------------------------------------------------- */

/**
 * @brief Initialise une file vide.
 *
 * Met la tête et la queue à NULL pour indiquer que la file est vide.
 *
 * @param f  Pointeur vers la file à initialiser
 */
void file_init(FileTQ *f)
{
    f->tete  = NULL;
    f->queue = NULL;
}

/* -----------------------------------------------------------------------
 * Test de vacuité
 * ----------------------------------------------------------------------- */

/**
 * @brief Indique si la file est vide.
 *
 * @param f  Pointeur vers la file à tester
 * @return   1 si la file est vide, 0 sinon
 */
int file_est_vide(const FileTQ *f)
{
    return f->tete == NULL;
}

/* -----------------------------------------------------------------------
 * Enfiler
 * ----------------------------------------------------------------------- */

/**
 * @brief Ajoute un processus en queue de file.
 *
 * Alloue une nouvelle cellule, y copie le processus fourni et l'enchaîne
 * en fin de file. En cas d'échec d'allocation, le programme est arrêté.
 *
 * @param f  Pointeur vers la file cible
 * @param p  Processus à ajouter
 */
void file_enfiler(FileTQ *f, Process p)
{
    File nouvelle = malloc(sizeof(struct cellule));
    if (nouvelle == NULL) {
        fprintf(stderr, "Erreur : allocation mémoire impossible (file_enfiler)\n");
        exit(EXIT_FAILURE);
    }

    nouvelle->process  = p;
    nouvelle->suivant  = NULL;

    if (file_est_vide(f)) {
        /* La file était vide : la nouvelle cellule est à la fois tête et queue */
        f->tete  = nouvelle;
        f->queue = nouvelle;
    } else {
        /* On chaîne la nouvelle cellule après l'ancienne queue */
        f->queue->suivant = nouvelle;
        f->queue          = nouvelle;
    }
}

/* -----------------------------------------------------------------------
 * Défiler
 * ----------------------------------------------------------------------- */

/**
 * @brief Retire et retourne le processus en tête de file.
 *
 * La cellule en tête est supprimée et libérée. Le processus qu'elle
 * contenait est copié dans @p dest. Si la file est vide, le programme
 * est arrêté.
 *
 * @param f     Pointeur vers la file source
 * @param dest  Pointeur vers la variable qui recevra le processus retiré
 */
void file_defiler(FileTQ *f, Process *dest)
{
    if (file_est_vide(f)) {
        fprintf(stderr, "Erreur : défilement sur une file vide\n");
        exit(EXIT_FAILURE);
    }

    File a_supprimer = f->tete;
    *dest            = a_supprimer->process;

    f->tete = a_supprimer->suivant;

    /* Si la file devient vide, la queue doit aussi être remise à NULL */
    if (f->tete == NULL) {
        f->queue = NULL;
    }

    free(a_supprimer);
}

/* -----------------------------------------------------------------------
 * Consultation sans retrait
 * ----------------------------------------------------------------------- */

/**
 * @brief Consulte le processus en tête de file sans le retirer.
 *
 * @param f     Pointeur vers la file à consulter
 * @param dest  Pointeur vers la variable qui recevra une copie du processus
 * @return      1 si la consultation a réussi, 0 si la file est vide
 */
int file_tete(const FileTQ *f, Process *dest)
{
    if (file_est_vide(f)) {
        return 0;
    }
    *dest = f->tete->process;
    return 1;
}

/* -----------------------------------------------------------------------
 * Taille
 * ----------------------------------------------------------------------- */

/**
 * @brief Retourne le nombre de processus présents dans la file.
 *
 * Parcourt la liste chaînée pour compter les cellules.
 *
 * @param f  Pointeur vers la file à mesurer
 * @return   Nombre d'éléments dans la file
 */
int file_taille(const FileTQ *f)
{
    int compte  = 0;
    File courant = f->tete;

    while (courant != NULL) {
        compte++;
        courant = courant->suivant;
    }
    return compte;
}

/* -----------------------------------------------------------------------
 * Affichage
 * ----------------------------------------------------------------------- */

/**
 * @brief Affiche le contenu de la file sur la sortie standard.
 *
 * Parcourt la file de la tête vers la queue et affiche pour chaque
 * processus ses principaux attributs (pid, arrivée, burst CPU, burst E/S).
 *
 * @param f  Pointeur vers la file à afficher
 */
void file_afficher(const FileTQ *f)
{
    if (file_est_vide(f)) {
        printf("[file vide]\n");
        return;
    }

    printf("%-6s %-12s %-10s %-10s\n",
           "PID", "Arrivée(ms)", "CPU(ms)", "E/S(ms)");
    printf("--------------------------------------\n");

    File courant = f->tete;
    while (courant != NULL) {
        const Process *p = &courant->process;
        printf("%-6d %-12d %-10d %-10d\n",
               p->pid, p->arrival_time, p->cpu_burst, p->io_burst);
        courant = courant->suivant;
    }
}

/* -----------------------------------------------------------------------
 * Libération
 * ----------------------------------------------------------------------- */

/**
 * @brief Vide la file et libère toute la mémoire allouée.
 *
 * Après cet appel, la file est dans le même état qu'après file_init().
 *
 * @param f  Pointeur vers la file à vider
 */
void file_vider(FileTQ *f)
{
    File courant = f->tete;

    while (courant != NULL) {
        File suivant = courant->suivant;
        free(courant);
        courant = suivant;
    }

    f->tete  = NULL;
    f->queue = NULL;
}
