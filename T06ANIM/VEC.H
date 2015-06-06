/* FILE NAME: VEC.H
 * PROGRAMMER: PD6
 * DATE: 06.06.2015
 * PURPOSE: All mathemetical samples.
 */

#ifndef __VEC_H_
#define __VEC_H_

#include <math.h>
#include <windows.h>
#define PI 3.14159265358979323846

#define DEGREE2RADIANL 0.01745329251994329576L
/* Multiplyer degree to radian constant */
long double MultiplierDegree2Radian = DEGREE2RADIANL;

#define D2R(A) ((A) * PI / 180.0)

/* базовый вещественный тип */
typedef double DBL;

/* тип для вектора в простанстве */
typedef struct tagVEC
{
  DBL X, Y, Z;
} VEC;

/* тип для матрицы - массив в структуре */
typedef struct tagMATR
{
  DBL A[4][4];
} MATR;


/* Standard function for swaping tow numbers */
__inline VOID Swap( DBL *A, DBL *B )
{
  DBL tmp = *A;

  *A = *B;
  *B = tmp;
} /* End of 'Swap' function */

/* Identity matrix */
__inline MATR MatrIdentity( VOID )
{
  MATR M =
  {
    {
      {1, 0, 0, 0},
      {0, 1, 0, 0},
      {0, 0, 1, 0},
      {0, 0, 0, 1}
    }
  };

  return M;
} /* End of 'MatrIdentity' function */

/* Matrix for translating */
__inline MATR MatrTranslate( DBL Dx, DBL Dy, DBL Dz )
{
  MATR M =
  {
    {
      { 1,  0,  0, 0},
      { 0,  1,  0, 0},
      { 0,  0,  1, 0},
      {Dx, Dy, Dz, 1}
    }
  };

  return M;
} /* End of 'MatrTranslate' function */

/* Matrix for scaling */
__inline MATR MatrScale( DBL Sx, DBL Sy, DBL Sz )
{
  MATR M =
  {
    {
      {Sx,  0,  0, 0},
      { 0, Sy,  0, 0},
      { 0,  0, Sz, 0},
      { 0,  0,  0, 1}
    }
  };

  return M;
} /* End of 'MatrScale' function */

/* Matrix for rotating by x */
__inline MATR MatrRotateX( DBL AngleInDegree )
{
  MATR M = MatrIdentity();
  DBL sine, cosine;

  __asm {
    /* FST(0) Angle (from degree to radian) */
    fld  AngleInDegree
    fmul MultiplierDegree2Radian
    
    /* FST(0) - cos, FST(1) - sin */
    fsincos
    fstp cosine /* cos -> cosine */
    fstp sine   /* sin -> sine */
  }
  M.A[1][1] =  M.A[2][2] = cosine;
  M.A[1][2] = sine;
  M.A[2][1] = -sine;

  return M;
} /* End of 'MatrRotateX' function */


/* Matrix for rotating by y */
__inline MATR MatrRotateY( DBL AngleInDegree )
{
  MATR M = MatrIdentity();
  DBL sine, cosine;

  __asm {
    /* FST(0) Angle (from degree to radian) */
    fld  AngleInDegree
    fmul MultiplierDegree2Radian
    
    /* FST(0) - cos, FST(1) - sin */
    fsincos
    fstp cosine /* cos -> cosine */
    fstp sine   /* sin -> sine */
  }
  M.A[0][0] =  M.A[2][2] = cosine;
  M.A[2][0] = sine;
  M.A[0][2] = -sine;

  return M;
} /* End of 'MatrRotateY' function */

/* Matrix for rotating by z */
__inline MATR MatrRotateZ( DBL AngleInDegree )
{
  MATR M = MatrIdentity();
  DBL sine, cosine;

  __asm {
    /* FST(0) Angle (from degree to radian) */
    fld  AngleInDegree
    fmul MultiplierDegree2Radian
    
    /* FST(0) - cos, FST(1) - sin */
    fsincos
    fstp cosine /* cos -> cosine */
    fstp sine   /* sin -> sine */
  }
  M.A[0][0] =  M.A[1][1] = cosine;
  M.A[0][1] = sine;
  M.A[1][0] = -sine;

  return M;
} /* End of 'MatrRotateZ' function */

