#pragma once

#include "../core/IEncryptor.h"
#include "Prerequisites.h"


/**
 * @class VigenereFileEncryptor
 * @brief Implementación del cifrado Vigenère para archivos, conforme a la interfaz IEncryptor.
 *
 * Utiliza una clave de texto para cifrar el contenido de un archivo byte a byte.
 * Los caracteres no alfabéticos en la clave son ignorados.
 */
class VigenereFileEncryptor : public IEncryptor {
public:
    bool encrypt(const std::filesystem::path& inputFile, const std::filesystem::path& outputFile, const std::string& key) const override;
    bool decrypt(const std::filesystem::path& inputFile, const std::filesystem::path& outputFile, const std::string& key) const override;

private:
    /**
     * @brief Normaliza la clave, eliminando caracteres no alfabéticos y convirtiéndola a mayúsculas.
     * @param rawKey La clave original.
     * @return La clave normalizada.
     */
    std::string normalizeKey(const std::string& rawKey) const {
        std::string normalized;
        for (char c : rawKey) {
            if (std::isalpha(static_cast<unsigned char>(c))) {
                normalized += std::toupper(static_cast<unsigned char>(c));
            }
        }
        return normalized;
    }

    /**
     * @brief Procesa el archivo aplicando el cifrado Vigenère.
     * @param inputFile Archivo de entrada.
     * @param outputFile Archivo de salida.
     * @param key La clave de cifrado.
     * @param encrypt_mode Si es true, cifra. Si es false, descifra.
     * @return true si la operación tuvo éxito, false en caso contrario.
     */
    bool processFile(const std::filesystem::path& inputFile, const std::filesystem::path& outputFile, const std::string& key, bool encrypt_mode) const {
        std::string processed_key = normalizeKey(key);
        if (processed_key.empty()) {
            std::cerr << "Error (Vigenere): La clave no puede estar vacía o no contener letras.\n";
            return false;
        }

        std::ifstream inFile(inputFile, std::ios::binary);
        if (!inFile) {
            std::cerr << "Error (Vigenere): No se pudo abrir el archivo de entrada '" << inputFile << "'\n";
            return false;
        }

        std::ofstream outFile(outputFile, std::ios::binary | std::ios::trunc);
        if (!outFile) {
            std::cerr << "Error (Vigenere): No se pudo crear el archivo de salida '" << outputFile << "'\n";
            return false;
        }

        char byte;
        size_t key_index = 0;
        while (inFile.get(byte)) {
            int shift = processed_key[key_index % processed_key.length()] - 'A';
            if (!encrypt_mode) {
                shift = -shift;
            }
            outFile.put(static_cast<char>(static_cast<unsigned char>(byte) + shift));
            key_index++;
        }
        return true;
    }
};

inline bool VigenereFileEncryptor::encrypt(const std::filesystem::path& inputFile, const std::filesystem::path& outputFile, const std::string& key) const {
    return processFile(inputFile, outputFile, key, true);
}

inline bool VigenereFileEncryptor::decrypt(const std::filesystem::path& inputFile, const std::filesystem::path& outputFile, const std::string& key) const {
    return processFile(inputFile, outputFile, key, false);
}