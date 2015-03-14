/*
 * tree.c
 *
 * Build a random tree and print it.
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct node {
    int elem;
    struct node *leftmost_child;
    struct node *right_sibling;
    struct node *parent;
}node_t;

#define ELEM_MAX          150
#define MAX_SIBLING_NUM   4

/*
 * create_random_subtrees
 *
 * Invoked by create_random_tree(). Create random subtree(s) of a parent with their
 * heights no more than max_height. Return the root of the leftmost subtree as the
 * left most child of the parent.
 */
static node_t *
create_random_subtrees(
	node_t *const parent,
	const unsigned int max_height)
{
    unsigned int i;
    unsigned int siblings;
    node_t *leftmost_sibling, *sibling;
    
    if (max_height == 0)
	return NULL;

    leftmost_sibling = (node_t *) malloc(sizeof(node_t));
    leftmost_sibling->elem = (int) ((ELEM_MAX + 1.0) * (rand() / (RAND_MAX + 1.0)));
    leftmost_sibling->leftmost_child =
       	create_random_subtrees(leftmost_sibling,
	       	(unsigned int) (((double) max_height) * (rand() / (RAND_MAX + 1.0))));
    leftmost_sibling->parent = parent;

    /* Create sibling trees. */
    siblings = (unsigned int) (((double) MAX_SIBLING_NUM) * (rand() / (RAND_MAX + 1.0)));
    sibling = leftmost_sibling;
    for (i = 0; i < siblings; i++) {
	sibling->right_sibling = (node_t *) malloc(sizeof(node_t));
	sibling = sibling->right_sibling;
	sibling->elem = (int) ((ELEM_MAX + 1.0) * (rand() / (RAND_MAX + 1.0)));
	sibling->leftmost_child = create_random_subtrees(sibling,
	       	(unsigned int) (((double) max_height) * (rand() / (RAND_MAX + 1.0))));
	sibling->parent = parent;
    }
    sibling->right_sibling = NULL;

    return leftmost_sibling;
}


/*
 * create_random_tree
 *
 * Create a random tree and return its root.
 */
node_t *create_random_tree()
{
    node_t *root;
    unsigned int my_max_height;
    const unsigned int max_height = 20;

    my_max_height = 1 + (unsigned int) (((double) max_height) * (rand() / (RAND_MAX + 1.0)));
    if (my_max_height == 0)
	return NULL;

    root = (node_t *) malloc(sizeof(node_t));
    root->elem = (int) ((ELEM_MAX + 1.0) * (rand() / (RAND_MAX + 1.0)));
    root->leftmost_child = create_random_subtrees(root,
	    (unsigned int) (((double) my_max_height) * (rand() / (RAND_MAX + 1.0))));
    root->right_sibling = NULL;
    root->parent = NULL;

    return root;
}


/*
 * free_tree
 *
 * Deallocate the memory of a tree and its right siblings if it has.
 */
void free_tree(node_t *leftmost_sibling)
{
    if (leftmost_sibling != NULL) {
	free_tree(leftmost_sibling->leftmost_child);
	free_tree(leftmost_sibling->right_sibling);
	free(leftmost_sibling);
    }
}

/*
 * We are going to print a tree-like picture to an output STREAM. Therefore,
 * we need a data structure to store the things needed to print before we
 * actually print a node to the stream. These things are stored in a stack,
 * as we trace back to the root from the node we are going to print.
 */
typedef struct p_stack_node {
    const char *branch_str;               /* pointing to one of the four branch strings below */
    struct p_stack_node *next;
}p_stack_node_t;

static const char *const branch_urhhhh = "└──";
static const char *const branch_vrhhhh = "├──";
static const char *const branch_vvspsp = "│  ";
static const char *const branch_spspsp = "   ";

/*
 * print_node
 *
 * Print a specific node of a tree to an output stream of fp.
 * Note that the node is assumed to be non-empty, i.e., pointer to the
 * node is not NULL.
 */
static void fprint_node(FILE *fp, const node_t *const node)
{
    const node_t *trace;
    p_stack_node_t *p_stack_top, *p_stack_aux;

    if (node->parent != NULL) {    /* node is not root */
	p_stack_top = (p_stack_node_t *) malloc(sizeof(p_stack_node_t));
	p_stack_top->branch_str =
	    node->right_sibling == NULL ? branch_urhhhh : branch_vrhhhh;
	p_stack_top->next = NULL;
    }
    else
	p_stack_top = NULL;

    trace = node->parent;
    p_stack_aux = p_stack_top;

    /* while the depth of trace is greater than 0 */
    while (trace != NULL && trace->parent != NULL) {
	/* stack push */
	p_stack_top = (p_stack_node_t *) malloc(sizeof(p_stack_node_t));
	p_stack_top->branch_str =
	    trace->right_sibling == NULL ? branch_spspsp : branch_vvspsp;
	p_stack_top->next = p_stack_aux;
	p_stack_aux = p_stack_top;

	trace = trace->parent;
    }

    /* Print and free stack */
    while (p_stack_top != NULL) {
	fprintf(fp, "%s", p_stack_top->branch_str);
	p_stack_aux = p_stack_top;
	p_stack_top = p_stack_top->next;
	free(p_stack_aux);
    }
    fprintf(fp, "%d\n", node->elem);
}

/*
 * print_tree
 *
 * Print a tree by pre-traverse order to the file pointed to by fp. That is,
 * the element in root is printed first, then the subtrees is recursively
 * printed from left to right.
 */
void fprint_tree(FILE *fp, node_t *root)
{
    /*
     * The implementation is non-recursive, therefore we need to use a stack
     * for backtrace. Since each node in a tree contains a pointer to its
     * parent, we can use part of the tree as a stack. Thus, only the head
     * of stack needs to be known.
     */
    node_t *stack_head;

    stack_head = root;

    if (stack_head == NULL)
	fprintf(fp, "(empty tree)\n");

    while (stack_head != NULL) {
	fprint_node(fp, stack_head);
	while (stack_head->leftmost_child != NULL) {
	    stack_head = stack_head->leftmost_child;
	    fprint_node(fp, stack_head);
	}

	if (stack_head->right_sibling != NULL)
	    stack_head = stack_head->right_sibling;
	else {
	    do {
		stack_head = stack_head->parent;
	    } while(stack_head != NULL && stack_head->right_sibling == NULL);

	    if (stack_head != NULL)
		stack_head = stack_head->right_sibling;
	}
    }
}


int main()
{
    node_t *rooted_tree, *aux, *aux2;

    srand(time(NULL));
    rooted_tree = create_random_tree();
    fprint_tree(stdout, rooted_tree);
    free_tree(rooted_tree);

    return 0;
}

