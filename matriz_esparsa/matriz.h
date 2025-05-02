#ifndef MATRIZ_H_INCLUDED
#define MATRIZ_H_INCLUDED

//typedef struct celula_t { ... } celula;
//Aqui você está dizendo:
//"Estou criando uma struct chamada celula_t"
//"E também quero dar a ela um apelido mais curto: celula"


//Isso aqui é só um jeito de não ter que digitar struct celula_t * toda vez.
typedef struct celula_t *apontador;//Cria um apelido (typedef) chamado apontador, que representa um ponteiro para uma estrutura celula_t.Cria um tipo

typedef struct celula_t {//Começa a definição da struct celula_t, que é como uma classe em Java.
    apontador direita; // tipo um this.proximo numa lista ligada horizontal. vai ir para o proximo da direita na linha//ponteiro
    apontador abaixo;//como se fosse this.abaixo, para navegar verticalmente na matriz.vai ir para o proximo abaixo na coluna//ponteiro
    int linha;//indice da linha
    int coluna;//indice da coluna
    float valor;//o valor dentro da celula//valor real da célula
} celula;// Finaliza a definição e cria um apelido chamado celula

typedef struct {
    int m_linhas;
    int n_colunas;
    apontador raiz;
} matriz;

void le_matriz        (matriz *mat);
void apaga_matriz     (matriz *mat);
void soma_matriz      (const matriz *mat1, const matriz *mat2, matriz *mat_soma);
void multiplica_matriz(const matriz *mat1, const matriz *mat2, matriz *mat_produto);
void imprime_matriz   (const matriz* mat);
void insere_na_matriz (int linha, int coluna, float valor, matriz *mat);

#endif // MATRIZ_H_INCLUDED





/*Etapa 1 — Vamos dar um nome pra nossa peça (struct)
c
Copiar
Editar
struct celula_t { ... };
🧠 Isso aqui quer dizer:
"Estou criando um tipo de dado chamado celula_t, que é uma estrutura com vários campos."

🏷️ Etapa 2 — Eu quero dar apelidos para não precisar repetir nomes grandes
Primeiro apelido: apontador

c
Copiar
Editar
typedef struct celula_t *apontador;
📌 Isso diz:

"Toda vez que eu escrever apontador, estou falando de um ponteiro para struct celula_t."

Isso aqui é só um jeito de não ter que digitar struct celula_t * toda vez. 😅*/