/* Matrix for rotating by random radius vector */
__inline MATR MatrRotate( DBL AngleDegree, DBL X, DBL Y, DBL Z )
{
  DBL a, si, co, len;
  MATR M;

  /* correct source parameters */
  a = D2R(AngleDegree);
  si = sin(a);
  co = cos(a);
  len = X * X + Y * Y + Z * Z;
  if (len != 0 && len != 1)
    len = sqrt(len), X /= len, Y /= len, Z /= len;

  /* Build quaternion matrix */
  X *= si;
  Y *= si;
  Z *= si;

  M.A[0][0] = 1 - 2 * (Y * Y + Z * Z);
  M.A[0][1] = 2 * X * Y - 2 * co * Z;
  M.A[0][2] = 2 * co * Y + 2 * X * Z;
  M.A[0][3] = 0;

  M.A[1][0] = 2 * X * Y + 2 * co * Z;
  M.A[1][1] = 1 - 2 * (X * X + Z * Z);
  M.A[1][2] = 2 * Y * Z - 2 * co * X;
  M.A[1][3] = 0;

  M.A[2][0] = 2 * X * Z - 2 * co * Y;
  M.A[2][1] = 2 * co * X + 2 * Y * Z;
  M.A[2][2] = 1 - 2 * (X * X + Y * Y);
  M.A[2][3] = 0;

  M.A[3][0] = 0;
  M.A[3][1] = 0;
  M.A[3][2] = 0;
  M.A[3][3] = 1;
  return M;
} /* End of 'MatrRotate' function */

/* Transposing the matrix */
__inline MATR MatrTranspose( MATR M )
{
  INT i, j;

  for (i = 0; i < 4; i++)
    for (j = 0; j < 4; j++)
      if (j <= i)
        Swap(&M.A[i][j], &M.A[j][i]);

  return M;
} /* End of 'MatrTranspose' function */

/* Multiply the matrixes */
__inline MATR MatrMulMatr( MATR M1, MATR M2 )
{
  MATR M;
  INT i, j, k;

  for (i = 0; i < 4; i++)
    for (j = 0; j < 4; j++)
      for (M.A[i][j] = 0, k = 0; k < 4; k++)
        M.A[i][j] += M1.A[i][k] * M2.A[k][j];
  return M;
} /* End of 'MatrMulMatr' function */

/* Determinant for matrix 3x3 */
__inline DBL MatrDeterm3x3( DBL A11, DBL A12, DBL A13,
                   DBL A21, DBL A22, DBL A23,
                   DBL A31, DBL A32, DBL A33 )
{
  return A11 * A22 * A33 + A12 * A23 * A31 + A13 * A21 * A32 +
        -A11 * A23 * A32 - A12 * A21 * A33 - A13 * A22 * A31;
} /* End of 'MatrDeterm3x3' function */

/* Counting the determinant of matrix 4x4 */
__inline DBL MatrDeterm( MATR M )
{
  return
    M.A[0][0] * MatrDeterm3x3(M.A[1][1], M.A[1][2], M.A[1][3],
                              M.A[2][1], M.A[2][2], M.A[2][3],
                              M.A[3][1], M.A[3][2], M.A[3][3]) -
    M.A[0][1] * MatrDeterm3x3(M.A[1][0], M.A[1][2], M.A[1][3],
                              M.A[2][0], M.A[2][2], M.A[2][3],
                              M.A[3][0], M.A[3][2], M.A[3][3]) +
    M.A[0][2] * MatrDeterm3x3(M.A[1][0], M.A[1][1], M.A[1][3],
                              M.A[2][0], M.A[2][1], M.A[2][3],
                              M.A[3][0], M.A[3][1], M.A[3][3]) -
    M.A[0][3] * MatrDeterm3x3(M.A[1][0], M.A[1][1], M.A[1][2],
                              M.A[2][0], M.A[2][1], M.A[2][2],
                              M.A[3][0], M.A[3][1], M.A[3][2]);
} /* End of 'MatrDeterm' function */

