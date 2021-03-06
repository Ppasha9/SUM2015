/* FILENAME: UCONTRL.C
 * PROGRAMMER: PD6
 * PURPOSE: Control unit handle module.
 * LAST UPDATE: 08.06.2015
 */

#include <stdio.h>
#include <string.h>
#include <time.h>

#include "anim.h"
#include "render.h"
#include "vec.h"

/* ����������� ����� */
#include <al.h>
#include <alc.h>
#include <alut.h>

#pragma comment(lib, "alut")
#pragma comment(lib, "openal32")

/* ��� ������������� ���� */
typedef struct tagpd6UNIT_CTRL
{
  PD6_UNIT_BASE_FIELDS;
  HFONT hFnt; /* ����� ��� ������ FPS */
  UINT ASrc[2];
  INT ABuf[2];

  FLT Head, V;
  VEC Pos;
  pd6GEOM Geom;
} pd6UNIT_CTRL;

VEC Dir;

/* ������� ������������� ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       pd6UNIT_CTRL *Uni;
 *   - ��������� �� �������� ��������:
 *       pd6ANIM *Ani;
 * ������������ ��������: ���.
 */
static VOID PD6_AnimUnitInit( pd6UNIT_CTRL *Uni, pd6ANIM *Ani )
{
  INT format;
  UINT size, freq;
  VOID *mem;
  CHAR loop;

  Uni->hFnt = CreateFont(30, 0, 0, 0, FW_BOLD, FALSE, FALSE,
    FALSE, RUSSIAN_CHARSET, OUT_DEFAULT_PRECIS,
    CLIP_DEFAULT_PRECIS, PROOF_QUALITY,
    VARIABLE_PITCH, "Bookman Old Style");

  /* ������������� ����� ������� */
  alutInit(NULL, 0);
  alGetError();

  /* ������������� ����� */
  /* ������� ������ */
  alGenBuffers(1, Uni->ABuf);

  /* ��������� ���� � ����� */
  alutLoadWAVFile("a1.wav", &format, &mem, &size, &freq, &loop);
  alBufferData(Uni->ABuf[0], format, mem, size, freq);
  alutUnloadWAV(format, mem, size, freq);

/*  alutLoadWAVFile("a1.wav", &format, &mem, &size, &freq, &loop);
  alBufferData(Uni->ABuf[1], format, mem, size, freq);
  alutUnloadWAV(format, mem, size, freq);   */

  /* ������� ��������� ����� � ��������������� �� */
  alGenSources(1, Uni->ASrc);

  alSourcei(Uni->ASrc[0], AL_BUFFER, Uni->ABuf[0]); /* ���������� ����� �� ���������� */
  alSourcef(Uni->ASrc[0], AL_PITCH, 1);             /* �������� ���������������: 1.0 - �������*/
  alSourcef(Uni->ASrc[0], AL_GAIN, 0.3);            /* ���������: 1.0 � ������� */
  alSourcei(Uni->ASrc[0], AL_LOOPING, 1);           /* ���� �������: 0 � ���, 1 � ���������� */

  alSourcePlay(Uni->ASrc[0]); /* ������ ������������ */

  Uni->Head = Uni->V = 1.0;

  PD6_GeomLoad(&Uni->Geom, "Mirage\\Mirage\\Mirage.g3d");
  //alSourcei(Uni->ASrc[1], AL_BUFFER, Uni->ABuf[1]); /* ���������� ����� �� ���������� */
  //alSourcef(Uni->ASrc[1], AL_PITCH, 1);             /* �������� ���������������: 1.0 - �������*/
  //alSourcef(Uni->ASrc[1], AL_GAIN, 1);              /* ���������: 1.0 � ������� */
  //alSourcei(Uni->ASrc[1], AL_LOOPING, 0);           /* ���� �������: 0 � ���, 1 � ���������� */
} /* End of 'PD6_AnimUnitInit' function */

/* ������� ��������������� ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       pd6UNIT_CTRL *Uni;
 *   - ��������� �� �������� ��������:
 *       pd6ANIM *Ani;
 * ������������ ��������: ���.
 */
