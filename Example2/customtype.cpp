#include "customtype.h"

CustomType::CustomType()
{

}

double CustomType::getS() const
{
    return s;
}

void CustomType::setS(double newS)
{
    if(newS > smax) {
        s = (double)smax;
        return;
    }
    if(newS < smin) {
        s = (double)smin;
        return;
    }
    s = newS;
}

double CustomType::getN() const
{
    return n;
}

void CustomType::setN(double newN)
{
    if(newN > nmax) {
        n = (double)nmax;
        return;
    }
    if(newN < nmin) {
        n = (double)nmin;
        return;
    }
    n = newN;
}

double CustomType::getAdapt() const
{
    return adapt;
}

void CustomType::setAdapt(double newAdapt)
{
    adapt = newAdapt;
}

double CustomType::W() const
{
    double f = A * (n - C* (v() / (s * n)) )/sin(B / (s*n));
    return f;
}

double CustomType::W(double s, double n, double v)
{
    double f = A * (n - C* (v / (s * n)) )/sin(B / (s*n));
    return f;
}

double CustomType::v() const
{
    double vv = ( n/C - wopt*sin( B/(s*n) )/(A*C) )*s*n;
    return vv;
}

double CustomType::v(double s, double n)
{
    double vv = ( n/C - wopt*sin( B/(s*n) )/(A*C) )*s*n;
    return vv;
}

double CustomType::cost() const
{
    double f = v()/100.0 + 23.63/(s*n);
    return f;
}

double CustomType::cost(double s, double n, double v)
{
    double f = v/100.0 + 23.63/(s*n);
    return f;
}
