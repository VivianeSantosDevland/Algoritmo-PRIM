#include<stdio.h>
#include<stdlib.h>
#include<locale.h>

#define maxVertice 20

struct listaVertices
{
	int pesoAresta;
	int origemAdjacencia;
	int destinoAdjacencia;
	struct listaVertices *proximo_lista;
};

typedef struct listaVertices *vertices;

vertices add_lista(int peso, int linhaAdjacencia, int colunaAdjacencia, vertices anterior)
{
	vertices verticeNew = (vertices)malloc(sizeof(struct listaVertices));
	verticeNew->origemAdjacencia = linhaAdjacencia;
	verticeNew->destinoAdjacencia = colunaAdjacencia;
	verticeNew->pesoAresta = peso;	
	verticeNew->proximo_lista = NULL;
	if(anterior != NULL)
	{
		anterior->proximo_lista = verticeNew;
	}
	return verticeNew;//esse passa a ser o novo anterior	
}
/*este m�todo pega os valores da matriz de adjac�ncia passada, valores diferentes de 0,
 e coloca numa lista encadeada simples, guardando o peso e o v�rtice de origem e destino
*/
vertices matrizAdjacenciaLista(int matriz[][maxVertice], int numeroVertices)
{
	//vou colocar na lista apenas os pesos diferentes de 0
	vertices inicioLista = NULL;
	vertices verticesLista = NULL;
	int i, j;
	
	for(i = 0; i < numeroVertices; i++)
	{
		//printf("\nfor de i");
		for(j = 0; j < numeroVertices; j++)
		{
			//printf("\nfor de j");
			if(matriz[i][j] != 0)
			{
				//printf("\n [%d, %d] � %d", i, j, matriz[i][j]);
				if(inicioLista == NULL)
				{
					inicioLista = add_lista(matriz[i][j], i, j, NULL);//primeiro n�
				}else
				{
					if(verticesLista == NULL)
					{
						verticesLista = add_lista(matriz[i][j], i, j, inicioLista);//segundo n�
						
					}else
					{
						verticesLista = add_lista(matriz[i][j], i, j, verticesLista);//outros n�s
					}
				}
			}/*else
			{
				printf("\n [%d, %d] � 0", i, j);
			}*/
		}
	}
	
	return inicioLista;//retorna o in�cio da lista
}

void imprimeListaAdjacencia(vertices inicioLista)
{
	vertices percorreLista = inicioLista;
	while(percorreLista->proximo_lista != NULL)
	{
		printf("\nPeso: %d\tAresta: (%d, %d)", percorreLista->pesoAresta, percorreLista->origemAdjacencia, percorreLista->destinoAdjacencia);
		percorreLista = percorreLista->proximo_lista;
	}
	//conte�do que aponta para NULL
	printf("\nPeso: %d\tAresta: (%d, %d)", percorreLista->pesoAresta, percorreLista->origemAdjacencia, percorreLista->destinoAdjacencia);
}


int verificaListaVisitados(vertices listaVerticesVisitados, int origem, int destino)
{
	vertices percorreLista = listaVerticesVisitados;
	int resultado = 0;
	while(percorreLista->proximo_lista != NULL)
	{//(0,1) (1,0)
		if(percorreLista->origemAdjacencia == destino && percorreLista->destinoAdjacencia == origem)
		{
			//printf("\n(%d, %d)J� t� na lista como (%d, %d)", origem, destino, percorreLista->origemAdjacencia, percorreLista->destinoAdjacencia);
			resultado = 1;
			break;
		}/*else
		{
			//printf("\n(%d, %d)N�O t� na lista", origem, destino);
			//resultado = 0;
		}*/
		percorreLista = percorreLista->proximo_lista;
	}
	//para fazer compara��o do n� que aponta para NULL
		if(percorreLista->origemAdjacencia == destino && percorreLista->destinoAdjacencia == origem)
		{
			//printf("\n(%d, %d)J� t� na lista como (%d, %d)", origem, destino, percorreLista->destinoAdjacencia, percorreLista->origemAdjacencia);
			resultado = 1;
		}/*else
		{
			printf("\n(%d, %d)N�O t� na lista", origem, destino);
			resultado = 0;
		}
	*/
	return resultado;
}

