/* FILENAME: RENDER.H
 * PROGRAMMER: PD6
 * PURPOSE: Rendering system declaration module.
 * LAST UPDATE: 09.06.2015
 */

#ifndef __RENDER_H_
#define __RENDER_H_

#include "vec.h"

/* ��� ������������� ������� - ����� ������ � ������������� */
typedef struct tagpd6GOBJ
{
  VEC *V;      /* ������ ������ ������� */
  INT NumOfV;  /* ���������� ������ */
  INT (*F)[3]; /* ������ ������ ������� */
  INT NumOfF;  /* ���������� ������ */
} pd6GOBJ;

/* ������� */
extern MATR
  PD6_RndMatrWorld,
  PD6_RndMatrView,
  PD6_RndMatrProj;

/* ��������� ������������� */
extern DBL
  PD6_RndWp, PD6_RndHp,    /* ������� ������� ������������� */
  PD6_RndProjDist;         /* ���������� �� ��������� �������� */

/* ������� �������������� �� ������� ������� ��������� � ����.
 * ���������:
 *   - �������� �����:
 *       VEC P;
 * ������������ ��������:
 *   (POINT) ���������� � �����.
 */
POINT PD6_RndWorldToScreen( VEC P );

/* ������� �������� ��������������� �������.
 * ���������:
 *   - ��������� ������� ��� ��������:
 *       pd6GOBJ *GObj;
 *   - ��� �����:
 *       CHAR *FileName;
 * ������������ ��������:
 *   (BOOL) TRUE ��� ������, FALSE �����.
 */
BOOL PD6_RndGObjLoad( pd6GOBJ *GObj, CHAR *FileName );

/* ������� ������������ ������ ��-��� ��������������� �������.
 * ���������:
 *   - ��������� ������� ��� ��������:
 *       pd6GOBJ *GObj;
 * ������������ ��������: ���.
 */
VOID PD6_RndGObjFree( pd6GOBJ *GObj );

/* ������� ��������� ��������������� �������.
 * ���������:
 *   - ��������� ������� ��� ��������:
 *       pd6GOBJ *GObj;
 * ������������ ��������: ���.
 */
VOID PD6_RndGObjDraw( pd6GOBJ *GObj );

#endif /* __RENDER_H_ */

/* END OF 'RENDER.H' FILE */
