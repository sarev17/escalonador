#include<stdlib.h>
#include<windows.h>
#include <stdio.h>
#include <conio.h>
#include <math.h>


int const linhaTempo=20;


struct lista {
 int periodo;
 int ciclo;
 int deadline;
 struct lista* prox;
};
typedef struct lista Lista;





/* função de inicialização: retorna uma lista vazia */
Lista* inicializa (void)
{
 return NULL;
}





/* inserção no início: retorna a lista atualizada */
Lista* insere (Lista* l, int p, int c,int d )
{
 Lista* novo = (Lista*) malloc(sizeof(Lista));
 novo->ciclo = c;
 novo->deadline = d;
 novo->periodo = p;
 novo->prox=l;

 return novo;
}

int mmc(int num1, int num2) {

    int resto, a, b;

    a = num1;
    b = num2;

    do {
        resto = a % b;

        a = b;
        b = resto;

    } while (resto != 0);

    return ( num1 * num2) / a;
}


/* função imprime: imprime valores dos elementos na struct */
void imprime (Lista* l)
{
 Lista* p; /* variável auxiliar para percorrer a lista */
 for (p = l; p != NULL; p = p->prox)
 printf("P=%d C=%d D=%d\n", p->periodo,p->ciclo,p->deadline);
}

//função para desenhar carcteres na tela em posições especificas
 
void desenhar(int x,int y)
{
	
    COORD p={x,y};
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),p);
}


int testeEDF(int p[10],int c[10], int tarefas){
	
	int i;
	float u=0.0;
	
	for(i=0;i<tarefas;i++){
		u+=c[i]/(float)p[i];
	}
	
	if(u<=1){
		printf("\n Utilizacao total %f <= 1 O sistema eh escalonavel\n",u);
		return 1;
	}else{
		printf("\n Utilizacao total %f > 1 O sistema nao eh escalonavel\n",u);
		return 0;
	}
	
}

void escalonarEDF(int dl[10],int cc[10],int tarefas,int ciclo)
{
	
	int t1,t2,t3,i,j,menor,aux=0,indice=0;
	char pintar[3][140]; // sistema so suporta 3 tarefas e 140 periodos
	int dlAux[10],ccAux[10];
	
	//setando '_' no vetor dos resultados;
	
	for(i=0;i<3;i++)
		for(j=0;j<140;j++)
			pintar[i][j]='_';	
	
	
	//preenchendo os vetores auxiliares
	for(i=0;i<10;i++){
		dlAux[i] = dl[i];
		ccAux[i]=cc[i];
	}


	//ciclo principal

		//limite do tamanho da tela ( impressão legivel)
	if(ciclo>139)
		ciclo=139;

	for(i=0;i<ciclo;i++){


	//testando se é preciso renovar os ciclos
	for(aux=0;aux<tarefas;aux++){
		if(ccAux[aux]==0)
			if(i==(dlAux[aux]-dl[aux]))
				ccAux[aux]=cc[aux];			
		
	}
	
		// pegando indice do menor dlAux com ccAux != 0		
		indice=0;
		for(aux=0;aux<tarefas;aux++){
			if(ccAux[indice]==0){ //encerrou tempo de computação
				indice++;
				continue;
			}
			if(dlAux[indice]>dlAux[aux])
				if(ccAux[aux]!=0)
					indice=	aux;
					
		}
		
		
		
			//tarefa selecionada
			if(ccAux[indice]!=0 && indice<tarefas){
				pintar[indice][i] = 178;
				//diminuindo um ciclo
				ccAux[indice]-=1;
				//teste se completou os ciclcos para o deadline
				if(ccAux[indice]==0)
					dlAux[indice]+=dl[indice];
				}
		
										
	}

	
	
	
	
	//imprimindo grafico
	int auxx=3,auxy=17,auxx2=3,cont=1; //posição que começa a imprimir o grafico
	for(i=0;i<tarefas;i++){
		desenhar(0,auxy);	
		printf("T%d ",i);
		for(j=0;j<ciclo;j++){		
			if(auxx<=72){
				desenhar(auxx,auxy);
				printf("%c",pintar[i][j]);
				auxx++;
			}else{
				desenhar(0,auxy+5);	
				printf("T%d ",i);
				desenhar(auxx2,auxy+5);
				printf("%c",pintar[i][j]);
				auxx2++;
				auxx++;
			}	
		}
		printf("\n\n");
		auxx=3;
		auxx2=3;
		auxy++;
		printf("\n%d",dl[i]);
	}
	
	//linha temporal
	
	tarefas=linhaTempo; //aproveitada para setar posição da linha temporal
	int indice2=3;
	j=1;indice=3,aux=1;
	for(i=0;i<ciclo;i++){
		
		if(indice<=72){
			if(j>9){ //preencher as dezenas 
				j=0;
				desenhar(indice,tarefas);
				printf("%d",aux);
				desenhar(indice,tarefas+1);
				printf("0");
				j++;aux++;indice++;continue;
		}
			desenhar(indice,tarefas);
			printf("%d",j);
			j++;
			indice++;	
		}else{
			if(j>9){ //preencher as dezenas 
			j=0;
			desenhar(indice2,tarefas+5);
			printf("%d",aux);
			desenhar(indice2,tarefas+6);
			printf("0");
			j++;aux++;indice2++;continue;
		}	
			desenhar(indice2,tarefas+5);
			printf("%d",j);
			j++;
			indice2++;
		}
	}

	printf("\n\n");
	
			
		
}
		
