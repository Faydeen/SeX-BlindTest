/**********************************************************************/
/**********************************************************************/
/*                                                                    */
/*                    Projet Systeme d'EXploitation                   */
/*                                                                    */
/**********************************************************************/
/*                                                                    */
/*              Develloped by : Simon Anché                           */
/*                          Sandjiv Parassouramanaick                 */
/*                                                                    */
/**********************************************************************/
/*                                                                    */
/*              Description:                                          */
/*		Fichier serveur.c comprenant la gestion des           */
/*     connections et des données envoyées aux divers clients         */
/*                                                                    */
/**********************************************************************/
/*                                                                    */
/*	Version: 0.0.1                                                */
/*	Serveur.c gere les connections multiple à l'aide des Threads  */
/*	Date : 10/10/2014                                             */
/*                                                                    */
/**********************************************************************/

#include "pse.h"
#define    CMD      "serveur"
#define    nbMaxCo  20


/***********************    Variable Globale    ***********************/
int Connection[nbMaxCo];
pthread_t Thread[nbMaxCo];

typedef struct Utilisateur{
	char pseudo[20];
	char mdp[20];
	int score;
};

Utilisateur Util[20];
strcpy(Util[0].pseudo,"Simon");
strcpy(Util[0].mdp,"Anche");
Util[0].score=0;
strcpy(Util[1].pseudo,"Sandjiv");
strcpy(Util[1].mdp,"Para");
Util[1].score=0;
strcpy(Util[2].pseudo,"Toto");
strcpy(Util[2].mdp,"Lolo");
Util[2].score=0;
/**********************************************************************/


/***********************   Threads principals   ***********************/
void* Connect(void *val){

}

void* Salon(void *val)

void* Jeu (void *val){
	int ret, mode, log, arret=0;
	char buf[LIGNE_MAX];
	
	while(arret==0){
		mode = O_WRONLY|O_APPEND|O_CREAT;
	  	ret = lireLigne(Connection[(int)val], buf);
	  	if (ret == LIGNE_MAX) {
				erreur_IO("lireLigne");
	  	}else if(ret<=0)
			continue;
		else if(strcmp(buf,"init")==0){
			printf("Initialisation du fichier log\n");			
			mode|=O_TRUNC;
			log=open("journal.log", mode, 0660);
			close(log);
		}
		else if(strcmp(buf,"fin")==0){
				printf("Arret du programme\n");
				arret=1;
			}	
		else{
			log=open("journal.log", mode, 0660);
			ecrireLigne(log,buf);
			close(log);  		
			printf("%s: reception %d octets : \"%s\"\n", CMD, ret, buf);
		}
  	}	
  	Connection[(int)val]=-1;
  	pthread_exit(NULL);
}
/**********************************************************************/

/***********************    Programme Principal    ********************/
int main(int argc, char *argv[]) {
 		


 		//Définition des variables
 	int ecoute, i, receptionlen, port;
	struct sockaddr_in adrEcoute, reception;
	
		/*************************/
	


		//test si le nombre d'argument est correct (nom du prog / adresse ip / port)
	if (argc != 2) {
   		erreur("usage: %s port\n", argv[0]);
        }
    	        /*************************/



	for(i=0;i<nbMaxCo;i++)
		Connection[i]=-1;
    


		//Definir le socket		
 	ecoute = socket (AF_INET, SOCK_STREAM, 0);
	if (ecoute ==-1) {
		perror("Socket");
		exit(EXIT_FAILURE);
  	}
  		/*************************/
  	


  		//Liaison socket port addresse
  	adrEcoute.sin_family = AF_INET;
	port = (short) atoi(argv[1]);
	adrEcoute.sin_port = htons(port);
	adrEcoute.sin_addr.s_addr = INADDR_ANY;
		//printf("%s: binding to INADDR_ANY address on port %d\n", CMD, port);
	if(bind (ecoute,  (struct sockaddr *) &adrEcoute, sizeof(adrEcoute))!=0){
		perror("Bind");
		exit(EXIT_FAILURE);
	}
		/*************************/
		



		//Boucle d'écoute
	while(1){
			//On écoute les connections
  		if (listen (ecoute, 20)==-1) {
    			perror("Bind");
			exit(EXIT_FAILURE);
  		}
  		
			//On regarde si il reste de la place	
		i=0;
		while(Connection[i]==-1 || i<nbMaxCo){
			if(Connection[i]==-1){
					//Si on a de la place, on lance la connection
				receptionlen = sizeof(reception);
				printf("%s: accepting a connection\n", CMD);
				Connection[i] = accept(ecoute, (struct sockaddr *) &reception, &receptionlen);
				if(Connection[i]==-1){
					perror("Accept");
					exit(EXIT_FAILURE);
				}
				printf("%s: adr %s, port %hu\n", CMD, stringIP(ntohl(reception.sin_addr.s_addr)), ntohs(reception.sin_port));
					//Si la connection c'est bien passé, on lance le thread
				if(pthread_create(&Thread[i],NULL,Jeu,i)!=0){
					perror("Pthread_create");
					exit(EXIT_FAILURE);
				}
				i=nbMaxCo;   //On quitte la boucle
			}
			i++;
		}
  			
  			/*************************/
	
	
	}
	
	if (close(ecoute) == -1) {
    	erreur_IO("close ecoute");
 	}

	exit(EXIT_SUCCESS);
}

