#pragma once

#include "../core/IEncryptor.h"
#include "Prerequisites.h"

/**
 * @class CesarFileEncryptor
 * @brief Implementación del cifrado César para archivos, conforme a la interfaz IEncryptor.
 *
 * Aplica un desplazamiento a cada byte del archivo. La clave debe ser un número entero
 * representado como una cadena de texto.
 */
class CesarFileEncryptor : public IEncryptor {
public:
    bool encrypt(const std::filesystem::path& inputFile, const std::filesystem::path& outputFile, const std::string& key) const override;
    bool decrypt(const std::filesystem::path& inputFile, const std::filesystem::path& outputFile, const std::string& key) const override;

private:
    /**
     * @brief Procesa el archivo aplicando el desplazamiento César.
     * @param inputFile Archivo de entrada.
     * @param outputFile Archivo de salida.
     * @param key La clave (un número como string).
     * @param encrypt_mode Si es true, cifra. Si es false, descifra.
     * @return true si la operación tuvo éxito, false en caso contrario.
     */
    bool processFile(const std::filesystem::path& inputFile, const std::filesystem::path& outputFile, const std::string& key, bool encrypt_mode) const {
        int shift;
        try {
            shift = std::stoi(key);
        } catch (const std::invalid_argument& e) {
            std::cerr << "Error (Cesar): La clave debe ser un número entero. Valor recibido: " << key << std::endl;
            return false;
        }

        std::ifstream inFile(inputFile, std::ios::binary);
        if (!inFile) {
            std::cerr << "Error (Cesar): No se pudo abrir el archivo de entrada '" << inputFile << "'\n";
            return false;
        }

        std::ofstream outFile(outputFile, std::ios::binary | std::ios::trunc);
        if (!outFile) {
            std::cerr << "Error (Cesar): No se pudo crear el archivo de salida '" << outputFile << "'\n";
            return false;
        }

        int final_shift = encrypt_mode ? shift : -shift;

        char byte;
        while (inFile.get(byte)) {
            outFile.put(static_cast<char>(static_cast<unsigned char>(byte) + final_shift));
        }

        return true;
    }
};

inline bool CesarFileEncryptor::encrypt(const std::filesystem::path& inputFile, const std::filesystem::path& outputFile, const std::string& key) const {
    return processFile(inputFile, outputFile, key, true);
}

inline bool CesarFileEncryptor::decrypt(const std::filesystem::path& inputFile, const std::filesystem::path& outputFile, const std::string& key) const {
    return processFile(inputFile, outputFile, key, false);
}