#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>

#include <stdlib.h>
#include <errno.h>

/* Port local du serveur */
#define PORT 9600

int main(int argc, char *argv[]){

/*
 * Variables du serveur
 * 
 * Déclarer ici les variables suivantes:
 * -sockfd le descripteur de socket
 * -structure d'addresse locale du serveur
 * -structure d'adresse du client
 * -taille de l'addrese du client
 */	
	//Sockfd
	int sockfd;
	//Sock du client
	int sockClient;
	char buffer[1024];
	//Structure d'addresse local du serveur et du client
	struct sockaddr_in servaddr, clientaddr;
	//Taille de l'addresse du client
	socklen_t size;

/*
 * Code du serveur
 * 
 * -Ouvrir le socket du serveur
 * -Remplir la structure d'adresse local du serveur:
 * 		-la famille d'adresse
 * 		-l'adresse IP
 * 		-le port
 * -Spécifier l'adresse locale du socket du serveur
 */
 
 	//Ouvre le socket du serveur
	if((sockfd = socket(AF_INET, SOCK_STREAM, 0))<0){
		perror("Echec de la creation du socket");
		exit(errno);
	}

	//Remplissage
	servaddr.sin_family = AF_INET; /*Famille d'adresse*/
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY); /*Il s'agit du serveur donc toutes les adresses sont acceptées*/
	servaddr.sin_port = htons(PORT); //Le port
		
	// Bind du socket avec l'adresse serveur avec gestion de l'echec
    if (bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0 ) { 
        perror("bind failed"); 
        exit(errno); 
    } 	
    
    //Fonction listen, on attend 10 connections avant acceptation
	if(listen(sockfd, 10)<0){
		perror("listen() failed");
		exit(errno);
	}
    
/*
 * Boucle générale du serveur (infinie)
 */

	while(1){
		
		/*
		 * Code de l'intérieur de la boucle
		 */
		
		//Accepte les connections entrantes
		if((sockClient = accept(sockfd, (struct sockaddr *)&clientaddr, &size))<0){
			perror("Accept failed");
			exit(errno);
		}
		
		int n; 
		while(1){
			printf("%s\n", "En attente du message");
			// Recupere le message, avec gestion de l'erreur
			if((n = read(sockClient, buffer, size)) < 0){
				perror("read() echoue");
				exit(errno);
			} else if(n == 0){
				//On sort de la boucle si on change de client
				break;
			}
		//Lecture du buffer envoyé du client jusqu'au signe \0
		buffer[n]='\0';
		printf("%s\n", "Message recu");
		printf("%s\n", buffer);
		//Réinitialise le buffer pour le message suivant
		memset(buffer, 0, sizeof(buffer));
		}
	}
	
	//Fermeture du socket
	close(sockfd);
	close(sockClient);
	
	return 0;
}
