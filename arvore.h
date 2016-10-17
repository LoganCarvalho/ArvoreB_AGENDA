/*****************************************************ARVORE*B**************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define M 2
#define MM M*2

#define TRUE 1
#define FALSE 0

#define tam 100

typedef char String[tam];

typedef struct indice{
	
	String nomeCompleto;
	long telCelular;
/*******************************************************************************************************************/
	char situacao; //d: disponivel, i: indisponivel.
	String titulo;
	long posicao;
	long codigo;
}Indice;

typedef struct Pagina *Apontador;

typedef struct Pagina{   //declaracao dos vetores de indices(estruturas) e apontdores com uma adicao
	int n;
	Indice r[MM];
	Apontador p[MM+1];
}Pagina;

/*****************************************************PESQUISA**************************************************************/
Indice *PesquisaArvore(Indice IndAux, Apontador Ap, int id){
	
	long i=1;
	
	if(!(Ap)) return(NULL);
	
	while((id==1) && (i<Ap->n) && (IndAux.telCelular>Ap->r[i-1].telCelular)) i++;
	while((id==2) && (i<Ap->n) && (strcmpi(IndAux.nomeCompleto, Ap->r[i-1].nomeCompleto)>0)) i++;
	
	if(((id==1) && (IndAux.telCelular == Ap->r[i-1].telCelular)) || ((id==2) && (strcmpi(IndAux.nomeCompleto, Ap->r[i-1].nomeCompleto)==0)))
		return (&Ap->r[i-1]);
	
	if(((id==1) && (IndAux.telCelular<Ap->r[i-1].telCelular)) || ((id==2) && (strcmpi(IndAux.nomeCompleto, Ap->r[i-1].nomeCompleto)<0)))
		PesquisaArvore(IndAux, Ap->p[i-1], id);
	else
		PesquisaArvore(IndAux, Ap->p[i], id);
	
}

int StartsWith(const char *a, const char *b){ //método que busca uma substring 
   if(strncmp(a, b, strlen(b)) == 0) return 1;
   return 0;
}

/*****************************************************PESQUISA*BINARIA******************************************************/
int busca_binaria(Apontador aux, Indice Ind, int id){
	
	int meio, i, f;
	
	i=0;
	f = aux->n-1;
	
	while(i<=f){
		meio = (i+f)/2;
		if(aux->r[meio].codigo == Ind.codigo) return(meio);
		else if(aux->r[meio].codigo>Ind.codigo) f = meio-1;
		else i = meio+1;
	}
	return(i);
}


Indice *BuscaArvore(Indice Ind, Apontador Ap, int id){
	
	Apontador aux;
	int pos;
	aux = Ap;
	
	while(aux != NULL){
		pos = busca_binaria(aux, Ind, id);
		if(pos<aux->n && aux->r[pos].codigo == Ind.codigo) return(&aux->r[pos]);
		else aux = aux->p[pos];
	}
	return(NULL);
}

/*****************************************************INSERE****************************************************************/
void InsereNaPagina(Apontador Ap, Indice Ind, Apontador ApDir, int id){
	
	short NaoAchouPosicao;
	int k, x;
	
	k = Ap->n;
	NaoAchouPosicao = (k>0);
	
	while(NaoAchouPosicao){
		x = strcmpi(Ind.nomeCompleto, Ap->r[k-1].nomeCompleto);
		if(((id==1) && (Ind.telCelular>=Ap->r[k-1].telCelular)) || ((id==2) && (x>=0))){
			NaoAchouPosicao = FALSE;
			break;
		}
		Ap->r[k] = Ap->r[k-1];
		Ap->p[k+1] = Ap->p[k];
		k--;
		if(k<1) NaoAchouPosicao = FALSE;
	}
	
	Ap->r[k] = Ind;
	Ap->p[k+1] = ApDir;
	Ap->n++;
}

