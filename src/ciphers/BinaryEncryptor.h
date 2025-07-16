#pragma once

#include "../core/IEncryptor.h"
#include "Prerequisites.h"

/**
 * @class BinaryEncoder
 * @brief Implementa una codificación de archivos a texto binario.
 *
 * No es un cifrado real ya que no utiliza una clave. Simplemente transforma
 * cada byte del archivo en su representación de 8 bits en formato de texto.
 */
class BinaryEncryptor : public IEncryptor
{
public:
    /**
     * @brief Codifica un archivo a su representación de texto binario.
     * La clave es ignorada en esta implementación.
     */
    bool encrypt(const std::filesystem::path& inputFile, const std::filesystem::path& outputFile,
                 const std::string& key) const override
    {
        (void)key;

        std::ifstream inFile(inputFile, std::ios::binary);
        if (!inFile)
        {
            std::cerr << "Error (Binary): No se pudo abrir el archivo de entrada.\n";
            return false;
        }

        std::ofstream outFile(outputFile, std::ios::trunc);
        if (!outFile)
        {
            std::cerr << "Error (Binary): No se pudo crear el archivo de salida.\n";
            return false;
        }

        char byte;
        bool first_byte = true;
        while (inFile.get(byte))
        {
            if (!first_byte)
            {
                outFile << ' ';
            }
            outFile << std::bitset<8>(static_cast<unsigned char>(byte)).to_string();
            first_byte = false;
        }

        return true;
    }

    /**
     * @brief Decodifica un archivo de texto binario a su contenido original.
     * La clave es ignorada en esta implementación.
     */
    bool decrypt(const std::filesystem::path& inputFile, const std::filesystem::path& outputFile,
                 const std::string& key) const override
    {
        (void)key;

        std::ifstream inFile(inputFile);
        if (!inFile)
        {
            std::cerr << "Error (Binary): No se pudo abrir el archivo de entrada.\n";
            return false;
        }

        std::ofstream outFile(outputFile, std::ios::binary | std::ios::trunc);
        if (!outFile)
        {
            std::cerr << "Error (Binary): No se pudo crear el archivo de salida.\n";
            return false;
        }

        std::string binary_chunk;
        while (inFile >> binary_chunk)
        {
            if (binary_chunk.length() == 8)
            {
                try
                {
                    unsigned long ulong = std::bitset<8>(binary_chunk).to_ulong();
                    outFile.put(static_cast<char>(ulong));
                }
                catch (const std::invalid_argument& e)
                {
                    std::cerr << "Error (Binary): Se encontró un trozo no válido en el archivo: " << binary_chunk <<
                        std::endl;
                }
            }
        }

        return true;
    }
};
