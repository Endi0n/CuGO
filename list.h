typedef unsigned int uint_t;
#define TOKEN_PASTE(x, y) x ## y
#define NODE_NAME(T) TOKEN_PASTE(list_node_, T)
#define LIST_NAME(T) TOKEN_PASTE(list_, T)

struct NODE_NAME(TYPE) {
    TYPE value;
    NODE_NAME(TYPE) *next;
    NODE_NAME(TYPE) *prev;
};

struct LIST_NAME(TYPE) {
    uint_t length;
    NODE_NAME(TYPE) *first;
    NODE_NAME(TYPE) *last;
};

void list_init(LIST_NAME(TYPE)*&);

inline bool list_is_empty(LIST_NAME(TYPE) *list) { return !list->length; }

void list_prepend(LIST_NAME(TYPE)*, const TYPE&);

void list_append(LIST_NAME(TYPE)*, const TYPE&);

NODE_NAME(TYPE)* list_find_node(NODE_NAME(TYPE)*, const TYPE&);

TYPE* list_find(NODE_NAME(TYPE)*, const TYPE&);

bool list_contains(LIST_NAME(TYPE)*, const TYPE&);

void list_delete(LIST_NAME(TYPE)*, NODE_NAME(TYPE)*);

void list_delete(LIST_NAME(TYPE)*, const TYPE&);

void list_delete(LIST_NAME(TYPE)*&);
