#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <limits.h>
#include "list.h"
#include "objet.h"

/* ---------- callbacks ---------- */

static void del_objet(void *ptr)
{
    free(ptr);
}

static void print_objet(void *ptr)
{
    t_objet *o = (t_objet *)ptr;

    printf("  %-30s  poids unit.: %5d g  qte: %3d  ligne: %6d g\n",
        o->nom, o->poids, o->quantite, o->poids * o->quantite);
}

/* ---------- helpers ---------- */

static int  compute_total(t_list *lst)
{
    t_objet *o;
    int     total;

    total = 0;
    while (lst)
    {
        o = (t_objet *)lst->content;
        total += o->poids * o->quantite;
        lst = lst->next;
    }
    return (total);
}

static t_list   *find_node(t_list *lst, const char *name)
{
    t_objet *o;

    while (lst)
    {
        o = (t_objet *)lst->content;
        if (strcmp(o->nom, name) == 0)
            return (lst);
        lst = lst->next;
    }
    return (NULL);
}

static int  read_line(const char *prompt, char *buf, int size)
{
    int len;
    int c;

    printf("%s", prompt);
    fflush(stdout);
    if (!fgets(buf, size, stdin))
        return (0);
    len = (int)strlen(buf);
    if (len > 0 && buf[len - 1] == '\n')
        buf[len - 1] = '\0';
    else
    {
        while ((c = getchar()) != '\n' && c != EOF)
            ;
    }
    return (1);
}

static int  read_int(const char *prompt, int *out)
{
    char    buf[64];
    char    *end;
    long    val;

    if (!read_line(prompt, buf, sizeof(buf)))
        return (0);
    if (buf[0] == '\0')
        return (-1);
    errno = 0;
    val = strtol(buf, &end, 10);
    if (errno == ERANGE || *end != '\0')
        return (-1);
    if (val < 0 || val > INT_MAX)
        return (-1);
    *out = (int)val;
    return (1);
}

static int  parse_menu(const char *s)
{
    char    *end;
    long    v;

    if (!s[0])
        return (-1);
    errno = 0;
    v = strtol(s, &end, 10);
    if (errno == ERANGE || *end != '\0')
        return (-1);
    if (v < 1 || v > 4)
        return (-1);
    return ((int)v);
}

/* ---------- actions ---------- */

static void ramasser(t_list **inventaire)
{
    char    nom[NOM_LEN];
    int     poids;
    int     quantite;
    int     total;
    int     ajout;
    t_list  *existing;
    t_objet *obj;
    t_list  *node;

    if (!read_line("Nom de l'objet: ", nom, NOM_LEN))
        return ;
    if (nom[0] == '\0')
    {
        printf("Erreur: nom vide.\n");
        return ;
    }
    if (read_int("Poids unitaire (g): ", &poids) != 1 || poids <= 0)
    {
        printf("Erreur: poids invalide.\n");
        return ;
    }
    if (read_int("Quantite: ", &quantite) != 1 || quantite <= 0)
    {
        printf("Erreur: quantite invalide.\n");
        return ;
    }
    /* ---- verification AVANT toute allocation ---- */
    ajout = poids * quantite;
    total = compute_total(*inventaire);
    if (total + ajout > POIDS_MAX)
    {
        printf("Refus: depassement de %d g (actuel %d g + %d g > max %d g).\n",
            total + ajout - POIDS_MAX, total, ajout, POIDS_MAX);
        return ;
    }
    /* ---- bonus B1 : fusion ---- */
    existing = find_node(*inventaire, nom);
    if (existing)
    {
        obj = (t_objet *)existing->content;
        obj->quantite += quantite;
        printf("Objet '%s' fusionne (nouvelle qte = %d).\n", nom, obj->quantite);
        return ;
    }
    /* ---- allocation ---- */
    obj = malloc(sizeof(t_objet));
    if (!obj)
    {
        printf("Erreur memoire.\n");
        return ;
    }
    strncpy(obj->nom, nom, NOM_LEN - 1);
    obj->nom[NOM_LEN - 1] = '\0';
    obj->poids = poids;
    obj->quantite = quantite;
    node = ft_lstnew(obj);
    if (!node)
    {
        free(obj);
        printf("Erreur memoire.\n");
        return ;
    }
    ft_lstadd_back(inventaire, node);
    printf("Objet '%s' ajoute (%d x %d g).\n", nom, quantite, poids);
}

static void jeter(t_list **inventaire)
{
    char    nom[NOM_LEN];
    t_list  *prev;
    t_list  *cur;
    t_objet *o;

    if (!read_line("Nom de l'objet a jeter: ", nom, NOM_LEN))
        return ;
    if (nom[0] == '\0')
    {
        printf("Erreur: nom vide.\n");
        return ;
    }
    prev = NULL;
    cur = *inventaire;
    while (cur)
    {
        o = (t_objet *)cur->content;
        if (strcmp(o->nom, nom) == 0)
        {
            if (prev)
                prev->next = cur->next;
            else
                *inventaire = cur->next;
            ft_lstdelone(cur, del_objet);
            printf("Objet '%s' jete.\n", nom);
            return ;
        }
        prev = cur;
        cur = cur->next;
    }
    printf("Objet '%s' introuvable dans l'inventaire.\n", nom);
}

static void lister(t_list *inventaire)
{
    int     total;
    int     n;
    int     i;
    int     j;
    t_list  **arr;
    t_list  *cur;
    t_list  *tmp;
    t_objet *a;
    t_objet *b;

    if (!inventaire)
    {
        printf("Inventaire vide. Poids porte: 0 g / %d g.\n", POIDS_MAX);
        return ;
    }
    /* ---- bonus B2 : tri decroissant par poids de ligne ---- */
    n = ft_lstsize(inventaire);
    arr = malloc(sizeof(t_list *) * (size_t)n);
    if (!arr)
    {
        printf("Erreur memoire.\n");
        return ;
    }
    cur = inventaire;
    i = 0;
    while (cur)
    {
        arr[i++] = cur;
        cur = cur->next;
    }
    i = 0;
    while (i < n - 1)
    {
        j = 0;
        while (j < n - 1 - i)
        {
            a = (t_objet *)arr[j]->content;
            b = (t_objet *)arr[j + 1]->content;
            if (a->poids * a->quantite < b->poids * b->quantite)
            {
                tmp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = tmp;
            }
            j++;
        }
        i++;
    }
    printf("---- Inventaire ----\n");
    i = 0;
    while (i < n)
    {
        print_objet(arr[i]->content);
        i++;
    }
    free(arr);
    total = compute_total(inventaire);
    printf("Poids total porte: %d g / %d g\n", total, POIDS_MAX);
}

/* ---------- main ---------- */

int main(void)
{
    t_list  *inventaire;
    char    buf[64];
    int     choix;
    int     running;

    inventaire = NULL;
    running = 1;
    while (running)
    {
        printf("\n===== INVENTAIRE RPG =====\n");
        printf("1. Ramasser un objet\n");
        printf("2. Jeter un objet\n");
        printf("3. Lister l'inventaire\n");
        printf("4. Quitter\n");
        if (!read_line("Choix: ", buf, sizeof(buf)))
            break ;
        choix = parse_menu(buf);
        if (choix == 1)
            ramasser(&inventaire);
        else if (choix == 2)
            jeter(&inventaire);
        else if (choix == 3)
            lister(inventaire);
        else if (choix == 4)
            running = 0;
        else
            printf("Choix invalide, merci de saisir 1, 2, 3 ou 4.\n");
    }
    ft_lstclear(&inventaire, del_objet);
    printf("Inventaire libere. A bientot !\n");
    return (0);
}
