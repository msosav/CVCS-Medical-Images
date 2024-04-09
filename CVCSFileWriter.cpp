#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <typeinfo>
#include <opencv2/opencv.hpp>

#include "./Visor/VisorMatriz.h"

using namespace std;

class CVCSFileWriter
{
public:
    struct CVCSData
    {
        string idPaciente;
        string nombrePaciente;
        string edadPaciente;
        string fechaExamen;
        string diagnosticos;
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
        cout << "ID del paciente: ";
        getline(cin, data.idPaciente);
        cout << "Nombre del paciente: ";
        getline(cin, data.nombrePaciente);
        cout << "Edad del paciente: ";
        getline(cin, data.edadPaciente);
        cout << "Fecha del examen: ";
        getline(cin, data.fechaExamen);
        cout << "Diagnósticos: ";
        getline(cin, data.diagnosticos);
    }

    static void ReplaceSpacesWithHyphens(string &str)
    {
        for (size_t i = 0; i < str.size(); ++i)
        {
            if (str[i] == ' ')
            {
                str[i] = '-';
            }
        }
    }

    static void WriteCVCSFile(const string &filename, const string &inputFile)
    {
        vector<vector<VisorMatrizImg::Pixel>> matriz;
        BMPHeader header;
        matriz = leerArchivoBMP(inputFile.c_str(), header);

        ofstream file(filename, ios::binary);

        if (!file.is_open())
        {
            cerr << "Error opening file!" << endl;
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

        file.write(reinterpret_cast<const char *>(&header.width), sizeof(int));
        file.write("x", 1);
        file.write(reinterpret_cast<const char *>(&header.height), sizeof(int));

        for (int i = 0; i < matriz.size(); i++)
        {
            for (int j = 0; j < matriz[0].size(); j++)
            {
                file.write(reinterpret_cast<char *>(&matriz[i][j]), sizeof(VisorMatrizImg::Pixel));
            }
            file << "\n";
        }

        file.close();
    }
};