#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <typeinfo>

#include "./Visor/VisorMatriz.h"

using namespace std;

class CVCSFileWriter
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

#pragma pack(push, 1)
    struct BMPHeader
    {
        char signature[2];
        int fileSize;
        int reserved;
        int dataOffSet;
        int headerSize;
        int width;
        int height;
        short planes;
        short bitsPerPixel;
        int compression;
        int dataSize;
        int horizontalResolution;
        int verticalResolution;
        int colors;
        int importantColors;
    };

#pragma pack(pop)
    static vector<vector<VisorMatrizImg::Pixel>> leerArchivoBMP(const char *nombreArchivo, BMPHeader &header)
    {
        ifstream archivo(nombreArchivo, ios::binary);
        if (!archivo)
        {
            cerr << "No se pudo abrir el archivo" << endl;
            exit(1);
        }
        archivo.read(reinterpret_cast<char *>(&header), sizeof(BMPHeader));
        archivo.seekg(header.dataOffSet, ios::beg);
        vector<vector<VisorMatrizImg::Pixel>> matriz(header.height, vector<VisorMatrizImg::Pixel>(header.width));
        for (int i = 0; i < header.height; i++)
        {
            for (int j = 0; j < header.width; j++)
            {
                archivo.read(reinterpret_cast<char *>(&matriz[i][j]), sizeof(VisorMatrizImg::Pixel));
            }
            archivo.seekg(header.width % 4, ios::cur);
        }
        archivo.close();
        return matriz;
    }

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

    static void ReplaceSpacesWithHyphens(std::string &str)
    {
        for (size_t i = 0; i < str.size(); ++i)
        {
            if (str[i] == ' ')
            {
                str[i] = '-';
            }
        }
    }

    static void WriteCVCSFile(const std::string &filename, const string &inputFile)
    {
        vector<vector<VisorMatrizImg::Pixel>> matriz;
        BMPHeader header;
        matriz = leerArchivoBMP(inputFile.c_str(), header);

        std::ofstream file(filename, std::ios::binary);

        if (!file.is_open())
        {
            std::cerr << "Error opening file!" << std::endl;
            return;
        }

        CVCSData data;

        UpdateCVCSData(data);

        ReplaceSpacesWithHyphens(data.idPaciente);
        ReplaceSpacesWithHyphens(data.nombrePaciente);
        ReplaceSpacesWithHyphens(data.edadPaciente);
        ReplaceSpacesWithHyphens(data.fechaExamen);
        ReplaceSpacesWithHyphens(data.diagnosticos);

        // Write patient data
        file << data.idPaciente << " "
             << data.nombrePaciente << " "
             << data.edadPaciente << " "
             << data.fechaExamen << " "
             << data.diagnosticos << "\n";

        cout << reinterpret_cast<const char *>(&header.height) << endl;
        cout << header.height << endl;
        file.write(reinterpret_cast<const char *>(&header.width), sizeof(int));
        file.write("x", 1);
        file.write(reinterpret_cast<const char *>(&header.height), sizeof(int));

        for (int i = 0; i < header.height; i++)
        {
            for (int j = 0; j < header.width; j++)
            {
                file.write(reinterpret_cast<const char *>(&matriz[i][j].red), sizeof(unsigned char));
                file.write(reinterpret_cast<const char *>(&matriz[i][j].green), sizeof(unsigned char));
                file.write(reinterpret_cast<const char *>(&matriz[i][j].blue), sizeof(unsigned char));
            }
            file << "\n";
        }

        file.close();
    }
};