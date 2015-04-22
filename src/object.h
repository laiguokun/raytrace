#ifndef OJBECT1_H
#define OJBECT1_H
#include "common.h"
#include "Vec3f.h"
class object
{
public:
    virtual bool intersect(Ray ray, double &dist, Vec3f &point,Vec3f &N,Vec3f &color)
    {
        return false;
    }
    virtual Vec3f calc_reflection(Ray ray,Vec3f point,Vec3f N){};
    bool o1;//是否平滑法向
    Vec3f color;
    double diff;
    double ref;
    double refcrate;
    double refc;
    double dref;
    Vec3f boxmin;
    Vec3f boxmax;
    int Istexture;//0为无纹理，1自定纹理，2映射纹理
    QImage* text;
    int t_h;
    int t_w;
    object()
    {
        diff=0;
        ref=0;
        refc=0;
        refcrate=0;
        dref=0;
        Istexture=0;
        o1=false;
    }
};
class Light
{
public:
    Vec3f point;
    Vec3f color;
    Light(Vec3f p)
    {
        point=p;
        color=Vec3f(1,1,1);
    }
};

class sphere : public object
{
public:
    Vec3f center;
    double r;
    bool intersect(Ray ray, double &dist, Vec3f &point,Vec3f &N,Vec3f &color);
    Vec3f get_Normal(Vec3f point);
    Vec3f calc_reflection(Ray ray,Vec3f point,Vec3f N);
    Vec3f get_color(Vec3f point);
    sphere(Vec3f c_,double r_,int b_,QString filename):object()
    {
        center=c_;
        r=r_;
        boxmin=center-Vec3f(r,r,r);
        boxmax=center+Vec3f(r,r,r);
        text=new QImage;
        Istexture=b_;
        if (b_==1)
        {
            text->load(filename);
            t_w=text->width();
            t_h=text->height();
            if (t_w>t_h)
                t_w=t_h;
            else
                t_h=t_w;
        }
    }
};
class triangle : public object
{
public:
    Vec3f p0,p1,p2;
    Vec3f vn0,vn1,vn2;
    Vec3f vt0,vt1,vt2;
    bool intersect(Ray ray,double &dist,Vec3f &point,Vec3f &N,Vec3f &color);
    Vec3f calc_reflection(Ray ray,Vec3f point,Vec3f N);
    void get_Normal(Vec3f point);
    triangle(Vec3f a,Vec3f b,Vec3f c,Vec3f n0,Vec3f n1,Vec3f n2,Vec3f t0,Vec3f t1,Vec3f t2,bool o,int Ist,QImage* T):object()
    {
        p0=a;p1=b;p2=c;
        if (o)
        {
            vn0=n0;vn1=n1;vn2=n2;
            o1=true;
        }
        else o1=false;
        Istexture=Ist;
        if (Ist==2 || Ist==3)
        {
            vt0=t0;vt1=t1;vt2=t2;
            text=T;
            t_w=text->width();
            t_h=text->height();
            if (t_w>t_h)
                t_w=t_h;
            else
                t_h=t_w;
        }
        boxmin=Vec3f(f3getmin(a.x,b.x,c.x),f3getmin(a.y,b.y,c.y),f3getmin(a.z,b.z,c.z));
        boxmax=Vec3f(f3getmax(a.x,b.x,c.x),f3getmax(a.y,b.y,c.y),f3getmax(a.z,b.z,c.z));
    }
};

class Scene
{
public:
    object** obj;
    Light** light;
    int num;
    int LNum;
    Vec3f camera;
    double force;
    double density;
    int width;
    int height;
    Vec3f screen;
    Vec3f dx;
    Vec3f dy;
    double deep;
    double** dist;
};

#endif // OJBECT1_H
