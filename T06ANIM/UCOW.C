/* FILENAME: UBALL.C
 * PROGRAMMER: PD6
 * PURPOSE: Cow object unit handle module.
 * LAST UPDATE: 08.06.2015
 */

#include <string.h>

#include "anim.h"
#include "vec.h"
#include "render.h"

/* Тип представления мяча */
typedef struct tagpd6UNIT_COW
{
  PD6_UNIT_BASE_FIELDS;

  pd6GOBJ Model;
} pd6UNIT_СOW;

/* Функция инициализации объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       pd6UNIT_COW *Uni;
 *   - указатель на контекст анимации:
 *       pd6ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID PD6_CowUnitInit( pd6UNIT_СOW *Uni, pd6ANIM *Ani )
{
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
static VOID PD6_CowUnitClose( pd6UNIT_СOW *Uni, pd6ANIM *Ani )
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
static VOID PD6_CowUnitResponse( pd6UNIT_СOW *Uni, pd6ANIM *Ani )
{
/*  srand(30);

  if (Ani->Keys[VK_UP])
    Uni->Pos.Y -= 10;
  if (Ani->Keys[VK_DOWN])
    Uni->Pos.Y += 10;
  if (Ani->Keys[VK_LEFT])
    Uni->Pos.X -= 10;
  if (Ani->Keys[VK_RIGHT])
    Uni->Pos.X += 10;
  if (Ani->KeysClick['R'])
    Uni->Color = RGB(rand() % 255, rand() % 255, rand() % 255);

  /* Controling by JoyStick */
/*  if (!Ani->IsPause)
  {
    if (Ani->JButs[0])
      Uni->Pos.X -= 10;
    if (Ani->JButs[2])
      Uni->Pos.X += 10;
    if (Ani->JButs[1])
      Uni->Pos.Y += 10;
    if (Ani->JButs[3])
      Uni->Pos.Y -= 10;
  }*/

/*  if (!Ani->IsPause)
  {
    if (Ani->JY >= 0)
      Uni->Pos.Y += 10;
    if (Ani->JY < -0.00783)
      Uni->Pos.Y -= 10;
    if (Ani->JX >= 0)
      Uni->Pos.X += 10;
    if (Ani->JX < -0.00783)
      Uni->Pos.X -= 10;
  }        */

} /* End of 'PD6_AnimUnitResponse' function */

/* Функция построения объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       pd6UNIT_BALL *Uni;
 *   - указатель на контекст анимации:
 *       pd6ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID PD6_CowUnitRender( pd6UNIT_СOW *Uni, pd6ANIM *Ani )
{
  PD6_RndMatrWorld = MatrMulMatr(MatrScale(0.1, 0.1, 0.1),
    MatrRotateX(30 * Ani->Time));

  SetDCPenColor(Ani->hDC, RGB(255, 255, 255));
  PD6_RndGObjDraw(&Uni->Model);
} /* End of 'PD6_AnimUnitRender' function */

/* Функция создания объекта анимации "корова".
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (pd6UNIT *) указатель на созданный объект анимации.
 */
pd6UNIT * PD6_UnitCowCreate( VOID )
{
  pd6UNIT_СOW *Uni;

  if ((Uni = (VOID *)PD6_AnimUnitCreate(sizeof(pd6UNIT_СOW))) == NULL)
    return NULL;
  /* заполняем поля */
  Uni->Init = (VOID *)PD6_CowUnitInit;
  Uni->Close = (VOID *)PD6_CowUnitClose;
  Uni->Response = (VOID *)PD6_CowUnitResponse;
  Uni->Render = (VOID *)PD6_CowUnitRender;
  return (pd6UNIT *)Uni;
} /* End of 'PD6_UnitCowCreate' function */

/* END OF 'UCOW.C' FILE */
