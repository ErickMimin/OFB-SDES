#ifndef CRYPTO_H_
#define CRYPTO_H_

typedef struct{
    unsigned long* order;
    unsigned long tamanio;
} tablePermutation;

typedef struct{
    char* chars;
    unsigned long tamanio;
} menssage;

/* 
*   Funciones para crear tablas de permutacion
*   In:
*       char* -> nombre del archivo archivo
*   Out:
*       tablePermutation -> tabla de permutación
*/
tablePermutation* createPermutationTable(char*);
tablePermutation* createPermutationTableInv(char*);
void deleteTable(tablePermutation*);

/* 
*   Funcion para modificar el mensaje
*   In:
*       menssage* -> mensaje a modificar
*       tablePermutation -> tabla de permutación
*   Out:
*       menssage* -> mensaje modificado
*/
void permutation(menssage*, menssage*, tablePermutation*);

/* 
*   Funciones para modificar tratar menssage
*/
menssage* createMenssage(unsigned long);
void deleteMenssage(menssage*);

#endif