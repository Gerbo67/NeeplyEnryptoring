#pragma once

#include "core/IEncryptor.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <bitset>
#include <array>

namespace DES_Internal {
    // Tablas estándar de DES
    const int IP[] = { 58, 50, 42, 34, 26, 18, 10, 2, 60, 52, 44, 36, 28, 20, 12, 4,
                       62, 54, 46, 38, 30, 22, 14, 6, 64, 56, 48, 40, 32, 24, 16, 8,
                       57, 49, 41, 33, 25, 17, 9,  1, 59, 51, 43, 35, 27, 19, 11, 3,
                       61, 53, 45, 37, 29, 21, 13, 5, 63, 55, 47, 39, 31, 23, 15, 7 };

    const int FP[] = { 40, 8, 48, 16, 56, 24, 64, 32, 39, 7, 47, 15, 55, 23, 63, 31,
                       38, 6, 46, 14, 54, 22, 62, 30, 37, 5, 45, 13, 53, 21, 61, 29,
                       36, 4, 44, 12, 52, 20, 60, 28, 35, 3, 43, 11, 51, 19, 59, 27,
                       34, 2, 42, 10, 50, 18, 58, 26, 33, 1, 41, 9, 49, 17, 57, 25 };

    const int E[] = { 32, 1,  2,  3,  4,  5,  4,  5,  6,  7,  8,  9,
                      8,  9,  10, 11, 12, 13, 12, 13, 14, 15, 16, 17,
                      16, 17, 18, 19, 20, 21, 20, 21, 22, 23, 24, 25,
                      24, 25, 26, 27, 28, 29, 28, 29, 30, 31, 32, 1 };

    const int P[] = { 16, 7,  20, 21, 29, 12, 28, 17, 1,  15, 23, 26, 5,  18, 31, 10,
                      2,  8,  24, 14, 32, 27, 3,  9,  19, 13, 30, 6,  22, 11, 4,  25 };

    const int S_BOX[8][4][16] = {
        {{14,4,13,1,2,15,11,8,3,10,6,12,5,9,0,7},{0,15,7,4,14,2,13,1,10,6,12,11,9,5,3,8},{4,1,14,8,13,6,2,11,15,12,9,7,3,10,5,0},{15,12,8,2,4,9,1,7,5,11,3,14,10,0,6,13}},
        {{15,1,8,14,6,11,3,4,9,7,2,13,12,0,5,10},{3,13,4,7,15,2,8,14,12,0,1,10,6,9,11,5},{0,14,7,11,10,4,13,1,5,8,12,6,9,3,2,15},{13,8,10,1,3,15,4,2,11,6,7,12,0,5,14,9}},
        {{10,0,9,14,6,3,15,5,1,13,12,7,11,4,2,8},{13,7,0,9,3,4,6,10,2,8,5,14,12,11,15,1},{13,6,4,9,8,15,3,0,11,1,2,12,5,10,14,7},{1,10,13,0,6,9,8,7,4,15,14,3,11,5,2,12}},
        {{7,13,14,3,0,6,9,10,1,2,8,5,11,12,4,15},{13,8,11,5,6,15,0,3,4,7,2,12,1,10,14,9},{10,6,9,0,12,11,7,13,15,1,3,14,5,2,8,4},{3,15,0,6,10,1,13,8,9,4,5,11,12,7,2,14}},
        {{2,12,4,1,7,10,11,6,8,5,3,15,13,0,14,9},{14,11,2,12,4,7,13,1,5,0,15,10,3,9,8,6},{4,2,1,11,10,13,7,8,15,9,12,5,6,3,0,14},{11,8,12,7,1,14,2,13,6,15,0,9,10,4,5,3}},
        {{12,1,10,15,9,2,6,8,0,13,3,4,14,7,5,11},{10,15,4,2,7,12,9,5,6,1,13,14,0,11,3,8},{9,14,15,5,2,8,12,3,7,0,4,10,1,13,11,6},{4,3,2,12,9,5,15,10,11,14,1,7,6,0,8,13}},
        {{4,11,2,14,15,0,8,13,3,12,9,7,5,10,6,1},{13,0,11,7,4,9,1,10,14,3,5,12,2,15,8,6},{1,4,11,13,12,3,7,14,10,15,6,8,0,5,9,2},{6,11,13,8,1,4,10,7,9,5,0,15,14,2,3,12}},
        {{13,2,8,4,6,15,11,1,10,9,3,14,5,0,12,7},{1,15,13,8,10,3,7,4,12,5,6,11,0,14,9,2},{7,11,4,1,9,12,14,2,0,6,10,13,15,3,5,8},{2,1,14,7,4,10,8,13,15,12,9,0,3,5,6,11}}
    };

