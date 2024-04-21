#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

/*#include "mots_5_lettres.h"*/

/* Met toutes les lettres minuscules présentes dans ch en majuscule (les autres
 * caractères sont inchangés).
 * Précondition : ch contient un caractère nul qui sert de délimiteur de fin */
void chaine_toupper(char *ch);

/* Vider le tampon de l'entrée standard */
void vider_tampon();

/* Met dans la chaine prop (tableau d'au moins 6 char) un mot de 5 lettres saisi
 * par l'utilisateur, si besoin mis en majuscule, et terminé par un '\0'.
 * Redemande la saisie tant que
 * - le mot de l'utilisateur a moins de 5 lettres ou
 * - n'est pas dans la liste de mots.
 * Si l'utilisateur saisit un mot de plus de 5 lettres, seules les 5 premières
 * sont prises en compte.
 */
void saisir_prop(char *prop);

/* Pour chaque lettre de prop_joueur, affiche :
 * * cette lettre en majuscule si elle figure à la même position dans a_deviner
 * * cette lettre en minuscule si elle fait partie de a_deviner mais à une autre
 *   position
 * * le caractère _ sinon
 * L'affichage se termine par un saut de ligne.
 * Retourne le nombre de lettres trouvées à la bonne position
 * Préconditions : prop_joueur et a_deviner contiennent au moins 5 caractères
 *                 qui sont tous des lettres majuscules */
int traiter_prop(const char *prop_joueur, const char *a_deviner, char *a_proposer);

/*->1*/int main1()
{
    int nb_tentatives = 0;
    char prop_joueur[6], a_deviner[6];

    srand(time(NULL));
    mot_alea5(a_deviner);

    // do {
    // nb_tentatives = nb_tentatives + 1;
    // saisir_prop(prop_joueur);
    // } while (traiter_prop(prop_joueur, a_deviner) < 5);

    printf("Bravo, mot trouvé en %d tentatives\n", nb_tentatives);

    return 0;
}

void chaine_toupper(char *ch)
{
    int i;
    for (i = 0; ch[i] != '\0'; i = i + 1)
        ch[i] = toupper(ch[i]);
}

int traiter_prop(const char *prop_joueur, const char *a_deviner, char *a_proposer)
{
    int i, nb_lettres_trouvees = 0;
    int cpt = 0;
    for (i = 0; i < 5; i = i + 1)
    {
        if (prop_joueur[i] == a_deviner[i])
        {
            a_proposer[cpt++] = prop_joueur[i];
            nb_lettres_trouvees = nb_lettres_trouvees + 1;
        }
        else if (strchr(a_deviner, prop_joueur[i]))
        {
            a_proposer[cpt++] = tolower(prop_joueur[i]);
        }
        else
        {
            a_proposer[cpt++] = '_';
        }
    }
    a_proposer[cpt] = '\0';
    return nb_lettres_trouvees;
}

void vider_tampon()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
}

void saisir_prop(char *prop)
{
    int correct = 0;
    while (!correct)
    {
        printf("Votre proposition : ");
        if (scanf("%5s", prop) == EOF)
        {
            printf("\n");
            exit(1);
        }
        if (strlen(prop) < 5)
        {
            fprintf(stderr, "Mot trop court, entrer un mot de 5 lettres.\n");
        }
        else
        {
            chaine_toupper(prop);
            if (!est_dans_liste_mots(prop))
                fprintf(stderr, "Ce mot n'est pas dans la liste de mots.\n");
            else
                correct = 1;
        }
        vider_tampon();
    }
}
