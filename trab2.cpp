#include<iostream>
#include<stdlib.h>
#include<fstream>
#include <string.h>
using namespace std;

// Trabalho desenvolvido por Natan Garcias e Lucas Reis
// O algoritmo aborda o tema de ordenação externa
// Desenvolvido entre Setembro e Outubro de 2019

struct Linha{
	char **l;
};

void meItoa (char *buffer,int QualArquivoSou, int numArquivosCriados){ // Criação dos nomes dos arquivos
	sprintf(buffer,"%d.%d",QualArquivoSou, numArquivosCriados);
}

void NomeDosArquivosPrincipais(char*NomeEnt1,char*NomeEnt2,char*NomeSai,char**argv){ // Adicionando .txt nos arquivos principais
	strcpy(NomeEnt1,argv[4]); // Copiando o nome do Primeiro Arquivo para NomeEnt1
	strcpy(NomeEnt2,argv[5]); // Copiando o nome do Segundo Arquivo para NomeEnt2
	strcpy(NomeSai,argv[6]); // Copiando o nome do Terceiro Arquivo para NomeSai
}

void ColunasDeComparacao(char *quebra, int *&VetorColunas,int &NumeroColunas, char **argv, int Pos){ //Guarda as colunas utilizadas para comparação
	
	quebra = strtok(argv[Pos], ","); //Utilizando a funcao strtok para capturar os índices pros vetores
	
	while (quebra != NULL){ //Lendo as colunas do arquivo ...
		VetorColunas = (int*)realloc(VetorColunas,(NumeroColunas+1)*sizeof(int));
		VetorColunas[NumeroColunas] = atoi(quebra);
		NumeroColunas++;
    	quebra = strtok(NULL, ",");
    }
}

void desalocaMatriz(Linha *matriz, int numArquivos, int numColunas){ // Libera a memoria ocupada pela matriz 
	
	for(int v=0;v<numArquivos;v++){ //Deletando as memorias alocadas
		for(int w=0;w<numColunas;w++){
			free(matriz[v].l[w]);
		}
		free(matriz[v].l);
	}
	free(matriz);
}

void mergeDaSeparacao(Linha *matriz,int colunaTenhoQueOlhar,int p, int q, int r, int numeroColunas){ //Intercala
	
	int tam= r-p;
	
	Linha *aux = (Linha *) malloc(tam * sizeof(Linha));
	
	int i = p; //Cursor 1
	int j = q; //Cursor 2
	int k = 0; //Cursor para aux
	
	while(i<q && j<r){
		if(strcmp(matriz[i].l[colunaTenhoQueOlhar],matriz[j].l[colunaTenhoQueOlhar])<=0)
			aux[k++] = matriz[i++];
		else
			aux[k++] = matriz[j++];
	}
	
	while(i < q) //Dois while para preencher o restante
		aux[k++] = matriz[i++];
	
	while(j < r)
		aux[k++] = matriz[j++];
	
	for(k=0;k < tam; k++) //transferindo o ordenado p/ o original
		matriz[p+k] = aux[k];

	free(aux);
}

void mergeSortDaSeparacao(Linha *matriz,int *vetorColunas,int numVetorColunas,int inicio, int fim, int numeroColunas) { //MergeSort para separarmos os arquivos em subarquivos.
	
	int colunaTenhoQueOlhar;
	
	for(int u=numVetorColunas-1;u>=0;u--){
		if(inicio<fim-1){
	
			int meio= (inicio+fim)/2;
	
			mergeSortDaSeparacao(matriz,vetorColunas,numVetorColunas,inicio,meio,numeroColunas);
			mergeSortDaSeparacao(matriz,vetorColunas,numVetorColunas,meio,fim,numeroColunas);
	
			colunaTenhoQueOlhar = vetorColunas[u];
	
			mergeDaSeparacao(matriz,colunaTenhoQueOlhar,inicio,meio,fim,numeroColunas);
		}
	}
}

