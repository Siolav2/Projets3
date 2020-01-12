/// \file tools.c
/// \author Vincent Dugat
/// \date summer 2019
/// \brief fonctions diverses

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include "tspstat.h"

void erreur(char *message){
/// \brief imprime à l'écran un message d'erreur (chaîne de caractères)
/// \brief le message est sensé être composé du nom du sous-programme+" : "+message d'erreur
/// \param[in] message : le message à imprimer
  printf("*** ERREUR : %s ***\n",message);
  exit(1);
}

int random_node(int min,int max){
  /// \brief crée un nombre aléatoire entre min et max
  /// \returns le nombre
  return (rand()%(max-min)) + min;
}

int * creer_tab_int(int dim){
  /// \brief crée un tableau dynamique d'entiers de dim cases
  /// \param[in] dim : la dimension du tableau voulu
  /// \returns adresse du tableau
  return (int *)malloc(dim*sizeof(int));
}

char * creer_tab_char(int dim){
   /// \brief crée un tableau dynamique de char de dim cases
  return (char *)malloc(dim*sizeof(char));
}

void affiche_tab_int(int *tab,int dim){
  printf("[");
  for (int i=0; i<dim; i++)
  {
    printf("%d ",tab[i]);
  }
  printf("]");
}

void affiche_tab_char_hex(char *tab,int dim, FILE *logfp){
  for (int i=0;i<dim;i++)
    fprintf(logfp,"%x",tab[i]);
  fprintf(logfp,"\n");
}

void affiche_tab_char(char *tab,int dim, FILE *logfp){
  for (int i=0;i<dim;i++)
    fprintf(logfp,"%c",tab[i]);
  fprintf(logfp,"\n");
}

int ** creer_mat_int(int nbRows,int nbCol){
  int ** tab = (int **)malloc(nbRows*sizeof(int *));
  for (int i=0;i<nbRows;i++)
    tab[i] = (int *) malloc(nbCol*sizeof(int));
  return tab;
}

double ** creer_mat_dist(int dimension){
  double ** tab = (double **)malloc(dimension*sizeof(double *));
  for (int i=0;i<dimension;i++)
  {
    tab[i] = (double *) malloc(dimension*sizeof(double)); 
  }
  dimension--;
  return tab;
}
/// \brief crée la matrice carrées des distances = tab dynamique triangulaire
  /// \brief chaque ligne a une case de moins que la précédente

void swap (int *tab,int ind1,int ind2)
/// \brief inverse le contenu des cases ind1 et ind2 dans tab
/// \param[in,out] tab : le tableau
/// \param[in] ind1 : premier indice
/// \param[in] ind2 : deuxième indice
/// \post *tab[ind1]=*TAB[ind2] && *tab[ind2]=*TAB[ind1]
{
    int temp;
    temp = tab[ind1];
    tab[ind1] = tab[ind2];
    tab[ind2] = temp;
}

void renverse_tab(int *tab,int prem,int der){
  /// \brief reverse le contenu du tableau entre les indices prem et der
  /// \param[in,out] tab : le tableau à modifier
  /// \param[in] prem : indice de début
  /// \param[in] der : indice de fin
  int ind;
  int dim=der-prem+1;

  for (ind=0;ind<dim/2;ind++)
    swap(tab,prem+ind,der-ind);
}




//FONCTIONS RAJOUTEES

double calcul_distance(int x1, int y1, int x2, int y2)
{
    //formule mathematique.
    return sqrt((double)((x2-x1)*(x2-x1) + (y2-y1)*(y2-y1)));
}



double calcul_distance_totale(instance_t* instance)
{
  double distance=0;
  int dim = instance->dimension-1;
  //calcul de la distance depuis le point de depart de coordonnee 0,0 vers le premier indice de tabTour.
  distance = calcul_distance(0, 0, instance->tabCoord[instance->tabTour[1]-1][0], instance->tabCoord[instance->tabTour[1]-1][1]);
  //tabTour contrairement a tabCoord contient en premier indice le point 0. Qui a pour coordonnees 0,0.
  //donc tabTour contient 1 element de plus que tabCoord d'ou le "-1" apres instance->tabTour[1] qui est la pour pas sortir du tableau tabCoord.
  

  //on ajoute à la variable distance la distance entre chaque indice contenus dans tabTour.
  for (int i=1; i<=dim; i++)
  {
    distance=distance+calcul_distance(instance->tabCoord[instance->tabTour[i]-1][0],instance->tabCoord[instance->tabTour[i]-1][1],instance->tabCoord[instance->tabTour[i+1]-1][0],instance->tabCoord[instance->tabTour[i+1]-1][1]);
  }
  //on fini par ajouter la distance entre le dernier indice de tabTour et le premier indice de tabTour qui a pour coordonnees 0,0.
  distance = distance + calcul_distance(instance->tabCoord[instance->tabTour[dim+1]-1][0], instance->tabCoord[instance->tabTour[dim+1]-1][1], 0, 0);
  return distance;
}


//copie tab1 dans tab2.
void copier_tableau(int* tab1,int* tab2, int dim)
{
  int i;
  for (i=0; i<dim; i++)
  {
    tab2[i] = tab1[i];
  }
}



//recherche la valeur val dans le tableau tab de dimension dim. Si elle est presente renvoi false sinon renvoi true.
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



//fonction utilisee pour algo_genet.
int ppv(instance_t* tsp, int ind, int* tab, int* elem)
{
  //le tableau tab ici represente le tableau reconstruction dans croisement DPX.
  double distance;
  double distance_min = 100000000000000;
  int to_return;
  int* possible = creer_tab_int(tsp->dimension);

  for (int i=0; i<tsp->dimension; i++)
  {
      possible[i]=i+1;
  }
  //le tableau possible contient tous les ppv possibles.
  if (ind==0)
  {
    for (int i=0; i<tsp->dimension; i++)
    {
      distance = calcul_distance(0,0,tsp->tabCoord[possible[i]-1][0],tsp->tabCoord[possible[i]-1][1]);
      if (distance<distance_min)
        {
          distance_min = distance;
          to_return = i+1;
        }
    }
  }
  /*si on cherche le plus proche voisin de 0, alors on calcule toutes les distances entre le point 0 et les indices differents de 0 (possile de contient pas 0)
  comme possible ne contient pas 0 on renvoi i+1.
  sinon si indice est different de 0 on passe dans le else.
  */
  else
  {
    for (int i=0; i<tsp->dimension; i++)
    {
      if (possible[i]!=ind && recherche_valeur(tab, possible[i], tsp->dimension+1) && recherche_valeur(elem, possible[i], tsp->dimension+1))
      /*si la l'indice donne par possible[i] est pas present dans le tableau que l'on veut remplir et si possible[i] n'est pas present non plus
      dans le tableau des element que l'on veut pas retourner elem (par exemple pour att10, le plus proche voisin de 4 c'est 10. si on met 10 dans elem
      alors la fonction de pourra pas return 10 car on cherche le 2 ieme ppv de 4 et pas le premier.) alors on peut faire le calcul de distance.
      */ 
      
      {
        distance = calcul_distance(tsp->tabCoord[possible[i]-1][0],tsp->tabCoord[possible[i]-1][1],tsp->tabCoord[ind-1][0],tsp->tabCoord[ind-1][1]);
        if (distance<distance_min)
        {
          distance_min = distance;
          to_return = possible[i];
        }
      }
      //on calcul pour chaque i les distance entre l'indice demande et i et on en retient l'indice qui est le plus proche de l'indice demande
      //en prenant en compte les tableaux tab et elem.
      
    }
  }
  return to_return;

}