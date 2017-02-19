//
// Created by tts on 2/18/17.
//
#include "SDES/SDES.h"

int SDES::BinToDec(std::string bin_str) {
    int num = 0;

    // check valid string
    if (bin_str.length() < 32) {
        for (char c : bin_str) {
            if (c != '0' && c != '1') {
                printf("Error: Invalid bit string (SDES::BinToDec) \n");
                return -1;
            }
            else {
                // shift left num and OR with c fall back 48
                num = (num << 1) | (c - '0');
            }
        }
    }
    else {
        printf("Error: Invalid bit string (SDES::BinToDec) \n");
        return -1;
    }
    return num;
}

std::string SDES::DecToBin(int dec, PermutationTypes type) {
    switch (type) {
        case Type_P10:
        case Type_P8:
            return std::bitset<10>(dec).to_string();
        case Type_P4:
        case Type_EP:
            return std::bitset<4>(dec).to_string();
        case Type_IP:
        case Type_IP_:
        case Type_SW:
            return std::bitset<8>(dec).to_string();
        case Type_S:
            return std::bitset<2>(dec).to_string();
        case Type_P5:
            return std::bitset<5>(dec).to_string();
    }
    return "";
}

void SDES::SetConfiguration(std::string file_name) {
    int current_array = 0;
    std::string line;
    std::ifstream file(file_name);

    if (file.is_open()) {
        while (getline(file, line)) {
            if (line[0] == '/' || line[0] == ' ' || line.empty()) continue;
            else {
                std::string item;
                size_t index = 0;
                std::stringstream string_stream;
                switch (current_array) {
                    case 0:
                        // read P10
                        index = 0;
                        string_stream.str(line);
                        while (string_stream >> item) {
                            P10[index++] = std::stoi(item);
                        }
                        current_array++;
                        break;
                    case 1:
                        // read P8
                        index = 0;
                        string_stream.str(line);
                        while (string_stream >> item) {
                            P8[index++] = std::stoi(item);
                        }
                        current_array++;
                        break;
                    case 2:
                        // read IP
                        index = 0;
                        string_stream.str(line);
                        while (string_stream >> item) {
                            IP[index++] = std::stoi(item);
                        }
                        current_array++;
                        break;
                    case 3:
                        // read IP_
                        index = 0;
                        string_stream.str(line);
                        while (string_stream >> item) {
                            IP_[index++] = std::stoi(item);
                        }
                        current_array++;
                        break;
                    case 4:
                        // read EP
                        index = 0;
                        string_stream.str(line);
                        while (string_stream >> item) {
                            EP[index++] = std::stoi(item);
                        }
                        current_array++;
                        break;
                    case 5:
                        // read S0
                        index = 0;
                        string_stream.str(line);
                        while (string_stream >> item) {
                            S0[index / 4][index % 4] = std::stoi(item);
                            ++index;
                        }
                        current_array++;
                        break;
                    case 6:
                        // read S1
                        index = 0;
                        string_stream.str(line);
                        while (string_stream >> item) {
                            S1[index / 4][index % 4] = std::stoi(item);
                            ++index;
                        }
                        current_array++;
                        break;
                    case 7:
                        // read P4
                        index = 0;
                        string_stream.str(line);
                        while (string_stream >> item) {
                            P4[index++] = std::stoi(item);
                        }
                        current_array++;
                        break;
                    default:
                        break;
                }
            }
        }
        file.close();
    }
    else {
        printf("Error: Cannot open file (SDES::SetConfiguration) \n");
    }
}

void SDES::Permutation(int &in, PermutationTypes type) {
    std::string bin;
    std::string tmp;
    switch (type) {
        case Type_P10:
            bin = DecToBin(in, Type_P10);
            for (size_t i = 0; i < 10; ++i) {
                tmp += bin[P10[i] - 1];
            }
            in = BinToDec(tmp);
            break;
        case Type_P8:
            bin = DecToBin(in, Type_P8);
            for (size_t i = 0; i < 8; ++i) {
                tmp += bin[P8[i] - 1];
            }
            in = BinToDec(tmp);
            break;
        case Type_IP:
            bin = DecToBin(in, Type_IP);
            for (size_t i = 0; i < 8; ++i) {
                tmp += bin[IP[i] - 1];
            }
            in = BinToDec(tmp);
            break;
        case Type_IP_:
            bin = DecToBin(in, Type_IP_);
            for (size_t i = 0; i < 8; ++i) {
                tmp += bin[IP_[i] - 1];
            }
            in = BinToDec(tmp);
            break;
        case Type_EP:
            bin = DecToBin(in, Type_EP);
            for (size_t i = 0; i < 8; ++i) {
                tmp += bin[EP[i] - 1];
            }
            in = BinToDec(tmp);
            break;
        case Type_P4:
            bin = DecToBin(in, Type_P4);
            for (size_t i = 0; i < 4; ++i) {
                tmp += bin[P4[i] - 1];
            }
            in = BinToDec(tmp);
            break;
        case Type_SW:
            bin = DecToBin(in, Type_SW);
            for (size_t i = 0; i < 8; ++i) {
                tmp += bin[SW[i] - 1];
            }
            in = BinToDec(tmp);
            break;
        case Type_S:
        case Type_P5:
            // not a permutation type
            break;
    }
}

