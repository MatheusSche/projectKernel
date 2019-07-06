#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include "driver.h"

#define DEVICE_NAME "/dev/so"

//"01212M-p-p-p-pp-p-p-p--p-p-p-p----------------b-b-b-b--b-b-b-bb-b-b-b-0"
char code[100];
char sendCode[100];
char originalCode[100] = "01212M-p-p-p-pp-p-p-p--p-p-p-p----------------b-b-b-b--b-b-b-bb-b-b-b-0";
int fp, rv, ret;
/*
char matriz[10][10] = {
 {' ', '1', '2', '3', '4', '5', '6', '7', '8', ' '},
 {'1', '-', 'p', '-', 'p', '-', 'p', '-', 'p', '1'},
 {'2', 'p', '-', 'p', '-', 'p', '-', 'p', '-', '2'},
 {'3', '-', 'p', '-', 'p', '-', 'p', '-', 'p', '3'},
 {'4', '-', '-', '-', '-', '-', '-', '-', '-', '4'},
 {'5', '-', '-', '-', '-', '-', '-', '-', '-', '5'},
 {'6', 'b', '-', 'b', '-', 'b', '-', 'b', '-', '6'},
 {'7', '-', 'b', '-', 'b', '-', 'b', '-', 'b', '7'},
 {'8', 'b', '-', 'b', '-', 'b', '-', 'b', '-', '8'},
 {' ', '1', '2', '3', '4', '5', '6', '7', '8', ' '}
 };
 */
char matriz[10][10] = {
 {' ', '1', '2', '3', '4', '5', '6', '7', '8', ' '},
 {'1', '-', '-', '-', '-', '-', '-', '-', '-', '1'},
 {'2', '-', '-', '-', '-', '-', '-', '-', '-', '2'},
 {'3', '-', '-', '-', '-', '-', '-', '-', '-', '3'},
 {'4', '-', '-', '-', '-', '-', '-', '-', '-', '4'},
 {'5', '-', '-', '-', '-', '-', '-', '-', '-', '5'},
 {'6', '-', '-', '-', '-', '-', '-', '-', '-', '6'},
 {'7', '-', '-', '-', '-', '-', '-', '-', '-', '7'},
 {'8', '-', '-', '-', '-', '-', '-', '-', '-', '8'},
 {' ', '1', '2', '3', '4', '5', '6', '7', '8', ' '}
};
char tabuleiro[10][10] = {
 {'-','-','-','-','-','-','-','-','-','-'},
 {'-','c','e','c','e','c','e','c','e','-'},
 {'-','e','c','e','c','e','c','e','c','-'},
 {'-','c','e','c','e','c','e','c','e','-'},
 {'-','e','c','e','c','e','c','e','c','-'}, 
 {'-','c','e','c','e','c','e','c','e','-'},
 {'-','e','c','e','c','e','c','e','c','-'}, 
 {'-','c','e','c','e','c','e','c','e','-'},
 {'-','e','c','e','c','e','c','e','c','-'},
 {'-','-','-','-','-','-','-','-','-','-'} 
};
 
int x, y;
int linhaOrigem, colunaOrigem, linhaDestino, colunaDestino;
int player1 = 1, player2 = 2, playerTime = 0, qtdP1=12, qtdP2=12;
int euSou = 0;
char vitoria = '0';


void montaString(){
    //para pegar o placar
    char p1[2], p2[2];
    //monta a parte da string que diz qual jogador eu sou
    sendCode[0] = playerTime+48;
    printf("Eu sou %s\n", sendCode);

    //monta placar da branca
    sprintf(p1,"%d",qtdP1);
    printf("%s\n", p1);
    //concatena a string7
    if(qtdP1>=10){
        sendCode[1] = p1[0];
        sendCode[2] = p1[1];
    } else if(qtdP1<=9) {
        sendCode[1] = '0';
        sendCode[2] = p1[0];
    }
    
    //monta placar da branca
    sprintf(p2,"%d",qtdP2);
    
    //concatena a st
    if(qtdP2>=10){
        sendCode[3] = p2[0];
        sendCode[4] = p2[1];
    } else if(qtdP2<=9) {
        sendCode[3] = '0';
        sendCode[4] = p2[0];
    }
    

    printf("SendCode: [%s]\n", sendCode);

    sendCode[5] = 'M';
    int k = 6;
    //MONTA A PARTE DA MATRIZ
    for(int i = 1; i<=8; i++){
        for(int j = 1; j<=8; j++){
            sendCode[k] = matriz[i][j];
            k++;
        }
    }

    if (vitoria=='1')
    {
       sendCode[70] = '1';
    } else {
        if(vitoria=='2'){
            sendCode[70] = '2';
        } else {
            sendCode[70] = '0';
        }
    }
    
    

    printf("[%s]\n", sendCode);

}

