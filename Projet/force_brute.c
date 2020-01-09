#include "force_brute.h"
#include "tspstat.h"

bool prochaine_permutation(int tab[],int longueur) {
    // Find non-increasing suffix
    if (longueur == 0)
        return false;
    int i = longueur - 1;
    while (i > 0 && tab[i - 1] >= tab[i])
        i--;
    if (i == 0)
        return false;
    
    // Find successor to pivot
    int j = longueur - 1;
    while (tab[j] <= tab[i - 1])
        j--;
    int temp = tab[i - 1];
    tab[i - 1] = tab[j];
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




void force_brute(instance_t* tsp, int* tour_max)
{   
    int dim=tsp->dimension+1;
    int* indice=creer_tab_int(dim);
    int* tour_min=creer_tab_int(dim);

    copier_tableau(tsp->tabTour, tour_max, dim);
    copier_tableau(tsp->tabTour, tour_min, dim);

    double distance;
    double distance_min;
    double distance_max;

    distance_max = calcul_distance_totale(tsp);
    distance_min = calcul_distance_totale(tsp);


    
    while (prochaine_permutation(tsp->tabTour+1, dim-1))
    {
        distance = calcul_distance_totale(tsp);
        if (distance < distance_min)
        {
            distance_min = distance;
            copier_tableau(tsp->tabTour, tour_min, dim);
        }
        if (distance > distance_max)
        {
            distance_max = distance;
            copier_tableau(tsp->tabTour, tour_max, dim);
        }
    }
    copier_tableau(tour_min, tsp->tabTour, dim);
    tsp->length = distance_min;
}
