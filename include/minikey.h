#ifndef MINIKEY_H
#define MINIKEY_H
#include <stdint.h>
#include <stddef.h>
extern void expand_minikey(uint8_t* key, uint8_t* expanded_key, size_t original_key_size, size_t expanded_size);
#endif