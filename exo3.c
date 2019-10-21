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
/*--------------------------------------------------*/
int simple_cmd(char*argv[]){
	if(exit_shell(argv[0])==true){exit(-1);}else{
		for(i=0; i<NBARG; i++){
			if(argv[i]!=NULL){printf("%s",argv[i]);}
		}
		return 0;
	}
	return -1;
}
char ** split_string(char *s){
	char *str = NULL;
	char **a=NULL;
	a=malloc(sizeof(char*)*NBARG);
	if (a!=NULL){
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
	}
	return a;
}

int parse_line (char*s){
	char **a=NULL;
	remove_CR(s);
	remove_commentary(s);
	a=split_string(s);
	simple_cmd(a);
	free_tab_arguments(a);
	return 0;
}
int main(void)
{
	while(1){
	char buffer [BUFSIZE];
		printf("$");
		fgets(buffer,BUFSIZE,stdin);
		parse_line(buffer);
	}
	return 0;
}