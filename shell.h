#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
#define BUFSIZE 512
#define NBARG 25

/*-------FONCTIONS---------*/
	/* shell() :
	 - la boucle principale du programme 
	 - elle n'attend aucun paramètre et ne renvoie pas de valeur
	*/
	void shell();
	/*
	 simple_cmd(char*s): 
	  -reçoit les arguments(commandes)et les exécute
		et affiche les arguments de la chaine découpée
	  - renvoie 0 en cas de succés et -1 sinon
	*/
	int simple_cmd(char*argv[]);
	/* parse_line (char *s) :
		- enléve le saut de ligne de la touche entrée 
			pris en compte par fgets lors de la saisie de la commande
		- remplace le diése et ce qui suit par des espaces
		- découpe la chaine s passé en paramètre
			et la transforme en un tableau de chaines de caractères
		- renvoie 0 en cas de succés et -1 sinon
	*/
	int parse_line (char*s);
	 /*
	  execute_script(char *fname): 
	 	- éxecute le fichier de script fname  donné en paramètre
	 	- renvoie 0 en cas de succés et -1 sinon
	 */

	int execute_script(char *s);
	/*
		change_directory(char *s)
		- change de répertoire et affiche celui sur la sortie standard
		-ne renvoie rien
	*/
	void change_directory(char * repo);
	/* redir_cmd(char *argv[],char*in char *out)
		- éxecute la commande de redirection en recevant deux fichiers 
		(un pour l'entrée et l'autre pour la sortie)
		- renvoie 0 en cas  de succés et -1 sinon
	*/
	int redir_cmd(char *argv[], char *in, char *out); 