/*A fun��o a seguir verifica se a conex�o a ser feita entre o v�rtice origem e destino formar� um ciclo
para isso busca saber quem � o par de origem, e quem � o par de destino na lista de visitados?
se forem iguais, forma um ciclo ex: (4, 2) (3, 4) (4, 5) (5, 3), no caso a �ltima aresta 5-3 n�o deve entrar na MTS 
porque o 3 est� conectado a 4, e 5 tamb�m est� conectado a 4, se for adicionar o 3 e 5 como uma arestaformar� um ciclo
	
*/
int verificaCiclo(vertices listaVerticesVisitados, int origem, int destino)
{
	int resultado = 0;
	vertices percorreListaVisitados = listaVerticesVisitados;
	vertices verificacaoInternaListaVisitados = listaVerticesVisitados;
	int achou = 0;
	while(percorreListaVisitados->proximo_lista != NULL)
	{	
		if(percorreListaVisitados->origemAdjacencia == destino)
		{
			//vai verificar a destino de percorreListaVisitados
			if(percorreListaVisitados->destinoAdjacencia == origem)
			{
				
				achou = percorreListaVisitados->origemAdjacencia;
				resultado = verificaListaVisitados(verificacaoInternaListaVisitados, achou, destino);
				if(resultado == 1)
				{
					break;//se j� encontrou pode sair do loop
				}
			}
		
		}
		percorreListaVisitados = percorreListaVisitados->proximo_lista;
	}
	//n� que aponta para NULL...n�o precisou aqui
	
	return resultado;
}


//Este m�todo encontra o "melhor caminho" considerando o menor peso do v�rtice origem para o v�rtice de destino
vertices TreeMin(vertices listaGrafoG, int numeroVertices)
{
	int numVertices;
	int booleanVerificaListaVisitados;
	int booleanVerificaCiclo;
	vertices percorreLista = listaGrafoG;
	vertices listaVisitados = NULL;
	vertices anterior = NULL;
	vertices atual = NULL;
	vertices arvoreGeradoraMinima = NULL;
	vertices mts = NULL;
	vertices menorPeso;
	
	menorPeso = percorreLista;
	anterior = percorreLista;
	atual = percorreLista->proximo_lista;
	
		anterior = percorreLista;
		atual = percorreLista->proximo_lista;
		
		for(numVertices = 0; numVertices < numeroVertices; numVertices++)
		{
			//printf("\n i = %d\n\n", numVertices);
			while(percorreLista->origemAdjacencia == numVertices && percorreLista->proximo_lista != NULL)//mesmo v�rtice de origem
			{
				//printf("\n i = %d", numVertices);
				//printf("\nanterior->origem: %d\t atual->origem: %d MenorPeso: %d", anterior->origemAdjacencia, atual->origemAdjacencia, menorPeso->pesoAresta);
				if(atual->pesoAresta < anterior->pesoAresta)
				{
					menorPeso = atual;
					//printf("\n MenorPeso: %d", menorPeso->pesoAresta);
					
					if(mts == NULL)	
					{
						mts = add_lista(menorPeso->pesoAresta, menorPeso->origemAdjacencia, menorPeso->destinoAdjacencia, NULL);
						arvoreGeradoraMinima = mts;
						//printf("\n adicionando o primeiro v�rtice da lista... %d", menorPeso->pesoAresta);
					}else
					{
						booleanVerificaListaVisitados = verificaListaVisitados(arvoreGeradoraMinima, menorPeso->origemAdjacencia, menorPeso->destinoAdjacencia);
						if(booleanVerificaListaVisitados != 1)
						{
							booleanVerificaCiclo = verificaCiclo(arvoreGeradoraMinima, menorPeso->origemAdjacencia, menorPeso->destinoAdjacencia);
							//printf("\n verificando lista de visitados... %d", booleanVerificaListaVisitados);
							if(booleanVerificaCiclo != 1)
							{
								if(listaVisitados == NULL)
								{
									listaVisitados = add_lista(menorPeso->pesoAresta, menorPeso->origemAdjacencia, menorPeso->destinoAdjacencia, arvoreGeradoraMinima);
									//printf("\n adicionando o segundo v�rtice da lista... %d", menorPeso->pesoAresta);
								}else
								{
									listaVisitados = add_lista(menorPeso->pesoAresta, menorPeso->origemAdjacencia, menorPeso->destinoAdjacencia, listaVisitados);
									//printf("\n adicionando outro v�rtice na lista... %d", menorPeso->pesoAresta);
								}
							}
						}
					}
				}
				//menorPeso = atual;
				anterior = atual;
				if(atual->proximo_lista != NULL)
				{
					atual = atual->proximo_lista;	
				}			
				if(numVertices == (numeroVertices - 1))
				{
					break;
				}
				percorreLista = percorreLista->proximo_lista;
			}
}
		
	return arvoreGeradoraMinima;
}
//Este m�todo faz a soma dos pesos da �rvore geradora m�nima
int somaPesosCustoMinimo(vertices arvoreGeradoraMinima)
{
	vertices percorreListaMTS = arvoreGeradoraMinima;
	int custoMinimo = 0;
	while(percorreListaMTS->proximo_lista != NULL)
	{
		custoMinimo = custoMinimo + percorreListaMTS->pesoAresta;
		percorreListaMTS = percorreListaMTS->proximo_lista;
	}
	//para n� que aponta para NULL
	custoMinimo = custoMinimo + percorreListaMTS->pesoAresta;
	return custoMinimo;
}

