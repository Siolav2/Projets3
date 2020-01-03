#include "force_brute.h"
#include "tspstat.h"





bool prochaine_permutation(int tab[], int longueur) {
    int i,j;
    // Find non-increasing suffix
    if (longueur == 0)
        return false;
    i = longueur - 1;
    while (i > 0 && tab[i - 1] >= tab[i])
        i--;
    if (i == 0)
        return false;
    i--;
    // Find successor to pivot
    j = longueur - 1;
    while (tab[j] <= tab[i])
        j--;
    int temp = tab[i];
    tab[i] = tab[j];
    tab[j] = temp;
    
    // Reverse suffix
    j = longueur - 1;
    while (i < j) {
        temp = tab[i];
        tab[i] = tab[j];
        tab[j] = temp;
        i++;
        j--;
    }
    return true;
}



void force_brute(instance_t* instance)
{
    
    while (prochaine_permutation(instance->tabTour,instance->dimension))
    {

    }
}
