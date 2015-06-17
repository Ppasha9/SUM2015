/* FILENAME: UCONTRL.C
 * PROGRAMMER: PD6
 * PURPOSE: Control unit handle module.
 * LAST UPDATE: 08.06.2015
 */

#include <stdio.h>
#include <string.h>
#include <time.h>

#include "anim.h"
#include "render.h"
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
  if (Ani->KeysClick[VK_ESCAPE] || Ani->JButsClick[6])
    PD6_AnimDoExit();
  if (Ani->KeysClick['F'])
    PD6_AnimFlipFullScreen();
  if (Ani->KeysClick['P'])
    PD6_AnimSetPause(!Ani->IsPause);

  /* Camera controling */
  if (Ani->Keys[VK_NUMPAD8])
    PD6_RndCameraMoveDir(&Ani->RndCamera, 1);
  if (Ani->Keys[VK_NUMPAD2])
    PD6_RndCameraMoveDir(&Ani->RndCamera, -1);

  if (Ani->Keys[VK_LEFT])
    PD6_RndCameraMoveRight(&Ani->RndCamera, -1);
  if (Ani->Keys[VK_RIGHT])
    PD6_RndCameraMoveRight(&Ani->RndCamera, 1);

  if (Ani->Keys[VK_UP])
    PD6_RndCameraMoveUp(&Ani->RndCamera, 1);
  if (Ani->Keys[VK_DOWN])
   PD6_RndCameraMoveUp(&Ani->RndCamera, -1);

  if (Ani->Keys[VK_NUMPAD4])
    PD6_RndCameraRotateUp(&Ani->RndCamera, 1);
  if (Ani->Keys[VK_NUMPAD6])
   PD6_RndCameraRotateUp(&Ani->RndCamera, -1);

  if (Ani->Keys[VK_NUMPAD9])
    PD6_RndCameraRotateRight(&Ani->RndCamera, 1);
  if (Ani->Keys[VK_NUMPAD3])
    PD6_RndCameraRotateRight(&Ani->RndCamera, -1);

  if (Ani->JButs[0] || Ani->Keys['W'])
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  if (Ani->JButs[2] || Ani->Keys['Q'])
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  glEnable(GL_DEPTH_TEST);
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
  static DBL count = 30;
  static CHAR Buf[1000];

  count += Ani->GlobalDeltaTime;
  if (count > 1)
  {
    count = 0;
    sprintf(Buf, "FPS: %.3f", Ani->FPS);
    SetWindowText(Ani->hWnd, Buf);
  }

  /*
  SetTextColor(Ani->hDC, RGB(255, 255, 255));
  SetBkMode(Ani->hDC, TRANSPARENT);
  rc.left = 0;
  rc.top = 0;
  rc.right = Ani->W;
  rc.bottom = Ani->H;
  DrawText(Ani->hDC, Buf,
    sprintf(Buf,
      "FPS: %.3f\n"
      "J: %i %i %i, POV:%i\n"
      "%.5f %.5f %.5f %.5f %.5f %.5f",
      Ani->FPS,
      Ani->JButs[0], Ani->JButs[1], Ani->JButs[2], Ani->JPOV,
      Ani->JX, Ani->JY, Ani->JZ, Ani->JR, Ani->JU, Ani->JV),
    &rc,
    DT_TOP | DT_LEFT);
  SelectObject(Ani->hDC, hFntOld);
  */
} /* End of 'PD6_AnimUnitRender' function */

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