/*****************************************************PRINCIPAL*************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include "arvore.h"

typedef struct registro{
	String nomeCompleto;
	String dataNascimento;	
	String endereco;
	String profissao;
	String email;
	int telCelular;
	int telComercial;
	int telResidencial;
	int cPF;
/*******************************************************************************************************************/	
	int numExemplares;
	int qntDisponivel;
	String titulo;
	String autor;
	String tipo;
	String area;
	long codigo;
}Registro;

/*****************************************************GOTO*X*Y**************************************************************/
void gotoxy(int x, int y){
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),(COORD){x-1,y-1});
}

/*****************************************************INICIALIZA************************************************************/
void Inicializa(Apontador *Ap1, Apontador *Ap2, Indice *IndAux, Registro *RegAux){
	*Ap1 = *Ap2 = NULL;
	if(!(IndAux = (Indice*) malloc(sizeof(Indice)))) exit(1);
	if(!(RegAux = (Registro*) malloc(sizeof(Registro)))) exit(1);
}

/*****************************************************INSERE*ARQUIVO*PRINCIPAL***********************************************/
long InsereArquivoPrincipal(FILE *arquivoPrincipal, Registro RegAux){
	
	long posAux;
	
    //Abre o arquivo para atualizacao (leitura e escrita) por causa do parametro "a+b".
	if(!(arquivoPrincipal = fopen("arquivoPrincipal", "a+b"))){
		printf("Erro ao abrir arquivo principal.\n");
		system("pause");
		return 1;
	}
	
    //permite a leitura e escrita randomica, neste caso partindo do final do file de 0 em 0 bytes
	fseek(arquivoPrincipal, 0, SEEK_END);
	
    //retorna a posicao corrente de leitura ou escrita no arquivo (em bytes), ou seja,
    //ira retornar quantos bytes o arquivo possui
	posAux = ftell(arquivoPrincipal);
	
	fwrite(&RegAux, sizeof(Registro), 1, arquivoPrincipal);  //o fwrite permite escrever uma estrutura definida pelo programador
															//os paremtros são os dados a serem gravados, o tamanho dos dados,
															// a qtd de unidades a serem gravadas e o ponteiro para o arquivo.
															//a funcao retorna um int. se o retorno for != da qtd de unidades entao é erro na escrita.(VERIFICAR)
	
	fclose(arquivoPrincipal);
	
	return(posAux);
}

/*****************************************************CARREGA*ARQUIVO*INDICE************************************************/
void carregaArquivoIndice(FILE *arquivoIndice, Apontador *Ap1, Apontador *Ap2, int *qntRegistros){
	
	Indice IndAux;
	
	if(!(arquivoIndice = fopen("arquivoIndice", "rb"))){
		printf("Erro ao abrir arquivo indice.\n");
		system("pause");
		return;
	}
	
	while(!(feof(arquivoIndice))){
		fread(&IndAux, sizeof(Indice), 1, arquivoIndice);
		if(feof(arquivoIndice)) break;
		InsereArvore(IndAux, &*Ap1, 1);
		InsereArvore(IndAux, &*Ap2, 2);
		(*qntRegistros)++;
	}
	
	fclose(arquivoIndice);
}

/*****************************************************SALVA*ARQUIVO*INDICE**************************************************/
void salvaArquivoIndice(FILE *arquivoIndice, Pagina *Ap){

	int i;

	if(Ap != NULL){
		for(i=0; i<Ap->n; i++){
			salvaArquivoIndice(arquivoIndice, Ap->p[i]);
			fwrite(&Ap->r[i], sizeof(Indice), 1, arquivoIndice);
		}
		salvaArquivoIndice(arquivoIndice, Ap->p[i]);
	}
	
}

