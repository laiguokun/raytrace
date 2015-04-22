#ifndef KDTREE_H
#define KDTREE_H
#include "common.h"
#include "object.h"
class kdtree
{
public:
    struct Node
    {
        Node* lch;
        Node* rch;
        int k;//分割坐标
        Vec3f boxmin;
        Vec3f boxmax;
        double pos;
        int n;
        int l,r;
    };
    Scene* scene;
    kdtree(Scene*);
    int gethit(Ray ray,double& d);
    Node* root;
    int* obj;
    void buildtree(Node*);
    void sort(int **obj,int l,int r,int k);
    void travel(Ray ray,Node* node,int& crash,double& dist);
    bool inside(Vec3f p,Node* node);
    bool intersect(Ray ray,Node* node,double& t);
};

#endif // KDTREE_H
