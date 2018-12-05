typedef unsigned int uint_t;
#define TOKEN_PASTE(x, y) x ## y
#define NODE_NAME(T) TOKEN_PASTE(list_node_, T)
#define STRUCT_NAME(T) TOKEN_PASTE(list_, T)

struct NODE_NAME(TYPE) {
    TYPE value;
    NODE_NAME(TYPE) *next;
};

struct STRUCT_NAME(TYPE) {
    uint_t length;
    NODE_NAME(TYPE) *first;
    NODE_NAME(TYPE) *last;
};

void list_init(STRUCT_NAME(TYPE)*&);

bool list_is_empty(STRUCT_NAME(TYPE)*);

void list_delete_first(STRUCT_NAME(TYPE)*);

void list_delete(STRUCT_NAME(TYPE)*);

void list_add(STRUCT_NAME(TYPE)*, TYPE);
