#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "vm.h"
#include "API.h"
#include "list.h"

struct Node *head;

/*
 * FIFO is a simple queue, where the victim page is selected from the
 * head and the requested page is inserted at the tail.
 */
int fifo()
{
	int PFN = head->PFN;
	head = list_remove_head(head);
	return PFN;
} // fifo

/*
 * LRU is a priority queue, where the victim page is selected from the tail and
 * the requested page is inserted at the head. If the page is already in the queue
 * then the page is shifted to the head.
 */
int lru()
{
	struct Node *temp = head;

	while (temp->next != NULL)
	{
		temp = temp->next;
	} // while
	int PFN = temp->PFN;

	head = list_remove_tail(head);
	return PFN;
} // lru

/*
 * Clock is a second chance algorithm where each active page is added to a
 * circular linked list and a reference bit of 1. When a victim is needed to
 * be selected the head pointer selects a victim with a reference bit of 0.
 * If the head pointer is currently on a page with a reference bit of 1 the reference
 * bit is changed to 0 and is shifted to the next node.
 */
int clock()
{
	struct Node *temp = head->next;
	int victimFound = 0;
	int PFN = 0;
	while (victimFound == 0)
	{
		if (temp->bitRef == 0)
		{
			// Victim page is found
			victimFound = 1;
			PFN = temp->PFN;

			// Delete page from circular linked list
			head = delete_begin(temp);
			return PFN;
		} // if
		else
		{
			// Reset reference bit
			temp->bitRef = 0;
		} // else

		// Select the next node
		temp = temp->next;
	} // while

} // clock

int find_replacement()
{
	int PFN;
	if (replacementPolicy == ZERO)
	{
		PFN = 0;
	} // if
	else if (replacementPolicy == FIFO)
	{
		PFN = fifo();
	} // else if
	else if (replacementPolicy == LRU)
	{
		PFN = lru();
	} // else if
	else if (replacementPolicy == CLOCK)
	{
		PFN = clock();
	} // else if
	return PFN;
} // find_replacement

int pagefault_handler(int pid, int VPN, char reqType)
{
	int PFN;

	// find a free PFN.
	PFN = get_freeframe();

	// no free frame available. find a victim using a page replacement algorithm.
	if (PFN < 0)
	{
		PFN = find_replacement();

		// Read the current page at the request page frame
		IPTE ipte = read_IPTE(PFN);
		PTE swapOutPTE = read_PTE(ipte.pid, ipte.VPN);

		// Set the swapped out page to be invalid
		swapOutPTE.valid = false;
		write_PTE(ipte.pid, ipte.VPN, swapOutPTE);

		// If the swapped out page is dirty write the page to memory
		if (swapOutPTE.dirty == true)
		{
			swapOutPTE.dirty = false;
			write_PTE(ipte.pid, ipte.VPN, swapOutPTE);
			swap_out(ipte.pid, ipte.VPN, PFN);
		} // if

		// Read the requested page
		PTE pte = read_PTE(pid, VPN);

		// Set the requested page to valid and not dirty
		pte.PFN = PFN;
		pte.valid = true;

		if (reqType == 'R')
		{
			pte.dirty = false;
		} // if
		else if (reqType == 'W')
		{
			pte.dirty = true;
		} // else if

		// Set the requested page's pid and VPN to the inverted page table entry
		ipte.pid = pid;
		ipte.VPN = VPN;

		// Write to the page table and inverted page table entries
		write_PTE(pid, VPN, pte);
		write_IPTE(PFN, ipte);

		// Swap the requested page into memory
		swap_in(pid, VPN, PFN);
	} // if
	else
	{
		// Free Frame exists

		// Read and Write to the inverted page table entry
		IPTE ipte = read_IPTE(PFN);
		PTE pte = read_PTE(pid, VPN);

		ipte.pid = pid;
		ipte.VPN = VPN;

		// Read and Write to the page table entry
		pte.PFN = PFN;
		pte.valid = true;

		if (reqType == 'R')
		{
			pte.dirty = false;
		} // if
		else if (reqType == 'W')
		{
			pte.dirty = true;
		} // else if

		write_PTE(pid, VPN, pte);
		write_IPTE(PFN, ipte);

		// Swap the requested page into memory
		swap_in(pid, VPN, PFN);

	} // else

	// Return the page frame of the requested page
	return PFN;

} // pagefault_handler

