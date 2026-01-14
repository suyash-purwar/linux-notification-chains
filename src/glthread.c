#include <stdlib.h>
#include <stdio.h>
#include "glthread.h"

#define LIST_SIZE 2
#define GROWTH_FACTOR 2

void *glthread_node_container(const glthread_t *glthread, const glthread_node_t *node) {
    return (char *) node - glthread->offset;
}

glthread_t *glthread_init(glthread_node_t *head_node, const size_t offset) {
    glthread_t *glthread = malloc(sizeof(glthread_t));

    if (head_node != NULL) {
        glthread->head = head_node;
    }

    glthread->offset = offset;

    return glthread;
}

void glthread_init_node(glthread_node_t *node) {
    node->next = NULL;
    node->prev = NULL;
}

int glthread_add_node(glthread_t *glthread, glthread_node_t *node, const Position position) {
    switch (position) {
        case HEAD:
            glthread_add_node_at_head(glthread, node);
            return 0;
        case TAIL:
            glthread_add_node_at_tail(glthread, node);
            return 0;
        default:
            return -1;
    }
}

void glthread_add_node_at_head(glthread_t *glthread, glthread_node_t *node) {
    glthread_node_t *temp = glthread->head;
    glthread->head = node;
    node->next = temp;
    node->prev = NULL;
}

void glthread_add_node_at_tail(glthread_t *glthread, glthread_node_t *node) {
    glthread_node_t *curr_node = glthread->head;

    if (curr_node == NULL) {
        glthread->head = node;
        return;
    }

    while (curr_node->next != NULL) {
        curr_node = curr_node->next;
    }

    curr_node->next = node;
    node->prev = curr_node;
}

int glthread_remove_node(glthread_t *glthread, const Position position) {
    switch (position) {
        case HEAD:
            glthread_remove_node_at_head(glthread);
            return 0;
        case TAIL:
            glthread_remove_node_at_tail(glthread);
            return 0;
        default:
            return -1;
    }
}

void glthread_remove_node_at_head(glthread_t *glthread) {
    free(glthread->head);
    glthread->head = glthread->head->next;
    glthread->head->prev = NULL;
}

void glthread_remove_node_at_tail(const glthread_t *glthread) {
    glthread_node_t *curr_node = glthread->head;

    while (curr_node->next != NULL) {
        curr_node = curr_node->next;
    }

    curr_node->prev->next = NULL;
    free(curr_node);
}

void glthread_free(const glthread_t *glthread) {
    const glthread_node_t *curr_node = glthread->head;

    while (curr_node != NULL) {
        const glthread_node_t *next_node = curr_node->next;

        void *container_struct_addr = glthread_node_container(glthread, curr_node);

        curr_node = next_node;

        free(container_struct_addr);
    }

    free((void *) glthread);
    glthread = NULL;
}

void glthread_print_node(const glthread_t *glthread, const glthread_node_t *node, void print(void *arg)) {
    void *node_container = glthread_node_container(glthread, node);
    print(node_container);
}

size_t glthread_len(const glthread_t *glthread) {
    const glthread_node_t *node = glthread->head;
    size_t size = 0;

    while (node != NULL) {
        node = node->next;
        size++;
    }

    return size;
}

void glthread_foreach(const glthread_t *glthread, void cb(void *arg, size_t index)) {
    const glthread_node_t *curr_node = glthread->head;
    size_t index = 0;

    while (curr_node != NULL) {
        void *node_container = glthread_node_container(glthread, curr_node);
        cb(node_container, index);

        curr_node = curr_node->next;
        index++;
    }
}

void **glthread_where(const glthread_t *glthread, unsigned char cb(const void *arg, size_t index),
                      size_t *out_list_size) {
    const glthread_node_t *curr_node = glthread->head;
    size_t glthread_index = 0;

    void **filtered_nodes_list = malloc(LIST_SIZE * sizeof(void *));
    size_t filtered_nodes_list_max_size = LIST_SIZE;
    size_t filtered_nodes_list_size = 0;

    while (curr_node != NULL) {
        void *node_container = glthread_node_container(glthread, curr_node);

        if (cb(node_container, glthread_index)) {
            filtered_nodes_list[filtered_nodes_list_size] = node_container;
            filtered_nodes_list_size++;
        }

        if (filtered_nodes_list_size == filtered_nodes_list_max_size && curr_node->next != NULL) {
            filtered_nodes_list_max_size *= GROWTH_FACTOR;
            void **temp_filtered_nodes_list = realloc(filtered_nodes_list,
                                                      filtered_nodes_list_max_size * sizeof(void *));

            if (temp_filtered_nodes_list == NULL) {
                perror("Failed to allocate memory");
                exit(-1);
            }

            filtered_nodes_list = temp_filtered_nodes_list;
            temp_filtered_nodes_list = NULL;
        }

        curr_node = curr_node->next;
        glthread_index++;
    }

    *out_list_size = filtered_nodes_list_size;

    return filtered_nodes_list;
}