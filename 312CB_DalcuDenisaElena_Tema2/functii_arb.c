#include "tema2.h"
#include <string.h>


//am modificat functia din laborator astfel incat nodul sa contina mai multe informatii
/* Creare nod */
Tree createNode(char data) {
	Tree newnode = malloc(sizeof(Node));
	if(!newnode)
		return NULL;
	newnode->info = data;
	newnode->left = NULL;
	newnode->right = NULL;
	newnode->este_terminal = 0;
	newnode->fisiere = NULL;
	return newnode;
}

//adaug cuvintele in arbore in ordine lexicografica
void addChild(Tree parent, Tree child) {
	if (parent->left == NULL ||parent->left->info > child->info) {
		child->right = parent->left;
		parent->left = child;
		
	} else {
		Tree current = parent->left;
		//caut locul unde sa inserez cuvantul in arbore
		while (current->right && current->right->info < child->info)
			current = current->right;
		//fac inserarea
		child->right = current->right;
		current->right = child;
	}
}

//functia returneaza nodul corespunzator ultimei litere ale unui
//cuvant, ca sa pot lega de el referinta catre fisier
Tree adauga_cuv (Tree A, char* cuvant){
	Tree nod_curent = A;
	for(int i=0;cuvant[i]!='\0';i++){
		Tree copil = nod_curent->left;
		while(copil&&copil->info!=cuvant[i])
			copil=copil->right;
		if(copil==NULL){
			copil=createNode(cuvant[i]);
			addChild(nod_curent,copil);
		}
		nod_curent=copil;
	}
	nod_curent->este_terminal=1;
	return nod_curent;
}

//functia de adaugare referinta la un nod terminal, in ordinea id-urilor
void ad_referinta(Tree nod, TFisier *fisier) {
	TReferinta *referinta = malloc(sizeof(TReferinta));
	referinta->fisier=fisier;
	referinta->urm=NULL;
	//adaugare in lista sortat, dupa id
	if(nod->fisiere==NULL || strcmp(fisier->id, nod->fisiere->fisier->id)<0){
		referinta->urm=nod->fisiere;
		nod->fisiere=referinta;
	} else{
		TListaRef curent=nod->fisiere;
		while(curent->urm!=NULL){
			if(strcmp(fisier->id, curent->urm->fisier->id)<0)
				break;
			curent=curent->urm;
		}
		referinta->urm=curent->urm;
		curent->urm=referinta;
	}
}


//functie care cauta nodul corespunzator ultimei litere ale unui cuvant
Tree cauta_nod_terminal(Tree root, char *cuvant) {
	Tree nod_curent=root;
	for(int i=0; cuvant[i]!='\0';i++) {
		Tree copil=nod_curent->left;
		while(copil!=NULL && copil->info != cuvant[i])
			copil=copil->right;
		if(copil==NULL)
			return NULL;
		nod_curent=copil;
	}
	if(nod_curent->este_terminal==1)
		return nod_curent;
	else return NULL;
}

//functie care sterge referinta catre un fisier din lista de ref a unui nod terminal
void elim_ref(Tree nod_terminal, TFisier *fisier){
	TListaRef pre=NULL, curent=nod_terminal->fisiere;
	while(curent!=NULL){
		//caut referinta in lista
		if(curent->fisier == fisier){
			//am gasit fisierul, elimin din lista
			if(pre!=NULL)
				pre->urm=curent->urm;
			else
				nod_terminal->fisiere=curent->urm;
			//eliberez memoria
			free(curent);
			return;
		}
		pre=curent;
		curent=curent->urm;
	}
}

void print_dfs(Tree nod, char *cuv, int i, FILE *fout) {
	if(nod==NULL)
		return;
	//reconstruiesc cuvantul
	cuv[i] = nod->info;

	if(nod->este_terminal==1){
		cuv[i+1]='\0';
		//printez cuvantul reconstruit
		fprintf(fout, "%s", cuv);
		int nr_ref=0;
		TListaRef referinte=nod->fisiere;
		while(referinte!=NULL){
			nr_ref++;
			referinte=referinte->urm;
		}
		//printez nr de referinte ale cuvantului
		fprintf(fout, " %d", nr_ref);
		//afisez id-urile fisierelor
		for(referinte=nod->fisiere; referinte!=NULL;referinte=referinte->urm)
			fprintf(fout, " %s", referinte->fisier->id);
		fprintf(fout, "\n");
	}
	//apelez functia recursiv
	print_dfs(nod->left, cuv, i+1, fout);
	print_dfs(nod->right, cuv, i, fout);
}

//functie care elimina un nod din arbore- returneaza 1 daca nodul poate fi eliminat
int elim_din_arb(Tree parinte, Tree nod, Tree root){
	if(nod==NULL)
		return 0;//nodul nu exista
	Tree frate=nod->right;
	//elimin copiii nodului dat
	elim_din_arb(nod,nod->left,root);

	//daca nodul nu e terminal si nu are copii, il sterg
	if(nod->este_terminal==0 && nod->left==NULL){
		if(parinte->left==nod)
			parinte->left=nod->right;
		else{
			Tree curent=parinte->left;
			//caut nodul in lista de copii a parintelui
			while(curent!=NULL && curent->right!=nod)
				curent=curent->right;
			if(curent)
				curent->right=nod->right;
		}
		free(nod);
		return 1;//nodul a putut fi eliminat
	}
	elim_din_arb(parinte,frate,root);
	return 0;
}

void freeTree(Tree root) {
	if(!root)
		return;
	freeTree(root->left);
	freeTree(root->right);
	//eliberez lista de referinta, in cazul unui nod terminal
	TListaRef r=root->fisiere;
	while(r!=NULL) {
		TListaRef urm=r->urm;
		free(r);
		r=urm;
	}
	free(root);
}
