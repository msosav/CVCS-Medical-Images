#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <typeinfo>

#include "./Visor/VisorMatriz.h"

using namespace std;

class CVCSFileReader
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

    static void ReadCVCSFile(const string &filename)
    {
        ifstream file(filename, ios::binary);

        if (!file.is_open())
        {
            cerr << "Error opening file!" << endl;
        }

        CVCSData data;

        // Read patient data
        getline(file, data.idPaciente, ' ');
        getline(file, data.nombrePaciente, ' ');
        getline(file, data.edadPaciente, ' ');
        getline(file, data.fechaExamen, ' ');
        getline(file, data.diagnosticos);

        cout << "ID del paciente: " << data.idPaciente << endl;
        cout << "Nombre del paciente: " << data.nombrePaciente << endl;
        cout << "Edad del paciente: " << data.edadPaciente << endl;
        cout << "Fecha del examen: " << data.fechaExamen << endl;
        cout << "Diagnósticos: " << data.diagnosticos << endl;
        cout << endl;

        int width, height;
        file.read(reinterpret_cast<char *>(&width), sizeof(int));
        file.seekg(1, ios::cur); // Skip separator
        file.read(reinterpret_cast<char *>(&height), sizeof(int));

        cout << "Matrix Width: " << width << endl;
        cout << "Matrix Height: " << height << endl;

        vector<vector<VisorMatrizImg::Pixel>> matriz(height, vector<VisorMatrizImg::Pixel>(width));

        for (int i = 0; i < height; ++i)
        {
            for (int j = 0; j < width; ++j)
            {
                file.read(reinterpret_cast<char *>(&matriz[i][j]), sizeof(VisorMatrizImg::Pixel));
            }
            file.seekg(1, ios::cur); // Skip separator
        }

        file.close();

        VisorMatrizImg visor(matriz);
        visor.show("CVCS File");
    }
};
