#pragma once

#include "ftxui/component/component.hpp"
#include "ftxui/component/screen_interactive.hpp"
#include "ftxui/dom/elements.hpp"

#include "core/EncryptorFactory.h"

namespace fs = std::filesystem;

class Tui
{
public:
    Tui()
    {
        try
        {
            fs::create_directory(work_dir_);
        }
        catch (const fs::filesystem_error& e)
        {
            status_message_ = "Error CRITICO al crear directorio 'Enviroments': " + std::string(e.what());
        }
        ScanFiles();
    }

    void Run()
    {
        using namespace ftxui;

        std::string placeholder_text = "Escribe tu clave aquí";
        auto password_input = Input(&password_, &placeholder_text);


        const std::vector<std::string> password_placeholders = {
            "Cualquier caracter (ej: pass123)", // XOR
            "Cualquier caracter (ej: mi_clave)", // DES
            "Solo números (ej: 13)", // Cesar
            "Solo letras (ej: SECRETO)", // Vigenere
            "No requiere clave" // Binary
        };

        auto encrypt_button = Button(" Cifrar ", [&] { ProcessFiles(true); }, ButtonOption::Animated(Color::Green));
        auto decrypt_button = Button(" Descifrar ", [&] { ProcessFiles(false); }, ButtonOption::Animated(Color::Red));
        auto refresh_button = Button(" Refrescar ", [&] { ScanFiles(); }, ButtonOption::Animated(Color::Blue));

        std::vector<std::string> algorithm_entries = {
            "XOR", "DES", "Cesar", "Vigenere", "Codificación Binaria"
        };
        auto algorithm_radiobox = Radiobox(&algorithm_entries, &selected_algorithm_);

        auto main_container = Container::Vertical({
            password_input,
            algorithm_radiobox,
            encrypt_button,
            decrypt_button,
            refresh_button
        });

        auto final_component = Renderer(main_container, [&]
        {
            placeholder_text = password_placeholders[selected_algorithm_];

            auto password_renderer = password_input->Render();
            if (selected_algorithm_ == 4)
            {
                password_renderer = password_renderer | dim;
            }

            main_container->DetachAllChildren();
            auto txt_container = Container::Vertical({});
            for (size_t i = 0; i < txt_files_.size(); ++i)
            {
                txt_container->Add(Checkbox(txt_files_[i].filename().string(), &txt_checkboxes_state_[i]));
            }

            auto cif_container = Container::Vertical({});
            for (size_t i = 0; i < cif_files_.size(); ++i)
            {
                cif_container->Add(Checkbox(cif_files_[i].filename().string(), &cif_checkboxes_state_[i]));
            }

            main_container->Add(password_input);
            main_container->Add(algorithm_radiobox);
            main_container->Add(encrypt_button);
            main_container->Add(decrypt_button);
            main_container->Add(refresh_button);
            main_container->Add(txt_container);
            main_container->Add(cif_container);


            auto layout = vbox({
                text("🔐 ENCRYPTOR TUI 🔐") | bold | color(Color::Cyan) | hcenter,
                separatorLight(),
                hbox({
                    vbox({
                        window(text(" Archivos .txt para Cifrar ") | bold,
                               txt_container->Render() | vscroll_indicator | frame | size(HEIGHT, LESS_THAN, 10)) |
                        color(Color::Green),
                        separator(),
                        window(text(" Archivos .cif para Descifrar ") | bold,
                               cif_container->Render() | vscroll_indicator | frame | size(HEIGHT, LESS_THAN, 10)) |
                        color(Color::Red),
                    }) | flex,
                    separator(),
                    vbox({
                        window(text(" Controles ") | bold,
                               vbox({
                                   text("Clave:") | color(Color::Yellow),
                                   password_renderer, // Usamos la versión renderizada
                                   separator(),
                                   text("Algoritmo:") | color(Color::Yellow),
                                   algorithm_radiobox->Render() | hcenter,
                               })
                        ),
                        separator(),
                        window(text(" Acciones ") | bold,
                               vbox({
                                   encrypt_button->Render() | hcenter,
                                   decrypt_button->Render() | hcenter,
                                   refresh_button->Render() | hcenter,
                               })
                        )
                    }) | size(WIDTH, EQUAL, 40),
                }),
                separatorLight(),
                text("Estado: " + status_message_) | dim,
            }) | borderRounded | color(Color::Default);

            return layout;
        });

        auto screen = ScreenInteractive::TerminalOutput();
        screen.Loop(final_component);
    }

private:
    const fs::path work_dir_ = "Enviroments";