void extraiString(){
     //para pegar o placar de peças
     char p1[2], p2[2];

     //pega a parte da string que diz qual jogador eu sou
     //euSou = code[0]-48;
     if((code[0]-48)==0 || (code[0]-48)==2){
         playerTime = 1;
     } else {
         playerTime = 2;
     } 
     printf("Eu sou: [%d]\n", euSou);
     //converte o placar das peças brancas 
     if(code[1]=='0'){
         p1[0] = code[2];
         qtdP1 = p1[0]-48;

     } else {
        p1[0] = code[1];
        p1[1] = code[2];
        qtdP1 = atoi(p1);

     }
     printf("Minhas Peças[%d]\n", qtdP1);
     //coverte o placar de peças pretas
     
     if(code[3]=='0'){
         p2[0] = code[4];
         qtdP2 = p2[0]-48;
     } else {
        p2[0] = code[3];
        p2[1] = code[4];
        qtdP2 = atoi(p2);

     }
     printf("Minhas Peças[%d]\n", qtdP2);
    //comeca a extracao da matriz
    
    int k = 6;
    for(int i=1;i<=8;i++){
         for(int j=1;j<=8;j++){
             matriz[i][j] = code[k];
             k++;
         }
    }
    //verifica a vitoria
    vitoria = (code[70]-48);
    printf("Vitoria [%d]\n", vitoria);
     
}

void exibeMatriz(){
    printf("\n");
    if(playerTime==1){
        printf("Vez do jogador 1\n");
    }
    if(playerTime==2){
        printf("Vez do jogador 2\n\n");
    }
    printf("Peças Brancas: [%d]\n", qtdP1);
    printf("Peças Pretas:  [%d]\n", qtdP2);
    for (x = 0; x <= 9; x++){
        for (y = 0; y <= 9; y++){
            printf("%c ", matriz[x][y]);

             if (y == 9){
                printf("\n");
             }
        }
         y = 0;
    }
 
}

int validaBD(){
 if(tabuleiro[linhaDestino][colunaDestino]=='e'){
    if(matriz[linhaDestino][colunaDestino]=='-'){
    // verifica se a peça deu um pulo
    if(abs((linhaDestino) - linhaOrigem) == 1 && abs((colunaDestino) - colunaOrigem)==1){
    printf("Movimento realizado\n");
    matriz[linhaDestino][colunaDestino] = matriz[linhaOrigem][colunaOrigem];
    matriz[linhaOrigem][colunaOrigem] = '-';
    
    if (linhaDestino==1 && matriz[linhaDestino][colunaDestino] == 'b'){
        printf("vIROU DAMA\n");
        matriz[linhaDestino][colunaDestino] = 'B';
    }
    if (linhaDestino==8 && matriz[linhaDestino][colunaDestino] == 'p'){
        printf("vIROU DAMA\n");
        matriz[linhaDestino][colunaDestino] = 'P';
    }
 
    return 1;
    } else{
        //se deu mais de 1 pulo, verifica se foi dois em caso de captura de peça 
        if(abs((linhaDestino) - linhaOrigem) == 2 && abs((colunaDestino) - colunaOrigem)==2){
            if(playerTime==1){
                if(matriz[linhaDestino-1][colunaDestino-1]=='p' || matriz[linhaDestino-1][colunaDestino-1]=='P'){
                    printf("Capturou o p\n");
                    matriz[linhaDestino-1][colunaDestino-1]='-';
                    matriz[linhaOrigem][colunaOrigem]='-';
                    matriz[linhaDestino][colunaDestino]='b';
                    qtdP2--;

                    if (linhaDestino==1){
                        matriz[linhaDestino][colunaDestino]='B';
                    }

                    return 2;
                } else {
                    //significa que ocorreu um movimento inválido
                    printf("Inválido CD\n");
                    return 0;
                }
             }
            
            if(playerTime==2){
                if(matriz[linhaDestino-1][colunaDestino-1]=='b' || matriz[linhaDestino-1][colunaDestino-1]=='B'){
                    printf("Capturou o b\n");
                    matriz[linhaDestino-1][colunaDestino-1]='-';
                    matriz[linhaOrigem][colunaOrigem]='-';
                    matriz[linhaDestino][colunaDestino]='p';
                     qtdP1--;

                    if (linhaDestino==8){
                        matriz[linhaDestino][colunaDestino]='P';
                    }

                    return 2;
                } else {
                    //significa que ocorreu um movimento inválido
                    printf("Inválido CE\n");
                    return 0;
                }

            }
        } else {
            printf("Pulos d+\n");
            return 0;
            }
    }

 }
 }

 return 0;
}

