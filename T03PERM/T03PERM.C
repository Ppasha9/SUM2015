/* FILE NAME: T03PERM.C
 * PROGRAMMER: PD6
 * DATE: 03.06.2015
 * PURPOSE: Permutation samples.
 */

#include <stdio.h>
#include <conio.h>
#include <windows.h>

#define PD6_N 4

INT PD6_P[PD6_N];
INT Parity = 0;
FILE *F;

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
  static INT k = 0;

  if ((F = fopen("Res.log", "a")) != NULL)
  {
    k++;
    fprintf(F, "%d ", A);

    if ((k % PD6_N) == 0)
      fprintf(F, "- %s\n", Parity ? "Odd permutation" : "Even permutation");

    fclose(F);
  }
} /* End of 'WriteToFile' function */


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
  INT i, x, save;

  if (Pos == PD6_N)
  {
    for (i = 0; i < PD6_N; i++)
      WriteToFile(PD6_P[i]);
    return;
  }

  save = Parity;
  Go(Pos + 1);
  for (i = Pos + 1; i < PD6_N; i++)
  {
    Parity = !Parity;
    Swap(&PD6_P[Pos], &PD6_P[i]);
    Go(Pos + 1);
  }

  Parity = save;
  x = PD6_P[Pos];

  for (i = Pos + 1; i < PD6_N; i++)
    PD6_P[i - 1] = PD6_P[i];
  PD6_P[PD6_N - 1] = x;
} /* End of 'Go' function */


/* Главная функция */
VOID main( VOID )
{
  INT i;

  if ((F = fopen("Res.log", "w")) != NULL)
    fclose(F);

  for (i = 0; i < PD6_N; i++)
    PD6_P[i] = i + 1;


  Go(0);
} /* End of 'main' function */

/* END OF 'T03PERM.C' FILE */