static VOID PD6_AnimUnitClose( pd6UNIT_CTRL *Uni, pd6ANIM *Ani )
{
  DeleteObject(Uni->hFnt);
  PD6_GeomFree(&Uni->Geom);
} /* End of 'PD6_AnimUnitClose' function */

/* ������� ���������� ����������� ���������� ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       pd6UNIT_CTRL *Uni;
 *   - ��������� �� �������� ��������:
 *       pd6ANIM *Ani;
 * ������������ ��������: ���.
 */
static VOID PD6_AnimUnitResponse( pd6UNIT_CTRL *Uni, pd6ANIM *Ani )
{
  VEC At, Dir, Move;

  if (Ani->KeysClick[VK_ESCAPE] || Ani->JButsClick[6])
    PD6_AnimDoExit();
  if (Ani->KeysClick['F'])
    PD6_AnimFlipFullScreen();
  if (Ani->KeysClick['P'])
    PD6_AnimSetPause(!Ani->IsPause);

  /* Camera controling */
  if (Ani->IsPause)
  {
    if (Ani->Keys[VK_NUMPAD8])
      PD6_RndCameraMoveDir(&Ani->RndCamera, 1);
    if (Ani->Keys[VK_NUMPAD2])
      PD6_RndCameraMoveDir(&Ani->RndCamera, -1);

    if (Ani->Keys[VK_LEFT])
      PD6_RndCameraMoveRight(&Ani->RndCamera, -1);
    if (Ani->Keys[VK_RIGHT])
      PD6_RndCameraMoveRight(&Ani->RndCamera, 1);

    if (Ani->Keys[VK_UP])
      PD6_RndCameraMoveUp(&Ani->RndCamera, 1);
    if (Ani->Keys[VK_DOWN])
     PD6_RndCameraMoveUp(&Ani->RndCamera, -1);

    if (Ani->Keys[VK_NUMPAD4])
      PD6_RndCameraRotateUp(&Ani->RndCamera, 1);
    if (Ani->Keys[VK_NUMPAD6])
     PD6_RndCameraRotateUp(&Ani->RndCamera, -1);

    if (Ani->Keys[VK_NUMPAD9])
      PD6_RndCameraRotateRight(&Ani->RndCamera, 1);
    if (Ani->Keys[VK_NUMPAD3])
      PD6_RndCameraRotateRight(&Ani->RndCamera, -1);
  }

  if (Ani->Keys['G'])
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  if (Ani->Keys['H'])
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  glEnable(GL_DEPTH_TEST);

  if (Ani->JButsClick[0])
    alSourcePlay(Uni->ASrc[1]);
  if (Ani->KeysClick['S'])
    alSourceStopv(2, Uni->ASrc);

  if (!Ani->IsPause)
  {
    Uni->Head += 300 * Ani->JR * Ani->DeltaTime;
    Dir = VectorTransform(VecSet(0, 0, 1), MatrRotateY(Uni->Head));
    Uni->V += -3 * 30 * Ani->JY * Ani->DeltaTime;
    Uni->V *= max(1 - Ani->GlobalDeltaTime, 0);
    Uni->Pos = VecAddVec(Uni->Pos, VecMulNum(Dir, Uni->V * Ani->DeltaTime));

    Uni->Pos = VecAddVec(Uni->Pos, VecMulNum(VectorTransform(Dir, MatrRotateY(-90)), 30 * Ani->JX * Ani->DeltaTime));


    Uni->Pos.Y += 100 * (Ani->JButs[1] - Ani->JButs[2]) * Ani->DeltaTime;

    At = VecSubVec(Uni->Pos, VecMulNum(Dir, 12));
    At.Y += 6.30;

    Move = VecSubVec(At, Ani->RndCamera.Loc);
    Ani->RndCamera.Loc = VecAddVec(Ani->RndCamera.Loc, VecMulNum(Move, Ani->GlobalDeltaTime));

    Ani->RndCamera.At = Uni->Pos;

    PD6_RndMatrView = MatrView(Ani->RndCamera.Loc, Uni->Pos, VecSet(0, 1, 0));
/*    PD6_GeomHelicDraw(&Uni->Geom, Uni->Pos);*/
/*    Uni->Head = 300 * (Ani->JZ + 0.00002) * Ani->GlobalDeltaTime;
    Uni->V += Ani->JY * Ani->GlobalDeltaTime;
    Uni->V *= 1 - Ani->GlobalDeltaTime;
    Dir = VectorTransform(VecSet(0, 0, 1), MatrRotateY(Uni->Head));
    At = VecAddVec(Uni->Pos, VecMulNum(Dir, 1));
    At.Y += 3;
    At.Z -= 9;
    Move = VecSubVec(At, Ani->RndCamera.Loc);
    Uni->Pos = VecAddVec(Uni->Pos, VecMulNum(Dir, Uni->V * Ani->GlobalDeltaTime));
    Ani->RndCamera.Loc = VecAddVec(Ani->RndCamera.Loc, VecMulNum(Move, Ani->GlobalDeltaTime));

    PD6_RndMatrView = MatrView(Ani->RndCamera.Loc, Uni->Pos, VecSet(0, 1, 0));      */
  }

} /* End of 'PD6_AnimUnitResponse' function */

/* ������� ���������� ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       pd6UNIT_CTRL *Uni;
 *   - ��������� �� �������� ��������:
 *       pd6ANIM *Ani;
 * ������������ ��������: ���.
 */
static VOID PD6_AnimUnitRender( pd6UNIT_CTRL *Uni, pd6ANIM *Ani )
{
  HFONT hFntOld = SelectObject(Ani->hDC, Uni->hFnt);
  static DBL count = 30;
  static CHAR Buf[1000];

  count += Ani->GlobalDeltaTime;
  if (count > 1)
  {
    count = 0;
    sprintf(Buf, "FPS: %.3f", Ani->FPS);
    SetWindowText(Ani->hWnd, Buf);
  }

  PD6_RndMatrWorld = MatrMulMatr(
                        MatrRotateZ(180),
                        MatrMulMatr(MatrRotateX(-90), MatrMulMatr(MatrRotateY(Uni->Head),
                        MatrTranslate(Uni->Pos.X, Uni->Pos.Y, Uni->Pos.Z))));
  PD6_GeomHelicDraw(&Uni->Geom);
  /*
  SetTextColor(Ani->hDC, RGB(255, 255, 255));
  SetBkMode(Ani->hDC, TRANSPARENT);
  rc.left = 0;
  rc.top = 0;
  rc.right = Ani->W;
  rc.bottom = Ani->H;
  DrawText(Ani->hDC, Buf,
    sprintf(Buf,
      "FPS: %.3f\n"
      "J: %i %i %i, POV:%i\n"
      "%.5f %.5f %.5f %.5f %.5f %.5f",
      Ani->FPS,
      Ani->JButs[0], Ani->JButs[1], Ani->JButs[2], Ani->JPOV,
      Ani->JX, Ani->JY, Ani->JZ, Ani->JR, Ani->JU, Ani->JV),
    &rc,
    DT_TOP | DT_LEFT);
  SelectObject(Ani->hDC, hFntOld);
  */
} /* End of 'PD6_AnimUnitRender' function */

/* ������� �������� ������� �������� "����������".
 * ���������: ���.
 * ������������ ��������:
 *   (pd6UNIT *) ��������� �� ��������� ������ ��������.
 */
pd6UNIT * PD6_UnitControlCreate( VOID )
{
  pd6UNIT_CTRL *Uni;

  if ((Uni = (VOID *)PD6_AnimUnitCreate(sizeof(pd6UNIT_CTRL))) == NULL)
    return NULL;
  /* ��������� ���� */
  Uni->Init = (VOID *)PD6_AnimUnitInit;
  Uni->Close = (VOID *)PD6_AnimUnitClose;
  Uni->Response = (VOID *)PD6_AnimUnitResponse;
  Uni->Render = (VOID *)PD6_AnimUnitRender;
  return (pd6UNIT *)Uni;
} /* End of 'PD6_UnitBallCreate' function */

/* END OF 'UCONTRL.C' FILE */