    const int PC1[] = { 57, 49, 41, 33, 25, 17, 9,  1, 58, 50, 42, 34, 26, 18,
                        10, 2,  59, 51, 43, 35, 27, 19, 11, 3,  60, 52, 44, 36,
                        63, 55, 47, 39, 31, 23, 15, 7,  62, 54, 46, 38, 30, 22,
                        14, 6,  61, 53, 45, 37, 29, 21, 13, 5,  28, 20, 12, 4 };

    const int PC2[] = { 14, 17, 11, 24, 1,  5,  3,  28, 15, 6,  21, 10,
                        23, 19, 12, 4,  26, 8,  16, 7,  27, 20, 13, 2,
                        41, 52, 31, 37, 47, 55, 30, 40, 51, 45, 33, 48,
                        44, 49, 39, 56, 34, 53, 46, 42, 50, 36, 29, 32 };

    const int SHIFTS[] = { 1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1 };

    // Función para permutar bits
    template<size_t N_IN, size_t N_OUT>
    std::bitset<N_OUT> permute(const std::bitset<N_IN>& input, const int table[N_OUT]) {
        std::bitset<N_OUT> output;
        for (size_t i = 0; i < N_OUT; ++i) {
            output[N_OUT - 1 - i] = input[N_IN - table[i]];
        }
        return output;
    }

    // Función para hacer un left shift circular
    template<size_t N>
    std::bitset<N> left_shift(std::bitset<N> bs, int amount) {
        return (bs << amount) | (bs >> (N - amount));
    }
}

class DesEncryptor : public IEncryptor {
public:
    bool encrypt(const std::filesystem::path& inputFile, const std::filesystem::path& outputFile, const std::string& key) const override {
        return processFile(inputFile, outputFile, key, true);
    }

    bool decrypt(const std::filesystem::path& inputFile, const std::filesystem::path& outputFile, const std::string& key) const override {
        return processFile(inputFile, outputFile, key, false);
    }

private:
    // Deriva una clave de 64 bits a partir de una contraseña de texto.
    std::bitset<64> deriveKey_manual(const std::string& password) const {
        std::array<unsigned char, 8> key_bytes{}; // Inicializa a ceros
        for (size_t i = 0; i < password.length(); ++i) {
            key_bytes[i % 8] ^= password[i];
        }
        uint64_t key_val = 0;
        for (int i = 0; i < 8; ++i) {
            key_val |= (uint64_t)key_bytes[i] << (56 - i * 8);
        }
        return std::bitset<64>(key_val);
    }

    // Genera las 16 subclaves de 48 bits
    std::vector<std::bitset<48>> generate_subkeys(const std::bitset<64>& main_key) const {
        std::vector<std::bitset<48>> subkeys;
        std::bitset<56> pc1_key = DES_Internal::permute<64, 56>(main_key, DES_Internal::PC1);

        std::bitset<28> C;
        std::bitset<28> D;
        for(int i=0; i<28; ++i) {
            C[27-i] = pc1_key[55-i];
            D[27-i] = pc1_key[27-i];
        }

        for (int round = 0; round < 16; ++round) {
            C = DES_Internal::left_shift<28>(C, DES_Internal::SHIFTS[round]);
            D = DES_Internal::left_shift<28>(D, DES_Internal::SHIFTS[round]);

            std::bitset<56> combined_key;
            for(int i=0; i<28; ++i) {
                combined_key[55-i] = C[27-i];
                combined_key[27-i] = D[27-i];
            }
            subkeys.push_back(DES_Internal::permute<56, 48>(combined_key, DES_Internal::PC2));
        }
        return subkeys;
    }

    // La función F de Feistel
    std::bitset<32> feistel_function(std::bitset<32> R, const std::bitset<48>& subkey) const {
        std::bitset<48> expanded_R = DES_Internal::permute<32, 48>(R, DES_Internal::E);
        expanded_R ^= subkey;

        std::bitset<32> output;
        for (int i = 0; i < 8; ++i) {
            int row = (expanded_R[47 - i*6] << 1) + expanded_R[42 - i*6];
            int col = (expanded_R[46 - i*6] << 3) + (expanded_R[45 - i*6] << 2) + (expanded_R[44 - i*6] << 1) + expanded_R[43 - i*6];

            int s_val = DES_Internal::S_BOX[i][row][col];
            std::bitset<4> s_bits(s_val);

            for(int j=0; j<4; ++j) {
                output[31-i*4-j] = s_bits[3-j];
            }
        }
        return DES_Internal::permute<32, 32>(output, DES_Internal::P);
    }