int validaBE(){
 if(tabuleiro[linhaDestino][colunaDestino]=='e'){
 if(matriz[linhaDestino][colunaDestino]=='-'){
 // verifica se a peça deu um pulo
 if(abs((linhaDestino) - linhaOrigem) == 1 && abs((colunaDestino) - colunaOrigem)==1){
 printf("Movimento realizado\n");
 matriz[linhaDestino][colunaDestino] = matriz[linhaOrigem][colunaOrigem];
 matriz[linhaOrigem][colunaOrigem] = '-';

 if (linhaDestino==1 && matriz[linhaDestino][colunaDestino] == 'b'){
 printf("vIROU DAMA\n");
 matriz[linhaDestino][colunaDestino] = 'B';
 }
 if (linhaDestino==8 && matriz[linhaDestino][colunaDestino] == 'p'){
 printf("vIROU DAMA\n");
 matriz[linhaDestino][colunaDestino] = 'P';
 }

 return 1;
 } else{
 //se deu mais de 1 pulo, verifica se foi dois em caso de captura de peça 
 if(abs((linhaDestino) - linhaOrigem) == 2 && abs((colunaDestino) - colunaOrigem)==2){
 if(playerTime==1){
 if(matriz[linhaDestino-1][colunaDestino+1]=='p' || matriz[linhaDestino-1][colunaDestino+1]=='P'){
 printf("Capturou o p\n");
 matriz[linhaDestino-1][colunaDestino+1]='-';
 matriz[linhaOrigem][colunaOrigem]='-';
 matriz[linhaDestino][colunaDestino]= 'b';
 qtdP2--;

 if (linhaDestino==1){
 matriz[linhaDestino][colunaDestino]='B';
 }

 return 2;
 } else {
 //significa que ocorreu um movimento inválido
 printf("Inválido CD\n");
 return 0;
 }
 }
    if(playerTime==2){
        if(matriz[linhaDestino-1][colunaDestino+1]=='b' || matriz[linhaDestino-1][colunaDestino+1]=='B'){
            printf("Capturou o b\n");
            matriz[linhaDestino-1][colunaDestino+1]='-';
            matriz[linhaOrigem][colunaOrigem]='-';
            matriz[linhaDestino][colunaDestino]= 'p';
            qtdP1--;
            
            
            if (linhaDestino==8){
            matriz[linhaDestino][colunaDestino]='P';
            }
            
            return 2;
        } else {
            //significa que ocorreu um movimento inválido
            printf("Inválido CE\n");
            return 0;
    }

    }
 } else {
 printf("Pulos d+\n");
 return 0;
 }
 }

 }
 }

 return 0;
}

int validaCE(){
 if(tabuleiro[linhaDestino][colunaDestino]=='e'){
 if(matriz[linhaDestino][colunaDestino]=='-'){
 // verifica se a peça deu um pulo
 if(abs((linhaDestino) - linhaOrigem) == 1 && abs((colunaDestino) - colunaOrigem)==1){
 printf("Movimento realizado\n");
 matriz[linhaDestino][colunaDestino] = matriz[linhaOrigem][colunaOrigem];
 matriz[linhaOrigem][colunaOrigem] = '-';

 if (linhaDestino==1 && matriz[linhaDestino][colunaDestino] == 'b'){
 printf("vIROU DAMA\n");
 matriz[linhaDestino][colunaDestino] = 'B';
 }
 if (linhaDestino==8 && matriz[linhaDestino][colunaDestino] == 'p'){
 printf("vIROU DAMA\n");
 matriz[linhaDestino][colunaDestino] = 'P';
 }

 return 1;
 } else{
 //se deu mais de 1 pulo, verifica se foi dois em caso de captura de peça 
 if(abs((linhaDestino) - linhaOrigem) == 2 && abs((colunaDestino) - colunaOrigem)==2){
 if(playerTime==1){
 if(matriz[linhaDestino+1][colunaDestino+1]=='p' || matriz[linhaDestino+1][colunaDestino+1]=='P'){
 printf("Capturou o p\n");
 matriz[linhaDestino+1][colunaDestino+1]='-';
 matriz[linhaOrigem][colunaOrigem]='-';
 matriz[linhaDestino][colunaDestino]='b';
 qtdP2--;

 if (linhaDestino==1){
 matriz[linhaDestino][colunaDestino]='B';
 }

 return 2;
 } else {
 //significa que ocorreu um movimento inválido
 printf("Inválido CD\n");
 return 0;
 }
 }
 if(playerTime==2){
 if(matriz[linhaDestino+1][colunaDestino+1]=='b' || matriz[linhaDestino+1][colunaDestino+1]=='B'){
 printf("Capturou o b\n");
 matriz[linhaDestino+1][colunaDestino+1]='-';
 matriz[linhaOrigem][colunaOrigem]='-';
 matriz[linhaDestino][colunaDestino]='p';
  qtdP1--;
 
 if (linhaDestino==8){
 matriz[linhaDestino][colunaDestino]='P';
 }
 
 return 2;
 } else {
 //significa que ocorreu um movimento inválido
 printf("Inválido CE\n");
 return 0;
 }

 }
 } else {
 printf("Pulos d+\n");
 return 0;
 }
 }

 }
 }

 return 0;
}


