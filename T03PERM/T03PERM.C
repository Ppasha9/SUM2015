/* FILE NAME: T03PERM.C
 * PROGRAMMER: PD6
 * DATE: 03.06.2015
 * PURPOSE: Permutation samples.
 */

#include <stdio.h>
#include <conio.h>
#include <windows.h>

#define PD6_N 5

INT PD6_P[PD6_N];
INT Parity = 0;


/* Функция заменяющая два значения.
 * АРГУМЕНТЫ:
 *   - указатель на первое значение:
 *       INT *A;
 *   - указатель на второе значение:
 *       INT *B;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */   
VOID Swap( INT *A, INT *B )
{
  INT tmp = *A;

  *A = *B;
  *B = tmp;
} /* End of 'Swap' function */


/* Функция для дозаписи в конец файла.
 * АРГУМЕНТЫ:
 *   - число, которое требуется записать:
 *       INT A;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */   
VOID WriteToFile( INT A )
{
  FILE *F;
  static INT k = 0;

  if ((F = fopen("Res.log", "a")) != NULL)
  {
    k++;
    fprintf(F, "%d ", A);

    if ((k % 5) == 0)
      fprintf(F, "\n");

    fclose(F);

  }
} /* End of 'WriteToFile' function */


/* Функция подсчета инверсий.
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */   
INT CountParity( VOID )
{
  INT i, j, cnt = 0;

  for (i = 0; i < PD6_N - 1; i++)
    for (j = i + 1; j < PD6_N; j++)
      cnt += PD6_P[i] > PD6_P[j];

  return cnt;
} /* End of 'CountParity' function */


/* Функция создания перестановок с заданной позиции.
 * АРГУМЕНТЫ:
 *   - позиция, в которую поочередно переставляются элементы:
 *       INT Pos;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */   
/*VOID Go( INT Pos )
{
  INT i;

  if (Pos == PD6_N)
    for (i = 0; i < PD6_N; i++)
      WriteToFile(PD6_P[i]);
  else
    for (i = Pos; i < PD6_N - 1; i++)
      Swap(&PD6_P[i], &PD6_P[i + 1]), Go(Pos + 1);
} /* End of 'Go' function */


/* Функция создания перестановок с заданной позиции.
 * АРГУМЕНТЫ:
 *   - позиция, в которую поочередно переставляются элементы:
 *       INT Pos;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */   
VOID Go( INT Pos )
{
  INT i;

  if (Pos == PD6_N)
  {
    for (i = 0; i < PD6_N; i++)
      WriteToFile(PD6_P[i]);
    return;
  }
  for (i = Pos; i < PD6_N; i++)
  {
    Swap(&PD6_P[Pos], &PD6_P[i]);
    Go(Pos + 1);
    Swap(&PD6_P[Pos], &PD6_P[i]);
  }
} /* End of 'Go' function */


/* Главная функция */
VOID main( VOID )
{
  INT i;

  for (i = 0; i < PD6_N; i++)
    PD6_P[i] = i + 1;


  Go(0);
} /* End of 'main' function */

/* END OF 'T03PERM.C' FILE */