/*****************************************************PEGAR*REGISTRO********************************************************/
Registro pegaRegistroArquivoPrincipal(FILE *arquivoPrincipal, long pos){
	
	Registro RegAux;
	
	if(!(arquivoPrincipal = fopen("arquivoPrincipal", "rb"))){
		printf("Erro ao abrir arquivo principal.\n");
		system("pause");
        EXIT_FAILURE;
	}
	
	fseek(arquivoPrincipal, pos, SEEK_SET);

	fread(&RegAux, sizeof(Registro), 1, arquivoPrincipal);
	
	fclose(arquivoPrincipal);

	return(RegAux);	
}

/*****************************************************LE*REGISTRO*USUARIO***************************************************/
void leRegistroUsuario(Registro *RegAux){

	printf("\tNome Completo: ");
	fgets((*RegAux).nomeCompleto, tam, stdin);  //lendo os caracteres digitados pelo usuário e colocando na struct. Stdin é a const que representa o keyboard
	printf("\tData Nascimento: ");
	fgets((*RegAux).dataNascimento, tam, stdin);					
	printf("\tCPF: ");
	scanf("%d", &(*RegAux).cPF);
	fflush(stdin);
	printf("\tEndereço: ");
	fgets((*RegAux).endereco, tam, stdin);
	fflush(stdin);
	printf("\tProfissão: ");
	fgets((*RegAux).profissao, tam, stdin);
	printf("\tEmail: ");
	fgets((*RegAux).email, tam, stdin);
	printf("\tTelefone Celular: ");
	scanf("%d", &(*RegAux).telCelular);
	printf("\tTelefone Residêncial: ");
	scanf("%d", &(*RegAux).telResidencial);
	printf("\tTelefone Comercial: ");
	scanf("%d", &(*RegAux).telComercial);

/********************************************************************************************************/	
//	printf("\tTitulo: ");
//	fgets((*RegAux).titulo, tam, stdin);  //lendo os caracteres digitados pelo usuário e colocando na struct. Stdin é a const que representa o keyboard
//	printf("\tAutor: ");
//	fgets((*RegAux).autor, tam, stdin);					
//	printf("\tCodigo: ");
//	scanf("%ld", &(*RegAux).codigo);
//	fflush(stdin);
//	printf("\tArea: ");
//	fgets((*RegAux).area, tam, stdin);
//	fflush(stdin);
//	printf("\tTipo: ");
//	fgets((*RegAux).tipo, tam, stdin);
//	printf("\tExemplares: ");
//	scanf("%d", &(*RegAux).numExemplares);
//	(*RegAux).qntDisponivel=(*RegAux).numExemplares;

}

/*****************************************************IMPRIME*REGISTRO******************************************************/
void imprimeRegistro(FILE *arquivoPrincipal, Indice *Ind){
	
	Registro RegAux;
	
	if(Ind==NULL){
    	gotoxy(27, 10); printf("Erro: Pessoa nao Encontrada.\n");
		return;
	}
	
	RegAux = pegaRegistroArquivoPrincipal(arquivoPrincipal, Ind->posicao);

	printf("\tNome Completo: %s", RegAux.nomeCompleto);
	printf("\tData Nascimento: %s", RegAux.dataNascimento);
	printf("\tCPF: %ld\n", RegAux.cPF);
	printf("\tEndereco: %s", RegAux.endereco);
	printf("\tProfissao: %s", RegAux.profissao);
	printf("\tEmail: %s\n", RegAux.email);
	printf("\tTelefone Residencial: %ld\n", RegAux.telResidencial);
	printf("\tTelefone Celular: %ld\n", RegAux.telCelular);
	printf("\tTelefone Comercial: %ld\n", RegAux.telComercial);
	

}

/*****************************************************IMPRIME*TODOS*REGISTROS***********************************************/
void imprimeTodosRegistros(FILE *arquivoPrincipal, Apontador Ap){
	
	int i;
	
	if(Ap != NULL){
		for(i=0; i<Ap->n; i++){
			imprimeTodosRegistros(arquivoPrincipal, Ap->p[i]);
			imprimeRegistro(arquivoPrincipal, &Ap->r[i]);
		}
		imprimeTodosRegistros(arquivoPrincipal, Ap->p[i]);
	}
	
}

