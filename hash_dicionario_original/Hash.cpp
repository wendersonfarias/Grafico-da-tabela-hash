#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Hash.h"


//Inicio
//Nesta fun��o de inicializa��o iremos percorrer cada posi��o de nossa tabela de hash inicializando a lista duplamente ligada de cada posi��o (Defini��o em http://www.jppreti.com/2019/07/29/tabela-hash/#Hashh)
//init usado � o da DoublyLinkedList, Lista Duplamente Ligada.

void initHash(HashStruct *hashStruct) {
    for (int i=0;i<MAX;i++) {
        //chamando init de DoublyLinkedList para inicializar cada lista do vetor
        init(&(hashStruct->hashes[i]));
    }
    hashStruct->size = 0;
}

bool isHashEmpty(HashStruct *hashStruct) {
    return hashStruct->size==0;
}
// hash (by Matheus Santiago) : Recebe uma chave e calcula qual posi��o deveremos inserir o dado associado a chave.
//A chave pode ser um nome, numero ou codigo de barras, normalmente � um dado unico.
int hash(char *key) {
    int sum = 0;
    // percorremos todos os caracteres da string passada
    for (int i = 0; key[i]!=0;i++) {
         //acumulamos os c�digos ascii de cada letra com um peso
        sum+=key[i]*(i+1);
    }
    return sum%MAX; //retorna o resto da divis�o
}
// put by Wenderson Farias / verifica se a chave ja foi inserida na tabela
// caso nao, ent�o � inserido um novo elemento na tabela.
int put(HashStruct *hashStruct, char *key, void *data, compare equal)  {
	if (!containsKey(hashStruct, key, equal)){
        //adiciona na fila que est� na posi��o devolvida pela fun��o hash
        int res = enqueue(&hashStruct->hashes[hash(key)],data);
        //incrementa a qtde de elementos baseado na quantidade inserida por enqueue
        hashStruct->size+=res;
        return res;
    }
    return 0;
}

// containsKey (by Leandro Klein) : verificar se a chave j� existe na tabela de hash.
bool containsKey(HashStruct *hashStruct, char *key, compare equal) {
    //calcula a posi��o
    int hashValue = hash(key);//fun��o para descobrir em que lista est� a chave.
    //busca na fila a posi��o da chave
    int pos = indexOf(&hashStruct->hashes[hashValue], key, equal); //A fun��o indexOf retorna a posi��o da chave na lista. Caso o retorno seja -1 a chave n�o est� na lista.
    //printf("\n Posicao >>> %d \n",pos);
    return (pos!=-1)?true:false;

}


//Fun��o get by Carlos Henrique: Reqaliza busca no codigo e retorna o dado procurado, se n�o houverem dados retorna o primeiro n� (sentinela) de valor nulo.
void* get(HashStruct *hashStruct, char *key, compare equal) {
    // descobre em qual fila/lista est� o dado
    int hashValue = hash(key);
    //first � n� sentinela, come�amos do segundo n�
    Node *aux = hashStruct->hashes[hashValue].first->next;
    // procuramos o dado na lista
    while(aux!=hashStruct->hashes[hashValue].first && !equal(aux->data, key))
        aux=aux->next;
    return aux->data;
}
// fun��o hash(thiago ramalho) gera uma chave e calcula qual a posi��o em que deve-se inserir o dado associado a chave, na qual a chave pode ser um nome, c�digo de barras ou n�mero, mas na maioria dos casos � um dado �nico

//Fun��o removeKey (by Wallatan Fran�a / Mickael Luiz)	remove um par (chave, valor)
void* removeKey(HashStruct *hashStruct, char *key, compare equal) {
    int hashValue = hash(key);// ap�s calcular o hash da chave enviada, atribui o valor a v�riavel do tipo hashValue
    int pos = indexOf(&hashStruct->hashes[hashValue], key, equal);// a v�riavel pos tipo int, recebe a posi��o encontrada por indexOf com base nos par�metros passados
	//remove o valor da posi��o
    void* result = removePos(&hashStruct->hashes[hashValue], pos);
	//Verifica se � o unico n� da lista, se nao for, diminui um valor de size em 1
    if (result!=NULL) hashStruct->size--;
    return result;
}

//fun��o ShowHashStruct por Lucio Lisboa. Fun��o com o proposito de exibir os pares armazenados, ou seja, mostra quantos hash tem e quantos elementos cada hash tem
void showHashStruct(HashStruct *hashStruct, printNode print) {
    //printf mostrando na tela quantos elementos a hash tem
    printf("Existem %d elementos nesta tabela hash\n\n",hashStruct->size);
    //estrutura de repeti��o com o intuito de navegar entre as hashes e mostrar quantos elementos cada hash tem
    for (int i=0; i < MAX; i++) {
        printf("Hash %d tem %d elementos: ",i,hashStruct->hashes[i].size);
        show(&hashStruct->hashes[i],print);
        printf("\n");
    }
}