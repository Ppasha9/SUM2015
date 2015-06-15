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

/* Тип представления мяча */
typedef struct tagpd6UNIT_CTRL
{
  PD6_UNIT_BASE_FIELDS;
  HFONT hFnt; /* Шрифт для вывода FPS */
} pd6UNIT_CTRL;

/* Функция инициализации объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       pd6UNIT_CTRL *Uni;
 *   - указатель на контекст анимации:
 *       pd6ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID PD6_AnimUnitInit( pd6UNIT_CTRL *Uni, pd6ANIM *Ani )
{
  Uni->hFnt = CreateFont(30, 0, 0, 0, FW_BOLD, FALSE, FALSE,
    FALSE, RUSSIAN_CHARSET, OUT_DEFAULT_PRECIS,
    CLIP_DEFAULT_PRECIS, PROOF_QUALITY,
    VARIABLE_PITCH, "Bookman Old Style");

  Ani->Coef = 1.0;
} /* End of 'PD6_AnimUnitInit' function */

/* Функция деинициализации объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       pd6UNIT_CTRL *Uni;
 *   - указатель на контекст анимации:
 *       pd6ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID PD6_AnimUnitClose( pd6UNIT_CTRL *Uni, pd6ANIM *Ani )
{
  DeleteObject(Uni->hFnt);
} /* End of 'PD6_AnimUnitClose' function */

/* Функция обновления межкадровых параметров объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       pd6UNIT_CTRL *Uni;
 *   - указатель на контекст анимации:
 *       pd6ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID PD6_AnimUnitResponse( pd6UNIT_CTRL *Uni, pd6ANIM *Ani )
{
  if (Ani->KeysClick[VK_ESCAPE] || Ani->JButsClick[6])
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
  if (Ani->JButs[3])
    Ani->Coef += 0.05;
  if (Ani->JButs[1])
    Ani->Coef -= 0.05;

  /* Camera controling */
  if (Ani->IsPause)
  {
    if (Ani->Keys['O'])
      PD6_RndCameraMoveDir(&Ani->RndCamera, 0.5);
    if (Ani->Keys['L'])
      PD6_RndCameraMoveDir(&Ani->RndCamera, -0.5);

    if (Ani->JY)
      PD6_RndCameraMoveDir(&Ani->RndCamera, -Ani->JY);

    if (Ani->Keys['A'])
      PD6_RndCameraMoveRight(&Ani->RndCamera, -2);
    if (Ani->Keys['D'])
      PD6_RndCameraMoveRight(&Ani->RndCamera, 2);
  
    if (Ani->JX)
      PD6_RndCameraMoveRight(&Ani->RndCamera, Ani->JX);

    if (Ani->Keys['W'])
      PD6_RndCameraMoveUp(&Ani->RndCamera, 0.5);
    if (Ani->Keys['S'])
     PD6_RndCameraMoveUp(&Ani->RndCamera, -0.5);

    if (Ani->JR)
      PD6_RndCameraRotateUp(&Ani->RndCamera, -Ani->JR);

    if (Ani->Keys['Q'])
      PD6_RndCameraRotateUp(&Ani->RndCamera, 2);
    if (Ani->Keys['E'])
     PD6_RndCameraRotateUp(&Ani->RndCamera, -2);

    if (Ani->JZ)
     PD6_RndCameraRotateRight(&Ani->RndCamera, -Ani->JZ);
    PD6_RndCameraRotateUp(&Ani->RndCamera, (DBL)PD6_MouseWheel);

    if (Ani->Keys['I'])
      PD6_RndCameraRotateRight(&Ani->RndCamera, 2);
    if (Ani->Keys['K'])
      PD6_RndCameraRotateRight(&Ani->RndCamera, -2);
  }
} /* End of 'PD6_AnimUnitResponse' function */

/* Функция построения объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       pd6UNIT_CTRL *Uni;
 *   - указатель на контекст анимации:
 *       pd6ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
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

/* Функция создания объекта анимации "управление".
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (pd6UNIT *) указатель на созданный объект анимации.
 */
pd6UNIT * PD6_UnitControlCreate( VOID )
{
  pd6UNIT_CTRL *Uni;

  if ((Uni = (VOID *)PD6_AnimUnitCreate(sizeof(pd6UNIT_CTRL))) == NULL)
    return NULL;
  /* заполняем поля */
  Uni->Init = (VOID *)PD6_AnimUnitInit;
  Uni->Close = (VOID *)PD6_AnimUnitClose;
  Uni->Response = (VOID *)PD6_AnimUnitResponse;
  Uni->Render = (VOID *)PD6_AnimUnitRender;
  return (pd6UNIT *)Uni;
} /* End of 'PD6_UnitBallCreate' function */

/* END OF 'UCONTRL.C' FILE */
