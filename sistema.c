/*
Dúvidas:

Melhorias:
	fazer busca no título também ao adicionar um livro
	Passar trabalho pra sem cabeça
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX 100

// Estrutura da lista de livros existentes no acervo
typedef struct Livro {
	int id;
	char titulo[100];
	char autor[50];
	int anoPublicacao;
	int qtdade; // Total livros: disponíveis + emprestados
	int qtdisponivel; // Total de livros disponíveis
	struct Livro *prox;
} Livro;


// Estrutura da lista de exemplares emprestados
typedef struct Exemplar {
	int id;
	char titulo[100];
	char autor[50];
	long long int RGA;
	struct Exemplar *prox;
} Exemplar;

Livro *busca(Livro *livro, int id){
	if(livro == NULL)
		return NULL;
	if(livro->id == id)
		return livro;

	return busca(livro->prox, id);
}
Exemplar* exemplaresEmprestados = NULL;

// Adiciona um título a lista de livros
void adicionar(int id, char titulo[], char autor[], int ano, Livro *livro, int qtd){
    Livro *x = busca(livro, id);

    if (x == NULL) {
        Livro *nova = (Livro*)malloc(sizeof(Livro));

		if(nova == NULL){
			printf("Alocação de memória falhou!\n");
		}else{
	        strcpy(nova->titulo, titulo);
	        strcpy(nova->autor, autor);
	        nova->id = id;
	        nova->anoPublicacao = ano;
	        nova->qtdade = qtd;
	        nova->qtdisponivel = qtd;
	        nova->prox = NULL;

	        // Adicione o novo livro no final da lista
	        if (livro->prox == NULL) {
	            livro->prox = nova;
	        } else {
	            Livro *current = livro->prox;
	            while (current->prox != NULL) {
	                current = current->prox;
	            }
	            current->prox = nova;
	        }

	        printf("Livro adicionado com sucesso.\n");
		}
    } else {
        x->qtdade += qtd;
        x->qtdisponivel += qtd;
        printf("Livro atualizado com sucesso.\n");
    }
}


// Remove um título da lista de livros
void remover(int id, Livro *livro){
	Livro *p = livro, *q = livro->prox;

	for(; q != NULL && q->id != id;){
//		printf("p->id = %d, q->id = %d\n", p->id, q->id);
		p = q;
		q = q->prox;
	}

	if(q != NULL){
		p->prox = q->prox;
		free(q);
//		print(livro);
		printf("Livro removido com sucesso.\n");
	}else{
		printf("O título não está cadastrado.\n");
	}
}


// Lista os títulos da lista de livros
void listar_livros(Livro *livro){
	Livro *p = livro->prox;

	printf("Lista de livros na biblioteca:\n");
	for(; p != NULL; p = p->prox)
		printf("ID: %d | %s | %s | %d %d\n", p->id, p->titulo, p->autor, p->anoPublicacao, p->qtdade);
}


// Lista os títulos emprestados
void listar_emprestimo(){
	printf("Lista de livros emprestados:\n");
    Exemplar* atual = exemplaresEmprestados;
    while (atual != NULL) {
        printf("ID: %d | %s | %s | RGA: %lld\n", atual->id, atual->titulo, atual->autor, atual->RGA);
        atual = atual->prox;
    }
}


// Faz um emprestimo de livro
void emprestimo(int id, long long int RGA, Livro *livro){
	Livro* atual = livro->prox;

    while (atual != NULL && atual->id != id) {
        atual = atual->prox;
    }

    if (atual == NULL) {
        printf("Livro não encontrado.\n");
        return;
    }

    if (atual->qtdisponivel > 0) {
        Exemplar* novoExemplar = (Exemplar*)malloc(sizeof(Exemplar));

		if(novoExemplar == NULL){
			printf("Alocação de memória falhou!\n");
		}else{
			novoExemplar->id = atual->id;
	        strcpy(novoExemplar->titulo, atual->titulo);
	        strcpy(novoExemplar->autor, atual->autor);
	        novoExemplar->RGA = RGA;  // Pode adicionar o RGA do estudante que emprestou aqui
	        novoExemplar->prox = exemplaresEmprestados;
	        exemplaresEmprestados = novoExemplar;
	        atual->qtdisponivel--;
	        printf("Livro emprestado com sucesso.\n");
		}
    } else {
        printf("Livro esgotado. Não é possível emprestar.\n");
    }

}


// Faz a devolucao de um livro
void devolucao(int id, long long int RGA, Livro *livro){
	Exemplar* atual = exemplaresEmprestados;
    Exemplar* anterior = NULL;

    while (atual != NULL && atual->id != id && atual->RGA != RGA) {
        anterior = atual;
        atual = atual->prox;
    }

    if (atual == NULL) {
        printf("Exemplar não encontrado.\n");
        return;
    }

    if (anterior == NULL) {
        exemplaresEmprestados = atual->prox;
    } else {
        anterior->prox = atual->prox;
    }

    Livro* livroi = livro->prox;
    while (livroi != NULL && livroi->id != id) {
        livroi = livroi->prox;
    }

    if (livroi != NULL) {
        livroi->qtdisponivel++;
    }

    free(atual);
    printf("Livro devolvido com sucesso.\n");
}

void admin(char c, Livro *livro){
	char titulo[100], autor[100];
	int id, ano, qtd;
	long long int RGA;

	if(c == 'A' || c == 'a'){
		scanf(" %d \"%99[^\"]\" \"%99[^\"]\" %d %d", &id, titulo, autor, &ano, &qtd);
		adicionar(id, titulo, autor, ano, livro, qtd);
	}else if(c == 'R' || c == 'r'){
		scanf(" %d", &id);
		remover(id, livro);
	}else if(c == 'P' || c == 'p'){
		listar_livros(livro);
	}else if(c == 'E' || c == 'e'){
		scanf("%d %lld", &id, &RGA);
		emprestimo(id, RGA, livro);
	}else if(c == 'D' || c == 'd'){
		scanf("%d %lld", &id, &RGA);
		devolucao(id, RGA, livro);
	}else if(c == 'I' || c == 'i'){
		listar_emprestimo();
	}
}

void header(){
	printf(" || Sistema de Gerenciamento da Biblioteca || \n");
	printf("\n");
	printf("Comandos: A (Adicionar) - R (Remover) - P (Listar livros) -\n");
	printf("E (Empréstimo) - D (Devolução) - I (Listar empréstimos) - S (Sair)\n");
	printf("\n");
	printf("Digite um comando:\n");
}

// Recebe os valores
int main(){
	char c;
	Livro *livro = (Livro*)malloc(sizeof(Livro));

	if(livro == NULL){
		printf("Alocação de memória falhou!\n");
		return 0;
	}

	livro->prox = NULL;

	header();

	do{
		scanf(" %c", &c);
		admin(c, livro);
	}while(c != 'S' && c != 's');

	return 0;
}
