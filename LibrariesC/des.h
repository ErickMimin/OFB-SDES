#include "crypto.h"
#include <bitset>
#include <string>
using namespace std;
#ifndef DES_H_
#define DES_H_

bitset<16> createKeyDes(bitset<10>);

/* Llave S-DES */

/* Permutaciones para la llave de S-DES */
void perBlock10(bitset<10>&, tablePermutation*);
void perBlock8(bitset<8>&, tablePermutation*);
void perBlock4(bitset<4>&, tablePermutation*);
/* Corrimiento circular a la izq */
void shiftKeyL(bitset<5>&, unsigned int);

/* S-DES */
void cryptSDES(bitset<8>&, bitset<8>&, bitset<8>&);
void decryptSDES(bitset<8>&);
void sSDES(bitset<4>&, bitset<4>&);

/* Mode operation OFB for S-DES */
void cryptOFBSDES(string, bitset<10>&, bool);
bitset<8> generateIV();

/* Auxilary functions */
void toFile(string, string);

#endif