void mergeDaUniao(int *aberto,Linha *matriz,int colunaTenhoQueOlhar,int p, int q, int r, int numeroColunas){ //Intercala
	int tam= r-p;
	
	Linha *aux = (Linha *) malloc(tam * sizeof(Linha));
	
	int *aux2 = (int*) malloc(tam * sizeof(int));
	
	int i = p; //Cursor 1
	int j = q; //Cursor 2
	int k = 0; //Cursor para aux
	
	while(i<q && j<r){
		if(strcmp(matriz[i].l[colunaTenhoQueOlhar],matriz[j].l[colunaTenhoQueOlhar])<=0){
			aux[k] = matriz[i];
			aux2[k++] = aberto[i++];
		}
		else{
			aux[k] = matriz[j];
			aux2[k++] = aberto[j++];
		}
	}
	
	while(i < q){ //Dois while para preencher o restante
		aux[k] = matriz[i];
		aux2[k++] = aberto[i++];
	}
	
	while(j < r){
		aux[k] = matriz[j];
		aux2[k++] = aberto[j++];
	}
	
	for(k=0;k < tam; k++){ //transferindo o ordenado p/ o original
		matriz[p+k] = aux[k];
		aberto[p+k] = aux2[k];
	}
	
	free(aux);
	free(aux2);
}

void mergeSortDaUniao(int *vetorAuxiliar, Linha *matriz,int *vetorColunas,int numVetorColunas,int inicio, int fim, int numeroColunas){ //MergeSort para unirmos subarquivos em um grande.
	
	int colunaTenhoQueOlhar;
	
	for(int u=numVetorColunas-1;u>=0;u--){
		if(inicio<fim-1){
			
			int meio= (inicio+fim)/2;
			
			mergeSortDaUniao(vetorAuxiliar,matriz,vetorColunas,numVetorColunas,inicio,meio,numeroColunas);
			mergeSortDaUniao(vetorAuxiliar,matriz,vetorColunas,numVetorColunas,meio,fim,numeroColunas);
			
			colunaTenhoQueOlhar = vetorColunas[u];
			
			mergeDaUniao(vetorAuxiliar,matriz,colunaTenhoQueOlhar,inicio,meio,fim,numeroColunas);
		}
	}
}

void JogaNoArquivo(ofstream &arquivo,Linha *matriz,int inicio, int fim,int numeroColunas){ // Joga as strings guardadas na matriz no arquivo
	
	for(int ii=inicio;ii<fim;ii++){
		for(int jj=0;jj<numeroColunas;jj++){
			if(jj!=numeroColunas-1)
				arquivo << matriz[ii].l[jj] << "\t"; //Se não for a ultima coluna, insiro um tab \t
			else
				arquivo << matriz[ii].l[jj] << "\n"; // Ultima coluna do arquivo, dou \n e não dou tab
		}
	}
}

void contaColunasDoArquivo(char *buffer, int &numeroColunas){//Contando numero de colunas do arquivo
	
	numeroColunas =1;
	
	for(int i=0;buffer[i]!='\0';i++){
		if(buffer[i]=='\t')
			numeroColunas++;
	}
	return;
}

void StrTokQueLeEAlocaNaMemoria(char *buffer, Linha *matriz, int QualLinhaEstou){ //Pega os dados do arquivos e armazena-os na memoria
	
	int QualColunaEstou=0;  //Contadores para nos situarmos na leitura
	int contTamanho=0;
	
	char *quebra;
	
	quebra = strtok(buffer,"\t\r\0\n"); //Caracteres para quebrarmos o ponteiro de Strtok: TAB e FIM DE LINHA  (funciona no Windows com \r\n e no Linux com \n )
	
	while(quebra != NULL){
		
		contTamanho = strlen(quebra); //Alocaremos na memoria o tamanho de cada elemento da matriz do arquivo.
		matriz[QualLinhaEstou].l[QualColunaEstou] =(char *) malloc((contTamanho+1) * sizeof(char));
		
		strcpy(matriz[QualLinhaEstou].l[QualColunaEstou],quebra); //copiar do ponteiro do StrTok para nossa matriz com as informacoes.
		
		QualColunaEstou++;
		quebra = strtok(NULL,"\t\n\r\0"); //Caracteres para quebrarmos o ponteiro de StrTok: TAB e FIM DE LINHA  (funciona no Windows com \r\n e no Linux com \n )
	}
	return;
}

