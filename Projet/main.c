#include "tspstat.h"
#include "force_brute.h"
#include "plusprochevoisin.h"
#include "randomwalk.h"
#include "opt2.h"
#include "algo_genet.h"
#include "fb_matrice.h"
#include <time.h>
#define NBCHARMAX 80

int csv_ecriture(char* chemin_csv, instance_t tsp, char* chemin, char* methode, double temps)
{
    FILE* fichier = fopen(chemin_csv,"r+");
    if (fichier == NULL)
    {
        // Erreur
        fclose(fichier);
        printf("Impossible a ouvrir");
        return 0;
    }
    else
    {
        fprintf(fichier,"J'ouvre le fichier : %s\n", chemin);
        fprintf(fichier,"*** Instance ***\n");
        fprintf(fichier,"Nom de l'instance ; %s\n", tsp.name);
        fprintf(fichier,"Nombre de villes (avec(0,0)) ; %d\n", tsp.dimension+1);
        fprintf(fichier,"Type %s\n", tsp.type);
        fprintf(fichier,"Point ; Abs ; Ord\n");
        fprintf(fichier,"\t0;\t0;\t0\n");
        for (int i=0; i<tsp.dimension; i++)
        {
            fprintf(fichier,"\t%d",i+1);
            for (int j=0; j<2; j++)
            {
                fprintf(fichier,";\t%d", tsp.tabCoord[i][j]);
            }
            fprintf(fichier,"\n");
        }
        fprintf(fichier,"\nMéthode ;\tLongueur ;\tTemps CPU (sec) ;\tTour\n");
        fprintf(fichier,"%s ;\t%lf ;\t%2f ;\t", methode, tsp.length, temps/1000000);
        for (int i=0;i<tsp.dimension+1;i++)
        {
            fprintf(fichier,"%d ",tsp.tabTour[i]);
        }
        fprintf(fichier,"\n");
    }
    return 1;
}



//fonction qui va tout simplement afficher proprement le resultat obtenu suivant une methode.
void afficher_resultat(instance_t tsp, char* chemin, char* methode, double temps)
{
    printf("J'ouvre le fichier : %s\n", chemin);
    printf("*** Instance ***\n");
    printf("Nom de l'instance ; %s\n", tsp.name);
    printf("Nombre de villes (avec(0,0)) ; %d\n", tsp.dimension+1);
    printf("Type %s\n", tsp.type);
    printf("Point ; Abs ; Ord\n");
    printf("\t0;\t0;\t0\n");
    for (int i=0; i<tsp.dimension; i++)
    {
        printf("\t%d",i+1);
        for (int j=0; j<2; j++)
        {
            printf(";\t%d", tsp.tabCoord[i][j]);
        }
        printf("\n");
    }
    printf("\nMéthode ;\tLongueur ;\tTemps CPU (sec) ;\tTour\n");
    printf("%s ;\t%lf ;\t%2f ;\t", methode, tsp.length, temps/1000000);
    affiche_tab_int(tsp.tabTour, tsp.dimension+1);
    printf("\n");
    
}






