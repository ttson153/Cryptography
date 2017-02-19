#include <iostream>
#include "SDES/SDES.h"

int main(int argc, char* argv[]) {
    std::string in_file_name = "";
    std::string out_file_name = "";
    std::string config_file_name = "";
    std::string key = "";
    std::string init_vector = "";
    bool is_encrypt = false;
    bool is_decrypt = false;
    bool use_ECB = false;
    bool use_CBC = false;

    if ( argc < 10 ) {
        std::cout << "usage: " << argv[0] <<
                  " [-e] [-d] [-CBC] [-ECB] [-c configuration file]\n"
                  " [-i input] [-o output] [-k key] [-iv initial vector]\n";
    }
    else {
        // parsing arguments
        for (int i = 1; i < argc; ++i) {
            if (std::string(argv[i]) == "-i") {
                if (i + 1 < argc) {
                    in_file_name = std::string(argv[++i]);
                }
            }
            if (std::string(argv[i]) == "-o") {
                if (i + 1 < argc) {
                    out_file_name = std::string(argv[++i]);
                }
            }
            if (std::string(argv[i]) == "-k") {
                if (i + 1 < argc) {
                    key = std::string(argv[++i]);
                }
            }
            if (std::string(argv[i]) == "-iv") {
                if (i + 1 < argc) {
                    init_vector = std::string(argv[++i]);
                }
            }
            if (std::string(argv[i]) == "-c") {
                if (i + 1 < argc) {
                    config_file_name = std::string(argv[++i]);
                }
            }
            if (std::string(argv[i]) == "-e") is_encrypt = true;
            if (std::string(argv[i]) == "-d") is_decrypt = true;
            if (std::string(argv[i]) == "-CBC") use_CBC = true;
            if (std::string(argv[i]) == "-ECB") use_ECB = true;
        }

        SDES sdes;
        sdes.SetConfiguration(config_file_name);
        if (is_encrypt) {
            if (use_CBC) {
                printf("SDES encrypt CBC mode\n");
                init_vector = sdes.GenInitVector();
                std::cout << "Initial vector: " << init_vector << std::endl;
                sdes.EncryptCBC(in_file_name, out_file_name, key, init_vector);
            } else {
                printf("SDES encrypt ECB mode\n");
                sdes.EncryptECB(in_file_name, out_file_name, key);
            }
        }
        else {
            if (use_CBC) {
                printf("SDES decrypt CBC mode\n");
                std::cout << "Initial vector: " << init_vector << std::endl;
                sdes.DecryptCBC(in_file_name, out_file_name, key, init_vector);
            } else {
                printf("SDES decrypt ECB mode\n");
                sdes.DecryptECB(in_file_name, out_file_name, key);
            }
        }
    }
    return 0;
}