#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define TAM 4

typedef struct
{
    char tentativa[10];
    int certosPosicao;
    int certosErrados;
} Historico;

void gerarSequencia(int seq[])
{
    int usados[10] = {0};
    int i, num;

    srand(time(NULL));

    for(i = 0; i < TAM; i++)
    {
        do
        {
            num = rand() % 10;
        }while(usados[num]);

        usados[num] = 1;
        seq[i] = num;
    }
}

int validarEntrada(char entrada[])
{
    if(strlen(entrada) != 4)
        return 0;

    int usados[10] = {0};

    for(int i=0;i<4;i++)
    {
        if(entrada[i] < '0' || entrada[i] > '9')
            return 0;

        int num = entrada[i]-'0';

        if(usados[num])
            return 0;

        usados[num]=1;
    }

    return 1;
}

void analisarTentativa(int seq[], char entrada[], int *posicao, int *fora)
{
    *posicao = 0;
    *fora = 0;

    for(int i=0;i<TAM;i++)
    {
        int numero = entrada[i]-'0';

        if(numero == seq[i])
            (*posicao)++;
        else
        {
            for(int j=0;j<TAM;j++)
            {
                if(numero == seq[j])
                {
                    (*fora)++;
                    break;
                }
            }
        }
    }
}

void salvarArquivo(Historico historico[], int qtd)
{
    FILE *arquivo = fopen("historico.txt","w");

    if(arquivo == NULL)
    {
        printf("Erro ao criar arquivo.\n");
        return;
    }

    fprintf(arquivo,"======= HISTORICO DO JOGO =======\n\n");

    for(int i=0;i<qtd;i++)
    {
        fprintf(arquivo,
                "Tentativa %d: %s | Posicao correta: %d | Numero correto em posicao errada: %d\n",
                i+1,
                historico[i].tentativa,
                historico[i].certosPosicao,
                historico[i].certosErrados);
    }

    fclose(arquivo);
}

void mostrarSequencia(int seq[])
{
    printf("\nSequencia sorteada: ");

    for(int i=0;i<TAM;i++)
        printf("%d ",seq[i]);

    printf("\n");
}

void desempenho(int usadas,int max)
{
    float porcentagem = (float)usadas/max;

    printf("\n===== ANALISE DE DESEMPENHO =====\n");

    if(porcentagem <= 0.30)
        printf("Excelente desempenho!\n");

    else if(porcentagem <= 0.60)
        printf("Bom desempenho!\n");

    else if(porcentagem <= 0.90)
        printf("Desempenho razoavel.\n");

    else
        printf("Desempenho baixo. Tente novamente!\n");
}

int main()
{
    int sequencia[TAM];
    int maxTentativas;

    printf("      JOGO DE LOGICA NUMERICA\n");
    printf("\n");

    printf("Informe o numero maximo de tentativas: ");
    scanf("%d",&maxTentativas);

    gerarSequencia(sequencia);

    Historico historico[maxTentativas];

    int ganhou = 0;

    for(int tentativa=0; tentativa<maxTentativas; tentativa++)
    {
        char entrada[10];

        printf("\nTentativa %d de %d\n",
               tentativa+1,
               maxTentativas);

        do
        {
            printf("Digite 4 numeros diferentes (ex: 5381): ");
            scanf("%s",entrada);

            if(!validarEntrada(entrada))
                printf("Entrada invalida! Tente novamente.\n");

        }while(!validarEntrada(entrada));

        strcpy(historico[tentativa].tentativa,entrada);

        analisarTentativa(
            sequencia,
            entrada,
            &historico[tentativa].certosPosicao,
            &historico[tentativa].certosErrados
        );

        printf("Numeros na posicao correta: %d\n",
               historico[tentativa].certosPosicao);

        printf("Numeros corretos em posicao errada: %d\n",
               historico[tentativa].certosErrados);

        if(historico[tentativa].certosPosicao==4)
        {
            ganhou=1;

            printf("\nPARABENS! Voce descobriu a sequencia!\n");

            salvarArquivo(historico,tentativa+1);

            mostrarSequencia(sequencia);

            printf("Total de tentativas utilizadas: %d\n",
                   tentativa+1);

            desempenho(tentativa+1,maxTentativas);

            printf("\nHistorico salvo em historico.txt\n");

            return 0;
        }
    }

    printf("\nVOCE PERDEU!\n");

    mostrarSequencia(sequencia);

    printf("Total de tentativas utilizadas: %d\n",
           maxTentativas);

    desempenho(maxTentativas,maxTentativas);

    salvarArquivo(historico,maxTentativas);

    printf("\nHistorico salvo em historico.txt\n");

    return 0;
}