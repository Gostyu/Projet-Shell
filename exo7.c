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
void copy(char *src,char * dest){
	unsigned int taille_chaine=strlen(dest);
	for(unsigned int i=0; i<strlen(src);i++){
				src[i]='\0';
	}
	for(unsigned int i=1; i<taille_chaine;i++){
		src[i-1]=dest[i];
	}
	src[taille_chaine-1]='\0';
}
int operations_on_env_variables(char * chaine,char*valeur){
	if(isset_env_variable(chaine)==true){
		if(setenv(chaine,valeur,1)==0){
			return 0;
		}else{
			perror("setenv");
			return -1;
		}
	}else{
		if(setenv(chaine,valeur,0)==0){
			return 0;
		}else{
			perror("setenv");
			return -1;
		}
	}
	return -1;
}
void env_variables(char **a){

		char * chaine =NULL;
		char * valeur=NULL;
		char *chaine2=NULL;
		char tmp[BUFSIZE];
		int l;
		for(l=0; a[l]!=NULL; l++){
			if(strpbrk(a[l],"=")!=NULL){
			chaine=calloc(BUFSIZE+1,sizeof(char));
			valeur=calloc(BUFSIZE+1,sizeof(char));
				if(chaine!=NULL && valeur!=NULL){
					int i;
					for(i=0; a[l][i]!='=';i++){
						chaine[i] = a[0][i];
						if(a[l][i]=='='){
							chaine[i]='\0';
						}
					}
					int k=0;
					for(int j=i+1;a[l][j]!='\0';j++){
						valeur[k]= a[l][j];
						k++;
						if(a[l][j+1]=='\0'){valeur[k]=a[l][j+1];}
					}
					operations_on_env_variables(chaine,valeur);
					free(chaine);
					free(valeur);
				}
		}else if(strpbrk(a[l],"$")!=NULL){
			chaine2=calloc(BUFSIZE+1,sizeof(char));
			if(chaine2!=NULL){
				for(unsigned int i=0; a[l][i]!='\0';i++){
					chaine2[i]=a[l][i];
					if(a[l][i]=='\0'){chaine2[i]='\0';}
				}
				if(chaine2[0]=='$'){
					copy(tmp,chaine2);
					if(isset_env_variable(tmp)==true){
						char * env_value=getenv(tmp);
						if(env_value!=NULL){printf("%s\n",env_value);}
					}
				free(chaine2);
				}
			}
		}
	}
}
/*--------------------------------------*/
int execute_script(char *fname){
	char buffer[BUFSIZE];	
	FILE * fd = fopen(fname,"r");
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
int redir_cmd(char *argv[], char *in, char *out){
	int fd_out;
	unsigned int i;
	for(i=0;argv[i]!=NULL; i++){
		if(strcmp(argv[i],"<")==0){
			if(in!=NULL){
				fd_out=open(in,O_CREAT|O_APPEND|O_TRUNC|O_RDWR,S_IWUSR|S_IRUSR);
				if(fd_out<0){perror("open fd[0]");}
				else{	
					if(dup2(fd_out,0)<0){perror("dup2(fd[0],0)");}else{
							simple_cmd(argv);
					}
					close(fd_out);
				}
				close(fd_out);
			}
		}else if(strcmp(argv[i],">")==0){
				if(out!=NULL){
					fd_out=open(out,O_CREAT|O_APPEND|O_TRUNC|O_RDWR,S_IWUSR|S_IRUSR);
					if(fd_out<0){perror("open fd_out");}else{
						if(dup2(fd_out,1)<0){perror("dup2(fd[1],1)");}else{
							if(strcmp(argv[i],">")==0){argv[i]=NULL; argv[i+1]=NULL;
									simple_cmd(argv);
							}
							close(fd_out);
						}
					close(fd_out);
					}
				}
			}
	}
	return -1;
}
/*-------------------------------------*/


/*-------------------------------------
int start_cmd(char *argv[], char *in){return -1;}
int next_cmd(char *argv[]){return -1;}
int last_cmd(char *argv[], char *out){return -1;}
int filters(char*argv[]){
	for(int i=0; argv[i]!=NULL;i++){
		if(strcmp(argv[i],"|")==0){
			argv[i]=NULL;
			if(strcmp(argv[i-2],"<")==0||strcmp(argv[i-2],">")==0){
				start_cmd(argv,argv[i-1]);				
			}else if(strcmp(argv[i+2],"<")==0||strcmp(argv[i+2],">")==0){
				start_cmd(argv,argv[i+3])
			}else{
				start_cmd(argv,NULL);
			}
		}
	}
	return -1;
}

-------------------------------------*/
int simple_cmd(char*argv[]){
	int* statut_fils=NULL;
	pid_t fils;
	if(exit_shell(argv[0])==true){exit(-1);
	}else if(strcmp(argv[0],"cd")==0){
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
	for(int i=0; a[i]!=NULL; i++){
		if(strcmp(a[i],"<")==0){
			redir_cmd(a,a[i+1],NULL);
		}else if(strcmp(a[i],">")==0){
			redir_cmd(a,NULL,a[i+1]);
		}else if(strpbrk(a[i],"=")!=NULL || strpbrk(a[i],"$")!=NULL){
			env_variables(a);
		}else if(strpbrk(a[i],"|")!=NULL){
			printf("OK\n");
			//filters(a);
		}else{
			break;
		}
	}
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
	if(argc!=2){
		shell();
	}else{
		execute_script(argv[1]);
	}	
	return 0;
}