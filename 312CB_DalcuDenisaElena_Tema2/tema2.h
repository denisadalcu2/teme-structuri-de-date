#include <stdio.h>
#include <stdlib.h>

#ifndef TEMA2


//lista simplu inlantuita pentru cuvinte-cheie
typedef struct cuv_cheie {
	char *cuvant;
	struct cuv_cheie *urm;
}TCuvant, *TListaCuvinte;


//lista dublu inlantuita pentru fisiere
typedef struct fisier {
	char *id;
	int scor;
	TListaCuvinte cuvinte;
	struct fisier *pre, *urm;
}TFisier;

//lista de fisiere
typedef struct {
	TFisier *head;
	TFisier *tail;
}TListaFisiere;

//lista de referinte pentru un cuvant cheie
typedef struct referinte {
	TFisier *fisier;
	struct referinte *urm;
} TReferinta, *TListaRef;

//nodul din arborele multicai de regasire
typedef struct nod {
    int este_terminal;//salveaza daca nodul este terminal sau nu 
	TListaRef fisiere;//daca nodul este terminal, are o lista de referinte
	char info;
    struct nod* left;   /* primul copil */
    struct nod* right;  /* fratele urmator */
} Node, *Tree;

//functia de comparare pentru heap
typedef int (*TFCmp)(TFisier*, TFisier*);

//structura pentru heap
typedef struct Heap {
	int nrMax, nrElem;
	TFisier **v;
	TFCmp comp;
} THeap;


//functii pentru operatii pe arbore
Tree createNode(char data);
void addChild(Tree parent, Tree child);
Tree adauga_cuv (Tree A, char* cuvant);
void ad_referinta(Tree nod, TFisier *fisier);
Tree cauta_nod_terminal(Tree root, char *cuvant);
void elim_ref(Tree nod_terminal, TFisier *fisier);
void print_dfs(Tree nod, char *cuv, int i, FILE *fout);
int elim_din_arb(Tree parinte, Tree nod, Tree root);
void freeTree(Tree root);


//functii pentru operatii cu fisiere
TFisier *aloca_fisier(char *id, int scor);
void ad_fisier(TListaFisiere *L, TFisier *fisier);
TFisier *cauta_fisier(TListaFisiere *L, char *id);
void adauga_cuvant_fisier(TFisier *f,char *cuvant);
void free_lista_fisiere(TListaFisiere *L);

//functii heap
THeap* AlocaHeap(int nrMax, TFCmp comp);
int RelMaxHeap(TFisier *a, TFisier *b);
void pushUp(THeap* h, int pos);
void inserareHeap(THeap *h, TFisier *val);
void pushDown(THeap *h, int pos);
TFisier* extrHeap(THeap *h);
void DistrugeHeap(THeap **h);

//operatii sistem
void ADD(TListaFisiere *L, Tree root, char *id, int scor, int t, char **cuvinte, FILE *fout);
void DEL(TListaFisiere *L, Tree root, char *id, FILE *fout);
void PRINT(Tree root, FILE *fout);
void ADDKW(TListaFisiere *L, Tree root, char *id, char *cuvant, FILE *fout);
void DELKW(TListaFisiere *L, Tree root, char *id, char *cuvant, FILE *fout);
void FIND(Tree root, char *cuvant, FILE *fout);
void TOPK(Tree rooti, char *cuvant, int k, FILE *fout);

#endif