/* FILENAME: OBJ3D.H
 * PROGRAMMER: PD6
 * PURPOSE: Load 3D nmodel from *.OBJ files.
 * LAST UPDATE: 06.06.2015
 */

#ifndef __OBJ3D_H_
#define __OBJ3D_H_

#include "vec.h"

#include "def.h"

/* Draw object functioln.
 * ARGUMENTS:
 *   - context of the window:
 *       HDC hDC;
 * RETURNS: None.
 */
VOID ObjDraw( HDC hDC, INT W, INT H );

/* Draw object functioln.
 * ARGUMENTS:
 *   - object (*.OBJ) file name:
 *       CHAR *FileName;
 * RETURNS:
 *   (BOOL) TRUE if loading is success, FALSE otherwise.
 */
BOOL ObjLoad( CHAR *FileName );

#endif /* __OBJ3D_H_ */

/* END OF 'OBJ3D.H' FILE */