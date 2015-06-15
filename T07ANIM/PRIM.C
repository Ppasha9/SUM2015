/* FILENAME: PRIM.C
 * PROGRAMMER: PD6
 * PURPOSE: Primitive handle module.
 * LAST UPDATE: 13.06.2015
 */

#include "anim.h"
#include "render.h"

/* Матрица изменения примитива при создании */
MATR PD6_RndPrimMatrConvert = PD6_UNIT_MATR;

/* Функция создания примитива.
 * АРГУМЕНТЫ:
 *   - указатель на примитив:
 *       pd6PRIM *Prim;
 *   - тип примитива (PD6_PRIM_***):
 *       pd6PRIM_TYPE Type;
 *   - количество вершин и индексов:
 *       INT NoofV, NoofI;
 *   - массив вершин:
 *       pd6VERTEX *Vertices;
 *   - массив индексов:
 *       INT *Indices;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 *   (BOOL) TRUE при успехе, иначе FALSE.
 */
VOID PD6_PrimCreate( pd6PRIM *Prim, pd6PRIM_TYPE Type,
                     INT NoofV, INT NoofI, pd6VERTEX *Vertices, INT *Indices)
{
  Prim->Type = Type;
  Prim->NumOfI = NoofI;
  /* Создаем буфера OpenGL */
  glGenVertexArrays(1, &Prim->VA);
  glGenBuffers(1, &Prim->VBuf);
  glGenBuffers(1, &Prim->IBuf);

  /* делаем активным массив вершин */
  glBindVertexArray(Prim->VA);
  /* делаем активным буфер вершин */
  glBindBuffer(GL_ARRAY_BUFFER, Prim->VBuf);
  /* сливаем данные */
  glBufferData(GL_ARRAY_BUFFER, sizeof(pd6VERTEX) * NoofV, Vertices, GL_STATIC_DRAW);
  /* делаем активным буфер индексов */
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Prim->IBuf);
  /* сливаем данные */
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(INT) * NoofI, Indices, GL_STATIC_DRAW);

  /* задаем порядок данных */
  /*                    layout,
   *                       количество компонент,
   *                          тип,
   *                                    надо ли нормировать,
   *                                           размер в байтах одного элемента буфера,
   *                                                           смещение в байтах до начала данных */
  glVertexAttribPointer(0, 3, GL_FLOAT, FALSE, sizeof(pd6VERTEX), (VOID *)0); /* позиция */
  glVertexAttribPointer(1, 2, GL_FLOAT, FALSE, sizeof(pd6VERTEX), (VOID *)sizeof(VEC)); /* текстура */
  glVertexAttribPointer(2, 3, GL_FLOAT, FALSE, sizeof(pd6VERTEX), (VOID *)(sizeof(VEC) + sizeof(pd6UV))); /* нормаль */
  glVertexAttribPointer(3, 4, GL_FLOAT, FALSE, sizeof(pd6VERTEX), (VOID *)(sizeof(VEC) * 2 + sizeof(pd6UV))); /* цвет */

  /* включаем нужные аттрибуты (layout) */
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glEnableVertexAttribArray(2);
  glEnableVertexAttribArray(3);

  /* отключаемся от массива вершин */
  glBindVertexArray(0);
} /* End of 'pd6_PrimCreate' function */

/* Функция удаления примитива.
 * АРГУМЕНТЫ:
 *   - указатель на примитив:
 *       pd6PRIM *Prim;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID PD6_PrimFree( pd6PRIM *Prim )
{
  /* делаем активным массив вершин */
  glBindVertexArray(Prim->VA);
  /* "отцепляем" буфера */
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glDeleteBuffers(1, &Prim->VBuf);
  glDeleteBuffers(1, &Prim->IBuf);
  /* делаем неактивным массив вершин */
  glBindVertexArray(0);
  glDeleteVertexArrays(1, &Prim->VA);

  memset(Prim, 0, sizeof(pd6PRIM));
} /* End of 'pd6_PrimFree' function */

/* Функция отрисовки примитива.
 * АРГУМЕНТЫ:
 *   - указатель на примитив:
 *       pd6PRIM *Prim;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID PD6_PrimDraw( pd6PRIM *Prim )
{
  INT loc;
  MATR M;

  PD6_RndMatrWorldViewProj = MatrMulMatr(MatrMulMatr(PD6_RndMatrWorld, PD6_RndMatrView), PD6_RndMatrProj);

  /* оставлено для отлдадки, если нет шейдеров */
  glLoadMatrixf(PD6_RndMatrWorldViewProj.A[0]);

  /* рисуем треугольники */
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

  M = MatrMulMatr(PD6_RndMatrWorld, PD6_RndMatrView);
  loc = glGetUniformLocation(PD6_RndProg, "MatrWV");
  if (loc != -1)
    glUniformMatrix4fv(loc, 1, FALSE, M.A[0]);

  M = MatrInverse(MatrMulMatr(PD6_RndMatrWorld, PD6_RndMatrView));
  loc = glGetUniformLocation(PD6_RndProg, "WheelMatr");
  if (loc != -1)
    glUniformMatrix4fv(loc, 1, FALSE, M.A[0]);

  loc = glGetUniformLocation(PD6_RndProg, "Time");
  if (loc != -1)
    glUniform1f(loc, PD6_Anim.Time);

  glPrimitiveRestartIndex(0xFFFFFFFF);
  if (Prim->Type == PD6_PRIM_GRID)
    glDrawElements(GL_TRIANGLE_STRIP, Prim->NumOfI, GL_UNSIGNED_INT, NULL);
  else
    glDrawElements(GL_TRIANGLES, Prim->NumOfI, GL_UNSIGNED_INT, NULL);

  glUseProgram(0);
  glBindVertexArray(0);
} /* End of 'PD6_PrimDraw' function */

/* Функция создания примитива плоскость.
 * АРГУМЕНТЫ:
 *   - указатель на примитив:
 *       pd6PRIM *Prim;
 *   - касательные вектора-стороны:
 *       VEC Du, Dv;
 *   - разбиение:
 *       INT N, M;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (BOOL) TRUE при успехе, иначе FALSE.
 */
BOOL PD6_PrimCreatePlane( pd6PRIM *Prim, VEC Du, VEC Dv, INT N, INT M )
{
} /* End of 'PD6_PrimCreatePlane' function */

/* Функция создания примитива сфера.
 * АРГУМЕНТЫ:
 *   - указатель на примитив:
 *       pd6PRIM *Prim;
 *   - центр сферы:
 *       VEC С;
 *   - радиус сферы:
 *       FLT R;
 *   - разбиение:
 *       INT N, M;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (BOOL) TRUE при успехе, иначе FALSE.
 */
BOOL PD6_PrimCreateSphere( pd6PRIM *Prim, VEC C, FLT R, INT N, INT M )
{
} /* End of 'PD6_PrimCreateSphere' function */

/* END OF 'PRIM.C' FILE */
