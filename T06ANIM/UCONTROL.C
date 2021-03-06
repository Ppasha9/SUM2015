/* FILENAME: UCONTRL.C
 * PROGRAMMER: PD6
 * PURPOSE: Control unit handle module.
 * LAST UPDATE: 08.06.2015
 */

#include <stdio.h>
#include <string.h>
#include <time.h>

#include "anim.h"
#include "vec.h"

/* ��� ������������� ���� */
typedef struct tagpd6UNIT_CTRL
{
  PD6_UNIT_BASE_FIELDS;
  HFONT hFnt; /* ����� ��� ������ FPS */
} pd6UNIT_CTRL;

/* ������� ������������� ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       pd6UNIT_CTRL *Uni;
 *   - ��������� �� �������� ��������:
 *       pd6ANIM *Ani;
 * ������������ ��������: ���.
 */
static VOID PD6_AnimUnitInit( pd6UNIT_CTRL *Uni, pd6ANIM *Ani )
{
  Uni->hFnt = CreateFont(30, 0, 0, 0, FW_BOLD, FALSE, FALSE,
    FALSE, RUSSIAN_CHARSET, OUT_DEFAULT_PRECIS,
    CLIP_DEFAULT_PRECIS, PROOF_QUALITY,
    VARIABLE_PITCH, "Bookman Old Style");
} /* End of 'PD6_AnimUnitInit' function */

/* ������� ��������������� ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       pd6UNIT_CTRL *Uni;
 *   - ��������� �� �������� ��������:
 *       pd6ANIM *Ani;
 * ������������ ��������: ���.
 */
static VOID PD6_AnimUnitClose( pd6UNIT_CTRL *Uni, pd6ANIM *Ani )
{
  DeleteObject(Uni->hFnt);
} /* End of 'PD6_AnimUnitClose' function */

/* ������� ���������� ����������� ���������� ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       pd6UNIT_CTRL *Uni;
 *   - ��������� �� �������� ��������:
 *       pd6ANIM *Ani;
 * ������������ ��������: ���.
 */
static VOID PD6_AnimUnitResponse( pd6UNIT_CTRL *Uni, pd6ANIM *Ani )
{
  if (Ani->KeysClick[VK_ESCAPE])
    PD6_AnimDoExit();
  if (Ani->KeysClick['F'])
    PD6_AnimFlipFullScreen();
  if (Ani->KeysClick['P'])
    PD6_AnimSetPause(!Ani->IsPause);

  /* Controling by JoyStick */
  if (Ani->JButs[9])
    PD6_AnimFlipFullScreen();
  if (Ani->JButs[8])
    PD6_AnimSetPause(!Ani->IsPause);
/*  if (Ani->JButsClick[6])
    PD6_AnimDoExit();*/
} /* End of 'PD6_AnimUnitResponse' function */

/* ������� ���������� ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       pd6UNIT_CTRL *Uni;
 *   - ��������� �� �������� ��������:
 *       pd6ANIM *Ani;
 * ������������ ��������: ���.
 */
static VOID PD6_AnimUnitRender( pd6UNIT_CTRL *Uni, pd6ANIM *Ani )
{
  HFONT hFntOld = SelectObject(Ani->hDC, Uni->hFnt);
  RECT rc;
  static CHAR Buf[1000];

  SetTextColor(Ani->hDC, RGB(255, 255, 255));
  SetBkMode(Ani->hDC, TRANSPARENT);

  rc.left = 8;
  rc.top = 5;
  rc.right = Ani->W;
  rc.bottom = Ani->H;
  DrawText(Ani->hDC, Buf,
    sprintf(Buf,
      "JoyStick parametrs:\n"
      "Buttons: %i %i %i %i\n"
      "POV:%i\n"
      "%.5f %.5f %.5f %.5f",
      Ani->JButs[0], Ani->JButs[1], Ani->JButs[2], Ani->JButs[3], Ani->JPOV,
      Ani->JX, Ani->JY, Ani->JZ, Ani->JR),
    &rc,
    DT_TOP | DT_LEFT);

  SelectObject(Ani->hDC, hFntOld);
} /* End of 'VG4_AnimUnitRender' function */

/* ������� �������� ������� �������� "����������".
 * ���������: ���.
 * ������������ ��������:
 *   (pd6UNIT *) ��������� �� ��������� ������ ��������.
 */
pd6UNIT * PD6_UnitControlCreate( VOID )
{
  pd6UNIT_CTRL *Uni;

  if ((Uni = (VOID *)PD6_AnimUnitCreate(sizeof(pd6UNIT_CTRL))) == NULL)
    return NULL;
  /* ��������� ���� */
  Uni->Init = (VOID *)PD6_AnimUnitInit;
  Uni->Close = (VOID *)PD6_AnimUnitClose;
  Uni->Response = (VOID *)PD6_AnimUnitResponse;
  Uni->Render = (VOID *)PD6_AnimUnitRender;
  return (pd6UNIT *)Uni;
} /* End of 'PD6_UnitBallCreate' function */

/* END OF 'UCONTRL.C' FILE */
