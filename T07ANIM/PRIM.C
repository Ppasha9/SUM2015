/* FILENAME: PRIM.C
 * PROGRAMMER: PD6
 * PURPOSE: Primitive handle module.
 * LAST UPDATE: 13.06.2015
 */

#include "anim.h"
#include "render.h"
#include "image.h"

/* ������� ��������� ��������� ��� �������� */
MATR PD6_RndPrimMatrConvert = PD6_UNIT_MATR;

/* ���� ����� �������� �� ��������� */
COLOR PD6_RndPrimDefaultColor = {1, 1, 1, 1};

/* ������� �������� ���������.
 * ���������:
 *   - ��������� �� ��������:
 *       pd6PRIM *Prim;
 *   - ��� ��������� (PD6_PRIM_***):
 *       pd6PRIM_TYPE Type;
 *   - ���������� ������ � ��������:
 *       INT NoofV, NoofI;
 *   - ������ ������:
 *       pd6VERTEX *Vertices;
 *   - ������ ��������:
 *       INT *Indices;
 * ������������ ��������: ���.
 *   (BOOL) TRUE ��� ������, ����� FALSE.
 */
VOID PD6_PrimCreate( pd6PRIM *Prim, pd6PRIM_TYPE Type,
                     INT NoofV, INT NoofI, pd6VERTEX *Vertices, INT *Indices)
{
  Prim->Type = Type;
  Prim->NumOfI = NoofI;
  /* ������� ������ OpenGL */
  glGenVertexArrays(1, &Prim->VA);
  glGenBuffers(1, &Prim->VBuf);
  glGenBuffers(1, &Prim->IBuf);

  /* ������ �������� ������ ������ */
  glBindVertexArray(Prim->VA);
  /* ������ �������� ����� ������ */
  glBindBuffer(GL_ARRAY_BUFFER, Prim->VBuf);
  /* ������� ������ */
  glBufferData(GL_ARRAY_BUFFER, sizeof(pd6VERTEX) * NoofV, Vertices, GL_STATIC_DRAW);
  /* ������ �������� ����� �������� */
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Prim->IBuf);
  /* ������� ������ */
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(INT) * NoofI, Indices, GL_STATIC_DRAW);

  /* ������ ������� ������ */
  /*                    layout,
   *                       ���������� ���������,
   *                          ���,
   *                                    ���� �� �����������,
   *                                           ������ � ������ ������ �������� ������,
   *                                                           �������� � ������ �� ������ ������ */
  glVertexAttribPointer(0, 3, GL_FLOAT, FALSE, sizeof(pd6VERTEX), (VOID *)0); /* ������� */
  glVertexAttribPointer(1, 2, GL_FLOAT, FALSE, sizeof(pd6VERTEX), (VOID *)sizeof(VEC)); /* �������� */
  glVertexAttribPointer(2, 3, GL_FLOAT, FALSE, sizeof(pd6VERTEX), (VOID *)(sizeof(VEC) + sizeof(pd6UV))); /* ������� */
  glVertexAttribPointer(3, 4, GL_FLOAT, FALSE, sizeof(pd6VERTEX), (VOID *)(sizeof(VEC) * 2 + sizeof(pd6UV))); /* ���� */

  /* �������� ������ ��������� (layout) */
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glEnableVertexAttribArray(2);
  glEnableVertexAttribArray(3);

  /* ����������� �� ������� ������ */
  glBindVertexArray(0);
} /* End of 'pd6_PrimCreate' function */

/* ������� �������� ���������.
 * ���������:
 *   - ��������� �� ��������:
 *       pd6PRIM *Prim;
 * ������������ ��������: ���.
 */
VOID PD6_PrimFree( pd6PRIM *Prim )
{
  /* ������ �������� ������ ������ */
  glBindVertexArray(Prim->VA);
  /* "���������" ������ */
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glDeleteBuffers(1, &Prim->VBuf);
  glDeleteBuffers(1, &Prim->IBuf);
  /* ������ ���������� ������ ������ */
  glBindVertexArray(0);
  glDeleteVertexArrays(1, &Prim->VA);

  memset(Prim, 0, sizeof(pd6PRIM));
} /* End of 'pd6_PrimFree' function */

/* ������� ��������� ���������.
 * ���������:
 *   - ��������� �� ��������:
 *       pd6PRIM *Prim;
 * ������������ ��������: ���.
 */
