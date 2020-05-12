/* PageRank header file */
#ifndef CORE_H
#define CORE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>


#define D 0.850000              // Damping factor
#define EPSILON 1E-12           // Prevents comparison errors due to precison 

typedef struct{  
	int id;                    	// Node Id
	int *inputList;             // Every edge pointing to @id
	int n_inputs;             	// Input degree
	int n_outputs;              // Output degree
	long double currentRank;    // Current rank
	long double previousRank;   // Previous rank
}PAGE;


void initialize(PAGE network[], int n) {

	for(int i=0;i<n;i++) {

		network[i].id = i;
		network[i].inputList = (int*) malloc(1*sizeof(int));
		network[i].n_inputs = 0;
		network[i].n_outputs = 0;
		network[i].currentRank= 1.0/n;
		network[i].previousRank= 0;

	}
	
}

void readInput(PAGE network[]){

    int nTemp;          // Page index
    char cTemp;         // Handling trash

    // Reading page index
	while(scanf("%d", &nTemp) == 1){ 
		
        // Reading : or ,
        scanf("%c", &cTemp);

		int j = 1;  
        int nTemp2 = nTemp;

		while(true){

            // Reading 0: or 1,
			scanf("%d%c", &nTemp, &cTemp); 
			
			if(network[nTemp].n_inputs == 0) {

                // Fills in the first space without needing to realocate
				network[nTemp].inputList[network[nTemp].n_inputs] = nTemp2; 
				network[nTemp].n_inputs++;

			}
			else {
    
				network[nTemp].n_inputs++; 
				network[nTemp].inputList = realloc(network[nTemp].inputList,network[nTemp].n_inputs*sizeof(int));
                network[nTemp].inputList[network[nTemp].n_inputs-1] = nTemp2;   // Page i links to current page			
            }

			if(cTemp != ',') { 
                // Amount of pages @j that node @nTemp2 connects to
				network[nTemp2].n_outputs = j; 
				break;
			}

			j++; 
		}
	}
}

// Sorting function - descending
int cpmfunc (const void * a, const void * b){

	PAGE *PAGE_A = (PAGE *)a;  
	PAGE *PAGE_B = (PAGE *)b;  
	
	if(fabs(PAGE_A->currentRank - PAGE_B->currentRank) < EPSILON) {
		if(PAGE_A->id > PAGE_B->id)
			return 1;
		else
			return -1;
	}
	else {
		if(PAGE_B->currentRank > PAGE_A->currentRank)
			return 1;
		else
			return -1;
	}
}

void printResults(PAGE network[], int n) {

    for(int i=0;i<n;i++)
		printf("%d %Lg\n",network[i].id,network[i].currentRank);

}


void clear(PAGE network[], int n) {

	for(int i=0;i<n;i++)  
		free(network[i].inputList);
	free(network);

} 


#endif