void main()
{
	setlocale(LC_ALL, "Portuguese");
	
	int nVertices = 0;
	int linha = 0;
	int coluna = 0;
	FILE *arquivoMatrizPesos;
	int aresta_peso[maxVertice][maxVertice];
	int custoMinimo = 0;
	vertices grafoG = NULL;
	vertices arvoreGeradoraMinima = NULL;
	//Entrada do n�mero de v�rtices (colocar para calcular com base no arquivo de pesos depois)
	printf("Entre com o n�mero de v�rtices: ");
	scanf("%d", &nVertices);
	
	arquivoMatrizPesos = fopen("pesos2.txt", "r");
	
	if(arquivoMatrizPesos != NULL)
	{
		printf("\n O arquivo foi aberto com sucesso!\n");
	
	for(linha = 0; linha < nVertices; linha++)
	{
		for(coluna = 0; coluna < nVertices; coluna++)
		{
			/*Para entrada manual da matriz:
			printf("\nInforme o peso da aresta da linha %d, coluna %d: ", linha, coluna);
			scanf("%d", &aresta_peso[linha][coluna]);*/
			//Leitura a partir de arquivo txt
			fscanf(arquivoMatrizPesos, "%d", &aresta_peso[linha][coluna]);
		}
	}
	
		printf("\n ------------- Matriz de pesos encontrada -------------\n\n");
		for(linha = 0; linha < nVertices; linha++)
		{
			printf("\n");
			for(coluna = 0; coluna < nVertices; coluna++)
			{
				printf(" %d  ", aresta_peso[linha][coluna]);
			}
		}
	}else
	{
		printf("\nErro ao abrir arquivo");
	}
	fclose(arquivoMatrizPesos);
	
	//PONDO NA LISTA DE ADJAC�NCIA
	grafoG = matrizAdjacenciaLista(aresta_peso, nVertices);
	vertices inicioLista = grafoG;
	printf("\n\n ------------ Grafo gerado ------------\n\n");
	imprimeListaAdjacencia(inicioLista);
	arvoreGeradoraMinima = TreeMin(grafoG, nVertices);
	printf("\n\n ------------ �rvore geradora m�nima (MTS) ------------\n\n");
	imprimeListaAdjacencia(arvoreGeradoraMinima);
	custoMinimo = somaPesosCustoMinimo(arvoreGeradoraMinima);
	printf("\nCusto m�nimo: %d", custoMinimo);
}
