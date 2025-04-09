#include <stddef.h>
typedef struct list_item {
    int value;
    struct list_item *next;
} list_item_t;

typedef struct {
    struct list_item *head;
} list_t;

static inline size_t list_size(list_t *l)
{
    size_t size = 0;
    for (list_item_t *cur = l->head; cur != NULL; cur = cur->next)
        size++;
    return size;
}

static inline void list_insert_before(list_t *l,
                                      list_item_t *before,
                                      list_item_t *item);
