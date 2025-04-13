#include <stdint.h>
#include "list2.h"

#define ARRAY_SIZE(x) (sizeof(x) / sizeof(x[0]))

struct listitem {
    uint16_t i;
    struct list_head list;
};

static inline int cmpint(const void *p1, const void *p2)
{
    const uint16_t *i1 = (const uint16_t *)p1;
    const uint16_t *i2 = (const uint16_t *)p2;

    return *i1 - *i2;
}

static uint16_t values[256];

static inline uint8_t getnum(void)
{
    static uint16_t s1 = UINT16_C(2);
    static uint16_t s2 = UINT16_C(1);
    static uint16_t s3 = UINT16_C(1);

    s1 *= UINT16_C(171);
    s1 %= UINT16_C(30269);

    s2 *= UINT16_C(172);
    s2 %= UINT16_C(30307);

    s3 *= UINT16_C(170);
    s3 %= UINT16_C(30323);

    return s1 ^ s2 ^ s3;
}

static uint16_t get_unsigned16(void)
{
    uint16_t x = 0;
    size_t i;

    for (i = 0; i < sizeof(x); i++) {
        x <<= 8;
        x |= getnum();
    }

    return x;
}

static inline random_shuffle_array(uint16_t *operations, uint16_t len)
{
    for (uint16_t i = 0; i < len; i++) {
        uint16_t j = get_unsigned16() % (i + 1);
        operations[i] = operations[j];
        operations[j] = i;
    }
}

static void list_quicksort(struct list_head *head)
{
    struct list_head list_less, list_greater;
    struct listitem *pivot;
    struct listitem *item = NULL, *is = NULL;

    if (list_empty(head) || list_is_singular(head))
        return;
    
    INIT_LIST_HEAD(&list_less);
    INIT_LIST_HEAD(&list_greater);

    // get the pivot and remove it from the circular list
    pivot = list_first_entry(head, struct listitem, list);
    list_del(&pivot->list);

    list_for_each_entry_safe (item, is, head, list) {
        if (cmpint(&item->i, &pivot->i) < 0)
            list_move_tail(&item->list, &list_less);
        else
            list_move_tail(&item->list, &list_greater);
    }

    list_quicksort(&list_less);
    list_quicksort(&list_greater);

    list_add(&pivot->list, head);
    list_splice(&list_less, head);
    list_splice_tail(&list_greater, head);
}

int main(void)
{
    struct list_head testlist;
    struct listitem *item, *is = NULL;
    size_t i;

    random_shuffle_array(values, ARRAY_SIZE(values));

    INIT_LIST_HEAD(&testlist);

    assert(list_empty(&testlist));
    for (i = 0; i < ARRAY_SIZE(values); i++) {
        item = malloc(sizeof(*item));
        assert(item);
        item->i = values[i];
        list_add_tail(&item->list, &testlist);
    }

    assert(!list_empty(&testlist));

    list_quicksort(&testlist);

    i = 0;
    list_for_each_entry_safe (item, is, &testlist, list) {
        assert(item->i == values[i]);
        list_del(&item->list);
        free(item);
        i++;
    }

    assert(i == ARRAY_SIZE(values));
    assert(list_empty(&testlist));

    return 0;
}