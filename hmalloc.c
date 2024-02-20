#include "hmalloc.h"

/*You may include any other relevant headers here.*/
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>






/*Add additional data structures and globals here as needed.*/
void *free_list = NULL;

/* traverse
 * Start at the free list head, visit and print the length of each
 * area in the free pool. Each entry should be printed on a new line.
 */
void traverse()
{
   /* Printing format:
	 * "Index: %d, Address: %08x, Length: %d\n"
	 *    -Index is the position in the free list for the current entry. 
	 *     0 for the head and so on
	 *    -Address is the pointer to the beginning of the area.
	 *    -Length is the length in bytes of the free area.
	 */

	//Pointer to node being computed
	int* current = (int*)free_list;

	//Counter for index in list
	int position = 0;

	//If there are no entries in the free list, return
	if(!current) return;

	//While there are entries remaining
	do
	{
		//Print entry metadata
		printf("Index: %d, Address: %08x, Length: %d\n", position, current, *current);

		position++;
		current = (int)current + (int)*(current + 1);
	}
	while(current && *(current + 1)); //Continue until we reach an entry that doesn't point to another

	return;


}

/* hmalloc
 * Allocation implementation.
 *    -will not allocate an initial pool from the system and will not 
 *     maintain a bin structure.
 *    -permitted to extend the program break by as many as user 
 *     requested bytes (plus length information).
 *    -keeps a single free list which will be a linked list of 
 *     previously allocated and freed memory areas.
 *    -traverses the free list to see if there is a previously freed
 *     memory area that is at least as big as bytes_to_allocate. If
 *     there is one, it is removed from the free list and returned 
 *     to the user.
 */
void *hmalloc(int bytes_to_alloc)
{

	//Check to see valid mem size
	if(bytes_to_alloc <= 0) return 0;

	//Get a pointer to the first element in the free list
	int *current = free_list;

	//Pointer to previous node for
	//removing node from linked list
	int *previous = NULL;

	//While we haven't reached the end of the free list
	if(current) //Used to check that there are actually elements in free list
	do
	{

		//If we find space that is suitable, use it
		if (*current >= bytes_to_alloc)
		{
			//Point the previous node in the free-list
			//to the next node in the free-list
			//This effectively removes 
			//the current node from the list
			if(previous)
			{	

				//Point the previous link to the next link in the free list
				//Calculate distances for new
				//relative pointer
				int d_previous_current = (int)current - (int)previous;
				int d_current_next = *(current + 1); 
				*(previous + 1) = (int)d_previous_current + (int)d_current_next;
			}
			else 	//If this was the head pointer
			{
				free_list = (int)current + (int)*(current + 1);
			}

			//Do not set the size field to the actual size
			//Leave it as the size of the total memory segment

			//Set the next-link pointer to null
			*(current + 1) = 0;	
			void *ret_ptr = (current + 2); //Get the pointer to the actual memory segment
			return ret_ptr;	

		}

		//Update the "previous" pointer
		previous = current;

		//Go to the next element in the free list
		current = (int)current + (int)*(current + 1);

	}
	
	//Continue until we reach an element that doesn't point to another
	while(current && *(current + 1)); 

	//If there is not space in the free-list
	
	//Allocate space for memory and metadata
	void *ret_ptr = sbrk(bytes_to_alloc + 8);

	//Set the member pointer to the start of the segment
	int *meta_ptr = ret_ptr;

	//Set the return pointer to be 8 bytes later.
	ret_ptr = ret_ptr + 8;

	//Set the size byte to the size of the malloc
	*meta_ptr = bytes_to_alloc;

	//Set the link pointer to 0, given that it is not in the free list. 
	*(meta_ptr + 1) = 0;

   	return ret_ptr; 
}

/* hcalloc
 * Performs the same function as hmalloc but also clears the allocated 
 * area by setting all bytes to 0.
 */
void *hcalloc(int bytes_to_alloc)
{

	//Check to see valid mem size
	if(bytes_to_alloc <= 0) return 0;

	char *ret_ptr = (char*)hmalloc(bytes_to_alloc);

	//Free all allocated memory
	for(int i = 0; i < bytes_to_alloc; i++)
	{
		ret_ptr[i] = 0;
	}
	
   return ret_ptr; 
}

/* hfree
 * Responsible for returning the area (pointed to by ptr) to the free
 * pool.
 *    -simply appends the returned area to the beginning of the single
 *     free list.
 */
void hfree(void *ptr)
{

	//Metadata pointer, points to beginning of total segment
	int* meta_ptr = ptr - 8;

	//Set the current link to be the relative pointer to the head of the free-list
	*(meta_ptr + 1) = (int)free_list - (int)meta_ptr;

	//Set the free list to point to the metadata of this memory space
	free_list = meta_ptr;

	return;

}

/* For the bonus credit implement hrealloc. You will need to add a prototype
 * to hmalloc.h for your function.*/
void *hrealloc(void *ptr, int bytes_to_alloc)
{

	//Check to see valid mem size
	if(bytes_to_alloc <= 0) return 0;

	//Pointer to be returned
	void *ret_ptr = hmalloc(bytes_to_alloc);

	//Pointer to beginning of metadata
	int *meta_ptr = ret_ptr - 8;

	//For readability
	int size = *meta_ptr;

	//Make sure that we don't copy out of bounds
	if (bytes_to_alloc < size) size = bytes_to_alloc;

	//Copy memory from old allocation to new allocation.
	char *mem1_ptr = ptr;
	char *mem2_ptr = ret_ptr;
	for(int i = 0; i < size; i++)
	{
		mem2_ptr[i] = mem1_ptr[i];
	}
		
	hfree(ptr);
	return ret_ptr;

}


/*You may add additional functions as needed.*/