void JuntaArq(ofstream &arquivo, int numArquivos, int venhoDeQual, int numeroColunas, int *vetorComparar, int numColunasComparar){ //Utilizadas para unir os arquivos menores
	
	char *buffer = new char [1000000];
	
	Linha *linha;
	
	linha = (Linha*) malloc(numArquivos * sizeof(Linha));
	
	ifstream *meusArquivos = new ifstream [numArquivos]; //cria um vetor de ifstream
	
	for(int j=0;j<numArquivos;j++){
	
		char nomeDoTxt[12]; // nomeando arquivo
	
		sprintf(nomeDoTxt,"%d.%d",venhoDeQual,j); //numera arquivo
	
		meusArquivos[j].open(nomeDoTxt); //abre arquivo
	
		if(meusArquivos[j].is_open()){
			meusArquivos[j].getline(buffer,1000000);
			linha[j].l =(char **) malloc(numeroColunas * sizeof(char*));
			StrTokQueLeEAlocaNaMemoria(buffer,linha, j); //le a linha atual
		}
	}
	
	int inicioMerge=0;
	int *vetorAuxiliar = new int [numArquivos];
	
	for(int e=0;e<numArquivos;e++)
		vetorAuxiliar[e]=e;
	
	while(inicioMerge!=numArquivos){ // Pega partes ordenadas e ordena em apenas um arquivo grande, formando o Junto1/Junto2
		
		mergeSortDaUniao(vetorAuxiliar,linha,vetorComparar,numColunasComparar,inicioMerge,numArquivos,numeroColunas);
		JogaNoArquivo(arquivo,linha,inicioMerge,inicioMerge+1,numeroColunas);
		
		if(meusArquivos[vetorAuxiliar[inicioMerge]].peek()==-1){
			meusArquivos[vetorAuxiliar[inicioMerge]].close();
			inicioMerge++;
		}else{
			
			meusArquivos[vetorAuxiliar[inicioMerge]].getline(buffer,1000000);
			
			int QualColunaEstou=0;
			int contTamanho=0;
			
			char *quebra;
			
			quebra = strtok(buffer,"\t\r\0\n");
			
			for(int w=0;w<numeroColunas;w++){
				free(linha[inicioMerge].l[w]);
			}
			
			while(quebra != NULL){
				contTamanho = strlen(quebra);
				linha[inicioMerge].l[QualColunaEstou] =(char *) malloc((contTamanho+1) * sizeof(char));
				strcpy(linha[inicioMerge].l[QualColunaEstou],quebra);
				QualColunaEstou++;
				quebra = strtok(NULL,"\t\n\r\0");
			}
		}
	}
	desalocaMatriz(linha,numArquivos,numeroColunas);
	
	delete[] vetorAuxiliar;
	delete[] buffer;
	delete[] meusArquivos;
}

