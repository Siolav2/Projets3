#include "force_brute.h"

bool prochaine_permutation(int tab[],int longueur) {
    // on cherche le plus long suffix du tableau tab qui est deccroissant.
    if (longueur == 0)
        return false;
    int i = longueur - 1;
    while (i > 0 && tab[i - 1] >= tab[i])
        i--;
    if (i == 0)
        return false;
    //si tout le tableau est deccroissant alors il ne peut pas y avoir de prochaine permutation.
    
    // on cherche le premier element en partant de la droite qui est superieur a l'element juste avant le suffix deccroissant.
    int j = longueur - 1;
    while (tab[j] <= tab[i - 1])
        j--;
    int temp = tab[i - 1];
    tab[i - 1] = tab[j];
    tab[j] = temp;
    //on effectue un swap entre ces deux elements.
    //et on met le suffix deccroissant dans l'ordre croissant.
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

    //initialisation des tableau tour_max et tour_min
    double distance;
    double distance_min;
    double distance_max;

    distance_max = calcul_distance_totale(tsp);
    distance_min = calcul_distance_totale(tsp);
    //calcul de leur distance.

    //tant qu'un prochaine permutation est possible
    while (prochaine_permutation(tsp->tabTour+1, dim-1))
    {   
        //realisation prochaine permutation
        //calcul de la distance de la tournee
        distance = calcul_distance_totale(tsp);
        //si cette distance est plus petite que la distance_min on effectue le changement 
        //si cette distance est plus grande que la distance_max on effectue le changement
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
