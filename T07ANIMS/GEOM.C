/* FILENAME: GEOM.C
 * PROGRAMMER: PD6
 * PURPOSE: Rendering system declaration module.
 * LAST UPDATE: 13.06.2015
 */

#include <stdio.h>
#include <stdlib.h>

#include "render.h"

/* ������� ���������� ��������� � ��������������� �������.
 * ���������:
 *   - ��������� �� �������������� ������:
 *       pd6GEOM *G;
 *   - ��������� �� ����������� ��������:
 *       pd6PRIM *Prim;
 * ������������ ��������:
 *   (INT) ����� ������������ ��������� � ������� (-1 ��� ������).
 */
INT PD6_GeomAddPrim( pd6GEOM *G, pd6PRIM *Prim )
{
  pd6PRIM *new_bulk;

  /* �������� ������ ��� ����� ������ ���������� */
  if ((new_bulk = malloc(sizeof(pd6PRIM) * (G->NumOfPrimitives + 1))) == NULL)
    return -1;

  /* �������� ������ ��������� */
  if (G->Prims != NULL)
  {
    memcpy(new_bulk, G->Prims, sizeof(pd6PRIM) * G->NumOfPrimitives);
    free(G->Prims);
  }
  /* ��������� �� ����� ������ ���������� */
  G->Prims = new_bulk;

  /* ��������� ����� ������� */
  G->Prims[G->NumOfPrimitives] = *Prim;
  return G->NumOfPrimitives++;
} /* End of 'PD6_GeomAddPrim' function */

/* ������� ������������ ��������������� �������.
 * ���������:
 *   - ��������� �� �������������� ������:
 *       pd6GEOM *G;
 * ������������ ��������: ���.
 */
VOID PD6_GeomFree( pd6GEOM *G )
{
  INT i;

  if (G->Prims != NULL)
  {
    for (i = 0; i < G->NumOfPrimitives; i++)
      PD6_PrimFree(&G->Prims[i]);
    free(G->Prims);
  }
  memset(G, 0, sizeof(pd6GEOM));
} /* End of 'PD6_GeomFree' function */

/* ������� ����������� ��������������� �������.
 * ���������:
 *   - ��������� �� �������������� ������:
 *       pd6GEOM *G;
 * ������������ ��������: ���.
 */
VOID PD6_GeomDraw( pd6GEOM *G )
{
  INT i, loc;

  /* �������� ���������� ������ */
  glUseProgram(PD6_RndProg);
  loc = glGetUniformLocation(PD6_RndProg, "TotalParts");
  if (loc != -1)
    glUniform1f(loc, G->NumOfPrimitives);
  glUseProgram(0);

  /* ������ ������������ ������� */
  for (i = 0; i < G->NumOfPrimitives; i++)
    if (PD6_MtlLib[G->Prims[i].MtlNo].Kt == 1)
    {
      /* �������� ����� ������� ����� */
      glUseProgram(PD6_RndProg);
      loc = glGetUniformLocation(PD6_RndProg, "PartNo");
      if (loc != -1)
        glUniform1f(loc, i);
      glUseProgram(0);
      PD6_PrimDraw(&G->Prims[i]);
    }

  /* ������ ���������� ������� */
  for (i = 0; i < G->NumOfPrimitives; i++)
    if (PD6_MtlLib[G->Prims[i].MtlNo].Kt != 1)
    {
      /* �������� ����� ������� ����� */
      glUseProgram(PD6_RndProg);
      loc = glGetUniformLocation(PD6_RndProg, "PartNo");
      if (loc != -1)
        glUniform1f(loc, i);
      glUseProgram(0);
      PD6_PrimDraw(&G->Prims[i]);
    }
} /* End of 'PD6_GeomDraw' function */

/* ������� �������� ��������������� ������� �� G3D �����.
 * ���������:
 *   - ��������� �� �������������� ������:
 *       pd6GEOM *G;
 *   - ��� �����:
 *       CHAR *FileName;
 * ������������ ��������:
 *   (BOOL) TRUE ��� ������, ����� - FALSE.
 */
BOOL PD6_GeomLoad( pd6GEOM *G, CHAR *FileName )
{
  FILE *F;
  INT i, j, n;
  CHAR Sign[4];
  MATR M;
  static CHAR MtlName[300];
  static CHAR
    path_buffer[_MAX_PATH],
    drive[_MAX_DRIVE],
    dir[_MAX_DIR],
    fname[_MAX_FNAME],
    ext[_MAX_EXT];

  _splitpath(FileName, drive, dir, fname, ext);

  memset(G, 0, sizeof(pd6GEOM));
  if ((F = fopen(FileName, "rb")) == NULL)
    return FALSE;

  M = MatrTranspose(MatrInverse(PD6_RndPrimMatrConvert));

  /* ������ ��������� */
  fread(Sign, 1, 4, F);
  if (*(DWORD *)Sign != *(DWORD *)"G3D")
  {
    fclose(F);
    return FALSE;
  }

  /* ������ ���������� ���������� � ������� */
  fread(&n, 4, 1, F);
  fread(MtlName, 1, 300, F);

  /* ������ � ��������� ���������� ���������� */
  _makepath(path_buffer, drive, dir, MtlName, "");
  PD6_MtlLoad(path_buffer);

  /* ������ ��������� */
  for (i = 0; i < n; i++)
  {
    INT nv, ni, *Ind;
    pd6VERTEX *Vert;
    pd6PRIM P;

    /* ������ ���������� ������ � �������� */
    fread(&nv, 4, 1, F);
    fread(&ni, 4, 1, F);
    /* ������ ��� ��������� �������� ��������� */
    fread(MtlName, 1, 300, F);

    /* �������� ������ ��� ������� � ������� */
    if ((Vert = malloc(sizeof(pd6VERTEX) * nv + sizeof(INT) * ni)) == NULL)
      break;
    Ind = (INT *)(Vert + nv);

    /* ������ ������ */
    fread(Vert, sizeof(pd6VERTEX), nv, F);
    /* ������������ ��������� */
    for (j = 0; j < nv; j++)
    {
      Vert[j].P = PointTransform(Vert[j].P, PD6_RndPrimMatrConvert);
      Vert[j].N = VectorTransform(Vert[j].N, M);
    }
    fread(Ind, sizeof(INT), ni, F);

    /* ������� � �������� */
    PD6_PrimCreate(&P, PD6_PRIM_TRIMESH, nv, ni, Vert, Ind);
    P.MtlNo = PD6_MtlFind(MtlName);

    free(Vert);

    /* ��������� �������� � ������� */
    PD6_GeomAddPrim(G, &P);
  }
  fclose(F);
  PD6_RndPrimMatrConvert = MatrIdentity();
  return TRUE;
} /* End of 'PD6_GeomDraw' function */

/* END OF 'GEOM.C' FILE */
