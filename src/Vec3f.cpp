#include "vec3f.h"
#include <cstring>
#include <cmath>
using namespace std;
Vec3f::Vec3f(void)
{
    memset(_p,0,sizeof(double)*_len);
}

Vec3f::Vec3f(double x, double y, double z)
{
    this->x = x;
    this->y = y;
    this->z = z;
}

Vec3f::Vec3f(const Vec3f &v)
{
    memcpy(_p,v._p,sizeof(double)*_len);
}

Vec3f::~Vec3f(void)
{

}

//////////////////////////////////////////////////////////////////////////
// Operators

Vec3f& Vec3f::operator =( const Vec3f& v)
{
    memcpy(_p,v._p,sizeof(double)*_len);
    return (*this);
}

void Vec3f::operator +=(const Vec3f& v)
{
    for(int i=0;i<_len;i++)
        _p[i] += v._p[i];
}
void Vec3f::operator +=(double f)
{
    for(int i=0;i<_len;i++)
        _p[i] += f;
}

void Vec3f::operator -=(const Vec3f& v)
{
    for(int i=0;i<_len;i++)
        _p[i] -= v._p[i];
}
void Vec3f::operator -=(double f)
{
    for(int i=0;i<_len;i++)
        _p[i] -= f;
}

void Vec3f::operator *=(const Vec3f& v)
{
    for(int i=0;i<_len;i++)
        _p[i] *= v._p[i];
}
void Vec3f::operator *=(double f)
{
    for(int i=0;i<_len;i++)
        _p[i] *= f;
}

void Vec3f::operator /=(const Vec3f& v)
{
    for(int i=0;i<_len;i++)
        _p[i] /= v._p[i];
}
void Vec3f::operator /=(double f)
{
    for(int i=0;i<_len;i++)
        _p[i] /= f;
}

Vec3f Vec3f::operator +(const Vec3f&v) const
{
    Vec3f res;
    for(int i=0;i<_len;i++)
        res[i] = (*this)[i] + v[i];
    return res;
}
Vec3f Vec3f::operator +(double f) const
{
    Vec3f res;
    for(int i=0;i<_len;i++)
        res[i] = (*this)[i] + f;
    return res;
}

Vec3f Vec3f::operator -(const Vec3f&v) const
{
    Vec3f res;
    for(int i=0;i<_len;i++)
        res[i] = (*this)[i] - v[i];
    return res;
}
Vec3f Vec3f::operator -(double f) const
{
    Vec3f res;
    for(int i=0;i<_len;i++)
        res[i] = (*this)[i] - f;
    return res;
}

Vec3f Vec3f::operator *(const Vec3f&v) const
{
    Vec3f res;
    for(int i=0;i<_len;i++)
        res[i] = (*this)[i] * v[i];
    return res;
}
Vec3f Vec3f::operator *(double f) const
{
    Vec3f res;
    for(int i=0;i<_len;i++)
        res[i] = (*this)[i] * f;
    return res;
}

Vec3f Vec3f::operator /(const Vec3f&v) const
{
    Vec3f res;
    for(int i=0;i<_len;i++)
        res[i] = (*this)[i] / v[i];
    return res;
}
Vec3f Vec3f::operator /(double f) const
{
    Vec3f res;
    for(int i=0;i<_len;i++)
        res[i] = (*this)[i] / f;
    return res;
}

Vec3f Vec3f::operator - () const
{
    Vec3f res;
    for(int i=0;i<_len;i++)
        res[i] = -(*this)[i];
    return res;
}

//////////////////////////////////////////////////////////////////////////
// Other Methods
void Vec3f::Normalize()
{
    double fSqr = L2Norm_Sqr();
    if(fSqr>1e-6)
        (*this) *= 1.0f/sqrt(fSqr);
}

double Vec3f::L2Norm_Sqr()
{
    return _p[0]*_p[0] + _p[1]*_p[1] + _p[2]*_p[2];
}
