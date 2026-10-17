/* DALCU Denisa-Elena 312CB */
#include "structuri.h"

//creeaza coada vida
TCoada* InitQ()
{
	TCoada* c;
	c = (TCoada*)malloc(sizeof(TCoada));
	if (!c)
		return NULL;
	c->sf = NULL;
	return c;
}

//adauga pointer la sfarsitul cozii
int IntrQ(TCoada *c, void* x)
{
	TLista aux;
	aux = (TLista)malloc(sizeof(TCelula));
	if (!aux)
		return 0;
	aux->info = x;
	if(!c->sf) { //coada vida
		aux->urm = aux;
		c->sf = aux;
	} else {
		aux->urm = c->sf->urm;
		c->sf->urm = aux;
		c->sf = aux;
	}
	return 1;
}

//adauga pointer la inceputul cozii
int IntrQFr(TCoada *c, void* x)
{
	TLista aux;
	aux = (TLista)malloc(sizeof(TCelula));
	if (!aux)
		return 0;
	aux->info = x;
	if(!c->sf) { //coada vida
		aux->urm = aux;
		c->sf = aux;
	} else {
		aux->urm = c->sf->urm;
		c->sf->urm = aux;
		//sf ramane acelasi, sf->urm se schimba
	}
	return 1;
}


int ExtrQ(TCoada *c, void **x)  /* extrage primul element din coada la adresa x */
/* intoarce 1: operatie reusita, 0: in caz contrar */
{
	if(c->sf == NULL)
		return 0;
	TLista prim = c->sf->urm;
	*x = prim->info;
	if(prim == c->sf) { //un sg el in coada
		free(c->sf);
		c->sf = NULL;
	} else {
		c->sf->urm = prim->urm;
		free(prim);
	}	
	return 1;
}

//calculeaza lungimea cozii(nr de elemente)
int LungimeQ(TCoada *c)
{
	if(!c->sf)
		return 0;
	int cnt = 1;
	TLista p = c->sf->urm;
	while (p != c->sf) {
		cnt++;
		p = p->urm;
	}
	return cnt;
}

//functie de distrugere coada
void DistrQ(TCoada **c)
{ 
	if ((*c)->sf == NULL) {
		free(*c);
		*c = NULL;
		return;
	}

	TLista p, aux;
	p = (*c)->sf->urm;
	while(p!=(*c)->sf) {
		aux = p;
		p = p->urm;
		free(aux);
	}
	free((*c)->sf);
	free(*c);
}

//functii pentru stiva
int Push(TStiva* vf, void *x)
{
	TStiva aux = (TStiva)malloc(sizeof(TCelulaStiva));
	if	(!aux)
		return 0;
	aux->info = x;
	aux->urm = *vf;
	*vf = aux;
	return 1;
}

int Pop(TStiva* vf, void** x)
{
	TStiva aux;
	if (*vf == NULL)
		return 0;
	*x = (*vf)->info;
	aux = *vf;
	*vf = aux->urm;
	free(aux);
	return 1;
}

void DistrugeS(TStiva* vf)
{
	TStiva aux;
	while (*vf) {
		aux = *vf;
		*vf = (*vf)->urm;
		free(aux);
	}
}

//santinela pt lista de incidente
IncidentList InitIncidentList() {
	IncidentList aux = (IncidentList)malloc(sizeof(IncidentNode));
	if (!aux)
		return NULL;
	aux->info.id = 0;
	strcpy(aux->info.priority, "low");
	aux->info.description = (char*)malloc(strlen("test incident") + 1);
	strcpy(aux->info.description, "test incident");
	strcpy(aux->info.status, "solved");
	aux->pre = aux;
	aux->urm = aux;
	return aux;
}

//santinela pt lista de interventii
InterventionList InitInterventionList() {
	InterventionList aux = (InterventionList)malloc(sizeof(InterventionNode));
	if (!aux)
		return NULL;
	aux->info.incident = NULL;
	aux->info.unit = NULL;
	aux->pre = aux;
	aux->urm = aux;
	return aux;
}


