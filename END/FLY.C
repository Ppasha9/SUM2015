/* FILENAME: FLY.C
 * PROGRAMMER: PD6
 * PURPOSE: Flying unit handle module.
 * LAST UPDATE: 08.06.2015
 */

#include <string.h>
#include <time.h>

#include "anim.h"
#include "render.h"
#include "camera.h"
#include "vec.h"

MATR MatrVL = PD6_UNIT_MATR;

typedef struct tagpd6UNIT_FLY
{
  PD6_UNIT_BASE_FIELDS;  /* Base fields */
  pd6GEOM Geom;          /* Geometry Object */
  VEC CamShift;          /* Camera position */
  pd6CAMERA
    ObjCam;              /* Object camera */
  VEC Speed;             /* Object physics */
  VEC RotSpeed;
  VEC Acc;
} pd6UNIT_FLY;

/* Unit fly init function.
 * ARGUMENTS:
 *   - Self pointer:
 *       pd6UNIT_FLY *Unit;
 *   - Animation context pointer:
 *       pd6ANIM *Ani;
 * RETURNS: None.
 */
static VOID UnitFlyInit( pd6UNIT_FLY *Unit, pd6ANIM *Ani )
{
  PD6_RndCameraSet(&Unit->ObjCam, Unit->ObjCam.Loc, VecAddVec(Unit->ObjCam.Loc, VecSet(1, 0, 0)), VecSet(0, 1, 0));

  PD6_RndCameraUpdateInfo(&Unit->ObjCam);
  Unit->CamShift = VecSet(0, 2.0, -18);

  PD6_GeomLoad(&Unit->Geom, "E:\\SPR09\\Mirage\\Mirage\\Mirage.g3d");
} /* End of 'PD6_UnitFlyInit' function */

/* Unit Fly destructor function.
* ARGUMENTS:
*   - Self pointer:
*       pd6UNIT_FLY *Unit;
*   - Animation context pointer:
*       pd6ANIM *Ani;
* RETURNS: None.
*/
static VOID UnitFlyClose( pd6UNIT_FLY *Unit, pd6ANIM *Ani )
{
  PD6_GeomFree(&Unit->Geom);
} /* End of 'PD6_UnitFlyClose' function */

/* Unit Fly mouse stting response function.
 * ARGUMENTS:
 *   - Self pointer:
 *       pd6UNIT *Unit;
 *   - Animation context pointer:
 *       pd6ANIM *Ani;
 * RETURNS: None.
 */
static VOID UnitFlyCameraSet( pd6UNIT_FLY *Unit, pd6ANIM *Ani )
{
  MATR MatrCam;

  if (!Ani->IsPause)
  {
    PD6_RndCameraUpdateInfo(&Unit->ObjCam);

    MatrVL = MatrTranspose(MatrView(VecSet(0, 0, 0), VecNeg(Unit->ObjCam.Dir), Unit->ObjCam.Up));

    MatrCam = MatrTranspose(MatrView(VecSet(0, 0, 0), VecNeg(Unit->ObjCam.Dir), VecSet(0, 1, 0)));
    PD6_RndCameraSet(&Ani->RndCamera, VecAddVec(Unit->ObjCam.Loc, VectorTransform(Unit->CamShift, MatrCam)),
      Unit->ObjCam.Loc, VecSet(0, 1, 0));

    PD6_RndCameraUpdateInfo(&Ani->RndCamera);
  }
} /* End of 'UnitFlyCameraSet' function */

/* Unit Fly response function.
 * ARGUMENTS:
 *   - Self pointer:
 *       pd6UNIT *Unit;
 *   - Animation context pointer:
 *       pd6ANIM *Ani;
 * RETURNS: None.
 */
