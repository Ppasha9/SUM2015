/* FILE NAME: GLOBEDRAW.C
 * PROGRAMMER: PD6
 * DATE: 05.06.2015
 * PURPOSE: WinAPI sphere drawing.
 */

#include <math.h>
#include <time.h>
#include <stdlib.h>

#include <windows.h>

#include "globe.h"
#include "image.h"

#define N 100
#define M 100

/* ������ �������� ��������� */
static VEC Grid[N][M];

extern INT IsWire, PicH, PicW;
INT Radius = 228;
extern BYTE *Pic;

/* Main picture */
IMAGE GlobeImg;

/* ������� ���������� ������������ */
VEC VecCrossVec( VEC A, VEC B )
{
  VEC r;

  r.X = A.Y * B.Z - A.Z * B.Y;
  r.Y = A.Z * B.X - A.X * B.Z;
  r.Z = A.X * B.Y - A.Y * B.X;
  return r;
} /* End of 'VecCrossVec' function */

/* ������� ��������� �������� */
VEC VecSubVec( VEC A, VEC B )
{
  VEC r;

  r.X = A.X - B.X;
  r.Y = A.Y - B.Y;
  r.Z = A.Z - B.Z;
  return r;
} /* End of 'VecSubVec' function */

/* ������� �������� ����� ������ ��� X */
VEC RotateX( VEC P, DOUBLE AngleDegree )
{
  DOUBLE a = AngleDegree * PD6_PI / 180, si = sin(a), co = cos(a);
  VEC r;

  r.X = P.X;
  r.Y = P.Y * co - P.Z * si;
  r.Z = P.Y * si + P.Z * co;
  return r;
} /* End of 'RotateX' function */

/* ��������� ���������������� */
VOID DrawQuad( HDC hDC, VEC P0, VEC P1, VEC P2, VEC P3, INT W, INT H )
{
  //VEC Norm = VecCrossVec(VecSubVec(P3, P0), VecSubVec(P1, P0));
  POINT pnts[4];

  /* back-face culling */
  /*if (Norm.Z > 0)
    return;  */

  pnts[0].x = P0.X + W / 2;
  pnts[0].y = -P0.Y + H / 2;

  pnts[1].x = P1.X + W / 2;
  pnts[1].y = -P1.Y + H / 2;

  pnts[2].x = P2.X + W / 2;
  pnts[2].y = -P2.Y + H / 2;

  pnts[3].x = P3.X + W / 2;
  pnts[3].y = -P3.Y + H / 2;

  if ((pnts[0].x - pnts[1].x) * (pnts[0].y + pnts[1].y) +
      (pnts[1].x - pnts[2].x) * (pnts[1].y + pnts[2].y) +
      (pnts[2].x - pnts[3].x) * (pnts[2].y + pnts[3].y) +
      (pnts[3].x - pnts[0].x) * (pnts[3].y + pnts[0].y) < 0)
    return;

  Polygon(hDC, pnts, 4);
} /* End of 'DrawQuad' function */


/* ������� ���������� ����� - ������� ��������� �����.
 * ���������: ���.
 * ������������ ��������: ���.
 */
VOID GlobeBuild( VOID )
{
  DOUBLE phi, theta, t = clock() / (DOUBLE)CLOCKS_PER_SEC;
  INT i, j;

  if (GlobeImg.hBm == NULL)
    ImageLoad(&GlobeImg, "M.BMP");

  srand(30);
  for (i = 0; i < N; i++)
  {
    theta = i / (N - 1.0) * PD6_PI;
    for (j = 0; j < M; j++)
    {
      phi = j / (M - 1.0) * 2 * PD6_PI + t;

      Grid[i][j].X = Radius * sin(theta) * sin(phi);
      Grid[i][j].Y = Radius * cos(theta);
      Grid[i][j].Z = Radius * sin(theta) * cos(phi);

      Grid[i][j] = RotateX(Grid[i][j], sin(t * 3) * 30);
    }
  }
} /* End of 'GlobeBuild' function */

/* ������� ��������� �����.
 * ���������:
 *   - ���������� ��������� ����:
 *       HDC hDC;
 *   - ������ ����:
 *       INT W;
 *   - ������ ����:
 *       INT H;
 * ������������ ��������: ���.
 */
VOID GlobeDraw( HDC hDC, INT W, INT H )
{
  INT i, j;

  if (!IsWire)
  {
    SelectObject(hDC, GetStockObject(NULL_PEN));
    SelectObject(hDC, GetStockObject(DC_BRUSH));
  }
  else
  {
    SelectObject(hDC, GetStockObject(BLACK_PEN));
    SelectObject(hDC, GetStockObject(NULL_BRUSH));
  }

  for (i = 0; i < N - 1; i++)
    for (j = 0; j < M - 1; j++)
    {
      INT
        ix = j * (GlobeImg.W - 1) / (M - 1),
        iy = i * (GlobeImg.H - 1) / (N - 1),
        x = Grid[i][j].X + W / 2,
        y = Grid[i][j].Y + H / 2;

      SetDCBrushColor(hDC, ImageGetP(&GlobeImg, ix, iy));
      DrawQuad(hDC, Grid[i][j], Grid[i][j + 1], Grid[i + 1][j + 1], Grid[i + 1][j], W, H);
    }
} /* End of 'GlobeDraw' function */


/* END OF 'GLOBEDRAW.C' FILE */
