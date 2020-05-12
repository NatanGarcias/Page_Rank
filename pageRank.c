/* Project developed by Natan Garcias and Lucas Reis */
#include "core.h"

// Responsible for detecting convergence, which is the stopping criteria
long double error(PAGE network[], int n){

	long double total = 0.0;

	for(int i=0;i<n;i++)
		total += powl(network[i].currentRank - network[i].previousRank,2.0);

    // Difference between current page rank and previous one
	return sqrtl(total); 
}

void calculateRank(PAGE network[], int n){
	
    long double total = 0.0; 

	for(int i=0;i<n;i++){

        total = 0.0;
        // |Out(i)| != 0
		if(network[i].n_outputs > 0){ 

			for(int j=0;j<network[i].n_inputs;j++)
				total += network[network[i].inputList[j]].previousRank/network[network[i].inputList[j]].n_outputs;
			
            total *= D;
			total += (1.0-D)/n;
			
            network[i].currentRank = total;
		}
        // |Out(i)| = 0
		else {
			
            for(int j=0;j<network[i].n_inputs;j++)
				total += network[network[i].inputList[j]].previousRank/network[network[i].inputList[j]].n_outputs;
			
            total*=D;
			total+= (1-D)/n + D*network[i].previousRank;
			
            network[i].currentRank = total;
		}
	}
}


void updateRank (PAGE network[], int n){

	for(int i=0;i<n;i++)
		network[i].previousRank = network[i].currentRank;

}

bool getRanks(PAGE network[], int n) {

	updateRank(network,n); 
	calculateRank(network,n); 
	
	// Stopping criteria		
	if(error(network,n) < EPSILON) {

		qsort(network,n,sizeof(PAGE), cpmfunc);
		printResults(network, n);
		return false; 

	}

	return true;

}

void pageRank(PAGE network[], int n) { 
	
	while(getRanks(network,n));
	
}


int main(){

	int n;

	scanf("%d", &n);
	PAGE *network = (PAGE*) malloc(sizeof(PAGE) * n);

	initialize(network, n);	
    readInput(network);
	
	pageRank(network, n);


	clear(network, n);

	return 0;
}
