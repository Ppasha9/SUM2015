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
  PD6_RndMatrProj;

/* Параметры проецирования */
extern DBL
  PD6_RndWp, PD6_RndHp,    /* размеры обрасти проецирования */
  PD6_RndProjDist;         /* расстояние до плоскости проекции */


/* Шейдер по умолчанию */
extern UINT PD6_RndProg;

/* Функция преобразования из мировой системы координат в кадр.
 * АРГУМЕНТЫ:
 *   - исходная точка:
 *       VEC P;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (POINT) координаты в кадре.
 */
POINT PD6_RndWorldToScreen( VEC P );

/* Функция загрузки геометрического объекта.
 * АРГУМЕНТЫ:
 *   - структура объекта для загрузки:
 *       pd6GOBJ *GObj;
 *   - имя файла:
 *       CHAR *FileName;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (BOOL) TRUE при успехе, FALSE иначе.
 */
BOOL PD6_RndGObjLoad( pd6GOBJ *GObj, CHAR *FileName );

/* Функция освобождения памяти из-под геометрического объекта.
 * АРГУМЕНТЫ:
 *   - структура объекта для загрузки:
 *       pd6GOBJ *GObj;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID PD6_RndGObjFree( pd6GOBJ *GObj );

/* Функция отрисовки геометрического объекта.
 * АРГУМЕНТЫ:
 *   - структура объекта для загрузки:
 *       pd6GOBJ *GObj;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID PD6_RndGObjDraw( pd6GOBJ *GObj );

static VOID PD6_SaveLog( CHAR *Text );


static CHAR * PD6_TextLoad( CHAR *FileName );


UINT PD6_ShaderLoad( CHAR *FileNamePrefix );


VOID PD6_ShaderFree( UINT PrgId );

#endif /* __RENDER_H_ */

/* END OF 'RENDER.H' FILE */
