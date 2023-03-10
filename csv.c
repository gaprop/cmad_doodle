#include <csv.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>

#define ROOMS 15

typedef enum mark {
  Mark,
  Nothing
} mark;

typedef struct list_p {
  struct list_p *next;

  mark marks[ROOMS];
  char date[30]; // This is more than enough storage

} list_p;

typedef struct list {
  list_p *head;
  list_p *last;
} list;

typedef struct person {
  int room;
  int priority;
  int has_mark;
} person;

typedef struct parsed_data {
  list *lst;
  person persons[ROOMS];
} parsed_data;

typedef struct payload {
  int row;
  int col;

  list *lst;
  person persons[ROOMS];

} payload;

list* list_new() {
  list *ret = malloc(sizeof(list));
  ret->head = NULL;
  ret->last = NULL;
  return ret;
}

void push(void* data, size_t len, list *lst) {
  list_p *p = malloc(sizeof(list_p));
  char *date = (char *) data;

  p->next = NULL;
  for (int i = 0; i < len; i++) {
    p->date[i] = date[i];
  }
  for (int i = 0; i < ROOMS; i++) {
    p->marks[i] = Nothing;
  }

  if (lst->head == NULL) {
    lst->head = p;
    lst->last = p;
  } else {
    lst->last->next = p;
    lst->last = p;
  }
}

void remove_head(list *lst) {
  if (lst->head == NULL) return;

  list_p *p = lst->head;

  lst->head = p->next;
  free(p);
}

void set_mark(int room, mark mark, list *lst) {
  list_p *p = lst->last;
  p->marks[room] = mark;
}

void list_free(list *lst) {
  for (list_p *p; p != NULL; p = lst->head) {
    remove_head(lst);
  }
  free(lst);
}

void cb1 (void *s, size_t len, void *data) {
  payload *payload = (struct payload *) data;
  int col = payload->col;
  if (payload->row == 0) return;

  if (col == 0) {
    push(s, len, payload->lst);
  } else {
    mark m = s ? Mark : Nothing;
    set_mark(col - 1, m, payload->lst);
    if (m == Mark) {
      payload->persons[col - 1].priority++;
      payload->persons[col - 1].has_mark = 1;
    }
  }

  payload->col++;
}

void cb2 (int c, void *data) {
  payload *p = (payload *) data;
  p->row++;
  p->col = 0;
}

parsed_data parse_csv(FILE* fp) {
  struct csv_parser p;
  char buf[1024];
  size_t bytes_read;
  payload payload;
  payload.row = 0;
  payload.col = 0;
  payload.lst = list_new();

  for (int i = 0; i < ROOMS; i++) {
    payload.persons[i].room = 500 + i + 1;
    payload.persons[i].priority = 0;
    payload.persons[i].has_mark = 0;
  }

  if (csv_init(&p, CSV_EMPTY_IS_NULL | CSV_APPEND_NULL) != 0) exit(EXIT_FAILURE);
  if (!fp) exit(EXIT_FAILURE);

  while ((bytes_read=fread(buf, 1, 1024, fp)) > 0) {
    if (csv_parse(&p, buf, bytes_read, cb1, cb2, &payload) != bytes_read) {
      fprintf(stderr, "Error while parsing file: %s\n",
          csv_strerror(csv_error(&p)));
      exit(EXIT_FAILURE);
    }
  }

  csv_fini(&p, cb1, cb2, &payload);

  csv_free(&p);

  parsed_data data;

  data.lst = payload.lst;
  for (int i = 0; i < ROOMS; i++) {
    data.persons[i] = payload.persons[i];
  }

  return data; 
}
