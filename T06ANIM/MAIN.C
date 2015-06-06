/* FILE NAME: MAIN.C
 * PROGRAMMER: PD6
 * DATE: 06.06.2015
 * PURPOSE: All mathemetical samples.
 */

#include <conio.h>
#include <stdio.h>

#include "vec.h"

void main( void )
{
  VEC v = {1, 2, 3}, v1 = {2, 4, 6};
  MATR m =
  {
    {
      {1,  2, 3, 4},
      {1,  0, 3, 4},
      {1, 78, 3, 4},
      {1,  2, 2, 4}
    }
  }, m1 =
  {
    {
      {0, 0, 0, 0},
      {0, 0, 0, 0},
      {0, 0, 0, 0},
      {0, 0, 0, 0}
    }
  }, res;
  INT i, j;


  res = MatrTranspose(m);

  for (i = 0; i < 4; i++)
  {
    for (j = 0; j < 4; j++)
      printf("%g ", res.A[i][j]);
    printf("\n");
  }

  v = VecAddVec(v, v1);
  printf("%g %g %g", v.X, v.Y, v.Z);

  getch();
}
