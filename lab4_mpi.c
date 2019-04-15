#include "lab4_mpi.h"

#include <malloc.h>
#include "mpi.h"


// /*
// 	*****************************************************
// 		TODO -- You must implement this function
// 	*****************************************************
// */
void periodic_pattern_matching (
		int n, 
		char *text, 
		int num_patterns, 
		int *m_set, 
		int *p_set, 
		char **pattern_set, 
		int **match_counts, 
		int **matches)
{
	printf("n: %d, #patterns: %d\n", n, num_patterns);
	printf("text: %s\n", text);

	printf("m_set: ");
	for (int i=0; i<num_patterns; i++)
		printf("%d ", m_set[i]);
	printf("\n");

	printf("p_set: ");
	for (int i=0; i<num_patterns; i++)
		printf("%d ", p_set[i]);
	printf("\n");

	printf("patterns:\n");
	for (int i=0; i<num_patterns; i++)
		printf("%s\n", pattern_set[i]);

}