void criacaoDeArquivo(Linha *matriz,int QualArquivoSou, int &numArquivosCriados, int numeroColunas, int &QualLinhaEstou){ //Cria um arquivo .txt
	
	char *bufferizado = new char[12];
	
	meItoa(bufferizado,QualArquivoSou,numArquivosCriados); //Funcao para escrevermos o nome do arquivo que sera gerado, ex: "1.0.txt"
	
	ofstream f;
	
	f.open(bufferizado); //Criamos esse arquivo gerado na pasta do Trabalho
	
	numArquivosCriados++;
	
	JogaNoArquivo(f,matriz,0,QualLinhaEstou,numeroColunas); //Jogamos a informacao da memoria para o arquivo gerado.
	
	delete []bufferizado;
	
	desalocaMatriz(matriz,QualLinhaEstou,numeroColunas); //Desalocamos a matriz que tinhamos as informacoes na memoria.
	
	QualLinhaEstou=0; // "Voltaremos a linha zero" para criar um novo arquivo com as informacoes ate esgotar a memoria novamente.
}

void compara(int &menor,int quantosComparar, Linha *grandoes, int *vetorComparar1, int *vetorComparar2){
	for(int cont=0;cont<quantosComparar;cont++){
		if(strcmp(grandoes[0].l[vetorComparar1[cont]], grandoes[1].l[vetorComparar2[cont]])!=0){ //Colunas diferentes!
			if(strcmp(grandoes[0].l[vetorComparar1[cont]], grandoes[1].l[vetorComparar2[cont]])>0){
				menor=1;// segundo eh o menor, iremos descer a linha dele!
				break;
			}
			else{
				menor=0; //o primeiro eh o menor, desceremos a linha dele
				break;
			}
		}
	}
}

void JogaNoArquivoFinal(bool *jaImprimiDoUm, bool *jaImprimiDoDois, int quantosComparar, int *vetorComparar1, int *vetorComparar2, ofstream &ArqSaida, int numColunas1, int numColunas2, Linha *grandoes){
	for(int i=0;i<quantosComparar;i++){
		jaImprimiDoUm[vetorComparar1[i]]= true; //marcando que ja imprimimos essa coluna do ArqUm
		jaImprimiDoDois[vetorComparar2[i]]= true; //marcando que ja imprimimos essa coluna do ArqDois
		ArqSaida<<grandoes[0].l[vetorComparar1[i]];
		if(i!=(quantosComparar-1)){
			ArqSaida<<"\t"; //Adiciona tab no arquivo.
		}
	}
	//Imprimindo o restante das colunas do ArquivoUm para fazer o join (colunas que nao sao da comparacao)
	for(int c=0;c<numColunas1;c++){
		if(jaImprimiDoUm[c]==false){
			ArqSaida<<"\t"<<grandoes[0].l[c];
		}
	}
	//Imprimindo o restante das colunas do ArquivoDois para fazer o join (colunas que nao sao da comparacao)
	for(int c=0;c<numColunas2;c++){
		if(jaImprimiDoDois[c]==false){
			ArqSaida <<"\t"<<grandoes[1].l[c];
		}
	}
	ArqSaida <<"\n"; //Adiciona \n na linha recorrente do arquivo de saida.
} 

