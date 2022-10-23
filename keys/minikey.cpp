#include "../include/minikey.h"
#include <memory.h>
#include <iostream>
const uint8_t factors[4] = {0x35, 0x9E, 0x82, 0xB6};
void expand_minikey(uint8_t* key, uint8_t* expanded, size_t orig, size_t target)
{
    if (!(orig > 4))
    {
        std::cout << "Original key must be at least 4 characters in length\n";
    }
    memcpy(expanded, key, orig);
    for (size_t i = orig; i < target; i++)
    {
        expanded[i] = factors[0] * expanded[i - 4] + factors[1] * expanded[i - 3] + 
            factors[2] * expanded[i - 2] + factors[3] * expanded[i - 1];
    }
}