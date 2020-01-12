/*!
*\file algo_genet.c
*\brief Realise la methode algo genetique pour repondre au probleme.
*Ce fichier est compose de deux fonctions: croisement_DPX ainsi que mutation.
*
*\version 1.0
*\date 11/01/2020 11:40:00
*\author Alois BELIN
*/

/*!
*\mainpage Algorithme genetique.
*\content Ce programme utilise la methode d'algorithme genetique pour repondre au probleme pose.
*En ayant initialise une population de tour aleatoirement, il va permettre d'optimiser les tour grace a l'aide
*de differentes methodes comme croisement DPX et 2-optimisation.
*/


#include "algo_genet.h"
#include "randomwalk.h"
#include "opt2.h"






void croisement_DPX(instance_t* tsp, int* parent_1, int* parent_2)
{
    int dim = tsp->dimension;
    int* fille = creer_tab_int((dim+1)*2);   
    int ind = 0;
    int i=0;
    //!initialisation de la tournee fille avec les separateurs.
    while (i<dim+1)
    {
        int j=0;
        while (j<dim+1)
        {
            if (parent_1[i]==parent_2[j])
            {
                while (parent_1[i]==parent_2[j])
                {
                    if (i<dim+1 && j<dim+1)
                    {
                        fille[ind]=parent_1[i];
                        ind++;
                        i++;
                        j++;
                    }
                }
                //!on insere le segment que parent_1 et parent_2 ont en commun
                //!et on le separe du prochain avec un 100.
                //!le tableau commencera toujours par 0 et finira toujours par 100.
                fille[ind]=100;
                j+=dim+1;
                ind++;
            }
            else
            {
                j++;
            }
        }
    }


    int* reconstruction = creer_tab_int(dim+1);
    
    int* elem=creer_tab_int(dim+1);
    //!le tableau elem permettra de supprimer des possibilitees de ppv d'un indice.
    i=0;
    int k;

    //!le tableau reconstruction sera a la fin de la fonction le tableau fille reconstruit.
    while (fille[i]!=100)
        {
            reconstruction[i]=fille[i];
            i++;
        }
    int n=0;

    //!le tableau reconstruction doit commencer par un 0 on insere les element de fille jusqu'a rencontrer un 100.
    while (i<dim+1)
    {
        for (int l=0; l<dim+1; l++)
        {
            elem[l]=50;
        }
        int l=0;
        //!a chaque debut de boucle on reinitialise le tableau elem.
        int sortie=0;
        for (int j=0;j<ind-1;j++)
        {
            if (ppv(tsp,fille[n],reconstruction,elem)==fille[j] && fille[j+1]==100)
            {
                //!on cherche le plus proche voisin du debut du segment fille dans lequel on est parmis les extremitees des autres segments.
                sortie++;
                k=j;
                while (fille[k]!=100)
                {
                    reconstruction[i]=fille[k];
                    i++;
                    k--;
                    //!on insere avant tout l'extremitee et on insere les element du segments de gauche jusqu'a arriver sur un 100 dans le tableau fille.
                }
            }
        }
        if (sortie==0)
        {
            //!si le programme passe dans ce if, cela veut dire qu'il n'a pas trouve le ppv de fille[n] parmis les extremitees des autres segments.
            while (sortie==0)
            {
                //!donc tant que le ppv de fille[n] ne pas contenu dans une extremitée de segment autre que celui de n, 
                //!on ajoute le ppv de fille[n] dans le tableau elem afin de l'ignorer dans la fonction ppv et de prendre le suivant.
                elem[l]=ppv(tsp,fille[n],reconstruction,elem);
                l++;
                for (int j=0;j<ind-1;j++)
                {
                    if (ppv(tsp,fille[n],reconstruction,elem)==fille[j] && fille[j+1]==100)
                    {                        
                        sortie++;
                        k=j;
                        while (fille[k]!=100)
                        {
                            reconstruction[i]=fille[k];
                            i++;
                            k--;
                        }
                        
                    }
                }
            }
        } 
        n=k+1;
        
    }


    //!on copie le tableau reconstruction dans la tournee de l'instance tsp.

    copier_tableau(reconstruction,tsp->tabTour,dim+1);
    
}









void mutation(instance_t* tsp)
{
    printf("********AVANT MUTATION********\n\n");
    int dim = tsp->dimension;
    int** matrice_tour = creer_mat_int(20,dim+1);

    int* parent_1 = creer_tab_int(dim+1);
    int* parent_2 = creer_tab_int(dim+1);
    //!on initialise 20 individus (tournees) au hasard grace a la methode random walk.
    for (int i=0; i<20; i++)
    {
        random_walk(tsp);
        for (int j=0; j<dim+1; j++)
        {
            matrice_tour[i][j] = tsp->tabTour[j];
        }
        copier_tableau(matrice_tour[i],tsp->tabTour,dim+1);
        //!pour pouvoir calculer la distance du tour matrice_tour[i] en utilisant la fonction calcul_distance_totale,
        //!on est contraint de copier son contenu dans la tournee de l'instance tsp.
        printf("Tour %d\tlenght = %lf;\ttour : ",i, calcul_distance_totale(tsp));
        affiche_tab_int(matrice_tour[i],dim+1);
        printf("\n");
    }
    for (int generation=0; generation<20; generation++)
    {
        //!on fait maximum 20 mutation.
        int ind1 = rand()%20;
        int ind2 = rand()%20;

        copier_tableau(matrice_tour[ind1],parent_1, dim+1);
        copier_tableau(matrice_tour[ind2],parent_2, dim+1);

        //!parmis les N tournees initialisees au hasard, on en choisi 2 aleatoirement pour les copier dans parent_1 et parent_2.
        int p=1;
        int mutation = rand()%3;
        //!avec une probabilitee d'environ 0,3 (realitee 0,333333333...) on realise une mutation.
        if (mutation==p)
        {
            double distance = 0;
            double distance_max = 0;
            int ind_dmax;
            for (int i=0; i<20; i++)
            {
                //!on calcul la distance totale de chaque individu.
                copier_tableau(matrice_tour[i],tsp->tabTour,dim+1);
                distance = calcul_distance_totale(tsp);
                if (distance>distance_max)
                {
                    ind_dmax = i;
                    distance_max = distance;
                }
                //!on en garde l'indice de l'individu qui genere la pire tournee (distance la plus longue).
            }
            croisement_DPX(tsp, parent_1, parent_2);
            //!on effectue le croisement DPX des tableau parent_1 et parent_2 qui va donc nous donner un tableau fille qui va etre copie dans l'instance tsp.
            optimisation(tsp);
            //!on effectue la 2 optimisation de cette tournee et on remplace la pire tournee des individus par le resultat obtenu.
            copier_tableau(tsp->tabTour,matrice_tour[ind_dmax],dim+1);   
        }
        printf("\n");
    }
    printf("\n********APRES MUTATION********\n\n");
    //!on affiche le resultat final.
    for (int i=0; i<20; i++)
    {
        copier_tableau(matrice_tour[i],tsp->tabTour,dim+1);
        printf("Tour %d\tlenght = %lf;\ttour : ",i, calcul_distance_totale(tsp));
        affiche_tab_int(matrice_tour[i],dim+1);
        printf("\n");
    }

    


}


