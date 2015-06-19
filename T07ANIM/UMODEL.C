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

  pd6GEOM Model;
  pd6GEOM Geom;
  pd6PRIM Pr;
  INT TextId;
} pd6UNIT_MODEL;

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
  pd6VERTEX V[]= 
  {
    {{0, 0, 0}, {0, 0}, {0, 0, 1}, {1, 1, 1, 1}},
    {{1, 0, 0}, {5, 0}, {0, 0, 1}, {1, 0, 1, 1}},
    {{0, 1, 0}, {0, 5}, {0, 0, 1}, {1, 1, 0, 1}},
    {{1, 1, 0}, {5, 5}, {0, 0, 1}, {1, 1, 0, 1}},
  };
  INT I[] = {0, 1, 2, 2, 1, 3};
  BYTE txt[2][2][3] =
  {
    {{255, 255, 255}, {0, 0, 0}},
    {{0, 0, 0}, {255, 255, 255}}
  };

  //Uni->TextId = PD6_TextureLoad("ElphSkin.bmp");

  //PD6_PrimCreate(&Uni->Pr, PD6_PRIM_TRIMESH, 4, 6, V, I);

  //PD6_PrimCreateHeightField(&Uni->Pr, VecSet(-5, 0, 5), VecSet(-10, 0, 0), VecSet(0, 0, 10), 1.50, "E:\\SPR09\\HF.BMP");

  //PD6_RndPrimMatrConvert = MatrMulMatr(MatrScale(5, 5, 5), MatrRotateX(-90));
  //PD6_GeomLoad(&Uni->Model, "NISPF.g3d");

  PD6_RndPrimMatrConvert = MatrMulMatr(MatrScale(15, 15, 15), MatrRotateX(-90));
  PD6_GeomLoad(&Uni->Geom, "Nissan_Pathfinder\\NISPF.g3d");
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
  PD6_GeomFree(&Uni->Model);
  PD6_GeomFree(&Uni->Geom);
  PD6_PrimFree(&Uni->Pr);
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

  if (Ani->JButs[0])
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  if (Ani->JButs[2])
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  glEnable(GL_DEPTH_TEST);

  for (i = 0; i < 1; i++)
    for (j = 0; j < 1; j++)
    {
      PD6_RndMatrWorld =
        MatrMulMatr(MatrMulMatr(MatrMulMatr(
          MatrTranslate(Ani->JX * 59, Ani->JY * 88, 0),
          MatrScale(0.5, 0.5, 0.5)),
          MatrRotateY(30 * Ani->Time + Ani->JR * 180)),
          MatrTranslate(j * 1.30, -5, i * 1.30 + 100 * Ani->JZ));
      glColor3d(i & 1, j & 1, 1 - ((i & 1) + (j & 1)) / 2);
      PD6_GeomDraw(&Uni->Geom);
    }
  //PD6_RndMatrWorld = MatrRotateY(30 * Ani->Time);
  //MatrMulMatr(MatrRotateZ(30 * sin(Ani->Time * 3.0)), MatrRotateY(30 * Ani->Time));
  //PD6_GeomDraw(&Uni->Geom);

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