/*****************************************************EDITA*REGISTRO********************************************************/
void editaRegistro(FILE *arquivoPrincipal, Indice *Ind1, Indice *Ind2, Apontador *Ap1, Apontador *Ap2){
	
	Registro RegAux;
	Indice IndAux;
	
	if(Ind1==NULL || Ind2==NULL){
    	gotoxy(27, 10); printf("Erro: Exemplar nao encontrado.\n");
    	gotoxy(27, 11); system("pause");
		return;
    }
    
	imprimeRegistro(arquivoPrincipal, Ind1);
	
	Retira(*Ind1, &*Ap1, 1);
	Retira(*Ind2, &*Ap2, 2);
	
	leRegistroUsuario(&RegAux);
	
	strcpy(IndAux.titulo, RegAux.titulo); 
	IndAux.codigo = RegAux.codigo; 
	
	if(RegAux.numExemplares>0) IndAux.situacao='d';
	else IndAux.situacao='i';
	
	IndAux.posicao = InsereArquivoPrincipal(arquivoPrincipal, RegAux); 
	
	InsereArvore(IndAux, &*Ap1, 1); 
	InsereArvore(IndAux, &*Ap2, 2);
	
}

/*****************************************************RETIRA*REGISTRO*ARQUIVO*PRINCIPAL*************************************/
void retiraRegistroArquivoPrincipal(FILE *arquivoPrincipal, Indice *Ind){
	
	Registro RegAux;
	
	if(Ind == NULL){
    	printf("Nao foi possivel retirar o exemplar do arquivo principal.\n");
    	system("pause");
		return;
    }
	
	if(!(arquivoPrincipal = fopen("arquivoPrincipal", "r+b"))){
		printf("Erro ao abrir arquivo principal.\n");
		system("pause");
		return;
	}
	
	RegAux.codigo = -1;
	
	fseek(arquivoPrincipal, Ind->posicao, SEEK_SET);
	fwrite(&RegAux, sizeof(Registro), 1, arquivoPrincipal);
	
	fclose(arquivoPrincipal);
	
}

/*****************************************************EMPRESTA*EXEMPLAR*****************************************************/
void EmprestaExemplar(FILE *arquivoPrincipal, Indice *Ind1, Indice *Ind2){

	Registro RegAux;
    
	if(Ind1==NULL || Ind2==NULL){
    	printf("Exemplar nao encontrado.\n");
    	system("pause");
		return;
    }
    
    if(Ind1->situacao == 'd'){
        RegAux = pegaRegistroArquivoPrincipal(arquivoPrincipal, Ind1->posicao);
        RegAux.qntDisponivel--;
        Ind1->posicao = Ind2->posicao = InsereArquivoPrincipal(arquivoPrincipal, RegAux);
        if(RegAux.qntDisponivel<1)
        	Ind1->situacao = Ind2->situacao = 'i';
        gotoxy(27, 10);puts("Emprestimo realizado com sucesso!");
    }
    else{
    	gotoxy(27, 10);printf("Exemplar indisponinel para emprestimo.\n");
    }
    gotoxy(27, 11);system("pause");
}

