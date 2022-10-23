#include <caesar.h>
#include <aes.h>
#include <encryption.h>
#include <filesystem>
#include <iostream>
#include <string.h>
#include <stdio.h>
#include <boost/program_options.hpp>
#include <sstream>
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
int decrypt_file(string input_path, string output_path, string keystr, string encryption_type)
{
    ifstream in(input_path.c_str());
    in.seekg(0, ios::end);
    size_t input_size = in.tellg();
    size_t output_size = input_size;
    in.seekg(0, ios::beg);
    encryption_header header;
    encrypter* enc;
    in.read((char*)&header, sizeof(header));
    if (header.magic != 0x454E435259505430)
    {
        in.seekg(0, ios::beg);
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
    }
    else
    {
        output_size = header.file_size;
        input_size -= sizeof(encryption_header);
        if (header.enc_type == ENC_CAESAR)
        {
            enc = new caesar();
        }
        else if (header.enc_type == ENC_AES128)
        {
            enc = new aes128();
        }
        else if (header.enc_type == ENC_AES192)
        {
            enc = new aes192();
        }
        else if (header.enc_type == ENC_AES256)
        {
            enc = new aes256();
        }
        else
        {
            return 1;
        }
    }
    uint8_t* key;
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
    cout << "Decrypting '" << input_path << "'\nInput Size: " << input_size << "\n" << "File Output: " << output_path 
        << endl;
    uint8_t* input_data = new uint8_t[input_size];
    in.read((char*)input_data, input_size);
    in.close();
    uint8_t* output_data = new uint8_t[input_size];
    ofstream out(output_path.c_str());
    enc->decrypt(input_data, input_size, output_data, key);
    out.write((char*)output_data, output_size);
    delete[] input_data;
    delete[] output_data;
    out.close();
    return 0;
}
int decrypt_folder(string input_path, string output_path, string keystr, string encryption_type)
{
    if (!is_directory(output_path)) create_directory(output_path);
    for (auto entry : directory_iterator(input_path))
    {
        if (is_directory(entry.path().c_str()))
        {
            if (decrypt_folder(entry.path().c_str(), output_path + "/" + string(entry.path().filename()), keystr, encryption_type)) return 1;
        }
        else if (decrypt_file(entry.path().c_str(), output_path + "/" + string(entry.path().filename()), keystr, encryption_type)) return 1;
    }
    return 0;
}
int main(int argc, char const *argv[])
{
    po::options_description desc("Allowed options");
    desc.add_options()
        ("help,h", "print list of arguments")
        ("type,t", po::value<string>()->default_value("aes256"), "set type of encryption")
        ("key-file,K", po::value<string>(), "key file")
        ("key,k", po::value<vector<string>>(), "key")
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
    map<string, string> keymap;
    if (vm.count("key"))
    {
        vector<string> keystrs = vm["key"].as<vector<string>>();
        for (size_t i = 0; i < input_paths.size(); i++)
        {
            keymap.emplace(input_paths[i], keystrs[i]);
        }
    }
    else
    {
        ifstream keyf(key_file);
        string fname, k;
        for (size_t i = 0; i < input_paths.size(); i++)
        {
            getline(keyf, fname, ':');
            getline(keyf, k);
            keymap.emplace(fname, k);
        }
    }
    for (size_t i = 0; i < input_paths.size(); i++)
    {
        string input_path = input_paths[i], output_path = output_paths[i];
        string key_str = keymap.find(input_path) == keymap.end() ? (*keymap.begin()).second : keymap[input_path];
        if (is_directory(input_path))
        {
            return decrypt_folder(input_path, output_path, key_str, encryption_type);
        }
        else return decrypt_file(input_path, output_path, key_str, encryption_type);
    }
    return 0;
}