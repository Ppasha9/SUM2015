/* FILE NAME: GLOBE.H
 * PROGRAMMER: PD6
 * DATE: 05.06.2015
 * PURPOSE: Sphera drawing.
 */

#ifndef __GLOBE_H_
#define __GLOBE_H_

#pragma warning(disable: 4244)

/* ��� �������� ��������� ����� � ������������ */
typedef struct
{
  DOUBLE X, Y, Z;
} VEC;

/* �� */
#define PD6_PI 3.141592653589793238462643383278502884197169399375105820974944

/* ������� ���������� ����� - ������� ��������� �����. */
VOID GlobeBuild( VOID );

/* ������� ��������� �����. */
VOID GlobeDraw( HDC hDC, INT W, INT H );


#endif /* __GLOBE_H_ */

/* END OF 'GLOBE.H' FILE */





