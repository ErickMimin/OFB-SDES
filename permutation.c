#include "crypto.h"
#include <stdio.h>
#include <stdlib.h>

/*
*   Funcion 
*   argv[1] -> Modo Enc/Dec
*   argv[2] -> Archivo de tabla de permutacion
*   argv[3] -> Mensaje a modificar
*   argv[4] -> Mensaje modificado
*
*/

void main(int arc, char* argv[]){
    FILE *fileMenssage, *fileCipher;
    tablePermutation *table;
    menssage *men, *cipher;
    size_t leidos;

    fileMenssage = fopen(argv[3], "rb");
    if(fileMenssage == NULL){
        perror("ERROR: Abriendo el archivo con el mensaje");
        exit(1);
    }

    remove(argv[4]);
    fileCipher = fopen(argv[4], "ab");
    if(fileCipher == NULL){
        perror("ERROR: Abriendo el archivo de destino");
        exit(1);
    }

    switch (atoi(argv[1])){
        case 1:
            table = createPermutationTable(argv[2]);
            break;
        case 2:
            table = createPermutationTableInv(argv[2]);
            break;        
        default:
            perror("ERROR: Acción no especificada");
            exit(1);
            break;
    }

    men = createMenssage((table->tamanio) >> 3);
    cipher = createMenssage((table->tamanio) >> 3);

    leidos = fread (men->chars, 1, men->tamanio, fileMenssage);
    while (leidos != 0){
        permutation(cipher, men, table);
        fwrite(cipher->chars, 1, cipher->tamanio, fileCipher);
        /* y leer siguiente bloque */
        leidos = fread (men->chars, 1, men->tamanio, fileMenssage);
    }

    /* Liberación de memoria */
    deleteTable(table);
    deleteMenssage(men);
    deleteMenssage(cipher);
    /* Cerramos archivos */
    fclose(fileMenssage);
    fclose(fileCipher);
}