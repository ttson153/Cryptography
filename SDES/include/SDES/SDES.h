//
// Created by tts on 2/18/17.
//

#ifndef SDES_SDES_H
#define SDES_SDES_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stdlib.h>
#include <bitset>

#define VERBOSE_MODE 0

enum PermutationTypes {
    Type_P10,
    Type_P8,
    Type_IP,
    Type_IP_,
    Type_EP,
    Type_P4,
    Type_P5,
    Type_S,
    Type_SW,
};

class SDES {
public:
    // variables
    char P10[10];
    char P8[8];
    char IP[8];
    char IP_[8];
    char EP[8];
    char P4[4];
    char SW[8] = {5, 6, 7, 8, 1, 2, 3, 4};
    char S0[4][4];
    char S1[4][4];
    int k1;
    int k2;
    // output to console?
    bool is_verbose;

    // conversion function
    static int BinToDec(std::string bin_str);
    static std::string DecToBin(int dec, PermutationTypes type);

    // helper functions
    void Permutation(int &in, PermutationTypes type);
    void Substitution(int &in);
    void CircularLeftShift(std::string &in, int num_bits);
    void BlockMainLoop(int &in, int sub_key);
    void ProcessBlock(int &in, bool is_encrypt);

    // SDES functions
    void GenKey(std::string initial_key);
    std::string GenInitVector();

public:
    SDES();
    SDES(std::string configuration_file);
    void SetConfiguration(std::string file_name);
    ~SDES();

    void SetVerbose(bool enabled) { is_verbose = enabled; }
    void EncryptECB(std::string in_file_name, std::string out_file_name, std::string key);
    void DecryptECB(std::string in_file_name, std::string out_file_name, std::string key);
    void EncryptCBC(std::string in_file_name, std::string out_file_name, std::string key, std::string init_vector);
    void DecryptCBC(std::string in_file_name, std::string out_file_name, std::string key, std::string init_vector);
};

#endif //SDES_SDES_H
