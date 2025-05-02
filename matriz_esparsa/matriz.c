#include "matriz.h"
#include <stddef.h>

void le_matriz(matriz *mat)
{
    // Declaração de variáveis locais para armazenar as dimensões da matriz (número de linhas e colunas)
    int m, n;

    // Lê o número de linhas e colunas da matriz
    printf("Digite o número de linhas e colunas (por exemplo: 4 4): ");
    scanf("%d %d", &m, &n);

    // Armazena os valores lidos na estrutura da matriz apontada
    mat->m_linhas = m;
    mat->n_colunas = n;

    // Aloca dinamicamente uma célula especial chamada "raiz"
    // Essa célula será usada como âncora para acessar as listas de cabeças de linhas e colunas
    mat->raiz = malloc(sizeof(celula));

    // Define os campos linha e coluna da raiz como -1 (por convenção, indicando célula especial)
    mat->raiz->linha = -1;
    mat->raiz->coluna = -1;

    // Inicializa a raiz apontando para si mesma em ambas as direções (direita e abaixo)
    mat->raiz->direita = mat->raiz;
    mat->raiz->abaixo = mat->raiz;

    // === Criação das CÉLULAS-CABEÇA das COLUNAS ===

    // Ponteiro auxiliar para manter a última célula ligada (inicia na raiz)
    apontador ult = mat->raiz;

    // Para cada coluna, cria uma célula-cabeça associada
    for (int j = 0; j < n; j++)
    {
        // Aloca uma nova célula para a coluna j
        apontador nova = malloc(sizeof(celula));

        // Como é cabeça de coluna, define linha = -1 e coluna = índice da coluna
        nova->linha = -1;
        nova->coluna = j;

        // Inicializa o ponteiro "direita" como NULL (não é usado nas cabeças de coluna)
        nova->direita = NULL;

        // A célula aponta para si mesma no eixo vertical, formando lista circular (sem elementos ainda)
        nova->abaixo = nova;

        // Liga a última célula com essa nova à direita (horizontalmente)
        ult->direita = nova;

        // Atualiza o ponteiro ult para a próxima iteração
        ult = nova;
    }

    // Fecha a lista circular de colunas ligando a última célula criada de volta à raiz
    ult->direita = mat->raiz;

    // === Criação das CÉLULAS-CABEÇA das LINHAS ===

    // Reinicia o ponteiro ult para criar as cabeças de linha
    ult = mat->raiz;

    // Para cada linha, cria uma célula-cabeça associada
    for (int i = 0; i < m; i++)
    {
        // Aloca uma nova célula para a linha i
        apontador nova = malloc(sizeof(celula));

        // Define o índice da linha, e coluna = -1 por ser célula-cabeça
        nova->linha = i;
        nova->coluna = -1;

        // A célula aponta para si mesma na horizontal, formando lista circular (sem elementos ainda)
        nova->direita = nova;

        // Inicializa o ponteiro "abaixo" como ela mesma (boa prática para segurança de circularidade)
        nova->abaixo = nova;

        // Liga a última célula criada com essa nova na vertical (abaixo)
        ult->abaixo = nova;

        // Atualiza o ponteiro ult
        ult = nova;
    }

    // Fecha a lista circular de linhas ligando a última célula de volta à raiz
    ult->abaixo = mat->raiz;

    // === Inserção dos ELEMENTOS diferentes de zero ===

    int i, j;        // Índices da linha e coluna do elemento
    float valor;     // Valor do elemento

    // Agora, vamos pedir para o usuário inserir todos os elementos da matriz de uma vez
    printf("Digite os elementos da matriz (para cada elemento, insira linha, coluna e valor):\n");
    printf("Exemplo: 0 0 50  para inserir o valor 50 na posição [0][0]\n");

    // Loop para ler todos os elementos da matriz
    for (i = 0; i < m; i++)
    {
        for (j = 0; j < n; j++)
        {
            printf("Digite o valor para a posição [%d][%d]: ", i, j);
            scanf("%f", &valor);

            // Se o valor for 0, podemos ignorar a inserção
            if (valor == 0) continue;

            // Aloca uma nova célula para armazenar esse elemento não nulo
            apontador nova = malloc(sizeof(celula));

            // Preenche os campos da célula com os dados do elemento
            nova->linha = i;
            nova->coluna = j;
            nova->valor = valor;

            // === Inserção NA LISTA DA LINHA ===

            // Localiza a cabeça da linha i (descendo a partir da raiz)
            apontador pLinha = mat->raiz->abaixo;
            for (int k = 0; k < i; k++)
            {
                pLinha = pLinha->abaixo;
            }

            // Percorre a lista da linha até encontrar a posição correta (ordem crescente por coluna)
            apontador p = pLinha;
            while (p->direita != pLinha && p->direita->coluna < j)
            {
                p = p->direita;
            }

            // Insere a nova célula entre p e p->direita
            nova->direita = p->direita;
            p->direita = nova;

            // === Inserção NA LISTA DA COLUNA ===

            // Localiza a cabeça da coluna j (andando à direita a partir da raiz)
            apontador pColuna = mat->raiz->direita;
            for (int k = 0; k < j; k++)
            {
                pColuna = pColuna->direita;
            }

            // Percorre a lista da coluna até encontrar a posição correta (ordem crescente por linha)
            p = pColuna;
            while (p->abaixo != pColuna && p->abaixo->linha < i)
            {
                p = p->abaixo;
            }

            // Insere a nova célula entre p e p->abaixo
            nova->abaixo = p->abaixo;
            p->abaixo = nova;
        }
    }
}


