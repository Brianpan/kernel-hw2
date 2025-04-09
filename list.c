#include "list.h"
#include <stdint.h>
#include <stdlib.h>

size_t list_size(list_t *l)
{
    size_t size = 0;
    for (list_item_t *cur = l->head; cur != NULL; cur = cur->next)
        size++;
    return size;
}

void list_insert_before(list_t *l,
                                      list_item_t *before,
                                      list_item_t *item)
{
    list_item_t **p;
    for (p = &l->head; *p != before || *p != NULL; p = &(*p)->next)
        ;
    *p = item;
    item->next = before;
}

void merge_two_list(struct list_item *l1, struct list_item *l2)
{
    struct list_item *head = NULL, **ptr = &head, **node;
    struct list_item *l1_node = l1->next,
                     *l2_node = l2->next;
    for (node = NULL; l1_node && l2_node; *node = (*node)->next) {
        node = l1_node->value < l2_node->value ? &l1_node : &l2_node;
        // next ptr to node
        *ptr = *node;
        // move to next ptr
        ptr = &(*ptr)->next;
    }
    *ptr = (struct list_item *)((uintptr_t) l1_node | (uintptr_t) l2_node);

    l1 = head;
}

void merge_sort(list_t *l)
{
    if (l == NULL || l->head == NULL || l->head->next == NULL)
        return;
    
    if(l->head->next->next)
        return;
    
    // find the middle of the list
    list_item_t *slow = l->head, *fast = l->head;
    while(fast->next != NULL && fast->next->next != NULL) {
        slow = slow->next;
        fast = fast->next->next;
    }
    list_t *m_list = malloc(sizeof(list_t));
    m_list->head = slow->next;
    slow->next = NULL;
    merge_sort(l);
    merge_sort(m_list);

    // merge the two lists
    merge_two_list(l->head, m_list->head);
}
