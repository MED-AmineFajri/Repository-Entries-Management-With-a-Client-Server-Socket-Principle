    /*RÈalisÈ par FAJRI Mohamed-Amine
	 Serveur sockets TCP
     * affichage de ce qui arrive sur la socket
     *    socket_server port (port > 1024 sauf root)
     */
     
    #include <stdlib.h>
    #include <string.h>
    #include <strings.h>
    #include <stdio.h>
    #include <sys/types.h>
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include "fcts.h"
     void func_switch(int d, int newsockfd){
				char nom_rep[100], nom_sous_rep[100], nom_fich[100];
				switch(d){
				case 1 :{
				send(newsockfd, "Entrer le nom du r√©pertoire\n", 100, 0);
				recv(newsockfd, nom_rep, 100, 0);
				lister_contenu_arbitraire(newsockfd, nom_rep);
				break;
				}
				case 2 :{
				send(newsockfd, "Entrer le nom du r√©pertoire\n", 100, 0);
				recv(newsockfd, nom_rep, 100, 0);
				lister_contenu_trie(newsockfd, nom_rep);
				break;
                                }
				case 3 :{
				send(newsockfd, "Entrer le nom du nouveau sous repertoire\n", 100, 0);
				recv(newsockfd, nom_sous_rep, 100, 0);
				ajouter_sous_rep(newsockfd, nom_sous_rep);
        			break;
				}
				case 4 :{
				send(newsockfd, "Entrer le nom du sous repertoire voulu supprimer\n", 100, 0);
                		recv(newsockfd, nom_sous_rep, 100, 0);
                		supprimer_rep(newsockfd, nom_sous_rep);
        			break;
				}
				case 5 :{
				send(newsockfd, "Entrer le nom du sous repertoire qui contient le fichier voulu supprimer\n", 100, 0);
                		recv(newsockfd, nom_sous_rep, 100, 0);
				send(newsockfd, "Entrer le nom du fichier voulu supprimer\n", 100, 0);
                		recv(newsockfd, nom_fich, 100, 0);
                		supprimer_fich(newsockfd, nom_sous_rep, nom_fich);
        			break;
				}
				case 6 :{
        			send(newsockfd, "Entrer le nom du repertoire du fichier contenant l entree\n", 100, 0);
                		recv(newsockfd, nom_sous_rep, 100, 0);
				send(newsockfd, "Entrer le nom du fichier contenant l entree\n", 100, 0);
				recv(newsockfd, nom_fich, 100, 0);
                		rechercher_entree(newsockfd, nom_sous_rep,nom_fich);
        			break;
				}
				case 7 :{
        			send(newsockfd, "Entrer le nom du repertoire du fichier contenant l entree\n", 100, 0);
                		recv(newsockfd, nom_sous_rep, 100, 0);
				send(newsockfd, "Entrer le nom du fichier contenant l entree\n", 100, 0);
				recv(newsockfd, nom_fich, 100, 0);
                                modifier_entree(newsockfd, nom_sous_rep,nom_fich);
        			break;
				}
				case 8 :{
        			send(newsockfd, "Entrer le nom du repertoire du fichier contenant l entree\n", 100, 0);
                		recv(newsockfd, nom_sous_rep, 100, 0);
				send(newsockfd, "Entrer le nom du fichier contenant l entree\n", 100, 0);
				recv(newsockfd, nom_fich, 100, 0);
                                supprimer_entree(newsockfd, nom_sous_rep,nom_fich);
        			break;
				}
				default : {
				send(newsockfd, "Choix incorrect\n", 100, 0);
				recv(newsockfd, nom_sous_rep, 100, 0);
				break;
				}
			}
			}
     
    int main(int argc, char** argv )
    { 
	int    sockfd,newsockfd,clilen;
      char c[100], msg[100], message[100], ligneCommande[100], line[100], commande[100], newcommande[100];
      FILE *fp;
      FILE *fo;
      struct sockaddr_in cli_addr,serv_addr;
     
     
      if (argc!=2) {printf ("usage: socket_server port\n");exit(0);}
     
      printf ("server starting...\n");  
     
      /* ouverture du socket */
      sockfd = socket (AF_INET,SOCK_STREAM,0);
      if (sockfd<0) {printf ("impossible d'ouvrir le socket\n");exit(0);}
     
      /* initialisation des parametres */
      bzero((char*) &serv_addr, sizeof(serv_addr));
      serv_addr.sin_family       = AF_INET;
      serv_addr.sin_addr.s_addr  = htonl(INADDR_ANY);
      serv_addr.sin_port         = htons(atoi(argv[1]));
     
      /* effectue le bind */
      if (bind(sockfd,(struct sockaddr*)&serv_addr,sizeof(serv_addr))<0)
         {printf ("impossible de faire le bind\n");exit(0);}
     
      /* √©coute */
      listen(sockfd,10);
     int i=0;
      /* attente de connection d'un client */
      clilen = sizeof (cli_addr);
	while(i<10){
      		newsockfd = accept (sockfd,(struct sockaddr*) &cli_addr, &clilen);
      		if (newsockfd<0) {printf ("accept error\n"); exit(0);}
      		printf ("connection accepted\n");
      		switch(fork()){
      		case 0 : {
      			close(sockfd);
			send(newsockfd, "Menu de gestion du repertoire d USMS\nPour lister le contenu du repertoire arbitrairement sasir  1\nPour lister le contenu du repertoire trie sasir  2\nPour ajouter un sous repertoire contenant deux fichiers textes saisir  3\nPour supprimer un repertoire sasir  4\nPour supprimer un fichier sasir  5\nPour rechercher les informations d une entree sasir  6\nPour modifier une entree sasir  7\nPour supprimer une entree sasir  8\n", 550, 0);
     		 while (1)
       			{ 
			//while (read(newsockfd,&c,1)!=1);
        		 //printf("%c",c);
			//if( c == '@') {
			//	close(newsockfd);
			//	break;}
			//write(newsockfd,&c,
			strcpy(c, "");
			recv(newsockfd, c, 100, 0);
				int d = atoi(c);
				func_switch(d, newsockfd);
				
				//fp = popen(message,"r");
			//
			//send(newsockfd, "", 0, 0);
			//printf("Entrez la chaine a envoyer: \n");
			//gets(messageToSend);
			//
			//pclose(fp);
       		}
       		exit(EXIT_SUCCESS);
       		}
       		case 1 : {
       			perror("fork");
       			return -1;
       			}
       			default : {
       			wait(NULL);
       			close(newsockfd);
       			}
       			}
	i++;
     	}
       /*  attention il s'agit d'une boucle infinie 
        *  la socket n'est jamais ferme !
        */
     
       return 1;
     }

