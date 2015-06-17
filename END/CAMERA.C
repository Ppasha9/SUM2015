/* FILENAME: CAMERA.C
 * PROGRAMMER: PD6
 * PURPOSE: 3D render handle module.
 * LAST UPDATE: 13.08.2014
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "render.h"
#include "camera.h"

/* Setting cameras properties 
 * ARGUMENTS:
 *   - Vector camera new properties:
 *     PD6VEC Loc, At, Up;
 * RETURNS: None.
 */
VOID PD6_RndCameraSet( pd6CAMERA *Cam, VEC Loc, VEC At, VEC Up )
{
  Cam->Loc = Loc;
  Cam->At = At;
  Cam->Up = Up;
} /* End of 'PD6_RndCameraSet' funciton */

/* Cameras getting matrix function.
* ARGUMENTS: None.
* RETURNS: 
*   (MATR) Camera matrix.
*/
MATR PD6_RndCameraGetMatrix( pd6CAMERA *Cam )
{
  MATR m = 
  {
    {
      { Cam->Dir.X  , Cam->Dir.Y  , Cam->Dir.Z  , 0 },
      { Cam->Up.X   , Cam->Up.Y   , Cam->Up.Z   , 0 },
      { Cam->Right.X, Cam->Right.Y, Cam->Right.Z, 0 },
      {                  0,                  0,                 0 , 1 }
    }
  };

  return m;
} /* End of 'PD6_RndCameraGetMatrix' funciton */

/* Cameras normalizing vectors function.
* ARGUMENTS: None.
* RETURNS: None.
*/
VOID PD6_RndCameraNormalize( pd6CAMERA *Cam )
{
  Cam->Dir = VecNormalize(Cam->Dir);
  Cam->Up = VecNormalize(Cam->Up);
  Cam->Right = VecNormalize(Cam->Right);
} /* End of 'PD6_RndCameraGetMatrix' funciton */

/* Camera move by Dir-axes properties 
 * ARGUMENTS:
 *   - Distance to move:
 *       DBL Coef;  
 * RETURNS: None.
 */
VOID PD6_RndCameraMoveDir( pd6CAMERA *Cam, DBL Coef )
{
  Cam->Dir = VecNormalize(VecSubVec(Cam->At, Cam->Loc));
  Cam->Loc = VecAddVec(Cam->Loc, VecMulNum(Cam->Dir, Coef));
  Cam->At = VecAddVec(Cam->At, VecMulNum(Cam->Dir, Coef));
}  /* End of 'PD6_RndCameraMoveByDir' funciton */

/* Camera move by Right-Axes function.
 * ARGUMENTS:
 *   - Distance to move:
 *       DBL Coef;  
 * RETURNS: None.
 */
VOID PD6_RndCameraMoveRight( pd6CAMERA *Cam, DBL Coef )
{
  Cam->Dir = VecSubVec(Cam->At, Cam->Loc);
  Cam->Right = VecCrossVec(Cam->Dir, Cam->Up);
  Cam->Loc = VecAddVec(Cam->Loc, VecMulNum(VecNormalize(Cam->Right), Coef));
  Cam->At = VecAddVec(Cam->At, VecMulNum(VecNormalize(Cam->Right), Coef));
}  /* End of 'PD6_RndCameraMoveByDir' funciton */

/* Camera move by Up-Axes function.
 * ARGUMENTS:
 *   - Distance to move:
 *       DBL Coef;  
 * RETURNS: None.
 */
VOID PD6_RndCameraMoveUp( pd6CAMERA *Cam, DBL Coef )
{
  Cam->Loc = VecAddVec(Cam->Loc, VecMulNum(VecNormalize(Cam->Up), Coef));
  Cam->At = VecAddVec(Cam->At, VecMulNum(VecNormalize(Cam->Up), Coef));
}  /* End of 'PD6_RndCameraMoveByDir' funciton */

