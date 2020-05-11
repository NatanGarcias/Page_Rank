#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

// Trabalho desenvolvido por Natan Garcias e Lucas Reis
// O algoritmo aborda o tema de alocação de memória
// Desenvolvido em Setembro de 2018

#define D 0.850000 // Constante d da fórmula do pageRank

typedef struct{  // Struct onde serão armazenados os detalhes sobre o nó do grafo
	int num; // Identificação do nó
	int *ligaMim; // Quais nós se conectam ao 'num'
	int NumLigaMim; // Numero de nós que se conectam ao nó 'num'
	int NumLiga; // A quantos outros nós 'num' se liga
	long double RankAtual; // Numero atual do PageRank
	long double RankPassado; // Numero passado do pageRank
}PAGINA;

//-----------------------------------Função que calcula quando E(k) < £ / Caso da parada-----------------------------------//

long double Erro(PAGINA A[], int n){
	int i; //Variável Auxiliar
	long double total=0.0; // Variável Auxiliar

	for(i=0;i<n;i++)
		total += powl(A[i].RankAtual - A[i].RankPassado,2.0);

	return sqrtl(total); // Retorna a diferença entre os pageRanks atuais e os passados
}

//---------------------------------------Cálculo do pageRank---------------------------------------//

void PageRank(PAGINA A[], int n){
	int i,j,k=0; // Variáveis Auxiliares
	long double total = 0.0; // Variável Auxiliar

	for(i=0;i<n;i++){
		if(A[i].NumLiga > 0){ // Quando |Out(i)| != 0
			total = 0.0;
			for(j=0;j<A[i].NumLigaMim;j++)
					total += A[A[i].ligaMim[j]].RankPassado/A[A[i].ligaMim[j]].NumLiga;
			total *= D;
			total+= (1.0-D)/n;
			A[i].RankAtual = total;
		}
		else // Se o nó não se contecta a ninguém // Quando |Out(i)| = 0
		{
			total = 0.0;
			for(j=0;j<A[i].NumLigaMim;j++)
				total += A[A[i].ligaMim[j]].RankPassado/A[A[i].ligaMim[j]].NumLiga;
			total*=D;
			total+= (1-D)/n + D*A[i].RankPassado;
			A[i].RankAtual = total;
		}
	}
}

//---------------------------------------Funçao de Ordenaçao---------------------------------------//

int cpmfunc (const void * a, const void * b){
	PAGINA *PAGINA_A = (PAGINA *)a;  //Variável Auxiliar 
	PAGINA *PAGINA_B = (PAGINA *)b;  //Variável Auxiliar 
	
	if((PAGINA_A->RankAtual - PAGINA_B->RankAtual)== 0.0) //Quando o pageRank das Paginas é igual 
	{
		if(PAGINA_A->num > PAGINA_B->num)
			return 1;
		else
			return -1;
	}
	else
	{
		if(PAGINA_B->RankAtual > PAGINA_A->RankAtual)
			return 1;
		else
			return -1;
	}
}

//---------------------------------------Passa pageRank Atual para o Passado---------------------------------------//

void PassarPR (PAGINA A[],int n){
	int i; //Variável Auxiliar
	for(i=0;i<n;i++)
		A[i].RankPassado = A[i].RankAtual; // Passando pageRank Atual para o Passado de cada nó
}

int main(){

	char cTemp; // Variaveis usadas na leitura dos componentes textuais
	int nTemp;	// Variável usada para leitura dos inteiros
	int nTemp2; // Auxiliar
	int n,i,j,k;  // Auxiliares
	bool fim;   // Auxiliar
	fim = false; 

	scanf("%d", &n);

	PAGINA *redeA = (PAGINA*) malloc(sizeof(PAGINA) * n);

	//---------------------------------------Inicializacao das Structs---------------------------------------//
	for(i=0;i<n;i++)
	{
		redeA[i].num = i;
		redeA[i].ligaMim=(int*) malloc(1*sizeof(int));
		redeA[i].NumLigaMim = 0;
		redeA[i].NumLiga = 0;
		redeA[i].RankAtual= 1.0/n;
		redeA[i].RankPassado= 0;
	}
	
	//---------------------------------------Leitura---------------------------------------//

	while(scanf("%d", &nTemp)== 1){ // Leitura do índice da pagina
		scanf("%c", &cTemp); //Leitura de : ou ,
		j = 1;  
		nTemp2 = nTemp; // Recebe o indice da pagina 

		while(true){
			scanf("%d%c", &nTemp, &cTemp); // Leitura do número e a pontuação que o segue
			
			if(redeA[nTemp].NumLigaMim == 0){ 
				redeA[nTemp].ligaMim[redeA[nTemp].NumLigaMim] = nTemp2; // Preenche o 1º espaço inicializado sem necessidade de realocar
				redeA[nTemp].NumLigaMim++; // Incrementa a Variável Auxiliar
			}

			else{
				redeA[nTemp].NumLigaMim++; // Incrementa a Variável Auxiliar
				redeA[nTemp].ligaMim = realloc(redeA[nTemp].ligaMim,redeA[nTemp].NumLigaMim*sizeof(int));//realloc +1 posição no vetor
				redeA[nTemp].ligaMim[redeA[nTemp].NumLigaMim-1] = nTemp2; // adicionando no vetor que a pagina i liga a esta pagina
			}
			if(cTemp != ','){
				redeA[nTemp2].NumLiga = j; //Colocando a quantidade de paginas que ela se liga
				break;
			}
			j++; //Incrementando j que é a quantidade de paginas que esta pagina se liga
		}
	}
	//---------------------------------------PageRank---------------------------------------//
	
	while(true)
	{
		PassarPR(redeA,n); // Passa PAGE RANK atual para o passado
		PageRank(redeA,n); // Calcula o pageRank atual 
		
		//---------------------------------------Condicao de parada---------------------------------------//
			
		if(Erro(redeA,n) < 0.0000000000000001)
		{
			qsort(redeA,n,sizeof(PAGINA),cpmfunc); //Funcao de organizar
			for(i=0;i<n;i++){
				printf("%d %Lg\n",redeA[i].num,redeA[i].RankAtual); // Imprimindo resultado
				fim = true; 
			}
		}
		if(fim)
			break;
	}

	//---------------------------------------Desalocaçao---------------------------------------//
	
	for(i=0;i<n;i++)  
		free(redeA[i].ligaMim);
	free(redeA);

	return 0;
}