int validaCD(){
 if(tabuleiro[linhaDestino][colunaDestino]=='e'){
 if(matriz[linhaDestino][colunaDestino]=='-'){
 // verifica se a peça deu um pulo
 if(abs((linhaDestino) - linhaOrigem) == 1 && abs((colunaDestino) - colunaOrigem)==1){
 printf("Movimento realizado\n");
 matriz[linhaDestino][colunaDestino] = matriz[linhaOrigem][colunaOrigem];
 matriz[linhaOrigem][colunaOrigem] = '-';

 if (linhaDestino==1 && matriz[linhaDestino][colunaDestino] == 'b'){
 printf("vIROU DAMA\n");
 matriz[linhaDestino][colunaDestino] = 'B';
 }
 if (linhaDestino==8 && matriz[linhaDestino][colunaDestino] == 'p'){
 printf("vIROU DAMA\n");
 matriz[linhaDestino][colunaDestino] = 'P';
 }

 return 1;
 } else{
 //se deu mais de 1 pulo, verifica se foi dois em caso de captura de peça 
 if(abs((linhaDestino) - linhaOrigem) == 2 && abs((colunaDestino) - colunaOrigem)==2){
 if(playerTime==1){
 if(matriz[linhaDestino+1][colunaDestino-1]=='p' || matriz[linhaDestino+1][colunaDestino-1]=='P'){
 printf("Capturou o p\n");
 matriz[linhaDestino+1][colunaDestino-1]='-';
 matriz[linhaOrigem][colunaOrigem]='-';
 matriz[linhaDestino][colunaDestino]='b';
 qtdP2--;

 if (linhaDestino==1){
 matriz[linhaDestino][colunaDestino]='B';
 }
 

 return 2;
 } else {
 //significa que ocorreu um movimento inválido
 printf("Inválido CD\n");
 return 0;
 }
 }
 if(playerTime==2){
 if(matriz[linhaDestino+1][colunaDestino-1]=='b' || matriz[linhaDestino+1][colunaDestino-1]=='B'){
 printf("Capturou o b\n");
 matriz[linhaDestino+1][colunaDestino-1]='-';
 matriz[linhaOrigem][colunaOrigem]='-';
 matriz[linhaDestino][colunaDestino]='p';
 qtdP1--;
 
 if (linhaDestino==8){
 matriz[linhaDestino][colunaDestino]='P';
 }
 
 return 2;
 } else {
 //significa que ocorreu um movimento inválido
 printf("Inválido CD\n");
 return 0;
 }

 }
 } else {
 printf("Pulos d+\n");
 return 0;
 }
 }

 }
 }

 return 0;
}

int validaPecaMov(){
    int sair =0;
    if(matriz[linhaOrigem][colunaOrigem] != 'b'){
            if(matriz[linhaOrigem][colunaOrigem] != 'p' ){
                if(matriz[linhaOrigem][colunaOrigem] != 'B'){
                    if(matriz[linhaOrigem][colunaOrigem] != 'P'){
                        printf("Nao escolheu uma peca valida\n");
                        printf("Valor: [%c]", matriz[linhaOrigem][colunaOrigem]);
                    } else {
                        printf("É um P\n");
                        sair = 1;
                    }
                } else {
                    printf("É um B\n");
                    sair = 1;
                }
            } else {
                printf("É um p\n");
                sair = 1;
            }
        
        } else {
            printf("É um b\n");
            sair = 1;
        }

        return 1;

}

int verificaTerminoTurno(){
    if(playerTime==1){
        if((matriz[linhaDestino-1][colunaDestino-1]=='p' && matriz[linhaDestino-2][colunaDestino-2]=='-') || (matriz[linhaDestino-1][colunaDestino-1]=='P' && matriz[linhaDestino-2][colunaDestino-2]=='-')){
            //possível capturar para cima e para esquerda
            return 0;
        } else {
            if((matriz[linhaDestino-1][colunaDestino+1]=='p' && matriz[linhaDestino-2][colunaDestino+2]=='-') || (matriz[linhaDestino-1][colunaDestino+1]=='P' && matriz[linhaDestino-2][colunaDestino+2]=='-')){
                //possível capturar para cima e para direita
                return 0;
            } else {
                if((matriz[linhaDestino+1][colunaDestino-1]=='p' && matriz[linhaDestino+2][colunaDestino-2]=='-') || (matriz[linhaDestino+1][colunaDestino-1]=='P' && matriz[linhaDestino+2][colunaDestino-2]=='-')){
                    //possível capturar para cima e para esquerda
                    return 0;
                } else {
                    if((matriz[linhaDestino+1][colunaDestino+1]=='p' && matriz[linhaDestino+2][colunaDestino+2]=='-') || (matriz[linhaDestino+1][colunaDestino+1]=='P' && matriz[linhaDestino+2][colunaDestino+2]=='-')){
                        //possível capturar para cima e para esquerda
                        return 0;
                    } 
                }
            }
        }
    }
    
    if(playerTime==2){
        if((matriz[linhaDestino-1][colunaDestino-1]=='b' && matriz[linhaDestino-2][colunaDestino-2]=='-') || (matriz[linhaDestino-1][colunaDestino-1]=='B' && matriz[linhaDestino-2][colunaDestino-2]=='-')){
            //possível capturar para cima e para esquerda
            return 0;
        } else {
            if((matriz[linhaDestino-1][colunaDestino+1]=='b' && matriz[linhaDestino-2][colunaDestino+2]=='-') || (matriz[linhaDestino-1][colunaDestino+1]=='B' && matriz[linhaDestino-2][colunaDestino+2]=='-')){
                //possível capturar para cima e para direita
                return 0;
            } else {
                if((matriz[linhaDestino+1][colunaDestino-1]=='b' && matriz[linhaDestino+2][colunaDestino-2]=='-') || (matriz[linhaDestino+1][colunaDestino-1]=='B' && matriz[linhaDestino+2][colunaDestino-2]=='-')){
                    //possível capturar para cima e para esquerda
                    return 0;
                } else {
                    if((matriz[linhaDestino+1][colunaDestino+1]=='b' && matriz[linhaDestino+2][colunaDestino+2]=='-') || (matriz[linhaDestino+1][colunaDestino+1]=='B' && matriz[linhaDestino+2][colunaDestino+2]=='-')){
                        //possível capturar para cima e para esquerda
                        return 0;
                    } 
                }
            }
        }
    }

    return 1;
    
}