void unindoGigantes(ifstream &um, ifstream &dois,ofstream &ArqSaida, int quantosComparar, int *vetorComparar1, int *vetorComparar2, int numColunas1, int numColunas2){ // Intercala os 2 arquivos já ordenados
	
	Linha *grandoes = (Linha*) malloc(2 *sizeof(Linha)); //Vetor de tamanho 2 para as linha recorrentes
	
	bool acabouOJoin=false;
	char *buffer = new char[1000000]; //Vetor para capturar as linhas do arquivo

	//Lendo a primeira linha do primeiro arquivo e alocando.
	um.getline(buffer,1000000);
	grandoes[0].l =(char **) malloc(numColunas1 * sizeof(char*)); //Espaco que guardara a linha recorrente do primeiro arquivo.
	StrTokQueLeEAlocaNaMemoria(buffer, grandoes, 0);

	//Lendo a primeira linha do segundo arquivo e alocando.
	dois.getline(buffer,1000000);
	grandoes[1].l  =(char **) malloc(numColunas2 * sizeof(char*));//Espaco que guardara a linha recorrente do segundo arquivo.
	StrTokQueLeEAlocaNaMemoria(buffer, grandoes, 1);

	while(!acabouOJoin){
		int menor=-1;
		/*Variavel int menor:
		Menor==    [-1]: Colunas iguais, ninguem eh o menor. Imprime as duas linhas! Em seguida buscamos as proximas duas linhas dos arquivos.
				   [ 1]: O menor eh o primeiro arquivo. Buscamos a proxima linha do primeiro arquivo!
				   [ 2]: O menor eh o segundo arquivo. Buscamos a proxima linha do segundo arquivo!
		*/

		//funcao que compara e retorna o menor como referencia
		compara(menor, quantosComparar, grandoes, vetorComparar1, vetorComparar2); 
		
		if(menor==-1){	//Sao iguais! Dai imprime os dois juntos, unindo!
			bool *jaImprimiDoUm = (bool*)calloc(numColunas1, sizeof(bool));   //Vetores para sabermos quem ja imprimimos do ArqUm
			bool *jaImprimiDoDois = (bool*)calloc(numColunas2, sizeof(bool));	//Vetores para sabermos quem ja imprimimos do ArqDois
			
			//imprime no arquivo de saida
			JogaNoArquivoFinal(jaImprimiDoUm, jaImprimiDoDois, quantosComparar, vetorComparar1, vetorComparar2, ArqSaida, numColunas1, numColunas2, grandoes);
			
			free(jaImprimiDoUm); //liberando nossos vetores de orientacao de quem ja imprimimos.
			free(jaImprimiDoDois);

			for(int w=0;w<numColunas1;w++){ //Liberando as informacoes do arquivoUm armazenadas no vetor
				free(grandoes[0].l[w]);
			}
			for(int w=0;w<numColunas2;w++){ //Liberando as informacoes do arquivoDois armazenadas no vetor
				free(grandoes[1].l[w]);
			}

			if(um.peek()==-1){ //Verificando se o arquivoUm acabou, se sim terminamos.Se nao, lemos a prox linha.
				break;
			}
			else{
				um.getline(buffer,1000000); //Buscando a linha do proximo arquivo
				StrTokQueLeEAlocaNaMemoria(buffer, grandoes, 0);
			}

			if(dois.peek()==-1){ //Verificando se o arquivoDois acabou, se sim terminamos.Se nao, lemos a prox linha.
				break;
			}
			else{
				dois.getline(buffer,1000000);
				StrTokQueLeEAlocaNaMemoria(buffer, grandoes, 1);
			}
		}
		else if(menor==0){ //Busca a proxima linha do arquivoUm, ja que ele e o menor e nao acabou ainda.
			for(int w=0;w<numColunas1;w++){
				free(grandoes[0].l[w]);
			}
			if(um.peek()==-1){ //Se acabou, terminamos, caso contrario leremos a prox linha.
				break;
			}
			else{
				um.getline(buffer,1000000);
				StrTokQueLeEAlocaNaMemoria(buffer, grandoes, 0);
			}
		}
		else if(menor==1){ //Busca a proxima linha do arquivoDois, ja que ele e o menor e nao acabou ainda.
			for(int w=0;w<numColunas2;w++){
				free(grandoes[1].l[w]);
			}
			if(dois.peek()==-1){ //Se acabou, terminamos, caso contrario leremos a prox linha.
				break;
			}
			else{
				dois.getline(buffer,1000000);
				StrTokQueLeEAlocaNaMemoria(buffer, grandoes, 1);
			}
		}
	}
	delete[]buffer;//Deletando o buffer de leitura
	
	free(grandoes[0].l); //Deletando o vetor que guardavamos as linhas.
	free(grandoes[1].l);
	free(grandoes);
}

