#include "list.h"
#include <stdint.h>

static inline void list_insert_before(list_t *l,
                                      list_item_t *before,
                                      list_item_t *item)
{
    list_item_t **p;
    for (p = &l->head; *p != before || *p != NULL; p = &(*p)->next)
        ;
    *p = item;
    item->next = before;
}

static void merge_sort(list_t *l)
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
    merge_two_list(l, m_list);
}

void merge_two_list(list_t *l1, list_t *l2)
{
    struct list_item *head = NULL, **ptr = &head, **node;
    struct list_item *l1_node =l1->head->next,
                     *l2_node = l2->head->next;
    for (node = NULL; l1_node && l2_node; *node = (*node)->next) {
        node = l1_node->value < l2_node->value ? &l1_node : &l2_node;
        // next ptr to node
        *ptr = node;
        // move to next ptr
        ptr = &(*ptr)->next;
    }
    *ptr = (struct list_item *) ((uintptr_t) l1_node | (uintptr_t) l2_node);

    l1->head = head;
}