int validaBD_DAMA(){
    int nPulosLinha=0, nPulosColuna;
    int capturou = 0;
    nPulosLinha = abs(linhaDestino - linhaOrigem);
    nPulosColuna = abs(colunaDestino - colunaOrigem);

    if (nPulosColuna==nPulosLinha){
        if(playerTime==1){
            if(linhaOrigem+nPulosLinha==linhaDestino && colunaOrigem+nPulosColuna==colunaDestino){
                if(matriz[linhaDestino][colunaDestino]=='-'){
                    for (int i=1; i<=nPulosLinha;i++){
                        if(matriz[linhaOrigem+i][colunaOrigem+i]=='-'){
                            printf("ok");
                        }
                        if(matriz[linhaOrigem+i][colunaOrigem+i]=='b' || matriz[linhaOrigem+i][colunaOrigem+i]=='B'){
                            printf("NAO EH VALIDO");
                            return 0;
                        }
                        if(matriz[linhaOrigem+i][colunaOrigem+i]=='P' || matriz[linhaOrigem+i][colunaOrigem+i]=='p'){
                            printf("Capturou uma peca");
                            //faz as rotinas de tirar a peca do tabuleiro, substituir por - 
                            //diminui o numero de peças do adversario
                            //continua
                            //vai ter que retornar para verificar se tem que jogar de novo
                            matriz[linhaOrigem+i][colunaOrigem+i]='-';
                            qtdP2--;
                            capturou = 1;
                        }
                    }
                    matriz[linhaDestino][colunaDestino]=matriz[linhaOrigem][colunaOrigem];
                    matriz[linhaOrigem][colunaOrigem] = '-';
                }
            }
        }
        if(playerTime==2){
            if(linhaOrigem+nPulosLinha==linhaDestino && colunaOrigem+nPulosColuna==colunaDestino){
                if(matriz[linhaDestino][colunaDestino]=='-'){
                    for (int i=1; i<=nPulosLinha;i++){
                        if(matriz[linhaOrigem+i][colunaOrigem+i]=='-'){
                            printf("ok");
                        }
                        if(matriz[linhaOrigem+i][colunaOrigem+i]=='p' || matriz[linhaOrigem+i][colunaOrigem+i]=='P'){
                            printf("NAO EH VALIDO");
                            //retorna de cara pois jogada nao eh valida
                            return 0 ;
                        }
                        if(matriz[linhaOrigem+i][colunaOrigem+i]=='b' || matriz[linhaOrigem+i][colunaOrigem+i]=='B'){
                            printf("Capturou uma peca");
                            //faz as rotinas de tirar a peca do tabuleiro, substituir por - 
                            //diminui o numero de peças do adversario
                            //continua
                            //vai ter que retornar para verificar se tem que jogar de novo
                            matriz[linhaOrigem+i][colunaOrigem+i]='-';
                            qtdP1--;
                            capturou = 1;
                        }
                    }
                    matriz[linhaDestino][colunaDestino]=matriz[linhaOrigem][colunaOrigem];
                    matriz[linhaOrigem][colunaOrigem] = '-';
                }
            }

        }
    } else{
        printf("Nao confere a posicao\n");
        return 0 ;
    }

    if (capturou==0){
        return 1;
    } else {
        return 2;
    }
}


int validaBE_DAMA(){
    int nPulosLinha=0, nPulosColuna;
    int capturou = 0;
    nPulosLinha = abs(linhaDestino - linhaOrigem);
    nPulosColuna = abs(colunaDestino - colunaOrigem);

    if (nPulosColuna==nPulosLinha){
        if(playerTime==1){
            if(linhaOrigem+nPulosLinha==linhaDestino && colunaOrigem-nPulosColuna==colunaDestino){
                if(matriz[linhaDestino][colunaDestino]=='-'){
                    for (int i=1; i<=nPulosLinha;i++){
                        if(matriz[linhaOrigem+i][colunaOrigem-i]=='-'){
                            printf("ok");
                        }
                        if(matriz[linhaOrigem+i][colunaOrigem-i]=='b' || matriz[linhaOrigem+i][colunaOrigem-i]=='B'){
                            printf("NAO EH VALIDO");
                            return 0;
                        }
                        if(matriz[linhaOrigem+i][colunaOrigem-i]=='P' || matriz[linhaOrigem+i][colunaOrigem-i]=='p'){
                            printf("Capturou uma peca");
                            //faz as rotinas de tirar a peca do tabuleiro, substituir por - 
                            //diminui o numero de peças do adversario
                            //continua
                            //vai ter que retornar para verificar se tem que jogar de novo
                            matriz[linhaOrigem+i][colunaOrigem-i]='-';
                            qtdP2--;
                            capturou = 1;
                        }
                    }
                    matriz[linhaDestino][colunaDestino]=matriz[linhaOrigem][colunaOrigem];
                    matriz[linhaOrigem][colunaOrigem] = '-';
                }
            }
        }
        if(playerTime==2){
            if(linhaOrigem+nPulosLinha==linhaDestino && colunaOrigem-nPulosColuna==colunaDestino){
                if(matriz[linhaDestino][colunaDestino]=='-'){
                    for (int i=1; i<=nPulosLinha;i++){
                        if(matriz[linhaOrigem+i][colunaOrigem-i]=='-'){
                            printf("ok");
                        }
                        if(matriz[linhaOrigem+i][colunaOrigem-i]=='p' || matriz[linhaOrigem+i][colunaOrigem-i]=='P'){
                            printf("NAO EH VALIDO");
                            //retorna de cara pois jogada nao eh valida
                            return 0 ;
                        }
                        if(matriz[linhaOrigem+i][colunaOrigem-i]=='b' || matriz[linhaOrigem+i][colunaOrigem-i]=='B'){
                            printf("Capturou uma peca");
                            //faz as rotinas de tirar a peca do tabuleiro, substituir por - 
                            //diminui o numero de peças do adversario
                            //continua
                            //vai ter que retornar para verificar se tem que jogar de novo
                            matriz[linhaOrigem+i][colunaOrigem-i]='-';
                            qtdP1--;
                            capturou = 1;
                        }
                    }
                    matriz[linhaDestino][colunaDestino]=matriz[linhaOrigem][colunaOrigem];
                    matriz[linhaOrigem][colunaOrigem] = '-';
                }
            }

        }
    } else{
        printf("Nao confere a posicao\n");
        return 0 ;
    }

    if (capturou==0){
        return 1;
    } else {
        return 2;
    }
}

