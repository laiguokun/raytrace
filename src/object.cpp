#include "object.h"
#include "Vec3f.h"
const double eps=1e-9;
bool sphere::intersect(Ray ray, double &dist, Vec3f &point,Vec3f &N,Vec3f& color)
{
    double b=dot(ray.dir,(ray.o-center));
    double c=dot(ray.o-center,ray.o-center)-r*r;
    if (b*b-c<0) return false;
    double t1=-b-sqrt(b*b-c);
    double t2=-b+sqrt(b*b-c);
    if (t1<eps)
    {
        if (t2<eps) return false;
        else
        {
            point=ray.o+ray.dir*t2;
            dist=t2;
            color=get_color(point);
            N=get_Normal(point);
            return true;
        }
    }
    else
    {
        point=ray.o+ray.dir*t1;
        dist=t1;
        color=get_color(point);
        N=get_Normal(point);
        return true;
    }
}
Vec3f sphere::calc_reflection(Ray ray,Vec3f point,Vec3f N)
{
    Vec3f reflection=ray.dir-N*2*dot(ray.dir,N);
    reflection.Normalize();
    return reflection;
}
Vec3f sphere::get_Normal(Vec3f point)
{
    return Vec3f((point-center)*(1.0/r));
}
Vec3f sphere::get_color(Vec3f point)
{
    if (Istexture!=1)
        return color;
    else
    {
        double theta,phi,u,v;
        theta=acos((point.z-center.z)/r);
        phi=atan((point.y-center.y)/(point.x-center.x));
        if (fabs(point.x-center.x)<eps)
        {
            if (point.y-center.y>0) phi=0;
            else phi=M_PI;
        }
        else
        {
            if (point.y-center.y>0 && point.x-center.x<0)
                phi=phi+M_PI;
            if (point.y-center.y<0 && point.x-center.x<0)
                phi=phi+M_PI;
            if (point.y-center.y<0 && point.x-center.x>0)
                phi=phi+2*M_PI;
        }
        u=theta/M_PI*(double)t_w;
        v=phi/M_PI/2*(double)t_h;
/*        if ((int)v==-145)
        {
            print(point);
            qDebug()<<theta<<endl;
            qDebug()<<phi<<endl;
            system("pause");
        }*/
        QColor c=text->pixel((int)(u),(int)(v));
        return Vec3f((double)c.red()/255,(double)c.blue()/255,(double)c.green()/255);
    }
}

bool triangle::intersect(Ray ray, double &dist, Vec3f &point,Vec3f &N,Vec3f &color)
{
    dist=maxn;
    Vec3f x;
    x=Vec3f(det(p0-ray.o,p0-p1,p0-p2),
            det(ray.dir,p0-ray.o,p0-p2),
            det(ray.dir,p0-p1,p0-ray.o));
    double t=(det(ray.dir,p0-p1,p0-p2));
    x=x/t;
    if (!(x[0]>eps && x[1]>=-eps && x[1]<=1+eps && x[2]>=-eps && x[2]<=1+eps && x[1]+x[2]<=1+eps))
        return false;
    double alpha=1-x[1]-x[2],beta=x[1],zeta=x[2];
    if (o1)
    {
        N=vn0*alpha+vn1*beta+vn2*zeta;
        N.Normalize();
    }
    else
    {
        Vec3f v1=p1-p0;Vec3f v2=p2-p1;
        N=cross(v1,v2);
        N.Normalize();
    }
    dist=x[0]+dot(N,ray.dir)*eps;
    point=ray.o+ray.dir*dist;
    if (Istexture==2 || Istexture==3)
    {
        Vec3f vt=vt0*alpha+vt1*beta+vt2*zeta;
        double u,v;
        QColor c;
        u=vt.x*t_w;v=vt.y*t_h;
//        if (u>0 && u<t_w && v>0 && v<t_h)
        c=text->pixel((int)(u),(int)(v));
//        qDebug()<<u<<" "<<v<<endl;
//        system("pause");
        color=Vec3f((double)c.red()/255,(double)c.blue()/255,(double)c.green()/255);
    }
    if (Istexture==1)
    {
        int x=(int)(point.x/8);
        int z=(int)(point.z/8);
        int t;
        t=abs(x)%2+abs(z)%2;
        if (point.x<0) t++;
//        if (point.z>0) t++;
        t=t%2;
        if (t==1)
            color=Vec3f(1,1,1);
        else
            color=Vec3f(0,0,0);
    }
    if (Istexture==0)
        color=this->color;
    return true;
}

Vec3f triangle::calc_reflection(Ray ray,Vec3f point,Vec3f N)
{
    Vec3f reflection;
    reflection=ray.dir-N*2*dot(ray.dir,N);
    reflection.Normalize();
    return reflection;
}
