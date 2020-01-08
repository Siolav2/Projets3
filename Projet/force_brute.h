#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>
#include "tspstat.h"





bool prochaine_permutation(int tab[], int longueur);
void force_brute(instance_t* instance, int* tour_max);