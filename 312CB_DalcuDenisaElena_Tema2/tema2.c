#include <string.h>
#include "tema2.h"

void ADD(TListaFisiere *L, Tree root, char *id, int scor, int t, char **cuvinte, FILE *fout) {
	//verific daca id-ul fisierul exista deja
	TFisier *f=cauta_fisier(L,id);
	if(f) {
		fprintf(fout, "EXISTS\n");
		return;
	}
	//daca nu exista, creez fisierul si il adaug in lista dublu inlantuita
	f=aloca_fisier(id,scor);
	ad_fisier(L,f);

	//pentru fiecare cuvant-cheie, adaug in arb de regasire
	for(int i=0;i<t;i++){
		//verific daca exista cuvinte duplicate, ca sa adaug fiecare cuvant o sg data
		int dup=0;
		for(int j=0;j<i && dup == 0; j++)
			if(strcmp(cuvinte[i],cuvinte[j])==0)
				dup=1;
		if(dup==0){
			adauga_cuvant_fisier(f,cuvinte[i]);
			Tree terminal=adauga_cuv(root,cuvinte[i]);
			ad_referinta(terminal,f);
		}
	}
	//operatie reusita
	fprintf(fout, "OK\n");	
}

void DEL(TListaFisiere *L, Tree root, char *id, FILE *fout) {
	//verific daca fisierul cu id-ul dat exista
	TFisier *fisier=cauta_fisier(L,id);
	if(fisier==NULL){//fisierul nu exista
		fprintf(fout, "NOT FOUND\n");
		return;
	}

	//pentru fiecare cuvant cheie, elimin referintele catre fisier
	TCuvant *cuv=fisier->cuvinte;
	while(cuv!=NULL){
		//caut nodul terminal al cuvantului
		Tree nod_terminal = cauta_nod_terminal(root, cuv->cuvant);
		if(nod_terminal!=NULL){
			elim_ref(nod_terminal, fisier);
			//daca nodul terminal a ramas fara nicio referinta, sterg tot cuvantul din arbore
			if(nod_terminal->fisiere==NULL) {
				nod_terminal->este_terminal = 0;
				elim_din_arb(root,root->left,root);
			}
		}
		cuv=cuv->urm;
	}

	//elimin fisierul din lista de fisiere
	if(fisier->pre!=NULL)
		fisier->pre->urm=fisier->urm;
	else
		L->head=fisier->urm;
	if(fisier->urm!=NULL)
		fisier->urm->pre=fisier->pre;
	else
		L->tail=fisier->pre;
	//eliberez memoria
	TListaCuvinte c=fisier->cuvinte;
	while(c){
		TListaCuvinte urmator=c->urm;
		free(c->cuvant);
		free(c);
		c=urmator;
	}
	free(fisier->id);
	free(fisier);
	//operatie reusita
	fprintf(fout, "OK\n");
}

void PRINT(Tree root, FILE *fout) {
	if(!root->left) {
		fprintf(fout, "EMPTY\n");
		return;
	}
	char *cuv=malloc(101*sizeof(char));
	print_dfs(root->left,cuv,0,fout);
	free(cuv);
}

void ADDKW(TListaFisiere *L, Tree root, char *id, char *cuvant, FILE *fout){
	//verific daca exista fisierul
	TFisier *fisier = cauta_fisier(L,id);
	if(fisier==NULL){
		fprintf(fout, "NOT FOUND\n");
		return;
	}

	//verific daca cuvantul se afla deja in lista de cuvinte a fisierului
	TCuvant *c=fisier->cuvinte;
	while(c!=NULL){
		if(strcmp(c->cuvant,cuvant)==0){
			fprintf(fout, "OK\n");
			return;
		}
		c=c->urm;
	}
	//daca nu exista deja, il adaug
	adauga_cuvant_fisier(fisier,cuvant);
	//adaug cuvantul in arborele de regasire si ii salvez nodul terminal
	Tree nod_terminal=adauga_cuv(root,cuvant);
	//adaug referinta catre fisier la nodul terminal
	ad_referinta(nod_terminal,fisier);
	fprintf(fout,"OK\n");
}