VOID PD6_PrimDraw( pd6PRIM *Prim )
{
  INT loc;
  MATR M;

  PD6_RndMatrWorldViewProj = MatrMulMatr(MatrMulMatr(PD6_RndMatrWorld, PD6_RndMatrView), PD6_RndMatrProj);

  /* ��������� ��� ��������, ���� ��� �������� */
  glLoadMatrixf(PD6_RndMatrWorldViewProj.A[0]);

  /* ������ ������������ */
  glBindVertexArray(Prim->VA);
  glUseProgram(PD6_RndProg);

  loc = glGetUniformLocation(PD6_RndProg, "MatrWorld");
  if (loc != -1)
    glUniformMatrix4fv(loc, 1, FALSE, PD6_RndMatrWorld.A[0]);
  loc = glGetUniformLocation(PD6_RndProg, "MatrView");
  if (loc != -1)
    glUniformMatrix4fv(loc, 1, FALSE, PD6_RndMatrView.A[0]);
  loc = glGetUniformLocation(PD6_RndProg, "MatrProj");
  if (loc != -1)
    glUniformMatrix4fv(loc, 1, FALSE, PD6_RndMatrProj.A[0]);
  loc = glGetUniformLocation(PD6_RndProg, "MatrWVP");
  if (loc != -1)
    glUniformMatrix4fv(loc, 1, FALSE, PD6_RndMatrWorldViewProj.A[0]);

  M = MatrTranspose(MatrInverse(MatrMulMatr(PD6_RndMatrWorld, PD6_RndMatrView)));
  loc = glGetUniformLocation(PD6_RndProg, "MatrWVInverse");
  if (loc != -1)
    glUniformMatrix4fv(loc, 1, FALSE, M.A[0]);

  M = MatrTranspose(MatrInverse(PD6_RndMatrWorld));
  loc = glGetUniformLocation(PD6_RndProg, "MatrWInverse");
  if (loc != -1)
    glUniformMatrix4fv(loc, 1, FALSE, M.A[0]);

  M = MatrMulMatr(PD6_RndMatrWorld, PD6_RndMatrView);
  loc = glGetUniformLocation(PD6_RndProg, "MatrWV");
  if (loc != -1)
    glUniformMatrix4fv(loc, 1, FALSE, M.A[0]);

  loc = glGetUniformLocation(PD6_RndProg, "Time");
  if (loc != -1)
    glUniform1f(loc, PD6_Anim.Time);

  /* ���������� ��������� */
  loc = glGetUniformLocation(PD6_RndProg, "Ka");
  if (loc != -1)
    glUniform3fv(loc, 1, &PD6_MtlLib[Prim->MtlNo].Ka.X);
  loc = glGetUniformLocation(PD6_RndProg, "Kd");
  if (loc != -1)
    glUniform3fv(loc, 1, &PD6_MtlLib[Prim->MtlNo].Kd.X);
  loc = glGetUniformLocation(PD6_RndProg, "Ks");
  if (loc != -1)
    glUniform3fv(loc, 1, &PD6_MtlLib[Prim->MtlNo].Ks.X);
  loc = glGetUniformLocation(PD6_RndProg, "Kp");
  if (loc != -1)
    glUniform1f(loc, PD6_MtlLib[Prim->MtlNo].Kp);
  loc = glGetUniformLocation(PD6_RndProg, "Kt");
  if (loc != -1)
    glUniform1f(loc, PD6_MtlLib[Prim->MtlNo].Kt);

  loc = glGetUniformLocation(PD6_RndProg, "IsTextureUse");
  if (PD6_MtlLib[Prim->MtlNo].TexId == 0)
    glUniform1f(loc, 0);
  else
  {
    glUniform1f(loc, 1);
    glBindTexture(GL_TEXTURE_2D, PD6_MtlLib[Prim->MtlNo].TexId);
  }

  glEnable(GL_PRIMITIVE_RESTART);
  glPrimitiveRestartIndex(0xFFFFFFFF);
  if (Prim->Type == PD6_PRIM_GRID)
    glDrawElements(GL_TRIANGLE_STRIP, Prim->NumOfI, GL_UNSIGNED_INT, NULL);
  else
    glDrawElements(GL_TRIANGLES, Prim->NumOfI, GL_UNSIGNED_INT, NULL);

  glUseProgram(0);
  glBindVertexArray(0);
} /* End of 'PD6_PrimDraw' function */

