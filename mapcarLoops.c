#include <stdio.h>
#include <stdlib.h>

typedef void *(*Func)(void *);

// Узел списка
typedef struct Node {
  void *data; 
  struct Node *next; 
} Node;

// Функция заполняет список в формате - элемент->остаток списка
Node *cons(void *data, Node *rest) {
  Node *node = malloc(sizeof(Node));

  node->next = rest;
  node->data = data;

  return node;
}

// Эта функция разворачивает список, который получаем в функции mapcar
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
// Функция mapcar: принимает функцию и список, возвращает измененный список
Node *mapcar(Func func, Node *list) {
  Node *new_list = NULL;

  while (list) {
    new_list = cons(func(list->data), new_list);
    list = list->next;
  }
 
  return nreverse(new_list);
}

// Функция для вывода списка и очистки памяти
void foreach (void (*func)(void *), Node * list) {
  while (list) {
    func(list->data);
    list = list->next;
  }
}

// Функция для очистки выделенной памяти
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

// Выводит на экран полученный список
void print_elem(void *elem) {
  printf("%d ", *(int *)elem);
}

int main(void) {
  int nums[] = {1, 2, 3, 4};

  // Просто заполняю список 
  Node *list =
    cons(&nums[0], cons(&nums[1], cons(&nums[2], cons(&nums[3], NULL))));


  //Получаем обработанный с помощью mapcar список 
  Node *new_list = mapcar(square, list);

  foreach (print_elem, new_list);
  foreach (free, new_list);

  delete_list(new_list);
  delete_list(list);

  return 0;
}