/* Inverse matrix of source matrix */
__inline MATR MatrInverse( MATR M )
{
  MATR r;
  DBL det = MatrDeterm(M);
/*  INT p[4][3] =
  {
    {1, 2, 3},
    {0, 2, 3},
    {0, 1, 3},
    {0, 1, 2}
  };*/

  if (det == 0)
    return MatrIdentity();

  r.A[0][0] = MatrDeterm3x3(M.A[1][1], M.A[1][2], M.A[1][3],
                            M.A[2][1], M.A[2][2], M.A[2][3],
                            M.A[3][1], M.A[3][2], M.A[3][3]) / det;
  r.A[1][0] = -MatrDeterm3x3(M.A[1][0], M.A[1][2], M.A[1][3],
                            M.A[2][0], M.A[2][2], M.A[2][3],
                            M.A[3][0], M.A[3][2], M.A[3][3]) / det;
  r.A[2][0] = MatrDeterm3x3(M.A[1][0], M.A[1][1], M.A[1][3],
                            M.A[2][0], M.A[2][1], M.A[2][3],
                            M.A[3][0], M.A[3][1], M.A[3][3]) / det;
  r.A[3][0] = -MatrDeterm3x3(M.A[1][0], M.A[1][1], M.A[1][2],
                            M.A[2][0], M.A[2][1], M.A[2][2],
                            M.A[3][0], M.A[3][1], M.A[3][2]) / det;

  r.A[0][1] = -MatrDeterm3x3(M.A[0][1], M.A[0][2], M.A[0][3],
                            M.A[2][1], M.A[2][2], M.A[2][3],
                            M.A[3][1], M.A[3][2], M.A[3][3]) / det;
  r.A[1][1] = MatrDeterm3x3(M.A[0][0], M.A[0][2], M.A[0][3],
                            M.A[2][0], M.A[2][2], M.A[2][3],
                            M.A[3][0], M.A[3][2], M.A[3][3]) / det;
  r.A[2][1] = -MatrDeterm3x3(M.A[0][0], M.A[0][1], M.A[0][3],
                            M.A[2][0], M.A[2][1], M.A[2][3],
                            M.A[3][0], M.A[3][1], M.A[3][3]) / det;
  r.A[3][1] = MatrDeterm3x3(M.A[0][0], M.A[0][1], M.A[0][2],
                            M.A[2][0], M.A[2][1], M.A[2][2],
                            M.A[3][0], M.A[3][1], M.A[3][2]) / det;

  r.A[0][2] = MatrDeterm3x3(M.A[0][1], M.A[0][2], M.A[0][3],
                            M.A[1][1], M.A[1][2], M.A[1][3],
                            M.A[3][1], M.A[3][2], M.A[3][3]) / det;
  r.A[1][2] = -MatrDeterm3x3(M.A[0][0], M.A[0][2], M.A[0][3],
                            M.A[1][0], M.A[1][2], M.A[1][3],
                            M.A[3][0], M.A[3][2], M.A[3][3]) / det;
  r.A[2][2] = MatrDeterm3x3(M.A[0][0], M.A[0][1], M.A[0][3],
                            M.A[1][0], M.A[1][1], M.A[1][3],
                            M.A[3][0], M.A[3][1], M.A[3][3]) / det;
  r.A[3][2] = -MatrDeterm3x3(M.A[0][0], M.A[0][1], M.A[0][2],
                            M.A[1][0], M.A[1][1], M.A[1][2],
                            M.A[3][0], M.A[3][1], M.A[3][2]) / det;

  r.A[0][3] = -MatrDeterm3x3(M.A[0][1], M.A[0][2], M.A[0][3],
                            M.A[1][1], M.A[1][2], M.A[1][3],
                            M.A[2][1], M.A[2][2], M.A[2][3]) / det;
  r.A[1][3] = MatrDeterm3x3(M.A[0][0], M.A[0][2], M.A[0][3],
                            M.A[1][0], M.A[1][2], M.A[1][3],
                            M.A[2][0], M.A[2][2], M.A[2][3]) / det;
  r.A[2][3] = -MatrDeterm3x3(M.A[0][0], M.A[0][1], M.A[0][3],
                            M.A[1][0], M.A[1][1], M.A[1][3],
                            M.A[2][0], M.A[2][1], M.A[2][3]) / det;
  r.A[3][3] = MatrDeterm3x3(M.A[0][0], M.A[0][1], M.A[0][2],
                            M.A[1][0], M.A[1][1], M.A[1][2],
                            M.A[2][0], M.A[2][1], M.A[2][2]) / det;

/*  for (i = 0; i < 4; i++)
    for (j = 0; j < 4; j++)
      if ((2 * (i + j) - 1) % 2 == 0)
        r.A[j][i] =
          MatrDeterm3x3(M.A[p[i][0]][p[j][0]], M.A[p[i][0]][p[j][1]], M.A[p[i][0]][p[j][2]],
                        M.A[p[i][1]][p[j][0]], M.A[p[i][1]][p[j][1]], M.A[p[i][1]][p[j][2]],
                        M.A[p[i][2]][p[j][0]], M.A[p[i][2]][p[j][1]], M.A[p[i][2]][p[j][2]]) / det;
      else
        r.A[j][i] =
          -MatrDeterm3x3(M.A[p[i][0]][p[j][0]], M.A[p[i][0]][p[j][1]], M.A[p[i][0]][p[j][2]],
                        M.A[p[i][1]][p[j][0]], M.A[p[i][1]][p[j][1]], M.A[p[i][1]][p[j][2]],
                        M.A[p[i][2]][p[j][0]], M.A[p[i][2]][p[j][1]], M.A[p[i][2]][p[j][2]]) / det;*/


  return r;
} /* End of 'MatrInverse' function */


/* Vectors */
/* Function for setting the vector by coordinats */
__inline VEC VecSet( DBL X, DBL Y, DBL Z )
{
  VEC r = {X, Y, Z};

  return r;
} /* End of 'VecSet' function */