/* ������� �������� ��������� ���������.
 * ���������:
 *   - ��������� �� ��������:
 *       pd6PRIM *Prim;
 *   - ������� �����:
 *       VEC Loc;
 *   - ����������� �������-�������:
 *       VEC Du, Dv;
 *   - ���������:
 *       INT N, M;
 * ������������ ��������:
 *   (BOOL) TRUE ��� ������, ����� FALSE.
 */
BOOL PD6_PrimCreatePlane( pd6PRIM *Prim, VEC Loc, VEC Du, VEC Dv, INT N, INT M )
{
  INT i, j;
  VEC norm;
  INT *Ind, *iptr;
  pd6VERTEX *V, *ptr;

  memset(Prim, 0, sizeof(pd6PRIM));

  if ((V = malloc(sizeof(pd6VERTEX) * N * M +
                  sizeof(INT) * ((2 * M + 1) * (N - 1)))) == NULL)
    return FALSE;
  Ind = (INT *)(V + N * M);

  /* ��������� ������� */
  norm = VecNormalize(VecCrossVec(Du, Dv));
  for (ptr = V, i = 0; i < N; i++)
    for (j = 0; j < M; j++, ptr++)
    {
      ptr->P = VecAddVec(Loc,
                 VecAddVec(VecMulNum(Du, j / (M - 1.0)),
                           VecMulNum(Dv, i / (N - 1.0))));
      ptr->C = PD6_RndPrimDefaultColor;
      ptr->N = norm;
      ptr->T = UVSet(j / (M - 1.0), i / (N - 1.0));
    }

  /* ��������� ������� */
  for (iptr = Ind, i = 0; i < N - 1; i++)
  {
    for (j = 0; j < M; j++)
    {
      /* ������� */
      *iptr++ = i * M + j + M;
      /* ������ */
      *iptr++ = i * M + j;
    }
    /* ��������� ������ ������� ��������� */
    *iptr++ = 0xFFFFFFFF;
  }

  PD6_PrimCreate(Prim, PD6_PRIM_GRID, M * N, (2 * M + 1) * (N - 1), V, Ind);

  free(V);

  return TRUE;
} /* End of 'PD6_PrimCreatePlane' function */

/* ������� �������� ��������� �����.
 * ���������:
 *   - ��������� �� ��������:
 *       pd6PRIM *Prim;
 *   - ����� �����:
 *       VEC �;
 *   - ������ �����:
 *       FLT R;
 *   - ���������:
 *       INT N, M;
 * ������������ ��������:
 *   (BOOL) TRUE ��� ������, ����� FALSE.
 */
BOOL PD6_PrimCreateSphere( pd6PRIM *Prim, VEC C, FLT R, INT N, INT M )
{
  INT i, j;
  INT *Ind, *iptr;
  pd6VERTEX *V, *ptr;

  memset(Prim, 0, sizeof(pd6PRIM));

  if ((V = malloc(sizeof(pd6VERTEX) * N * M +
                  sizeof(INT) * ((2 * M + 1) * (N - 1)))) == NULL)
    return FALSE;
  Ind = (INT *)(V + N * M);

  /* ��������� ������� */
  for (ptr = V, i = 0; i < N; i++)
    for (j = 0; j < M; j++, ptr++)
    {
      DBL
        theta = PI * i / (N - 1.0), phi = 2 * PI * j / (M - 1.0),
        x = sin(theta) * sin(phi),
        y = cos(theta),
        z = sin(theta) * cos(phi);

      ptr->P = VecAddVec(C, VecSet(R * x, R * y, R * z));
      ptr->C = PD6_RndPrimDefaultColor;
      ptr->N = VecSet(x, y, z);
      ptr->T = UVSet(j / (M - 1.0), i / (N - 1.0));
    }

  /* ��������� ������� */
  for (iptr = Ind, i = 0; i < N - 1; i++)
  {
    for (j = 0; j < M; j++)
    {
      /* ������� */
      *iptr++ = i * M + j + M;
      /* ������ */
      *iptr++ = i * M + j;
    }
    /* ��������� ������ ������� ��������� */
    *iptr++ = 0xFFFFFFFF;
  }

  PD6_PrimCreate(Prim, PD6_PRIM_GRID, M * N, (2 * M + 1) * (N - 1), V, Ind);

  free(V);

  return TRUE;
} /* End of 'PD6_PrimCreateSphere' function */

