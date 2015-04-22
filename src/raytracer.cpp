#include "raytracer.h"
const int maxdepth=3;
const double eps=1e-9;
const double Kd=0.7;
const double Ks=0.3;
const double Ka=0.1;
const Vec3f I(1,1,1);
const Vec3f O(0,0,0);
const int n=30;
void Raytracer::initialize()
{
    tree=new kdtree(scene);
//    qDebug()<<"yes"<<endl;
}
int shadow_point[16][2]={{1,2},{3,3},{2,0},{0,1},{2,3},{0,3},
                        {0,0},{2,2},{3,1},{1,3},{1,0},{3,2},
                        {2,1},{3,0},{1,1},{0,2}};
void Raytracer::raytrace(Ray ray,Vec3f& color,double& dist,int depth)
{
    if (depth>maxdepth) return;
    object* obj=0;
    Vec3f point;
    double d=10000000;
    int now=tree->gethit(ray,d);
    if (now==-1)
    {
        return;
    }
    point=ray.o+ray.dir*d;
    obj=scene->obj[now];
    if (obj->Istexture==3)
    {
        color=Vec3f(1,1,1);
        dist=1000000;
        return;
    }
    Vec3f obj_N;
    Vec3f obj_color;
    obj->intersect(ray,dist,point,obj_N,obj_color);
    //monte calo
    if (depth==-1)
    {
        Vec3f R=obj_N;
        Vec3f R1=Vec3f(R.z,R.y,-R.x);
        Vec3f R2=cross(R,R1);
        int cir_sample=16;
        for (int i=0;i<cir_sample;i++)
        {
            double x,y;
            x=((double)rand()/16384-1)*10;
            y=((double)rand()/16384-1)*10;
            Vec3f r=R+R1*x+R2*y;
            r.Normalize();
            Vec3f c_(0,0,0);
            double d_;
            Ray r_(point+r*eps,r);
            raytrace(r_,c_,d_,depth+1);
            color+=(c_*obj_color*Ka/cir_sample);
        }
    }
    else
    {
        color+=(I*obj->diff*Ka);
    }
    //shadow and diffuse
    Vec3f guyou;
    for (int i=0;i<scene->LNum;i++)
    {
        double shade=1;
        int shadow_sample=0;
        if (depth>0) shadow_sample=0;
        for (int t1=0;t1<shadow_sample;t1++)
            {
                Vec3f lightpoint=scene->light[i]->point
                        +Vec3f(shadow_point[t1][0],0,shadow_point[t1][1]);
                if (shadow_sample>1) lightpoint+=Vec3f((double)rand()/32768,0,(double)rand()/32768);
                Vec3f L=lightpoint-point;
                double Len=len(L);
                L.Normalize();
                Ray nray(point+L*eps,L);
                double tmp_d;
                int tmp=tree->gethit(nray,tmp_d);
                    if (tmp!=-1 && scene->obj[tmp]->Istexture!=3 && tmp_d<Len)
                    {
                        shade=shade-(1.0-(scene->obj[tmp]->refc)*(scene->obj[tmp]->refc))/shadow_sample;
                    }
            }
        Vec3f L=scene->light[i]->point-point;
        L.Normalize();
        Vec3f N=obj_N;
        double d=fabs(dot(L,N));
        if (d>0)
        {
            color+=(scene->light[i]->color)*(obj_color)*(obj->diff)*Kd*d*shade;
            guyou=(scene->light[i]->color)*(obj_color)*(obj->diff)*Kd*d*shade;
            L=-L;
            Vec3f R=L-N*2*dot(L,N);
            R.Normalize();
            d=(dot(-ray.dir,R));
            if (d>0)
            {
                color+=((scene->light[i]->color)*Ks*power(d,n))*shade;
            }
        }
    }
    //reflection
    if (depth<0 && obj->dref>0)
    {
        Vec3f reflection=obj->calc_reflection(ray,point,obj_N);
        Vec3f R=reflection;
        Vec3f R1=Vec3f(R.z,R.y,-R.x);
        Vec3f R2=cross(R,R1);
        int ref_sample=16;
        double ref=obj->ref/ref_sample;
        for (int i=0;i<ref_sample;i++)
        {
            double x,y;
            do
            {
                x=((double)rand()/16384-1)*obj->dref;
                y=((double)rand()/16384-1)*obj->dref;
            }
            while (x*x+y*y>obj->dref*obj->dref);
            Vec3f r=R+R1*x+R2*y;
            r.Normalize();
            Vec3f c_(0,0,0);
            double d_;
            Ray r_(point+r*eps,r);
            raytrace(r_,c_,d_,depth+1);
            color+=c_*ref*obj_color;
        }
    }
    else
        if (obj->ref>0)
        {
            Vec3f c_(0,0,0);
            double d_=0;
            Vec3f reflection=obj->calc_reflection(ray,point,obj_N);
            raytrace(Ray(point+reflection*eps,reflection),c_,d_,depth+1);
            color+=(c_*obj_color*obj->ref);
        }
    //refraction
    if (obj->refcrate>0)
    {
        Vec3f N=obj_N;
        Vec3f I=-ray.dir;
        double d=dot(I,N);
        double n=1.0/obj->refcrate;
        if (d<0)
        {
            N=-N;
            n=1.0/n;
            d=-d;
        }
        double c=1.0-n*n*(1.0-d*d);
        if (c>0)
        {
            c=sqrt(c);
            Vec3f T=(-I)*n+N*(n*d-c);
            T.Normalize();
            Vec3f c_(0,0,0);
            double d_;
            raytrace(Ray(point+T*eps,T),c_,d_,depth+1);
            double tmp=pow(fabs(dot(ray.dir,obj_N)),0.5);
            color+=c_*obj->refc*pow(fabs(dot(ray.dir,obj_N)),2);
            color+=guyou*(1-tmp);
        }
        else
        {
            Vec3f c_(0,0,0);
            double d_=0;
            Vec3f reflection=obj->calc_reflection(ray,point,obj_N);
            raytrace(Ray(point+reflection*eps,reflection),c_,d_,depth+1);
            color+=(c_*obj->refc);
        }
    }
/*    if (depth==0 && fabs(ray.dir.x-0)<1e-5 && fabs(ray.dir.y)<1e-6)
    {
        print(color);
        print(point);
        print(obj_N);
        qDebug()<<now<<endl;
        system("pause");
    }*/
}
const int step[4][2]={{0,-1},{0,1},{1,0},{-1,0}};
Vec3f p[800][600];
void Raytracer::render(int x,int y,int w,int h)
{
    double start=clock();
    camera=scene->camera;
    for (int i=x;i<x+w;i++)
    {
        for (int j=y;j<y+h;j++)
        {
            Vec3f pos=scene->dx*i+scene->dy*j+scene->screen;
            Vec3f dir=(pos-camera);;
            dir.Normalize();
            Ray ray(camera,dir);
            Vec3f color(0,0,0);
            double d;
            raytrace(ray,color,scene->dist[i][j],0);
            p[i][j]=color;
        }
    }
    for (int i=x;i<x+w;i++)
    {
        for (int j=y;j<y+h;j++)
        {
            double r=0,g=0,b=0;
            double max=0;
            for (int k=0;k<4;k++)
                if (i+step[k][0]>=0 && i+step[k][0]<width && j+step[k][1]>=0 && j+step[k][1]<height)
                {
                    Vec3f g=(p[i][j])-(p[i+step[k][0]][j+step[k][1]]);
                    double t=fabs(g.x)+fabs(g.y)+fabs(g.z);
                    max=fgetmax(t,max);
                }
            if (max>1.0/6.0)
            {
                for (int k=0;k<4;k++)
                {
                    Vec3f pos=scene->dx*(i+(double)step[k][0]/2)+scene->dy*(j+(double)step[k][1]/2)+scene->screen;
                    Vec3f dir=(pos-camera);
                    dir.Normalize();
                    Ray ray(camera,dir);
                    double dist=0;
                    Vec3f color(0,0,0);
                    raytrace(ray,color,dist,0);
                    r+=color.x;b+=color.y;g+=color.z;
                }
            }
            else
            {
                r=p[i][j].x*4;b=p[i][j].y*4;g=p[i][j].z*4;
            }
            QColor c(0,0,0);
            r=r/4;b=b/4;g=g/4;
            if (r>=1) c.setRed(255);
            else c.setRed((int)(r*255));
            if (b>=1) c.setBlue(255);
            else c.setBlue((int)(b*255));
            if (g>=1) c.setGreen(255);
            else c.setGreen((int)(g*255));
            pixel[i][j].color=c;
        }
    }
//    print(scene->camera);
}
