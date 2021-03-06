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
/*    Fichier client.c toute la gestion de l'affichage coté           */
/*                                                                    */
/**********************************************************************/
/*                                                                    */
/*	Version: 0.0.2                                                    */
/*	Client.c ecrit dans un fichier log et envoie au serveur toute     */
/*              ce que l'utilisateur ecrit                            */
/*	Date : 10/10/2014                                                 */
/*                                                                    */
/**********************************************************************/
/**********************************************************************/
/*                                                                    */
/*              compiler avec:                                        */
/*    gcc -g -I. -I../include -Wall client.c  -L../lib -lpse -lm -pthread -o client  $(pkg-config --cflags --libs gtk+-3.0 gmodule-2.0)  */
/*                                                                    */
/**********************************************************************/
/**********************************************************************/


#include "pse.h"
#include <gtk/gtk.h>
#define CMD "client"

	//Déclaration des variables

		//Variables gérant les "objets" graphique
typedef enum {Salon,Disney,Pop}Salons;
typedef struct _my_widgets{

	GtkWidget *Chat;
	GtkWidget *Texte;
	GtkTextBuffer *buffer;
	GtkTextIter iter;
	
}my_widgets;

typedef struct {

	GtkWidget *window1;
	GtkNotebook *notebook;
	my_widgets *Salon;
	my_widgets *Disney;
	my_widgets *Pop;
	
	Salons salAct;
	
	#define readTEXT(xx) \
   		gtk_entry_get_text(GTK_ENTRY(app->xx->Texte))
   	#define getBUFFER(xx) \
   		app->xx->buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (app->xx->Chat));	
   	#define getITER(xx) \
   		gtk_text_buffer_get_iter_at_offset(app->xx->buffer, &(app->xx->iter),gtk_text_buffer_get_char_count (app->xx->buffer))
   		
} my_app;
	
my_app *app; 


		//Varables gérant la connection au serveur
pthread_t Thread;
int sd, ret; 
struct sockaddr_in *sa;
char buf[LIGNE_MAX];
	/**********************************/


	//Prototype des fonctions
void connection(int argc,char *argv[]);
int GUI(int argc,char *argv[]);
void* Chat_Update (void *val);
void on_window_destroy ();
void Send_Clicked(GtkEntry *text_entry);
	/**********************************/


	//Fonction Principale
int main(int argc, char *argv[]) {
      
  connection(argc,argv);
  GUI(argc,argv);
  
  if (close(sd) == -1) {
	erreur_IO("close");
  } 
  
  exit(EXIT_SUCCESS);
}
	/**********************************/


	//Gere la connection au serveur
void connection(int argc,char *argv[]){
	      
  if (argc != 3) {   //On doit avoir ./client addrIp port
		erreur("usage: %s machine port\n", argv[0]);
  }
  
  	//Creation du Socket
  printf("%s: creating a socket\n", CMD);
  sd = socket (AF_INET, SOCK_STREAM, 0);
  if (sd < 0) {
		erreur_IO("socket");
  }


  printf("%s: DNS resolving for %s, port %s\n", CMD, argv[1], argv[2]);
  sa = resolv(argv[1], argv[2]);
  if (sa == NULL) {
	erreur("adresse %s port %s inconnus\n", argv[1], argv[2]);
  }
  printf("%s: adr %s, port %hu\n", CMD,
     stringIP(ntohl(sa->sin_addr.s_addr)),
	 ntohs(sa->sin_port));

  	/* connexion sur site distant */
  printf("%s: connecting the socket\n", CMD);
  ret = connect(sd, (struct sockaddr *) sa, sizeof(struct sockaddr_in));
  if (ret < 0) {
	erreur_IO("Connect");
  }

  freeResolv();
  
}
	/**********************************/


	//Gere l'interface graphique
