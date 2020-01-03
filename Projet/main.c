#include "tspstat.h"
#include "force_brute.h"
#define NBCHARMAX 100

int lire_fichier_test(char* chemin, instance_t* tsp )
{
    int depart;
    char premier_mot[NBCHARMAX];
    char x[NBCHARMAX];
    char y[NBCHARMAX];
    char ligne[NBCHARMAX];
    int j,k=0;

    FILE* fichier = fopen(chemin, "r");
    if (fichier != NULL)
    {
        while (fgets(ligne, NBCHARMAX, fichier)!=NULL)
        {
            int i=0;
            while (ligne[i]!=' ' && ligne[i]!='\n')
            {
                premier_mot[i]=ligne[i];
                i++;
            }
            premier_mot[i]='\0';
            if (strcmp(premier_mot, "NAME")==0)
            {
                depart=7;
                while (ligne[depart]!='\n' && depart<=NBCHARMAX)
                {
                    tsp->name[depart-7]=ligne[depart];
                    depart++;
                }
                tsp->name[depart-7]='\0';
            }
            if (strcmp(premier_mot, "TYPE")==0)
            {
                depart=7;
                while (ligne[depart]!='\n' && depart<=NBCHARMAX)
                {
                    tsp->type[depart-7]=ligne[depart];
                    depart++;
                }
                tsp->type[depart-7]='\0';
            }
            if (strcmp(premier_mot, "DIMENSION")==0)
            {
                depart=12;
                char char_dim[NBCHARMAX];
                while (ligne[depart]!='\n' && depart<=NBCHARMAX)
                {
                    char_dim[depart-12]=ligne[depart];
                    depart++;
                }
                char_dim[depart-12]='\0';
                tsp->dimension=atoi(char_dim);
                for (int n=0; n<tsp->dimension; n++)
                {
                    tsp->tabTour[n]=n+1;
                }
            }
            if (strcmp(premier_mot, "EDGE_WEIGHT_TYPE")==0)
            {
                depart=19;
                while (ligne[depart]!='\n' && depart<=NBCHARMAX)
                {
                    tsp->EDGE_TYPE[depart-19]=ligne[depart];
                    depart++;
                }
                tsp->EDGE_TYPE[depart-19]='\0';
            }
            if (strcmp(premier_mot, "NODE_COORD_SECTION")==0)
            {
                tsp->tabCoord=creer_mat_int(tsp->dimension,2);
                if (tsp->dimension>=1)
                {
                    for (int li=0; li<tsp->dimension; li++)
                    {
                        fgets(ligne, NBCHARMAX, fichier);
                        k=0;
                        while (ligne[k]!=' ')
                        {
                            k++;
                        }
                        k++;
                        j=k;
                        while (ligne[k]!=' ' && k<=NBCHARMAX)
                        {
                            x[k-j]=ligne[k];
                            k++;
                        }
                        tsp->tabCoord[li][0]=atoi(x);
                        k++;
                        j=k;
                        while (ligne[k]!='\n' && k<=NBCHARMAX)
                        {
                            y[k-j]=ligne[k];
                            k++;
                        }
                        tsp->tabCoord[li][1]=atoi(y);

                    }
                }
            }
        }
        fclose(fichier);
    }
    else
    {
        erreur("ERREUR: fichier introuvable");
        fclose(fichier);
        return 0;
    }
    return 1;   
}







int main(int n, char* param[])
{
    instance_t problemeTsp;
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
    int i=0;
    while (i<n)
    {
        if (strcmp(param[i],"-f")==0)
        {
            if (i+1<n)
            {
                fichiertest = param[i];
            }
            else
            {
               erreur("erreur parametre: fichier test manquant");
            }
        }
        else if (strcmp(param[i],"-t")==0)
        {
            if (i+1<n)
            {
                fichiersol = param[i];
            }
            else
            {
               erreur("erreur parametre: fichier solution manquant");
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
    }
    if (fichiertest == NULL)
    {
        erreur("ERREUR: Fichier test non spécifié");
    }
    if (forceBrute+fbmatrice+prochevoisin+random+opt2+algogenetique==0)
    {
        erreur("ERREUR: Methode de resolution non specifiee");
    }
    lire_fichier_test(fichiertest, &problemeTsp);

    if (forceBrute)
    {
        force_brute(&problemeTsp);
    }
    return 0;
}