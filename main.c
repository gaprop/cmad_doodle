#include "csv.c"

void print_all(list *lst) {
  for (list_p *p = lst->head; p != NULL; p = p->next) {
    printf("%s\n", p->date);
  }
}

void print_rooms(person persons[15]) {
  for (int i = 0; i < 15; i++) {
    printf("%d\n", persons[i].room);
    printf("%d\n", persons[i].priority);
  }
}

void gen_date(list_p *p, person **ps) {
  person *persons = *ps;
  int max_prio = 0;
  int room = 15; // We set this to an arbitrary number to make it clear if something happens
  for (int i = 0; i < ROOMS; i++) {
    int prio = persons[i].priority;
    mark m = p->marks[i];
    if ((prio > max_prio) && (m == Mark)) {
      max_prio = prio;
      room = i;
    }
  }

  for (int i = 0; i < ROOMS; i++) {
    int prio = persons[i].priority;
    int has_mark = persons[i].has_mark;
    if (prio > max_prio && (has_mark == 0)) {
      max_prio = prio;
      room = i;
    }
  }

  for (int i = 0; i < ROOMS; i++) {
    persons[i].priority++;
  }
  persons[room].priority = 0;

  printf("%s,", p->date);
  printf("%d\n", 500 + room + 1);
}

void gen_schedule(list *lst, person persons[ROOMS]) {
  list_p *p = lst->head;
  while (p != NULL) {
    gen_date(p, &persons);

    remove_head(lst);
    p = lst->head;
  }
}

/* void print_marks(list *lst) { */
  /* list_p *p = lst->head; */
  /* while (p != NULL) { */
    /* for (int i = 0; i < ROOMS; i++) { */
      /* if (p->marks[i] == Mark) { */
        /* printf("x,"); */
      /* } else { */
        /* printf(" ,"); */
      /* } */
    /* } */
    /* printf("\n"); */

    /* remove_head(lst); */
    /* p = lst->head; */
  /* } */
/* } */

int main(int argc, char *argv[]) {
  FILE *fp;
  fp = fopen(argv[1], "rb");
  parsed_data data = parse_csv(fp);
  fclose(fp);

  gen_schedule(data.lst, data.persons);

  list_free(data.lst);

  exit(EXIT_SUCCESS);
}