void Ins(Indice Ind, Apontador Ap, short *Cresceu, Indice *IndRetorno, Apontador *ApRetorno, int id, short *inseriu){

	long i=1, j;
	
	Apontador ApTemp;
	
	if(Ap == NULL){  //se o que vier do outro metodo for NULL
		*Cresceu = TRUE;
		*IndRetorno = Ind;
		*ApRetorno = NULL;
		return;
	}
	
	while((id==1) && (i<Ap->n) && (Ind.telCelular>Ap->r[i-1].telCelular)) i++; //n é a quantidade de elementos no vetor. I tem que ser sempre menos que n pra que haja espaço. Ele vai avançando até encontrar a posicao certa para a insercao
	while((id==2) && (i<Ap->n) && (strcmpi(Ind.nomeCompleto, Ap->r[i-1].nomeCompleto)>0)) i++;
	
	if(PesquisaArvore(Ind, Ap, 1) || PesquisaArvore(Ind, Ap, 2)){  //verifica se o contato já existe nas arvores
		printf("Erro: Contato ja esta cadastrado.\n");   
		system("pause");
		*inseriu = *Cresceu = FALSE;
		return;
	}
	
	if(((id==1) && (Ind.telCelular<Ap->r[i-1].telCelular)) || ((id==2) && (strcmpi(Ind.nomeCompleto, Ap->r[i-1].nomeCompleto)<0))) i--;
	
	Ins(Ind, Ap->p[i], Cresceu, IndRetorno, ApRetorno, id, inseriu);  //recussao
	
	if(!(*Cresceu)) return;
	
	if(Ap->n < MM){
		InsereNaPagina(Ap, *IndRetorno, *ApRetorno, id);
		*Cresceu = FALSE;
		return;
	}
	
	ApTemp = (Apontador) malloc(sizeof(Pagina));
	ApTemp->n = 0;
	ApTemp->p[0] = NULL;
	
	if(i< M+1){
		InsereNaPagina(ApTemp, Ap->r[MM-1], Ap->p[MM], id);
		Ap->n--;
		InsereNaPagina(Ap, *IndRetorno, *ApRetorno, id);
	}
	else 
		InsereNaPagina(ApTemp, *IndRetorno, *ApRetorno, id);
	
	for(j=M+2; j<=MM; j++)
		InsereNaPagina(ApTemp, Ap->r[j-1], Ap->p[j], id);
	
	Ap->n = M;
	ApTemp->p[0] = Ap->p[M+1];
	*IndRetorno = Ap->r[M];
	*ApRetorno = ApTemp;
}

short InsereArvore(Indice Ind, Apontador *Ap, int id){
	//chego nesse metodo com a copia do nome e telefone, o apontador da arvore especifico e um inteiro que pode ser 1 ou 2
	short inseriu = TRUE;
	short Cresceu = FALSE;
	Indice IndRetorno;
	Pagina *ApRetorno, *ApTemp;
	
	//passo pra esse metodo a copia do nome e telefone dentro de ind, o apontador especifico, as outras variaveis e o id que pode ser 1 ou 2
	Ins(Ind, *Ap, &Cresceu, &IndRetorno, &ApRetorno, id, &inseriu);
	
	if(Cresceu){
		ApTemp = (Pagina*) malloc(sizeof(Pagina));
		ApTemp->n = 1;
		ApTemp->r[0] = IndRetorno;
		ApTemp->p[1] = ApRetorno;
		ApTemp->p[0] = *Ap;
		*Ap = ApTemp;
	}
	return(inseriu);
}

/*****************************************************RETIRA*****************************************************************/
void Reconstitui(Apontador ApPag, Apontador ApPai, int PosPai, int *Diminuiu, int id){
	
	Apontador Aux;
  	int DispAux, j;
	  
	if(PosPai<ApPai->n){ 
    	Aux = ApPai->p[PosPai+1];
    	DispAux = (Aux->n - M + 1) / 2;
    	ApPag->r[ApPag->n] = ApPai->r[PosPai];
    	ApPag->p[ApPag->n + 1] = Aux->p[0];
    	ApPag->n++;
    	
    	if(DispAux > 0){
      		for (j = 1; j < DispAux; j++)
        		InsereNaPagina(ApPag, Aux->r[j-1], Aux->p[j], id);
        		
      		ApPai->r[PosPai] = Aux->r[DispAux - 1];
      		Aux->n -= DispAux;
      		
      		for(j = 0; j < Aux->n; j++)
        		Aux->r[j] = Aux->r[j + DispAux];
        		
      		for (j = 0; j <= Aux->n; j++)
        		Aux->p[j] = Aux->p[j + DispAux];
        		
      		*Diminuiu = 0;
    	}
    	else{
      		for (j = 1; j <= M; j++)
        		InsereNaPagina(ApPag, Aux->r[j-1], Aux->p[j], id);
      		free(Aux);
      		for (j = PosPai + 1; j < ApPai->n; j++){
        		ApPai->r[j-1] = ApPai->r[j];
        		ApPai->p[j] = ApPai->p[j+1];
     		}
      		ApPai->n--;
      		if (ApPai->n >= M)
        	*Diminuiu = 0;
    	}
	}
	else{ 
    	Aux = ApPai->p[PosPai - 1];
    	
    	DispAux = (Aux->n - M + 1) / 2;
    	
		for (j = ApPag->n; j >= 1; j--)
      		ApPag->r[j] = ApPag->r[j - 1];
      		
    	ApPag->r[0] = ApPai->r[PosPai - 1];
    	
    	for (j = ApPag->n; j >= 0; j--)
      		ApPag->p[j + 1] = ApPag->p[j];
      		
    	ApPag->n++;
    	
    	if(DispAux > 0){
      		for(j = 1; j < DispAux; j++)
        		InsereNaPagina(ApPag, Aux->r[Aux->n - j], Aux->p[Aux->n - j + 1], id);
        		
      		ApPag->p[0] = Aux->p[Aux->n - DispAux + 1];
      		ApPai->r[PosPai - 1] = Aux->r[Aux->n - DispAux];
      		
      		Aux->n -= DispAux;
      		*Diminuiu = 0;
    	}
		else{  
      		for(j = 1; j <= M; j++)
        		InsereNaPagina(Aux, ApPag->r[j - 1], ApPag->p[j], id);
        		
      		free(ApPag);
      		
      		ApPai->n--;
      		
      		if(ApPai->n >= M) *Diminuiu = 0;
    	}
  	}
} 

