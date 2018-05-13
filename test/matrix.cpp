#include <iostream>
#include "matrix.h"

void SDIFMatrixData :: essai()
{

std::cout << " essai " << std::endl;
};

template <class T> double SDIFDoubleMatrix :: GetDouble(T &a)
{
     data = static_cast<double>(a);
     std::cout << "Data a=  " << data <<std::endl;
     return data;
     
};

template <class T2> void SDIFDoubleMatrix :: GetInt(T2 &b)
{
     b = static_cast<int>(b);
     std::cout << "Data b=  " << b <<std::endl;
     
};


int main()
{

    SDIFMatrixData mat;
    SDIFDoubleMatrix cas;

    double s = 3.4;
    int z=2;
    double d=2.3;

    mat.essai();
    s = cas.GetDouble(z);
    std::cout << "apres getdouble, on a z=:  " << s <<std::endl;
    std::cout << "avant GetInt, on a d=:  " << d <<std::endl;
    cas.GetInt(d);

    std::cout << "apres GetInt, on a  d=" << d <<std::endl; 
    std::cout << "Data =  " << z <<std::endl;

    return 0;
}
