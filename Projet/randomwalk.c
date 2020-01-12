#include "randomwalk.h"


int next_i(int i, int* indice, int dim)
{
    int prochain_indice = rand()%dim+1;
    //selection aleatoire du prochain indice
    if (recherche_valeur(indice, prochain_indice, dim+1)==false)
    {

        while(recherche_valeur(indice, prochain_indice, dim+1)==false)
        {
            //tant que cet indice est deja present dans le tableau on en choisi un autre
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
    //indice commence toujours par 0.
    printf("\n");
    int i=0;
    int prochain_indice;
    //pour tous les elements du tableau indice
    while (i<dim)
    {
        //appel de la fonction indice qui va nous renvoyer un indice choisi aleatoirement
        prochain_indice = next_i(i, indice, dim);
        indice[i+1] = prochain_indice;
        //on l'insere dans le tableau indice
        i++;
    }
    
    copier_tableau(indice, tsp->tabTour, dim+1);
    double longueur = calcul_distance_totale(tsp);
    tsp->length = longueur;

}