/* FILENAME: URECT.C
 * PROGRAMMER: PD6
 * PURPOSE: Bounce rectangle unit handle module.
 * LAST UPDATE: 08.06.2015
 */

#include <string.h>
#include <time.h>

#include "anim.h"
#include "vec.h"

/* ��� ������������� ���� */
typedef struct tagpd6UNIT_RECT
{
  PD6_UNIT_BASE_FIELDS;

  VEC Pos;     /* ������� �������� */
  DWORD Color; /* ���� �������� */
  DBL
    PhaseShift; /* ����� �� ���� */
} pd6UNIT_RECT;


/* ������� ������������� ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       pd6UNIT_BALL *Uni;
 *   - ��������� �� �������� ��������:
 *       pd6ANIM *Ani;
 * ������������ ��������: ���.
 */
static VOID PD6_RectUnitInit( pd6UNIT_RECT *Uni, pd6ANIM *Ani )
{
  DBL t = clock() / (DOUBLE)CLOCKS_PER_SEC;

  Uni->Pos = VecSet(rand() % 1920, rand() % 1080, 0);
  Uni->Color = RGB(rand() % 256, rand() % 256, rand() % 256);
  Uni->PhaseShift = t * 30 * 59;
} /* End of 'PD6_AnimUnitInit' function */

/* ������� ��������������� ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       pd6UNIT_BALL *Uni;
 *   - ��������� �� �������� ��������:
 *       pd6ANIM *Ani;
 * ������������ ��������: ���.
 */
static VOID PD6_RectUnitClose( pd6UNIT_RECT *Uni, pd6ANIM *Ani )
{
} /* End of 'PD6_AnimUnitClose' function */

/* ������� ���������� ����������� ���������� ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       pd6UNIT_BALL *Uni;
 *   - ��������� �� �������� ��������:
 *       pd6ANIM *Ani;
 * ������������ ��������: ���.
 */
static VOID PD6_RectUnitResponse( pd6UNIT_RECT *Uni, pd6ANIM *Ani )
{
} /* End of 'PD6_AnimUnitResponse' function */

/* ������� ���������� ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       pd6UNIT_BALL *Uni;
 *   - ��������� �� �������� ��������:
 *       pd6ANIM *Ani;
 * ������������ ��������: ���.
 */
static VOID PD6_RectUnitRender( pd6UNIT_RECT *Uni, pd6ANIM *Ani )
{
  DBL shift = fabs(sin(Uni->PhaseShift + (DBL)clock() / CLOCKS_PER_SEC * 3)) * 30;

  SetDCBrushColor(Ani->hDC, Uni->Color);
  Rectangle(Ani->hDC, Uni->Pos.X - 5 - shift, Uni->Pos.Y - 5, Uni->Pos.X + 5, Uni->Pos.Y + 5 - shift);
} /* End of 'PD6_AnimUnitRender' function */

/* ������� �������� ������� �������� "�������".
 * ���������: ���.
 * ������������ ��������:
 *   (pd6UNIT *) ��������� �� ��������� ������ ��������.
 */
pd6UNIT * PD6_UnitRectCreate( VOID )
{
  pd6UNIT_RECT *Uni;

  if ((Uni = (VOID *)PD6_AnimUnitCreate(sizeof(pd6UNIT_RECT))) == NULL)
    return NULL;
  /* ��������� ���� */
  Uni->Init = (VOID *)PD6_RectUnitInit;
  Uni->Close = (VOID *)PD6_RectUnitClose;
  Uni->Response = (VOID *)PD6_RectUnitResponse;
  Uni->Render = (VOID *)PD6_RectUnitRender;
  return (pd6UNIT *)Uni;
} /* End of 'PD6_UnitRectCreate' function */

/* END OF 'URECT.C' FILE */