#include "assets.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#define INITIAL_BUCKETS 256

typedef struct assets_node_t {
    char *key;
    texture_t *texture;
    struct assets_node_t *next;
} assets_node_t;

typedef struct {
    assets_node_t **buckets;
    size_t size;
    size_t count;
} assets_table_t;

static assets_table_t assets_table;

static unsigned long hash_djb2(const char *str) {
    unsigned long hash = 5381;
    int c;
    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c; // hash * 33 + c
    }
    return hash;
}

void assets_init(void) {
    assets_table.size = INITIAL_BUCKETS;
    assets_table.count = 0;
    assets_table.buckets = calloc(assets_table.size, sizeof(assets_node_t *));
}

void assets_shutdown(void) {
    for(size_t i = 0; i < assets_table.size; i++) {
        assets_node_t *node = assets_table.buckets[i];
        while(node != NULL) {
            assets_node_t *next = node->next;
            platform_free_texture(node->texture);
            free(node->key);
            free(node);
            node = next;
        }
    }
    free(assets_table.buckets);
    assets_table.buckets = NULL;
    assets_table.size = 0;
    assets_table.count = 0;
}

static void assets_put(const char *key, texture_t *t) {
    if(!key || !t) return;

    unsigned long hash = hash_djb2(key);
    size_t idx = hash % assets_table.size;

    assets_node_t *current = assets_table.buckets[idx];
    while(current != NULL) {
        if(strcmp(current->key, key) == 0) {
            current->texture = t;
            return;
        }
        current = current->next;
    }

    assets_node_t *new_node = malloc(sizeof(assets_node_t));
    new_node->texture = t;
    new_node->key = strdup(key);

    new_node->next = assets_table.buckets[idx];
    assets_table.buckets[idx] = new_node;
    assets_table.count++;
}

texture_t* assets_get_texture(renderer_t *r, const char *path) {
    if(!r || !path) return NULL;

    unsigned long hash = hash_djb2(path);
    size_t idx = hash % assets_table.size;

    assets_node_t *current = assets_table.buckets[idx];
    while(current != NULL) {
        if(strcmp(current->key, path) == 0) {
            return current->texture;
        }
        current = current->next;
    }

    texture_t *t = platform_load_texture(r, path);
    if(t) {
        assets_put(path, t);
    }
    return t;
}

int assets_has_texture(const char *path) {
    if(!path) return 0;

    unsigned long hash = hash_djb2(path);
    size_t idx = hash % assets_table.size;

    assets_node_t *current = assets_table.buckets[idx];
    while(current != NULL) {
        if(strcmp(current->key, path) == 0) return 1;
        current = current->next;
    }
    return 0;
}

int assets_unload_texture(const char *path) {
    if(!path) return 1;

    unsigned long hash = hash_djb2(path);
    size_t idx = hash % assets_table.size;

    assets_node_t *cur = assets_table.buckets[idx];
    assets_node_t *prev = NULL;
    while(cur != NULL) {
        if(strcmp(cur->key, path) == 0) {
            if(prev == NULL) {
                assets_table.buckets[idx] = cur->next;
            } else {
                prev->next = cur->next;
            }
            platform_free_texture(cur->texture);
            free(cur->key);
            free(cur);
            assets_table.count--;
            return 0;
        }
        prev = cur;
        cur = cur->next;
    }
    return 2;
}
