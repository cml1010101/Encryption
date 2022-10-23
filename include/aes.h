#ifndef AES_H
#define AES_H
#include <fstream>
#include <encryption.h>
enum AES_TYPE
{
    AES_128 = 0,
    AES_192 = 1,
    AES_256 = 2
};
const size_t aes_nr[3] = {10, 12, 14};
const size_t aes_nk[3] = {4, 6, 8};
extern uint8_t* aes_encrypt(const char* input, std::ofstream& output_file, size_t input_size, AES_TYPE type);
extern void aes_decrypt(const char* input, std::ofstream& output_file, size_t input_size, uint8_t* key, AES_TYPE type, size_t out_size);
class aes256 : public encrypter
{
public:
    aes256() = default;
    size_t key_length();
    size_t block_size();
    void encrypt(const uint8_t* input, size_t input_size, uint8_t* output, const uint8_t* key, encryption_header* header = 0);
    void decrypt(const uint8_t* input, size_t input_size, uint8_t* output, const uint8_t* key);
};
class aes192 : public encrypter
{
public:
    aes192() = default;
    size_t key_length();
    size_t block_size();
    void encrypt(const uint8_t* input, size_t input_size, uint8_t* output, const uint8_t* key, encryption_header* header = 0);
    void decrypt(const uint8_t* input, size_t input_size, uint8_t* output, const uint8_t* key);
};
class aes128 : public encrypter
{
public:
    aes128() = default;
    size_t key_length();
    size_t block_size();
    void encrypt(const uint8_t* input, size_t input_size, uint8_t* output, const uint8_t* key, encryption_header* header = 0);
    void decrypt(const uint8_t* input, size_t input_size, uint8_t* output, const uint8_t* key);
};
#endif