int lire_fichier_test(char* chemin, instance_t* Tsp)
{
    

    char premier_mot[NBCHARMAX];
    char ligne[NBCHARMAX];
    char x[20];
    char y[20];

    //on ouvre le fichier suivant le chemin donne dans le Makefile
    FILE* fichier = fopen(chemin, "r");
    if(fichier != NULL) //si on arrive pas a ouvrir le fichier
    {
        //on lit le fichier ligne par ligne
        while(fgets(ligne, NBCHARMAX, fichier) != NULL)
        {
            //premier mot va a chaque fois recuper le premier mot de la ligne
            int depart = 0;
            while(depart < NBCHARMAX && ligne[depart] != ' ' && ligne[depart] != '\n')
            {
                premier_mot[depart] = ligne[depart];
                depart++;
            }
            premier_mot[depart] = '\0';
            //on fini toujour une chaine char avec un '\0' qui represente la fin de la chaine.
            
            //suivant la chaine premier_mot on rentre dans un if.
            //le int depart va permettre de commencer l'etude de la ligne a partir du debut de la chaine que l'on doit rentrer dans l'instance Tsp.
            //strcmp permet de comparer 2 chaines de caracteres. Si strcmp==0 alors les deux chaines sont les memes.

            //on rentrera le caractere dans Tsp jusqu'a rencontrer un retour a la ligne '\n'.
            if(strcmp(premier_mot, "NAME")==0)
            {
                int depart = 7;
                while(ligne[depart] != '\n' && depart-7< NBCHARMAX)
                {
                    Tsp->name[depart-7] = ligne[depart];
                    depart++;
                }
                Tsp->name[depart] = '\0';
            }

            if (strcmp(premier_mot, "TYPE")==0)
            {
                depart=7;
                while (ligne[depart]!='\n' && depart-7<NBCHARMAX)
                {
                    Tsp->type[depart-7]=ligne[depart];
                    depart++;
                }
                Tsp->type[depart]='\0';
            }

            if(strcmp(premier_mot, "DIMENSION")==0)
            {
                char dim[NBCHARMAX];
                int depart = 12;
                while(ligne[depart] != '\n' && depart-12< NBCHARMAX)
                {
                    dim[depart-12] = ligne[depart];
                    depart++;
                }
                dim[depart] = '\0';

                Tsp->dimension = atoi(dim);
            }

            if(strcmp(premier_mot, "EDGE_WEIGHT_TYPE")==0)
            {
                int depart = 19;
                while(ligne[depart] != '\n' && depart-19< NBCHARMAX)
                {
                    Tsp->EDGE_TYPE[depart-19] = ligne[depart];
                    depart++;
                }
                Tsp->EDGE_TYPE[depart] = '\0';
            }
            if(strcmp(premier_mot, "NODE_COORD_SECTION")==0)
            {
                int indice;
                Tsp->tabCoord = creer_mat_int(Tsp->dimension,2);
                Tsp->tabTour = creer_tab_int(Tsp->dimension+1);
                for (int n=0; n<Tsp->dimension+1; n++)
                {
                    Tsp->tabTour[n]=n;
                }
                for(int i = 0; i < Tsp->dimension; i++)
                {
                    //pour chaque indice de Tsp on recupere la ligne du fichier
                    fgets(ligne, NBCHARMAX, fichier);

                    int k = 0;
                    
                    while(ligne[k] != ' ') 
                    {
                        k++;
                    }
                    //on passe l'indice des coordonnees
                    k++;
                    indice = 0;
                    while(ligne[k] != ' ') //on recupere l'abscisse
                    {
                        x[indice] = ligne[k];

                        k++;
                        indice++;
                    }
                    x[indice] = '\0';

                    Tsp->tabCoord[i][0] = atoi(x);
                    //atoi converti une chaine de caracteres en int.
                    indice = 0;
                    while(ligne[k] != '\n') //on recupere l'ordonnees
                    {
                        y[indice] = ligne[k];

                        k++;
                        indice++;
                    }
                    y[indice] = '\0';

                    Tsp->tabCoord[i][1] = atoi(y);
                }
            }
        }
        //fermeture du fichier
        fclose(fichier);
    }
    else //si le chemin ne mene à aucun fichier
    {
        erreur("Fichier introuvable");
        fclose(fichier);
        return 0;
    }

    return 1;
}







/********IMPORTANT*********
 * pour executer le programme rentrer dans la variable METHODE du Makefile les balise et taper dans le terminal "make run".
 * */
