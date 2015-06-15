/* FILENAME: GEOM.C
 * PROGRAMMER: PD6
 * PURPOSE: Rendering system declaration module.
 * LAST UPDATE: 13.06.2015
 */

#include <stdio.h>
#include <stdlib.h>

#include "render.h"

/* Функция добавления примитива к геометрическому объекту.
 * АРГУМЕНТЫ:
 *   - указатель на геометрический объект:
 *       pd6GEOM *G;
 *   - указатель на добавляемый примитив:
 *       pd6PRIM *Prim;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (INT) номер добавленного примитива в массиве (-1 при ошибке).
 */
INT PD6_GeomAddPrim( pd6GEOM *G, pd6PRIM *Prim )
{
  pd6PRIM *new_bulk;

  /* Выделяем память под новый массив примитивов */
  if ((new_bulk = malloc(sizeof(pd6PRIM) * (G->NumOfPrimitives + 1))) == NULL)
    return -1;

  /* Копируем старые примитивы */
  if (G->Prims != NULL)
  {
    memcpy(new_bulk, G->Prims, sizeof(pd6PRIM) * G->NumOfPrimitives);
    free(G->Prims);
  }
  /* указываем на новый массив примитивлв */
  G->Prims = new_bulk;

  /* Сохраняем новый элемент */
  G->Prims[G->NumOfPrimitives] = *Prim;
  return G->NumOfPrimitives++;
} /* End of 'PD6_GeomAddPrim' function */

/* Функция освобождения геометрического объекта.
 * АРГУМЕНТЫ:
 *   - указатель на геометрический объект:
 *       pd6GEOM *G;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
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

/* Функция отображения геометрического объекта.
 * АРГУМЕНТЫ:
 *   - указатель на геометрический объект:
 *       pd6GEOM *G;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID PD6_GeomDraw( pd6GEOM *G )
{
  INT i, loc;

  /* посылаем количество частей */
  glUseProgram(PD6_RndProg);
  loc = glGetUniformLocation(PD6_RndProg, "TotalParts");
  if (loc != -1)
    glUniform1f(loc, G->NumOfPrimitives);
  glUseProgram(0);

  /* рисуем непрозрачные объекты */
  for (i = 0; i < G->NumOfPrimitives; i++)
    if (PD6_MtlLib[G->Prims[i].MtlNo].Kt == 1)
    {
      /* посылаем номер текущей части */
      glUseProgram(PD6_RndProg);
      loc = glGetUniformLocation(PD6_RndProg, "PartNo");
      if (loc != -1)
        glUniform1f(loc, i);
      glUseProgram(0);
      PD6_PrimDraw(&G->Prims[i]);
    }

  /* рисуем прозрачные объекты */
  for (i = 0; i < G->NumOfPrimitives; i++)
    if (PD6_MtlLib[G->Prims[i].MtlNo].Kt != 1)
    {
      /* посылаем номер текущей части */
      glUseProgram(PD6_RndProg);
      loc = glGetUniformLocation(PD6_RndProg, "PartNo");
      if (loc != -1)
        glUniform1f(loc, i);
      glUseProgram(0);
      PD6_PrimDraw(&G->Prims[i]);
    }
} /* End of 'PD6_GeomDraw' function */

/* Функция загрузки геометрического объекта из G3D файла.
 * АРГУМЕНТЫ:
 *   - указатель на геометрический объект:
 *       pd6GEOM *G;
 *   - имя файла:
 *       CHAR *FileName;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (BOOL) TRUE при успехе, иначе - FALSE.
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

  /* читаем сигнатуру */
  fread(Sign, 1, 4, F);
  if (*(DWORD *)Sign != *(DWORD *)"G3D")
  {
    fclose(F);
    return FALSE;
  }

  /* читаем количество примитивов в объекте */
  fread(&n, 4, 1, F);
  fread(MtlName, 1, 300, F);

  /* читаем и загружаем библиотеку материалов */
  _makepath(path_buffer, drive, dir, MtlName, "");
  PD6_MtlLoad(path_buffer);

  /* читаем примитивы */
  for (i = 0; i < n; i++)
  {
    INT nv, ni, *Ind;
    pd6VERTEX *Vert;
    pd6PRIM P;

    /* читаем количество вершин и индексов */
    fread(&nv, 4, 1, F);
    fread(&ni, 4, 1, F);
    /* читаем имя материала текущего примитива */
    fread(MtlName, 1, 300, F);

    /* выделяем память под вершины и индексы */
    if ((Vert = malloc(sizeof(pd6VERTEX) * nv + sizeof(INT) * ni)) == NULL)
      break;
    Ind = (INT *)(Vert + nv);

    /* читаем данные */
    fread(Vert, sizeof(pd6VERTEX), nv, F);
    /* конвертируем геометрию */
    for (j = 0; j < nv; j++)
    {
      Vert[j].P = PointTransform(Vert[j].P, PD6_RndPrimMatrConvert);
      Vert[j].N = VectorTransform(Vert[j].N, M);
    }
    fread(Ind, sizeof(INT), ni, F);

    /* заносим в примитив */
    PD6_PrimCreate(&P, PD6_PRIM_TRIMESH, nv, ni, Vert, Ind);
    P.MtlNo = PD6_MtlFind(MtlName);

    free(Vert);

    /* добавляем примитив к объекту */
    PD6_GeomAddPrim(G, &P);
  }
  fclose(F);
  PD6_RndPrimMatrConvert = MatrIdentity();
  return TRUE;
} /* End of 'PD6_GeomDraw' function */

/* END OF 'GEOM.C' FILE */
