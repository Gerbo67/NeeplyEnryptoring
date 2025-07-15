#pragma once

#include <string>
#include <filesystem>

/**
 * @class IEncryptor
 * @brief Interfaz abstracta para todos los algoritmos de cifrado.
 */
class IEncryptor {
public:
    virtual ~IEncryptor() = default;

    /**
     * @brief Cifra un archivo.
     */
    virtual bool encrypt(const std::filesystem::path& inputFile, const std::filesystem::path& outputFile, const std::string& key) const = 0;

    /**
     * @brief Descifra un archivo.
     */
    virtual bool decrypt(const std::filesystem::path& inputFile, const std::filesystem::path& outputFile, const std::string& key) const = 0;
};
