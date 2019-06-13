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
	if((sockfd = socket(PF_INET, SOCK_DGRAM, 0))<0){
		perror("Echec de la creation du socket");
		exit(errno);
	}
 
	//Remplissage de la structure de données du serveur
	servaddr.sin_family = AF_INET; /*Famille d'adresse*/
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY); /*Il s'agit du serveur donc toutes les adresses sont acceptées*/
	servaddr.sin_port = htons(PORT); //Le port
	
	// Bind du socket avec l'adresse serveur avec gestion de l'echec
    if ( bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0 ) { 
        perror("bind failed"); 
        exit(errno); 
    } 
  
/*
 * Boucle générale du serveur (infinie)
 */

	while(1){
		/*
		 * Code de l'intérieur de la boucle
		 */
		int n; 
		printf("%s\n", "En attente du message");
		 // Recupere le message, avec gestion de l'erreur saisi au clavier
		if((n = recvfrom(sockfd, (char *)buffer, sizeof(buffer), 0, (struct sockaddr *)&clientaddr, &size)) < 0){
			perror("recvfrom() echoue");
			exit(errno);
		}
		//Lecture du buffer envoyé du client jusqu'au signe \0
		buffer[n]='\0';
		printf("%s\n", "Message recu");
		printf("%s\n", buffer);
	}
	
	//Fermeture du socket
	close(sockfd);
	
	return 0;
}