int GUI(int argc,char *argv[]){

  GtkBuilder *builder;
  
  app= g_slice_new(my_app);
  app->Salon=g_slice_new(my_widgets);
  app->Disney=g_slice_new(my_widgets);
  app->Pop=g_slice_new(my_widgets);
  
  GError *error=NULL;
  
	
  gtk_init(&argc,&argv);
  
  	//On récupere le builder lié à notre .glade
  builder=gtk_builder_new();
  if(!gtk_builder_add_from_file(builder,"UIBT.glade",&error)){
  	g_warning("%s",error->message);
  	g_free(error);
  	return(1);  
  }
  
  
  #define appGET(xx,yy,zz) \
		app->yy->xx=GTK_WIDGET(gtk_builder_get_object(builder,#zz))
	app->window1=GTK_WIDGET(gtk_builder_get_object(builder,"window1"));
	app->notebook=(GtkNotebook*)GTK_WIDGET(gtk_builder_get_object(builder,"notebook"));
	appGET(Chat,Salon,Chat_Salon);
	appGET(Texte,Salon,Texte_Salon);
	appGET(Chat,Disney,Chat_Disney);
	appGET(Texte,Disney,Texte_Disney);
	appGET(Chat,Pop,Chat_Pop);
	appGET(Texte,Pop,Texte_Pop);
  
  gtk_window_set_position(GTK_WINDOW(app->window1), GTK_WIN_POS_CENTER);
  gtk_window_set_default_size(GTK_WINDOW(app->window1), 800, 200);
  gtk_builder_connect_signals(builder,app);
  
  g_object_unref(G_OBJECT(builder));
  
  gtk_widget_show(app->window1);
  
  if(pthread_create(&Thread,NULL,Chat_Update,NULL)!=0){
					perror("Pthread_create");
					exit(EXIT_FAILURE);
  }
  
  gtk_main();
  g_slice_free(my_widgets, app->Salon);
  g_slice_free(my_widgets, app->Disney);
  g_slice_free(my_widgets, app->Pop);
  g_slice_free(my_app, app);
  
  return 0;
}
	/**********************************/

	// Quitter la fenetre 
void on_window_destroy (){
  gtk_main_quit ();
}
	/**********************************/


void Send_Clicked(GtkEntry *text_entry){   //Gerer l'event sur la touche entrée	
		
		
		//On récupere le buffer et le compteur d'itération du TextView
        
        switch(gtk_notebook_get_current_page(app->notebook)){
        case 0:
		    //getBUFFER(Salon);
		    //getITER(Salon);
	   		//on ajoute le message à la fin
	   		if(strcmp(readTEXT(Salon),"")!=0){
	   		
		   		if(ecrireLigne(sd,(char*)readTEXT(Salon))==-1){
		 			perror("Ecrire ligne");
		   		}
		  		//gtk_text_buffer_insert(app->Salon->buffer,&(app->Salon->iter),readTEXT(Salon),-1);
		  		gtk_entry_set_text((GtkEntry*)app->Salon->Texte,"");
   			}
   		break;
   		case 1:		    
		    //getBUFFER(Disney);
		    //getITER(Disney);
	   		
	   		//on ajoute le message à la fin
	   		if(strcmp(readTEXT(Disney),"")!=0){
	   		
		   		if(ecrireLigne(sd,(char*)readTEXT(Disney))==-1){
		 			perror("Ecrire ligne");
		   		}
		  		//gtk_text_buffer_insert(app->Disney->buffer,&(app->Disney->iter),readTEXT(Disney),-1);
		  		gtk_entry_set_text((GtkEntry*)app->Disney->Texte,"");
   			}
   		break;
   		case 2:		    
		    //getBUFFER(Pop);
		    //getITER(Pop);
	   		
	   		//on ajoute le message à la fin
	   		if(strcmp(readTEXT(Pop),"")!=0){
	   		
		   		if(ecrireLigne(sd,(char*)readTEXT(Pop))==-1){
		 			perror("Ecrire ligne");
		   		}
		  		//gtk_text_buffer_insert(app->Pop->buffer,&(app->Pop->iter),readTEXT(Pop),-1);
		  		gtk_entry_set_text((GtkEntry*)app->Pop->Texte,"");
   			}
   		break;
   		}
}



	//Mise a jour automatique des chats!!! 
void *Chat_Update (void *val){
char buf[LIGNE_MAX];

	while(1){
		ret = lireLigne(sd, buf);
	  	if (ret == LIGNE_MAX) {
				erreur_IO("lireLigne");
	  	}else if(ret<=0)
			continue;
			
	    getBUFFER(Salon);
		getITER(Salon);
		strcat(buf,"\n");
		gtk_text_buffer_insert(app->Salon->buffer,&(app->Salon->iter),buf,-1);
		  		
	}
	return 0;
}