void add_incident(sys *sistem, TCoada *q_high, TCoada *q_medium, 
				TCoada *q_low, int id, char *priority, 
				char *description) {
	IncidentList inc = (IncidentList)malloc(sizeof(IncidentNode));
	if(!inc)
		return;
	inc->info.id = id;
	strcpy(inc->info.priority, priority);
	inc->info.description = (char*)malloc(strlen(description) + 1);
	strcpy(inc->info.description, description);
	strcpy(inc->info.status, "queued");

	//inserare in lista, la final
	IncidentList lista_inc = sistem->incidents;
	inc->urm = lista_inc;
	inc->pre = lista_inc->pre;
	lista_inc->pre->urm = inc;
	lista_inc->pre = inc;

	//salvez in coada coresp prioritatii pointer catre incident
	if(strcmp(priority, "high") == 0)
		IntrQ(q_high, inc);
	else if(strcmp(priority, "medium") == 0)
		IntrQ(q_medium, inc);
	else
		IntrQ(q_low, inc);
	/* -cand se raporteaza un nou incident, il salvez la finalul
	listei de incidente si primeste status queued
	-in functie de severitate, introduc incidentul intr o coada de
	asteptare
	-3 cozi: queue_high, queue_medium, queue_low
	-nu copiez structuri in cozi, ci doar salvez pointeri catre
	celulele listei de incidente
	*/
}

void check_units_availability(TCoada *q_available, FILE *out) {
	int num_available_units = LungimeQ(q_available);
	fprintf(out, "Number of available units: %d\n", num_available_units);
}

void dispatch(sys *sistem, TCoada *q_high, TCoada *q_medium, TCoada *q_low, TCoada *q_available, TStiva *istoric, FILE *out) {
	//verific daca sunt incidente existente
	if (q_high->sf == NULL && q_medium->sf == NULL && q_low->sf == NULL) {
		fprintf(out, "INVALID OPERATION! ERROR 404\n");
		return;
	}
	//verific daca sunt echipaje disponibile
	if (q_available->sf == NULL) {
		fprintf(out, "INVALID OPERATION! ERROR 404\n");
		return;
	}
	//caut mai intai in coada prioritara, apoi spre cea mai neimportanta
	void* p;
	if(q_high->sf != NULL)
		ExtrQ(q_high, &p);
	else if(q_medium->sf != NULL)
		ExtrQ(q_medium, &p);
	else
		ExtrQ(q_low, &p);
	IncidentList inc = (IncidentList)p;

	//extrag primul echipaj disponibil
	ExtrQ(q_available, &p);
	unit *u = (unit*)p;

	//creez o interventioe
	InterventionList interv = (InterventionList)malloc(sizeof(InterventionNode));
	if(!interv)
		return;
	interv->info.incident = &inc->info;
	interv->info.unit = u;

	//inserez in lista de interventii, folosind santinela
	InterventionList s = sistem->interventions;
	interv->urm = s;
	interv->pre = s->pre;
	s->pre->urm = interv;
	s->pre = interv;

	//modific statusurile
	strcpy(inc->info.status, "intervened");
	u->availability = 0;
	//salvez in istoric
	Push(istoric, interv);
}

void undo_last_dispatch(TCoada *q_high, TCoada *q_medium, TCoada *q_low, TCoada *q_available, TStiva *istoric, FILE *out) {
	InterventionList interv = NULL;
	void *p;
	//caut in stiva pana dau de o int nefinalizata
	while (*istoric) {
		Pop(istoric, &p);
		InterventionList k = (InterventionList)p;
		if (strcmp(k->info.incident->status, "solved") != 0) {
			interv = k;
			break;
		}
		//le ignor pe cele solved
	}
	if(interv == NULL) {
		fprintf(out, "INVALID OPERATION! ERROR 404\n");
		return;
	}
	incident *inc = interv->info.incident;
	unit *u = interv->info.unit;
	strcpy(inc->status, "queued");
	IncidentList i = (IncidentList)inc;

	//introduc la inceputul cozii corespunzatoare prioritatii
	if(strcmp(inc->priority, "high") == 0)
		IntrQFr(q_high, i);
	else if(strcmp(inc->priority, "medium") == 0)
		IntrQFr(q_medium, i);
	else
		IntrQFr(q_low, i);
	//schimb statusul echipajului si il introduc inapoi in lista, la final
	u->availability = 1;
	IntrQ(q_available, u);

	//elimin din lista
	interv->pre->urm = interv->urm;
	interv->urm->pre = interv->pre;
	free(interv);
}

