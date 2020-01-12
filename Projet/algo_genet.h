/*!
*\file algo_genet.h
*
*\brief pour utiliser algo_genet.c grace au Makefile.
*
*\version 1.0
*\date 11/01/2020 11:30:00
*\author Alois BELIN
*/





#include "tspstat.h"
//! inclusion des fonctions generales implementees dans le fichier tspstat.c.

/*!
*\brief entrees : un pointeur vers une instance_t, et deux tableaux de dimension tsp->dimension+1
*realise le croisement DPX des tours parent_1 et parent_2
*et copie le resultat fille dans l'instance pointee tsp.
*/
void croisement_DPX(instance_t* tsp, int* parent_1, int* parent_2);

/*!
*\brief entree : un pointeur vers une instance_t
*realise une serie de mutations. Pour 20 individus (tours) initialises au hazard
*grace a la methode random walk, realise le croisement DPX de 2 tours choisis aleatoirements
*et avec une probabilite d'environ 0,3, effectue la mutation de la tournee fille obtenue (2_optimisation)
*pour ensuite remplacer la pire tournee des 20 individus par la tournee obtenue avec la mutation.
*/
void mutation(instance_t* tsp);