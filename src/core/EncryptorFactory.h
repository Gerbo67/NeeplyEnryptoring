#pragma once

#include <memory>
#include "core/IEncryptor.h"
#include "ciphers/XorEncryptor.h"
#include "ciphers/DesEncryptor.h"

/**
 * @class EncryptorFactory
 * @brief Crea instancias de los diferentes algoritmos de cifrado.
 * Este es un patrón de diseño (Factory) que nos ayuda a desacoplar el código.
 */
class EncryptorFactory {
public:
    enum class Type {
        XOR,
        DES
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
        default:
            return nullptr;
        }
    }
};