void apaga_matriz(matriz *mat)
{
    // Começamos liberando todas as células de dados (elementos diferentes de zero)
    // Isso é feito percorrendo cada linha, ignorando as cabeças

    // Ponteiro auxiliar para andar pelas linhas (pLinha)
    apontador pLinha = mat->raiz->abaixo;

    // Enquanto não voltarmos à raiz, ainda há linhas a visitar
    while (pLinha != mat->raiz)
    {
        // Começamos pelo primeiro elemento da linha (após a cabeça)
        apontador atual = pLinha->direita;

        // Vamos percorrer a lista circular de elementos na linha
        while (atual != pLinha)
        {
            // Guardamos o próximo elemento antes de liberar o atual
            apontador temp = atual->direita;

            // Libera a célula atual
            free(atual);

            // Avança para o próximo elemento da linha
            atual = temp;
        }

        // Guardamos a cabeça da linha para liberar depois
        apontador tempCabeca = pLinha;

        // Avança para a próxima linha
        pLinha = pLinha->abaixo;

        // Libera a célula cabeça da linha
        free(tempCabeca);
    }

    // Agora liberamos as cabeças de coluna (sem acessar os dados, já foram apagados)
    apontador pColuna = mat->raiz->direita;

    // Percorremos a lista circular de cabeças de coluna
    while (pColuna != mat->raiz)
    {
        // Guardamos a célula para liberar depois
        apontador tempCabecaCol = pColuna;

        // Avança para a próxima coluna
        pColuna = pColuna->direita;

        // Libera a célula cabeça da coluna
        free(tempCabecaCol);
    }

    // Finalmente, liberamos a célula raiz
    free(mat->raiz);

    // Como segurança, colocamos o ponteiro da raiz como NULL
    mat->raiz = NULL;

    // Também podemos limpar os valores de m_linhas e n_colunas por segurança
    mat->m_linhas = 0;
    mat->n_colunas = 0;
}

