#include <stdio.h>
#include <stdlib.h>

typedef void *(*Func)(void *);

//Также создаю узел списка
typedef struct Node {
  void *data;
  struct Node *next;
} Node;

//Заполняю список в формате - элемент->остаток списка
Node *cons(void *data, Node *rest) {
  Node *node = malloc(sizeof(Node));

  node->next = rest;
  node->data = data;

  return node;
}

/*
  Эта функця разворачивает результирующий список, 
  поскольку хвостовая рекурсия использует вставку в начало
*/
Node *nreverse(Node *list) {
  Node *new_list = NULL;

  while (list) {
    Node *node = list;
    list = list->next;

    node->next = new_list;
    new_list = node;
  }

  return new_list;
}

// Чтобы избежать переполнения стека - использую хвостовую рекурсию
Node *mapcar_tailrec_worker(Func func, Node *list, Node *new_list) {
  if (!list) {
    return nreverse(new_list);
  }

  new_list = cons(func(list->data), new_list);
  list = list->next;

  return mapcar_tailrec_worker(func, list, new_list);
}

// Возвращает результат рекурсивной функции worker
Node *mapcar_tailrec(Func func, Node *list) {
  return mapcar_tailrec_worker(func, list, NULL);
}

//Упрощает вывод списка и очистку памяти
void foreach (void (*func)(void *), Node * list) {
  while (list) {
    func(list->data);
    list = list->next;
  }
}

// Нужна для очистки памяти
void delete_list(Node *list) {
  while (list) {
    Node *node = list;
    list = list->next;
    free(node);
  }
}

// Пример пользовательской функции, которая передается в mapcar
void *square(void *num) {
  int *n = (int *)num;
  int *result = (int *)malloc(sizeof(int));
  *result = (*n) * (*n);
  return result;
}

//Вывод элементов списка
void print_elem(void *elem) { printf("%d ", *(int *)elem); }

int main(void) {
  int nums[] = {1, 2, 3, 4, 5};

  Node *list = cons(
      &nums[0],
      cons(&nums[1], cons(&nums[2], cons(&nums[3], cons(&nums[4], NULL)))));

  //Получаю обработанный список
  Node *new_list = mapcar_tailrec(square, list);

  foreach (print_elem, new_list);
  foreach (free, new_list);

  delete_list(new_list);
  delete_list(list);

  return 0;
}
