#pragma once

#include "core/IEncryptor.h"
#include "Prerequisites.h"

/**
 * @class XorEncryptor
 * @brief Implementación del cifrado XOR.
 *
 * Este cifrador aplica una operación XOR byte a byte entre el contenido
 * del archivo y una clave proporcionada. Es un cifrado de flujo simétrico,
 * lo que significa que la misma función sirve para cifrar y descifrar.
 */
class XorEncryptor : public IEncryptor {
public:
    bool encrypt(const std::filesystem::path& inputFile, const std::filesystem::path& outputFile, const std::string& key) const override {
        return processFile(inputFile, outputFile, key);
    }

    bool decrypt(const std::filesystem::path& inputFile, const std::filesystem::path& outputFile, const std::string& key) const override {
        // La operación de cifrado y descifrado XOR es idéntica.
        return processFile(inputFile, outputFile, key);
    }

private:
    /**
     * @brief Procesa un archivo aplicando el cifrado XOR.
     * @param inputFile Ruta al archivo de entrada.
     * @param outputFile Ruta al archivo de salida.
     * @param key La clave a utilizar para la operación XOR.
     * @return true si la operación fue exitosa, false en caso contrario.
     */
    bool processFile(const std::filesystem::path& inputFile, const std::filesystem::path& outputFile, const std::string& key) const {
        if (key.empty()) {
            std::cerr << "Error (XOR): La clave no puede estar vacía.\n";
            return false;
        }

        std::ifstream inFile(inputFile, std::ios::binary);
        if (!inFile) {
            std::cerr << "Error (XOR): No se pudo abrir el archivo de entrada '" << inputFile << "'\n";
            return false;
        }

        std::ofstream outFile(outputFile, std::ios::binary | std::ios::trunc);
        if (!outFile) {
            std::cerr << "Error (XOR): No se pudo crear el archivo de salida '" << outputFile << "'\n";
            return false;
        }

        char byte;
        size_t keyIndex = 0;
        while (inFile.get(byte)) {
            outFile.put(byte ^ key[keyIndex % key.length()]);
            keyIndex++;
        }

        return true;
    }
};
