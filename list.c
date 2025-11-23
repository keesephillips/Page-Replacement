#include <stdio.h>
#include <stdlib.h>
#include "list.h"
#include "vm.h"

// Creates a new Node and returns pointer to it.
struct Node *list_new_node(int PFN, int pid, int VPN)
{
	struct Node *newNode = (struct Node *)malloc(sizeof(struct Node));
	newNode->PFN = PFN;
	newNode->bitRef = 0;
	newNode->pid = pid;
	newNode->VPN = VPN;
	newNode->prev = NULL;
	newNode->next = NULL;
	return newNode;
}

struct Node *list_remove_tail(struct Node *head)
{
	if (head == NULL)
		return NULL;

	struct Node *ret = head;

	while (ret->next != NULL)
		ret = ret->next; // Go To last Node
	ret->prev->next = NULL;
	return head;
}

struct Node *list_remove_head(struct Node *head)
{
	if (head == NULL)
		return NULL;

	struct Node *ret = head;
	head = head->next;
	if (head != NULL)
		head->prev = NULL;

	return head;
}

struct Node *list_return(struct Node *head, int PFN, int pid, int VPN)
{
	if (head == NULL)
		return NULL;

	if ((PFN == head->PFN) && ((pid == head->pid) && (VPN == head->VPN)))
	{
		return head;
	}

	struct Node *ret = head;
	while (ret->next != NULL)
	{
		if ((PFN == ret->PFN)) //&& (pid == ret->pid) && (VPN == ret->VPN))
		{
			return head;
		}
		ret = ret->next;
	}
	return NULL;
}

struct Node *list_remove(struct Node *head, int PFN, int pid, int VPN)
{
	if (head == NULL)
		return NULL;

	struct Node *ret = head;

	while (ret != NULL)
	{
		if (PFN == ret->PFN)
		{
			if (ret == head)
				return list_remove_head(head);
			if (ret->next == NULL)
				return list_remove_tail(head);
			if (ret->prev)
				ret->prev->next = ret->next;
			if (ret->next)
				ret->next->prev = ret->prev;
			return head;
		}
		ret = ret->next;
	}
	return head;
}

// Inserts a Node at head of doubly linked list
struct Node *list_insert_head(struct Node *head, int x, int pid, int VPN)
{
	struct Node *newNode = list_new_node(x, pid, VPN);
	if (head == NULL)
	{
		head = newNode;
		return head;
	}
	head->prev = newNode;
	newNode->next = head;
	head = newNode;

	return head;
}

// Inserts a Node at tail of Doubly linked list
struct Node *list_insert_tail(struct Node *head, int x, int pid, int VPN)
{
	struct Node *temp = head;
	struct Node *newNode = list_new_node(x, pid, VPN);
	if (head == NULL)
	{
		head = newNode;
		return head;
	}
	while (temp->next != NULL)
		temp = temp->next; // Go To last Node
	temp->next = newNode;
	newNode->prev = temp;

	return head;
}

// Prints all the elements in linked list in forward traversal order
void list_print(struct Node *head)
{
	struct Node *temp = head;
	printf("Forward [PFN pid VPN] ");
	while (temp != NULL)
	{
		printf("[%d %d %d] ", temp->PFN, temp->pid, temp->VPN);
		temp = temp->next;
	}
	printf("\n");
}

// Prints all elements in linked list in reverse traversal order.
void list_reverse_print(struct Node *head)
{
	struct Node *temp = head;
	if (temp == NULL)
		return; // empty list, exit
	// Going to last Node
	while (temp->next != NULL)
	{
		temp = temp->next;
	}
	// Traversing backward using prev pointer
	printf("Reverse: ");
	while (temp != NULL)
	{
		printf("%d ", temp->PFN);
		temp = temp->prev;
	}
	printf("\n");
}

// creates a new Node
struct Node *create(int PFN, int pid, int VPN)
{
	struct Node *new_Node = (struct Node *)malloc(sizeof(struct Node));

	if (new_Node == NULL)
	{
		printf("\nMemory can't be allocated\n");
		return NULL;
	}

	new_Node->PFN = PFN;
	new_Node->bitRef = 1;
	new_Node->pid = pid;
	new_Node->VPN = VPN;
	new_Node->next = NULL;
	new_Node->prev = NULL;

	return new_Node;
}

// insert a new Node at the beginning of the list
struct Node *insert_begin(struct Node *head, int PFN, int pid, int VPN)
{
	struct Node *new_Node = create(PFN, pid, VPN);

	if (new_Node)
	{
		// if list is empty
		if (head == NULL)
		{
			new_Node->next = new_Node;
			new_Node->prev = new_Node;
			head = new_Node;
			return head;
		}
		head->prev->next = new_Node;
		new_Node->prev = head->prev;
		new_Node->next = head;
		head->prev = new_Node;
		head = new_Node;
	}
	return head;
}

// inserts a new Node at the end
struct Node *insert_end(struct Node *head, int PFN, int pid, int VPN)
{
	struct Node *new_Node = create(PFN, pid, VPN);

	if (new_Node)
	{
		if (head == NULL)
		{
			new_Node->next = new_Node;
			new_Node->prev = new_Node;
			head = new_Node;
			return head;
		}
		head->prev->next = new_Node;
		new_Node->prev = head->prev;
		new_Node->next = head;
		head->prev = new_Node;
	}
	return head;
}

