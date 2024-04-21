#include "mots_5_lettres.h"
/* fichiers de la bibliothèque standard */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/* bibliothèque standard unix */
#include <unistd.h> /* close, read, write */
#include <sys/types.h>
#include <sys/socket.h>
#include <time.h>
/* spécifique à internet */
#include <arpa/inet.h> /* inet_pton */
#include <pthread.h>
#include <netdb.h>


/* port du protocole du jeu */ 
#define PORT_WDLRP 4242

			

void *jouer(void *);

int main(int argc, char *argv[])
{


    FILE *fichier;
    fichier = fopen("journal.txt","a"); // ouverture du journal en mode ajout 
    
    /* 1. Création d'une socket tcp ipv4 */
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {
        perror("socket");
        exit(2);
    }

    /* 2. Création de la sockaddr */
    struct sockaddr_in sa = {.sin_family = AF_INET,
                             .sin_port = htons(PORT_WDLRP),
                             .sin_addr.s_addr = htonl(INADDR_ANY)};
    /* Optionnel : faire en sorte de pouvoir réutiliser l'adresse sans
     * attendre après la fin du serveur. */
    int opt = 1;
    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(int));

    /* 3. Attacher la socket d'écoute à l'adresse */
    if (bind(sock, (struct sockaddr *)&sa, sizeof(sa)) < 0)
    {
        perror("bind");
        exit(3);
    }


    /* 4. Écouter sur la socket d'écoute */
    if (listen(sock, 10) < 0)
    {
        perror("listen");
        exit(2);
    }

   while(1){

    int sock_echange = accept(sock, NULL, NULL);
    // Pour avoir l'enregistrement dans le journal de la date de connexion d'un joueur au bon format 
    time_t t = time(NULL);
    // pour la manipulation des dates 
    struct tm tm = *localtime(&t);
    fprintf(fichier,"connexion d'un joueur le  %d-%02d-%02d à %02d:%02d:%02d\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
    fflush(fichier);

    // creation du thread 
    pthread_t thread;
    int iret1 = pthread_create(&thread, NULL, jouer, &sock_echange);
   }
   
}

void *jouer(void *socket_desc)
{


    int sock_echange = *((int *)socket_desc);
    char code_message;
    int nb_tentatives = 0;
    char prop_joueur[6], a_deviner[6], a_proposer[6];

    srand(time(NULL));
    a_deviner[0] = '\0';
    int jeux = 1;

    while (jeux)
    {
        read(sock_echange, &code_message, 1);

        switch (code_message)
        {
        case 'J':
            printf("connexion reçu pour jouer au jeu\n");
            mot_alea5(a_deviner);
            nb_tentatives = 0;
            printf("mot a deviner %s\n", a_deviner);
            code_message = 'C';
            write(sock_echange, &code_message, 1);
            break;
        case 'P':
            if (strlen(a_deviner) == 0)
            {
                // erreur de mal usage du protocole applicatif
                code_message = 'E';
                write(sock_echange, &code_message, 1);
                // suivit d'un message de l'erreur
                break;
            }
            else
            {
                read(sock_echange, prop_joueur, 6);
                chaine_toupper(prop_joueur);
                if (strlen(prop_joueur) < 5)
                {
                    code_message = 'I';
                    write(sock_echange, &code_message, 1);
                    code_message = 'T';
                    write(sock_echange, &code_message, 1);
                }
                else if (!est_dans_liste_mots(prop_joueur))
                {
                    code_message = 'I';
                    write(sock_echange, &code_message, 1);
                    code_message = 'D';
                    write(sock_echange, &code_message, 1);
                }
                else
                {
                    int nb_lettres_trouves = traiter_prop(prop_joueur, a_deviner, a_proposer);
                    if (nb_lettres_trouves == 5)
                    {
                        // envoyer bravo G xxxx
                        nb_tentatives++;
                        code_message = 'G';
                        write(sock_echange, &code_message, 1);
                        // convertir le nombre de tentatives en string pour l'envoyer lisible
                        sprintf(a_proposer, "%d", nb_tentatives);
                        write(sock_echange, a_proposer, 6);
                        code_message = 'R';
                        write(sock_echange, &code_message, 1);
                        break;
                    }
                    else
                    {
                        nb_tentatives++;
                        code_message = 'N';
                        write(sock_echange, &code_message, 1);
                        printf("le serveur envoie %s\n", a_proposer);
                        write(sock_echange, a_proposer, 6);
                    }
                }
            }
            break;
        case 'A':
                jeux = 0;
                printf("Au revoir joueur\n");
                break;
        }


    }
    

    close(sock_echange);
    return 0;
}







