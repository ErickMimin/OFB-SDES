DEPENDENCIES = LibrariesC/crypto.c
DEPENDENCIES_CPP = LibrariesC/crypto.c LibrariesC/des.cpp
LIBRARIES = -I./LibrariesC
FILE = Menssages/mensaje
TABLE = table.txt

run_per_en: permutation
	./permutation 1 $(TABLE) $(FILE).txt $(FILE).per

run_per_de: permutation
	./permutation 2 $(TABLE) $(FILE).per $(FILE)2.txt

run_des_en: simDes
	./simDes

run_des_de: simDes
	./simDes

permutation: permutation.c $(DEPENDENCIES)
	gcc $(LIBRARIES) permutation.c $(DEPENDENCIES) -o permutation

simDes: simDes.cpp $(DEPENDENCIES_CPP)
	g++ $(LIBRARIES) simDes.cpp $(DEPENDENCIES_CPP) -o simDes