// inserts Node at the given position
struct Node *insert_mid(struct Node *head, int position, int PFN, int pid, int VPN)
{
	// checking if the position is valid or not
	if (position <= 0)
	{
		printf("\nInvalid Position\n");
	}
	else if (head == NULL && position > 1)
	{
		printf("\nInvalid Position\n");
	}
	else if (head != NULL && position > list_size(head))
	{
		printf("\nInvalid Position\n");
	}
	else if (position == 1)
	{
		insert_begin(head, PFN, pid, VPN);
	}
	else
	{
		struct Node *new_Node = create(PFN, pid, VPN);

		if (new_Node != NULL)
		{
			struct Node *temp = head, *prev = NULL;
			int i = 1;

			// traverse the list to the given position
			while (++i <= position)
			{
				prev = temp;
				temp = temp->next;
			}

			// update the prev Node to the new noe
			prev->next = new_Node;

			// update the new Node to the temp (position Node)
			new_Node->next = temp;
		}
	}
	return head;
}

struct Node *delete_begin(struct Node *head)
{
	if (head == NULL)
	{
		printf("\nList is Empty\n");
		return NULL;
	}
	else if (head->next == head)
	{
		free(head);
		head = NULL;
		return head;
	}

	struct Node *temp = head;
	head->prev->next = head->next;
	head->next->prev = head->prev;
	head = head->next;

	free(temp);
	temp = NULL;
	return head;
}

// deletes the Node from the end of the list
struct Node *delete_end(struct Node *head)
{
	if (head == NULL)
	{
		printf("\nList is Empty\n");
		return NULL;
	}
	else if (head->next == head)
	{
		free(head);
		head = NULL;
		return head;
	}

	struct Node *last_Node = head->prev;

	last_Node->prev->next = head;
	head->prev = last_Node->prev;

	free(last_Node);
	last_Node = NULL;
	return head;
}

// deletes the Node from the given position
struct Node *delete_mid(struct Node *head, int position)
{
	if (position <= 0)
	{
		printf("\n Invalid Position \n");
	}
	else if (position > list_size(head))
	{
		printf("\n Invalid position \n");
	}
	else if (position == 1)
	{
		delete_begin(head);
	}
	else if (position == list_size(head))
	{
		delete_end(head);
	}
	else
	{
		struct Node *temp = head;
		struct Node *prev = NULL;
		int i = 1;

		while (i < position)
		{
			prev = temp;
			temp = temp->next;
			i += 1;
		}
		prev->next = temp->next;
		temp->next->prev = prev;
		free(temp);
		temp = NULL;
	}
	return head;
}

// search the Node with the given key item
struct Node *search(struct Node *head, int PFN, int pid, int VPN)
{
	if (head == NULL)
	{
		return NULL;
	}

	struct Node *temp = head;
	do
	{
		if ((temp->PFN == PFN) && (temp->pid == pid) && (temp->VPN == VPN))
		{
			temp->bitRef = 1;
			head = temp;
			return head;
		}
		temp = temp->next;
	} while (temp != head);

	return NULL;
}

// updates the PFN of the given Node position
struct Node *update(struct Node *head, int position, int new_value)
{
	if (head == NULL)
	{
		return NULL;
	}
	else if (position <= 0 || position > list_size(head))
	{
		return head;
	}

	struct Node *temp = head;
	int i = 0;

	while (++i < position)
	{
		temp = temp->next;
	}
	temp->PFN = new_value;
	temp->bitRef = 0;
	return head;
}

// sorts the linked list PFN using insertion sort
struct Node *sort(struct Node *head)
{
	if (head == NULL)
	{
		printf("\nList  is Empty\n");
		return NULL;
	}
	struct Node *temp1 = head;
	struct Node *temp2 = head;
	int key = 0, value;

	do
	{
		temp2 = temp1->next;

		while (temp2 != head)
		{
			if (temp1->PFN > temp2->PFN)
			{
				value = temp1->PFN;
				temp1->PFN = temp2->PFN;
				temp2->PFN = value;
			}
			temp2 = temp2->next;
		}
		temp1 = temp1->next;
	} while (temp1->next != head);
	return head;
}

// calculate the size of the list
int list_size(struct Node *head)
{
	if (head == NULL)
	{
		return 0;
	}

	struct Node *temp = head;
	int count = 0;

	do
	{
		count += 1;
		temp = temp->next;
	} while (temp != head);

	return count;
}

int get_data()
{
	int PFN;
	printf("\n\nEnter PFN: ");
	scanf("%d", &PFN);

	return PFN;
}

int get_position()
{
	int position;
	printf("\n\nEnter Position: ");
	scanf("%d", &position);

	return position;
}

// display the list
void display(struct Node *head)
{
	if (head == NULL)
	{
		return;
	}

	struct Node *temp = head;
	do
	{
		printf("[%d, %d, %d, %d] ", temp->PFN, temp->pid, temp->VPN, temp->bitRef);
		temp = temp->next;
		if (temp == head)
			break;
	} while (temp != head);
	printf("\n");
}

// display the list from end to start
void display_reverse(struct Node *head)
{
	struct Node *temp = head;
	if (temp->next == head)
	{
		printf("%d ", temp->PFN);
		return;
	}

	display_reverse(temp->next);
	printf("%d ", temp->PFN);
}
