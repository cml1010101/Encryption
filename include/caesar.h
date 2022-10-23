#ifndef CAESAR_H
#define CAESAR_H
#include <fstream>
#include <encryption.h>
extern uint8_t* caesar_encrypt(const char* input, std::ofstream& output_file, size_t input_size);
extern void caesar_decrypt(const char* input, std::ofstream& output_file, size_t input_size, uint8_t* key, size_t out_size);
class caesar : public encrypter
{
public:
    caesar() = default;
    size_t key_length();
    size_t block_size();
    void encrypt(const uint8_t* input, size_t input_size, uint8_t* output, const uint8_t* key, encryption_header* header = 0);
    void decrypt(const uint8_t* input, size_t input_size, uint8_t* output, const uint8_t* key);
};
#endif