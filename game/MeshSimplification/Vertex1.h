#ifndef VERTEX_H
#define VERTEX_H

#include "Vec3D.h"

/************************************************************
 * Vertex of the mesh (containing normal n and vertetx v)
 ************************************************************/
class Vertex1 {
public:
    inline Vertex1 () {}
    inline Vertex1 (const Vec3Df & p) : p (p) {}
    inline Vertex1 (const Vec3Df & p, const Vec3Df & n) : p (p), n (n) {}
    inline Vertex1 (const Vertex1 & v) : p (v.p), n (v.n) {}
    inline virtual ~Vertex1 () {}
    inline Vertex1 & operator= (const Vertex1 & v) {
        p = v.p;
        n = v.n;
        return (*this);
    }
    Vec3Df p;
    Vec3Df n;
};


#endif // VERTEX_H
