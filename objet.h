#ifndef OBJET_H
# define OBJET_H

# define POIDS_MAX 10000
# define NOM_LEN   32

typedef struct s_objet
{
    char    nom[NOM_LEN];
    int     poids;
    int     quantite;
}   t_objet;

#endif
