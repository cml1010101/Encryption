#ifndef ENCRYPTION_H
#define ENCRYPTION_H
#include <stddef.h>
#include <stdint.h>
#include <random>
enum ENC_TYPE
{
    ENC_AES256 = 0,
    ENC_AES192 = 1,
    ENC_AES128 = 2,
    ENC_CAESAR = 3
};
struct encryption_header
{
    size_t magic;
    ENC_TYPE enc_type;
    size_t file_size;
};
inline void generate_key(uint8_t* key, size_t key_length)
{
    for (size_t i = 0; i < key_length; i++)
    {
        key[i] = rand() & 0xFF;
    }
}
class encrypter
{
public:
    encrypter() = default;
    virtual size_t key_length() = 0;
    virtual size_t block_size() = 0;
    virtual void encrypt(const uint8_t* input, size_t input_size, uint8_t* output, const uint8_t* key, encryption_header* header = 0) = 0;
    virtual void decrypt(const uint8_t* input, size_t input_size, uint8_t* output, const uint8_t* key) = 0;
};
#endif