    std::vector<fs::path> txt_files_;
    std::vector<fs::path> cif_files_;
    std::deque<bool> txt_checkboxes_state_;
    std::deque<bool> cif_checkboxes_state_;
    std::string password_;
    std::string status_message_ = "Listo. Selecciona archivos y una operación.";
    int selected_algorithm_ = 0;

    void ScanFiles()
    {
        txt_files_.clear();
        cif_files_.clear();
        txt_checkboxes_state_.clear();
        cif_checkboxes_state_.clear();

        try
        {
            fs::create_directory(work_dir_);
            for (const auto& entry : fs::directory_iterator(work_dir_))
            {
                if (entry.is_regular_file())
                {
                    if (entry.path().extension() == ".txt")
                    {
                        txt_files_.push_back(entry.path());
                    }
                    else if (entry.path().extension() == ".cif")
                    {
                        cif_files_.push_back(entry.path());
                    }
                }
            }
        }
        catch (const fs::filesystem_error& e)
        {
            status_message_ = "Error al escanear directorio: " + std::string(e.what());
        }

        txt_checkboxes_state_.resize(txt_files_.size(), false);
        cif_checkboxes_state_.resize(cif_files_.size(), false);
        status_message_ = "Directorio 'Enviroments' escaneado. " + std::to_string(txt_files_.size()) + " .txt y " +
            std::to_string(cif_files_.size()) + " .cif encontrados.";
    }

    fs::path getUniqueOutputPath(const fs::path& base_path) const
    {
        if (!fs::exists(base_path))
        {
            return base_path;
        }

        fs::path stem = base_path.stem();
        fs::path ext = base_path.extension();
        fs::path parent_path = base_path.parent_path();

        int counter = 1;
        fs::path new_path;
        do
        {
            std::string new_stem = stem.string() + "(" + std::to_string(counter) + ")";
            new_path = parent_path / (new_stem + ext.string());
            counter++;
        }
        while (fs::exists(new_path));

        return new_path;
    }

    void ProcessFiles(bool encrypt_mode)
    {
        if (password_.empty() && selected_algorithm_ != 4)
        {
            status_message_ = "Error: La clave no puede estar vacía para este algoritmo.";
            return;
        }

        EncryptorFactory::Type algorithm_type;
        switch (selected_algorithm_)
        {
        case 0: algorithm_type = EncryptorFactory::Type::XOR;
            break;
        case 1: algorithm_type = EncryptorFactory::Type::DES;
            break;
        case 2: algorithm_type = EncryptorFactory::Type::CESAR;
            break;
        case 3: algorithm_type = EncryptorFactory::Type::VIGENERE;
            break;
        case 4: algorithm_type = EncryptorFactory::Type::BINARY;
            break;
        default:
            status_message_ = "Error: Algoritmo no válido seleccionado.";
            return;
        }

        auto encryptor = EncryptorFactory::createEncryptor(algorithm_type);

        if (!encryptor)
        {
            status_message_ = "Error: No se pudo crear el algoritmo de cifrado. (Puede que no esté implementado)";
            return;
        }

        int files_processed = 0;
        if (encrypt_mode)
        {
            for (size_t i = 0; i < txt_files_.size(); ++i)
            {
                if (txt_checkboxes_state_[i])
                {
                    fs::path input_file = txt_files_[i];
                    fs::path base_output_file = work_dir_ / input_file.stem().concat(".cif");
                    fs::path unique_output_file = getUniqueOutputPath(base_output_file);

                    if (encryptor->encrypt(input_file, unique_output_file, password_))
                    {
                        files_processed++;
                    }
                }
            }
            status_message_ = "Cifrado completado. " + std::to_string(files_processed) + " archivos procesados.";
        }
        else // Descifrado
        {
            for (size_t i = 0; i < cif_files_.size(); ++i)
            {
                if (cif_checkboxes_state_[i])
                {
                    fs::path input_file = cif_files_[i];
                    fs::path base_output_file = work_dir_ / input_file.stem().concat(".txt");
                    fs::path unique_output_file = getUniqueOutputPath(base_output_file);

                    if (encryptor->decrypt(input_file, unique_output_file, password_))
                    {
                        files_processed++;
                    }
                }
            }
            status_message_ = "Descifrado completado. " + std::to_string(files_processed) + " archivos procesados.";
        }
        ScanFiles();
    }
};
