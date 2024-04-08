#include "CVCSFileWriter.cpp"

#include "CVCSFileReader.cpp"

int main()
{
    CVCSFileWriter::WriteCVCSFile("test.cvcs", "street.bmp");
    CVCSFileReader::ReadCVCSFile("test.cvcs");
    return 0;
}