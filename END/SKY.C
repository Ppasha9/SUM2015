/* FILENAME: UBALL.C
 * PROGRAMMER: PD6
 * PURPOSE: Bounce ball unit handle module.
 * LAST UPDATE: 08.06.2015
 */

#include <string.h>
#include <time.h>

#include "anim.h"
#include "render.h"
#include "vec.h"

/* ��� ������������� ���� */
typedef struct tagpd6UNIT_SKY
{
  PD6_UNIT_BASE_FIELDS;

  pd6PRIM Sky;
  INT TextId;
} pd6UNIT_SKY;

/* ������� ������������� ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       pd6UNIT_SKY *Uni;
 *   - ��������� �� �������� ��������:
 *       pd6ANIM *Ani;
 * ������������ ��������: ���.
 */
static VOID PD6_AnimUnitInit( pd6UNIT_SKY *Uni, pd6ANIM *Ani )
{
  pd6MATERIAL Mtl = PD6_MtlLib[3];

  PD6_PrimCreateSphere(&Uni->Sky, VecSet(-600, 0, 600), 900, 200, 200);
  Mtl.TexId = PD6_TextureLoad("sky1.bmp");
  Uni->Sky.MtlNo = Uni->TextId = PD6_MtlAdd(&Mtl);
} /* End of 'PD6_AnimUnitInit' function */

/* ������� ��������������� ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       pd6UNIT_SKY *Uni;
 *   - ��������� �� �������� ��������:
 *       pd6ANIM *Ani;
 * ������������ ��������: ���.
 */
static VOID PD6_AnimUnitClose( pd6UNIT_SKY *Uni, pd6ANIM *Ani )
{
  PD6_PrimFree(&Uni->Sky);
} /* End of 'PD6_AnimUnitClose' function */

/* ������� ���������� ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       pd6UNIT_SKY *Uni;
 *   - ��������� �� �������� ��������:
 *       pd6ANIM *Ani;
 * ������������ ��������: ���.
 */
static VOID PD6_AnimUnitRender( pd6UNIT_SKY *Uni, pd6ANIM *Ani )
{
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, Uni->TextId);
  PD6_PrimDraw(&Uni->Sky);
} /* End of 'PD6_AnimUnitRender' function */

/* ������� �������� ������� �������� "����".
 * ���������: ���.
 * ������������ ��������:
 *   (pd6UNIT *) ��������� �� ��������� ������ ��������.
 */
pd6UNIT * PD6_UnitSkyCreate( VOID )
{
  pd6UNIT_SKY *Uni;

  if ((Uni = (VOID *)PD6_AnimUnitCreate(sizeof(pd6UNIT_SKY))) == NULL)
    return NULL;
  /* ��������� ���� */
  Uni->Init = (VOID *)PD6_AnimUnitInit;
  Uni->Close = (VOID *)PD6_AnimUnitClose;
  Uni->Render = (VOID *)PD6_AnimUnitRender;
  return (pd6UNIT *)Uni;
} /* End of 'PD6_UnitBallCreate' function */

/* END OF 'UBALL.C' FILE */