void soma_matriz(const matriz *mat1, const matriz *mat2, matriz *mat_soma)
{
    // Verifica se as matrizes têm o mesmo número de linhas e colunas.
    // Isso é essencial porque só podemos somar matrizes com dimensões iguais.
    if(mat1->m_linhas != mat2->m_linhas || mat1->n_colunas != mat2->n_colunas)
    {
        printf("Erro: Matrizes de tamanhos diferentes.\n");
        return; // Interrompe a execução da função, pois não é possível somar.
    }

    // Inicializa as dimensões da matriz soma com as mesmas de mat1 e mat2.
    mat_soma->m_linhas = mat1->m_linhas;
    mat_soma->n_colunas = mat2->n_colunas;

    // Cria a célula raiz da matriz soma.
    mat_soma->raiz = malloc(sizeof(celula));
    mat_soma->raiz->linha = -1; // Identificador especial para raiz.
    mat_soma->raiz->coluna = -1; // Identificador especial para raiz.
    mat_soma->raiz->direita = mat_soma->raiz; // Inicializa como lista circular.
    mat_soma->raiz->abaixo = mat_soma->raiz;

    // === Criação das cabeças de coluna ===
    apontador ult = mat_soma->raiz;
    for (int j = 0; j < mat_soma->n_colunas; j++)
    {
        apontador nova = malloc(sizeof(celula)); // Aloca nova célula cabeça de coluna.
        nova->linha = -1; // Indica que é cabeça de coluna.
        nova->coluna = j; // Marca qual coluna representa.
        nova->abaixo = nova; // Inicia apontando para si mesma (lista circular).
        ult->direita = nova; // Liga a nova célula à lista raiz.
        ult = nova; // Atualiza o ponteiro "último".
    }
    ult->direita = mat_soma->raiz; // Fecha o ciclo circular entre colunas.

    // === Criação das cabeças de linha ===
    ult = mat_soma->raiz;
    for (int i = 0; i < mat_soma->m_linhas; i++)
    {
        apontador nova = malloc(sizeof(celula)); // Cria nova cabeça de linha.
        nova->linha = i; // Marca qual linha representa.
        nova->coluna = -1; // Indica que é cabeça de linha.
        nova->direita = nova; // Inicia lista circular da linha.
        ult->abaixo = nova; // Conecta à lista de cabeças de linha.
        ult = nova; // Avança para a próxima cabeça.
    }
    ult->abaixo = mat_soma->raiz; // Fecha o ciclo circular entre linhas.

    // === Processamento da soma elemento a elemento ===
    for (int i = 0; i < mat_soma->m_linhas; i++)
    {
        // Localiza a cabeça da linha i em cada uma das três matrizes
        apontador l1 = mat1->raiz->abaixo;
        apontador l2 = mat2->raiz->abaixo;
        apontador l_soma = mat_soma->raiz->abaixo;

        for (int k = 0; k < i; k++)
        {
            l1 = l1->abaixo; // Avança para linha i em mat1
            l2 = l2->abaixo; // Avança para linha i em mat2
            l_soma = l_soma->abaixo; // Avança para linha i em mat_soma
        }

        // Agora percorremos os elementos (não zero) da linha i
        apontador e1 = l1->direita; // Primeiro elemento da linha i em mat1
        apontador e2 = l2->direita; // Primeiro elemento da linha i em mat2

        // Enquanto ainda houver elementos nas duas listas
        while (e1 != l1 || e2 != l2)
        {
            int col_e1 = (e1 != l1) ? e1->coluna : mat_soma->n_colunas;
            int col_e2 = (e2 != l2) ? e2->coluna : mat_soma->n_colunas;

            float valor;
            int coluna;

            if (col_e1 == col_e2)
            {
                // As duas matrizes têm elemento na mesma posição.
                valor = e1->valor + e2->valor;
                coluna = col_e1;
                e1 = e1->direita;
                e2 = e2->direita;
            }
            else if (col_e1 < col_e2)
            {
                // Somente mat1 tem elemento nesta coluna.
                valor = e1->valor;
                coluna = col_e1;
                e1 = e1->direita;
            }
            else
            {
                valor = e2->valor;
                coluna = col_e2;
                e2 = e2->direita;
            }

            // Se a soma resultou em valor diferente de zero, adicionamos na mat_soma
            if (valor != 0)
            {
                apontador nova = malloc(sizeof(celula)); // Nova célula para mat_soma
                nova->linha = i;
                nova->coluna = coluna;
                nova->valor = valor;

                // === Inserção ordenada na linha ===
                apontador p = l_soma;
                while (p->direita != l_soma && p->direita->coluna < coluna)
                    p = p->direita;
                nova->direita = p->direita;
                p->direita = nova;

                // === Inserção ordenada na coluna ===
                apontador cabec_col = mat_soma->raiz->direita;
                for (int k = 0; k < coluna; k++)
                    cabec_col = cabec_col->direita;

                p = cabec_col;
                while (p->abaixo != cabec_col && p->abaixo->linha < i)
                    p = p->abaixo;
                nova->abaixo = p->abaixo;
                p->abaixo = nova;
            }
        }
    }
}

