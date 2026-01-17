#ifndef GLTHREAD_H
#define GLTHREAD_H

#include <stddef.h>

typedef enum {
    HEAD,
    TAIL
} Position;

typedef struct glthread_node {
    struct glthread_node *prev;
    struct glthread_node *next;
} glthread_node_t;

#define GLTHREAD_NODE_CONTAINER(fn_name, struct_name, field_key)                    \
static inline struct_name *fn_name(glthread_node_t* node) {                     \
return (struct_name*)((char*) node - offsetof(struct_name, field_key));     \
}

void glthread_init_node(glthread_node_t *);

glthread_node_t* glthread_get_last_node(glthread_node_t *);

int glthread_add_node(glthread_node_t **, glthread_node_t *, Position);

void glthread_add_node_at_head(glthread_node_t **, glthread_node_t *);

void glthread_add_node_at_tail(glthread_node_t **, glthread_node_t *);

glthread_node_t* glthread_remove_node(glthread_node_t **, Position);

glthread_node_t* glthread_remove_node_at_head(glthread_node_t **);

glthread_node_t* glthread_remove_node_at_tail(glthread_node_t **);

size_t glthread_len(const glthread_node_t *);

void glthread_foreach(const glthread_node_t *, void (glthread_node_t *, size_t));

glthread_node_t **glthread_where(glthread_node_t *, unsigned char (glthread_node_t *, size_t), size_t *);

#endif