int validaCE_DAMA(){
    int nPulosLinha=0, nPulosColuna;
    int capturou = 0;
    nPulosLinha = abs(linhaDestino - linhaOrigem);
    nPulosColuna = abs(colunaDestino - colunaOrigem);

    if (nPulosColuna==nPulosLinha){
        if(playerTime==1){
            if(linhaOrigem-nPulosLinha==linhaDestino && colunaOrigem-nPulosColuna==colunaDestino){
                if(matriz[linhaDestino][colunaDestino]=='-'){
                    for (int i=1; i<=nPulosLinha;i++){
                        if(matriz[linhaOrigem-i][colunaOrigem-i]=='-'){
                            printf("ok");
                        }
                        if(matriz[linhaOrigem-i][colunaOrigem-i]=='b' || matriz[linhaOrigem-i][colunaOrigem-i]=='B'){
                            printf("NAO EH VALIDO");
                            return 0;
                        }
                        if(matriz[linhaOrigem-i][colunaOrigem-i]=='P' || matriz[linhaOrigem-i][colunaOrigem-i]=='p'){
                            printf("Capturou uma peca");
                            //faz as rotinas de tirar a peca do tabuleiro, substituir por - 
                            //diminui o numero de peças do adversario
                            //continua
                            //vai ter que retornar para verificar se tem que jogar de novo
                            matriz[linhaOrigem-i][colunaOrigem-i]='-';
                            qtdP2--;
                            capturou = 1;
                        }
                    }
                    matriz[linhaDestino][colunaDestino]=matriz[linhaOrigem][colunaOrigem];
                    matriz[linhaOrigem][colunaOrigem] = '-';
                }
            }
        }
        if(playerTime==2){
            if(linhaOrigem-nPulosLinha==linhaDestino && colunaOrigem-nPulosColuna==colunaDestino){
                if(matriz[linhaDestino][colunaDestino]=='-'){
                    for (int i=1; i<=nPulosLinha;i++){
                        if(matriz[linhaOrigem-i][colunaOrigem+i]=='-'){
                            printf("ok");
                        }
                        if(matriz[linhaOrigem-i][colunaOrigem-i]=='p' || matriz[linhaOrigem-i][colunaOrigem-i]=='P'){
                            printf("NAO EH VALIDO");
                            //retorna de cara pois jogada nao eh valida
                            return 0 ;
                        }
                        if(matriz[linhaOrigem-i][colunaOrigem-i]=='b' || matriz[linhaOrigem-i][colunaOrigem-i]=='B'){
                            printf("Capturou uma peca");
                            //faz as rotinas de tirar a peca do tabuleiro, substituir por - 
                            //diminui o numero de peças do adversario
                            //continua
                            //vai ter que retornar para verificar se tem que jogar de novo
                            matriz[linhaOrigem-i][colunaOrigem-i]='-';
                            qtdP1--;
                            capturou = 1;
                        }
                    }
                    matriz[linhaDestino][colunaDestino]=matriz[linhaOrigem][colunaOrigem];
                    matriz[linhaOrigem][colunaOrigem] = '-';
                }
            }

        }
    } else{
        printf("Nao confere a posicao\n");
        return 0 ;
    }

    if (capturou==0){
        return 1;
    } else {
        return 2;
    }
}



