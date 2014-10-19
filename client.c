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
#include "pse.h"
#include <gtk/gtk.h>

#define CMD   "client"

GtkBuilder *builder;
GtkWidget *window;
GtkWidget *onglet;
GError *error=NULL;


int main(int argc, char *argv[]) {
 /* int sd, ret;
  struct sockaddr_in *sa;
  char buf[LIGNE_MAX];*/
  
  gtk_init(&argc,&argv);
  
  builder=gtk_builder_new();
  if(!gtk_builder_add_from_file(builder,"UIBT.glade",&error)){
  	g_warning("%s",error->message);
  	g_free(error);
  	return(1);  
  }
  
  window=GTK_WIDGET(gtk_builder_get_object(builder,"window1"));
  gtk_window_set_default_size(GTK_WINDOW(window), 800, 200);
  gtk_builder_connect_signals(builder,NULL);
  
  g_object_unref(G_OBJECT(builder));
  gtk_widget_show(window);
  gtk_main();
  
/*
  if (argc != 3) {
    erreur("usage: %s machine port\n", argv[0]);
  }

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
*/
  /* connexion sur site distant */
  /*printf("%s: connecting the socket\n", CMD);
  ret = connect(sd, (struct sockaddr *) sa, sizeof(struct sockaddr_in));
  if (ret < 0) {
    erreur_IO("Connect");
  }

  freeResolv();
  
strcpy(buf,"debut");
while(strcmp(buf,"fin")!=0){
  printf("ligne> ");
  if (fgets(buf, LIGNE_MAX, stdin) == NULL) {
    erreur_IO("fgets");
  }
  
  ret = ecrireLigne(sd, buf);
  if (ret == -1) {
    erreur_IO("ecrireLigne");
  }


  printf("Done.\n");
  printf("%s: %d bytes sent\n", CMD, ret);
}
  if (close(sd) == -1) {
    erreur_IO("close");
  }
*/
  exit(EXIT_SUCCESS);
}


