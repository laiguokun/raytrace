#include "screen.h"
#include "ui_screen.h"
const int multi=4;
double judge(Vec3f a,Vec3f b,double thetay)
{
    Vec3f tmp=cross(b,a);
    if (dot(tmp,Vec3f(0,-1,0))<0)
        return thetay;
    else return -thetay;
}

void rotate_camera(Scene& scene,Vec3f p,double thetax,double thetay)
{
    Vec3f N=Vec3f(0,-1,0);
    Vec3f v,tmp_screen,tmp_x,tmp_y;
    v=scene.camera-p;
    rotate(v,N,cos(thetax),sin(thetax));
    scene.camera=p+v;
    tmp_screen=scene.screen-p;
    rotate(tmp_screen,N,cos(thetax),sin(thetax));
    tmp_x=scene.screen+scene.dx-p;
    rotate(tmp_x,N,cos(thetax),sin(thetax));
    tmp_y=scene.screen+scene.dy-p;
    rotate(tmp_y,N,cos(thetax),sin(thetax));
    scene.dx=tmp_x-tmp_screen;
    scene.dy=tmp_y-tmp_screen;
    scene.screen=p+tmp_screen;
    N=Vec3f(-1,0,0);
    double t;
    v=scene.camera-p;
    t=judge(v,N,thetay);
    rotate(v,N,cos(t),sin(t));
    scene.camera=p+v;
    tmp_screen=scene.screen-p;
    t=judge(tmp_screen,N,thetay);
    rotate(tmp_screen,N,cos(t),sin(t));
    tmp_x=scene.screen+scene.dx-p;
    t=judge(tmp_x,N,thetay);
    rotate(tmp_x,N,cos(t),sin(t));
    tmp_y=scene.screen+scene.dy-p;
    t=judge(tmp_y,N,thetay);
    rotate(tmp_y,N,cos(t),sin(t));
    scene.dx=tmp_x-tmp_screen;
    scene.dy=tmp_y-tmp_screen;
    scene.screen=p+tmp_screen;
}
void move_camera(Scene& scene,Vec3f move)
{
    scene.screen+=move;
    scene.camera+=move;
}

screen::screen(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::screen)
{
//    qDebug()<<"yes"<<endl;
    ui->setupUi(this);
    width=800;height=600;
    this->setFixedSize(width,height);
    pixel=new Pixel*[width];
    for (int i=0;i<width;i++)
        pixel[i]=new Pixel[height];
    scene.dist=new double*[width];
    for (int i=0;i<width;i++)
        scene.dist[i]=new double[height];
    init();
    scene.camera=Vec3f(0,0,10);
    scene.screen=Vec3f(-8,-6,0);
    scene.density=0.01;
    scene.width=800;
    scene.height=600;
    scene.deep=0;
    scene.force=0;
    scene.dx=Vec3f(16.0/width,0,0);
    scene.dy=Vec3f(0,12.0/height,0);
    move_camera(scene,Vec3f(0,0,20));
 //   rotate_camera(scene,Vec3f(0,0,-20),0,M_PI/12);
//    move_camera(scene,Vec3f(0,10,0));
    raytrace=new Raytracer(pixel,&scene);
    int x1=0,y1=0,dx=width/multi,dy=height/multi;
    for (int i=0;i<multi;i++)
        for (int j=0;j<multi;j++)
        {
            th[i*multi+j]=new renderThread(raytrace,x1+i*dx,y1+j*dy,dx,dy);
            connect(th[i*multi+j],SIGNAL(finished()),this,SLOT(multi_thread()));
        }
/*    video=210;
    for (int i=0;i<150;i++)
        rotate_camera(scene,Vec3f(0,0,-20),M_PI*2/150,-M_PI/6/150);
    for (int i=150;i<180;i++)
        rotate_camera(scene,Vec3f(0,0,-20),0,-M_PI/6/30);*/
//    rotate_camera(scene,Vec3f(0,0,-20),0,-M_PI/6);
    paint();
}
QColor VtoC(Vec3f color)
{
    QColor c(0,0,0);
    double r,g,b;
    r=color.x;b=color.y;g=color.z;
    if (r>=1) c.setRed(255);
    else c.setRed((int)(r*255));
    if (b>=1) c.setBlue(255);
    else c.setBlue((int)(b*255));
    if (g>=1) c.setGreen(255);
    else c.setGreen((int)(g*255));
    return c;
}
Vec3f CtoV(QColor c)
{
    return Vec3f((double)c.red()/255,(double)c.blue()/255,(double)c.green()/255);
}