    // Procesa un único bloque de 64 bits
    std::bitset<64> process_block(std::bitset<64> block, const std::vector<std::bitset<48>>& subkeys, bool is_encrypt) const {
        block = DES_Internal::permute<64, 64>(block, DES_Internal::IP);

        std::bitset<32> L, R;
        for(int i=0; i<32; ++i) {
            L[31-i] = block[63-i];
            R[31-i] = block[31-i];
        }

        for (int round = 0; round < 16; ++round) {
            std::bitset<32> temp_L = L;
            L = R;
            int subkey_index = is_encrypt ? round : 15 - round;
            R = temp_L ^ feistel_function(R, subkeys[subkey_index]);
        }

        std::bitset<64> combined;
        for(int i=0; i<32; ++i) {
            combined[63-i] = R[31-i];
            combined[31-i] = L[31-i];
        }

        return DES_Internal::permute<64, 64>(combined, DES_Internal::FP);
    }

    // Convierte un buffer de 8 chars a bitset
    std::bitset<64> bufferToBitset(const std::array<char, 8>& buffer) const {
        uint64_t val = 0;
        for(int i=0; i<8; ++i) {
            val |= (uint64_t)(unsigned char)buffer[i] << (56 - i*8);
        }
        return std::bitset<64>(val);
    }

    // Convierte un bitset a un array de 8 chars
    std::array<char, 8> bitsetToBuffer(const std::bitset<64>& bits) const {
        std::array<char, 8> buffer;
        uint64_t val = bits.to_ullong();
        for(int i=0; i<8; ++i) {
            buffer[i] = (val >> (56 - i*8)) & 0xFF;
        }
        return buffer;
    }

    // Lógica principal para procesar archivos
    bool processFile(const std::filesystem::path& inputFile, const std::filesystem::path& outputFile, const std::string& password, bool is_encrypt) const {
        auto main_key = deriveKey_manual(password);
        auto subkeys = generate_subkeys(main_key);

        std::ifstream inFile(inputFile, std::ios::binary);
        if (!inFile) { return false; }

        std::ofstream outFile(outputFile, std::ios::binary | std::ios::trunc);
        if (!outFile) { return false; }

        std::array<char, 8> buffer;
        if (is_encrypt) {
            while (inFile.read(buffer.data(), 8)) {
                auto block = bufferToBitset(buffer);
                auto processed_block = process_block(block, subkeys, true);
                outFile.write(bitsetToBuffer(processed_block).data(), 8);
            }

            // Padding PKCS#7
            int bytesRead = inFile.gcount();
            char padding_val = 8 - bytesRead;
            std::fill(buffer.begin() + bytesRead, buffer.end(), padding_val);
            auto block = bufferToBitset(buffer);
            auto processed_block = process_block(block, subkeys, true);
            outFile.write(bitsetToBuffer(processed_block).data(), 8);

        } else { // Decrypt
            std::vector<char> file_contents((std::istreambuf_iterator<char>(inFile)), std::istreambuf_iterator<char>());
            if (file_contents.size() % 8 != 0 || file_contents.empty()) {
                std::cerr << "Error (DES): El archivo cifrado tiene un tamaño incorrecto." << std::endl;
                return false;
            }

            for(size_t i = 0; i < file_contents.size(); i += 8) {
                std::copy(file_contents.begin() + i, file_contents.begin() + i + 8, buffer.begin());
                auto block = bufferToBitset(buffer);
                auto processed_block = process_block(block, subkeys, false);

                if (i + 8 == file_contents.size()) {
                    auto final_buffer = bitsetToBuffer(processed_block);
                    char padding_val = final_buffer[7];
                    if (padding_val > 0 && padding_val <= 8) {
                        outFile.write(final_buffer.data(), 8 - padding_val);
                    } else { // Padding inválido
                        outFile.write(final_buffer.data(), 8);
                    }
                } else {
                    outFile.write(bitsetToBuffer(processed_block).data(), 8);
                }
            }
        }
        return true;
    }
};
