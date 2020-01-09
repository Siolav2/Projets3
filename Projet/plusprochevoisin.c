#include "plusprochevoisin.h"
#include "tspstat.h"




int recherche_valeur(int* tab, int val, int dim)
{
    for (int i=0; i<dim; i++)
    {
        if (tab[i] == val)
        {
            return false;
        }
    }
    return true;
}






void plus_proche_voisin(instance_t* tsp)
{
    int dim = tsp->dimension;
    printf("%d\n",dim);
    double distance;
    double distance_min = 100000000000000;
    int ind_min;
    int* indice = creer_tab_int(dim+1);
    int* possible = creer_tab_int(dim);
    for (int i=0; i<dim; i++)
    {
        possible[i]=i+1;
    }
    //[1,2,3,4,5,6,7,8,9,10]
    indice[0] = 0;
    for (int i=0; i<dim; i++)
    {
        distance = calcul_distance(0, 0, tsp->tabCoord[possible[i]-1][0], tsp->tabCoord[possible[i]-1][1]);
        if (distance<distance_min)
        {
            distance_min = distance;
            ind_min = i+1;
        }
    }
    indice[1]=ind_min;
    int i=1;
    while (i<dim+1)
    {
        distance_min = 100000000000000;
        for (int j=0; j<dim; j++)
        {
            if(recherche_valeur(indice, possible[j], dim+1))
            {
                printf("%d %d\t%d %d\n",tsp->tabCoord[indice[i]-1][0], tsp->tabCoord[indice[i]-1][1], tsp->tabCoord[possible[j]-1][0], tsp->tabCoord[possible[j]-1][1]);
                distance = calcul_distance(tsp->tabCoord[indice[i]-1][0], tsp->tabCoord[indice[i]-1][1], tsp->tabCoord[possible[j]-1][0], tsp->tabCoord[possible[j]-1][1]);
                if (distance<distance_min)
                {
                    distance_min = distance;
                    ind_min = j+1;
                }
            }
        }
        indice[i+1] = ind_min;
        i++;
    }
    
    copier_tableau(indice, tsp->tabTour, dim+1);
    double longueur = calcul_distance_totale(tsp);
    tsp->length = longueur;


}
