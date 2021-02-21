#ifndef GRAPHER_H
#define GRAPHER_H

#include <functional>
#include <list>

struct PairXY
{
    double x;
    double y;
};

struct DataGraph
{
    int fX;
    double A;
    double B;
    double C;

    double From;
    double To;
    double Step;
    double X;

    bool fromSave;
    std::list<PairXY> data;
};

class FxFormula
{
public:
    static double fx1(double X, double A, double B, double C);
    static double fx2(double X, double A, double B, double C);
    static double fx3(double X, double A, double B, double C);
    static double fx4(double X, double A, double B, double C);
    std::function<double(double, double, double, double)> getFuncrion(int index);
};



#endif // GRAPHER_H
