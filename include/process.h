/**
 * @file process.h
 * @brief Définition de la structure représentant un processus.
 *
 * Ce fichier contient la structure Process utilisée par tous les
 * algorithmes d'ordonnancement du simulateur.
 *
 * @authors NOM1 (xx%), NOM2 (xx%), NOM3 (xx%)
 */

#ifndef PROCESS_H
#define PROCESS_H

/** Nombre maximum de processus supportés par le simulateur */
#define MAX_PROCESSES 64

/**
 * @brief Structure représentant un processus à ordonnancer.
 *
 * Un processus possède des cycles CPU et des cycles d'E/S parallélisées.
 * Les champs start_time, finish_time, waiting_time, turnaround_time et
 * response_time sont calculés par l'algorithme d'ordonnancement.
 */
typedef struct {
    int pid;             /**< Identifiant unique du processus */
    int arrival_time;    /**< Date d'arrivée dans la file (ms) */
    int cpu_burst;       /**< Durée du cycle CPU (ms) */
    int io_burst;        /**< Durée du cycle E/S parallélisé (ms, 0 si aucun) */

    /* Champs calculés par l'ordonnanceur */
    int start_time;      /**< Date de première exécution sur le CPU */
    int finish_time;     /**< Date de fin d'exécution */
    int waiting_time;    /**< Temps passé dans la file d'attente (ms) */
    int turnaround_time; /**< Temps de restitution : finish - arrival (ms) */
    int response_time;   /**< Temps de réponse : start - arrival (ms) */
} Process;

#endif /* PROCESS_H */
