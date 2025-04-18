#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "list2.h"

#define MAP_HASH_SIZE(bits) (1 << (bits))
#define GOLDEN_RATION_32 0x61C88647

struct hlist_head {
    struct hlist_node *first;
};

struct hlist_node {
    struct hlist_node *next, **pprev;
};

// definition of the map structs
typedef struct {
    int bits;
    struct hlist_head *ht;
} map_t;

struct hash_key {
    int key;
    void *data;
    struct hlist_node node;
};

map_t *map_init(int bits)
{
    map_t *map = malloc(sizeof(map_t));
    if (!map)
        return NULL;
    map->bits = bits;
    map->ht = malloc(sizeof(struct hlist_head) * MAP_HASH_SIZE(bits));
    if (map->ht) {
        for (int i = 0; i < MAP_HASH_SIZE(map->bits); i++)
            (map->ht)[i].first = NULL;
    } else {
        free(map);
        map = NULL;
    }
    return map;
}

static inline unsigned int hash(unsigned int val, unsigned int bits)
{
    return (val * GOLDEN_RATION_32) >> (32 - bits);
}

static struct hash_key *find_key(map_t *map, int key)
{
    struct hlist_head *head = &(map->ht)[hash(key, map->bits)];
    // find exact mach of the key from the hash table's hlist
    for (struct hlist_node *p = head->first; p; p = p->next) {
        struct hash_key *kn = container_of(p, struct hash_key, node);
        if (kn->key == key)
            return kn;
    }
    return NULL;
}

void *map_get(map_t *map, int key)
{
    struct hash_key *kn = find_key(map, key);

    return kn ? kn->data : NULL;
}

void map_add(map_t *map, int key, void *data)
{
    struct hash_key *kn = find_key(map, key);
    // key exists
    if (kn)
        return;
    
    kn = malloc(sizeof(struct hash_key));
    if (!kn)
        return;
    kn->key = key, kn->data = data;

    struct hlist_head *h = &map->ht[hash(key, map->bits)];
    struct hlist_node *n = &kn->node, *first = h->first;

    n->next = first;
    // renew pprev of old first node
    if (first)
        first->pprev = &n->next;
    h->first = n;
    n->pprev = &h->first;
}

void map_deinit(map_t *map)
{
    if (!map)
        return;
    
    for (int i = 0; i < MAP_HASH_SIZE(map->bits); i++) {
        struct hlist_head *head = &map->ht[i];
        for (struct hlist_node *p = head->first; p;) {
            struct hash_key *kn = container_of(p, struct hash_key, node);
            struct hlist_node *n = p;
            p = p->next;

            if (!n->pprev) /* unhashed */
                goto bail;
            
            // next's pprev to cur's pperv
            struct hlist_node *next = n->next, **pprev = n->pprev;
            *pprev = next;
            if (next)
                next->pprev = pprev;
            n->next = NULL, n->pprev = NULL;
        bail:
            free(kn->data);
            free(kn);
        }
    }
    free(map);
}

int *twoSum(int *nums, int numsSize, int target, int *returnSize)
{
    map_t *map = map_init(16);
    *returnSize = 0;
    int *ret = malloc(sizeof(int) * 2);
    if (!ret)
        goto bail;
    
    for (int i = 0; i < numsSize; i++) {
        int *p = map_get(map, target - nums[i]);
        if (p) {
            ret[0] = i, ret[1] = *p;
            *returnSize = 2;
            break;
        }

        p = malloc(sizeof(int));
        *p = i;
        map_add(map, nums[i], p);
    }

bail:
    map_deinit(map);
    return ret;
}

int main(void)
{
    int nums[] = {2, 7, 11, 15};
    int target = 13;
    int returnSize;
    int *result = twoSum(nums, sizeof(nums) / sizeof(nums[0]), target, &returnSize);
    
    if (result) {
        printf("Indices: %d, %d\n", result[0], result[1]);
        free(result);
    } else {
        printf("No solution found.\n");
    }
    
    return 0;
}