/* ������� �������� ��������� ����� �����.
 * ���������:
 *   - ��������� �� ��������:
 *       pd6PRIM *Prim;
 *   - ������� �����:
 *       VEC Loc;
 *   - ����������� �������-�������:
 *       VEC Du, Dv;
 *   - ������� �� ������:
 *       DBL Scale;
 *   - ��� ����� � ��������:
 *       CHAR *FileName;
 * ������������ ��������:
 *   (BOOL) TRUE ��� ������, ����� FALSE.
 */
BOOL PD6_PrimCreateHeightField( pd6PRIM *Prim, VEC Loc, VEC Du, VEC Dv, DBL Scale, CHAR *FileName )
{
  INT i, j;
  VEC norm;
  INT *Ind, *iptr;
  pd6VERTEX *V, *ptr;
  INT N, M;
  IMAGE Img;

  memset(Prim, 0, sizeof(pd6PRIM));

  if (!ImageLoad(&Img, FileName))
    return FALSE;
  M = Img.W;
  N = Img.H;

  if ((V = malloc(sizeof(pd6VERTEX) * N * M +
                  sizeof(INT) * ((2 * M + 1) * (N - 1)))) == NULL)
    return FALSE;
  Ind = (INT *)(V + N * M);

  /* ��������� ������� */
  norm = VecNormalize(VecCrossVec(Du, Dv));
  for (ptr = V, i = 0; i < N; i++)
    for (j = 0; j < M; j++, ptr++)
    {
      DWORD icolor = ImageGetP(&Img, j, i);
      DBL
        r = ((icolor >> 16) & 0xFF) / 255.0,
        g = ((icolor >> 8) & 0xFF) / 255.0,
        b = ((icolor >> 0) & 0xFF) / 255.0,
        H = r * 0.30 + g * 0.59 + b * 0.11;

      ptr->P = VecAddVec(Loc,
                 VecAddVec(VecAddVec(
                             VecMulNum(Du, j / (M - 1.0)),
                             VecMulNum(Dv, i / (N - 1.0))),
                             VecMulNum(norm, H * Scale)));
      ptr->C = PD6_RndPrimDefaultColor;
      ptr->N = norm;
      ptr->T = UVSet(j / (M - 1.0), i / (N - 1.0));
    }
  ImageFree(&Img);
  /* ��������� ������� */
  for (iptr = Ind, i = 0; i < N - 1; i++)
  {
    for (j = 0; j < M; j++)
    {
      /* ������� */
      *iptr++ = i * M + j + M;
      /* ������ */
      *iptr++ = i * M + j;
    }
    /* ��������� ������ ������� ��������� */
    *iptr++ = 0xFFFFFFFF;
  }

  /* ���������� �������� */
  
  /* �������� ������� ���� ������ */
  for (i = 0; i < N * M; i++)
    V[i].N = VecSet(0, 0, 0);

  /* ��������� �� ���� ������������� */
  for (i = 0; i < N - 1; i++)
    for (j = 0; j < M - 1; j++)
    {
      pd6VERTEX
        *Vij = V + i * M + j,
        *Vi1j = V + (i + 1) * M + j,
        *Vij1 = V + i * M + j + 1,
        *Vi1j1 = V + (i + 1) * M + j + 1;

      /* ������� ����������� */
      norm = VecNormalize(VecCrossVec(VecSubVec(Vij->P, Vi1j->P), VecSubVec(Vi1j1->P, Vi1j->P)));
      Vij->N = VecAddVec(Vij->N, norm);
      Vi1j->N = VecAddVec(Vi1j->N, norm);
      Vi1j1->N = VecAddVec(Vi1j1->N, norm);

      /* ������ ����������� */
      norm = VecNormalize(VecCrossVec(VecSubVec(Vi1j1->P, Vij1->P), VecSubVec(Vij->P, Vij1->P)));
      Vij->N = VecAddVec(Vij->N, norm);
      Vij1->N = VecAddVec(Vij1->N, norm);
      Vi1j1->N = VecAddVec(Vi1j1->N, norm);
    }

  /* ��������� ������� ���� ������ */
  for (i = 0; i < N * M; i++)
    V[i].N = VecNormalize(V[i].N);

  PD6_PrimCreate(Prim, PD6_PRIM_GRID, M * N, (2 * M + 1) * (N - 1), V, Ind);

  free(V);

  return TRUE;
} /* End of 'PD6_PrimCreatePlane' function */

/* END OF 'PRIM.C' FILE */