void SDES::Substitution(int &in) {
    std::string left_bin = DecToBin(in >> 4, Type_P4);
    std::string right_bin = DecToBin(in ^ BinToDec(left_bin + "0000"), Type_P4);

    // S0 for left_bin
    int row = BinToDec(std::string() + left_bin[0] + left_bin[3]);
    int col = BinToDec(std::string() + left_bin[1] + left_bin[2]);
    std::string s0 = DecToBin(S0[row][col], Type_S);

    // S1 for right_bin
    row = BinToDec(std::string() + right_bin[0] + right_bin[3]);
    col = BinToDec(std::string() + right_bin[1] + right_bin[2]);
    std::string s1 = DecToBin(S1[row][col], Type_S);

    // return 4 bits combined from s0 and s1
    in = BinToDec(s0 + s1);
}

void SDES::CircularLeftShift(std::string &in, int num_bits) {
    for (int i = 0; i < num_bits; ++i) {
        in.append(std::string() + in[0]);
        in.erase(0, 1);
    }
}

void SDES::GenKey(std::string initial_key) {
    int key = BinToDec(initial_key);
    // P10
    Permutation(key, Type_P10);
    // LS1
    std::string left_bin = DecToBin(key >> 5, Type_P5);
    std::string right_bin = DecToBin(key ^ BinToDec(left_bin + "00000"), Type_P5);
    CircularLeftShift(left_bin, 1);
    CircularLeftShift(right_bin, 1);
    // P8
    k1 = BinToDec(left_bin + right_bin);
    Permutation(k1, Type_P8);

    // LS2
    CircularLeftShift(left_bin, 2);
    CircularLeftShift(right_bin, 2);
    k2 = BinToDec(left_bin + right_bin);
    Permutation(k2, Type_P8);
}

/// helper function for ProcessBlock
void SDES::BlockMainLoop(int &in, int sub_key) {
    std::string left_bin = DecToBin(in >> 4, Type_P4);
    std::string right_bin = DecToBin(in ^ BinToDec(left_bin + "0000"), Type_P4);
    // calculate F(right_bin, sub_key)
    int Fk = BinToDec(right_bin);
    Permutation(Fk, Type_EP);
    // XOR with k1
    Fk = Fk ^ sub_key;
    // S0 and S1
    Substitution(Fk);
    // P4
    Permutation(Fk, Type_P4);
    // left_bin XOR Fk
    Fk = BinToDec(left_bin) ^ Fk;
    // Concat with right_bin
    in = (Fk << 4) | BinToDec(right_bin);
}

void SDES::ProcessBlock(int &in, bool is_encrypt) {
    // reverse order of key pair in decryption
    int key1 = is_encrypt ? k1 : k2;
    int key2 = is_encrypt ? k2 : k1;
    // IP
    Permutation(in, Type_IP);
    // first main loop
    BlockMainLoop(in, key1);
    // SW
    Permutation(in, Type_SW);
    // second main loop
    BlockMainLoop(in, key2);
    // IP-1
    Permutation(in, Type_IP_);
}

void SDES::EncryptECB(std::string in_file_name, std::string out_file_name, std::string key) {
    GenKey(key);
    // open file
    std::fstream in_file(in_file_name, std::ios_base::in | std::ios_base::binary);
    std::fstream out_file(out_file_name, std::ios_base::out | std::ios_base::binary | std::ios_base::trunc);

    char* block = new char;
    char* block_encrypted;
    if (in_file.is_open() && out_file.is_open()) {
        // get length of file:
        in_file.seekg (0, in_file.end);
        long length = in_file.tellg();
        in_file.seekg (0, in_file.beg);

        int out;
        for (long i = 0; i < length; ++i) {
            // read every 8 bits (1 char)
            in_file.read(block, 1);
            // encrypt using SDES in ECB mode
            out = *block;
            ProcessBlock(out, true);
            // write to output
            char out_block = static_cast<char>(out);
            block_encrypted = (&out_block);
            out_file.write(block_encrypted, 1);
        }

        // clean up
//        delete block;
//        delete block_encrypted;
        in_file.close();
        out_file.close();
    }
    else {
        printf("Error: Cannot open input file (SDES::EncryptECB) \n");
    }
}

