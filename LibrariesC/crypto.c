#include "crypto.h"
#include <stdio.h>
#include <stdlib.h>

tablePermutation* createPermutationTable(char* file){
    FILE *fk;
    tablePermutation *table;
    size_t i;
    table = (tablePermutation*) malloc(sizeof(tablePermutation));
    /* Abrimos el archivo donde esta la tabla */
    fk = fopen(file, "r");
    if(fk == NULL){
        perror("Error al abrir el archivo con la tabla de permutación");
        exit(1);
    }
    fscanf(fk, "%ld", &(table->tamanio));
    table->order = (size_t*) malloc(table->tamanio * sizeof(size_t));
    /* Leemos los renglones */
    for(i = 0; !feof(fk); i++)
        fscanf(fk, "%ld", table->order + i);
    fclose(fk);
    return table;
}

tablePermutation* createPermutationTableInv(char* file){
    FILE *fk;
    tablePermutation *table;
    size_t i, aux;
    table = (tablePermutation*) malloc(sizeof(tablePermutation));
    table->order = NULL;
    /* Abrimos el archivo donde esta la tabla */
    fk = fopen(file, "r");
    if(fk == NULL){
        perror("ERROR: Abriendo archivo con la tabla de permutación");
        exit(1);
    }
    fscanf(fk, "%ld", &(table->tamanio));
    table->order = (size_t*) malloc(table->tamanio * sizeof(size_t));
    /* Leemos los renglones */
    for(i = 1; !feof(fk); i++){
        /* Hacemos la inversion en este caso */
        fscanf(fk, "%ld", &aux);
        table->order[aux - 1] = i; 
    }
    fclose(fk);
    return table;
}

void deleteTable(tablePermutation* table){
    free(table->order);
    free(table);
}

menssage* createMenssage(size_t tamanio){
    menssage* men = (menssage*) malloc(sizeof(menssage));
    men->tamanio = tamanio;
    men->chars = (char*) malloc(tamanio);
    return men;
}

void deleteMenssage(menssage *men){
    free(men->chars);
    free(men);
}


void permutation(menssage *modifiedMenssge, menssage *m, tablePermutation *t){
    size_t index, mapIndexChar, bit;

    unsigned char aux, maskGetBit = 0b10000000;
    for(bit = 0; bit < t->tamanio; bit ++){
        /* Sacamos el char y el bit a mover */
        index = t->order[bit] - 1;
        /* Sacamos el bit que queremos */
        aux = m->chars[index >> 3];
        aux = aux & (maskGetBit >> index % 8);
        /* Mapeamos el char y el bit donde ira */           
        mapIndexChar = bit >> 3;
        /* Apagamos o encendemos el bit segun sea el caso */
        if(aux != 0)
            modifiedMenssge->chars[mapIndexChar] = modifiedMenssge->chars[mapIndexChar] | (maskGetBit >> bit % 8);
        else
            modifiedMenssge->chars[mapIndexChar] = modifiedMenssge->chars[mapIndexChar] & (~(maskGetBit >> bit % 8));
    }
}

