/**
 * @file rr.c
 * @brief Implémentation de l'algorithme d'ordonnancement Round Robin.
 *
 * @authors NOM1 (xx%), NOM2 (xx%), NOM3 (xx%)
 */

#include <stdio.h>
#include <stdlib.h>
#include "rr.h"

/* Taille maximale de la file circulaire d'indices.
 * Chaque processus peut être réenfilé au plus cpu_burst/quantum fois.
 * MAX_PROCESSES * 512 est largement suffisant pour les cas courants. */
#define RR_QUEUE_CAP (MAX_PROCESSES * 512)


/**
 * @brief Compare deux processus par leur date d'arrivée.
 * Même comparateur que FIFO, nécessaire pour le tri initial.
 */
static int compare_arrival(const void *a, const void *b)
{
    const Process *pa = (const Process *)a;
    const Process *pb = (const Process *)b;
    return pa->arrival_time - pb->arrival_time;
}


/**
 * @brief Exécute l'algorithme Round Robin sur un tableau de processus.
 *
 * Principe :
 *   - Les processus sont triés par date d'arrivée.
 *   - Une file circulaire stocke les indices des processus prêts.
 *   - À chaque tour, le premier processus de la file s'exécute pendant
 *     min(quantum, temps_restant).
 *   - Après chaque tranche, les processus nouvellement arrivés sont
 *     ajoutés à la file, PUIS le processus courant est réenfilé s'il
 *     n'est pas terminé (ordre important pour le départage).
 *   - Si la file est vide, l'horloge saute à la prochaine arrivée.
 *
 * Formules utilisées (identiques aux autres algorithmes) :
 *   - response_time   = premier start_time - arrival_time
 *   - finish_time     = current_time quand remaining == 0
 *   - turnaround_time = finish_time - arrival_time
 *   - waiting_time    = turnaround_time - cpu_burst
 *
 * @param processes  Tableau de processus à ordonnancer
 * @param n          Nombre de processus
 * @param quantum    Durée du quantum de temps (ms)
 */
void rr_schedule(Process processes[], int n, int quantum)
{
    /* ---- Étape 0 : tri par date d'arrivée ---- */
    qsort(processes, n, sizeof(Process), compare_arrival);

    /* ---- Étape 1 : tableaux de suivi locaux ---- */

    /* remaining[i] : temps CPU qu'il reste à exécuter pour le processus i */
    int remaining[MAX_PROCESSES];

    /* started[i] : 1 si le processus i a déjà obtenu le CPU au moins une fois */
    int started[MAX_PROCESSES];

    for (int i = 0; i < n; i++) {
        remaining[i] = processes[i].cpu_burst;
        started[i]   = 0;
    }

    /* ---- Étape 2 : file circulaire d'indices ---- */

    /* q[] stocke les indices (positions dans processes[]) des processus prêts.
     * On utilise head/tail modulo RR_QUEUE_CAP pour éviter un tableau infini. */
    int q[RR_QUEUE_CAP];
    int head = 0; /* prochain indice à défiler */
    int tail = 0; /* prochain emplacement libre pour enfiler */

    /* next_arrival : index du prochain processus pas encore arrivé */
    int next_arrival = 0;
    int current_time = 0;
    int done         = 0; /* nombre de processus terminés */

    /* Enfiler les processus déjà présents à t=0 */
    while (next_arrival < n &&
           processes[next_arrival].arrival_time <= current_time) {
        q[tail % RR_QUEUE_CAP] = next_arrival++;
        tail++;
    }

    /* ---- Étape 3 : boucle principale ---- */
    while (done < n) {

        /* Si la file est vide, aucun processus n'est prêt :
         * on avance l'horloge jusqu'à la prochaine arrivée. */
        if (head == tail) {
            current_time = processes[next_arrival].arrival_time;
            while (next_arrival < n &&
                   processes[next_arrival].arrival_time <= current_time) {
                q[tail % RR_QUEUE_CAP] = next_arrival++;
                tail++;
            }
        }

        /* Défiler le prochain processus prêt */
        int idx    = q[head % RR_QUEUE_CAP];
        head++;
        Process *p = &processes[idx];

        /* Enregistrer le début si c'est la première fois */
        if (!started[idx]) {
            p->start_time    = current_time;
            p->response_time = current_time - p->arrival_time;
            started[idx]     = 1;
        }

        /* Calculer la durée de cette tranche :
         * le processus s'exécute au maximum pendant quantum,
         * mais s'arrête avant s'il se termine plus tôt. */
        int run = (remaining[idx] < quantum) ? remaining[idx] : quantum;
        current_time    += run;
        remaining[idx]  -= run;

        /* Enfiler les processus arrivés PENDANT cette tranche */
        while (next_arrival < n &&
               processes[next_arrival].arrival_time <= current_time) {
            q[tail % RR_QUEUE_CAP] = next_arrival++;
            tail++;
        }

        /* Le processus est-il terminé ? */
        if (remaining[idx] == 0) {
            /* Oui : calculer les métriques finales */
            p->finish_time     = current_time;
            p->turnaround_time = p->finish_time - p->arrival_time;
            p->waiting_time    = p->turnaround_time - p->cpu_burst;
            done++;
        } else {
            /* Non : le replacer en fin de file */
            q[tail % RR_QUEUE_CAP] = idx;
            tail++;
        }
    }
}
