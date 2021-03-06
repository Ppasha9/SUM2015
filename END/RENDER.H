/* FILENAME: RENDER.H
 * PROGRAMMER: PD6
 * PURPOSE: Rendering system declaration module.
 * LAST UPDATE: 09.06.2015
 */

#ifndef __RENDER_H_
#define __RENDER_H_

#include "vec.h"
#include "camera.h"

#define GLEW_STATIC
#include <glew.h>
#include <gl/gl.h>
#include <gl/glu.h>

/* ��� ������������� ������� - ����� ������ � ������������� */
typedef struct tagpd6GOBJ
{
  INT
    VA,          /* ������ ������ */
    VBuf, IBuf,  /* ������ ������ � �������� */
    NumOfV,      /* ���������� ������ */
    NumOfF;      /* ���������� ������ */
} pd6GOBJ;

/* ������� */
extern MATR
  PD6_RndMatrWorld,
  PD6_RndMatrView,
  PD6_RndMatrProj,
  PD6_RndMatrWorldViewProj;

/* ��������� ������������� */
extern DBL
  PD6_RndWp, PD6_RndHp,    /* ������� ������� ������������� */
  PD6_RndProjDist;         /* ���������� �� ��������� �������� */

/* ������� ��������� ��������� ��� �������� */
extern MATR PD6_RndPrimMatrConvert;

extern MATR PD6_HelicMatr;

/* ������ �� ��������� */
extern UINT PD6_RndProg, PD6_WaterProg, PD6_HelicProg;

/* ������� �������� �������� ��� ����� ���������.
 * ���������:
 *   - ������� ����� �����:
 *       CHAR *FileNamePrefix;
 * ������������ ��������:
 *   (UINT) ����� ����������� ���������.
 */
UINT PD6_ShaderLoad( CHAR *FileNamePrefix );

/* ������� ����������� �������� ��� ���������.
 * ���������:
 *   - ����� ���������:
 *       UINT PrgId;
 * ������������ ��������: ���.
 */
VOID PD6_ShaderFree( UINT PrgId );

/***
 * ������ � �����������
 ***/

/* ��� ������� ��������� ����������� */
typedef struct tagpd6MATERIAL
{
  /* ��� ��������� */
  CHAR Name[300];

  /* ������������ ��������� */
  VEC Ka, Kd, Ks;
  FLT Kp; /* ������� "����������" */

  /* ����������� ������������ */
  FLT Kt;

  /* �������� ����������� */
  INT TexId;
} pd6MATERIAL;

/* ���������� ������� ���������� */
#define PD6_MATERIAL_MAX 300
extern INT PD6_MtlLibSize;
extern pd6MATERIAL PD6_MtlLib[PD6_MATERIAL_MAX];

/* ������� ���������� ��������� � ����������.
 * ���������:
 *   - ��������� � ����������� ���������:
 *       pd6MATERIAL *Mtl;
 * ������������ ��������:
 *   (INT) ���������� ����� ������������ ��������� (0 ��� ������).
 */
INT PD6_MtlAdd( pd6MATERIAL *Mtl );

/* ������� ������ ��������� �� �����.
 * ���������:
 *   - ��� ���������:
 *       CHAR *Name;
 * ������������ ��������:
 *   (INT) ���������� ����� ���������� ��������� (0 ��� ������).
 */
INT PD6_MtlFind( CHAR *Name );

/* ������� �������� ��������� �� ����� (*.MTL).
 * ���������:
 *   - ��� ����� ���������:
 *       CHAR *FileName;
 * ������������ ��������:
 *   (INT) ���������� ����������� ����������.
 */
INT PD6_MtlLoad( CHAR *FileName );

/***
 * ������ � �����������
 ***/

/* ���� ���������� */
typedef enum tagpd6PRIM_TYPE
{
  PD6_PRIM_TRIMESH,  /* �������� - ����� ������������� */
  PD6_PRIM_GRID      /* ���������� ����� ������������� (����� triangle strip) */
} pd6PRIM_TYPE;

/* ��� �������� ��������� */
typedef struct tagpd6PRIM
{
  pd6PRIM_TYPE Type; /* ��� ��������� (PD6_PRIM_***) */
  INT
    VA,          /* ������ ������ */
    VBuf, IBuf,  /* ������ ������ � �������� */
    NumOfI,      /* ���������� �������� ��� ������ ��������� */
    MtlNo;
  MATR M;        /* ������� ��������� */
} pd6PRIM;

/* ��� �������� ���������� ��������� */
typedef struct tagpd6UV
{
  FLT U, V; /* ���������� ���������� */
} pd6UV;

/* ������� ������� ���������� ���������.
 * ���������:
 *   - ����� ������� ���������:
 *       FLT U, V;
 * ������������ ��������:
 *   (PD6UV) ����������� ����������.
 */
__inline pd6UV UVSet( FLT U, FLT V )
{
  pd6UV UV = {U, V};

  return UV;
} /* End of 'UVSet' function */

/* ��� �������� ����� ������� ���������� */
typedef struct tagpd6VERTEX
{
  VEC P;    /* ������� ������� */
  pd6UV T;  /* ���������� ���������� ������� */
  VEC N;    /* ������� � ������� */
  COLOR C;  /* ���� ������� */
} pd6VERTEX;

/* ������� �������� ���������.
 * ���������:
 *   - ��������� �� ��������:
 *       pd6PRIM *Prim;
 *   - ��� ��������� (PD6_PRIM_***):
 *       pd6PRIM_TYPE Type;
 *   - ���������� ������ � ��������:
 *       INT NoofV, NoofI;
 *   - ������ ������:
 *       pd6VERTEX *Vertices;
 *   - ������ ��������:
 *       INT *Indices;
 * ������������ ��������: ���.
 */
