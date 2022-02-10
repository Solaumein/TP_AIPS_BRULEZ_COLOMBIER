/* librairie standard ... */
#include <stdlib.h>
/* pour getopt */
#include <unistd.h>
/* déclaration des types de base */
#include <sys/types.h>
/* constantes relatives aux domaines, types et protocoles */
#include <sys/socket.h>
/* constantes et structures propres au domaine UNIX */
#include <sys/un.h>
/* constantes et structures propres au domaine INTERNET */
#include <netinet/in.h>
/* structures retournées par les fonctions de gestion de la base de
données du réseau */
#include <netdb.h>
/* pour les entrées/sorties */
#include <stdio.h>
/* pour la gestion des erreurs */
#include <errno.h>

void construire_message(char *message, char motif, int lg) {
	for (int i=0;i<lg;i++) {
		message[i] = motif;
	}
}

void afficher_message(char *message, int lg) {
	printf("message construit : ");
	for (int i=0;i<lg;i++){
		printf("%c", message[i]);
	}
	printf("\n");
}



void main (int argc, char **argv){
	
	/* Variable de programme */
	extern char *optarg;
	extern int optind;
	int source = -1 ; /* 0=puits, 1=source */

	/* Variable de connection */
	char *hostname= "";
	struct hostent *host;
	int sock;
	int udp = 0; /* 0=TCP (par défaut), 1=UDP */

	/* Variable de message */
	int lg = 0; /* Longeur d'un messages */
	int nb_message = -1; /* Nb de messages à envoyer ou à recevoir, par défaut : 10 en émission, infini en réception */

	/* Parcours des options */
	while ((int c = getopt(argc, argv, "pn:su")) != -1) {
		switch (c) {
			case 'p':
				if (source == 1) {
					printf("Erreur : le programme ne peut pas etre lancé en source et en puits\nusage: cmd [-p|-s][-n ##]\n");
					exit(1);
				}
				source = 0;
				break;
			case 's':
				if (source == 0) {
					printf("Erreur : le programme ne peut pas etre lancé en puits et en source\nusage: cmd [-p|-s][-n ##]\n");
					exit(1) ;
				}
				source = 1;
				break;
			case 'n':
				nb_message = atoi(optarg);
				break;
			case 'u':
				udp = 1;
				break;
			default:
				printf("Option non reconnu\nusage: cmd [-p|-s][-n ##]\n");
				break;
		}
	}

	if (udp==1){
		sock = socket(AF_INET,SOCK_DGRAM,0);
		if (sock == -1){
			printf("Echec creation sock");
		}
	}else{
		printf("TCP non implanté\n");
	}

	if(source == 1){
		hostname = argv[argc-2];
	}
	int port = atoi(argv[argc-1]);
	port = htons(port);

	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = port;

	if(hostname!=""){
		host=gethostbyname(hostname);
		if(host==NULL){
			printf("erreur sur gethostname\n");
			exit(1);
		}
		memcpy((char*)&(addr.sin_addr.s_addr),host->h_addr,host->h_length);
	}else{
		//local
	}

	if (source == -1) {
		printf("Erreur : veuillez spécifié le mode d'utilisation du programme\nusage: cmd [-p|-s][-n ##]\n");
		exit(1) ;
	}

	if (source == 1){
		printf("Mode source\n");
		if((sendto(sock,"",lg,0,(struct sockaddr *)&addr,sizeof(addr)))==-1){
			printf("Erreur envoi message\n");
		}else {
			printf("message envoye\n");
		}
	}else{
		printf("mode puits\n");
	}

	if (nb_message != -1) {
		if (source == 1){
			printf("nb de tampons à envoyer : %d\n", nb_message);
		}else {
			printf("nb de tampons à recevoir : %d\n", nb_message);
		}
	}else {
		if (source == 1) {
			nb_message = 10 ;
			printf("nb de tampons à envoyer = 10 par défaut\n");
		}else{
		printf("nb de tampons à envoyer = infini\n");
		}
	}
}
