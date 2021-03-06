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

/* ��� ������������� ���� */
typedef struct tagpd6UNIT_CAM
{
  PD6_UNIT_BASE_FIELDS;

  pd6CAMERA Cam;
} pd6UNIT_CAM;

/* ������� ������������� ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       pd6UNIT_BALL *Uni;
 *   - ��������� �� �������� ��������:
 *       pd6ANIM *Ani;
 * ������������ ��������: ���.
 */
static VOID PD6_CamUnitInit( pd6UNIT_CAM *Uni, pd6ANIM *Ani )
{
  PD6_RndCameraSet(&Uni->Cam, VecSet(8, 8, 8), VecSet(0, 0, 0), VecSet(0, 1, 0));
} /* End of 'PD6_AnimUnitInit' function */

/* ������� ��������������� ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       pd6UNIT_BALL *Uni;
 *   - ��������� �� �������� ��������:
 *       pd6ANIM *Ani;
 * ������������ ��������: ���.
 */
static VOID PD6_CamUnitClose( pd6UNIT_CAM *Uni, pd6ANIM *Ani )
{
} /* End of 'PD6_AnimUnitClose' function */

/* ������� ���������� ����������� ���������� ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       pd6UNIT_BALL *Uni;
 *   - ��������� �� �������� ��������:
 *       pd6ANIM *Ani;
 * ������������ ��������: ���.
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

/* ������� ���������� ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       pd6UNIT_BALL *Uni;
 *   - ��������� �� �������� ��������:
 *       pd6ANIM *Ani;
 * ������������ ��������: ���.
 */
static VOID PD6_CamUnitRender( pd6UNIT_CAM *Uni, pd6ANIM *Ani )
{
} /* End of 'PD6_AnimUnitRender' function */

/* ������� �������� ������� �������� "������".
 * ���������: ���.
 * ������������ ��������:
 *   (pd6UNIT *) ��������� �� ��������� ������ ��������.
 */
pd6UNIT * PD6_UnitCameraCreate( VOID )
{
  pd6UNIT_CAM *Uni;

  if ((Uni = (VOID *)PD6_AnimUnitCreate(sizeof(pd6UNIT_CAM))) == NULL)
    return NULL;
  /* ��������� ���� */
  Uni->Init = (VOID *)PD6_CamUnitInit;
  Uni->Close = (VOID *)PD6_CamUnitClose;
  Uni->Response = (VOID *)PD6_CamUnitResponse;
  Uni->Render = (VOID *)PD6_CamUnitRender;
  return (pd6UNIT *)Uni;
} /* End of 'PD6_UnitBallCreate' function */

/* END OF 'UBALL.C' FILE */
