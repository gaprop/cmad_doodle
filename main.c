#include "csv.c"

void print_all(list *lst) {
  for (list_p *p = lst->head; p != NULL; p = p->next) {
    printf("%s\n", p->date);
  }
}

int main(int argc, char *argv[]) {
  FILE *fp;
  fp = fopen(argv[1], "rb");
  list* data = parse_csv(fp);
  fclose(fp);

  print_all(data);

  exit(EXIT_SUCCESS);
}
