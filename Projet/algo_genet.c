#include "algo_genet.h"
#include "randomwalk.h"
#include "opt2.h"

void croisement_DPX(instance_t* tsp, int* parent_1, int* parent_2)
{
    int dim = tsp->dimension;
    int* fille = creer_tab_int((dim+1)*2);   
    int ind = 0;
    int i=0;
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
    i=0;
    int k;
    while (fille[i]!=100)
        {
            reconstruction[i]=fille[i];
            i++;
        }
    int n=0;


    while (i<dim+1)
    {
        for (int l=0; l<dim+1; l++)
        {
            elem[l]=50;
        }
        int l=0;
        int sortie=0;
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
        if (sortie==0)
        {
            while (sortie==0)
            {
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



    copier_tableau(reconstruction,tsp->tabTour,dim+1);
    
}









void mutation(instance_t* tsp)
{
    printf("********AVANT MUTATION********\n\n");
    int dim = tsp->dimension;
    int** matrice_tour = creer_mat_int(20,dim+1);

    int* parent_1 = creer_tab_int(dim+1);
    int* parent_2 = creer_tab_int(dim+1);

    for (int i=0; i<20; i++)
    {
        random_walk(tsp);
        for (int j=0; j<dim+1; j++)
        {
            matrice_tour[i][j] = tsp->tabTour[j];
        }
        copier_tableau(matrice_tour[i],tsp->tabTour,dim+1);
        printf("Tour %d\tlenght = %lf;\ttour : ",i, calcul_distance_totale(tsp));
        affiche_tab_int(matrice_tour[i],dim+1);
        printf("\n");
    }
    for (int generation=0; generation<20; generation++)
    {
        int ind1 = rand()%20;
        int ind2 = rand()%20;

        copier_tableau(matrice_tour[ind1],parent_1, dim+1);
        copier_tableau(matrice_tour[ind2],parent_2, dim+1);

        int p=1;
        int mutation = rand()%3;

        if (mutation==p)
        {
            double distance = 0;
            double distance_max = 0;
            int ind_dmax;
            for (int i=0; i<20; i++)
            {
                copier_tableau(matrice_tour[i],tsp->tabTour,dim+1);
                distance = calcul_distance_totale(tsp);
                if (distance>distance_max)
                {
                    ind_dmax = i;
                    distance_max = distance;
                }
            }
            croisement_DPX(tsp, parent_1, parent_2);
            optimisation(tsp);
            copier_tableau(tsp->tabTour,matrice_tour[ind_dmax],dim+1);   
        }
        printf("\n");
    }
    printf("\n********APRES MUTATION********\n\n");
    for (int i=0; i<20; i++)
    {
        copier_tableau(matrice_tour[i],tsp->tabTour,dim+1);
        printf("Tour %d\tlenght = %lf;\ttour : ",i, calcul_distance_totale(tsp));
        affiche_tab_int(matrice_tour[i],dim+1);
        printf("\n");
    }

    


}