int validaCD_DAMA(){
    int nPulosLinha=0, nPulosColuna=0;
    int capturou = 0;
    nPulosLinha = abs(linhaDestino - linhaOrigem);
    nPulosColuna = abs(colunaDestino - colunaOrigem);
    printf("%d\n", nPulosLinha);
    printf("%d\n", nPulosColuna);
    printf("%d %d\n", linhaDestino, linhaOrigem);
    printf("%d %d\n", colunaDestino, colunaOrigem);

    if (nPulosColuna==nPulosLinha){
        if(playerTime==1){
            if(linhaOrigem-nPulosLinha==linhaDestino && colunaOrigem+nPulosColuna==colunaDestino){
                if(matriz[linhaDestino][colunaDestino]=='-'){
                    for (int i=1; i<=nPulosLinha;i++){
                        if(matriz[linhaOrigem-i][colunaOrigem+i]=='-'){
                            printf("ok");
                        }
                        if(matriz[linhaOrigem-i][colunaOrigem+i]=='b' || matriz[linhaOrigem-i][colunaOrigem+i]=='B'){
                            printf("NAO EH VALIDO");
                            return 0;
                        }
                        if(matriz[linhaOrigem-i][colunaOrigem+i]=='P' || matriz[linhaOrigem-i][colunaOrigem+i]=='p'){
                            printf("Capturou uma peca");
                            //faz as rotinas de tirar a peca do tabuleiro, substituir por - 
                            //diminui o numero de peças do adversario
                            //continua
                            //vai ter que retornar para verificar se tem que jogar de novo
                            matriz[linhaOrigem-i][colunaOrigem+i]='-';
                            qtdP2--;
                            capturou = 1;
                        }
                    }
                    matriz[linhaDestino][colunaDestino]=matriz[linhaOrigem][colunaOrigem];
                    matriz[linhaOrigem][colunaOrigem] = '-';
                }
            }
        }
        if(playerTime==2){
            if(linhaOrigem-nPulosLinha==linhaDestino && colunaOrigem+nPulosColuna==colunaDestino){
                if(matriz[linhaDestino][colunaDestino]=='-'){
                    for (int i=1; i<=nPulosLinha;i++){
                        if(matriz[linhaOrigem-i][colunaOrigem+i]=='-'){
                            printf("ok");
                        }
                        if(matriz[linhaOrigem-i][colunaOrigem+i]=='p' || matriz[linhaOrigem-i][colunaOrigem+i]=='P'){
                            printf("NAO EH VALIDO");
                            //retorna de cara pois jogada nao eh valida
                            return 0 ;
                        }
                        if(matriz[linhaOrigem-i][colunaOrigem+i]=='b' || matriz[linhaOrigem-i][colunaOrigem+i]=='B'){
                            printf("Capturou uma peca");
                            //faz as rotinas de tirar a peca do tabuleiro, substituir por - 
                            //diminui o numero de peças do adversario
                            //continua
                            //vai ter que retornar para verificar se tem que jogar de novo
                            matriz[linhaOrigem-i][colunaOrigem+i]='-';
                            qtdP1--;
                            capturou = 1;
                        }
                    }
                }
                matriz[linhaDestino][colunaDestino]=matriz[linhaOrigem][colunaOrigem];
                matriz[linhaOrigem][colunaOrigem] = '-';
            }

        }
    } else{
        printf("Nao confere a posicao\n");
        return 0 ;
    }

    if (capturou==0){
        return 1;
    } else {
        return 2;
    }
}


int validaJogadaDama(){
    int sair=0;
     //movimenta para cima e para esquerda
    if(colunaOrigem>colunaDestino && linhaOrigem>linhaDestino){
        //faz as verificações para ver se a jogada é valida CIMA-ESQUERDA
        sair = validaCE_DAMA();
    } else {
        //movimenta para cima e para direita
        if (colunaOrigem<colunaDestino && linhaOrigem>linhaDestino){
            //movimenta para cima e para direita CIMA-DIREITA
            sair = validaCD_DAMA();
        } else {
            //movimenta para baixo e para a esquerda
            if(colunaOrigem>colunaDestino && linhaOrigem<linhaDestino){
                // BAIXO --> ESQUERDA
                sair = validaBE_DAMA();
            } else {
                if(colunaOrigem<colunaDestino && linhaOrigem<linhaDestino){
                    sair = validaBD_DAMA();
                }
            }
        }
    }

}

