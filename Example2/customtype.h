#ifndef CUSTOMTYPE_H
#define CUSTOMTYPE_H

#include <QtMath>
#include <QDebug>

class CustomType
{
public:

    constexpr static const double A = 0.13044055;
    constexpr static const double B = 0.68203740;
    constexpr static const double C = 0.20000000;

    constexpr static const double smax = 0.00063;
    constexpr static const double smin = 0.00017;

    constexpr static const double nmax = 2000;
    constexpr static const double nmin = 1000;

    constexpr static const double vmax = 1800;
    constexpr static const double vmin = 800;

    constexpr static const double wopt = 110;

    constexpr static const double muts = 0.00001;
    constexpr static const double mutn = 10.0;

    CustomType();

    double getS() const;
    void setS(double newS = (double)(smin+smax)/2.0);// 0.0004

    double getN() const;
    void setN(double newN = (double)(nmin+nmax)/2.0);//1500

    double getAdapt() const;
    void setAdapt(double newAdapt);

    double W() const;
    static double W(double s, double n, double v);

    double v() const;
    static double v(double s, double n);

    double cost() const;
    static double cost(double s, double n, double v);

private:

    double s, n, adapt;
};

#endif // CUSTOMTYPE_H
