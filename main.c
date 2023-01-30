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
  int max_prio = persons[0].priority;
  int room = 0;
  for (int i = 0; i < ROOMS; i++) {
    int prio = persons[i].priority;
    if (prio > max_prio) {
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

int main(int argc, char *argv[]) {
  FILE *fp;
  fp = fopen(argv[1], "rb");
  parsed_data data = parse_csv(fp);
  fclose(fp);

  gen_schedule(data.lst, data.persons);

  list_free(data.lst);

  exit(EXIT_SUCCESS);
}