int main(int n, char* param[])
{
    //etude des balises. n defini le nombre de chaines de caracteres entre a l'execution separe par des espaces.
    //param[] defini le tableau des balises rentrees a l'execution.
    srand(time(NULL));
    instance_t Tsp;
    tour_t problemeTour;

    char* fichiertest = NULL;
    char* fichiersol = NULL;
    char* verbose = NULL;
    int modeverbose = 0;
    int csv = 0;
    int forceBrute = 0;
    int fbmatrice = 0;
    int prochevoisin = 0;
    int random = 0;
    int opt2 = 0;
    int algogenetique = 0;
    int i=1;
    //pour toutes les chaines de carateres on va comparer les balises possibles a la chaine
    while (i<n)
    {
        if (strcmp(param[i],"-f")==0)
        {
            if (i+1<n)
            {
                fichiertest = param[i+1];
                i++;
            }
            else
            {
               erreur("erreur parametre: fichier test manquant");
               //la balise -f doit obligatoirement etre suivie d'un chemin vers un fichier.
            }
        }
        else if (strcmp(param[i],"-t")==0)
        {
            if (i+1<n)
            {
                fichiersol = param[i+1];
                i++;
            }
            else
            {
               erreur("erreur parametre: fichier solution manquant");
               //la balise -s doit obligatoirement etre suivie d'un chemin vers un fichier.
            }
        }
        else if (strcmp(param[i-1],"-v")==0)
        {
            verbose=param[i];
        }
        else if (strcmp(param[i],"-v")==0)
        {
            modeverbose=1;
        }
        else if (strcmp(param[i],"-o")==0)
        {
            csv=1;
        }
        else if (strcmp(param[i],"-bf")==0)
        {
            forceBrute=1;
        }
        else if (strcmp(param[i],"-bfm")==0)
        {
            fbmatrice=1;
        }
        else if (strcmp(param[i],"-ppv")==0)
        {
            prochevoisin=1;
        }
        else if (strcmp(param[i],"-rw")==0)
        {
            random=1;
        }
        else if (strcmp(param[i],"-2opt")==0)
        {
            opt2=1;
        }
        else if (strcmp(param[i],"-ga")==0)
        {
            algogenetique=1;
        }
        else if (strcmp(param[i],"-h")==0)
        {
            printf("\nUsage :  ./tsp -f <file> [-t <tour>] [-v [<file>]] -<méthode> [-h]\n\n-f <file> : nom du fichier tsp (obligatoire)\n-t <file> : nom du fichier solution (optionnel)\n-v [<file>] : mode verbose (optionnel), écrit des informations intermédiaires à l’écran ou dans un fichier si un nom de fichier est présent.\n-o <file> : export des résultats sans un fichier csv\n-h : help, affiche ce texte\n\n<méthode> : méthodes de calcul (cumulables) :\n\n-bf : brute force,\n-bfm : brute force en utilisant la matrice des distances,\n-ppv : plus proche voisin,\n-rw : random walk,\n-2opt : 2 optimisation. Si -ppv ou -rw ne sont pas présentes on utilise -rw,\n-ga <nombre d’individus> <nombre de générations> <taux de mutation> : algorithme génétique,défaut = 20 individus, 200 générations, 0.3 mutation.\n");
        }
        i++;
    }
    if (fichiertest == NULL)
    {
        erreur("ERREUR: Fichier test non spécifié");
    }
    if (forceBrute+fbmatrice+prochevoisin+random+opt2+algogenetique==0)
    {
        //si aucune methode n'est specifiee on ne peut pas avancer.
        erreur("ERREUR: Methode de resolution non specifiee");
    }

    if (lire_fichier_test(fichiertest, &Tsp))
    {
        if (forceBrute==1)
        {
            int* tour_max = creer_tab_int(Tsp.dimension);
            int* tour_min = creer_tab_int(Tsp.dimension);

            //clock_t va permettre d'avoir le temps d'exectution.
            clock_t debut = clock();
            force_brute(&Tsp,tour_max);
            clock_t fin = clock();
            double temps = fin-debut;
            if (csv==0)
            {
                afficher_resultat(Tsp, fichiertest, "BF_meilleur", temps);
            }
            else
            {
                csv_ecriture("/Documents/Projet/csv.txt",Tsp, fichiertest, "BF_meilleur", temps);
            }
            
            

            //on garde le meilleur tour obtenu par force_brute et on calcul la distance totale du pire tour obtenu avec force_brute.
            copier_tableau(Tsp.tabTour, tour_min, Tsp.dimension+1);
            copier_tableau(tour_max, Tsp.tabTour, Tsp.dimension+1);
            double pire_distance = calcul_distance_totale(&Tsp);
            printf("BF_pire ;\t%lf ;\t%2f ;\t", pire_distance, temps);
            affiche_tab_int(tour_max, Tsp.dimension+1);
            copier_tableau(tour_min, Tsp.tabTour, Tsp.dimension+1);
            printf("\n");

        }
        if (prochevoisin==1)
        {
            clock_t debut = clock();
            plus_proche_voisin(&Tsp);
            clock_t fin = clock();
            double temps = fin-debut;
            if (csv==0)
            {
                afficher_resultat(Tsp, fichiertest, "PPV", temps);
            }
            else
            {
                csv_ecriture("/Documents/Projet/csv.txt",Tsp, fichiertest, "PPV", temps);
            }
            
        }
        if (random==1)
        {
            clock_t debut = clock();
            random_walk(&Tsp);
            clock_t fin = clock();
            double temps = fin-debut;
            if (csv==0)
            {
                afficher_resultat(Tsp, fichiertest, "Random_Walk", temps);
            }
            else
            {
                csv_ecriture("/Documents/Projet/csv.txt",Tsp, fichiertest, "Random_Walk", temps);
            }
            

        }

        if (opt2 == 1)
        {
            clock_t debut = clock();
            optimisation(&Tsp);
            clock_t fin = clock();
            double temps = fin-debut;
            if (csv==0)
            {
                afficher_resultat(Tsp, fichiertest, "2_optimisation", temps);
            }
            else
            {
                csv_ecriture("/Documents/Projet/csv.txt",Tsp, fichiertest, "2_optimisation", temps);
            }
            
        }

        if (algogenetique == 1)
        {
            clock_t debut = clock();
            mutation(&Tsp);
            clock_t fin = clock();
            double temps = fin-debut;
            

        }

        if (fbmatrice==1)
        {
            clock_t debut = clock();
            fb_matrice(&Tsp);
            clock_t fin = clock();
            double temps = fin-debut;
            if (csv==0)
            {
                afficher_resultat(Tsp, fichiertest, "force_brute_matrice", temps);
            }
            else
            {
                csv_ecriture("/Documents/Projet/csv.txt",Tsp, fichiertest, "force_brute_matrice", temps);
            }
        }
    }
    
    return 0;
}