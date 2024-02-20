#include "hmalloc.h"
#include <stdio.h>
#include <unistd.h>

/*You may include any other relevant headers here.*/


/*	main()
 *	Use this function to develop tests for hmalloc. You should not 
 *	implement any of the hmalloc functionality here. That should be
 *	done in hmalloc.c
 *	This file will not be graded. When grading I will replace main 
 *	with my own implementation for testing.*/
int main(int argc, char *argv[]){

	
	char *ptr = hmalloc(100);
	char *ptr2 = hmalloc(200);
	char *ptr3 = hmalloc(300);

	hfree(ptr);
	hfree(ptr2);
	hfree(ptr3);

	traverse();




/*	
	char* bytes = hmalloc(20);
	bytes[0] = 'a';
	bytes[1] = 'b';
	bytes[2] = 'c';
	bytes[3] = '\n';
	write(1, bytes, 4);
	hfree(bytes);
*/

	return 1;
}