void leArquivo(ifstream &arquivo, Linha *matriz, char *NomeEnt,int M, int *vetorColunas, int numVetorColunas, int QualArquivoSou, int &numArquivosCriados, int &numeroColunas){ // Lê e quebra o arquivo em subarquivos
	
	int QualLinhaEstou = 0; //Contador para nos situarmos durante a leitura do arquivo
	char *buffer = new char [1000000]; //buffer pra leitura
	
	while(true){ //While que so termina quando lemos o arquivo inteiro!
		if(QualLinhaEstou==0){ //Se estou na linha zero de leitura do arquivo, contarei as colunas do meu arquivo e irei ler a linha um.
			matriz = (Linha*) malloc(1 * sizeof(Linha)); //cria a primeira linha
			arquivo.getline(buffer,1000000);
			contaColunasDoArquivo(buffer,numeroColunas); //Contando num de colunas do arquivo que estou lendo.
			matriz[QualLinhaEstou].l =(char **) malloc(numeroColunas * sizeof(char*)); //Alocando esse numero de colunas.
		}
		if(QualLinhaEstou!=0){ //Se nao estou na primeira linha do meu arquivo, vou ler as linhas normalmente e armazenar
			arquivo.getline(buffer,1000000); //lendo
			matriz =(Linha*) realloc(matriz,(QualLinhaEstou+1) * sizeof(Linha)); //Realocando minha matriz da memoria com as linhas +1
			matriz[QualLinhaEstou].l =(char **) malloc(numeroColunas * sizeof(char*)); //agora alocando o numero de colunas
		}
		
		StrTokQueLeEAlocaNaMemoria(buffer,matriz,QualLinhaEstou); //Essa funcao capta as informacoes do buffer, e as aloca na minha Matriz passada para ela.
		QualLinhaEstou++;

		if(QualLinhaEstou == M){ //Hora de imprimir arquivo! Memoria lotada!
			mergeSortDaSeparacao(matriz,vetorColunas,numVetorColunas,0,QualLinhaEstou,numeroColunas); //Chamamos a funcao de ordenacao!
			criacaoDeArquivo(matriz,QualArquivoSou,numArquivosCriados,numeroColunas,QualLinhaEstou); //Chamamos a funcao para podermos criar o arquivo.
		}
		if(arquivo.peek()==-1){
			if(QualLinhaEstou!=0){ //Hora de imprimir arquivo! Acabou o arquivo principal!
				mergeSortDaSeparacao(matriz,vetorColunas,numVetorColunas,0,QualLinhaEstou,numeroColunas);//Chamamos a funcao de ordenacao!
				criacaoDeArquivo(matriz,QualArquivoSou,numArquivosCriados,numeroColunas,QualLinhaEstou);//Chamamos a funcao para podermos criar o arquivo.
			}
			break;
		}
	}
	delete[] buffer; //deletando o buffer utilizado para ler do arquivo.
}

