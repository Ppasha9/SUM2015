/* FILE NAME: T04DETERM.C
 * PROGRAMMER: PD6
 * DATE: 04.06.2015
 * PURPOSE: Determinant samples.
 */

#include <stdio.h>
#include <conio.h>
#include <windows.h>

/* ������������ ������ ������� */
#define MAX 10

INT PD6_P[MAX];                    /* ������ ������������(PD6_P) */
DOUBLE Matr[MAX][MAX], DETERM = 0; /* �������(Matr) � � ������������(DETERM) */
INT Parity = 0, N;                 /* �������� ������������(Parity) � ������ �������(N) */
FILE *F;                           /* ���� ��� ������ ���������� */

/* ������� �������� ������� � ��������� ������� �� �����.
 * ���������:
 *   - ��� ����� ��� ��������:
 *       CHAR *FileName;
 * ������������ ��������: ���.
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


/* ������� ���������� ��� ��������.
 * ���������:
 *   - ��������� �� ������ ��������:
 *       INT *A;
 *   - ��������� �� ������ ��������:
 *       INT *B;
 * ������������ ��������: ���.
 */   
VOID Swap( INT *A, INT *B )
{
  INT tmp = *A;

  *A = *B;
  *B = tmp;
} /* End of 'Swap' function */


/* ������� ��� �������� � ����� �����.
 * ���������:
 *   - �����, ������� ��������� ��������:
 *       INT A;
 * ������������ ��������: ���.
 */   
VOID WriteToFile( DOUBLE A )
{
  if ((F = fopen("Res.log", "a")) != NULL)
  {
    fprintf(F, "The determinant is %g", A);
    fclose(F);
  }
} /* End of 'WriteToFile' function */


/* ������� �������� ������������ � �������� �������.
 * ���������:
 *   - �������, � ������� ���������� �������������� ��������:
 *       INT Pos;
 * ������������ ��������: ���.
 */   
VOID Go( INT Pos )
{
  INT i, x, save;

  if (Pos == N)
  {
    /* ����� ����� ������������ - ������� ������������ */
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

  /* ������� ��� ����������� */
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


/* ������� ������� */
VOID main( VOID )
{
  INT i, j, k;

  /* ������ ���� ��� ������ ���������� */
  if ((F = fopen("Res.log", "w")) != NULL)
    fclose(F);

  /* ��������� ������� �� ����� */
  Load("m.txt");

  /* ��������� ������ ������������ */
  for (i = 0; i < N; i++)
    PD6_P[i] = i + 1;

  Go(0);
  WriteToFile(DETERM);
} /* End of 'main' function */

/* END OF 'T04DETERM.C' FILE */
