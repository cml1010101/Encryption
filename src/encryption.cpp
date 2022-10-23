#include <caesar.h>
#include <aes.h>
#include <encryption.h>
#include <filesystem>
#include <iostream>
#include <string.h>
#include <boost/program_options.hpp>
#include <minikey.h>
using namespace boost;
namespace po = boost::program_options;
using namespace std;
using namespace filesystem;
inline uint8_t* read_key(string key_str, size_t key_len)
{
    string substr;
    uint8_t* key = new uint8_t[key_len];
    for (size_t i = 0; i < key_len; i++)
    {
        substr = key_str.substr(i * 2, 2);
        key[i] = stoul(substr, 0, 16);
    }
    return key;
}
inline void print_key(uint8_t* key, size_t key_len, ostream& out)
{
    for (size_t i = 0; i < key_len; i++)
    {
        if (key[i] < 16)
        {
            out << "0" << hex << (size_t)key[i];
        }
        else
        {
            out << hex << (size_t)key[i];
        }
    }
    out << endl;
}
void encrypt_file(string input_path, string output_path, uint8_t* key, encrypter* enc, ostream& keyf)
{
    if (key == NULL)
    {
        key = new uint8_t[enc->key_length()];
        generate_key(key, enc->key_length());
    }
    ifstream in(input_path);
    encryption_header header;
    in.seekg(0, ios::end);
    size_t input_size = in.tellg();
    in.seekg(0, ios::beg);
    cout << "Encrypting '" << input_path << "'\nInput Size: " << input_size << "\n" << "File Output: " << output_path 
        << endl;
    header.file_size = input_size;
    char* input_data = new char[input_size];
    in.read(input_data, input_size);
    in.close();
    uint8_t* output_data = new uint8_t[((input_size + enc->block_size() - 1) / enc->block_size()) * enc->block_size()];
    enc->encrypt((uint8_t*)input_data, input_size, output_data, key, &header);
    ofstream out(output_path);
    out.seekp(0, ios::beg);
    out.write((char*)&header, sizeof(header));
    out.write((char*)output_data, ((input_size + enc->block_size() - 1) / enc->block_size()) * enc->block_size());
    out.close();
    keyf << path(input_path) << ":";
    print_key(key, enc->key_length(), keyf);
}
void encrypt_folder(string input_path, string output_path, uint8_t* key, encrypter* enc, ostream& keyf)
{
    if (!is_directory(output_path)) create_directory(output_path);
    for (auto entry : directory_iterator(input_path))
    {
        if (is_directory(entry.path().c_str())) encrypt_folder(entry.path().c_str(), output_path + "/" + string(entry.path().filename()), key, enc, keyf);
        else
        {
            encrypt_file(entry.path().c_str(), output_path + "/" + string(entry.path().filename()), key, enc, keyf);
        }
    }
}
int main(int argc, char const *argv[])
{
    srand(chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count());
    po::options_description desc("Allowed options");
    desc.add_options()
        ("help,h", "print list of arguments")
        ("type,t", po::value<string>()->default_value("aes256"), "set type of encryption")
        ("key-file,K", po::value<string>(), "key file")
        ("key,k", po::value<string>(), "key")
        ("output-file,o", po::value<vector<string>>(), "output file")
        ("input-file", po::value<vector<string>>(), "input file")
    ;
    po::positional_options_description p;
    p.add("input-file", -1);
    po::variables_map vm;
    po::store(po::command_line_parser(argc, argv).options(desc).positional(p).run(), vm);
    po::notify(vm);
    if (vm.count("help"))
    {
        cout << desc << endl;
        return 0;
    }
    auto input_paths = vm["input-file"].as<vector<string>>();
    vector<string> output_paths;
    if (vm.count("output-file")) output_paths = vm["output-file"].as<vector<string>>();
    else output_paths = input_paths;
    string encryption_type = vm["type"].as<string>();
    string key_file = vm.count("key-file") ? vm["key-file"].as<string>() : "";
    ofstream keyfile;
    if (key_file != "")
    {
        keyfile.open(key_file);
        keyfile.seekp(0);
    }
    ostream& keyf = key_file == "" ? cout : keyfile;
    uint8_t* key = 0;
    encrypter* enc;
    if (encryption_type == "caesar")
    {
        enc = new caesar();
    }
    else if (encryption_type == "aes256")
    {
        enc = new aes256();
    }
    else if (encryption_type == "aes192")
    {
        enc = new aes192();
    }
    else if (encryption_type == "aes128")
    {
        enc = new aes128();
    }
    else
    {
        cout << "Invalid encryption method" << endl;
        return 1;
    }
    if (vm.count("key"))
    {
        string keystr = vm["key"].as<string>();
        if (keystr.length() < enc->key_length() * 2)
        {
            key = new uint8_t[enc->key_length()];
            memcpy(key, keystr.c_str(), keystr.length());
            expand_minikey(key, key, keystr.length(), enc->key_length());
        }
        else if (keystr.length() == enc->key_length() * 2)
        {
            key = read_key(keystr, enc->key_length());
        }
    }
    else
    {
        key = new uint8_t[enc->key_length()];
        generate_key(key, enc->key_length());
    }
    for (size_t i = 0; i < input_paths.size(); i++)
    {
        string input_path = input_paths[i];
        string output_path = output_paths[i];
        if (is_directory(input_path)) encrypt_folder(input_path, output_path, key, enc, keyf);
        else encrypt_file(input_path, output_path, key, enc, keyf);
    }
    return 0;
}
