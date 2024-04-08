#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <typeinfo>

#include "./Visor/VisorMatriz.h"

class CVCSFileReader
{
public:
    struct CVCSData
    {
        std::string idPaciente;
        std::string nombrePaciente;
        std::string edadPaciente;
        std::string fechaExamen;
        std::string diagnosticos;
    };

    static void UpdateCVCSData(CVCSData &data)
    {
        std::cout << "ID del paciente: ";
        std::getline(std::cin, data.idPaciente);
        std::cout << "Nombre del paciente: ";
        std::getline(std::cin, data.nombrePaciente);
        std::cout << "Edad del paciente: ";
        std::getline(std::cin, data.edadPaciente);
        std::cout << "Fecha del examen: ";
        std::getline(std::cin, data.fechaExamen);
        std::cout << "Diagnósticos: ";
        std::getline(std::cin, data.diagnosticos);
    }

    static void ReadCVCSFile(const std::string &filename)
    {
        std::ifstream file(filename, std::ios::binary);

        if (!file.is_open())
        {
            std::cerr << "Error opening file!" << std::endl;
            return;
        }

        CVCSData data;

        // Read patient data
        std::getline(file, data.idPaciente, ' ');
        std::getline(file, data.nombrePaciente, ' ');
        std::getline(file, data.edadPaciente, ' ');
        std::getline(file, data.fechaExamen, ' ');
        std::getline(file, data.diagnosticos);

        std::cout << "ID del paciente: " << data.idPaciente << std::endl;
        std::cout << "Nombre del paciente: " << data.nombrePaciente << std::endl;
        std::cout << "Edad del paciente: " << data.edadPaciente << std::endl;
        std::cout << "Fecha del examen: " << data.fechaExamen << std::endl;
        std::cout << "Diagnósticos: " << data.diagnosticos << std::endl;

        int width, height;
        file.read(reinterpret_cast<char *>(&width), sizeof(int));
        file.seekg(1, std::ios::cur); // Skip separator
        file.read(reinterpret_cast<char *>(&height), sizeof(int));

        std::cout << "Matrix Width: " << width << std::endl;
        std::cout << "Matrix Height: " << height << std::endl;

        std::vector<std::vector<VisorMatrizImg::Pixel>> matriz(height, std::vector<VisorMatrizImg::Pixel>(width));
        for (int i = 0; i < height; ++i)
        {
            for (int j = 0; j < width; ++j)
            {
                file.read(reinterpret_cast<char *>(&matriz[i][j].red), sizeof(unsigned char));
                file.read(reinterpret_cast<char *>(&matriz[i][j].green), sizeof(unsigned char));
                file.read(reinterpret_cast<char *>(&matriz[i][j].blue), sizeof(unsigned char));
            }
            file.ignore(1);
        }

        VisorMatrizImg matrix2Image(matriz);

        matrix2Image.show(filename);
    }
};