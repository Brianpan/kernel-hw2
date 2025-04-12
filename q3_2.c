#include <stdio.h>
#include <stdlib.h>
#include "list2.h"

typedef struct __node {
    long value;
    struct list_head list;
} node_t;

// testing utility
void list_construct(struct list_head *list, int n)
{
    node_t *node = malloc(sizeof(node_t));
    node->value = n;
    list_add(&node->list, list);
}

void list_free(const struct list_head *head)
{
    node_t *entry, *safe;
    list_for_each_entry_safe(entry, safe, head, list)
    {
        list_del(&entry->list);
        free(entry);
    }
}

static int list_is_ordered(const struct list_head *head)
{
    int value = list_entry(head->next, node_t, list)->value;
    node_t *entry;
    list_for_each_entry (entry, head, list)
    {
        if (entry->value < value)
            return 0;
        value = entry->value;
    }

    return 1;
}

void shuffle(int *arr, size_t n)
{
    if (n <= 1)
        return;
    for (size_t i = n - 1; i > 0; i--) {
        size_t j = rand() % (i + 1);
        int temp = arr[i];
        arr[i] = arr[j];
        arr[j] = temp;
    }
}

// accessory functions
struct list_head* list_tail(struct list_head *list)
{
    struct list_head *tail = list->next;
    while (tail && tail->next)
        tail = tail->next;
    return tail;
}

int list_length(struct list_head *list)
{
    int count = 0;
    struct list_head *cur;
    list_for_each(cur, list)
    {
        count++;
    }
    return count;
}

static void rebuild_list_link(struct list_head *head)
{
    if (!head)
        return;
    struct list_head *node, *prev;
    prev = head;
    node = head->next;
    while (node) {
        node->prev = prev;
        prev = node;
        node = node->next;
    }
    prev->next = head;
    head->prev = prev;
}

// quick sort
void quick_sort(struct list_head *list)
{
    int n = list_length(list);
    int value;
    int i = 0;
    int max_level = 2 * n;

    struct list_head *begin[max_level];
    struct list_head *result = NULL, *left = NULL, *right = NULL;
    begin[0] = list->next;
    // disable circular list
    list->prev->next = NULL;
    while (i >= 0) {
        struct list_head *L = begin[i], *R = list_tail(begin[i]);
        if (L != R) {
            struct list_head *pivot = L;
            value = list_entry(pivot, node_t, list)->value;
            struct list_head *p = pivot->next;
            pivot->next = NULL;

            while (p) {
                struct list_head *n = p;
                p = p->next;
                p->prev = NULL;
                n->next = NULL;
                list_add(n, list_entry(n, node_t, list)->value > value ? &right : &left);
            }

            begin[i] = left;
            begin[i + 1] = pivot;
            begin[i + 2] = right;
            left = right = NULL;
            i += 2;
        } else {
            if (L) {
                L->next = result;
                result = L;
            }
            i--;
        }
        list->next = result;
        rebuild_list_link(list);
    }
}

int main(int argc, char **argv)
{
    struct list_head *list = malloc(sizeof(struct list_head));
    if (!list) {
        fprintf(stderr, "Memory allocation failed\n");
        return EXIT_FAILURE;
    }
    INIT_LIST_HEAD(list);
    size_t count = 10000;
    int *arr = malloc(count * sizeof(int));
    if (!arr) {
        fprintf(stderr, "Memory allocation failed\n");
        free(list);
        return EXIT_FAILURE;
    }
    for (size_t i = 0; i < count; i++)
        arr[i] = i;
    shuffle(arr, count);
    while (count--) {
        list_construct(list, arr[count]);
    }
    quick_sort(list);
    assert(list_is_ordered(list));
    list_free(list);
    free(list);
    free(arr);
    return 0;
}