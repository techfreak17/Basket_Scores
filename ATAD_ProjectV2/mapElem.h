#pragma once
#include "checkType.h"

typedef int MapKey;
typedef ToValue MapValue;

void mapKeyPrint(MapKey key);
void mapValuePrint(MapValue value);

/* funcao de comparacao de chaves */
int mapKeyEquals(MapKey key1, MapKey key2);
