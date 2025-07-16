# Versión mínima de CMake requerida.
cmake_minimum_required(VERSION 3.20)

# Definición del proyecto.
project(NeeplyEnryptoring LANGUAGES CXX)

# --- ESTÁNDAR C++20 ---
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# --- CORRECCIÓN PARA DISTRIBUCIÓN EN WINDOWS ---
# Si estamos usando el compilador MinGW (común en Windows con CLion),
# añadimos la bandera '-static'. Esto fuerza al enlazador a usar bibliotecas
# estáticas siempre que sea posible, creando un .exe independiente que no
# necesita archivos DLL externos y resuelve problemas como el error 0xc000007b.
if(MINGW)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static")
endif()

# --- Búsqueda de Dependencias ---
# Nota: Para que '-static' funcione correctamente con vcpkg, es posible que
# necesites tener instalada la versión estática de la biblioteca, por ejemplo:
# vcpkg install ftxui:x64-mingw-static
find_package(ftxui CONFIG REQUIRED)

# --- Definición del Ejecutable ---
add_executable(NeeplyEnryptoring src/main.cpp
        "src/ciphers/CesarEncryptor .h"
        src/ciphers/VeginereEncryptor.h
        src/ciphers/Prerequisites.h
        src/ciphers/BinaryEncryptor.h)

# Agregamos 'src' como un directorio de búsqueda para los headers.
target_include_directories(NeeplyEnryptoring PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)

# --- Enlazado de Bibliotecas ---
# Enlazamos nuestro ejecutable con FTXUI.
target_link_libraries(NeeplyEnryptoring PRIVATE ftxui::component)

# Mensaje final para el usuario.
message(STATUS "Proyecto configurado. Para compilar, crea un directorio 'build', entra en él y ejecuta 'cmake ..' y luego 'cmake --build .'")
