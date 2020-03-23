#include "des.h"
#include "crypto.h"
#include <iostream>
#include <bitset>
#include <fstream> 
#include <string>
#include <time.h>
#include <stdlib.h>
using namespace std;

string s0[16] = {"01", "00", "11", "10",
                 "11", "10", "01", "00",
                 "00", "10", "01", "11",
                 "11", "01", "11", "10"};

string s1[16] = {"00", "01", "10", "11",
                 "10", "00", "01", "11",
                 "11", "00", "01", "00",
                 "10", "01", "00", "11"};

/* Creamos tablas de permutacion */
tablePermutation *globalTableIP = createPermutationTable((char*)"Tables/tableIP.tab");
tablePermutation *globalTableEP = createPermutationTable((char*)"Tables/tableEP.tab");
tablePermutation *globalTable4 = createPermutationTable((char*)"Tables/table4.tab");
tablePermutation *globalTableInvIP = createPermutationTableInv((char*)"Tables/tableIP.tab");

bitset<16> createKeyDes(bitset<10> originalKey){
    bitset<16> finalKeys;
    bitset<5> midBlockL, midBlockD;
    bitset<8> block;
    size_t index;

    tablePermutation *table8, *table10;

    /* Creamos tablas de permutacion */
    table8 = createPermutationTable((char*)"Tables/table8.tab");
    table10 = createPermutationTable((char*)"Tables/table10.tab");

    /* Hacemos la permutacion de 10 bits */
    perBlock10(originalKey, table10);
    
    /* Separamos en dos bloques */
    for(index = 0; index < 5; index++){
        midBlockL.set(index, originalKey.test(index + 5));
        midBlockD.set(index, originalKey.test(index));
    }

    shiftKeyL(midBlockL, 1);
    shiftKeyL(midBlockD, 1);

    /* Unimos y creamos un bloque de 8 bits */
    for(index = 0; index < 5; index++)        
        block.set(index, midBlockD.test(index));
    for(index = 5; index < 8; index++)        
        block.set(index, midBlockL.test(index - 5));

    /* Creamos la primera llave */ 
    perBlock8(block, table8);
    for(index = 8; index < 16; index++)        
        finalKeys.set(index, block.test(index - 8));

    shiftKeyL(midBlockL, 2);
    shiftKeyL(midBlockD, 2);

    for(index = 0; index < 5; index++)        
        block.set(index, midBlockD.test(index));
    for(index = 5; index < 8; index++)        
        block.set(index, midBlockL.test(index - 5));  

    /* Creamos la segunda llave */ 
    perBlock8(block, table8);
    for(index = 0; index < 8; index++)        
        finalKeys.set(index, block.test(index));

    deleteTable(table8);
    deleteTable(table10);
    return finalKeys;
}

void perBlock10(bitset<10> &block, tablePermutation *table){
    bitset<10> auxBlock(block.to_string());
    for(size_t index = 0; index < table->tamanio; index++)
        block[9 - index] = auxBlock[10 - table->order[index]];
}

void perBlock8(bitset<8> &block, tablePermutation *table){
    size_t index;
    bitset<8> auxBlock(block.to_string());
    for(size_t index = 0; index < table->tamanio; index++)
        block[7 - index] = auxBlock[8 - table->order[index]];
}

void perBlock4(bitset<4> &block, tablePermutation *table){
    size_t index;
    bitset<4> auxBlock(block.to_string());
    for(size_t index = 0; index < table->tamanio; index++)
        block[3 - index] = auxBlock[4 - table->order[index]];
}

void shiftKeyL(bitset<5> &block, unsigned int shift){
    bool aux;
    for(size_t index = 0; index < shift; index++){
        aux = block[4];
        block <<= 1;
        block[0] = aux;
    }
}

