#include "tema2.h"
#include <string.h>

THeap* AlocaHeap(int nrMax, TFCmp comp) {
	THeap* h = (THeap*) malloc(sizeof(struct Heap));
	if (!h) {
		return NULL;
	}
	h->v = malloc(nrMax * sizeof(TFisier*));
	if (!h->v) {
		free(h);
		return NULL;
	}
	h->nrMax = nrMax;
	h->nrElem = 0;
	h->comp = comp;
	return h;
}

int RelMaxHeap(TFisier *a, TFisier *b) {
	if(a->scor != b->scor)
		return a->scor > b->scor;
	return strcmp(a->id, b->id)<0;
}

void pushUp(THeap* h, int pos) {
    while(pos!=0) { //pana cand ajung la rad
		int tata = (pos-1)/2;
		if(h->comp(h->v[pos], h->v[tata])) {
			//swap(h->v[(pos-1)/2],h->v[pos]);
			TFisier *aux = h->v[tata];
			h->v[tata] = h->v[pos];
			h->v[pos] = aux;
			  pos = tata;
		}else break;
	}
}

void inserareHeap(THeap *h, TFisier *val) {
	h->v[h->nrElem] = val;
	int pos = h->nrElem;
	pushUp(h,pos);
	h->nrElem++;
}

void pushDown(THeap *h, int pos) {
	while(2*pos+1 < h->nrElem) { //cat timp mai exista fiu stang
		int s = 2*pos + 1; //s = fiu_st
		if(2*pos+2<h->nrElem && h->comp(h->v[2*pos +2], h->v[s]))
			s= 2*pos+2;//s=fiu_dr
		if(h->comp(h->v[pos], h->v[s]))
			break;
		else{
			TFisier *aux = h->v[pos];
			h->v[pos] = h->v[s];
			h->v[s] = aux;
			pos = s;
		}
	}
}

TFisier* extrHeap(THeap *h) {
	TFisier *val = h->v[0];
	h->nrElem--;
	h->v[0] = h->v[h->nrElem];
	pushDown(h,0);
	return val;
}

void DistrugeHeap(THeap **h) {
	free((*h)->v);
	free(*h);
	*h = NULL; 
}