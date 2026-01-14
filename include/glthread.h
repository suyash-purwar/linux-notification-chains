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

typedef struct {
    glthread_node_t *head;
    unsigned int offset;
} glthread_t;

glthread_t *glthread_init(glthread_node_t *, size_t);

void glthread_init_node(glthread_node_t *);

void *glthread_node_container(const glthread_t *, const glthread_node_t *);

void glthread_print_node(const glthread_t *, const glthread_node_t *, void (void *));

int glthread_add_node(glthread_t *, glthread_node_t *, Position);

void glthread_add_node_at_head(glthread_t *, glthread_node_t *);

void glthread_add_node_at_tail(glthread_t *, glthread_node_t *);

int glthread_remove_node(glthread_t *, Position);

void glthread_remove_node_at_head(glthread_t *);

void glthread_remove_node_at_tail(const glthread_t *);

void glthread_free(const glthread_t *);

size_t glthread_len(const glthread_t*);

void glthread_foreach(const glthread_t *, void (void *, size_t));

void** glthread_where(const glthread_t*, unsigned char (const void *, size_t), size_t*);

#endif