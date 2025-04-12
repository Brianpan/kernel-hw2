#include <list2.h>

typedef struct __node {
    struct __node *left, *right;
    struct __node *next;
    long value;
} node_t;

node_t *list_tail(node_t **list)
{
    node_t *tail = *list;
    while (tail && tail->next)
        tail = tail->next;
    return tail;
}

void list_add(node_t **list, node_t *new_node)
{
    if (!*list) {
        *list = new_node;
        new_node->next = NULL;
    } else {
        node_t *tail = list_tail(list);
        tail->next = new_node;
        new_node->next = NULL;
    }
}

void quick_sort(node_t **list)
{
    int n = list_length(list);
    int value;
    int i = 0;
    int max_level = 2 * n;
    node_t *begin[max_level], *end[max_level];
    node_t *result = NULL, *left = NULL, *right = NULL;

    begin[0] = *list;
    end[0] = list_tail(list);

    while (i >= 0) {
        node_t *L = begin[i], *R = end[i];
        if (L != R) {
            node_t *pivot = L;
            value = pivot->value;
            node_t *p = pivot->next;

            while (p) {
                node_t *n = p;
                p = p->next;
                list_add(n->value > value ? &right : &left, n);
            }

            begin[i] = left;
            end[i] = right;
            begin[i + 1] = pivot;
            end[i + 1] = pivot;
            begin[i + 2] = right;
            end[i + 2] = right;

            left = right = NULL;
            i += 2;
        } else {
            if (L)
                list_add(&result, L);
            i--;
        }
    }
    *list = result;
}

bool check_sort(node_t **list)
{
    node_t *cur = *list;
    while (cur && cur->next) {
        if (cur->value > cur->next->value)
            return false;
        curr = cur->next;

    }
    return true;
}

int main() {
    node_t *list = NULL;
    node_t *node1 = malloc(sizeof(node_t));
    node1->value = 3;
    node_t *node2 = malloc(sizeof(node_t));
    node2->value = 1;
    node_t *node3 = malloc(sizeof(node_t));
    node3->value = 2;

    list_add(&list, node1);
    list_add(&list, node2);
    list_add(&list, node3);

    quick_sort(&list);

    if (check_sort(&list))
        printf("List is sorted\n");
    else
        printf("List is not sorted\n");

    return 0;
}
