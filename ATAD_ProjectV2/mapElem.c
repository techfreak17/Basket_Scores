#include "mapElem.h"

void mapKeyPrint(MapKey key) {
	printf("%d \n", key);
}

void mapValuePrint(MapValue value) {
	printValue(value);
}

/* funcao de comparacao de chaves */
int mapKeyEquals(MapKey key1, MapKey key2) {
	//no caso de MapKey == int. Alterar de acordo
	//com o tipo efetivo
	return (key1 == key2);
}