Vec3f t_p[800][800];
void screen::multi_thread()
{
    cnt++;
    if (cnt==multi*multi)
    {
//            qDebug()<<"yes"<<endl;
            for (int i=0;i<scene.width;i++)
                 for (int j=0;j<scene.height;j++)
                     t_p[i][j]=CtoV(pixel[i][j].color);
            QImage *pix=new QImage(this->size(),QImage::Format_RGB32);
            QRgb tmp;
            for (int i=0;i<scene.width;i++)
                for (int j=0;j<scene.height;j++)
                {
                    tmp=qRgb(pixel[i][j].color.red(),pixel[i][j].color.green(),pixel[i][j].color.blue());
                    pix->setPixel(i,j,tmp);
                }
            pix->save("simple.png");
 //           pix->save(QString::number(video)+".png");
/*            if (video<150)
            {
                video++;
                rotate_camera(scene,Vec3f(0,0,-20),M_PI*2/150,-M_PI/6/150);
                paint();
                qDebug()<<video<<endl;
            }
            if (video<180)
            {
                video++;
                rotate_camera(scene,Vec3f(0,0,-20),0,-M_PI/6/30);
                paint();
                qDebug()<<video<<endl;
            }
            if (video==210)
            {
                for (video=210;video<=240;video++)
                {
//                    scene.force-=1;
                    for (int i=0;i<scene.width;i++)
                        for (int j=0;j<scene.height;j++)
                        {
                            Vec3f color(0,0,0);
                            double sum=0;
                            double r=fabs(scene.dist[i][j]-scene.force)*scene.deep+1;
                            double tt=r;
                            if (r>20) r=20;
                            for (int x=getmax(0,i-r+1);x<getmin(scene.width,i+r);x++)
                                for (int y=getmax(0,j-r+1);y<getmin(scene.height,j+r);y++)
                                {
                                    double r2=sqr(x-i)+sqr(y-j);
                                    if (r2>r*r) continue;
                                    double t=1.0/(2*M_PI*sqr(r/3))*exp(-(r2)/(2*sqr(r/3)));
                                    color+=t_p[x][y]*t;
                                    sum=sum+t;
                                }
                            color=color/sum;
                            pixel[i][j].color=VtoC(color);
                        }
                    QImage *pix=new QImage(this->size(),QImage::Format_RGB32);
                    QRgb tmp;
                    for (int i=0;i<scene.width;i++)
                        for (int j=0;j<scene.height;j++)
                        {
                            tmp=qRgb(pixel[i][j].color.red(),pixel[i][j].color.green(),pixel[i][j].color.blue());
                            pix->setPixel(i,j,tmp);
                        }
                    pix->save("nodeep.png");
//                    pix->save(QString::number(video)+".png");
                    qDebug()<<video<<endl;
                    break;
                }
            }*/
    }
}

