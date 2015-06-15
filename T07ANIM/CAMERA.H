/* FILENAME: CAMERA.H
 * PROGRAMMER: PD6
 * PURPOSE: 3D render declaration module.
 * LAST UPDATE: 12.06.2014
 */

#ifndef __CAMERA_H_
#define __CAMERA_H_

#include "vec.h"

/* Camera properties */
typedef struct
{
  VEC
    Loc,             /* Location */
    Dir, Up, Right,  /* Orient vectors */
    At;              /* Point to view */
} pd6CAMERA;

/***
 * Camera functions
 ***/

/* Getting the camera's matrix */
MATR PD6_RndCameraGetMatrix( pd6CAMERA *Cam );

/* Setting cameras properties */
VOID PD6_RndCameraSet( pd6CAMERA *Cam, VEC Loc, VEC At, VEC Up );

/* Setting cameras properties */
VOID PD6_RndCameraMoveDir( pd6CAMERA *Cam, DBL Coef );

/* Camera move by Right-Axes function. */
VOID PD6_RndCameraMoveRight( pd6CAMERA *Cam, DBL Coef );

/* Camera move by Up-Axes function. */
VOID PD6_RndCameraMoveUp( pd6CAMERA *Cam, DBL Coef );

/* Camera rotation by axes Y function. */
VOID PD6_RndCameraRotateUp( pd6CAMERA *Cam, DBL Angle );

/* Camera rotation by camera-axes-right function. */
VOID PD6_RndCameraRotateRight( pd6CAMERA *Cam, DBL Angle );

/* Camera rotation by camera-axes-dir function. */
VOID PD6_RndCameraRotateDir( pd6CAMERA *Cam, DBL Angle );

/* Cameras normalizing vectors function */
VOID PD6_RndCameraNormalize( pd6CAMERA *Cam );

/* Camera update information function. */
VOID PD6_RndCameraUpdateInfo( pd6CAMERA *Cam );

#endif /* __CAMERA_H_ */

/* END OF 'CAMERA.H' FILE */