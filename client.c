    /*Réalisé par FAJRI Mohamed-Amine
	 Client pour les sockets
     *    socket_client ip_server port
     */
    #include <string.h>
    #include <stdlib.h>
    #include <strings.h>
    #include <stdio.h>
    #include <sys/types.h>
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
     
     int main(int argc, char** argv )
    { 
      int    d, sockfd,newsockfd,clilen;
      char c, fich[100], menu[550], msg[100], rep[100];
      struct sockaddr_in cli_addr,serv_addr;
     
      if (argc!=3) {printf ("usage  socket_client server port\n");exit(0);}
          
      /*
       *  partie client 
       */
      printf ("client starting\n");  
     
      /* initialise la structure de donnee */
      bzero((char*) &serv_addr, sizeof(serv_addr));
      serv_addr.sin_family       = AF_INET;
      serv_addr.sin_addr.s_addr  = inet_addr(argv[1]);
      serv_addr.sin_port         = htons(atoi(argv[2]));
     
      /* ouvre le socket */
      if ((sockfd=socket(AF_INET,SOCK_STREAM,0))<0)
        {printf("socket error\n");exit(0);}
     
      /* effectue la connection */
      if (connect(sockfd,(struct sockaddr*)&serv_addr,sizeof(serv_addr))<0)
        {printf("socket error\n");exit(0);}
      while (1) {
      		recv(sockfd, menu, 550, 0);
      		printf("%s", menu);
		gets(msg);
		d = atoi(msg);
		send(sockfd, msg, 100, 0);
		recv(sockfd, msg, 100, 0);
		printf("%s\n", msg);
		gets(rep);
		send(sockfd, rep, 100, 0);
		recv(sockfd, msg, 100, 0);
		printf("%s\n", msg);
		if(d==5 || d==6 || d==7 || d==8){
			gets(fich);
			send(sockfd, fich, 100, 0);
			if(d!=5){//les entrÃ©es ont besoin de plus d'infos donc il ya plus de send() et recv()
			//dans ce cas (d est 6 ou 7 ou 8) que dans les autre cas
				recv(sockfd, msg, 100, 0);
				printf("%s\n", msg);
				gets(msg);
				send(sockfd, msg, 100, 0);
			}
			if(d==7){//dans le cas de la modification d'entrÃ©e, le client doit aussi saisir les
			//nouveaux informations des personnes, donc il lui faut plus de send() recv()
			//que les autres cas
				recv(sockfd, msg, 100, 0);
				printf("%s\n", msg);
				gets(msg);
				send(sockfd, msg, 100, 0);
				recv(sockfd, msg, 100, 0);
				printf("%s\n", msg);
				gets(msg);
				send(sockfd, msg, 100, 0);
				recv(sockfd, msg, 100, 0);
				printf("%s\n", msg);
				gets(msg);
				send(sockfd, msg, 100, 0);
			}
			while(recv(sockfd, msg, 100, 0) > 0){
				if(strcmp(msg, "\n") == 0) break;
				else printf("%s\n", msg);
			}
		}
		else{
			while(recv(sockfd, msg, 100, 0) > 0){
				if(strcmp(msg, "\n") == 0) break;
				else printf("%s\n", msg);
			}
			recv(sockfd, msg, 100, 0);
		}
		}	  
          
      /*  attention il s'agit d'une boucle infinie 
       *  le socket n'est jamais ferme !
       */
     
       return 1;
     
    }