VOID PD6_PrimCreate( pd6PRIM *Prim, pd6PRIM_TYPE Type,
                     INT NoofV, INT NoofI, pd6VERTEX *Vertices, INT *Indices);

/* ������� �������� ���������.
 * ���������:
 *   - ��������� �� ��������:
 *       pd6PRIM *Prim;
 * ������������ ��������: ���.
 */
VOID PD6_PrimFree( pd6PRIM *Prim );

/* ������� ��������� ���������.
 * ���������:
 *   - ��������� �� ��������:
 *       pd6PRIM *Prim;
 * ������������ ��������: ���.
 */
VOID PD6_PrimDraw( pd6PRIM *Prim );

/* ������� ��������� ��������� "��������".
 * ���������:
 *   - ��������� �� ��������:
 *       pd6PRIM *Prim;
 * ������������ ��������: ���.
 */
VOID PD6_PrimHelicDraw( pd6PRIM *Prim );

/* ������� ��������� ��������� "����".
 * ���������:
 *   - ��������� �� ��������:
 *       pd6PRIM *Prim;
 * ������������ ��������: ���.
 */
VOID PD6_PrimWaterDraw( pd6PRIM *Prim );

/* ������� �������� ��������� ���������.
 * ���������:
 *   - ��������� �� ��������:
 *       pd6PRIM *Prim;
 *   - ������� �����:
 *       VEC Loc;
 *   - ����������� �������-�������:
 *       VEC Du, Dv;
 *   - ���������:
 *       INT N, M;
 * ������������ ��������:
 *   (BOOL) TRUE ��� ������, ����� FALSE.
 */
BOOL PD6_PrimCreatePlane( pd6PRIM *Prim, VEC Loc, VEC Du, VEC Dv, INT N, INT M );

/* ������� �������� ��������� �����.
 * ���������:
 *   - ��������� �� ��������:
 *       pd6PRIM *Prim;
 *   - ����� �����:
 *       VEC �;
 *   - ������ �����:
 *       FLT R;
 *   - ���������:
 *       INT N, M;
 * ������������ ��������:
 *   (BOOL) TRUE ��� ������, ����� FALSE.
 */
BOOL PD6_PrimCreateSphere( pd6PRIM *Prim, VEC C, FLT R, INT N, INT M );

/* ������� �������� ��������� ����� �����.
 * ���������:
 *   - ��������� �� ��������:
 *       pd6PRIM *Prim;
 *   - ������� �����:
 *       VEC Loc;
 *   - ����������� �������-�������:
 *       VEC Du, Dv;
 *   - ������� �� ������:
 *       DBL Scale;
 *   - ��� ����� � ��������:
 *       CHAR *FileName;
 * ������������ ��������:
 *   (BOOL) TRUE ��� ������, ����� FALSE.
 */
BOOL PD6_PrimCreateHeightField( pd6PRIM *Prim, VEC Loc, VEC Du, VEC Dv, DBL Scale, CHAR *FileName );

/* ������� �������� ��������������� �������.
 * ���������:
 *   - ��������� ������� ��� ��������:
 *       pd6PRIM *GObj;
 *   - ��� �����:
 *       CHAR *FileName;
 * ������������ ��������:
 *   (BOOL) TRUE ��� ������, FALSE �����.
 */
BOOL PD6_PrimLoad( pd6PRIM *GObj, CHAR *FileName );

/***
 * ������ � ��������������� ���������
 ***/

/* �������������� ������ - ����� ���������� */
typedef struct tagpd6GEOM
{
  INT NumOfPrimitives; /* ���������� ���������� */
  pd6PRIM *Prims;      /* ������ ���������� */
} pd6GEOM;

/* ������� ���������� ��������� � ��������������� �������.
 * ���������:
 *   - ��������� �� �������������� ������:
 *       pd6GEOM *G;
 *   - ��������� �� ����������� ��������:
 *       pd6PRIM *Prim;
 * ������������ ��������:
 *   (INT) ����� ������������ ��������� � ������� (-1 ��� ������).
 */
INT PD6_GeomAddPrim( pd6GEOM *G, pd6PRIM *Prim );

/* ������� ������������ ��������������� �������.
 * ���������:
 *   - ��������� �� �������������� ������:
 *       pd6GEOM *G;
 * ������������ ��������: ���.
 */
VOID PD6_GeomFree( pd6GEOM *G );

/* ������� ����������� ��������������� �������.
 * ���������:
 *   - ��������� �� �������������� ������:
 *       pd6GEOM *G;
 * ������������ ��������: ���.
 */
VOID PD6_GeomDraw( pd6GEOM *G );

/* ������� ����������� ��������������� ������� - ���������.
 * ���������:
 *   - ��������� �� �������������� ������:
 *       pd6GEOM *G;
 * ������������ ��������: ���.
 */
VOID PD6_GeomHelicDraw( pd6GEOM *G );

/* ������� �������� ��������������� ������� �� G3D �����.
 * ���������:
 *   - ��������� �� �������������� ������:
 *       pd6GEOM *G;
 *   - ��� �����:
 *       CHAR *FileName;
 * ������������ ��������:
 *   (BOOL) TRUE ��� ������, ����� - FALSE.
 */
BOOL PD6_GeomLoad( pd6GEOM *G, CHAR *FileName );

/* ������� �������� ��������.
 * ���������:
 *   - ��� �����:
 *       CHAR *FileName;
 * ������������ ��������:
 *   (INT ) ������������� OpenGL ��� ��������.
 */
INT PD6_TextureLoad( CHAR *FileName );

#endif /* __RENDER_H_ */

/* END OF 'RENDER.H' FILE */