void cryptSDES(bitset<8> &block, bitset<8> &key1, bitset<8> &key2){
    bitset<4> midBlockL, midBlockD, auxMidBlockL, auxMidBlockD;
    size_t index;

    /* Permutacion y separacion */
    perBlock8(block, globalTableIP);

    for(index = 0; index < 4; index ++){
        midBlockD.set(index, block.test(index));
        midBlockL.set(index, block.test(index + 4));
        block.set(index, midBlockD.test(index));
        block.set(index + 4, midBlockD.test(index));
    }

    perBlock8(block, globalTableEP);

    /* XOR con la llave 1 */
    block ^= key1;

    for(index = 0; index < 4; index ++){
        auxMidBlockD.set(index, block.test(index));
        auxMidBlockL.set(index, block.test(index + 4));
    }

    /* S-BOX de DES */
    sSDES(auxMidBlockL, auxMidBlockD);
    perBlock4(auxMidBlockL, globalTable4);

    /* Preparamos la segunda parte */
    auxMidBlockL ^= midBlockL;
    midBlockL = midBlockD;     
    midBlockD = auxMidBlockL;

    /* Segunda ronda de S-DES */
    for(index = 0; index < 4; index ++){
        block.set(index, midBlockD.test(index));
        block.set(index + 4, midBlockD.test(index));
    }

    perBlock8(block, globalTableEP);

    /* XOR con la llave 2 */
    block ^= key2;

    for(index = 0; index < 4; index ++){
        auxMidBlockD.set(index, block.test(index));
        auxMidBlockL.set(index, block.test(index + 4));
    }

    /* S-BOX de DES */
    sSDES(auxMidBlockL, auxMidBlockD);
    perBlock4(auxMidBlockL, globalTable4);    

    /* Ultima parte de S-DES */
    midBlockL ^= auxMidBlockL;

    for(index = 0; index < 4; index++){
        block.set(index, midBlockD.test(index));
        block.set(index + 4, midBlockL.test(index));
    }

    perBlock8(block, globalTableInvIP);
}

void decryptSDES(bitset<8> &block, bitset<8> &key1, bitset<8> &key2){
    cryptSDES(block, key2, key1);
}


void sSDES(bitset<4> &id1, bitset<4> &id2){
    int index1 = (((id1[3] * 2) + id1[0]) * 4) + ((id1[2] * 2) + id1[1]);
    int index2 = (((id2[3] * 2) + id2[0]) * 4) + ((id2[2] * 2) + id2[1]);
    bitset<4> qBlock(s0[index1] + s1[index2]);
    id1 = qBlock;
}

/* Funcion para cifrar y descifra
    0 => Cifrar
    1 => Descifrar */
void cryptOFBSDES(string fileName, bitset<10> &key, bool sel){
    string dataFinal;
    size_t index, leidos;
    FILE *file;
    bitset<8> key1, key2, block, auxBlock;
    bitset<16> keys;
    char auxChar;

    /* Creamos las dos llaves para S-DES */
    keys = createKeyDes(key);
    for(index = 0; index < 8; index++){
        key1[index] = keys[index + 8];
        key2[index] = keys[index];
    }

    /* Leemos todo el mensaje */
    file = fopen(fileName.c_str(), "rb");

    /* Generamo el vector inicial */
    block = generateIV();

    /* Proceso de cifrado */

    leidos = fread (&auxChar, 1, 1, file);
    while (leidos != 0){
        cryptSDES(block, key1, key2);
        auxBlock = block;
        block ^= bitset<8>(auxChar);
        dataFinal.push_back((char) block.to_ulong());
        block = auxBlock;
        leidos = fread (&auxChar, 1, 1, file);
    }
        

    fclose(file);
    /* Guardamos en archivo */
    toFile(fileName.replace(fileName.find('.'), 4, sel ? ".txt" :".ofb"), dataFinal);
}


bitset<8> generateIV(){
    srand (time(NULL));
    bitset<8> iv;
    for(size_t index = 0; index < 8; index++)
        iv[index] = (bool) rand() % 2;
    return iv;
}

void toFile(string name, string content){
    ofstream fileO(name);
    fileO << content;
    fileO.close();
}