int main (int argc, char **argv){
//-------------------------------- Arquivos de Entrada e Saida --------------------------------------------//
	char *NomeEnt1 = new char[100], *NomeEnt2 = new char[100], *NomeSai = new char[100]; // Auxiliares
	
	NomeDosArquivosPrincipais(NomeEnt1,NomeEnt2,NomeSai,argv); //Colocando .txt nos nomes dos arquivos
	
	ifstream fileOne; // Primeiro arquivo
	ifstream fileTwo; // Segundo arquivo
	
	ofstream fileOut; // Arquivo de saida
	
	fileOne.open(NomeEnt1); //Abrindo Primeiro Arquivo
	fileTwo.open(NomeEnt2); //Abrindo Segundo Arquivo
	fileOut.open(NomeSai); //Abrindo arquivo de saida
//------------------ Leitura/Armazenamento das Colunas de Comparacao! -------------------------------------//
	int *VetorColunasArq1 =(int*) malloc(1* sizeof(int)); //Vetores dinamicos para armazenar os indices
	int	*VetorColunasArq2 =(int*) malloc(1* sizeof(int)); //Vetores dinamicos para armazenar os indices
	int NumeroColunasArq1=0; //Cont para as Colunas, tambem usado para o realloc() dos vetores.
	int NumeroColunasArq2=0; //Cont para as Colunas, tambem usado para o realloc() dos vetores.
	int MaxLinhas; // M  é o número máximo de registros (linhas) que cabem em memória principal
	
	MaxLinhas = atoi(argv[1]); //armazenando argv[1] <=> Numero M maximo de registros
	
	char *quebra;
    
    ColunasDeComparacao(quebra,VetorColunasArq1,NumeroColunasArq1,argv,2); //Armazena as colunas de comparacao
	ColunasDeComparacao(quebra,VetorColunasArq2,NumeroColunasArq2,argv,3); //Armazena as colunas de comparacao
//------------------------------- Fim do processo das Colunas de Comparacao! ------------------------------//

//------------------------------- Leitura dos Arquivo e Quebra de tais em subarquivos ---------------------//
	Linha *Matriz1;
	
	int numArquivos1=0; //Auxiliar
    int numArquivos2=0;  //Auxiliar
	int ColunasArquivo1 =1; //Auxiliar
	int ColunasArquivo2 =1; //Auxiliar
	
	leArquivo(fileOne,Matriz1,NomeEnt1,MaxLinhas,VetorColunasArq1,NumeroColunasArq1,1,numArquivos1,ColunasArquivo1); // Lê e joga em subarquivos o arquivo1
	leArquivo(fileTwo,Matriz1,NomeEnt2,MaxLinhas,VetorColunasArq2,NumeroColunasArq2,2,numArquivos2,ColunasArquivo2); // Lê e joga em subarquivos o arquivo2
//------------------------------------ Fim da Leitura dos Arquivo -----------------------------------------//

//--------------------------------- Juntando os subarquivos e depois Intercalando -------------------------//
	ofstream junto1; 
	junto1.open("junto1"); // Abre o arquivo junto1, arquivo que vai conter o arquivo1 ordenado
	JuntaArq(junto1,numArquivos1,1,ColunasArquivo1,VetorColunasArq1,NumeroColunasArq1); // Junta os subarquivos do arquivo1 em só um arquivo ordenado 
	junto1.close(); //Fecha arquivo junto2
	
	ofstream junto2;
	junto2.open("junto2"); // Abre o arquivo junto2, arquivo que vai conter o arquivo2 ordenado
	JuntaArq(junto2,numArquivos2,2,ColunasArquivo2,VetorColunasArq2,NumeroColunasArq2); // Junta os subarquivos do arquivo2 em só um arquivo ordenado 
	junto2.close(); //Fecha arquivo junto1
	
	ifstream oPrimeiro; // Abre de novo o arquivo junto1 para ser usando novamente como ifstream
	oPrimeiro.open("junto1");
	
	ifstream oSegundo; // Abre de novo o arquivo junto2 para ser usando novamente como ifstream
	oSegundo.open("junto2");
	
	unindoGigantes(oPrimeiro,oSegundo,fileOut,NumeroColunasArq1,VetorColunasArq1,VetorColunasArq2,ColunasArquivo1,ColunasArquivo2); // Intercala os dois arquivos
//------------------------------------- Fim da Junção e da Intercalação -----------------------------------//

//----------------------------------- Liberando memorias alocadas! ----------------------------------------//
	delete []NomeEnt1; 
	delete []NomeEnt2;
	delete []NomeSai;
	
	free(VetorColunasArq1);
	free(VetorColunasArq2);
//----------------------------------- Fim da liberacao! ---------------------------------------------------//

//----------------------------------------- Fechando os Arquivos ------------------------------------------//
	fileOne.close(); // Fechando o Primeiro Arquivo
	fileTwo.close(); // Fechando o Segundo Arquivo
	fileOut.close(); // Fechando o Arquivo Que contém a Interlação dos dois anteriores
//--------------------------------  Fechamento dos Arquivos concluidos ------------------------------------//
	return 0;
}