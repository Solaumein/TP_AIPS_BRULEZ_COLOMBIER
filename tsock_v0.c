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
  int i;
  for (i=0;i<lg;i++) message[i] = motif;
}
  
void afficher_message(char *message, int lg) {
    int i;
    printf("message construit : ");
    for (i=0;i<lg;i++) printf("%c", message[i]); printf("\n");
}



void main (int argc, char **argv)
{
	int c;
	extern char *optarg;
	extern int optind;
	char *hostname= "";
	int lg = 0;
	struct hostent *host;
	int sock;
	int nb_message = -1; /* Nb de messages à envoyer ou à recevoir, par défaut : 10 en émission, infini en réception */
	int source = -1 ; /* 0=puits, 1=source */
	int udp = 0;
	while ((c = getopt(argc, argv, "pn:su")) != -1) {
		switch (c) {
		case 'p':
			if (source == 1) {
				printf("Puit\nusage: cmd [-p|-s][-n ##]\n");
				exit(1);
			}
			source = 0;
			break;

		case 's':
			if (source == 0) {
				printf("Source\nusage: cmd [-p|-s][-n ##]\n");
				exit(1) ;
			}
			source = 1;
			break;

		case 'n':
			nb_message = atoi(optarg);
			break;
		case 'u':
		  udp = 1;
		  
		  // UDP
		  
		  break;
		  

		default:
			printf("Option invalide\nusage: cmd [-p|-s][-n ##]\n");
			break;
		}
	}

	if (udp==1){
	  sock = socket(AF_INET,SOCK_DGRAM,0);
	  if (sock == -1){printf("echec creation sock");
	  

	  }
	}else {printf("C'est pas de l'udp\n");}

	if(source == 1){
	 hostname = argv[argc-2];
	}
        int port = atoi(argv[argc-1]);
	port = htons(port);
	
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = port;

	if(hostname!=""){
	  if((host=gethostbyname(hostname))==NULL){
	    printf("erreur sur gethostname\n");
	    exit(1);
	  }
	  memcpy((char*)&(addr.sin_addr.s_addr),host->h_addr,host->h_length);
	}else{
	  //local
	}
	
	if (source == -1) {
		printf("usage: cmd [-p|-s][-n ##]\n");
		exit(1) ;
	}

	if (source == 1){
		printf("on est dans le source\n");
		if((sendto(sock,"",lg,0,(struct sockaddr *)&addr,sizeof(addr)))==-1){printf("Erreur envoi message\n");}
		else {printf("message envoye\n");}
	}
	
	else
		printf("on est dans le puits\n");

	if (nb_message != -1) {
		if (source == 1)
			printf("nb de tampons à envoyer : %d\n", nb_message);
		else
			printf("nb de tampons à recevoir : %d\n", nb_message);
	} else {
		if (source == 1) {
			nb_message = 10 ;
			printf("nb de tampons à envoyer = 10 par défaut\n");
		} else
		printf("nb de tampons à envoyer = infini\n");

	}
}