void screen::init()
{
    Vec3f p;
    QImage* pimage;
    scene.obj=new object*[1000000];
    scene.num=6;
 //   scene.obj[0]=new triangle(Vec3f(-1,-1,5),Vec3f(1,-1,5),Vec3f(0,1,5));
//    scene.obj[0]->diff=Vec3f(0,0,0);
    double hx,hy,hz,g;
    hx=40;hy=20;hz=40;g=20;
/*    scene.obj[2]=new triangle(Vec3f(-hx,-hy,hz),Vec3f(-hx,hy,hz),Vec3f(hx,-hy,hz));
    scene.obj[3]=new triangle(Vec3f(hx,-hy,hz),Vec3f(-hx,hy,hz),Vec3f(hx,hy,hz));
    scene.obj[4]=new triangle(Vec3f(-hx,-hy,hz),Vec3f(-hx,-hy,-hz),Vec3f(-hx,hy,hz));
    scene.obj[5]=new triangle(Vec3f(-hx,hy,hz),Vec3f(-hx,-hy,-hz),Vec3f(-hx,hy,-hz));
    scene.obj[6]=new triangle(Vec3f(hx,-hy,hz),Vec3f(hx,hy,hz),Vec3f(hx,-hy,-hz));
    scene.obj[7]=new triangle(Vec3f(hx,hy,hz),Vec3f(hx,hy,-hz),Vec3f(hx,-hy,-hz));*/
    scene.obj[0]=new triangle(Vec3f(-maxn,g,maxn),Vec3f(-maxn,g,-maxn),Vec3f(maxn,g,maxn),p,p,p,p,p,p,false,1,pimage);
    scene.obj[1]=new triangle(Vec3f(maxn,g,maxn),Vec3f(-maxn,g,-maxn),Vec3f(maxn,g,-maxn),p,p,p,p,p,p,false,1,pimage);
    scene.obj[2]=new triangle(Vec3f(-maxn,-maxn,-hz),Vec3f(maxn,-maxn,-hz),Vec3f(-maxn,maxn,-hz),p,p,p,p,p,p,false,0,pimage);
    scene.obj[3]=new triangle(Vec3f(maxn,-maxn,-hz),Vec3f(maxn,maxn,-hz),Vec3f(-maxn,maxn,-hz),p,p,p,p,p,p,false,0,pimage);
    scene.obj[4]=new triangle(Vec3f(-hx,-hy,hz),Vec3f(hx,-hy,hz),Vec3f(-hx,-hy,-hz),p,p,p,p,p,p,false,0,pimage);
    scene.obj[5]=new triangle(Vec3f(hx,-hy,hz),Vec3f(hx,-hy,-hz),Vec3f(-hx,-hy,-hz),p,p,p,p,p,p,false,0,pimage);
    for (int i=0;i<2;i++)
    {
        scene.obj[i]->diff=0.5;
        scene.obj[i]->color=Vec3f(1,1,1);
        scene.obj[i]->ref=0.5;
        scene.obj[i]->dref=0.1;
    }
    for (int i=2;i<4;i++)
    {
        scene.obj[i]->diff=0.7;
        scene.obj[i]->color=Vec3f(23.0/255,60.0/255,44.0/255);
        scene.obj[i]->ref=0;
        scene.obj[i]->dref=0;
    }
    for (int i=4;i<6;i++)
    {
        scene.obj[i]->diff=0.7;
        scene.obj[i]->color=Vec3f(0,1,0);
        scene.obj[i]->ref=0;
        scene.obj[i]->dref=0;
    }
//
/*    for (int i=-5;i<1;i++)
        for (int j=-5;j<1;j++)
        {
            scene.obj[scene.num]=new sphere(Vec3f(i*2,j*2,-20),0.5,0,"");
            scene.obj[scene.num]->diff=1;
            if ((i+j)%2==0)
                scene.obj[scene.num]->color=Vec3f(17.0/255,63.0/255,61.0/255);
            else
                scene.obj[scene.num]->color=Vec3f(56.0/255,13.0/255,49.0/255);
            scene.num++;
        }
    scene.obj[scene.num]=new sphere(Vec3f(-5,-5,-15),4,0,"");
    scene.obj[scene.num]->diff=1;
    scene.obj[scene.num]->color=Vec3f(0,0,0);
    scene.obj[scene.num]->refcrate=1.3;
    scene.obj[scene.num]->refc=0.7;
    scene.num++;
    scene.obj[scene.num]=new sphere(Vec3f(-15,10,-15),4,0,"");
    scene.obj[scene.num]->diff=0.7;
    scene.obj[scene.num]->color=Vec3f(0.7,0.7,0.7);
    scene.obj[scene.num]->ref=1;
    scene.num++;
    scene.obj[scene.num]=new sphere(Vec3f(8,7,-10),3,1,"F:\\obj\\1.jpg");
    scene.obj[scene.num]->diff=0.7;
    scene.obj[scene.num]->color=Vec3f(0.7,0.7,0.7);
    scene.obj[scene.num]->ref=0;
    scene.num++;
    scene.obj[scene.num]=new sphere(Vec3f(-20,-19,0),1,3,"");
    scene.num++;
    scene.obj[scene.num]=new sphere(Vec3f(20,-19,0),1,3,"");
    scene.num++;
    scene.obj[scene.num]=new sphere(Vec3f(-20,-19,-40),1,3,"");
    scene.num++;
    scene.obj[scene.num]=new sphere(Vec3f(20,-19,-40),1,3,"");
    scene.num++;
    get("F:\\obj\\cube.obj",Vec3f(13,20,-15),0,"",10,Vec3f(69.0/255,137.0/255,148.0/255),0.7,0,1.3,0.7,0);
//    get("F:\\obj\\bunny.fine.obj",Vec3f(-5,20,-15),2,"F:\\obj\\2.jpg",8,Vec3f(0.7,0.7,0.7),1,0,0,0,0);
/*    scene.obj[scene.num]=new sphere(Vec3f(-13,5,-15),4,1,"F:\\obj\\1.jpg");
    scene.obj[scene.num]->diff=1;
    scene.obj[scene.num]->color=Vec3f(0,0,0);
    scene.num++;
    scene.obj[scene.num]=new sphere(Vec3f(13,5,-15),4,1,"F:\\obj\\2.jpg");
    scene.obj[scene.num]->diff=1;
    scene.obj[scene.num]->color=Vec3f(0,0,0);
    scene.num++;
    scene.obj[scene.num]=new sphere(Vec3f(0,5,-15),4,0,"");
    scene.obj[scene.num]->diff=1;
    scene.obj[scene.num]->color=Vec3f(1,1,1);
    scene.obj[scene.num]->ref=0.7;
    scene.obj[scene.num]->dref=0;
    scene.num++;*/
//    get("F:\\obj\\trashbin.obj",Vec3f(7,20,-30),2,"F:\\obj\\rustbin.jpg",8,Vec3f(0,0,0),0.7,0,0,0,0);
//    get("F:\\obj\\DEATHWING.obj",Vec3f(15,10,-30),3,"",30,Vec3f(0,0,0),0.7,0,0,0,M_PI*3/4);
//    get("F:\\obj\\pandamale.obj",Vec3f(-10,20,-30),0,"",20,Vec3f(0.7,0.7,0.7),0.7,0,0,0,M_PI*5/4);
    get("F:\\obj\\kitten.50k.obj",Vec3f(0,10,-15),0,"F:\\obj\\2.jpg",20,Vec3f(0.7,0.7,0.7),1,0,0,0,0);
//    get("F:\\obj\\output.obj",Vec3f(0,10,-15),0,"F:\\obj\\2.jpg",20,Vec3f(0.7,0.7,0.7),1,0,0,0,0);
    scene.light=new Light*[100];
    scene.LNum=2;
    scene.light[0]=new Light(Vec3f(20,-19,0));
    scene.light[0]->color=Vec3f(1,1,1);
    scene.light[1]=new Light(Vec3f(-20,-19,0));
    scene.light[1]->color=Vec3f(1,1,1);
    scene.light[2]=new Light(Vec3f(20,-29,-40));
    scene.light[2]->color=Vec3f(1,1,1);
    scene.light[3]=new Light(Vec3f(-20,-29,-40));
    scene.light[3]->color=Vec3f(1,1,1);
    mousePress=false;
}
void screen::paint()
{
    cnt=0;
    double start=clock();
    for (int i=0;i<multi*multi;i++)
         th[i]->start();
    double end=clock();
    qDebug()<<end-start<<endl;
//    qDebug()<<"yes"<<endl;
}

