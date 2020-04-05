/*
Nome: Folha de exercícios nº 5 - Mastermind
Data: 04/04/2020
Autor: Diogo Luz

Resumo: O computador escolhe um padrão de quatro
pinos de código e o codebreaker/jogador tenta adivinhar o padrão, tanto na ordem como na cor, em doze
(ou dez, ou oito) tentativas.
Após cada tentativa, o codemaker fornece feedback colocando entre zero a quatro pinos, um pino
colorido por cada peça certa colocada na posição certa e um pino sem cor por cada peça certa
colocada na posição errada. O jogo termina quando é adivinhado o padrão ou quando for atingido
o limite de tentativas. 
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <locale.h>
#include <time.h>

#include "rlutil.h" //bibliotteca de terceiros que acrescenta funcionalidades de alterar opções como a cor do texto e o background da consola do nosso programa

#define CORMAX 53 //define o numero 53 que equivale ao numero 5 da tabela ASCII
#define CORMIN 48 //define o numero 48 que equivale ao numero 0 da tabela ASCII

#define NORMALCONSOLA 15 //para voltar a ter a cor branca no fim de escrever os codigos
#define VERMELHO 12 //para escrever em vermelho no setColor

//o codigo e as tentativas ficam definidos numa variavel global visto que vão ser utilizados por todas as funções
char codigo[4];
int tentativasFeitas = 0;
char palpitesFeitos[10][4]; //10 palpites possíveis de 4 digitos
char palpitesAcertos[10][4]; //variavel para guardar os resultados dos palpites (ACERTO/ACERTO+POSICAOCERTA/NADA)

//esta funcao serve para mostrar um menu ao utilizador para que este possa escolher entre um novo jogo ou sair
int menu() {

    int opcao;

    //apresentação do menu ao utilizador
    printf(" Feito por: Diogo Luz\n\n");
    printf("      MASTERMIND\n");
    printf("***********************\n");
    printf("1. Novo jogo\n");
    printf("2. Sair\n");
    printf("***********************\n");
    printf("Opcao: ");
    
    fflush(stdin);
	scanf("%d", & opcao);

    //return da opção para que fique na variável que chama esta função
    return opcao;
}

//esta funcao inicializa todas as tentativas efeutadas no inicio de um novo jogo
void inicializar_tentativas(){
	int i,j;
	
	for (i=0;i<10;i++){
		for (j=0;j<4;j++){
			palpitesFeitos[i][j] = ' ';
			palpitesAcertos[i][j] = ' ';
		}
	}
	
	tentativasFeitas = 0;
}

//vamos gerar o codigo secreto de 4 digitos aleatoriamente no intervalo de 0 a 5 correspondente ao intervalo 48 a 53 da tabela ASCII
void gerador_codigo(){
	int r,i;
	
	//vamos gerar um digito de cada vez com intervalo definido nas constantes (0 a 5 em ascii 48 a 53)
	for(i=0;i<4;i++){
		r = (rand() % (CORMAX + 1 - CORMIN)) + CORMIN;
		codigo[i]=r;
	}
}

//esta funcao serve para escrever o digito passado como parâmetro de entrada com a cor correspondente
void escreve_cor(char digito){
	//caso ainda não tenha nada guardado na prosicao entao escreve apenas o espaço
	if (digito == ' '){
		printf("%c",digito);
	}else{
		//as cores que escolhi vão de 9 a 14 no setcolor, assim tenho de retirar 39 a cada uma das cores para as escrever no ecrã
		setColor((int)digito-39);
		
		//para escrever o inteiro de 0 a 5 tenho de retirar 48 do random ASCII com que trabalhamos para usar chars
		printf("%d",(int)digito-48);
		
		setColor(NORMALCONSOLA);	
	}
}

//esta função escreve o código passado na variável de entrada com recurso à função escreve_cor para o escrever de forma colorida
void escreve_codigo(char codEscrever[4]){
	int i;
	
	for (i=0;i<4;i++){
		escreve_cor(codEscrever[i]);
	}
	
	//printf("\n");
}

//esta funcao escreve as dicas em relacao ao palpite efetuado pelo utilizador com as cores branco ou vermelho se a cor ou posicao + cor estão corretas
void escreve_dicas(char codEscrever[4]){
	int i;
	
	for (i=0;i<4;i++){
		switch(codEscrever[i]){
			case '0':
				//cor correta posicao errada
				setColor(NORMALCONSOLA); //setcolor a branco
				printf("0");
				break;
			case '1':
				//cor correta posicao correta
				setColor(VERMELHO); //setcolor a branco
				printf("0");
				break;
			default:
				//caso nao haja mais nada correto escreve o espaço que tem guardado
				printf("%c",codEscrever[i]);
				break;
		}
	}
	
	setColor(NORMALCONSOLA); //voltar a colocar a cor normal
}


//esta funcao vai mostrar todas as tentativas anteriores e o resultado/detalhes dessas tentativas
void desenhar_quadro(){
	int i=0;
	printf("PALPITES \t DETALHES\n");
	
	for(i=0;i<tentativasFeitas;i++){
		escreve_codigo(palpitesFeitos[i]);
		printf("\t\t  ");
		escreve_dicas(palpitesAcertos[i]);
		printf("\n");
	}
	printf("--------------------------\n");
}

//esta funcao vai verificar se os palpites sao validos
int tentativa_valida(char tentativa[4]){
	int i=0;
	
	for (i=0;i<4;i++){
		if(tentativa[i]<48 || tentativa[i]>53){
			printf("Tentativa invalida. Os 4 digitos do seu codigo devem estar entre 0 e 5, inclusive. Tente novamente!\n");
			return 0;
		}			
	}
	
	//caso não tenha disparado a funcao antes então está tudo bem e retornamos 1
	return 1;
}

//vai colocar a informação do acerto na variavel palpitesAcertos da tentativa atual na posicao acertos que sera incrementada aqui
void verifica_palpite(char palpite[4]){
	int i,j, acertos=0,codigovisto[4]={0,0,0,0},palpitevisto[4]={0,0,0,0}; //ignorar verificação nos codigovisto e palpitevisto que foram confirmados
	
	//verificacao de cor com posicao igual
	for (i=0;i<4;i++){
		if (codigo[i] == palpite[i]){
			//marcar como posicao e cor certas
			palpitesAcertos[tentativasFeitas][acertos] = '1';
			//marcar posicao do codigo e palpite vistas e confirmadas para nao voltar a verificar no ciclo seguinte
			codigovisto[i] = palpitevisto[i] = 1;
			acertos++;
			continue;
		}
	}
	
	//verificacao de cor sem posicao igual primeiro
	for (i=0;i<4;i++){
		if(codigovisto[i]!=1){
			for(j=0; j<4 ; j++){
				if (palpitevisto[j]!=1 && codigo[i]==palpite[j]){
					//marcar palpite como visto para que seja ignorado no if dos proximos ciclos
					palpitevisto[j] == 1;
					//marcar como cor certa e posicao errada
					palpitesAcertos[tentativasFeitas][acertos] = '0';
					acertos++;
					break;
				}
			}
		}
	}
	
	//preenchimento as restantes posicoes dos palpitesAcertos nao precisam de mais nada porque ja foram inicializadas com um espaco
}

//esta funcao le qual o palpite do utilizador
void tentativa() {
	char palpite[4];
	
	//vamos mostrar a legenda dos detalhes dos palpites anteriores antes de pedir o palpite caso ele ja tenha feito algum
	if (tentativasFeitas>0){
		printf("Legenda dos detalhes: \n");
		setColor(VERMELHO);
		printf("0 ");
		setColor(NORMALCONSOLA);
		printf("- Introduziu uma cor correta numa posicao correta\n");
		printf("0 - Introduziu uma cor correta numa posicao incorreta\n\n");
	}
	//vamos pedir um palpite enquanto este não estiver no intervalo correto (digitos de 0 a 5)
	do{
		printf("Qual o seu palpite? ");
		
		fflush(stdin);
		scanf(" %s", &palpite);
	}while(!tentativa_valida(palpite));
	
	//apos ler o palpite chamamos a funcao verifica_palpite para verificar se este palpite tem posicoes corretas ou cores corretas
	verifica_palpite(palpite);
	
	//guardamos o palpite numa string e incrementamos o numero de tentativas feitas
	strcpy(palpitesFeitos[tentativasFeitas],palpite);
	
	tentativasFeitas++;
}

//esta funcao verifica se o utilizador excedeu as 10 tentativas
int acabou() {
	return tentativasFeitas >= 10;  //so retorna 1 se tiver tentado 10 ou mais vezes. Caso contrario retorna 0, porque a condicao nao se verifica
}

//se as strings forem exatamente iguais retorna 1 se nao, retorna 0
int ganhou() {
	int i=0;
	
	for (i=0;i<tentativasFeitas;i++){
		//verifica as strings
		if (strcmp(codigo, palpitesFeitos[i]) == 0){
			return 1;
		}	
	}
	
	//se ainda nao disparou uma igualdade no for é porque não houve uma igualdade
	return 0;
}

int main() {
	//para randomizar o codigo gerado de forma aleatória
	srand(time(NULL));
	
	int opcao;
	//do while para que o menu se repita por todo o programa
    do {
        system("cls");
        opcao = menu(); //inteiro de 1 a 5 para que no switch digamos a que parte do menu corresponde
        //então chamamos o menu para o mostrar ao utilizador e quando este seleciona a opção ela é retornada pela função para aqui

        switch (opcao) {
            case 1:
            	system("cls");
            	inicializar_tentativas();
				gerador_codigo();
				
				//chamamos a função desenhar_quadro e vamos comecar a ler tentativas do utilizador
                do {
					system("cls");
					desenhar_quadro();
					tentativa();
				} while (!ganhou() && !acabou());
				system("cls");
				
				//desenhar o ultimo quadro para que fique visivel com o ecra de vitoria/derrota
				desenhar_quadro();
				
				if(ganhou()) {
					printf("\nParabens, derrotou o COMPUTADOR!\n");
					printf("Adivinhou o codigo: ");
					escreve_codigo(codigo);
				} else {
					printf("\nExcedeu as 10 tentativas, foi derrotado!\n");
					printf("O codigo secreto era: ");
					escreve_codigo(codigo);
				}
                break;
            case 2:
                printf("A sair...\n");
                //return dentro do main para o programa finalizar
                system("pause");
                return 0;
                break;
            default:
                //caso o utilizador insira uma opção para o qual o programa nao está preparado
                printf("Opcao invalida!\n");
                break;
        }
        printf("\n");
        system("pause");
    } while (1);
}
