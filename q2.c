#include <assert.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct block {
    size_t size;
    struct block *l, *r;
} block_t;

block_t **find_free_tree(block_t **root, block_t *target)
{
    block_t **node_ptr = root,
            **fit = NULL;

    while (*node_ptr) {
        if ((*node_ptr)->size == target->size)
            return node_ptr;
        
        if ((*node_ptr)->size > target->size)
            fit = node_ptr;
        node_ptr = target->size < (*node_ptr)->size ? &(*node_ptr)->l : &(*node_ptr)->r;
    }
    // not found target, return the fit
    return fit;
}

// predecessor is which node is the largest node in the left subtree
block_t *find_predecessor_free_tree(block_t **root, block_t *node)
{
    block_t **node_ptr = root;
    block_t *pred = NULL;
    while (*node_ptr) {
        if (((*node_ptr)->size < node->size) && (!pred || ((*node_ptr)->size > pred->size && (*node_ptr)->size < node->size)))
            pred = *node_ptr;
        node_ptr = (*node_ptr)->size >= node->size ? &(*node_ptr)->l : &(*node_ptr)->r;
    }

    return pred;
}

/*
 * Structure representing a free memory block in the memory allocator.
 * The free tree is a binary search tree that organizes free blocks (of type block_t)
 * to efficiently locate a block of appropriate size during memory allocation.
 */
void remove_free_tree(block_t **root, block_t *target)
{
    /* Locate the pointer to the target node in the tree. */
    block_t **node_ptr = find_free_tree(root, target);

    /* If the target node has two children, we need to find a replacement. */
    if ((*node_ptr)->l && (*node_ptr)->r) {
        /* Find the in-order predecessor:
         * This is the rightmost node in the left subtree.
         */
        block_t **pred_ptr = &(*node_ptr)->l;
        while (*pred_ptr && (*pred_ptr)->r)
            pred_ptr = &(*pred_ptr)->r;

        /* Verify the found predecessor using a helper function (for debugging).
         */
        block_t *expected_pred = find_predecessor_free_tree(root, *node_ptr);
	    assert(expected_pred == *pred_ptr);

        /* If the predecessor is the immediate left child. */
        if (*pred_ptr == (*node_ptr)->l) {
            block_t *old_right = (*node_ptr)->r;
            *node_ptr = *pred_ptr; /* Replace target with its left child. */
            (*node_ptr)->r = old_right; /* Attach the original right subtree. */
            assert(*node_ptr != (*node_ptr)->l);
            assert(*node_ptr != (*node_ptr)->r);
        } else {
            /* The predecessor is deeper in the left subtree. */
            block_t *old_left = (*node_ptr)->l;
            block_t *old_right = (*node_ptr)->r;
            block_t *pred_node = *pred_ptr;
            /* Remove the predecessor from its original location. */
            remove_free_tree(&old_left, *pred_ptr);
            /* Replace the target node with the predecessor. */
            *node_ptr = pred_node;
            (*node_ptr)->l = old_left;
            (*node_ptr)->r = old_right;
            assert(*node_ptr != (*node_ptr)->l);
            assert(*node_ptr != (*node_ptr)->r);
        }
    }
    /* If the target node has one child (or none), simply splice it out. */
    else if ((*node_ptr)->l || (*node_ptr)->r) {
        block_t *child = ((*node_ptr)->l) ? (*node_ptr)->l : (*node_ptr)->r;
        *node_ptr = child;
    } else {
        /* No children: remove the node. */
        *node_ptr = NULL;
    }

    /* Clear the removed node's child pointers to avoid dangling references. */
    target->l = NULL;
    target->r = NULL;
}

block_t* create_block(size_t size)
{
    block_t *block = malloc(sizeof(block_t));
    if (!block) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    block->size = size;
    block->l = NULL;
    block->r = NULL;
    return block;
}

// naive implementation without tree balancing
void insert_free_tree(block_t **root, size_t size)
{
    block_t *new_block = create_block(size);
    assert(new_block != NULL);
    block_t **node_ptr = root;
    while (*node_ptr) {
        if (size < (*node_ptr)->size)
            node_ptr = &(*node_ptr)->l;
        else
            node_ptr = &(*node_ptr)->r;    
    }
    *node_ptr = new_block;
}

void test0()
{
    // insert the tree to the root
    block_t **root;
    root = malloc(sizeof(block_t*));
    //** // tree is 
    ////  10
    ////   
    //// 5    15
    ///      
    // 2   7 12 20 
    //
    insert_free_tree(root, 10);
    insert_free_tree(root, 5);
    insert_free_tree(root, 15);
    insert_free_tree(root, 2);
    insert_free_tree(root, 7);
    insert_free_tree(root, 12);
    insert_free_tree(root, 20);

    block_t *target = create_block(10);
    remove_free_tree(root, target);
    // check the tree
    assert((*root)->size == 7);
    

}
int main(void)
{
    test0();

    return 0;
}
