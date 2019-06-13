	/*
	 * Code du client 
	 */
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>

#include <stdlib.h>
#include <errno.h>

#define SIZE 100
#define PORT 9600

int main(int argc, char *argv[]){
	/*
	 * Variables du client
	 */
	//Sock le descripteur de socket
	int sock; 
	//Structure d'adresse du serveur
    struct sockaddr_in servaddr;
    //Pointeur vers la structure descriptive de machine (hostent)
    struct hostent *hostinfo = NULL;
    //Zone de mémoire destinée à accueillir la chaine entrée au clavier	
    char buffer[SIZE];

    //String uniquement pour les tests
    //char *message = "Message du client"; 
    
    //Ouverture du socket du client
	if((sock = socket(AF_INET, SOCK_DGRAM, 0))<0){
		perror("socket()");
		exit(errno);
	}
	
	//Récupération de l'addresse IP du serveur à partir de son nom donné en ligne de commande
	hostinfo = gethostbyname(argv[1]);
	//On s'abstient d'utiliser bind()
	
	//Remplissage de la structure d'addresse du client
	servaddr.sin_addr = *(struct in_addr*)hostinfo->h_addr_list[0];
	servaddr.sin_port = htons(PORT);
	servaddr.sin_family = AF_INET;
		
	//boucle infinie pour ne pas rééxécuter
	while(1){

		//Lire une ligne de l'entrée standard
		printf("Saisir message\n");
		scanf("%s", buffer);
		
		//Envoie la chaine lue vers le serveur
		if(sendto(sock, (const char *)buffer, strlen(buffer), 0, (struct sockaddr *)&servaddr, sizeof(servaddr))< 0){
			perror("sendto()");
			exit(errno);
		}
		//Confirmation envoi réussit
		printf("Message envoyé\n");
		//Réinitialise le buffer pour la réponse : quitter oui ou non?
		memset(buffer, 0, sizeof(buffer));
		
		printf("voulez-vous quitter? Saisir y pour oui, autre chose pour continuer\n");
	
		scanf("%s", buffer);		
		if(buffer[0] == 'y') break;
		//Réinitialise le buffer pour le message suivant si on ne quitte pas le client
		memset(buffer, 0, sizeof(buffer));
	}
	//fermeture du socket
	close(sock);
	return 0;
}
