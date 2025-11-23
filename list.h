/*
 * NODE STRUCT
 */
struct Node
{
	int PFN;
	int pid;
	int VPN;
	int bitRef;
	struct Node *next;
	struct Node *prev;
};

/* DEFINE DOUBLY LINKED LIST
 *
 * CONSTRUCTOR
 */
struct Node *list_new_node(int PFN, int pid, int VPN);
/*
 * INSERTION
 */
struct Node *list_insert_head(struct Node *head, int x, int pid, int VPN);
struct Node *list_insert_tail(struct Node *head, int x, int pid, int VPN);
/*
 * DELETION
 */
struct Node *list_remove_head(struct Node *head);
struct Node *list_remove_tail(struct Node *head);
struct Node *list_remove(struct Node *head, int PFN, int pid, int VPN);
/*
 * SEARCH
 */
struct Node *list_return(struct Node *head, int PFN, int pid, int VPN);
/*
 * DISPLAY
 */
void list_print(struct Node *head);
void list_reverse_print(struct Node *head);
/* END DOUBLY LINKED LIST */

/* DEFINE CIRCULAR LINKED LIST
 *
 * CONSTRUCTOR
 */
struct Node *create(int, int, int);
/*
 * INSERTION
 */
struct Node *insert_begin(struct Node *head, int, int, int);
struct Node *insert_end(struct Node *, int, int, int);
struct Node *insert_mid(struct Node *, int, int, int, int);
/*
 * DELETION
 */
struct Node *delete_begin(struct Node *);
struct Node *delete_end(struct Node *);
struct Node *delete_mid(struct Node *, int);
/*
 * SEARCH
 */
struct Node *search(struct Node *, int, int, int);
int list_size(struct Node *);
/*
 * DISPLAY
 */
void display(struct Node *head);
void display_reverse(struct Node *head);
/* END CIRCULAR LINKED LIST */