/* Camera rotation by camera-axes-Up function.
 * ARGUMENTS:
 *   - Angle:
 *       DBL Angle;
 * RETURNS: None.
 */
VOID PD6_RndCameraRotateUp( pd6CAMERA *Cam, DBL Angle )
{
  MATR Matr;

  Cam->Dir = VecSubVec(Cam->At, Cam->Loc);
  Cam->Right = VecCrossVec(VecNormalize(Cam->Dir), Cam->Up);

  PD6_RndCameraNormalize(Cam);

  Matr = PD6_RndCameraGetMatrix(Cam);
  Matr = MatrMulMatr(MatrRotateY(Angle), Matr);

  PD6_RndCameraSet(Cam, Cam->Loc,                                                                  /* Location */
                   VecAddVec(Cam->Loc, 
                             VecSet(Matr.A[0][0], Matr.A[0][1], Matr.A[0][2])),                     /* At */
                   Cam->Up);                                                                       /* Up */
  // PD6_RndCameraUpdateInfo(Cam);
}  /* End of 'PD6_RndCameraRotateUp' funciton */

/* Camera rotation by camera-axes-right function.
* ARGUMENTS:
*   - Angle:
*       DBL Angle;
* RETURNS: None.
*/
VOID PD6_RndCameraRotateRight( pd6CAMERA *Cam, DBL Angle )
{
  MATR Matr;

  Cam->Dir = VecSubVec(Cam->At, Cam->Loc);
  Cam->Right = VecCrossVec(VecNormalize(Cam->Dir), Cam->Up);

  PD6_RndCameraNormalize(Cam);

  Matr = PD6_RndCameraGetMatrix(Cam);
  Matr = MatrMulMatr(MatrRotateZ(Angle), Matr);

  PD6_RndCameraSet(Cam, Cam->Loc,                                               /* Location */
    VecAddVec(Cam->Loc, 
    VecSet(Matr.A[0][0], Matr.A[0][1], Matr.A[0][2])),                    /* At */
    VecSet(Matr.A[1][0], Matr.A[1][1], Matr.A[1][2]));                    /* Up */
  // PD6_RndCameraUpdateInfo(Cam);
}  /* End of 'PD6_RndCameraRotateRight' funciton */

/* Camera rotation by camera-axes-dir function.
* ARGUMENTS:
*   - Angle:
*       DBL Angle;
* RETURNS: None.
*/
VOID PD6_RndCameraRotateDir( pd6CAMERA *Cam, DBL Angle )
{
  MATR Matr;

  Cam->Dir = VecSubVec(Cam->At, Cam->Loc);
  Cam->Right = VecCrossVec(VecNormalize(Cam->Dir), Cam->Up);
  PD6_RndCameraNormalize(Cam);

  Matr = PD6_RndCameraGetMatrix(Cam);
  Matr = MatrMulMatr(MatrRotateX(Angle), Matr);

  PD6_RndCameraSet(Cam, Cam->Loc,                                               /* Location */
    Cam->At,                                                                    /* At */
    VecSet(Matr.A[1][0], Matr.A[1][1], Matr.A[1][2]));                    /* Up */
  // PD6_RndCameraUpdateInfo(Cam);
} /* End of 'PD6_RndCameraRotateDir' funciton */

/* Camera update information function.
* ARGUMENTS:
*   - Angle:
*       DBL Angle;
* RETURNS: None.
*/
VOID PD6_RndCameraUpdateInfo( pd6CAMERA *Cam )
{
  PD6_RndCameraNormalize(Cam);
  Cam->Dir = VecNormalize(VecSubVec(Cam->At, Cam->Loc));
  Cam->Right = VecCrossVec(Cam->Dir, Cam->Up);
  PD6_RndCameraNormalize(Cam);
} /* End of 'PD6_RndCameraUpdateInfo' funciton */

/* END OF 'CAMERA.C' FILE */