void DELKW(TListaFisiere *L, Tree root, char *id, char *cuvant, FILE *fout){
	//verific daca exista fisierul
	TFisier *fisier=cauta_fisier(L,id);
	if(fisier==NULL){
		fprintf(fout, "NOT FOUND\n");
		return;
	}
	//verific daca cuvantul exista in lista de cuvinte a fisierului
	TCuvant *c=fisier->cuvinte, *pre=NULL;
	while(c!=NULL){
		if(strcmp(c->cuvant,cuvant)==0)
			break;
		pre=c;
		c=c->urm;
	}

	//daca cuvantul nu exista, nu fac nicio modificare
	if(c==NULL){
		fprintf(fout, "OK\n");
		return;
	}

	//elimin referinta sin nodul terminal al cuvantului
	Tree nod_terminal = cauta_nod_terminal(root, c->cuvant);
		if(nod_terminal!=NULL){
			elim_ref(nod_terminal, fisier);
			//daca nodul terminal a ramas fara nicio referinta, sterg tot cuvantul din arbore
			if(nod_terminal->fisiere==NULL) {
				nod_terminal->este_terminal = 0;
				elim_din_arb(root,root->left,root);
			}
		}
	//elimin cuvantul din lista fisierului
	if(pre!=NULL)
		pre->urm=c->urm;
	else
		fisier->cuvinte=c->urm;
	free(c->cuvant);
	free(c);

	//daca nu mai sunt cuvante in lista fisierului, elimin fisierul din lista de fisiere
	if(fisier->cuvinte==NULL){
	if(fisier->pre!=NULL)
		fisier->pre->urm=fisier->urm;
	else
		L->head=fisier->urm;
	if(fisier->urm!=NULL)
		fisier->urm->pre=fisier->pre;
	else
		L->tail=fisier->pre;
	//eliberez memoria
	free(fisier->id);
	free(fisier);
}
fprintf(fout,"OK\n");
}

void FIND(Tree root, char *cuvant, FILE *fout){
	//caut nodul terminal al cuvantului in arborele de regasire
	Tree nod=cauta_nod_terminal(root,cuvant);
	if(nod==NULL){
		fprintf(fout, "EMPTY\n");
		return;
	}
	int nr_ref=0;
	TListaRef referinte=nod->fisiere;
	while(referinte!=NULL){
		nr_ref++;
		referinte=referinte->urm;
		}
	//printez nr de referinte ale cuvantului
	fprintf(fout, "%d", nr_ref);
	//afisez id-urile fisierelor
	for(referinte=nod->fisiere; referinte!=NULL;referinte=referinte->urm)
		fprintf(fout, " %s", referinte->fisier->id);
	fprintf(fout, "\n");
}

void TOPK(Tree root, char *cuvant, int k, FILE *fout) {
	Tree nod=cauta_nod_terminal(root,cuvant);
	if(nod==NULL){
		fprintf(fout, "EMPTY\n");
		return;
	}
	int nr_ref=0;
	TListaRef referinte=nod->fisiere;
	while(referinte!=NULL){
		nr_ref++;
		referinte=referinte->urm;
	}
	THeap *h=AlocaHeap(nr_ref, RelMaxHeap);
	for(referinte=nod->fisiere;referinte!=NULL;referinte=referinte->urm)
		inserareHeap(h, referinte->fisier);

	if(k<nr_ref){
		fprintf(fout, "%d", k);;
		nr_ref=k;
	}
	else
		fprintf(fout, "%d", nr_ref);
	for(int i=0;i<nr_ref;i++)
		fprintf(fout, " %s", extrHeap(h)->id);
	fprintf(fout,"\n");
	DistrugeHeap(&h);
}