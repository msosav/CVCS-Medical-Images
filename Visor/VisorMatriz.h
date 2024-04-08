#ifndef VISOR_MATRIZ_H
#define VISOR_MATRIZ_H
#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>

class VisorMatrizImg
{
private:
    cv::Mat matrizImage;

public:
    struct Pixel
    {
        unsigned char red, green, blue;
    };
    // Constructor que inicializa la matriz con la matriz dada como parámetro
    VisorMatrizImg(const std::vector<std::vector<Pixel>> &inputMatrix);
    // Método para visualizar la matriz como una imagen
    void show(const std::string titulo);
};
#endif