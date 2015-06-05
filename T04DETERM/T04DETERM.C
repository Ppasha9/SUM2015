/* FILE NAME: T04DETERM.C
 * PROGRAMMER: PD6
 * DATE: 04.06.2015
 * PURPOSE: Determinant samples.
 */

#include <stdio.h>
#include <conio.h>
#include <windows.h>

/* Максимальный размер матрицы */
#define MAX 10

INT PD6_P[MAX];                    /* Массив перестановок(PD6_P) */
DOUBLE Matr[MAX][MAX], DETERM = 0; /* Матрица(Matr) и её определитель(DETERM) */
INT Parity = 0, N;                 /* Четность перестановки(Parity) и размер матрицы(N) */
FILE *F;                           /* Файл для записи результата */

/* Функция загрузки размера и элементов матрицы из файла.
 * АРГУМЕНТЫ:
 *   - имя файла для загрузки:
 *       CHAR *FileName;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID Load( CHAR *FileName )
{
  FILE *File;
  INT i, j;

  if ((File = fopen(FileName, "r")) != NULL)
  {
    fscanf(File, "%d", &N);
    for (i = 0; i < N; i++)
      for (j = 0; j < N; j++)
        fscanf(File, "%lf", &Matr[i][j]);

    fclose(File);
  }
} /* End of 'Load' function */


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
VOID WriteToFile( DOUBLE A )
{
  if ((F = fopen("Res.log", "a")) != NULL)
  {
    fprintf(F, "The determinant is %g", A);
    fclose(F);
  }
} /* End of 'WriteToFile' function */


/* Функция создания перестановок с заданной позиции.
 * АРГУМЕНТЫ:
 *   - позиция, в которую поочередно переставляются элементы:
 *       INT Pos;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */   
VOID Go( INT Pos )
{
  INT i, x, save;

  if (Pos == N)
  {
    /* Когда нашли перестановку - находим определитель */
    DOUBLE PROD = 1;

    for (i = 0; i < N; i++)
    {
      PROD *= Matr[i][PD6_P[i]];

      if (Parity == 0)
        DETERM += PROD;
      else
        DETERM -= PROD;
    }

    return;
  }

  /* Находим все перстановки */
  save = Parity;
  Go(Pos + 1);
  for (i = Pos + 1; i < N; i++)
  {
    Parity = !Parity;
    Swap(&PD6_P[Pos], &PD6_P[i]);
    Go(Pos + 1);
  }

  Parity = save;
  x = PD6_P[Pos];

  for (i = Pos + 1; i < N; i++)
    PD6_P[i - 1] = PD6_P[i];
  PD6_P[N - 1] = x;
} /* End of 'Go' function */


/* Главная функция */
VOID main( VOID )
{
  INT i, j, k;

  /* Чистим файл для записи результата */
  if ((F = fopen("Res.log", "w")) != NULL)
    fclose(F);

  /* Загружаем матрицу из файла */
  Load("m2.txt");

  /* Заполняем массив перестановок */
  for (i = 0; i < N; i++)
    PD6_P[i] = i + 1;

  Go(0);
  WriteToFile(DETERM);
} /* End of 'main' function */

/* END OF 'T04DETERM.C' FILE */
