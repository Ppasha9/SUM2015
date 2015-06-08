/* FILENAME: URECT.C
 * PROGRAMMER: PD6
 * PURPOSE: Bounce rectangle unit handle module.
 * LAST UPDATE: 08.06.2015
 */

#include <string.h>
#include <time.h>

#include "anim.h"
#include "vec.h"

/* Тип представления мяча */
typedef struct tagpd6UNIT_RECT
{
  PD6_UNIT_BASE_FIELDS;

  VEC Pos;     /* Позиция квадрата */
  DWORD Color; /* Цвет квадрата */
  DBL
    PhaseShift; /* Сдвиг по фазе */
} pd6UNIT_RECT;


/* Функция инициализации объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       pd6UNIT_BALL *Uni;
 *   - указатель на контекст анимации:
 *       pd6ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID PD6_RectUnitInit( pd6UNIT_RECT *Uni, pd6ANIM *Ani )
{
  DBL t = clock() / (DOUBLE)CLOCKS_PER_SEC;

  Uni->Pos = VecSet(rand() % 1920, rand() % 1080, 0);
  Uni->Color = RGB(rand() % 256, rand() % 256, rand() % 256);
  Uni->PhaseShift = t * 30 * 59;
} /* End of 'PD6_AnimUnitInit' function */

/* Функция деинициализации объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       pd6UNIT_BALL *Uni;
 *   - указатель на контекст анимации:
 *       pd6ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID PD6_RectUnitClose( pd6UNIT_RECT *Uni, pd6ANIM *Ani )
{
} /* End of 'PD6_AnimUnitClose' function */

/* Функция обновления межкадровых параметров объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       pd6UNIT_BALL *Uni;
 *   - указатель на контекст анимации:
 *       pd6ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID PD6_RectUnitResponse( pd6UNIT_RECT *Uni, pd6ANIM *Ani )
{
  if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)
    PD6_AnimDoExit();
  if (GetAsyncKeyState('F') & 0x8000)
    PD6_AnimFlipFullScreen();
} /* End of 'PD6_AnimUnitResponse' function */

/* Функция построения объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       pd6UNIT_BALL *Uni;
 *   - указатель на контекст анимации:
 *       pd6ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID PD6_RectUnitRender( pd6UNIT_RECT *Uni, pd6ANIM *Ani )
{
  DBL shift = fabs(sin(Uni->PhaseShift));

  SetDCBrushColor(Ani->hDC, Uni->Color);
  Rectangle(Ani->hDC, Uni->Pos.X - 5, Uni->Pos.Y - 5 - shift, Uni->Pos.X + 5, Uni->Pos.Y + 5 - shift);
} /* End of 'PD6_AnimUnitRender' function */

/* Функция создания объекта анимации "квадрат".
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (pd6UNIT *) указатель на созданный объект анимации.
 */
pd6UNIT * PD6_UnitRectCreate( VOID )
{
  pd6UNIT_RECT *Uni;

  if ((Uni = (VOID *)PD6_AnimUnitCreate(sizeof(pd6UNIT_RECT))) == NULL)
    return NULL;
  /* заполняем поля */
  Uni->Init = (VOID *)PD6_RectUnitInit;
  Uni->Close = (VOID *)PD6_RectUnitClose;
  Uni->Response = (VOID *)PD6_RectUnitResponse;
  Uni->Render = (VOID *)PD6_RectUnitRender;
  return (pd6UNIT *)Uni;
} /* End of 'PD6_UnitRectCreate' function */

/* END OF 'URECT.C' FILE */