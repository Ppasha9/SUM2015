/* FILENAME: UMODEL.C
 * PROGRAMMER: PD6
 * PURPOSE: Model view unit handle module.
 * LAST UPDATE: 10.06.2015
 */

#include "anim.h"
#include "render.h"

typedef struct tagpd6UNIT_WATER
{
  PD6_UNIT_BASE_FIELDS;

  pd6PRIM Pr;
  INT TextId;
} pd6UNIT_WATER;

/* Функция инициализации объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       pd6UNIT_WATER *Uni;
 *   - указатель на контекст анимации:
 *       pd6ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID PD6_AnimUnitInit( pd6UNIT_WATER *Uni, pd6ANIM *Ani )
{
  pd6MATERIAL Mtl = PD6_MtlLib[1];

  PD6_RndPrimMatrConvert = MatrIdentity();
  PD6_PrimCreatePlane(&Uni->Pr, VecSet(-220, 14, 260), VecSet(-370, 0, 0), VecSet(0, 0, 300), 100, 100);
  Mtl.TexId = PD6_TextureLoad("Water.bmp");
  Uni->Pr.MtlNo = Uni->TextId = PD6_MtlAdd(&Mtl);
} /* End of 'PD6_AnimUnitInit' function */

/* Функция деинициализации объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       pd6UNIT_WATER *Uni;
 *   - указатель на контекст анимации:
 *       pd6ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID PD6_AnimUnitClose( pd6UNIT_WATER *Uni, pd6ANIM *Ani )
{
  PD6_PrimFree(&Uni->Pr);
} /* End of 'PD6_AnimUnitClose' function */

/* Функция построения объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       pd6UNIT_WATER *Uni;
 *   - указатель на контекст анимации:
 *       pd6ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID PD6_AnimUnitRender( pd6UNIT_WATER *Uni, pd6ANIM *Ani )
{
  PD6_RndMatrView = MatrView(Ani->RndCamera.Loc,
                             Ani->RndCamera.At,
                             Ani->RndCamera.Up);

  PD6_RndMatrWorld = MatrIdentity();
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, Uni->TextId);
  PD6_PrimWaterDraw(&Uni->Pr);
} /* End of 'PD6_AnimUnitRender' function */

/* Функция создания объекта анимации "вода".
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (pd6UNIT *) указатель на созданный объект анимации.
 */
pd6UNIT * PD6_UnitWaterCreate( VOID )
{
  pd6UNIT_WATER *Uni;

  if ((Uni = (VOID *)PD6_AnimUnitCreate(sizeof(pd6UNIT_WATER))) == NULL)
    return NULL;
  Uni->Init = (VOID *)PD6_AnimUnitInit;
  Uni->Close = (VOID *)PD6_AnimUnitClose;
  Uni->Render = (VOID *)PD6_AnimUnitRender;
  return (pd6UNIT *)Uni;
} /* End of 'PD6_UnitModelCreate' function */

/* END OF 'UMODEL.C' FILE */