void screen::paintEvent(QPaintEvent *event)
{
    QPainter p(this);
    for (int i=0;i<width;i++)
        for (int j=0;j<height;j++)
        {
            p.setPen(pixel[i][j].color);
            p.drawPoint(i,j);
        }
    p.end();
}
void screen::mousePressEvent(QMouseEvent *event)
{
    if (event->button()==Qt::RightButton)
    {
        mousePress=true;
        mouseX=event->pos().x();
        mouseY=event->pos().y();
    }
    if (event->button()==Qt::LeftButton)
    {
        qDebug()<<event->pos().x()<<endl;
        qDebug()<<event->pos().y()<<endl;
    }
}
void screen::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button()==Qt::RightButton)
        mousePress=false;
}

void screen::mouseMoveEvent(QMouseEvent *event)
{
/*    if (mousePress)
    {
        double nowx=event->pos().x();
        double nowy=event->pos().y();
        double d,r;
        Vec3f tmp=scene.camera;
        r=(sqr(tmp.x)+sqr(tmp.z));
        d=2*(nowx-mouseX)/800/r;
        rotate(tmp,Vec3f(0,1,0),cos(d),sin(d));
        scene.camera=tmp;
        update();
    }*/
}
double Theta[100000];
Vec3f m_pVn[100000];
void screen::get(const char* fn,Vec3f d,int Istexture,QString filename,double size,Vec3f color,double diff,double ref,double refcrate,double refc,double theta)
{
    CSimpleObject load;
    load.LoadFromObj(fn);
    //rotate
 /*   {
        Vec3f N(0,-1,0);
        double c=cos(theta),s=sin(theta);
//        qDebug()<<c<<" "<<s<<endl;
        for (int i=0;i<load.m_nVertices;i++)
        {
//            print(load.m_pVertexList[i]);
            rotate(load.m_pVertexList[i],N,c,s);
 //           print(load.m_pVertexList[i]);
 //           system("pause");
        }
    }*/
    Vec3f max(-maxn,-maxn,-maxn);
    Vec3f min(maxn,maxn,maxn);
    for (int i=0;i<load.m_nVertices;i++)
    {
        for (int k=0;k<3;k++)
        {
            max[k]=fgetmax(max[k],load.m_pVertexList[i][k]);
            min[k]=fgetmin(min[k],load.m_pVertexList[i][k]);
        }
    }
    double maxd;
    maxd=max[0]-min[0];
    maxd=fgetmax(maxd,max[1]-min[1]);
    maxd=fgetmax(maxd,max[2]-min[2]);
    print(max);
    print(min);
    double rate=size/maxd;
 //   double rate=15;
    qDebug()<<rate<<endl;
    for (int i=0;i<load.m_nVertices;i++)
    {
        load.m_pVertexList[i][0]*=-1;
        load.m_pVertexList[i][1]*=-1;
        load.m_pVertexList[i]=load.m_pVertexList[i]*rate+d;
//        print(load.m_pVertexList[i]);
    }
    for (int i=0;i<load.m_nVertices;i++)
    {
        m_pVn[i]=Vec3f(0,0,0);
        Theta[i]=0;
    }
    for (int i=0;i<load.m_nTriangles;i++)
    {
        Vec3f p0=load.m_pVertexList[load.m_pTriangleList[i][0]];
        Vec3f p1=load.m_pVertexList[load.m_pTriangleList[i][1]];
        Vec3f p2=load.m_pVertexList[load.m_pTriangleList[i][2]];
        Vec3f v1,v2,vn;
        double theta;
        v1=p1-p0;v2=p2-p1;
        vn=cross(v1,v2);
        vn.Normalize();
        v1=p1-p0;v2=p2-p0;
        theta=acos(dot(v1,v2)/len(v1)/len(v2));
        if (fabs(theta)<=1e-9) theta=M_PI;
        m_pVn[load.m_pTriangleList[i][0]]+=vn*theta;
        Theta[load.m_pTriangleList[i][0]]+=theta;
        v1=p2-p1;v2=p0-p1;
        theta=acos(dot(v1,v2)/len(v1)/len(v2));
        if (fabs(theta)<=1e-9) theta=M_PI;
        m_pVn[load.m_pTriangleList[i][1]]+=vn*theta;
        Theta[load.m_pTriangleList[i][1]]+=theta;
        v1=p1-p2;v2=p0-p2;
        theta=acos(dot(v1,v2)/len(v1)/len(v2));
        if (fabs(theta)<=1e-9) theta=M_PI;
        m_pVn[load.m_pTriangleList[i][2]]+=vn*theta;
        Theta[load.m_pTriangleList[i][2]]+=theta;
    }
    for (int i=0;i<load.m_nVertices;i++)
    {
        m_pVn[i]/=Theta[i];
        m_pVn[i].Normalize();
    }
    bool o1;
    Vec3f p;
    Vec3f vn[3];
    Vec3f vt[3];
    QImage* text=new QImage;
    QImage *text1=new QImage;
    QImage *text2=new QImage;
    QImage *text3=new QImage;
    double u,v;
    QColor c;
    bool of=false;
    if (Istexture==2)
    {
        text->load(filename);
    }
    if (Istexture==3)
    {
        of=true;
        Istexture=2;
        text1->load("F:\\obj\\DEATHWING1.jpg");
        text2->load("F:\\obj\\DEATHWING2.jpg");
        text3->load("F:\\obj\\DEATHWING3.jpg");
    }
//    qDebug()<<Iste<<endl;
    for (int i=0;i<load.m_nTriangles;i++)
    {
        o1=true;
        for (int j=0;j<3;j++)
        {
           vn[j]=m_pVn[load.m_pTriangleList[i][j]];
        }
        if (load.m_nTriangles<1000) o1=false;
//        o1=false;
        if (Istexture==2)
        {
            if (load.m_nVt==0)
            {
                for (int j=0;j<3;j++)
                {
                     vt[j][0]=fabs(load.m_pVertexList[load.m_pTriangleList[i][j]][0]-(int)load.m_pVertexList[load.m_pTriangleList[i][j]][0]);
                     vt[j][1]=fabs(load.m_pVertexList[load.m_pTriangleList[i][j]][2]-(int)load.m_pVertexList[load.m_pTriangleList[i][j]][2]);
                }
            }
            else
            {
                for (int j=0;j<3;j++)
                {
                    vt[j][0]=fabs(load.m_pVt[load.m_pTriVt[i][j]][0]-(int)load.m_pVt[load.m_pTriVt[i][j]][0]);
                    vt[j][1]=1-fabs(load.m_pVt[load.m_pTriVt[i][j]][1]-(int)load.m_pVt[load.m_pTriVt[i][j]][1]);
//                    vt[j][0]=load.m_pVt[load.m_pTriangleList[i][j]][0];
//                    vt[j][1]=1-load.m_pVt[load.m_pTriangleList[i][j]][1];
     //               print(vt[j]);
     //               system("pause");
                }
            }
        }
        if (of)
        {
            if (load.m_ptexture[i]=="DEATHWING1")
            {
                text=text1;
            }
            if (load.m_ptexture[i]=="DEATHWING2")
            {
                text=text2;
            }
            if (load.m_ptexture[i]=="DEATHWING3")
            {
                text=text3;
            }
        }
        scene.obj[scene.num]=new triangle(
                    load.m_pVertexList[load.m_pTriangleList[i][0]],
                    load.m_pVertexList[load.m_pTriangleList[i][1]],
                    load.m_pVertexList[load.m_pTriangleList[i][2]],
                    vn[0],vn[1],vn[2],vt[0],vt[1],vt[2],o1,Istexture,text);
        scene.obj[scene.num]->color=color;
        scene.obj[scene.num]->diff=diff;
        scene.obj[scene.num]->ref=ref;
        scene.obj[scene.num]->refcrate=refcrate;
        scene.obj[scene.num]->refc=refc;
        scene.num++;
    }
    qDebug()<<scene.num<<endl;
}

screen::~screen()
{
    delete ui;
}
