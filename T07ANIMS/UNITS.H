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


#endif /* __UNITS_H_ */

/* END OF 'UNITS.H' FILE */