void SDES::DecryptECB(std::string in_file_name, std::string out_file_name, std::string key) {
    GenKey(key);
    // open file
    std::fstream in_file(in_file_name, std::ios_base::in | std::ios_base::binary);
    std::fstream out_file(out_file_name, std::ios_base::out | std::ios_base::binary | std::ios_base::trunc);

    char* block = new char;
    char* block_encrypted;
    if (in_file.is_open() && out_file.is_open()) {
        // get length of file:
        in_file.seekg (0, in_file.end);
        long length = in_file.tellg();
        in_file.seekg (0, in_file.beg);

        int out;
        for (long i = 0; i < length; ++i) {
            // read every 8 bits (1 char)
            in_file.read(block, 1);
            // encrypt using SDES in ECB mode
            out = *block;
            ProcessBlock(out, false);
            // write to output
            char out_block = static_cast<char>(out);
            block_encrypted = (&out_block);
            out_file.write(block_encrypted, 1);
        }

        // clean up
//        delete block;
//        delete block_encrypted;
        in_file.close();
        out_file.close();
    }
    else {
        printf("Error: Cannot open input file (SDES::EncryptECB) \n");
    }
}

std::string SDES::GenInitVector() {
    srand((unsigned int) time(NULL));
    std::__cxx11::string res = "";
    for (int i = 0; i < 8; ++i) {
        res += std::to_string(rand() % 2);
    }
    return res;
}

void SDES::EncryptCBC(std::string in_file_name, std::string out_file_name, std::string key, std::string init_vector) {
    GenKey(key);
    // open file
    std::fstream in_file(in_file_name, std::ios_base::in | std::ios_base::binary);
    std::fstream out_file(out_file_name, std::ios_base::out | std::ios_base::binary | std::ios_base::trunc);

    char* block = new char;
    char* block_encrypted;
    if (in_file.is_open() && out_file.is_open()) {
        // get length of file:
        in_file.seekg (0, in_file.end);
        long length = in_file.tellg();
        in_file.seekg (0, in_file.beg);

        int out;
        int previous_cipher = BinToDec(init_vector);
        for (long i = 0; i < length; ++i) {
            // read every 8 bits (1 char)
            in_file.read(block, 1);
            // encrypt using SDES in CBC mode
            out = *block;
            out = out ^ previous_cipher;
            ProcessBlock(out, true);
            previous_cipher = out;
            // write to output
            char out_block = static_cast<char>(out);
            block_encrypted = (&out_block);
            out_file.write(block_encrypted, 1);
        }

        // clean up
//        delete block;
//        delete block_encrypted;
        in_file.close();
        out_file.close();
    }
    else {
        printf("Error: Cannot open input file (SDES::EncryptECB) \n");
    }
}

void SDES::DecryptCBC(std::string in_file_name, std::string out_file_name, std::string key, std::string init_vector) {
    GenKey(key);
    // open file
    std::fstream in_file(in_file_name, std::ios_base::in | std::ios_base::binary);
    std::fstream out_file(out_file_name, std::ios_base::out | std::ios_base::binary | std::ios_base::trunc);

    char* block = new char;
    char* block_encrypted;
    if (in_file.is_open() && out_file.is_open()) {
        // get length of file:
        in_file.seekg (0, in_file.end);
        long length = in_file.tellg();
        in_file.seekg (0, in_file.beg);

        int out;
        int previous_cipher = BinToDec(init_vector);
        for (long i = 0; i < length; ++i) {
            // read every 8 bits (1 char)
            in_file.read(block, 1);
            // decrypt using SDES in CBC mode
            out = *block;
            ProcessBlock(out, false);
            out = out ^ previous_cipher;
            previous_cipher = *block;
            // write to output
            char out_block = static_cast<char>(out);
            block_encrypted = (&out_block);
            out_file.write(block_encrypted, 1);
        }

        // clean up
//        delete block;
//        delete block_encrypted;
        in_file.close();
        out_file.close();
    }
    else {
        printf("Error: Cannot open input file (SDES::EncryptECB) \n");
    }
}

SDES::SDES() {

}

SDES::~SDES() {

}

SDES::SDES(std::string configuration_file) {
    SetConfiguration(configuration_file);
}
