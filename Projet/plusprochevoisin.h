#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>
#include <stddef.h>
#include "tspstat.h"

int recherche_valeur(int* tab, int val, int dim);
void plus_proche_voisin(instance_t* tsp);