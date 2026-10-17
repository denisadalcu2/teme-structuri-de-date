#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tema2.h"

int main(){
	//deschid fisierele de citire/afisare
	FILE *fin=fopen("indexare.in", "r");
	FILE *fout=fopen("indexare.out", "w");

	int Q;
	fscanf(fin, "%d", &Q);

	Tree root=createNode('\0');
	TListaFisiere L_fisiere;
	L_fisiere.head=L_fisiere.tail=NULL;
	char *operatie=malloc(20*sizeof(char));
	for(int i=0;i<Q;i++){
		fscanf(fin,"%s", operatie);

		if(strcmp(operatie, "ADD")==0){
			char *id=malloc(101*sizeof(char));
			int scor, t;
			fscanf(fin,"%s", id);
			fscanf(fin,"%d%d", &scor, &t);

			char **cuvinte=malloc(t*sizeof(char *));
			for(int j=0;j<t;j++){
				cuvinte[j]=malloc(101*sizeof(char));
				fscanf(fin, "%s", cuvinte[j]);
			}
			ADD(&L_fisiere, root, id, scor, t, cuvinte, fout);
			//eliberare memorie
			for(int j=0;j<t;j++)
				free(cuvinte[j]);
			free(cuvinte);
			free(id);
		} else if(strcmp(operatie, "PRINT")==0){
			PRINT(root,fout);
		} else if(strcmp(operatie, "DEL")==0){
			char *id=malloc(101*sizeof(char));
			fscanf(fin,"%s", id);
			DEL(&L_fisiere,root,id,fout);
			free(id);
		} else if(strcmp(operatie, "ADDKW")==0){
			char *id=malloc(101*sizeof(char));
			char *cuvant=malloc(101*sizeof(char));
			fscanf(fin,"%s%s", id, cuvant);
			ADDKW(&L_fisiere,root,id,cuvant,fout);
			free(id);
			free(cuvant);
		} else if(strcmp(operatie, "DELKW")==0){
			char *id=malloc(101*sizeof(char));
			char *cuvant=malloc(101*sizeof(char));
			fscanf(fin,"%s%s", id, cuvant);
			DELKW(&L_fisiere,root,id,cuvant,fout);
			free(id);
			free(cuvant);
		}else if(strcmp(operatie, "FIND")==0){
			char *cuvant=malloc(101*sizeof(char));
			fscanf(fin,"%s", cuvant);
			FIND(root,cuvant,fout);
			free(cuvant);
		} else if(strcmp(operatie, "TOPK")==0){
			char *cuvant=malloc(101*sizeof(char));
			fscanf(fin,"%s", cuvant);
			int k;
			fscanf(fin, "%d", &k);
			TOPK(root,cuvant,k,fout);
			free(cuvant);
		}
	}
	free(operatie);
	free_lista_fisiere(&L_fisiere);
	freeTree(root);
	fclose(fin);
	fclose(fout);
	
	return 0;
}
