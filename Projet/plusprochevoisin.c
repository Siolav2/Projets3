#include "plusprochevoisin.h"











void plus_proche_voisin(instance_t* tsp)
{
    int dim = tsp->dimension;
    double distance;
    double distance_min = 100000000000000;
    //initialisation d'un grande distance_min
    int ind_min;
    int* indice = creer_tab_int(dim+1);
    int* possible = creer_tab_int(dim);
    for (int i=0; i<dim; i++)
    {
        possible[i]=i+1;
    }
    //initialisation du tableau des possibilitees a [1,2,3,4,5,6,7,8,9,10]
    indice[0] = 0;
    //une tournee commence toujours par 0. Donc on commence par chercher le plus proche voisin de l'indice 0 qui a pour coordonnees 0,0.
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
    //on par ensuite du plus proche voisin de 0 (ppv) et on insere le ppv du ppv de 0 à indice et ainsi de suite.
    while (i<dim+1)
    {
        distance_min = 100000000000000;
        for (int j=0; j<dim; j++)
        {
            //si l'indice que l'on veut tester est deja dans le tableau indice on n'effectue aucun calcul.
            if(recherche_valeur(indice, possible[j], dim+1))
            {
                distance = calcul_distance(tsp->tabCoord[indice[i]-1][0], tsp->tabCoord[indice[i]-1][1], tsp->tabCoord[possible[j]-1][0], tsp->tabCoord[possible[j]-1][1]);
                if (distance<distance_min)
                {
                    distance_min = distance;
                    ind_min = j+1;
                }
            }
            //on retient pour chaque indice son ppv et on l'insere dans le tableau indice.
        }
        indice[i+1] = ind_min;
        i++;
    }
    
    copier_tableau(indice, tsp->tabTour, dim+1);
    double longueur = calcul_distance_totale(tsp);
    tsp->length = longueur;


}