/* Adding two vectors */
__inline VEC VecAddVec( VEC A, VEC B )
{
  return VecSet(A.X + B.X, A.Y + B.Y, A.Z + B.Z);
} /* End of 'VecAddVec' function */

/* Subtraction two vectors */
__inline VEC VecSubVec( VEC A, VEC B )
{
  return VecSet(A.X - B.X, A.Y - B.Y, A.Z - B.Z);
} /* End of 'VecSubVec' function */

/* Multiply vector on number */
__inline VEC VecMulNum( VEC A, DBL N )
{
  return VecSet(A.X * N, A.Y * N, A.Z * N);
} /* End of 'VecMulNum' function */

/* Division vector on number */
__inline VEC VecDivNum( VEC A, DBL N )
{
  return VecSet(A.X / N, A.Y / N, A.Z / N);
} /* End of 'VecDivNum' function */

/* Opposite vector */
__inline VEC VecNeg( VEC A )
{
  return VecSet(-A.X, -A.Y, -A.Z);
} /* End of 'VecNeg' function */

/* Dot product two vectors */
__inline DBL VecDotVec( VEC A, VEC B )
{
  return (A.X * B.X + A.Y * B.Y + A.Z * B.Z);
} /* End of 'VecDotVec' function */

/* Cross product two vectors */
__inline VEC VecCrossVec( VEC A, VEC B )
{
  return VecSet(A.Y * B.Z - A.Z * B.Y, A.Z * B.X - A.X * B.Z, A.X * B.Y - A.Y * B.X);
} /* End of 'VecCrossVec' function */

/* Sqr length of vector */
__inline DBL VecLen2( VEC V )
{
  return (V.X * V.X + V.Y * V.Y + V.Z * V.Z);
} /* End of 'VecLen2' function */

/* Length of vector */
__inline DBL VecLen( VEC V )
{
  return sqrt(VecLen2(V));
} /* End of 'VecLen' function */

/* Normalize the vector */
__inline VEC VecNormalize( VEC V )
{
  return VecSet(V.X / VecLen(V), V.Y / VecLen(V), V.Z / VecLen(V));
} /* End of 'VecNormalize' function */

/* Transformation the point by matrix */
__inline VEC PointTransform( VEC V, MATR M )
{
  return VecSet(
    V.X * M.A[0][0] + V.Y * M.A[1][0] + V.Z * M.A[2][0] + M.A[3][0],
    V.X * M.A[0][1] + V.Y * M.A[1][1] + V.Z * M.A[2][1] + M.A[3][1],
    V.X * M.A[0][2] + V.Y * M.A[1][2] + V.Z * M.A[2][2] + M.A[3][2]);
} /* End of 'PointTransform' function */

/* Transformation the vector by matrix */
__inline VEC VectorTransform( VEC V, MATR M )
{
  return VecSet(
    V.X * M.A[0][0] + V.Y * M.A[1][0] + V.Z * M.A[2][0],
    V.X * M.A[0][1] + V.Y * M.A[1][1] + V.Z * M.A[2][1],
    V.X * M.A[0][2] + V.Y * M.A[1][2] + V.Z * M.A[2][2]);
} /* End of 'VectorTransform' function */

/* Rotate vector around X axis */
VEC VecRotateX( VEC P, DOUBLE AngleDegree )
{
  DOUBLE a = D2R(AngleDegree), si = sin(a), co = cos(a);
  VEC r;

  r.X = P.X;
  r.Y = P.Y * co - P.Z * si;
  r.Z = P.Y * si + P.Z * co;
  return r;
} /* End of 'VecRotateX' function */

/* Rotate vector around X axis */
VEC VecRotateY( VEC P, DOUBLE AngleDegree )
{
  DOUBLE a = D2R(AngleDegree), si = sin(a), co = cos(a);
  VEC r;

  r.Y = P.Y;
  r.X = P.X * co - P.Z * si;
  r.Z = P.X * si + P.Z * co;
  return r;
} /* End of 'VecRotateX' function */

/* Rotate vector around X axis */
VEC VecRotateZ( VEC P, DOUBLE AngleDegree )
{
  DOUBLE a = D2R(AngleDegree), si = sin(a), co = cos(a);
  VEC r;

  r.Z = P.Z;
  r.Y = P.X * co - P.Y * si;
  r.Z = P.X * si + P.Y * co;
  return r;
} /* End of 'VecRotateX' function */

/* Equal */
__inline VEC VecAddVecEq( VEC *VRes, VEC V2 )
{
  VRes->X += V2.X;
  VRes->Y += V2.Y;
  VRes->Z += V2.Z;
  return *VRes;
} /* End of 'VecAddVecEq' function */

#endif /* __VEC_H_ */