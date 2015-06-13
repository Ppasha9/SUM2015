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
  if (Ani->KeysClick['W'] || Ani->JButs[0])
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  if (Ani->KeysClick['Q'] || Ani->JButs[2])
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  glEnable(GL_DEPTH_TEST);

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
  INT i, j;

  PD6_RndMatrView = MatrView(VecSet(8, 8, 8),
                             VecSet(0, 0, 0),
                             VecSet(0, 1, 0));

  for (i = 0; i < 6; i++)
    for (j = 0; j < 6; j++)
    {
      PD6_RndMatrWorld =
        MatrMulMatr(MatrMulMatr(MatrMulMatr(
          MatrTranslate(Ani->JX * 59, Ani->JY * 88, 0),
          MatrScale(0.3, 0.3, 0.3)),
          MatrRotateY(30 * Ani->Time + Ani->JR * 180)),
          MatrTranslate(j * 1.30, 0, i * 1.30 + 100 * Ani->JZ));
      glColor3d(i & 1, j & 1, 1 - ((i & 1) + (j & 1)) / 2);
      PD6_RndGObjDraw(&Uni->Model);
    }
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