void realizaJogada(){
    int sair = 0;
    int terminajogada=0;
    int jogaDeNovo = 0;
    
    do
    {
            do {
            //Recebendo Origem
            printf("\nQual deseja mover?\n\tLinha: ");
            scanf(" %d", &linhaOrigem);
            printf("\t Coluna: ");
            scanf(" %d", &colunaOrigem);
            // valida a escolha da peça
            if((matriz[linhaOrigem][colunaOrigem]=='b' && playerTime==1) || (matriz[linhaOrigem][colunaOrigem]=='B' && playerTime==1)){
            printf("Pegou a peça valida\n");
            sair = 1;
            } else {
                if((matriz[linhaOrigem][colunaOrigem]=='p' && playerTime==2) || (matriz[linhaOrigem][colunaOrigem]=='P' && playerTime==2)){
                    printf("Pegou a peça valida\n");
                    sair = 1;
                } else {
                    printf("Pegou peça inválida\n");
                    sair=0;
                }

            }
            
            
        } while (sair==0);
        ////////////////////////////////////////////////////q
        sair = 0;

        if (matriz[linhaOrigem][colunaOrigem] == 'B' || matriz[linhaOrigem][colunaOrigem] == 'P'){
           //SE ENTRAR AQUI, FAZ O TRATAMENTO DE JOGADA PARA AS DAMAS
           //Recebendo Destino
            do{
                 printf("\nPara onde deseja mover?\n\tLinha: ");
                scanf(" %d", &linhaDestino);
                printf("\tColuna: ");
                scanf(" %d", &colunaDestino);

                //movimenta para cima e para esquerda
                if(colunaOrigem>colunaDestino && linhaOrigem>linhaDestino){
                    //faz as verificações para ver se a jogada é valida CIMA-ESQUERDA
                    sair = validaCE_DAMA();
                } else {
                    //movimenta para cima e para direita
                    if (colunaOrigem<colunaDestino && linhaOrigem>linhaDestino){
                        //movimenta para cima e para direita CIMA-DIREITA
                        sair = validaCD_DAMA();
                    } else {
                        //movimenta para baixo e para a esquerda
                        if(colunaOrigem>colunaDestino && linhaOrigem<linhaDestino){
                            // BAIXO --> ESQUERDA
                            sair = validaBE_DAMA();
                        } else {
                            if(colunaOrigem<colunaDestino && linhaOrigem<linhaDestino){
                                sair = validaBD_DAMA();
                            }
                        }
                    }
                }

                if(sair==2){
                    sair=1;
                    jogaDeNovo = 1;
                }

            } while (sair==0);
            
           


        } else{
            //faz jopgada e peca normal
             do{

                //Recebendo Destino
                printf("\nPara onde deseja mover?\n\tLinha: ");
                scanf(" %d", &linhaDestino);
                printf("\tColuna: ");
                scanf(" %d", &colunaDestino);

                //movimenta para cima e para esquerda
                if(colunaOrigem>colunaDestino && linhaOrigem>linhaDestino){
                //faz as verificações para ver se a jogada é valida CIMA-ESQUERDA
                sair = validaCE();
                } else {
                //movimenta para cima e para direita
                if (colunaOrigem<colunaDestino && linhaOrigem>linhaDestino){
                //movimenta para cima e para direita CIMA-DIREITA
                sair = validaCD();
                } else {
                //movimenta para baixo e para a esquerda
                if(colunaOrigem>colunaDestino && linhaOrigem<linhaDestino){
                // BAIXO --> ESQUERDA
                sair = validaBE();
                } else {
                if(colunaOrigem<colunaDestino && linhaOrigem<linhaDestino){
                sair = validaBD();
                }
                }
                }
                }

                if(sair==2){
                    sair=1;
                    jogaDeNovo = 1;
                }

            } while (sair==0);
        } 
       

        // SE TERMINAJPOGADA RECEBE 0 SIGNIFICA QUE O JOGADOR DEVE JOGARA NOVAMENTE
        // SE FOR IGUAL A 1, O JOGADOR VAI TER QUE PASSAR O TURNO
        if(jogaDeNovo==1){
            terminajogada = verificaTerminoTurno();
            jogaDeNovo = 1;
        } else
        {
            terminajogada = 1;
        }
        
        

    } while (terminajogada==0);
    

    
 
 
}

void define_jogadores(){
    //para pegar o placar de peças
     char p1[2], p2[2];

     //pega a parte da string que diz qual jogador eu sou
     //euSou = code[0]-48;
     if((code[0]-48)==0 || (code[0]-48)==2){
         euSou = 1;
         code[0] = '1';
     } else {
         euSou = 2;
         code[0] = '2';
     } 
}


void clear_buffer(){
    memset(code,0,100);
}

int leDrive(){
    fp = open("/dev/so", O_RDWR);
    if ( fp < 0 ){
        printf(".");
        return 0;
    }
    ret = read(fp, code);
    close(fp);
    return 1;
}

int escreveDrive(char val){
    fp = open("/dev/so", O_RDWR);
    if ( fp < 0 ){
        printf(".\n");
        return 0;
    }
    code[0] = val;
    ret = write(fp, code, 100);
    clear_buffer(code);
    close(fp);
    return 1;
}

int zeraJogo(){
    fp = open("/dev/so", O_RDWR);
    if ( fp < 0 ){
        //printf(".\n");
        return 0;
    }
    ret = read(fp, code);
    printf("%d", ret);
    if((code[0]=='2')){
        ret = write(fp, originalCode, 100);
    }
        
    
    clear_buffer(code);
    close(fp);
}

int validaVitoria(){
    if(qtdP1==0){
        vitoria='1';
        return 1;
    }
    if(qtdP2==0){
        vitoria='2';
        return 2;
    }

    return 0;
}

int main(){
    int ocupado, vitoriaJogo=0, teste;
    
    //teste = zeraJogo();
    
    ///////////////////////////////////////////////////////////////////////////////////
    ocupado = leDrive();
    printf("aqui");
    printf("%s", code);
   

    if(ocupado==0){
        
        while (ocupado==0){
            printf(".");
            sleep(2);
            ocupado = leDrive();
            
        }
    }

    if(code[0]=='0'){
        euSou = 1;
        escreveDrive('1');
    } else {
        if(code[0]=='1'){
            euSou = 2;
            escreveDrive('2');
        }
    }
    ////////////////////////////////////////////////////////////////////////////////
    
    while (vitoriaJogo == 0){
        ocupado = leDrive();
        if(ocupado==1){
            if(code[0]-48!=euSou){
            fp = open("/dev/so", O_RDWR);
            if ( fp < 0 ){
                perror("Nao foi possivel acessar\n");
                //exit(0);
            }

            rv = read(fp, code);
            printf("%s\n", code);
            
            extraiString();
            
            exibeMatriz();
                
            realizaJogada();
            
            vitoriaJogo = validaVitoria();

            montaString();
            
            ret = write(fp, sendCode, 100);
            clear_buffer(code);
            clear_buffer(sendCode);
            close(fp); 

        }

        ocupado = leDrive();
        printf("Aguarde1 - \n");
        sleep(2);
            
        }
        printf("Aguarde2 - \n");
        sleep(2);
        
    }

}