#pragma once

#include <memory>

#include "core/IEncryptor.h"

#include "ciphers/BinaryEncryptor.h"
#include "ciphers/XorEncryptor.h"
#include "ciphers/DesEncryptor.h"
#include "ciphers/CesarEncryptor .h"
#include "ciphers/VeginereEncryptor.h"


/**
 * @class EncryptorFactory
 * @brief Crea instancias de los diferentes algoritmos de cifrado.
 * Este es un patrón de diseño (Factory) que nos ayuda a desacoplar el código.
 */
class EncryptorFactory {
public:
    // Agregamos los nuevos tipos de cifrado
    enum class Type {
        XOR,
        DES,
        CESAR,
        VIGENERE,
        BINARY
    };

    /**
     * @brief Crea y devuelve un puntero único al encriptador solicitado.
     * @param type El tipo de encriptador a crear.
     * @return Un std::unique_ptr<IEncryptor> del tipo solicitado.
     */
    static std::unique_ptr<IEncryptor> createEncryptor(Type type) {
        switch (type) {
        case Type::XOR:
            return std::make_unique<XorEncryptor>();
        case Type::DES:
            return std::make_unique<DesEncryptor>();
        case Type::CESAR:
            return std::make_unique<CesarFileEncryptor>();
        case Type::VIGENERE:
            return std::make_unique<VigenereFileEncryptor>();
        case Type::BINARY:
            return std::make_unique<BinaryEncryptor>();
        default:
            return nullptr;
        }
    }
};