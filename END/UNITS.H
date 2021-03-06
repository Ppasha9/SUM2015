/* FILENAME: UNITS.H
 * PROGRAMMER: PD6
 * PURPOSE: Animation units samples declaration module.
 * LAST UPDATE: 08.06.2015
 */

#ifndef __UNITS_H_
#define __UNITS_H_

#include "def.h"


/* ������� �������� ������� �������� "���".
 * ���������: ���.
 * ������������ ��������:
 *   (pd6UNIT *) ��������� �� ��������� ������ ��������.
 */
pd6UNIT * PD6_UnitBallCreate( VOID );

/* ������� �������� ������� �������� "������".
 * ���������: ���.
 * ������������ ��������:
 *   (pd6UNIT *) ��������� �� ��������� ������ ��������.
 */
pd6UNIT * PD6_UnitModelCreate( VOID );

/* ������� �������� ������� �������� "�������".
 * ���������: ���.
 * ������������ ��������:
 *   (pd6UNIT *) ��������� �� ��������� ������ ��������.
 */
pd6UNIT * PD6_UnitRectCreate( VOID );

/* ������� �������� ������� �������� "����������".
 * ���������: ���.
 * ������������ ��������:
 *   (pd6UNIT *) ��������� �� ��������� ������ ��������.
 */
pd6UNIT * PD6_UnitControlCreate( VOID );

/* Unit flying camera create function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (pd6UNIT_CAMFLY *) - pointer for new animation unit.
 */
pd6UNIT *PD6_UnitCamFlyCreate( VOID );

/* ������� �������� ������� �������� "����".
 * ���������: ���.
 * ������������ ��������:
 *   (pd6UNIT *) ��������� �� ��������� ������ ��������.
 */
pd6UNIT * PD6_UnitWaterCreate( VOID );

/* ������� �������� ������� �������� "����".
 * ���������: ���.
 * ������������ ��������:
 *   (pd6UNIT *) ��������� �� ��������� ������ ��������.
 */
pd6UNIT * PD6_UnitSkyCreate( VOID );

/* Unit Fly create function.
 * ARGUMENTS:
 *   - Postion on world map:
 *       INT PosXPosY;
 *   - Angles of rotation:
 *       DBL RotAngleX, RotAngleY, RotAngleZ;
 * RETURNS:
 *   (pd6UNIT_FLY *) - pointer for new animation unit.
 */
pd6UNIT *PD6_UnitFlyCreate( INT PosX, INT PosY, INT PosZ );

#endif /* __UNITS_H_ */

/* END OF 'UNITS.H' FILE */
