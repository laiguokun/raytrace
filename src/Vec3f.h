#ifndef VEC3F_H
#define VEC3F_H

class Vec3f
{
public:

    //Constructors
    Vec3f();
    Vec3f(double x,double y, double z);
    Vec3f(const Vec3f& v);
    //Deconstructor
    virtual ~Vec3f();
public:
    //Operators

    //Operator []
    double& operator [](int index)
    {
        return _p[index];
    }
    const double& operator [](int index) const
    {
        return _p[index];
    }

    //Operator =
    Vec3f& operator = (const Vec3f& v);

    //Operators +=,-=, *=, /=
    void operator +=(const Vec3f& v);
    void operator +=(double f);
    void operator -=(const Vec3f& v);
    void operator -=(double f);
    void operator *=(const Vec3f& v);
    void operator *=(double f);
    void operator /=(const Vec3f& v);
    void operator /=(double f);

    //Operators +,-.*,/
    Vec3f operator +(const Vec3f&v) const;
    Vec3f operator +(double f) const;
    Vec3f operator -(const Vec3f&v) const;
    Vec3f operator -(double f) const;
    Vec3f operator *(const Vec3f&v) const;
    Vec3f operator *(double f) const;
    Vec3f operator /(const Vec3f&v) const;
    Vec3f operator /(double f) const;

    Vec3f operator -() const;

public:
    void Normalize();
    double L2Norm_Sqr();

public:
    union
    {
        struct
        { double _p[3]; };
        struct
        { double x,y,z; };
        struct
        { double r,g,b; };
    };
    enum {_len = 3};

};



#endif // VEC3F_H
