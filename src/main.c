/**
 * @file main.c
 * @brief Point d'entrée du simulateur d'ordonnancement.
 *
 * Usage :
 *   ./scheduler <fichier_processus.txt>
 *
 * Format du fichier d'entrée (une ligne par processus) :
 *   PID  arrival_time  cpu_burst  io_burst
 * Exemple :
 *   1  0   8  2
 *   2  1   4  0
 *   3  2   9  3
 *
 * @authors NOM1 (xx%), NOM2 (xx%), NOM3 (xx%)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "process.h"
#include "fifo.h"
#include "sjf.h"
#include "rr.h"
#include "metrics.h"

/**
 * @brief Charge les processus depuis un fichier texte.
 *
 * Chaque ligne du fichier doit contenir :
 *   PID  arrival_time  cpu_burst  io_burst
 * Les lignes commençant par '#' sont ignorées (commentaires).
 *
 * @param filename   Chemin vers le fichier d'entrée
 * @param processes  Tableau de processus à remplir
 * @return           Nombre de processus chargés, -1 en cas d'erreur
 */
static int load_processes(const char *filename, Process processes[])
{
    FILE *f = fopen(filename, "r");
    if (!f) {
        perror("Erreur ouverture fichier");
        return -1;
    }

    int n = 0;
    char line[256];

    while (fgets(line, sizeof(line), f) && n < MAX_PROCESSES) {
        /* Ignorer les commentaires et lignes vides */
        if (line[0] == '#' || line[0] == '\n') continue;

        Process p = {0};
        if (sscanf(line, "%d %d %d %d",
                   &p.pid, &p.arrival_time, &p.cpu_burst, &p.io_burst) >= 3) {
            processes[n++] = p;
        }
    }

    fclose(f);
    return n;
}

/**
 * @brief Point d'entrée principal.
 *
 * @param argc  Nombre d'arguments
 * @param argv  Tableau d'arguments (argv[1] = fichier d'entrée)
 * @return      0 si succès, 1 sinon
 */
int main(int argc, char *argv[])
{
    if (argc < 2) {
        fprintf(stderr, "Usage : %s <fichier_processus.txt>\n", argv[0]);
        return 1;
    }
    
    if (argc < 3) {
      fprintf(stderr, "Erreur choisissez un algorithme d'ordonnancement : FIFO, SJF, SRJF, RR\n");
      return 1;
    }

    Process processes[MAX_PROCESSES];
    int n = load_processes(argv[1], processes);

    if (n <= 0) {
        fprintf(stderr, "Aucun processus chargé.\n");
        return 1;
    }

    printf("Chargement de %d processus depuis '%s'.\n", n, argv[1]);
    
    char *mode_algo = argv[2];
    if(strcmp(mode_algo, "FIFO") == 0) {
        /* Lancement de l'algorithme FIFO */
        fifo_schedule(processes, n);

        /* Affichage console */
        print_metrics(processes, n, "FIFO");

        /* Sauvegarde CSV */
        save_csv(processes, n, "FIFO", "resultats_fifo.csv");
    }
    else if(strcmp(mode_algo, "SJF") == 0) {
        /* Lancement de l'algorithme SJF */
        sjf_schedule(processes, n);

        /* Affichage console */
        print_metrics(processes, n, "SJF");

        /* Sauvegarde CSV */
        save_csv(processes, n, "SJF", "resultats_sjf.csv");
    }
    else if (strcmp(mode_algo, "RR") == 0) {
        /* Le quantum est obligatoire pour RR */
        if (argc < 4) {
            fprintf(stderr, "Erreur : RR nécessite un quantum (ex: ./scheduler procs.txt RR 4)\n");
            return 1;
        }
        int quantum = atoi(argv[3]);
        if (quantum <= 0) {
            fprintf(stderr, "Erreur : le quantum doit être un entier strictement positif.\n");
            return 1;
        }
        rr_schedule(processes, n, quantum);
        print_metrics(processes, n, "RR");
        save_csv(processes, n, "RR", "resultats_rr.csv");
    }
    else {
        fprintf(stderr, "Algorithme inconnu : %s\n", mode_algo);
    }
    
    return 0;
}