/*****************************************************PRINCIPAL*************************************************************/
int main(int argc, char *argv[]) {

	int x;
	char opt='1';
	String strAux;
	Indice IndAux;
	Registro RegAux;
	Pagina *Ap1, *Ap2;
	int qntRegistros=0;
	Indice *IndX, *IndY;
	FILE *arquivoIndice;
	FILE *arquivoPrincipal;
	
	system("color 1e");
	
	Inicializa(&Ap1, &Ap2, &IndAux, &RegAux);    //reserva espaço para cada variável

	carregaArquivoIndice(arquivoIndice, &Ap1, &Ap2, &qntRegistros);
	
	while(1){
		system("cls");
        gotoxy(25, 2); printf("AGENDA DE CONTATOS");
		gotoxy(27, 9); printf("1 - Cadastrar Pessoa\n");
    	gotoxy(27, 10);printf("2 - Consultar Pessoa\n");
        gotoxy(27, 11);printf("3 - Editar Exemplar\n");
        gotoxy(27, 12);printf("4 - Retirar Exemplar\n");
        gotoxy(27, 13);printf("5 - Emprestar Exemplar\n");
        gotoxy(27, 14);printf("6 - Imprimir todos os exemplar\n");      
    	gotoxy(27, 15);printf("0 - Sair\n");
		gotoxy(27, 16);	fflush(stdin); scanf("%c", &opt);
		if(opt=='0') break;
		switch(opt){
			case '1': system("cls");
					gotoxy(25, 2); printf("AGENDA DE CONTATOS");
					gotoxy(27, 9); printf("Cadastrar Pessoa\n");
					fflush(stdin);  										// limpa o buffer do teclado no windows
					leRegistroUsuario(&RegAux);							     // método que lê o conteúdo digitado pelo user
					strcpy(IndAux.nomeCompleto, RegAux.nomeCompleto);         		    // copia o titulo de RegAux para o titulo de IndAux
					IndAux.telCelular = RegAux.telCelular; 							// copia o codigo de regAux para IndAux
		
					IndAux.posicao = InsereArquivoPrincipal(arquivoPrincipal, RegAux);
					if(InsereArvore(IndAux, &Ap1, 1) && InsereArvore(IndAux, &Ap2, 2))
					qntRegistros++;
					break;
			
			case '2':system("cls");
					gotoxy(25, 2); printf("AGENDA DE CONTATOS");
					gotoxy(27, 9); printf("Consultar Pessoa: ");
					fflush(stdin);
					fgets(strAux, tam, stdin);       //pega o que é digitado pelo usuario
					x = atoi(strAux);                //converte para inteiro
					if(x!=0){						 //se for um numero
						IndAux.telCelular = x;
						imprimeRegistro(arquivoPrincipal, PesquisaArvore(IndAux, Ap1, 1)); //a pesquisa esta sendo feita pelo telCelular 
					}
					else{
						strcpy(IndAux.nomeCompleto, strAux);
						imprimeRegistro(arquivoPrincipal, PesquisaArvore(IndAux, Ap2, 2));  //a pesquisa sendo feita pelo nome
					}
					printf("\t\t"); system("pause");
					break;
					
			case '3': system("cls");
					gotoxy(25, 2); printf("AGENDA DE CONTATOS");
					gotoxy(27, 9); printf("Contato a ser editado: ");
					fflush(stdin);
					fgets(strAux, tam, stdin);
					x = atoi(strAux);
					if(x!=0){
						IndAux.codigo = x;
						IndX = PesquisaArvore(IndAux, Ap1, 1);
						if(IndX) strcpy(IndAux.titulo, IndX->titulo);
						IndY = PesquisaArvore(IndAux, Ap2, 2);
						editaRegistro(arquivoPrincipal, IndX, IndY, &Ap1, &Ap2);
					}
					else{
						strcpy(IndAux.titulo, strAux);
						IndY = PesquisaArvore(IndAux, Ap2, 2);
						if(IndY) IndAux.codigo = IndY->codigo;
						IndX = PesquisaArvore(IndAux, Ap1, 1);
						editaRegistro(arquivoPrincipal, IndX, IndY, &Ap1, &Ap2);
					}
					break;
			
			case '4': system("cls");
					gotoxy(25, 2); printf("AGENDA DE CONTATOS");
					gotoxy(27, 9); printf("Contato a ser retirado: ");
					fflush(stdin);
					fgets(strAux, tam, stdin);
					x = atoi(strAux);
					if(x!=0){
						IndAux.codigo = x;
						IndX = PesquisaArvore(IndAux, Ap1, 1);
						retiraRegistroArquivoPrincipal(arquivoPrincipal, IndX);
						if(IndX) x = Retira(*IndX, &Ap1, 1);
						IndX = PesquisaArvore(IndAux, Ap2, 1);
						if(IndX) x = Retira(*IndX, &Ap2, 2);
					}
					else{
						strcpy(IndAux.titulo, strAux);
						IndX = PesquisaArvore(IndAux, Ap1, 2);
						retiraRegistroArquivoPrincipal(arquivoPrincipal, IndX);
						if(IndX) x = Retira(*IndX, &Ap1, 1);
						IndX = PesquisaArvore(IndAux, Ap2, 2);
						if(IndX) x = Retira(*IndX, &Ap2, 2);
					}
					if(x==1){
						gotoxy(27, 10);printf("Contato excluido com sucesso!\n");
						qntRegistros--;
					}
					else{
						gotoxy(27, 10);printf("Erro: Contato nao encontrado.\n");
					}
    				gotoxy(27, 11);system("pause");
					break;
					
			case '5': system("cls");
					gotoxy(25, 2); printf("AGENDA DE CONTTAOS");
					gotoxy(27, 9); printf("Emprestar exemplar: ");
					fflush(stdin);
					fgets(strAux, tam, stdin);
					x = atoi(strAux);
					if(x!=0){
						IndAux.codigo = x;
						IndX = PesquisaArvore(IndAux, Ap1, 1);
						if(IndX) strcpy(IndAux.titulo, IndX->titulo);
						IndY = PesquisaArvore(IndAux, Ap2, 2);
						EmprestaExemplar(arquivoPrincipal, IndX, IndY);
					}
					else{
						strcpy(IndAux.titulo, strAux);
						IndY = PesquisaArvore(IndAux, Ap2, 2);
						if(IndY) IndAux.codigo = IndY->codigo;
						IndX = PesquisaArvore(IndAux, Ap1, 1);
						EmprestaExemplar(arquivoPrincipal, IndX, IndY);
					}
					break;

			case '6': system("cls");
					gotoxy(25, 2); printf("AGENDA DE CONTATOS");
					if(!Ap1 || !Ap2){
						gotoxy(25, 9);printf("Nenhum exemplar cadastrado.\n");
						gotoxy(25, 10);system("pause");
						break;
					}
					gotoxy(25, 9); printf("Ordenado por titulo ou codigo? ");
					fflush(stdin);
					fgets(strAux, tam, stdin);
					printf("\n");
					if(strcmpi(strAux, "titulo\n")==0){
						gotoxy(25, 11);printf("Quantidade total de exemplares: %d\n\n", qntRegistros);
						imprimeTodosRegistros(arquivoPrincipal, Ap2); 
					}
					else if(strcmpi(strAux, "codigo\n")==0){
						gotoxy(25, 11);printf("Quantidade total de exemplares: %d\n\n", qntRegistros);
						imprimeTodosRegistros(arquivoPrincipal, Ap1);
					}	
					else{
						gotoxy(25, 10); printf("Nao eh possivel imprimir por %s", strAux);
						gotoxy(25, 11); system("pause");
						break;
					}
					system("pause");
					break;
			
			default:system("cls");
					gotoxy(25, 2); printf("AGENDA DE CONTATOS");
					gotoxy(25, 9); printf("Opcao invalida!\n");
					gotoxy(25, 10); system("pause");
		}
	}
	
	if(!(arquivoIndice = fopen("arquivoIndice", "wb"))){
		printf("Erro ao abrir arquivo indice.\n");
		system("pause");
		return;
	}
	
	salvaArquivoIndice(arquivoIndice, Ap1);
	
	fclose(arquivoIndice);
	
	return(0);
}
