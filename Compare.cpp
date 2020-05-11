#include<bits/stdc++.h>
using namespace std;

int main(){
	char a1[261],a2[261];
	long double c1,c2;
	int c3,c4;
	int i=0, j=1;
	
	cout << "Digite o nome do primeiro arquivo de saida (com extensao): ";
	cin.getline(a1,261);
	cout << "\n";
	cout << "Digite o nome do segundo arquivo de saida (com extensao): ";
	cin.getline(a2,261);
	cout << "\n";
			
	ifstream f1;
	ifstream f2;
	ofstream f3("Dif.txt");
		
	
	f1.open(a1);
	f2.open(a2);
	
	if(f1.fail() || f2.fail())
		goto x;
	
	while(true)
	{
		f1 >> c3 >> c1;
		f2 >> c4 >> c2;
		if(c1!=c2 || c3!=c4)
		{
			i++;
			f3 << "Linha " << j << " - " << i << " diferenca: " << c3 << " " << c1 << " " << c2 << " " << c1 << endl;
		}
		j++;
		if(f1.eof() || f2.eof())
			break;
	}
	
	cout << "Linhas diferentes: " << i << endl;
	cout << "Resultados detalhados foram salvos em dif.txt" << endl;
	
	if(false)
	{
	x:
	cerr << "Erro ao abrir arquivos" << endl;
	}
	
	f1.close();
	f2.close();
	
	return 0;
}