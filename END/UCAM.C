/* FILENAME: UCAM.C
 * PROGRAMMER: PD6
 * PURPOSE: Animation unit handle module.
 * LAST UPDATE: 09.06.2014
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "anim.h"
#include "render.h"

/* Unit flying camera struct definition */
typedef struct tagpd6UNIT_CAMFLY
{
  PD6_UNIT_BASE_FIELDS;   /* Base fields */
} pd6UNIT_CAMFLY;

/* Unit flying camera create function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (pd6UNIT_CAMFLY *) - pointer for new animation unit.
 */
pd6UNIT *PD6_UnitCamFlyCreate( VOID )
{
  pd6UNIT_CAMFLY *Unit;

  if ((Unit = (pd6UNIT_CAMFLY *)PD6_AnimUnitCreate(sizeof(pd6UNIT_CAMFLY))) == NULL)
    return NULL;

  return (pd6UNIT *)Unit;
} /* End of 'PD6_UnitCamFlyCreate' function */

/* END OF 'UCAM.C' FILE */