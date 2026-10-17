/* DALCU Denisa-Elena 312CB */
#include "structuri.h"

int main()
{
	//deschid fisierele de intrare si iesire
	FILE *in = fopen("tema1.in", "r");
	FILE *out = fopen("tema1.out", "w");

	int nr_units;
	fscanf(in, "%d", &nr_units);

	sys *sistem = (sys *)malloc(sizeof(sys));
	sistem->units = (unit*)malloc(nr_units * sizeof(unit));
	sistem->incidents = InitIncidentList();
	sistem->interventions = InitInterventionList();

	TCoada *queue_high = InitQ();
	TCoada *queue_medium = InitQ();
	TCoada *queue_low = InitQ();
	TCoada *queue_available_units = InitQ();
	TStiva istoric = NULL;

	//citesc echipajele
	for (int i = 0; i < nr_units; i++) {
		fscanf(in, "%d %c", &sistem->units[i].id, &sistem->units[i].type);
		sistem->units[i].availability = 1;
		IntrQ(queue_available_units, &sistem->units[i]);
	}

	int nr_ops;
	fscanf(in, "%d", &nr_ops);

	char op[50];
	for (int i = 0; i < nr_ops; i++) {
		fscanf(in, "%s", op);

		if(strcmp(op, "ADD_INCIDENT") == 0) {
			int id;
			char priority[10];
			char description[100];
			fscanf(in, "%d %s", &id, priority);
			//trb sa sar peste spatii si sa citesc doar ce este intre ghilimele
			fscanf(in, " \"%[^\"]\"", description);
			add_incident(sistem, queue_high, queue_medium, queue_low, id, priority, description);

		} else if (strcmp(op, "CHECK_UNITS_AVAILABILITY") == 0) {
			check_units_availability(queue_available_units, out);
		} else if (strcmp(op, "DISPATCH") == 0) {
			dispatch(sistem, queue_high, queue_medium, queue_low, queue_available_units, &istoric, out);

		} else if (strcmp(op, "UNDO_LAST_DISPATCH") == 0) {
			undo_last_dispatch(queue_high, queue_medium, queue_low, queue_available_units, &istoric, out);
		} else if (strcmp(op, "SOLVED_INCIDENT") == 0) {
			int id;
			fscanf(in, "%d", &id);
			solved_incident(sistem, queue_available_units, id, out);
		} else if (strcmp(op, "SHOW_UNIT") == 0) {
			int id;
			fscanf(in, "%d", &id);
			show_unit(sistem, nr_units, id, out);
		} else if (strcmp(op, "SHOW_INCIDENT") == 0) {
			int id;
			fscanf(in, "%d", &id);
			show_incident(sistem, id, out);
		} else if (strcmp(op, "SHOW_INTERVENTIONS") == 0) {
			show_interventions(sistem, out);
		}
	}
DistrS(&sistem, &queue_high, &queue_medium, &queue_low, &queue_available_units, &istoric);
fclose(in);
fclose(out);
return 0;
}