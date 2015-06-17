/* FILENAME: UMODEL.C
 * PROGRAMMER: PD6
 * PURPOSE: Model view unit handle module.
 * LAST UPDATE: 10.06.2015
 */

#include "anim.h"
#include "render.h"

typedef struct tagpd6UNIT_MODEL
{
  PD6_UNIT_BASE_FIELDS;

  pd6GEOM Geom;
  pd6GEOM GeomTree[3];
  pd6PRIM Pr;
  INT TextId;
} pd6UNIT_MODEL;

MATR PD6_HelicMatr = PD6_UNIT_MATR;
VEC
  YH,
  XH,
  ZH;

/* Функция инициализации объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       pd6UNIT_MODEL *Uni;
 *   - указатель на контекст анимации:
 *       pd6ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID PD6_AnimUnitInit( pd6UNIT_MODEL *Uni, pd6ANIM *Ani )
{
  pd6MATERIAL Mtl = PD6_MtlLib[0];
  INT i;


  YH = VecSet(0, 0, 0);
  XH = VecSet(0, 0, 0);
  ZH = VecSet(0, 0, 0);

  PD6_RndPrimMatrConvert = MatrIdentity();
  PD6_PrimCreateHeightField(&Uni->Pr, VecSet(0, 0, 0), VecSet(-1200, 0, 0), VecSet(0, 0, 1200), 180.0, "E:\\SPR09\\Test2.BMP");
  Mtl.TexId = PD6_TextureLoad("HF1TEX.BMP");
  Uni->Pr.MtlNo = Uni->TextId = PD6_MtlAdd(&Mtl);

  //PD6_PrimCreate(&Uni->Pr, PD6_PRIM_TRIMESH, 4, 6, V, I);

  //PD6_RndPrimMatrConvert = MatrMulMatr(MatrScale(5, 5, 5), MatrRotateX(-90));
  //PD6_GeomLoad(&Uni->Model, "NISPF.g3d");

  PD6_HelicMatr = MatrTranslate(20, 20, 20);
  PD6_RndPrimMatrConvert = MatrMulMatr(MatrMulMatr(MatrScale(2, 2, 2), MatrRotateX(-90)), PD6_HelicMatr);
  PD6_GeomLoad(&Uni->Geom, "E:\\SPR09\\UH60\\UH60\\uh60.g3d");

  for (i = 0; i < 3; i++)
    PD6_GeomLoad(&Uni->GeomTree[i], "E:\\SPR09\\Tree\\Tree\\Tree.g3d");
} /* End of 'PD6_AnimUnitInit' function */

/* Функция деинициализации объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       pd6UNIT_BALL *Uni;
 *   - указатель на контекст анимации:
 *       pd6ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID PD6_AnimUnitClose( pd6UNIT_MODEL *Uni, pd6ANIM *Ani )
{
  INT i;

  PD6_GeomFree(&Uni->Geom);
  PD6_PrimFree(&Uni->Pr);

  for (i = 0; i < 3; i++)
    PD6_GeomFree(&Uni->GeomTree[i]);
} /* End of 'PD6_AnimUnitClose' function */

/* Функция построения объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       pd6UNIT_BALL *Uni;
 *   - указатель на контекст анимации:
 *       pd6ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID PD6_AnimUnitRender( pd6UNIT_MODEL *Uni, pd6ANIM *Ani )
{
  INT i, j;

  PD6_RndMatrView = MatrView(Ani->RndCamera.Loc,
                             Ani->RndCamera.At,
                             Ani->RndCamera.Up);

  for (i = 0; i < 1; i++)
    for (j = 0; j < 1; j++)
    {
      PD6_RndMatrWorld =
        MatrMulMatr(MatrMulMatr(MatrMulMatr(
          MatrTranslate(0, 0, 0),
          MatrScale(0.5, 0.5, 0.5)),
          MatrRotateY(0)),
          MatrTranslate(0, 0, 0));
      glColor3d(i & 1, j & 1, 1 - ((i & 1) + (j & 1)) / 2);
      PD6_GeomHelicDraw(&Uni->Geom);
    }
  //PD6_RndMatrWorld = MatrRotateY(30 * Ani->Time);
  //MatrMulMatr(MatrRotateZ(30 * sin(Ani->Time * 3.0)), MatrRotateY(30 * Ani->Time));
  //PD6_GeomDraw(&Uni->Geom);

  PD6_RndMatrWorld = MatrMulMatr(MatrTranslate(-500, 30, 900), MatrScale(0.3, 0.3, 0.3));
  PD6_GeomDraw(&Uni->GeomTree[0]);

  PD6_RndMatrWorld = MatrIdentity();
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, Uni->TextId);
  PD6_PrimDraw(&Uni->Pr);
} /* End of 'PD6_AnimUnitRender' function */

/* Функция создания объекта анимации "модель".
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (pd6UNIT *) указатель на созданный объект анимации.
 */
pd6UNIT * PD6_UnitModelCreate( VOID )
{
  pd6UNIT_MODEL *Uni;

  if ((Uni = (VOID *)PD6_AnimUnitCreate(sizeof(pd6UNIT_MODEL))) == NULL)
    return NULL;
  Uni->Init = (VOID *)PD6_AnimUnitInit;
  Uni->Close = (VOID *)PD6_AnimUnitClose;
  Uni->Render = (VOID *)PD6_AnimUnitRender;
  return (pd6UNIT *)Uni;
} /* End of 'PD6_UnitModelCreate' function */

/* END OF 'UMODEL.C' FILE */