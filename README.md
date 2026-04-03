# OS_Simulation_d’Algorithme_d’Ordonnancement

Simulateur en C d'algorithmes d’ordonnancement de processus. Produit des métriques en console et un export CSV.

## Fonctionnalités
- Implémente les algorithmes : **FIFO**, **SJF**, **SJRF**, **RR**, avec possibilité d’ajouter facilement d’autres algorithmes.
- Entrée : jeux de processus avec cycles CPU et cycles d’E/S parallélisables (unité : ms).
- Sortie : textuelle (CSV compatible tableur) ou graphique, avec indicateurs de performance :
  - Temps d’attente moyen / par processus
  - Temps de restitution moyen / par processus
  - Temps de réponse moyen / par processus
  - Taux d’occupation du CPU

## Extensions possibles
- Interface graphique pour la restitution des résultats
- Génération automatique de graphiques via Office / LibreOffice
- Gestion des E/S non parallélisables

## Compilation
1. Compiler le projet :
```bash
make
```
2. Compile et lance les 4 algos sur le fichier sur le fichier de test :
```bash
make run
```
3. Supprimer les .o, le binaire et les CSV :
```bash
make clean
```
4. Générer la documentation Doxygen :
```bash
make doc
```

| Paramètre | Description |
|-----------|-------------|
| `fichier.txt` | Fichier de processus (voir format ci-dessous) |
| `ALGO` | `FIFO`, `SJF`, `RR` ou `SRJF` |
| `quantum` | Durée du quantum en ms — obligatoire pour `RR` |

Lancer le simulateur sur un algo précis : `./scheduler <fichier_processus.txt> <ALGO_D'ORDONNANCEMENT ex: FIFO, SJF, SRJF ou RR> <quantum pour RR>` 
Les résultats sont exportés dans un fichier CSV et afficher dans le terminal

**Exemples :**
```bash
./scheduler data/test_processus.txt FIFO
./scheduler data/test_processus.txt SJF
./scheduler data/test_processus.txt RR 4
./scheduler data/test_processus.txt SRJF
```

## Format du fichier d'entrée
Une ligne par processus. Les lignes vides et celles commençant par `#` sont ignorées.
```
PID  arrival_time  cpu0 [io0  cpu1 [io1  cpu2 ...]]
```
Les valeurs après `arrival_time` alternent cycles CPU et cycles E/S. La première valeur est toujours un CPU.
```
# PID  arr  cpu   io
1      0    8     2
2      1    4     0
3      2    9     3

# Multi-burst : cpu0  io0  cpu1  io1  cpu2
6      0    6     3    4    2    2
```
