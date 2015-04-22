#ifndef THREAD_H
#define THREAD_H
#include <QThread>
#include "raytracer.h"
class renderThread : public QThread
{
    Raytracer *raytrace;
    int x,y,w,h;
public:
    renderThread(Raytracer *rt,int x_,int y_,int w_,int h_)
    {
        raytrace=rt;
        x=x_;y=y_;w=w_;h=h_;
    }

    void run()
    {
        raytrace->render(x,y,w,h);
    }
};

#endif // THREAD_H
