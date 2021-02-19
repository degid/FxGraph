#include "grapher.h"

double FxFormula::fx1(double X, double A, double B, double C)
{
    return A*(X*X) + B*X + C;
}

double FxFormula::fx2(double X, double A, double B, double C)
{
    return A * sin(X) + B * cos(C*X);
}

double FxFormula::fx3(double X, double A, double B, double C)
{
    double Y = A*log(B*X);

    if(Y != Y)
    {
        return 0;
    }

    return Y;
}

double FxFormula::fx4(double X, double A, double B, double C)
{
    double Y = sin(X*X) * B;

    if(Y == 0)
    {
        return 0;
    }

    return A / Y;
}

std::function<double (double, double, double, double)> FxFormula::getFuncrion(int index)
{
    switch (index) {
    case 0:
        return this->fx1;
    case 1:
        return this->fx2;
        break;
    case 2:
        return this->fx3;
        break;
    case 3:
        return this->fx4;
    default:
        return this->fx1;
    }
}