int testeRM(int p[10],int c[10], int tarefas){
	
	int i;
	float u=0.0,res;
	
	for(i=0;i<tarefas;i++){
		u+=c[i]/(float)p[i];
	}
	
	
	res = ((float)tarefas)*(pow(2,1/(float)tarefas) -1);
	
	if(u<=res){
		printf("\n Utilizacao total %f <= %f O sistema eh escalonavel\n",u,res);
			return 1;
	}else{
		printf("\n Utilizacao total %f > %f O sistema pode nao ser escalonavel\n",u,res);
		return 0;
	}
	
}

void escalonarRM(int pp[10],int cc[10],int tarefas,int ciclo)
{
	
	int i,j,menor,aux=0,aux2=0,indice=0;
	char pintar[3][140];
	int ppAux[10],ccAux[10];
	
	
	//setando '_' no vetor dos resultados;
	
	for(i=0;i<3;i++)
		for(j=0;j<140;j++)
			pintar[i][j]='_';	
	
	
	
	//ordenando os vetores pelo P menor
	
	for(i=0;i<tarefas-1;i++)
		for(j=1;j<tarefas;j++){
			aux=pp[i];
			aux2=cc[i];
			if(pp[i]>pp[j]){
							
				
				
				pp[i]=pp[j];
				pp[j]=aux;
				
				cc[i]=cc[j];
				cc[j]=aux2;
			}
	}
			
	//preenchendo os vetores auxiliares
	for(i=0;i<10;i++){
		ppAux[i] = pp[i];
		ccAux[i]=cc[i];		
	}
	
	//ciclo principal

		//limite do tamanho da tela ( impressão legivel)
	if(ciclo>139)
		ciclo=139;

	indice=0;


	for(i=0;i<ciclo;i++){
		
		//printf("\n i=%d j=%d ppaux=%d ccaux=%d i=%d>=%d",i,j,ppAux[j],ccAux[j],i,(ppAux[j]-pp[j]));
		for(j=0;j<tarefas;j++){
				//printf("\n i=%d j=%d ppaux=%d ccaux=%d",i,j,ppAux[j],ccAux[j]);
			if(ccAux[j]==0){
			
				//printf("\n tarefa:%d i=%d j=%d ppaux=%d ccaux=%d",j,i,j,ppAux[j],ccAux[j]);
				if(i>=(ppAux[j]-pp[j])){ //testa se esta no proximo deadline
				
					ccAux[j]=cc[j];		// restaura os ciclos
					break;
				}
				continue;	
			}
			break;					
		}
			//printf("\nTarefa escolhida:%d ccAux=%d",j,ccAux[j]);
		
		if(ccAux[j]!=0 && j<tarefas){
			//printf("\nT=%d pintar[%d][%d] c = %d",i,j,i,ccAux[j]);	
			pintar[j][i]=178;
			ccAux[j]--;
			if(ccAux[j]==0){
				ppAux[j]+=pp[j];
			}								
		}	
	
	}
	
	
	


	
	
	///*
	
	
	//imprimindo grafico
	int auxx=3,auxy=17,auxx2=3,cont=1; //posição que começa a imprimir o grafico
	for(i=0;i<tarefas;i++){
		desenhar(0,auxy);	
		printf("T%d ",i);
		for(j=0;j<ciclo;j++){		
			if(auxx<=72){
				desenhar(auxx,auxy);
				printf("%c",pintar[i][j]);
				auxx++;
			}else{
				desenhar(0,auxy+5);	
				printf("T%d ",i);
				desenhar(auxx2,auxy+5);
				printf("%c",pintar[i][j]);
				auxx2++;
				auxx++;
			}
			
		
			
		}
		printf("\n");
		auxx=3;
		auxx2=3;
		auxy++;
	}
	
	//linha temporal
	
	tarefas=linhaTempo; //aproveitada para setar posição da linha temporal
	int indice2=3;
	j=1;indice=3,aux=1;
	for(i=0;i<ciclo;i++){
		
		if(indice<=72){
			if(j>9){ //preencher as dezenas 
				j=0;
				desenhar(indice,tarefas);
				printf("%d",aux);
				desenhar(indice,tarefas+1);
				printf("0");
				j++;aux++;indice++;continue;
		}
			desenhar(indice,tarefas);
			printf("%d",j);
			j++;
			indice++;	
		}else{
			if(j>9){ //preencher as dezenas 
			j=0;
			desenhar(indice2,tarefas+5);
			printf("%d",aux);
			desenhar(indice2,tarefas+6);
			printf("0");
			j++;aux++;indice2++;continue;
		}	
			desenhar(indice2,tarefas+5);
			printf("%d",j);
			j++;
			indice2++;
		}
	}

	printf("\n\n");
	printf("OBS: as tarefas foram reorganizadas pela ordem crescente de seus periodos ;) ");
	
	//*/
			
		
}

