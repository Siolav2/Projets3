#include "opt2.h"




int croisement(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4)
{
    //on cherche l'equation reduite de la droite qui passe par le premier segment a
    double m1 = (double)(y2-y1)/(double)(x2-x1);
    double p1 = (double)y1 - m1*(double)x1;
    double m2 = (double)(y4-y3)/(double)(x4-x3);
    double p2 = (double)y3 - m2*(double)x3;

    /*  y1 = m1*x1 + p1
        y2 = m2*x2 + p2
        m1*x +p1 = m2*x + p2
        x*(m1-m2) = p2 - p1
        x = (p2 - p1) / (m1 - m2)
    */
    
    if (m1 != m2)
    {
        //si les droite ne sont pas paralleles cela veut dire qu'elles se croisent quelque part
        double i = (double)(p2-p1)/(double)(m1-m2);
        double j = m1*i+p1;
        //calcul des coordonnees du point d'intersection des deux droites

        double distance_a1 = calcul_distance(x1, y1, i, j);
        double distance_a2 = calcul_distance(x2, y2, i, j);
        double distance_1_2 = calcul_distance(x2, y2, x1, y1);
        double distance_b1 = calcul_distance(x3, y3, i, j);
        double distance_b2 = calcul_distance(x4, y4, i, j);
        double distance_3_4 = calcul_distance(x3, y3, x4, y4);
        //calcul de plusieurs distances pour savoir si le point d'intersection se situe sur les deux segments

        if (distance_a1<=distance_1_2 && distance_a2<=distance_1_2 && distance_b1<=distance_3_4 && distance_b2<=distance_3_4)
        {
            //si le point d'intersection se situe sur les deux segments alors les segments se croisent
            return 1;
        }
    }
    return 0;
}

void optimisation(instance_t* tsp)
{
    int dim = tsp->dimension;
    //{0,1,2,3,4,5,6,7,8,9,10}
    for (int i=1; i<dim; i++)
    {
        for (int j=1; j<dim; j++)
        {
            if (i+1!=j && i!=j+1 && i!=j)
            {
                int x1 = tsp->tabCoord[tsp->tabTour[i]-1][0], y1 = tsp->tabCoord[tsp->tabTour[i]-1][1];
                int x2 = tsp->tabCoord[tsp->tabTour[i+1]-1][0], y2 = tsp->tabCoord[tsp->tabTour[i+1]-1][1];
                int x3 = tsp->tabCoord[tsp->tabTour[j]-1][0], y3 = tsp->tabCoord[tsp->tabTour[j]-1][1];
                int x4 = tsp->tabCoord[tsp->tabTour[j+1]-1][0], y4 = tsp->tabCoord[tsp->tabTour[j+1]-1][1];
                //on teste pour 2 segments avec aucun indice en commun si ils se croisent.
                if (croisement(x1, y1, x2, y2, x3, y3, x4, y4))
                {
                    //si c'est le cas on inverse le sens du tableau entre le premier indice du premier segment et le deuxieme indice du deuxieme segement.
                    renverse_tab(tsp->tabTour, i+1,j);
                }
            }
        }
    }
    tsp->length = calcul_distance_totale(tsp);





    
}