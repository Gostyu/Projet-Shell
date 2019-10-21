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
	simple_cmd(a);
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
	shell();
	return 0;
}