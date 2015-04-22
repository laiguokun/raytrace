#ifndef SCREEN_H
#define SCREEN_H

#include <QMainWindow>
#include "common.h"
#include "raytracer.h"
#include "object.h"
#include <QMouseEvent>
#include "simpleobject.h"
#include "thread.h"
namespace Ui {
class screen;
}

class screen : public QMainWindow
{
    Q_OBJECT

public:
    explicit screen(QWidget *parent = 0);
    ~screen();
    int width,height;
    Pixel **pixel;
    Scene scene;
    Raytracer* raytrace;
    renderThread* th[100];
    int cnt,video;
    bool th_end[100];
    void init();
    void paintEvent(QPaintEvent *event);
    void paint();
    void get(const char* fn,Vec3f d,int Istexture,QString filename,double size,Vec3f color,double diff,double ref,double refcrate,double refc,double theta);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    bool mousePress;
    double mouseX,mouseY;
public slots:
    void multi_thread();
private:
    Ui::screen *ui;
};

#endif // SCREEN_H