void solved_incident(sys *sistem, TCoada *q_available, int id, FILE *out) {
	//caut incidentul dupa id
	IncidentList s = sistem->incidents;
	IncidentList p= s->urm;
	incident *inc = NULL;
	while(p != s) {
		if(p->info.id == id) {
			inc = &p->info;
			break;
		}
		p = p->urm;
	}
	if(inc == NULL) {//id-ul nu este valid
		fprintf(out, "INVALID OPERATION! ERROR 404\n");
		return;
	}
	if(strcmp(inc->status, "intervened") !=0) {
		fprintf(out, "INVALID OPERATION! ERROR 404\n");
		return;
	}

	InterventionList si = sistem->interventions;
	InterventionList pi = si->urm;
	unit *u = NULL;
	while(pi != si) {
		if(pi->info.incident == inc) {
			u = pi->info.unit;
			break;
		}
		pi = pi->urm;
	}
	strcpy(inc->status, "solved");
	if(u) {
		u->availability = 1;
		IntrQ(q_available, u);
	}
}

void show_unit(sys *sistem, int nr_units, int id, FILE *out) {
	for(int i = 0; i< nr_units; i++) {
		if(sistem->units[i].id == id) {
			if(sistem->units[i].availability == 1)
				fprintf(out, "Unit %d is type %c and is available\n", 
				id, sistem->units[i].type);
			else
				fprintf(out, "Unit %d is type %c and is unavailable\n", 
				id, sistem->units[i].type);
			return;	
		}
	}
	fprintf(out, "INVALID OPERATION! ERROR 404\n");
}

void show_incident(sys *sistem, int id, FILE *out) {
	IncidentList s = sistem->incidents;
	IncidentList p = s->urm;
	while (p != s) {
		if(p->info.id == id) {
			fprintf(out, "Incident %d has %s priority, the following description: \"%s\" and is %s\n",
				 id, p->info.priority, p->info.description, p->info.status);
				 return;
		}
		p = p->urm;
	}
	fprintf(out, "INVALID OPERATION! ERROR 404\n");
}

void show_interventions(sys *sistem, FILE *out) {
	InterventionList s = sistem->interventions;
	if(s->urm == s) {
		fprintf(out, "No intervention has been initiated\n");
		return;
	}
	InterventionList p = s->urm;
	while (p != s) {
		fprintf(out, "Incident %d was assigned to unit %d, and has the following status: \"%s\"\n",
		p->info.incident->id, p->info.unit->id, p->info.incident->status);
		p = p->urm;
	}
}
//functie de distrugere a sistemului
void DistrS(sys **sistem, TCoada **q_high, TCoada **q_medium, TCoada **q_low, TCoada **q_available, TStiva *istoric)
{
	DistrQ(q_high);
	DistrQ(q_medium);
	DistrQ(q_low);
	DistrQ(q_available);
	DistrugeS(istoric);
	InterventionList s = (*sistem)->interventions;
	InterventionList j = s->urm;
	while(j != s) {
		InterventionList aux = j;
		j = j->urm;
		free(aux);
	}
	free(s);

	IncidentList si = (*sistem)->incidents;
	IncidentList p = si->urm;
	while (p != si) {
		IncidentList aux = p;
		p = p->urm;
		free(aux->info.description);
		free(aux);
	}
	free(si->info.description);
	free(si);
	free((*sistem)->units);
	free(*sistem);
	*sistem = NULL;
}