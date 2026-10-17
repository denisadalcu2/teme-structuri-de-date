/* DALCU Denisa-Elena 312CB */
#ifndef STRUCTURI_H
#define STRUCTURI_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* definire lista dublu inlantuita circulara cu santinela */
typedef struct celula2 {
	int info;
	struct celula2 *pre, *urm;
} TCelula2, *TL2;

//coada->celulele stocheaza void*
typedef struct celula{
	void *info;
	struct celula *urm;
} TCelula, *TLista;

typedef struct coada
{
	TLista sf;
} TCoada;

//stiva
typedef struct celst
{
	void* info;
	struct celst *urm;
} TCelulaStiva, *TStiva;

//structuri pentru sistem
//definire echipaj de interventie
typedef struct unit {
	int id;
	char type;
	int availability; //0 = unavailable, 1 = available
} unit;


//definire incident
typedef struct incident {
	int id;
	char priority[7]; // low/medium/high
	char *description;
	char status[11]; // queued/intervened/solved
} incident;

//definire interventie
typedef struct intervention {
	incident *incident;
	unit *unit;
} intervention;

//celulele pt listele dublu inlantuite
typedef struct IncidentNode {
	incident info;
	struct IncidentNode *pre, *urm;
} IncidentNode, *IncidentList;

typedef struct InterventionNode {
	intervention info;
	struct InterventionNode *pre, *urm;
} InterventionNode, *InterventionList;

/* definire sistem de gestionare a apelurilor de urgenta*/
typedef struct sys {
	unit* units;
	IncidentList incidents;
	InterventionList interventions;
} sys;

TCoada* InitQ();
int IntrQ(TCoada *c, void* x);
int IntrQFr(TCoada *c, void* x);
int ExtrQ(TCoada *c, void **x);
int LungimeQ(TCoada *c);
void DistrQ(TCoada **c);
int Push(TStiva* vf, void *x);
int Pop(TStiva* vf, void** x);
void DistrugeS(TStiva* vf);
IncidentList InitIncidentList();
InterventionList InitInterventionList();

void add_incident(sys *sistem, TCoada *q_high, TCoada *q_medium, 
				TCoada *q_low, int id, char *priority, 
				char *description);
void check_units_availability(TCoada *q_available, FILE *out);
void dispatch(sys *sistem, TCoada *q_high, TCoada *q_medium, TCoada *q_low, 
			TCoada *q_available, TStiva *istoric, FILE *out);
void undo_last_dispatch(TCoada *q_high, TCoada *q_medium, TCoada *q_low, 
						TCoada *q_available, TStiva *istoric, FILE *out);
void solved_incident(sys *sistem, TCoada *q_available, int id, FILE *out);
void show_unit(sys *sistem, int nr_units, int id, FILE *out);
void show_incident(sys *sistem, int id, FILE *out);
void show_interventions(sys *sistem, FILE *out);
void DistrS(sys **sistem, TCoada **q_high, TCoada **q_medium, 
			TCoada **q_low, TCoada **q_available, TStiva *istoric);
#endif