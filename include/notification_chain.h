#ifndef NOTIFICATION_CHAIN_H
#define NOTIFICATION_CHAIN_H

#include "glthread.h"
#include "stdbool.h"

#define MAX_NOTIF_KEY_LEN 64

typedef struct {
    char chain_name[MAX_NOTIF_KEY_LEN];
    glthread_node_t head;
} notification_chain_t;

typedef void (*callback)(void *, size_t);

typedef struct {
    char max[MAX_NOTIF_KEY_LEN];
    unsigned short key_size;
    bool is_key_set;
    callback callback;
    glthread_node_t node;
} notification_chain_element_t;

#endif //NOTIFICATION_CHAIN_H