int testeDM(int p[10],int c[10], int tarefas){
	
	int i;
	float u=0.0,res;
	
	for(i=0;i<tarefas;i++){
		u+=c[i]/(float)p[i];
	}
	
	
	res = ((float)tarefas)*(pow(2,1/(float)tarefas) -1);
	
	if(u<=res){
		printf("\n Utilizacao total %f <= %f O sistema eh escalonavel\n",u,res);
			return 1;
	}else{
		printf("\n Utilizacao total %f > %f O sistema pode nao ser escalonavel\n",u,res);
		return 0;
	}
	
}

void escalonarDM(int pp[10],int cc[10],int tarefas,int ciclo)
{
	
	int t1,t2,t3,i,j,menor,aux=0,aux2=0,indice=0;
	char pintar[3][140];
	int ppAux[10],ccAux[10];
	
	//setando '_' no vetor dos resultados;
	
	for(i=0;i<3;i++)
		for(j=0;j<140;j++)
			pintar[i][j]='_';	
	
	//ordenando os vetores pelo P menor
	
	for(i=0;i<tarefas-1;i++)
		for(j=1;j<tarefas;j++){
			aux=pp[i];
			aux2=cc[i];
			if(pp[i]>pp[j]){
				
				pp[i]=pp[j];
				pp[j]=aux;
				
				cc[i]=cc[j];
				cc[j]=aux2;
			}
	}
			
	//preenchendo os vetores auxiliares
	for(i=0;i<10;i++){
		ppAux[i] = pp[i];
		ccAux[i]=cc[i];		
	}
	
	//ciclo principal
		//limite do tamanho da tela ( impressão legivel)
	if(ciclo>139)
		ciclo=139;

	indice=0;


	for(i=0;i<ciclo;i++){
		
		j=0;
		for(j=0;j<tarefas;j++){
				//printf("\n i=%d j=%d ppaux=%d ccaux=%d",i,j,ppAux[j],ccAux[j]);
			if(ccAux[j]==0){
				if(i>=(ppAux[j]-pp[j])){
					ccAux[j]=cc[j];
					break;
				}
				continue;	
			}
			break;					
		}
			
		
		//printf("\npintar[%d][%d]",j,i);
		
		if(ccAux[j]!=0 && j<tarefas){
			//printf("\nT=%d pintar[%d][%d] c = %d",i,j,i,ccAux[j]);	
			pintar[j][i]=178;
			ccAux[j]--;
			if(ccAux[j]==0){
				ppAux[j]+=pp[j];
			}								
		}								
	}

	
	
///*
	
	
	//imprimindo grafico
	int auxx=3,auxy=17,auxx2=3,cont=1; //posição que começa a imprimir o grafico
	for(i=0;i<tarefas;i++){
		desenhar(0,auxy);	
		printf("T%d ",i);
		for(j=0;j<ciclo;j++){		
			if(auxx<=72){
				desenhar(auxx,auxy);
				printf("%c",pintar[i][j]);
				auxx++;
			}else{
				desenhar(0,auxy+5);	
				printf("T%d ",i);
				desenhar(auxx2,auxy+5);
				printf("%c",pintar[i][j]);
				auxx2++;
				auxx++;
			}
			
		
			
		}
		printf("\n");
		auxx=3;
		auxx2=3;
		auxy++;
	}
	
	//linha temporal
	
	tarefas=linhaTempo; //aproveitada para setar posição da linha temporal
	int indice2=3;
	j=1;indice=3,aux=1;
	for(i=0;i<ciclo;i++){
		
		if(indice<=72){
			if(j>9){ //preencher as dezenas 
				j=0;
				desenhar(indice,tarefas);
				printf("%d",aux);
				desenhar(indice,tarefas+1);
				printf("0");
				j++;aux++;indice++;continue;
		}
			desenhar(indice,tarefas);
			printf("%d",j);
			j++;
			indice++;	
		}else{
			if(j>9){ //preencher as dezenas 
			j=0;
			desenhar(indice2,tarefas+5);
			printf("%d",aux);
			desenhar(indice2,tarefas+6);
			printf("0");
			j++;aux++;indice2++;continue;
		}	
			desenhar(indice2,tarefas+5);
			printf("%d",j);
			j++;
			indice2++;
		}
	}

	printf("\n\n");
	printf("OBS: as tarefas foram reorganizadas pela ordem crescente de seus DL absolutos ;) ");
	
	//*/
		
			
		
}
	
