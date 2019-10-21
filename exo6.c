#include "shell.h"
void free_tab_arguments(char**a){
	for(int i=0; a[i]!=NULL; i++){free(a[i]);}
	free(a);
}
bool exit_shell(char*s){
	return strcmp(s,"exit")==0;
}
void remove_CR(char*s){
	for(unsigned int i=0;i<strlen(s);i++){
		if(s[i]=='\n'){s[i]='\0';}
	}
}
void remove_commentary(char*s){
	if(strpbrk(s,"#")!=NULL){
		for(unsigned int i=0;i<strlen(s); i++){
			if(s[i]=='#'){s[i]='\0';}
		}
	}
}
/*--------------------------------------*/
void change_directory(char *repo){
	char buf[BUFSIZE];
	char* path =NULL;
	if(chdir(repo)!=0){perror("chdir");}
		else{
		if((path=getcwd(buf,BUFSIZE))!=NULL){printf("%s\n",path);}
	}
}
/*--------------------------------------*/
bool isset_env_variable(char*s){
	return getenv(s)!=NULL;
}
void env_variables(char **a){
		char chaine[BUFSIZE];
		char valeur[BUFSIZE];
		char tmp[BUFSIZE];
		int i=0;
		for(i=0; a[0][i]!='=';i++){
			chaine[i] = a[0][i];
			if(a[0][i]=='='){
				chaine[i]='\0';
			}
		}
		int k=0;
		for(int j=i+1;a[0][j]!='\0';j++){
			valeur[k]= a[0][j];
			k++;
			if(a[0][j+1]=='\0'){valeur[k]=a[0][j+1];}
		}
		printf("variable : %s\n",chaine);
		printf("valeur : %s\n",valeur);
		strcat(tmp,"$");
		strcat(tmp,chaine);
		printf(" tmp : %s",tmp);
		/*if(isset_env_variable(tmp)==true){
			if(setenv(tmp,valeur,1)==0){
				printf("La valeur de %s a été changé\n",chaine);
			}else{
				perror("setenv");
			}
		}else{
			if(setenv(chaine,valeur,0)==0){
				printf("%s=%s a bien été ajouté avec dans l'environnement\n",chaine,valeur);
			}else{
				perror("setenv");
			}
		}*/
}
/*--------------------------------------*/
int execute_script(char *s){
	char buffer[BUFSIZE];
	FILE * fd = fopen(s,"r");
	if(fd!=NULL){
		while(fgets(buffer,BUFSIZE,fd)!=NULL){
			parse_line(buffer);
		}
		fclose(fd);
		return 0;
	}
	return -1;
}	

/*--------------------------------------*/
int simple_cmd(char*argv[]){
	int* statut_fils=NULL;
	pid_t fils;
	if(exit_shell(argv[0])==true){exit(-1);}
		else if(strcmp(argv[0],"cd")==0){
			 change_directory(argv[1]);
		}else{
				fils= fork();
				if(fils==0){execvp(argv[0],argv);}
				else 
					if(fils>0){wait(statut_fils);}
				else{
					perror("fork");
				}
			}
	return 0;
}
char** split_string(char * s){
	char**a=NULL;
	a=malloc(sizeof(char*)*NBARG);
	if(a!=NULL){
		char*str=NULL;
		str = strtok(s," ");
		int i=0;
		while(str!=NULL){
			a[i]=malloc((sizeof(char**)*strlen(str))+1);
			if(a[i]!=NULL){
				strcpy(a[i],str);
			}else{
				perror("Echec du malloc() pour les pointeurs a[i]");
			}
			str = strtok(NULL," ");
			i++;
		}
		a[i] = NULL;
		return a;
	}
	return NULL;
}
int parse_line (char*s){
	char **a=NULL;
	remove_CR(s);
	remove_commentary(s);
	a = split_string(s);
	if(strpbrk(a[0],"=")!=NULL){
		env_variables(a);
	}else{
		simple_cmd(a);
	}
	free_tab_arguments(a);
	return 0;
}
void shell(){
	while(1){
		char buffer [BUFSIZE];
		printf("$");
		fgets(buffer,BUFSIZE,stdin);
		parse_line(buffer);
	}
}
int main(int argc,char*argv[]){
	if(argc!=2){
		shell();
	}else{
		execute_script(argv[1]);
	}
	return 0;
}