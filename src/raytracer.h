#ifndef RAYTRACER_H
#define RAYTRACER_H
#include "common.h"
#include "Vec3f.h"
#include "object.h"
#include "kdtree.h"
class Raytracer
{
public:
    int width,height;
    double x1,y1,x2,y2,dx,dy;
    Vec3f camera;
    Pixel** pixel;
    Scene* scene;
    void render(int x,int y,int w,int h);
    void initialize();
    kdtree* tree;
    Raytracer(Pixel** pixel_,Scene* scene_)
    {
        pixel=pixel_;
        scene=scene_;
        width=scene->width;
        height=scene->height;
        initialize();
    };
    void raytrace(Ray ray,Vec3f& color,double& dist,int depth);
};

#endif // RAYTRACER_H
