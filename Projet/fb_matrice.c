#include "fb_matrice.h"





void fb_matrice(instance_t* tsp)
{
    double** matrice_dist = creer_mat_dist(tsp->dimension);
    double distance;
    int* tournee = creer_tab_int(tsp->dimension+1);
    for (int i=0;i<tsp->dimension+1;i++)
    {
        tournee[i]=100;
    }

    for (int i=0;i<tsp->dimension;i++)
    {
        for (int j=0;j<tsp->dimension;j++)
        {
            matrice_dist[i][j]=100000;
        }
    }
    
    double distance_min = 1111111111111;

    //calcul pour le point de coordonnees 0,0.
    for (int i=1;i<tsp->dimension+1;i++)
    {
        distance = calcul_distance(0, 0, tsp->tabCoord[tsp->tabTour[i]-1][0], tsp->tabCoord[tsp->tabTour[i]-1][1]);
        matrice_dist[0][i-1] = distance;
        printf("%lf\t",matrice_dist[0][i-1]);
    }
    printf("\n");

    //calcul distance pour les autres indices.
    for (int i=1; i<tsp->dimension+1; i++)
    {
        for (int j=i+1; j<tsp->dimension+1; j++)
        {
            distance = calcul_distance(tsp->tabCoord[tsp->tabTour[i]-1][0], tsp->tabCoord[tsp->tabTour[i]-1][1], tsp->tabCoord[tsp->tabTour[j]-1][0], tsp->tabCoord[tsp->tabTour[j]-1][1]);
            matrice_dist[i][j-i-1] = distance;  
            printf("%lf\t",matrice_dist[i][j-i-1]);
        }
        printf("\n");
    }

    
    int indtournee = 0;
    tournee[indtournee]=0;
    indtournee++;
    int indmin;
    for (int j=0;j<tsp->dimension;j++)
    {
        distance = matrice_dist[0][j];

        if (distance<distance_min)
        {
            distance_min = distance;
            indmin = j+1;
        }
    }
    tournee[indtournee]=indmin;
    indtournee++;
    while (indtournee<tsp->dimension+1)
    {
        int i=indmin;
        distance_min = 10000000000;
        for (int j=0;j<tsp->dimension;j++)
        {
            if (recherche_valeur(tournee,j+1,tsp->dimension+1))
            {
                distance = matrice_dist[i][j];
                if (distance<distance_min)
                {
                    distance_min = distance;
                    indmin = j+1;
                }
            }
        }
        i=indmin;
        tournee[indtournee]=indmin;
        indtournee++;

    }
    //on choisi la meilleur distance parmis chaque ligne de la matrice.
    copier_tableau(tournee,tsp->tabTour,tsp->dimension+1);
    int lenght = calcul_distance_totale(tsp);
    tsp->length = lenght;
    tsp->matDist = matrice_dist;

}