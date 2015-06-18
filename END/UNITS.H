/* FILENAME: UNITS.H
 * PROGRAMMER: PD6
 * PURPOSE: Animation units samples declaration module.
 * LAST UPDATE: 08.06.2015
 */

#ifndef __UNITS_H_
#define __UNITS_H_

#include "def.h"


/* Функция создания объекта анимации "мяч".
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (pd6UNIT *) указатель на созданный объект анимации.
 */
pd6UNIT * PD6_UnitBallCreate( VOID );

/* Функция создания объекта анимации "корова".
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (pd6UNIT *) указатель на созданный объект анимации.
 */
pd6UNIT * PD6_UnitModelCreate( VOID );

/* Функция создания объекта анимации "квадрат".
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (pd6UNIT *) указатель на созданный объект анимации.
 */
pd6UNIT * PD6_UnitRectCreate( VOID );

/* Функция создания объекта анимации "управление".
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (pd6UNIT *) указатель на созданный объект анимации.
 */
pd6UNIT * PD6_UnitControlCreate( VOID );

/* Unit flying camera create function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (pd6UNIT_CAMFLY *) - pointer for new animation unit.
 */
pd6UNIT *PD6_UnitCamFlyCreate( VOID );

/* Функция создания объекта анимации "вода".
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (pd6UNIT *) указатель на созданный объект анимации.
 */
pd6UNIT * PD6_UnitWaterCreate( VOID );

/* Функция создания объекта анимации "небо".
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (pd6UNIT *) указатель на созданный объект анимации.
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
