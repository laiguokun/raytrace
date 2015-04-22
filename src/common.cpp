#include "common.h"
double dot(Vec3f a,Vec3f b)
{
    return a.x*b.x+a.y*b.y+a.z*b.z;
}
double len(Vec3f a)
{
    return sqrt(dot(a,a));
}

double power(double a,int n)
{
    double res=1;
    while (n>0)
    {
        res=res*a;
        n=n-1;
    }
    return res;
}
Vec3f cross(Vec3f a,Vec3f b)
{
    Vec3f res;
    res.x=(a.y*b.z-a.z*b.y);
    res.y=(a.z*b.x-a.x*b.z);
    res.z=(a.x*b.y-a.y*b.x);
    return res;
}
double sqr(double a)
{
    return a*a;
}
void rotate(Vec3f &V, Vec3f N, double c,double s)
{
/*    double A[3][3];
//    qDebug()<<N.x<<" "<<c<<" "<<1-c<<endl;
    A[0][0]=N.x*N.x*(1-c)+c;
    A[0][1]=N.z*N.x*(1-c)-N.z*s;
    A[0][2]=N.x*N.z*(1-c)+N.y*s;
    A[1][0]=N.y*N.x*(1-c)+N.z*s;
    A[1][1]=N.z*N.x*(1-c)+c;
    A[1][2]=N.y*N.z*(1-c)-N.x*s;
    A[2][0]=N.z*N.x*(1-c)-N.y*s;
    A[2][1]=N.z*N.x*(1-c)-N.x*s;
    A[2][2]=N.z*N.z*(1-c)+c;
    Vec3f tmp(0,0,0);
    for (int i=0;i<3;i++)
        for (int j=0;j<3;j++)
        {
            qDebug()<<i<<" "<<j<<" "<<A[i][j]<<endl;
            tmp[i]+=A[i][j]*camera[j];
        }
    camera=tmp;*/
    V=V*c+cross(N,V)*s+N*dot(N,V)*(1-c);
//    print(camera);
}
void print(Vec3f a)
{
    qDebug()<<a.x<<" "<<a.y<<" "<<a.z<<endl;
}
double fgetmax(double a, double b)
{
    if (a>b) return a;else return b;
}
double fgetmin(double a, double b)
{
    if (a<b) return a;else return b;
}
double f3getmin(double a, double b, double c)
{
    return fgetmin(c,fgetmin(a,b));
}
double f3getmax(double a, double b, double c)
{
    return fgetmax(a,fgetmax(b,c));
}
double det(Vec3f a, Vec3f b, Vec3f c)
{
    return a[0]*b[1]*c[2]+a[1]*b[2]*c[0]+a[2]*b[0]*c[1]
            -a[2]*b[1]*c[0]-a[1]*b[0]*c[2]-a[0]*b[2]*c[1];
}
Vec3f Vgetmax(Vec3f a, Vec3f b)
{
    return Vec3f(fgetmax(a.x,b.x),fgetmax(a.y,b.y),fgetmax(a.z,b.z));
}
Vec3f Vgetmin(Vec3f a, Vec3f b)
{
    return Vec3f(fgetmin(a.x,b.x),fgetmin(a.y,b.y),fgetmin(a.z,b.z));
}
int getmax(int a, int b)
{
    if (a>b) return a;else return b;
}
int getmin(int a, int b)
{
    if (a<b) return a;else return b;
}