Lista* LerArquivo(char narq[])
{

 
 
 
 char p[2],c[2],d[2];


int t,i=0,controle;

 
 FILE *arq; 

 
 arq = fopen(narq, "r");
 
 		Lista* l; /* declara uma lista não inicializada */
	 	l = inicializa(); /* inicializa lista como vazia */

 
 
 if(arq== NULL)
   printf("Erro, nao foi possivel abrir o arquivo\n");
 else
 
 
	//inserindo dados do arquivo em uma struct
	

   
  while((fscanf(arq,"%s\t%s\t%s\n", &p, &c, &d))!=EOF ){
  	
  	controle = atoi(p); // testa se a linha é de numero ou texto
	   
	   if(controle!=0){
	   		l = insere(l,atoi(p),atoi(c),atoi(d));	
	   }
  }
  	
	   	
   	
 fclose(arq);



 //retorna a struct com os dados
 return l;
 
 
 
		
}

int main(){
	int x;
	//Aviso
	printf("\n\n\n\n       Coloque os arquivos na pasta do executavel.\n");
	printf("       Fazendo isso basta digitar o nome do arquivo + a extensao .txt\n");
	printf("       Exemplo 'sistema1.txt'\n");
	printf("       Para continuar tecle 's' ");
	desenhar(4,2);
	printf("********************************************************************");
	desenhar(4,8);
	printf("********************************************************************");
	
	for(x=2;x<9;x++){
		desenhar(3,x);
		printf("*");
		desenhar(71,x);
		printf("*");
	}
	
	
	switch(getch()){
		case 's':{
			system("cls");
			break;
		}
		default: return 0;
	}
		

while(1){	
	
	printf("nome do arquivo:\n");
	char arq[50],escolha;
	
	
	
	//entrar com o nome do arquivo
	scanf("%s",&arq);
	
				
	Lista* l; /* declara uma lista não inicializada */
	l = inicializa(); /* inicializa lista como vazia */
	
	//obter dados do arquivo
	l = LerArquivo(arq);	
	
	if(l!=NULL){
	
	system("cls");
		
	// organizando dados
	
		Lista* p;	
	int deadlines[10]={0,0,0,0,0,0,0,0,0,0,}, ciclos[10]={0,0,0,0,0,0,0,0,0,0},periodos[10]={0,0,0,0,0,0,0,0,0,0};
	int t = 0, c = 0,cicloMaior=1,i=0,aux,pri;
	
	for (p = l; p != NULL; p = p->prox){
		cicloMaior= mmc(cicloMaior,p->deadline); // mmc dos deadlines
		t++; // quantidade de tarefas	
		deadlines[i]=p->deadline;
		ciclos[i] = p->ciclo;
		periodos[i] = p->periodo;
		i++;
	}
	
	//organizando vetores
	
	for(i=0;i<t-1;i++){
		
			aux=deadlines[i];
			deadlines[i]=deadlines[t-(i+1)];
			deadlines[t-(i+1)]= aux;
			
			aux=ciclos[i];
			ciclos[i]=ciclos[t-(i+1)];
			ciclos[t-(i+1)]=aux;
			
			aux=periodos[i];
			periodos[i]=periodos[t-(i+1)];
			periodos[t-(i+1)]=aux;
				
	}
	
	printf("\n    %s",arq);
	printf("\n********************");
	printf("\nP	C	D\n");
	for(i=0;i<t;i++){
		printf("%d	%d	%d\n",periodos[i],ciclos[i],deadlines[i]);
	}
	printf("********************\n",arq);
	
	
	printf("\nEscolha o modo de escalonamento\n(E) EDF\n(R) RM\n(D) DM\n(S) SAIR\n\n");
	
	switch(getch()){
		case 'e':{
			
			testeEDF(periodos,ciclos,t);
			escalonarEDF(periodos,ciclos,t,cicloMaior);
			printf("\n\n");
			break;
		}
		
		case 'r':{
			testeRM(periodos,ciclos,t);
			escalonarRM(periodos,ciclos,t,cicloMaior);
			printf("\n\n");
			break;
		}
		
		case'd':{
			testeDM(deadlines,ciclos,t);
			escalonarDM(deadlines,ciclos,t,cicloMaior);
			break;
		}
		
		case 's':{
			return 0;
		}
		
		default:{
			printf("\n\nEntrada invalida!");
			break;
		}			
	}
	
	
	if(cicloMaior>139)
		printf("\n\nO sistema possui %d ciclos, serão impressos apenas 140\n",cicloMaior);
	
	}
	
	
	printf("\n\nDigite 'S' para fazer outro teste ou outra tecla para sair\n\n");
	
	switch(getch())
		case 's':{
			system("cls");
			continue;
			break;
		}
	
		
	system("pause");
	system("cls");
	
	return 0;
	exit(1);	
}
	
	
	
 	
 	return 0;
 	exit(1);
	
}
	
	



