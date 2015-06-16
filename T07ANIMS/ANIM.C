/* FILENAME: ANIM.C
 * PROGRAMMER: PD6
 * PURPOSE: Animation system module.
 * LAST UPDATE: 08.06.2015
 */

#include "anim.h"
#include "render.h"
#include <stdio.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm")

/* Получение значения оси джойстика */
#define PD6_GET_AXIS_VALUE(Axis) \
  (2.0 * (ji.dw ## Axis ## pos - jc.w ## Axis ## min) / (jc.w ## Axis ## max - jc.w ## Axis ## min) - 1.0)

/* Данные для синхронизации по времени */
static INT64
  TimeFreq,  /* единиц измерения в секунду */
  TimeStart, /* время начала анимации */
  TimeOld,   /* время прошлого кадра */
  TimePause, /* время простоя в паузе */
  TimeFPS;   /* время для замера FPS */
static INT
  FrameCounter; /* счетчик кадров */

/* Сохраненные мышиные координаты */
static INT
  PD6_MouseOldX, PD6_MouseOldY;

/* Системный контекст анимации */
pd6ANIM PD6_Anim;

/* Функция инициализации анимации.
 * АРГУМЕНТЫ:
 *   - дескриптор окна:
 *       HWND hWnd;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
BOOL PD6_AnimInit( HWND hWnd )
{
  INT i;
  LARGE_INTEGER li;
  POINT pt;
  PIXELFORMATDESCRIPTOR pfd = {0};

  memset(&PD6_Anim, 0, sizeof(pd6ANIM));
  PD6_Anim.hWnd = hWnd;
  PD6_Anim.hDC = GetDC(hWnd);

  PD6_Anim.W = 30;
  PD6_Anim.H = 30;
  PD6_Anim.NumOfUnits = 0;

  pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
  pfd.nVersion = 1;
  pfd.dwFlags = PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
  pfd.iPixelType = PFD_TYPE_RGBA;
  pfd.cColorBits = 32;
  pfd.cDepthBits = 32;
  i = ChoosePixelFormat(PD6_Anim.hDC, &pfd);
  DescribePixelFormat(PD6_Anim.hDC, i, sizeof(PIXELFORMATDESCRIPTOR), &pfd);
  SetPixelFormat(PD6_Anim.hDC, i, &pfd);

  PD6_Anim.hGLRC = wglCreateContext(PD6_Anim.hDC);
  wglMakeCurrent(PD6_Anim.hDC, PD6_Anim.hGLRC);

  if (glewInit() != GLEW_OK ||
      !(GLEW_ARB_vertex_shader && GLEW_ARB_fragment_shader))
  {
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(PD6_Anim.hGLRC);
    ReleaseDC(PD6_Anim.hWnd, PD6_Anim.hDC);
    memset(&PD6_Anim, 0, sizeof(pd6ANIM));
    return FALSE;
  }

  PD6_RndProg = PD6_ShaderLoad("TEST");

  QueryPerformanceFrequency(&li);
  TimeFreq = li.QuadPart;
  QueryPerformanceCounter(&li);
  TimeStart = TimeOld = TimeFPS = li.QuadPart;
  PD6_Anim.IsPause = FALSE;
  FrameCounter = 0;

  GetCursorPos(&pt);
  ScreenToClient(PD6_Anim.hWnd, &pt);
  PD6_MouseOldX = pt.x;
  PD6_MouseOldY = pt.y;
  GetKeyboardState(PD6_Anim.KeysOld);

  PD6_RndCameraSet(&PD6_Anim.RndCamera, VecSet(60, 60, 60),
                   VecSet(0, 0, 0),
                   VecSet(0, 1, 0));

  return TRUE;
} /* End of 'PD6_AnimInit' function */

/* Функция деинициализации анимации.
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID PD6_AnimClose( VOID )
{
  INT i;

  for (i = 0; i < PD6_Anim.NumOfUnits; i++)
  {
    PD6_Anim.Units[i]->Close(PD6_Anim.Units[i], &PD6_Anim);
    free(PD6_Anim.Units[i]);
    PD6_Anim.Units[i] = NULL;
  }

  PD6_ShaderFree(PD6_RndProg);
  PD6_RndProg = 0;
  wglMakeCurrent(NULL, NULL);
  wglDeleteContext(PD6_Anim.hGLRC);
  ReleaseDC(PD6_Anim.hWnd, PD6_Anim.hDC);
} /* End of 'PD6_AnimClose' function */

/* Функция обработки изменения размера области вывода.
 * АРГУМЕНТЫ:
 *   - новый размер области вывода:
 *       INT W, H;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID PD6_AnimResize( INT W, INT H )
{
  /* Сохранение размера */
  PD6_Anim.W = W;
  PD6_Anim.H = H;

  glViewport(0, 0, W, H);

  if (W > H)
    PD6_RndWp = (DBL)W / H * 3, PD6_RndHp = 3;
  else
    PD6_RndHp = (DBL)H / W * 3, PD6_RndWp = 3;

  PD6_RndMatrProj = MatrFrustum(-PD6_RndWp / 2, PD6_RndWp / 2, -PD6_RndHp / 2, PD6_RndHp / 2, PD6_RndProjDist * 2, 3000);
} /* End of 'PD6_AnimResize' function */

/* Функция построения кадра анимации.
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID PD6_AnimRender( VOID )
{
  INT i;
  LARGE_INTEGER li;
  static CHAR Buf[100];
  POINT pt;

  /*** Обновление таймера ***/
  QueryPerformanceCounter(&li);
  PD6_Anim.GlobalTime = (DBL)(li.QuadPart - TimeStart) / TimeFreq;
  PD6_Anim.GlobalDeltaTime = (DBL)(li.QuadPart - TimeOld) / TimeFreq;

  if (!PD6_Anim.IsPause)
    PD6_Anim.DeltaTime = PD6_Anim.GlobalDeltaTime;
  else
  {
    TimePause += li.QuadPart - TimeOld;
    PD6_Anim.DeltaTime = 0;
  }

  PD6_Anim.Time = (DBL)(li.QuadPart - TimePause - TimeStart) / TimeFreq;

  /* вычисляем FPS */
  if (li.QuadPart - TimeFPS > TimeFreq)
  {
    PD6_Anim.FPS = FrameCounter / ((DBL)(li.QuadPart - TimeFPS) / TimeFreq);
    TimeFPS = li.QuadPart;
    FrameCounter = 0;
  }

  /* время "прошлого" кадра */
  TimeOld = li.QuadPart;


  /*** Обновление ввода ***/

  /* Клавиатура */
  GetKeyboardState(PD6_Anim.Keys);
  for (i = 0; i < 256; i++)
    PD6_Anim.Keys[i] >>= 7;
  for (i = 0; i < 256; i++)
    PD6_Anim.KeysClick[i] = PD6_Anim.Keys[i] && !PD6_Anim.KeysOld[i];
  memcpy(PD6_Anim.KeysOld, PD6_Anim.Keys, sizeof(PD6_Anim.KeysOld));

  /* Мышь */
  /* колесо */
  PD6_Anim.MsWheel = PD6_MouseWheel;
  PD6_MouseWheel = 0;
  /* абсолютная позиция */
  GetCursorPos(&pt);
  ScreenToClient(PD6_Anim.hWnd, &pt);
  PD6_Anim.MsX = pt.x;
  PD6_Anim.MsY = pt.y;
  /* относительное перемещение */
  PD6_Anim.MsDeltaX = pt.x - PD6_MouseOldX;
  PD6_Anim.MsDeltaY = pt.y - PD6_MouseOldY;
  PD6_MouseOldX = pt.x;
  PD6_MouseOldY = pt.y;

  /* Джойстик */
  if ((i = joyGetNumDevs()) > 0)
  {
    JOYCAPS jc;

    /* получение общей информации о джостике */
    if (joyGetDevCaps(JOYSTICKID1, &jc, sizeof(jc)) == JOYERR_NOERROR)
    {
      JOYINFOEX ji;

      /* получение текущего состояния */
      ji.dwSize = sizeof(JOYCAPS);
      ji.dwFlags = JOY_RETURNALL;
      if (joyGetPosEx(JOYSTICKID1, &ji) == JOYERR_NOERROR)
      {
        /* Кнопки */
        memcpy(PD6_Anim.JButsOld, PD6_Anim.JButs, sizeof(PD6_Anim.JButs));
        for (i = 0; i < 32; i++)
          PD6_Anim.JButs[i] = (ji.dwButtons >> i) & 1;
        for (i = 0; i < 32; i++)
          PD6_Anim.JButsClick[i] = PD6_Anim.JButs[i] && !PD6_Anim.JButsOld[i];

        /* Оси */
        PD6_Anim.JX = PD6_GET_AXIS_VALUE(X);
        PD6_Anim.JY = PD6_GET_AXIS_VALUE(Y);
        if (jc.wCaps & JOYCAPS_HASZ)
          PD6_Anim.JZ = PD6_GET_AXIS_VALUE(Z);
        if (jc.wCaps & JOYCAPS_HASU)
          PD6_Anim.JU = PD6_GET_AXIS_VALUE(U);
        if (jc.wCaps & JOYCAPS_HASV)
          PD6_Anim.JV = PD6_GET_AXIS_VALUE(V);
        if (jc.wCaps & JOYCAPS_HASR)
          PD6_Anim.JR = PD6_GET_AXIS_VALUE(R);

        if (jc.wCaps & JOYCAPS_HASPOV)
        {
          if (ji.dwPOV == 0xFFFF)
            PD6_Anim.JPOV = 0;
          else
            PD6_Anim.JPOV = ji.dwPOV / 4500 + 1;
        }
      }
    }
  }

  /* опрос на изменение состояний объектов */
  for (i = 0; i < PD6_Anim.NumOfUnits; i++)
    PD6_Anim.Units[i]->Response(PD6_Anim.Units[i], &PD6_Anim);

  glClearColor(0.3, 0.5, 0.7, 1);
  glClearDepth(1);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glColorMask(TRUE, TRUE, TRUE, FALSE);

  /*
  glColor3d(0.3, 0.5, 0.7);
  glRectd(-2, -2, 2, 2);
  glColor3d(1, 0, 0);
  glRectd(0, 0, 0.88, 0.30);
  */

  for (i = 0; i < PD6_Anim.NumOfUnits; i++)
  {
    static DBL time = 3;

    time += PD6_Anim.GlobalDeltaTime;
    if (time > 3)
    {
      time = 0;
      PD6_ShaderFree(PD6_RndProg);
      PD6_RndProg = PD6_ShaderLoad("TEST");
    }

    glActiveTexture(GL_TEXTURE0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    PD6_RndMatrWorld = MatrIdentity();

    PD6_Anim.Units[i]->Render(PD6_Anim.Units[i], &PD6_Anim);
  }

  glFinish();
  FrameCounter++;
} /* End of 'PD6_AnimRender' function */

/* Функция вывода кадра анимации.
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID PD6_AnimCopyFrame( VOID )
{
  SwapBuffers(PD6_Anim.hDC);
} /* End of 'PD6_AnimCopyFrame' function */

/* Функция добавления в систему объекта анимации.
 * АРГУМЕНТЫ:
 *   - добавляемый объект анимации:
 *       pd6UNIT *Unit;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID PD6_AnimAddUnit( pd6UNIT *Unit )
{
  if (PD6_Anim.NumOfUnits < PD6_MAX_UNITS)
  {
    PD6_Anim.Units[PD6_Anim.NumOfUnits++] = Unit;
    Unit->Init(Unit, &PD6_Anim);
  }
} /* End of 'PD6_AnimAddUnit' function */


/* Функция переключения в/из полноэкранного режима
 * с учетом нескольких мониторов.
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID PD6_AnimFlipFullScreen( VOID )
{
  static BOOL IsFullScreen = FALSE; /* текущий режим */
  static RECT SaveRC;               /* сохраненный размер */

  if (!IsFullScreen)
  {
    RECT rc;
    HMONITOR hmon;
    MONITORINFOEX moninfo;

    /* сохраняем старый размер окна */
    GetWindowRect(PD6_Anim.hWnd, &SaveRC);

    /* определяем в каком мониторе находится окно */
    hmon = MonitorFromWindow(PD6_Anim.hWnd, MONITOR_DEFAULTTONEAREST);

    /* получаем информацию для монитора */
    moninfo.cbSize = sizeof(moninfo);
    GetMonitorInfo(hmon, (MONITORINFO *)&moninfo);

    /* переходим в полный экран */
    rc = moninfo.rcMonitor;

    AdjustWindowRect(&rc, GetWindowLong(PD6_Anim.hWnd, GWL_STYLE), FALSE);

    SetWindowPos(PD6_Anim.hWnd, HWND_TOPMOST,
      rc.left, rc.top,
      rc.right - rc.left, rc.bottom - rc.top,
      SWP_NOOWNERZORDER);
    IsFullScreen = TRUE;
  }
  else
  {
    /* восстанавливаем размер окна */
    SetWindowPos(PD6_Anim.hWnd, HWND_TOP,
      SaveRC.left, SaveRC.top,
      SaveRC.right - SaveRC.left, SaveRC.bottom - SaveRC.top,
      SWP_NOOWNERZORDER);
    IsFullScreen = FALSE;
  }
} /* End of 'PD6_AnimFlipFullScreen' function */

/* Функция выхода из анимации.
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID PD6_AnimDoExit( VOID )
{
  PostMessage(PD6_Anim.hWnd, WM_CLOSE, 0, 0);
} /* End of 'PD6_AnimDoExit' function */

/* Функция установки паузы анимации.
 * АРГУМЕНТЫ:
 *   - флаг паузы:
 *       BOOL NewPauseFlag;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID PD6_AnimSetPause( BOOL NewPauseFlag )
{
  PD6_Anim.IsPause = NewPauseFlag;
} /* End of 'PD6_AnimSetPause' function */

/* END OF 'ANIM.C' FILE */