void Antecessor(Apontador Ap, int i, Apontador ApPai, int *Diminuiu, int id){
	
	if(ApPai->p[ApPai->n] != NULL){
    	Antecessor(Ap, i, ApPai->p[ApPai->n], Diminuiu, id);
    	if(*Diminuiu)
			Reconstitui(ApPai->p[ApPai->n], ApPai, ApPai->n, Diminuiu, id);
    	return;
  	}
  	
	Ap->r[i - 1] = ApPai->r[ApPai->n - 1];
  	ApPai->n--;
  	*Diminuiu = ApPai->n < M;
} 

void Ret(Indice Ind, Apontador *Ap, int *Diminuiu, int id, int *excluiu){

	int i, j;
  	Apontador Pag;
	  
	if(*Ap == NULL){
    	*Diminuiu = 0;
    	*excluiu = 0;
    	return;
	}
	
	Pag = *Ap;
  	i = 1;
  	
  	while(id==1&& i<Pag->n && Ind.telCelular>Pag->r[i-1].telCelular) i++;
  	while(id==2&& i<Pag->n && strcmpi(Ind.nomeCompleto, Pag->r[i-1].nomeCompleto)>0) i++;	  
	
	if((id==1 && Ind.telCelular == Pag->r[i-1].telCelular) || (id==2 && strcmpi(Ind.nomeCompleto, Pag->r[i-1].nomeCompleto)==0)){
		if(Pag->p[i-1] == NULL){
      		Pag->n--;
      		*Diminuiu = Pag->n < M;
      		for(j=i; j<=Pag->n; j++){
        		Pag->r[j-1] = Pag->r[j];
        		Pag->p[j] = Pag->p[j+1];
      		}
      		return;
    	}
    	Antecessor(*Ap, i, Pag->p[i-1], Diminuiu, id);
    	if(*Diminuiu) Reconstitui(Pag->p[i-1], *Ap, i-1, Diminuiu, id);
    	return;
	}	
	if((id==1&&Ind.telCelular > Pag->r[i - 1].telCelular)|| (id==2 && strcmpi(Ind.nomeCompleto, Pag->r[i-1].nomeCompleto)>0)) i++;
	
	Ret(Ind, &Pag->p[i - 1], Diminuiu, id, excluiu);
	
	if(*Diminuiu)
    	Reconstitui(Pag->p[i - 1], *Ap, i - 1, Diminuiu, id);
}

int Retira(Indice Ind, Apontador *Ap, int id){
	
	int excluiu=1, Diminuiu;
	Apontador Aux;
	
	Ret(Ind, Ap, &Diminuiu, id, &excluiu);
	
	if(Diminuiu && (*Ap)->n == 0){
		Aux = *Ap;
		*Ap = Aux->p[0];
		free(Aux);
	}
	
	return(excluiu);
}

/*****************************************************IMPRIME***************************************************************/
void ImprimeArvore(Apontador Ap){
	
	int i;
	
	if(Ap != NULL){
		for(i=0; i<Ap->n; i++){
			ImprimeArvore(Ap->p[i]);
			printf("t\tTitulo: %s", Ap->r[i].titulo);
			printf("t\tCodigo: %d\n", Ap->r[i].codigo);
			printf("Situacao: %c\n\n", Ap->r[i].situacao);
		}
		ImprimeArvore(Ap->p[i]);
	}
}
