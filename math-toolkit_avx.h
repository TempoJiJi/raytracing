#ifndef __RAY_MATH_TOOLKIT_H
#define __RAY_MATH_TOOLKIT_H

#include <math.h>
#include <stdio.h>
#include <assert.h>
#include <xmmintrin.h>
#include <immintrin.h>
#include <avxintrin.h>

static inline __forceinline
void normalize(double *v)
{
    double d = sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
    assert(d != 0.0 && "Error calculating normal");

    v[0] /= d;
    v[1] /= d;
    v[2] /= d;
}

static inline __forceinline
double length(const double *v)
{
    return sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
}

static inline __forceinline
void add_vector(const double *a, const double *b, double *out)
{
    out[0] = a[0] + b[0];
    out[1] = a[1] + b[1];
    out[2] = a[2] + b[2];
}

static inline __forceinline
void subtract_vector(const double *a, const double *b, double *out)
{
    __m256d A = _mm256_set_pd(0,a[2],a[1],a[0]);
    __m256d B = _mm256_set_pd(0,b[2],b[1],b[0]);
    __m256d OUT = _mm256_sub_pd(A,B);
    out[0] = OUT[0];
    out[1] = OUT[1];
    out[2] = OUT[2];
}

static inline __forceinline
void multiply_vectors(const double *a, const double *b, double *out)
{
    out[0] = a[0] * b[0];
    out[1] = a[1] * b[1];
    out[2] = a[2] * b[2];
}

static inline __forceinline
void multiply_vector(const double *a, double b, double *out)
{
    out[0] = a[0] * b;
    out[1] = a[1] * b;
    out[2] = a[2] * b;
}

static inline __forceinline
void cross_product(const double *v1, const double *v2, double *out)
{
    out[0] = v1[1] * v2[2] - v1[2] * v2[1];
    out[1] = v1[2] * v2[0] - v1[0] * v2[2];
    out[2] = v1[0] * v2[1] - v1[1] * v2[0];
}

static inline __forceinline
double dot_product(const double *v1, const double *v2)
{
    __m256d x = _mm256_set_pd(0,v1[2],v1[1],v1[0]);
    __m256d y = _mm256_set_pd(0,v2[2],v2[1],v2[0]);
    __m256d xy = _mm256_mul_pd(x,y);
    return (xy[0]+xy[1]+xy[2]);
}

static inline __forceinline
void scalar_triple_product(const double *u, const double *v, const double *w,
                           double *out)
{
    cross_product(v, w, out);
    multiply_vectors(u, out, out);
}

static inline __forceinline
double scalar_triple(const double *u, const double *v, const double *w)
{
    double tmp[3];
    cross_product(w, u, tmp);
    return dot_product(v, tmp);
}

#endif
