#ifndef COMMON_H
#define COMMON_H
#include <QPainter>
#include <cmath>
#include "Vec3f.h"
#include <QDebug>
#include <time.h>
using namespace std;
const double maxn=10000000;
class Pixel
{
public:
    QColor color;
};
class Ray
{
public:
    Vec3f o;
    Vec3f dir;
    Ray(Vec3f o_,Vec3f dir_)
    {
        o=o_;
        dir=dir_;
    };
    Ray()
    {
    }
};
double dot(Vec3f a,Vec3f b);
double len(Vec3f a);
double power(double a,int n);
double sqr(double a);
Vec3f cross(Vec3f a,Vec3f b);
void rotate(Vec3f&,Vec3f N,double c,double s);
void print(Vec3f a);
double fgetmax(double a,double b);
double f3getmax(double a,double b,double c);
double fgetmin(double a,double b);
double f3getmin(double a,double b,double c);
int getmax(int a,int b);
int getmin(int a,int b);
Vec3f Vgetmin(Vec3f a,Vec3f b);
Vec3f Vgetmax(Vec3f a,Vec3f b);
double det(Vec3f a,Vec3f b,Vec3f c);
#endif // COMMON_H
