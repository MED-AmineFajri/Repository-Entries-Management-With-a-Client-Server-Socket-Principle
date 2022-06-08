//R�alis� par FAJRI Mohamed-Amine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>

typedef struct Personne{
        char nom[20];
        char prenom[20];
        char num_tel[20];
}Personne;

char rep_principal[30] = "/home/amine/rep_usms/";

void lister_contenu_arbitraire (int sock, char *rep){
        DIR *dir;
        struct dirent *entree;
	char chemin[100];
	strcpy(chemin,rep_principal);
	strcat(chemin,rep);
        dir = opendir(chemin);
        if(dir==NULL)
                return;
        send(sock, "Repertoire telephonique d'USMS : \n", 100, 0);
        while((entree=readdir(dir))!=NULL){
                send(sock, entree->d_name, 100, 0);
	}
	send(sock, "\n", 100, 0);
        closedir(dir);
}
void lister_contenu_trie (int sock, char *rep){
	struct dirent **entree;
	int i, n;
	char chemin[30];
        strcpy(chemin,rep_principal);
        strcat(chemin,rep);
	n = scandir(chemin, &entree, NULL, alphasort);
	if(n<0)
		perror("scandir");
	else{
		send(sock, "Repertoire telephonique d'USMS trié : \n", 100, 0);
		while(i<n){
			send(sock, entree[i]->d_name, 100, 0);
			free(entree[i]);
			++i;
		}
	}
	send(sock, "\n", 100, 0);
	free(entree);
}
void creer_2_fichiers (int sock, char *rep){
	char chemin1[100];
	char chemin2[100];
	strcpy(chemin1,rep);
	strcpy(chemin2,rep);
	FILE *f1, *f2;
	strcat(chemin1,"/fich1.txt");
	f1 = fopen(chemin1,"w");
	fclose(f1);
	strcat(chemin2,"/fich2.txt");
	f2 = fopen(chemin2,"w");
	fclose(f2);
}
void ajouter_sous_rep (int sock, char *nom_rep){
	char chemin[100];
	strcpy(chemin,rep_principal);
	strcat(chemin,nom_rep);
	if(mkdir(chemin,0777) != 0){ 
		perror("");
		exit(EXIT_FAILURE);
	}
	else
		send(sock, "Sous repertoire cree \n", 100, 0);
	creer_2_fichiers(sock, chemin);
	send(sock, "\n", 100, 0);
}
void supprimer_fich (int sock, char *nom_rep, char *nom_fich){
	char chemin[100];
	strcpy(chemin,rep_principal);
	strcat(chemin,nom_rep);
	strcat(chemin,"/");
	strcat(chemin,nom_fich);
	FILE *f;
	if ((f = fopen(chemin, "r+"))) //tester si le fichier existe
	{
        	fclose(f);
        	remove(chemin);
    	}
   	else
      		send(sock, "Fichier n'existe pas\n", 100, 0);
      	send(sock, "Suppression avec succés\n", 100, 0);
      	send(sock, "\n", 100, 0);
}
void supprimer_rep (int sock, char *nom_rep){
	FILE *f1, *f2;
	char chemin_rep[100], chemin1[100], chemin2[100];
	strcpy(chemin_rep,rep_principal);
	strcat(chemin_rep,nom_rep);
	strcpy(chemin1,chemin_rep);
	strcpy(chemin2,chemin1);
	strcat(chemin1,"/fich1.txt");
	strcat(chemin2,"/fich2.txt");
	if ((f1 = fopen(chemin1, "r")))
        {
                fclose(f1);
                supprimer_fich(sock, nom_rep,"fich1.txt");
	}
	if ((f2 = fopen(chemin2, "r")))
        {
                fclose(f2);
                supprimer_fich(sock, nom_rep,"fich2.txt");
        }
   	if(rmdir(chemin_rep) != 0){
     		perror("");
     		exit(EXIT_FAILURE);
   	}
   	else
     		send(sock, "Suppression du repertoire avec succés\n", 100, 0);
     	send(sock, "\n", 100, 0);
}
void rechercher_entree (int sock, char *nom_rep,char *nom_fich)
{
	FILE *f;
	Personne *p;
	char nom[100], chemin[100];
	strcpy(chemin,rep_principal);
	strcat(chemin,nom_rep);
	strcat(chemin,"/");
	strcat(chemin,nom_fich);
	f = fopen(chemin,"r+");
	if(f == NULL)
	{
		send(sock, "Erreur, fichier non lu\n", 100, 0);
		send(sock, "\n", 100, 0);
		exit(1);	}
	send(sock, "Saisir le nom de la personne voulue rechercher\n", 100, 0); 
	recv(sock, nom, 100, 0);
	send(sock, "La personne recherchée est :\n", 100, 0);
	while(fscanf(f, "%s %s %s",p->nom, p->prenom, p->num_tel) != EOF)
	{
		if(strcmp(p->nom,nom) == 0){
			send(sock, p->nom , 100, 0);
			send(sock, p->prenom , 100, 0);
			send(sock, p->num_tel, 100, 0);
			send(sock, "\n", 100, 0);
			fclose(f);
		        exit(EXIT_SUCCESS);
		}
	}
	fclose(f);
	send(sock, "Personne inexistante\n", 100, 0);
	send(sock, "\n", 100, 0);
}
void modifier_entree (int sock, char *nom_rep,char *nom_fich)
{
        FILE *f, *fich_temp;
        Personne *p;
        char nom[100], chemin[100], chemin_tmp[100], nv_nom[100], nv_prenom[100], nv_num_tel[100];
        strcpy(chemin,rep_principal);
        strcat(chemin,nom_rep);
        strcat(chemin,"/");
	strcpy(chemin_tmp,chemin);
        strcat(chemin,nom_fich);
	strcat(chemin_tmp,"temp.txt");
	f = fopen(chemin,"r+");
	fich_temp = fopen(chemin_tmp, "w+");
        if(f == NULL)
        {
                fprintf(stdout, "Erreur, fichier non lu\n");
                exit(1);
        }
        send(sock, "Saisir le nom de la personne que vous voulez modifier :\n", 100, 0);
	recv(sock, nom, 100, 0);
        while(fscanf(f, "%s %s %s",p->nom, p->prenom, p->num_tel) != EOF)
        {
                if(strcmp(p->nom,nom) == 0){
                	send(sock, "Nouveau Nom : \n", 100, 0);
                	recv(sock, nv_nom, 100, 0);
			send(sock, "Nouveau Prenom : %s\n", 100, 0);
			recv(sock, nv_prenom, 100, 0);
			send(sock, "Nouveau Numero de telephone : %s\n", 100, 0);
			recv(sock, nv_num_tel, 100, 0);
			fclose(f);
		        //exit(EXIT_SUCCESS);
			fprintf(fich_temp, "%s %s %s\n", nv_nom, nv_prenom, nv_num_tel);
                }
		else
			fprintf(fich_temp, "%s %s %s\n", p->nom, p->prenom, p->num_tel);
	}
        fclose(f);
	fclose(fich_temp);
	remove(nom_fich);
	rename("temp.txt", nom_fich);
	send(sock, "\n", 100, 0);
}
void supprimer_entree (int sock, char *nom_rep,char *nom_fich)
{
        FILE *f, *f_temp;
        Personne *p;
        char nom[100], chemin[100], fich_temp[100];
	strcpy(chemin,rep_principal);
        strcat(chemin,nom_rep);
        strcat(chemin,"/");
        strcpy(fich_temp,chemin);
        strcat(chemin,nom_fich);
	strcat(fich_temp,"temp.txt");
        f = fopen(chemin,"r+");
	f_temp = fopen(fich_temp, "w+");
        if(f == NULL)
        {
                fprintf(stdout, "Erreur, fichier non lu\n");
                exit(1);
        }
        send(sock, "Saisir le nom de la personne que vous voulez supprimer :\n", 100, 0);
	recv(sock, nom, 100, 0);
        while(fscanf(f, "%s %s %s",p->nom, p->prenom, p->num_tel) != EOF)
        {
                if(strcmp(p->nom,nom) == 0){
                }
                else
                        fprintf(f_temp, "%s %s %s\n", p->nom, p->prenom, p->num_tel);
        }
        fclose(f);
        fclose(f_temp);
        remove(nom_fich);
        rename("temp.txt", nom_fich);
        send(sock, "\n", 100, 0);
}