int get_PFN(int pid, int VPN, char reqType)
{
	/* Read page table entry for (pid, VPN) */
	PTE pte = read_PTE(pid, VPN);

	/* if PTE is valid, it is a page hit. Return physical frame number (PFN) */
	if (pte.valid)
	{
		/* Mark the page dirty, if it is a write request */
		if (reqType == 'W')
		{
			pte.dirty = true;
			write_PTE(pid, VPN, pte);
		} // if

		return pte.PFN;
	} // if

	/* PageFault, if the PTE is invalid. Return -1 */
	return -1;
} // get_PFN

int MMU(int pid, int virtAddr, char reqType, bool *hit)
{
	int PFN, physicalAddr;
	int VPN = 0, offset = 0;

	/* calculate VPN and offset */
	int16_t m = virtAddr;
	VPN = ((virtAddr >> 8));
	offset = ((virtAddr & 0xFF));

	// read page table to get Physical Frame Number (PFN)
	PFN = get_PFN(pid, VPN, reqType);

	if (PFN >= 0)
	{ // page hit
		stats.hitCount++;
		*hit = true;
	} // if
	else
	{ // page miss
		stats.missCount++;
		*hit = false;
		PFN = pagefault_handler(pid, VPN, reqType);
	} // else

	/*
	 * FIFO is a simple queue, where the victim page is selected from the
	 * head and the requested page is inserted at the tail.
	 */
	if (replacementPolicy == FIFO)
	{
		if (list_return(head, PFN, pid, VPN) == NULL || head == NULL)
		{
			head = list_insert_tail(head, PFN, pid, VPN);
		} // if

	} // FIFO

	/*
	 * LRU is a priority queue, where the victim page is selected from the tail and
	 * the requested page is inserted at the head. If the page is already in the queue
	 * then the page is shifted to the head.
	 */
	else if (replacementPolicy == LRU)
	{

		// Remove the current page if in memory
		if (list_return(head, PFN, pid, VPN) != NULL)
		{
			head = list_remove(head, PFN, pid, VPN);
		}
		if (head != NULL)
		{
			// The requested page matches head
			if ((PFN == head->PFN) && ((pid == head->pid) && (VPN == head->VPN)))
			{
				head = head->next;
				head->prev = NULL;
			} // if
			else
			{
				struct Node *ret = head;
				int isFound = 0;
				while (ret->next != NULL)
				{
					if ((PFN == ret->PFN))
					{
						// If the page frame matches the requested page frame
						if (ret->next == NULL)
						{
							ret->prev->next = NULL;
						} // if
						if (ret->prev)
						{
							ret->prev->next = ret->next;
						} // if
						if (ret->next)
						{
							ret->next->prev = ret->prev;
						} // if
						head = ret;
						isFound = 1;
					} // if
					ret = ret->next;
				} // while

				if ((PFN == ret->PFN) && (pid == ret->pid) && (VPN == ret->VPN))
				{
					if (ret->next == NULL)
					{
						ret->prev->next = NULL;
					} // if
					if (ret->prev)
					{
						ret->prev->next = ret->next;
					} // if
					if (ret->next)
					{
						ret->next->prev = ret->prev;
					} // if
					while (ret->prev != NULL)
					{
						ret = ret->prev;
					} // while
					head = ret;
				} // if
				if (isFound == 0)
				{
					head = list_insert_head(head, PFN, pid, VPN);
				} // if

			} // else

		} // if
		else
		{
			// Head is NULL
			head = list_insert_head(head, PFN, pid, VPN);
		} // else

	} // LRU

	/*
	 * Clock is a second chance algorithm where each active page is added to a
	 * circular linked list and a reference bit of 1. When a victim is needed to
	 * be selected the head pointer selects a victim with a reference bit of 0.
	 * If the head pointer is currently on a page with a reference bit of 1 the reference
	 * bit is changed to 0 and is shifted to the next node.
	 */
	else if (replacementPolicy == CLOCK)
	{
		// Check if the requested page is already in memory
		if (search(head, PFN, pid, VPN) != NULL)
		{
			// Set the reference bit of the requested page to 1
			head = search(head, PFN, pid, VPN);
		} // if
		else
		{
			// Insert the requested page at the current position of head
			head = insert_begin(head, PFN, pid, VPN);
		} // else

	} // CLOCK

	physicalAddr = (PFN << 8) + offset;
	return physicalAddr;

} // MMU
