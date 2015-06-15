/* FILENAME: RENDER.H
 * PROGRAMMER: PD6
 * PURPOSE: Rendering system declaration module.
 * LAST UPDATE: 09.06.2015
 */

#ifndef __RENDER_H_
#define __RENDER_H_

#include "vec.h"

#define GLEW_STATIC
#include <glew.h>
#include <gl/gl.h>
#include <gl/glu.h>

/* Тип представления объекта - набор вершин и треугольников */
typedef struct tagpd6GOBJ
{
  INT
    VA,          /* массив вершин */
    VBuf, IBuf,  /* буфера вершин и индексов */
    NumOfV,      /* количество вершин */
    NumOfF;      /* количество граней */
} pd6GOBJ;

/* Матрицы */
extern MATR
  PD6_RndMatrWorld,
  PD6_RndMatrView,
  PD6_RndMatrProj,
  PD6_RndMatrWorldViewProj;

/* Параметры проецирования */
extern DBL
  PD6_RndWp, PD6_RndHp,    /* размеры обрасти проецирования */
  PD6_RndProjDist;         /* расстояние до плоскости проекции */

/* Матрица изменения примитива при создании */
extern MATR PD6_RndPrimMatrConvert;

/* Шейдер по умолчанию */
extern UINT PD6_RndProg;

/* Функция загрузки шейдеров для одной программы.
 * АРНУМЕНТЫ:
 *   - префикс имени файла:
 *       CHAR *FileNamePrefix;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (UINT) номер загруженной программы.
 */
UINT PD6_ShaderLoad( CHAR *FileNamePrefix );

/* Функция уничтодения шейдеров для программы.
 * АРНУМЕНТЫ:
 *   - номер программы:
 *       UINT PrgId;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID PD6_ShaderFree( UINT PrgId );

/***
 * Работа с примитивами
 ***/

/* Виды примитивов */
typedef enum tagpd6PRIM_TYPE
{
  PD6_PRIM_TRIMESH,  /* примитив - набор треугольников */
  PD6_PRIM_GRID      /* регулярная сетка треугольников (через triangle strip) */
} pd6PRIM_TYPE;

/* Тип хранения примитива */
typedef struct tagpd6PRIM
{
  pd6PRIM_TYPE Type; /* Тип примитива (PD6_PRIM_***) */
  INT
    VA,          /* массив вершин */
    VBuf, IBuf,  /* буфера вершин и индексов */
    NumOfI;      /* количество индексов для вывода примитива */
} pd6PRIM;

/* Тип хранения текстурных координат */
typedef struct tagpd6UV
{
  FLT U, V; /* текстурные координаты */
} pd6UV;

/* Функция задания текстурных координат.
 * АРГУМЕНТЫ:
 *   - новые значени координат:
 *       FLT U, V;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (PD6UV) построенные координаты.
 */
__inline pd6UV UVSet( FLT U, FLT V )
{
  pd6UV UV = {U, V};

  return UV;
} /* End of 'UVSet' function */

/* Тип хранения одной вершины паримитива */
typedef struct tagpd6VERTEX
{
  VEC P;    /* позиция вершины */
  pd6UV T;  /* текстурные координаты вершины */
  VEC N;    /* нормаль в вершине */
  COLOR C;  /* цвет вершины */
} pd6VERTEX;

/* Функция создания примитива.
 * АРГУМЕНТЫ:
 *   - указатель на примитив:
 *       pd6PRIM *Prim;
 *   - тип примитива (PD6_PRIM_***):
 *       pd6PRIM_TYPE Type;
 *   - количество вершин и индексов:
 *       INT NoofV, NoofI;
 *   - массив вершин:
 *       pd6VERTEX *Vertices;
 *   - массив индексов:
 *       INT *Indices;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID PD6_PrimCreate( pd6PRIM *Prim, pd6PRIM_TYPE Type,
                     INT NoofV, INT NoofI, pd6VERTEX *Vertices, INT *Indices);

/* Функция удаления примитива.
 * АРГУМЕНТЫ:
 *   - указатель на примитив:
 *       pd6PRIM *Prim;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID PD6_PrimFree( pd6PRIM *Prim );

/* Функция отрисовки примитива.
 * АРГУМЕНТЫ:
 *   - указатель на примитив:
 *       pd6PRIM *Prim;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID PD6_PrimDraw( pd6PRIM *Prim );

/* Функция создания примитива плоскость.
 * АРГУМЕНТЫ:
 *   - указатель на примитив:
 *       pd6PRIM *Prim;
 *   - касательные вектора-стороны:
 *       VEC Du, Dv;
 *   - разбиение:
 *       INT N, M;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (BOOL) TRUE при успехе, иначе FALSE.
 */
BOOL PD6_PrimCreatePlane( pd6PRIM *Prim, VEC Du, VEC Dv, INT N, INT M );

/* Функция создания примитива сфера.
 * АРГУМЕНТЫ:
 *   - указатель на примитив:
 *       pd6PRIM *Prim;
 *   - центр сферы:
 *       VEC С;
 *   - радиус сферы:
 *       FLT R;
 *   - разбиение:
 *       INT N, M;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (BOOL) TRUE при успехе, иначе FALSE.
 */
BOOL PD6_PrimCreateSphere( pd6PRIM *Prim, VEC C, FLT R, INT N, INT M );

/* Функция загрузки геометрического объекта.
 * АРГУМЕНТЫ:
 *   - структура объекта для загрузки:
 *       pd6PRIM *GObj;
 *   - имя файла:
 *       CHAR *FileName;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (BOOL) TRUE при успехе, FALSE иначе.
 */
BOOL PD6_PrimLoad( pd6PRIM *GObj, CHAR *FileName );

/***
 * Работа с геометрическими объектами
 ***/

/* Геометрический объект - набор примитивов */
typedef struct tagpd6GEOM
{
  INT NumOfPrimitives; /* количество примитивов */
  pd6PRIM *Prims;      /* массив примитивов */
} pd6GEOM;

/* Функция добавления примитива к геометрическому объекту.
 * АРГУМЕНТЫ:
 *   - указатель на геометрический объект:
 *       pd6GEOM *G;
 *   - указатель на добавляемый примитив:
 *       pd6PRIM *Prim;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (INT) номер добавленного примитива в массиве (-1 при ошибке).
 */
INT PD6_GeomAddPrim( pd6GEOM *G, pd6PRIM *Prim );

/* Функция освобождения геометрического объекта.
 * АРГУМЕНТЫ:
 *   - указатель на геометрический объект:
 *       pd6GEOM *G;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID PD6_GeomFree( pd6GEOM *G );

/* Функция отображения геометрического объекта.
 * АРГУМЕНТЫ:
 *   - указатель на геометрический объект:
 *       pd6GEOM *G;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID PD6_GeomDraw( pd6GEOM *G );

/* Функция загрузки геометрического объекта из G3D файла.
 * АРГУМЕНТЫ:
 *   - указатель на геометрический объект:
 *       pd6GEOM *G;
 *   - имя файла:
 *       CHAR *FileName;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (BOOL) TRUE при успехе, иначе - FALSE.
 */
BOOL PD6_GeomLoad( pd6GEOM *G, CHAR *FileName );

/* Функция загрузки текстуры.
 * АРГУМЕНТЫ:
 *   - имя файла:
 *       CHAR *FileName;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (INT ) идентификатор OpenGL для текстуры.
 */
INT PD6_TextureLoad( CHAR *FileName );

#endif /* __RENDER_H_ */

/* END OF 'RENDER.H' FILE */
