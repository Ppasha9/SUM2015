/* FILENAME: UBALL.C
 * PROGRAMMER: PD6
 * PURPOSE: Bounce ball unit handle module.
 * LAST UPDATE: 08.06.2015
 */

#include <string.h>
#include <time.h>

#include "anim.h"
#include "vec.h"
#include "camera.h"

/* Тип представления мяча */
typedef struct tagpd6UNIT_CAM
{
  PD6_UNIT_BASE_FIELDS;

  pd6CAMERA Cam;
} pd6UNIT_CAM;

/* Функция инициализации объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       pd6UNIT_BALL *Uni;
 *   - указатель на контекст анимации:
 *       pd6ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID PD6_CamUnitInit( pd6UNIT_CAM *Uni, pd6ANIM *Ani )
{
  PD6_RndCameraSet(&Uni->Cam, VecSet(8, 8, 8), VecSet(0, 0, 0), VecSet(0, 1, 0));
} /* End of 'PD6_AnimUnitInit' function */

/* Функция деинициализации объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       pd6UNIT_BALL *Uni;
 *   - указатель на контекст анимации:
 *       pd6ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID PD6_CamUnitClose( pd6UNIT_CAM *Uni, pd6ANIM *Ani )
{
} /* End of 'PD6_AnimUnitClose' function */

/* Функция обновления межкадровых параметров объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       pd6UNIT_BALL *Uni;
 *   - указатель на контекст анимации:
 *       pd6ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID PD6_CamUnitResponse( pd6UNIT_CAM *Uni, pd6ANIM *Ani )
{
  if (Ani->JButs[4])
    PD6_RndCameraRotateRight(&Uni->Cam, 10);
  if (Ani->JButs[5])
    PD6_RndCameraMoveRight(&Uni->Cam, 10);
  if (Ani->JButs[6])
    PD6_RndCameraMoveUp(&Uni->Cam, -10);
  if (Ani->JButs[7])
    PD6_RndCameraMoveUp(&Uni->Cam, 10);

} /* End of 'PD6_AnimUnitResponse' function */

/* Функция построения объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       pd6UNIT_BALL *Uni;
 *   - указатель на контекст анимации:
 *       pd6ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID PD6_CamUnitRender( pd6UNIT_CAM *Uni, pd6ANIM *Ani )
{
} /* End of 'PD6_AnimUnitRender' function */

/* Функция создания объекта анимации "камера".
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (pd6UNIT *) указатель на созданный объект анимации.
 */
pd6UNIT * PD6_UnitCameraCreate( VOID )
{
  pd6UNIT_CAM *Uni;

  if ((Uni = (VOID *)PD6_AnimUnitCreate(sizeof(pd6UNIT_CAM))) == NULL)
    return NULL;
  /* заполняем поля */
  Uni->Init = (VOID *)PD6_CamUnitInit;
  Uni->Close = (VOID *)PD6_CamUnitClose;
  Uni->Response = (VOID *)PD6_CamUnitResponse;
  Uni->Render = (VOID *)PD6_CamUnitRender;
  return (pd6UNIT *)Uni;
} /* End of 'PD6_UnitBallCreate' function */

/* END OF 'UBALL.C' FILE */
