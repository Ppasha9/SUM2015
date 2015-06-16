/* FILENAME: UCAM.C
 * PROGRAMMER: PD6
 * PURPOSE: Animation unit handle module.
 * LAST UPDATE: 09.06.2014
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "anim.h"
#include "render.h"

/* Unit flying camera struct definition */
typedef struct tagpd6UNIT_CAMFLY
{
  PD6_UNIT_BASE_FIELDS;           /* Base fields */
  VEC CamPos;                  /* Points of cube */
} pd6UNIT_CAMFLY;

/* Unit flying camera getting coords function.
 * ARGUMENTS:
 *   - Self pointer:
 *       pd6UNIT *Unit;
 *   - Animation context pointer:
 *       pd6ANIM *Ani;
 *   - Last point position:
 *       VEC SrcPoint;
 *   - Time:
 *       DBL Time;
 * RETURNS: 
 *   (VEC) Coords of point.
 */
static VEC PD6_UnitCamFlyGetCoord(pd6UNIT_CAMFLY *Unit, pd6ANIM *Ani, VEC SrcPoint, DBL Time )
{
  VEC VecSaved = SrcPoint;
  static INT MinDist = 30, MaxDist = 300;

  SrcPoint.X += cos(Time * 3) * ((INT)Time % 20);
  if (SrcPoint.X < -MaxDist)
    SrcPoint.X = -MaxDist;
  if (SrcPoint.X > MaxDist)
    SrcPoint.X = MaxDist;

  SrcPoint.Y += sin(Time) * ((INT)Time % 10);
  if (SrcPoint.Y < -MaxDist)
    SrcPoint.Y = -MaxDist;
  if (SrcPoint.Y > MaxDist)
    SrcPoint.Y = MaxDist;
  
  SrcPoint.Z += sin(Time * 2) * ((INT)Time % 30);
  if (SrcPoint.Z< -MaxDist)
    SrcPoint.Z = -MaxDist;
  if (SrcPoint.Z > MaxDist)
    SrcPoint.Z = MaxDist;

  if (SrcPoint.X > -MinDist && SrcPoint.X < 0 &&
      SrcPoint.Y > -MinDist && SrcPoint.Y < 0 &&
      SrcPoint.Z > -MinDist && SrcPoint.Z < 0)
    return VecSaved;

  return SrcPoint;
} /* End of 'PD6_UnitCamFlyGetCoord' function */

/* Unit flying camera response function.
 * ARGUMENTS:
 *   - Self pointer:
 *       pd6UNIT *Unit;
 *   - Animation context pointer:
 *       pd6ANIM *Ani;
 * RETURNS: None.
 */
static VOID UnitCamFlyResponse( pd6UNIT_CAMFLY *Unit, pd6ANIM *Ani )
{
  if (!Ani->IsPause)
    Unit->CamPos = PD6_UnitCamFlyGetCoord(Unit, Ani, Unit->CamPos, sin(Ani->Time));
} /* End of 'UnitCamFlyResponse' function */

/* Unit flying camera render function.
 * ARGUMENTS:
 *   - Self pointer:
 *       pd6UNIT *Unit;
 *   - Animation context pointer:
 *       pd6ANIM *Ani;
 * RETURNS: None.
 */
static VOID UnitCamFlyRender( pd6UNIT_CAMFLY *Unit, pd6ANIM *Ani )
{
/*  if (Ani->IsPause)
    return;
  PD6_RndCameraSet(&Ani->RndCamera, Unit->CamPos, VecSet(0, 0, 0), VecSet(0, 1, 0));
  /* UnitCamFlyGetCoordByTime(Unit, Ani, Unit->CamPos, Ani->Time + 0.2); */

  /// Ani->RndMatrWorld = MatrIdentity();
} /* End of 'UnitCamFlyRender' function */

/* Unit flying camera create function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (pd6UNIT_CAMFLY *) - pointer for new animation unit.
 */
pd6UNIT *PD6_UnitCamFlyCreate( VOID )
{
  pd6UNIT_CAMFLY *Unit;

  if ((Unit = (pd6UNIT_CAMFLY *)PD6_AnimUnitCreate(sizeof(pd6UNIT_CAMFLY))) == NULL)
    return NULL;

  /* Base fields filling */
  Unit->Render = (VOID *)UnitCamFlyRender;
  Unit->Response = (VOID *)UnitCamFlyResponse;

  Unit->CamPos.X = Unit->CamPos.Y = Unit->CamPos.Z = 70;
  return (pd6UNIT *)Unit;
} /* End of 'PD6_UnitCamFlyCreate' function */

/* END OF 'UCAM.C' FILE */