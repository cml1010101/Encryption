#include <caesar.h>
using namespace std;
uint8_t* caesar_encrypt(const char* input, ofstream& output_file, size_t input_size)
{
    char* output = new char[input_size];
    size_t key = (rand() % 255) + 1;
    for (size_t i = 0; i < input_size; i++)
    {
        output[i] = input[i] + key;
    }
    output_file.write(output, input_size);
    uint8_t* k = new uint8_t;
    *k = key;
    return k;
}
void caesar_decrypt(const char* input, ofstream& output_file, size_t input_size, uint8_t* key, size_t out_size)
{
    char* output = new char[input_size];
    for (size_t i = 0; i < input_size; i++)
    {
        output[i] = input[i] - *key;
    }
    output_file.write(output, out_size);
}
size_t caesar::key_length()
{
    return 1;
}
size_t caesar::block_size()
{
    return 1;
}
void caesar::encrypt(const uint8_t* input, size_t input_size, uint8_t* output, const uint8_t* key, encryption_header* header)
{
    for (size_t i = 0; i < input_size; i++)
    {
        output[i] = input[i] + *key;
    }
    if (header != 0)
    {
        header->file_size = input_size;
        header->enc_type = ENC_CAESAR;
        header->magic = 0x454E435259505430;
    }
}
void caesar::decrypt(const uint8_t* input, size_t input_size, uint8_t* output, const uint8_t* key)
{
    for (size_t i = 0; i < input_size; i++)
    {
        output[i] = input[i] - *key;
    }
}