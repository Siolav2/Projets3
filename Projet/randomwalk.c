#include "randomwalk.h"


int next_i(int i, int* indice, int dim)
{
    int prochain_indice = rand()%dim+1;
    if (recherche_valeur(indice, prochain_indice, dim+1)==false)
    {

        while(recherche_valeur(indice, prochain_indice, dim+1)==false)
        {
            prochain_indice = rand()%dim+1;
        }
    }

    return prochain_indice;
}





void random_walk(instance_t* tsp)
{
    int dim = tsp->dimension;
    int* indice = creer_tab_int(dim+1);
    indice[0] = 0;

    int prochain_indice = rand()%dim+1;
    

    indice[1]=prochain_indice;
    printf("\n");
    int i=1;
    while (i<dim)
    {
        prochain_indice = next_i(i, indice, dim);
        indice[i+1] = prochain_indice;
        i++;
    }
    
    copier_tableau(indice, tsp->tabTour, dim+1);
    double longueur = calcul_distance_totale(tsp);
    tsp->length = longueur;

}