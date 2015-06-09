/* FILENAME: LOADOBJ.C
 * PROGRAMMER: PD6
 * PURPOSE: Load 3D nmodel from *.OBJ files.
 * LAST UPDATE: 06.06.2015
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "anim.h"
#include "obj3d.h"

extern BOOL IsWire, IsRainbow, IsRed, IsPause;

/* Global model data */

/* Vertex array */
VEC *ObjV; /* Vertex coordinates */
INT ObjNumOfV; /* Number of model vertices */

/* Draw object functioln.
 * ARGUMENTS:
 *   - context of the window:
 *       HDC hDC;
 *   - the size of the window:
 *       INT W, H;
 * RETURNS: None.
 */
VOID ObjDraw( HDC hDC, INT X, INT Y, DWORD Color, pd6ANIM *Ani )
{
  INT i;
  DBL x = sin(30);

  srand(30);
  for (i = 0; i < ObjNumOfV; i++)
  {
    /* рисуем точку ObjV[i] */
    if (!Ani->IsPause)
      ObjV[i] = VectorTransform(ObjV[i], MatrMulMatr(MatrRotateY(sin(Ani->Time * 0.5)), MatrRotateX(sin(Ani->Time * 3))));
    else
      ObjV[i] = VectorTransform(ObjV[i], MatrIdentity());

    SetDCBrushColor(hDC, Color);
    Ellipse(hDC,X + ObjV[i].X - 4, Y - ObjV[i].Y - 4, X + ObjV[i].X + 4, Y - ObjV[i].Y + 4);
  }

} /* End of 'ObjDraw' function */

/* Draw object functioln.
 * ARGUMENTS:
 *   - object (*.OBJ) file name:
 *       CHAR *FileName;
 * RETURNS:
 *   (BOOL) TRUE if loading is success, FALSE otherwise.
 */
BOOL ObjLoad( CHAR *FileName )
{
  FILE *F;
  INT nv = 0;
  static CHAR Buf[10000];

  /* Open file */
  if ((F = fopen(FileName, "r")) == NULL)
    return FALSE;

  /* Count vertices */
  while (fgets(Buf, sizeof(Buf), F) != NULL)
  {
    if (Buf[0] == 'v' && Buf[1] == ' ')
      nv++;
  }

  /* Allocate memory for data */
  if ((ObjV = malloc(sizeof(VEC) * nv)) == NULL)
  {
    fclose(F);
    return FALSE;
  }

  /* Read vertices */
  rewind(F);
  nv = 0;
  while (fgets(Buf, sizeof(Buf), F) != NULL)
  {
    if (Buf[0] == 'v' && Buf[1] == ' ')
    {
      sscanf(Buf + 2, "%lf%lf%lf",
        &ObjV[nv].X, &ObjV[nv].Y, &ObjV[nv].Z);
      ObjV[nv] = VecMulNum(ObjV[nv], 60);
      nv++;
    }
  }

  fclose(F);

  ObjNumOfV = nv;
  return TRUE;
} /* End of 'ObjLoad' function */

/* END OF 'LOADOBJ.C' FILE */
