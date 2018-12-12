#include "list.h"

void list_init(LIST_NAME(TYPE) *&list) {
    list = new LIST_NAME(TYPE);
    list->first = list->last = nullptr;
    list->length = 0;
}

void list_prepend(LIST_NAME(TYPE) *list, const TYPE &value) {
    NODE_NAME(TYPE) *node = new NODE_NAME(TYPE);
    node->value = value;
    node->next = list->first;
    node->prev = nullptr;

    if (!list->first) list->first = node;
    else list->first->prev = node;

    list->first = node;
    list->length++;
}

void list_append(LIST_NAME(TYPE) *list, const TYPE &value) {
    NODE_NAME(TYPE) *node = new NODE_NAME(TYPE);
    node->value = value;
    node->next = nullptr;
    node->prev = list->last;

    if (!list->first) list->first = node;
    else list->last->next = node;

    list->last = node;
    list->length++;
}

NODE_NAME(TYPE)* list_find_node(NODE_NAME(TYPE) *node, const TYPE &value) {
    for (; node; node = node->next) {
        uint_t n = sizeof(TYPE);
        for (
            unsigned char *a = (unsigned char*)&node->value, *b = (unsigned char*)&value;
            n && *a == *b;
            --n, ++a, ++b
        );
        if (!n) return node;
    }
    return nullptr;
}

TYPE* list_find(NODE_NAME(TYPE) *node, const TYPE &value) {
    NODE_NAME(TYPE) *found_node = list_find_node(node, value);
    if (!found_node) return nullptr;
    return &found_node->value;
}

bool list_contains(LIST_NAME(TYPE) *list, const TYPE &value) {
    return (list_find_node(list->first, value) != nullptr);
}

void list_delete(LIST_NAME(TYPE) *list, NODE_NAME(TYPE) *node) {
    if (!node) return;
    if (list->first == node)
        list->first = node->next;
    else {
        node->prev->next = node->next;
        if (node->next) node->next->prev = node->prev;
        else list->last = node->prev;
    }
    delete node;
    list->length--;
}

void list_delete(LIST_NAME(TYPE) *list, const TYPE &value) {
    list_delete(list, list_find_node(list->first, value));
}

void list_delete(LIST_NAME(TYPE) *list) {
    while (!list_is_empty(list))
        list_delete(list, list->last);
    delete list;
}
