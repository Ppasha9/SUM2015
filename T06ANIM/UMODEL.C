/* FILENAME: UMODEL.C
 * PROGRAMMER: PD6
 * PURPOSE: Model object unit handle module.
 * LAST UPDATE: 10.06.2015
 */

#include <string.h>

#include "anim.h"
#include "vec.h"
#include "render.h"

/* Тип представления мяча */
typedef struct tagpd6UNIT_MODEL
{
  PD6_UNIT_BASE_FIELDS;

  pd6GOBJ Model;  /* 3D objects */
} pd6UNIT_MODEL;

/* Функция инициализации объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       pd6UNIT_MODEL *Uni;
 *   - указатель на контекст анимации:
 *       pd6ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID PD6_ModelUnitInit( pd6UNIT_MODEL *Uni, pd6ANIM *Ani )
{
  Ani->PosModel = VecSet(0, 0, 0);
  Ani->AngleX = Ani->AngleY = Ani->AngleZ = 0.0;
  PD6_RndGObjLoad(&Uni->Model, "cow.object");
} /* End of 'PD6_AnimUnitInit' function */

/* Функция деинициализации объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       pd6UNIT_BALL *Uni;
 *   - указатель на контекст анимации:
 *       pd6ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID PD6_ModelUnitClose( pd6UNIT_MODEL *Uni, pd6ANIM *Ani )
{
  PD6_RndGObjFree(&Uni->Model);
} /* End of 'PD6_AnimUnitClose' function */

/* Функция обновления межкадровых параметров объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       pd6UNIT_BALL *Uni;
 *   - указатель на контекст анимации:
 *       pd6ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID PD6_ModelUnitResponse( pd6UNIT_MODEL *Uni, pd6ANIM *Ani )
{
  if (Ani->JButs[4])
    Ani->PosModel.X -= 10;
  if (Ani->JButs[5])
    Ani->PosModel.X += 10;
  if (Ani->JButs[6])
    Ani->PosModel.Y += 10;
  if (Ani->JButs[7])
    Ani->PosModel.Y -= 10;
} /* End of 'PD6_AnimUnitResponse' function */

/* Функция построения объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       pd6UNIT_BALL *Uni;
 *   - указатель на контекст анимации:
 *       pd6ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID PD6_ModelUnitRender( pd6UNIT_MODEL *Uni, pd6ANIM *Ani )
{
  static DBL MouseShift = 0;

  MouseShift += 30 * PD6_MouseWheel;
  PD6_RndMatrWorld = MatrMulMatr(MatrMulMatr(MatrMulMatr(MatrMulMatr(
    MatrRotateY(50 * Ani->Time), MatrScale(0.060, 0.060, 0.060)),
    MatrTranslate(Ani->JX, -Ani->JY, 0)), MatrRotateX(50 * sin(Ani->Time * 3))), MatrScale(0.10 + sin(Ani->Time * 3), 0.10 + sin(Ani->Time * 3), 0.10 + sin(Ani->Time * 3)));
  PD6_RndMatrView = MatrView(VecSet(8, 8, 8), VecSet(0, 0, 0), VecSet(0, 1, 0));


  SetDCPenColor(Ani->hDC, RGB(rand() % 255, rand() % 255, rand() % 255));
  PD6_RndGObjDraw(&Uni->Model);
} /* End of 'PD6_AnimUnitRender' function */

/* Функция создания объекта анимации "корова".
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (pd6UNIT *) указатель на созданный объект анимации.
 */
pd6UNIT * PD6_UnitModelCreate( VOID )
{
  pd6UNIT_MODEL *Uni;

  if ((Uni = (VOID *)PD6_AnimUnitCreate(sizeof(pd6UNIT_MODEL))) == NULL)
    return NULL;
  /* заполняем поля */
  Uni->Init = (VOID *)PD6_ModelUnitInit;
  Uni->Close = (VOID *)PD6_ModelUnitClose;
  Uni->Response = (VOID *)PD6_ModelUnitResponse;
  Uni->Render = (VOID *)PD6_ModelUnitRender;
  return (pd6UNIT *)Uni;
} /* End of 'PD6_UnitCowCreate' function */

/* END OF 'UCOW.C' FILE */
