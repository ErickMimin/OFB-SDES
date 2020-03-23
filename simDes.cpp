#include "crypto.h"
#include "des.h"
#include <iostream>
#include <bitset>
using namespace std;

bitset<10> generateKey(){
    srand (time(NULL));
    bitset<10> key;
    for(size_t index = 0; index < 10; index++)
        key[index] = (bool) rand() % 2;
    return key;
}

int main(int arc, char* argv[]){
    bitset<10> key("1010000010");
    //bitset<16> keys = createKeyDes(key);
    //cout << keys << endl;

    //cryptOFBSDES("Menssages/mensaje.txt", key, 0);
    
    cryptOFBSDES("Menssages/mensaje.ofb", key, 1);

    return 0;
}
