#include <string.h>
#include "tema2.h"

//functie de alocare "celula" pentru lista dublu inlantuita de fisiere
TFisier *aloca_fisier(char *id, int scor) {
	TFisier *fisier = malloc(sizeof(TFisier));
	fisier->id = malloc(strlen(id)+1);
	strcpy(fisier->id, id);
	fisier->scor = scor;
	fisier->cuvinte = NULL;
	fisier->pre = fisier->urm = NULL;
	return fisier;
}

//functie care adauga fisier in lista dublu inlantuita
void ad_fisier(TListaFisiere *L, TFisier *fisier) {
	if(L->head==NULL) {//lista e vida
		L->head=fisier;
		L->tail=fisier;
	} else {
		L->tail->urm = fisier;
		fisier->pre=L->tail;
		L->tail=fisier;
	}
}

//functie care verifica daca exista un alt fisier cu acelasi id
TFisier *cauta_fisier(TListaFisiere *L, char *id){
	TFisier *p=L->head;
	while(p){
		if(strcmp(p->id,id)==0)
			return p;
	p=p->urm;
	}
	return NULL;
}

//functie care insereaza un cuvant in lista de cuvinte a unui fisier
void adauga_cuvant_fisier(TFisier *f,char *cuvant){
	TCuvant *c=malloc(sizeof(TCuvant));
	c->cuvant=malloc(strlen(cuvant)+1);
	strcpy(c->cuvant, cuvant);
	c->urm=f->cuvinte;
	f->cuvinte=c;
}

void free_lista_fisiere(TListaFisiere *L) {
	TFisier *f=L->head;
	while(f!=NULL) {
		TFisier *urm=f->urm;
		TListaCuvinte c=f->cuvinte;
		while(c!=NULL){
			TListaCuvinte urm2=c->urm;
			free(c->cuvant);
			free(c);
			c=urm2;
		}
		free(f->id);
		free(f);
		f=urm;
	}
}