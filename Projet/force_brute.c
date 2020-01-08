#include "force_brute.h"
#include "tspstat.h"





bool prochaine_permutation(int array[],int longueur) {
    // Find non-increasing suffix
    if (longueur == 0)
        return false;
    int i = longueur - 1;
    while (i > 0 && array[i - 1] >= array[i])
        i--;
    if (i == 0)
        return false;
    
    // Find successor to pivot
    int j = longueur - 1;
    while (array[j] <= array[i - 1])
        j--;
    int temp = array[i - 1];
    array[i - 1] = array[j];
    array[j] = temp;
    
    // Reverse suffix
    j = longueur - 1;
    while (i < j) {
        temp = array[i];
        array[i] = array[j];
        array[j] = temp;
        i++;
        j--;
    }
    return true;
}


void force_brute(instance_t* instance, int* tour_max)
{   
    int dim=instance->dimension;
    printf("forcebrute\n");
    int* indice=creer_tab_int(instance->dimension);
    for (int i=0; i< instance->dimension; i++)
    {
        indice[i]=i+1;
    }
    affiche_tab_int(indice, instance->dimension);
    int tour_min[instance->dimension];
    
    copier_tableau(instance->tabTour, tour_max, instance);
    copier_tableau(instance->tabTour, tour_min, instance);
    double distance;
    
    double distance_min;
    double distance_max;
    distance_max = calcul_distance_totale(instance);
    distance_min = calcul_distance_totale(instance);
    printf("\n%d,%d\n", distance_max, distance_min);
    while (prochaine_permutation(indice+1,dim-1))
    {
        distance = calcul_distance_totale(instance);
        if (distance < distance_min)
        {
            distance_min = distance;
            copier_tableau(instance->tabTour, tour_min, instance);
        }
        if (distance > distance_max)
        {
            distance_max = distance;
            copier_tableau(instance->tabTour, tour_max, instance);
        }
    }

    copier_tableau(tour_min, instance->tabTour, instance);
    instance->length = distance_min;
}
