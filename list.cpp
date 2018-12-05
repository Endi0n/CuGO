#include "list.h"

void list_init(STRUCT_NAME(TYPE) *&list) {
    list = new STRUCT_NAME(TYPE);
    list->length = 0;
}

bool list_is_empty(STRUCT_NAME(TYPE) *list) {
    return (list->length == 0);
}

void list_delete_first(STRUCT_NAME(TYPE) *list) {
    if (list_is_empty(list)) return;
    NODE_NAME(TYPE) *node = list->first->next;
    delete list->first;
    list->first = node;
    list->length--;
}

void list_delete(STRUCT_NAME(TYPE) *list) {
    while (!list_is_empty(list))
        list_delete_first(list);
    delete list;
}

void list_add(STRUCT_NAME(TYPE) *list, TYPE value) {
    NODE_NAME(TYPE) *node = new NODE_NAME(TYPE);
    node->value = value;
    node->next = nullptr;

    if (list_is_empty(list)) list->first = node;
    else list->last->next = node;

    list->last = node;
    list->length++;
}
