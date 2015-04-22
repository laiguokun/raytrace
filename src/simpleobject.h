#ifndef SIMPLEOBJECT_H
#define SIMPLEOBJECT_H

#pragma once
#include "Vec3f.h"
#include <vector>
#include <cstdio>
#include <cstring>
#include <string>
using namespace std;
    template <typename T, int N> class Array
    {
    public:
        enum {_len = N};
        typedef T t_Val;
    public:
        T& operator[] (int i)
        {
            return _p[i];
        }
        const T& operator[] (int i) const
        {
            return _p[i];
        }

    protected:
        T _p[N];
    };

    class CSimpleObject
    {
    public:
        CSimpleObject(void);
        ~CSimpleObject(void);

    public:
        bool IsLoaded() { return m_pVertexList!=NULL;}

        void Destroy();
        bool LoadFromObj(const char* fn);
        bool SaveToObj(const char* fn);

    protected:
        bool Parse(FILE* fp);
        bool CheckParse(int nVertices,std::vector<Array<int,3> > & vecTriangles);

    public:

        int             m_nVertices;
        int             m_nVt;
        int             m_nVn;
        int             m_nTriangles;
        Vec3f*          m_pVertexList;
        Vec3f*          m_pVt;
        Vec3f*          m_pVn;
        Array<int,3>*   m_pTriangleList;
        Array<int,3>*   m_pTriVt;
        Array<int,3>*   m_pTriVn;
        std::string*         m_ptexture;
    };

#endif // SIMPLEOBJECT_H