void multiplica_matriz(const matriz *mat1, const matriz *mat2, matriz *mat_produto)
{
    if (mat1->n_colunas != mat2->m_linhas)
    {
        printf("Erro: Dimensões incompatíveis para multiplicação.\n");
        return;
    }

    mat_produto->m_linhas = mat1->m_linhas;
    mat_produto->n_colunas = mat2->n_colunas;

    // Cria raiz
    mat_produto->raiz = malloc(sizeof(celula));
    mat_produto->raiz->linha = -1;
    mat_produto->raiz->coluna = -1;
    mat_produto->raiz->direita = mat_produto->raiz;
    mat_produto->raiz->abaixo = mat_produto->raiz;

    // Cabeçalhos de colunas
    apontador ult = mat_produto->raiz;
    for (int j = 0; j < mat_produto->n_colunas; j++)
    {
        apontador nova = malloc(sizeof(celula));
        nova->linha = -1;
        nova->coluna = j;
        nova->abaixo = nova;
        ult->direita = nova;
        ult = nova;
    }
    ult->direita = mat_produto->raiz;

    // Cabeçalhos de linhas
    ult = mat_produto->raiz;
    for (int i = 0; i < mat_produto->m_linhas; i++)
    {
        apontador nova = malloc(sizeof(celula));
        nova->linha = i;
        nova->coluna = -1;
        nova->direita = nova;
        ult->abaixo = nova;
        ult = nova;
    }
    ult->abaixo = mat_produto->raiz;

    // Multiplicação
    for (int i = 0; i < mat1->m_linhas; i++)
    {
        apontador linha_mat1 = mat1->raiz->abaixo;
        for (int l = 0; l < i; l++) linha_mat1 = linha_mat1->abaixo;

        for (int j = 0; j < mat2->n_colunas; j++)
        {
            float soma = 0.0;

            apontador a = linha_mat1->direita;
            while (a != linha_mat1)
            {
                int k = a->coluna;

                apontador linha_mat2 = mat2->raiz->abaixo;
                for (int l = 0; l < k; l++) linha_mat2 = linha_mat2->abaixo;

                apontador b = linha_mat2->direita;
                while (b != linha_mat2 && b->coluna < j) b = b->direita;

                if (b != linha_mat2 && b->coluna == j)
                    soma += a->valor * b->valor;

                a = a->direita;
            }

            if (soma != 0.0)
            {
                apontador nova = malloc(sizeof(celula));
                nova->linha = i;
                nova->coluna = j;
                nova->valor = soma;

                // Inserção na linha (sem ordenar)
                apontador linha_dest = mat_produto->raiz->abaixo;
                for (int l = 0; l < i; l++) linha_dest = linha_dest->abaixo;
                apontador p = linha_dest;
                while (p->direita != linha_dest)
                    p = p->direita;
                p->direita = nova;
                nova->direita = linha_dest;

                // Inserção na coluna (sem ordenar)
                apontador col_dest = mat_produto->raiz->direita;
                for (int c = 0; c < j; c++) col_dest = col_dest->direita;
                p = col_dest;
                while (p->abaixo != col_dest)
                    p = p->abaixo;
                p->abaixo = nova;
                nova->abaixo = col_dest;
            }
        }
    }
}


void imprime_matriz(const matriz* mat)
{
// Percorre cada linha
    apontador linha = mat->raiz->abaixo;
    while(linha != mat ->raiz)
    {
        // Ponteiro para o primeiro elemento não-nulo da linha
        apontador atual = linha ->direita;

        // Percorre as colunas da linha atual
        for(int j = 0; j < mat->n_colunas; j++)
        {
            // Se houver célula na posição (i,j), imprime o valor dela
            if(atual != linha && atual -> coluna == j)
            {
                printf("%.2f ", atual->valor);
                atual = atual->direita; // Avança para o próximo elemento não-nulo da linha
            }
            else
            {
                // Caso contrário, imprime zero
                printf("0.00 ");
            }
        }
        printf("\n"); // Pula para a próxima linha
        linha = linha->abaixo; // Avança para a próxima linha
    }

}

void insere_na_matriz (int linha, int coluna, float valor, matriz *mat)
{

}





