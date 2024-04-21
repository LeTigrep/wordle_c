#include <stdio.h>
#include <string.h> //strlen
#include <sys/socket.h>
#include <arpa/inet.h> //inet_addr
#include<errno.h> //For errno - the error number
#include<netdb.h>	//hostent


// fonction pour ce connecter avec l'adresse ip 127.0.0.1 ou bien le hostname local de la machine 
int hostname_to_ip(char * hostname , char* ip)
{
	struct hostent *he;
	struct in_addr **addr_list;
	int i;
		
	if ( (he = gethostbyname( hostname ) ) == NULL) 
	{
		// obtenir les informations de l'hôte
		herror("gethostbyname");
		exit(1);
	}

	addr_list = (struct in_addr **) he->h_addr_list;
	
	for(i = 0; addr_list[i] != NULL; i++) 
	{
		//Retourner le premier 
		strcpy(ip , inet_ntoa(*addr_list[i]) );
		return 0;
	}
	
	return 1;
}

int main(int argc, char *argv[])
{
    int socket_desc;
    struct sockaddr_in server;
    char header, code_erreur;
    char essai[6];
    char ip[20];

    if (argc==1){
        printf("Erreur: la syntaxe est %s <ip>\n",argv[0]);
        exit(1);
    }
    // pour pouvoir mettre soit même l'ip lors de l'execution du client ici l'ip sera 127.0.0.1 
    if (argc==3){
        hostname_to_ip(argv[2],ip);
    }
    else strcpy(ip,argv[1]);


    // Creation de la socket
    socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_desc == -1)
    {
        perror("Creation de la socket");
        exit(1);
    }

    server.sin_addr.s_addr = inet_addr(ip);
    server.sin_family = AF_INET;
    server.sin_port = htons(4242);

    /* cette petite option qui suit est faite montrer l'attente d'un client à un serveur
    pour le voir il faut compiler et executer le client et rien faire du cote serveur pour observer l'attente du client
    avec les points qui représente le temps de chargement */
    puts("En attente que le serveur ce connect\n");
    // connection au serveur distant
    while (connect(socket_desc, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
        printf(".");    // affichage de point représentant le chargement
        fflush(stdout); // utilisé pour afficher les points directement
        sleep(2);
    }

    puts("Connecté au serveur\n");

    // envoie de données
    header = 'J';
    if (write(socket_desc, &header, 1) < 0)
    {
        perror("erreur envoie");
        exit(1);
    }

    // Recpetion d'une réponse du serveur
    if (read(socket_desc, &header, 1) < 0)
    {
        perror("erreur reception");
        exit(1);
    }
    if (header == 'C')
        printf("Je peux commencer a jouer :\n");

    int jeux = 1;
    while (jeux)
    {
        printf("Donner un mot pour jouer:");
        scanf("%s", essai);
        header = 'P';
        if (write(socket_desc, &header, 1) < 1)
            perror("erreur envoie\n");
        write(socket_desc, essai, 6);
        read(socket_desc, &header, 1);
        if (header == 'N')
        {
            read(socket_desc, essai, 6);
            printf("resultat:%s\n", essai);
        }
        if (header == 'G')
        {
            read(socket_desc, essai, 6);
            printf("Oh lala , j'ai gagné apres %s tentatives\n", essai);
            read(socket_desc, &header, 1);
            if (header == 'R')
            {
                printf("le serveur vous propose si vous voulez encore jouer (o/n):");
                char rep;
                fflush(stdin); //vider le tampon du clavier a cause de <enter>
                getchar(); 
                scanf("%c", &rep);
                if (rep == 'o')
                {
                    header = 'J';
                    write(socket_desc, &header, 1);
                    if (read(socket_desc, &header, 1) < 0)
                    {
                        perror("erreur reception");
                        exit(1);
                    }
                    if (header == 'C')
                        printf("Je peux commencer a jouer :\n");
                }
                else
                {
                    header = 'A';
                    write(socket_desc, &header, 1);
                    jeux = 0;
                }
            }
            else
                jeux = 0;
        }
        if (header == 'I')
        {
            // lire dans la suite des donnees recues la nature de l'erreur
            if (read(socket_desc, &code_erreur, 1) < 0)
            {
                perror("erreur reception");
                exit(1);
            }
            if (code_erreur == 'T')
            {
                printf("Le serveur a refuse le mot faute de taille\n");
            }
            else if (code_erreur == 'D')
            {
                printf("Le serveur a refuse le mot car il est abscent du dictionnaire\n");
            }
        }
    }

    return 0;
}