static VOID UnitFlyResponse( pd6UNIT_FLY *Unit, pd6ANIM *Ani )
{
  Unit->Acc = VecSet(0, 0, 0);
  PD6_RndCameraUpdateInfo(&Unit->ObjCam);

  if (!Ani->IsPause)
  {
    if (Ani->Keys['I'])
      Unit->Speed = VecAddVec(Unit->Speed, VecSet(0, 0, 0.01));
    if (Ani->Keys['K'])
      Unit->Speed = VecAddVec(Unit->Speed, VecSet(0, 0, -0.01));
    if (Ani->JY)
      Unit->Speed = VecAddVec(Unit->Speed, VecSet(0, 0, -Ani->JY * 0.1));

    if (Ani->Keys['Q'])
      PD6_RndCameraRotateDir(&Unit->ObjCam, -5);
    if (Ani->Keys['E'])
      PD6_RndCameraRotateDir(&Unit->ObjCam, 5);

    if (Ani->Keys['A'])
      PD6_RndCameraRotateUp(&Unit->ObjCam, 1);
    if (Ani->Keys['D'])
      PD6_RndCameraRotateUp(&Unit->ObjCam, -1);

    if (Ani->JR)
    {
      PD6_RndCameraRotateUp(&Unit->ObjCam, -Ani->JR);
      Unit->RotSpeed.Z = Ani->JR * 10;
    }

    if (Ani->Keys['W'])
      PD6_RndCameraRotateRight(&Unit->ObjCam, -0.5);
    if (Ani->Keys['S'])
      PD6_RndCameraRotateRight(&Unit->ObjCam, 0.5);
    if (Ani->JZ)
      PD6_RndCameraRotateRight(&Unit->ObjCam, -Ani->JZ);

    UnitFlyCameraSet(Unit, Ani);

    Unit->Speed = VecAddVec(Unit->Speed, Unit->Acc);
    Unit->ObjCam.Loc = VecAddVec(Unit->ObjCam.Loc, VectorTransform(Unit->Speed, MatrVL));
    Unit->ObjCam.At = VecAddVec(Unit->ObjCam.Loc, Unit->ObjCam.Dir);
  }
} /* End of 'PD6_UnitFlyResponse' function */

/* Unit Fly render function.
 * ARGUMENTS:
 *   - Self pointer:
 *       pd6UNIT *Unit;
 *   - Animation context pointer:
 *       pd6ANIM *Ani;
 * RETURNS: None.
 */
static VOID UnitFlyRender( pd6UNIT_FLY *Unit, pd6ANIM *Ani )
{
  // UINT loc;

  PD6_RndMatrWorld = MatrMulMatr(MatrMulMatr(MatrMulMatr(PD6_RndMatrWorld, MatrVL),
                                   MatrScale(0.7, 0.7, 0.7)), MatrRotateX(90));
  PD6_RndMatrWorld = MatrMulMatr(PD6_RndMatrWorld,
                                   MatrTranslate(Unit->ObjCam.Loc.X, Unit->ObjCam.Loc.Y, Unit->ObjCam.Loc.Z));

  PD6_RndCameraUpdateInfo(&Ani->RndCamera);

  PD6_RndMatrView = MatrView(Ani->RndCamera.Loc, Ani->RndCamera.At, Ani->RndCamera.Up);
  PD6_RndMatrWorldViewProj = MatrMulMatr(MatrMulMatr(PD6_RndMatrWorld, PD6_RndMatrView), PD6_RndMatrProj);

  //PD6_GeomHelicDraw(&Unit->Geom);

  //glUseProgram(Ani->ShaderDef);
  PD6_RndMatrWorld = MatrIdentity();
} /* End of 'PD6_UnitFlyRender' function */

/* Unit Fly create function.
 * ARGUMENTS:
 *   - Postion on world map:
 *       INT PosXPosY;
 *   - Angles of rotation:
 *       DBL RotAngleX, RotAngleY, RotAngleZ;
 * RETURNS:
 *   (pd6UNIT_FLY *) - pointer for new animation unit.
 */
pd6UNIT *PD6_UnitFlyCreate( INT PosX, INT PosY, INT PosZ )
{
  pd6UNIT_FLY *Unit;

  if ((Unit = (pd6UNIT_FLY *)PD6_AnimUnitCreate(sizeof(pd6UNIT_FLY))) == NULL)
    return NULL;

  /* Base fields filling */
  Unit->Render = (VOID *)UnitFlyRender;
  Unit->Init = (VOID *)UnitFlyInit;
  Unit->Close = (VOID *)UnitFlyClose;
  Unit->Response = (VOID *)UnitFlyResponse;

  Unit->ObjCam.Loc = VecSet(PosX, PosY, PosZ);
  return (pd6UNIT *)Unit;
} /* End of 'PD6_UnitFlyCreate' function */

/* END OF 'SAMPFly.C' FILE */

