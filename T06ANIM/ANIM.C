/* FILENAME: ANIM.C
 * PROGRAMMER: PD6
 * PURPOSE: Animation system module.
 * LAST UPDATE: 08.06.2015
 */

#include "anim.h"
#include <stdio.h>

/* Данные для синхронизации по времени */
static INT64
  TimeFreq,  /* единиц измерения в секунду */
  TimeStart, /* время начала анимации */
  TimeOld,   /* время прошлого кадра */
  TimePause, /* время простоя в паузе */
  TimeFPS;   /* время для замера FPS */
static INT
  FrameCounter; /* счетчик кадров */

/* Системный контекст анимации */
static pd6ANIM PD6_Anim;

/* Функция инициализации анимации.
 * АРГУМЕНТЫ:
 *   - дескриптор окна:
 *       HWND hWnd;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID PD6_AnimInit( HWND hWnd )
{
  HDC hDC = GetDC(hWnd);
  LARGE_INTEGER li;

  PD6_Anim.hWnd = hWnd;
  /* Инициализируем буфер кадра */
  PD6_Anim.hDC = CreateCompatibleDC(hDC);
  PD6_Anim.hBmFrame = CreateCompatibleBitmap(hDC, 30, 30);
  SelectObject(PD6_Anim.hDC, PD6_Anim.hBmFrame);
  PD6_Anim.W = 30;
  PD6_Anim.H = 30;
  PD6_Anim.NumOfUnits = 0;
  ReleaseDC(hWnd, hDC);

  /* Инициализация таймера */
  QueryPerformanceFrequency(&li);
  TimeFreq = li.QuadPart;
  QueryPerformanceCounter(&li);
  TimeStart = TimeOld = TimeFPS = li.QuadPart;
  PD6_Anim.IsPause = FALSE;
  FrameCounter = 0;
} /* End of 'PD6_AnimInit' function */

/* Функция деинициализации анимации.
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID PD6_AnimClose( VOID )
{
  INT i;

  /* Освобождение памяти из-под объектов анимации */
  for (i = 0; i < PD6_Anim.NumOfUnits; i++)
  {
    PD6_Anim.Units[i]->Close(PD6_Anim.Units[i], &PD6_Anim);
    free(PD6_Anim.Units[i]);
    PD6_Anim.Units[i] = NULL;
  }

  /* Удаляем объекты GDI */
  DeleteObject(PD6_Anim.hBmFrame);
  DeleteDC(PD6_Anim.hDC);
} /* End of 'PD6_AnimClose' function */

/* Функция обработки изменения размера области вывода.
 * АРГУМЕНТЫ:
 *   - новый размер области вывода:
 *       INT W, H;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID PD6_AnimResize( INT W, INT H )
{
  HDC hDC = GetDC(PD6_Anim.hWnd);

  /* Перевыделение памяти у буфера кадра */
  DeleteObject(PD6_Anim.hBmFrame);
  PD6_Anim.hBmFrame = CreateCompatibleBitmap(hDC, W, H);
  SelectObject(PD6_Anim.hDC, PD6_Anim.hBmFrame);

  /* Сохранение размера */
  PD6_Anim.W = W;
  PD6_Anim.H = H;

  ReleaseDC(PD6_Anim.hWnd, hDC);
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
  HFONT hFnt, hOldFnt;

  /* очистка фона */
  SelectObject(PD6_Anim.hDC, GetStockObject(DC_BRUSH));
  SelectObject(PD6_Anim.hDC, GetStockObject(NULL_PEN));
  SetDCBrushColor(PD6_Anim.hDC, RGB(0, 0, 0));
  Rectangle(PD6_Anim.hDC, 0, 0, PD6_Anim.W + 1, PD6_Anim.H + 1);

  /* Writing the time and the date at the moment */
  hFnt = CreateFont(60, 0, 0, 0, FW_BOLD, FALSE, FALSE,
    FALSE, RUSSIAN_CHARSET, OUT_DEFAULT_PRECIS,
    CLIP_DEFAULT_PRECIS, PROOF_QUALITY,
    VARIABLE_PITCH | FF_ROMAN, "");
  hOldFnt = SelectObject(PD6_Anim.hDC, hFnt);

  SetTextColor(PD6_Anim.hDC, RGB(59, 255, 230));
  SetBkColor(PD6_Anim.hDC, RGB(255, 255, 0));
  SetBkMode(PD6_Anim.hDC, TRANSPARENT);

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

  DeleteObject(hFnt);

  SelectObject(PD6_Anim.hDC, hOldFnt);
  DeleteObject(hFnt);

  /* опрос на изменение состояний объектов */
  for (i = 0; i < PD6_Anim.NumOfUnits; i++)
    PD6_Anim.Units[i]->Response(PD6_Anim.Units[i], &PD6_Anim);

  /* рисование объектов */
  for (i = 0; i < PD6_Anim.NumOfUnits; i++)
  {
    SelectObject(PD6_Anim.hDC, GetStockObject(DC_BRUSH));
    SelectObject(PD6_Anim.hDC, GetStockObject(DC_PEN));
    SetDCBrushColor(PD6_Anim.hDC, RGB(255, 255, 255));
    SetDCPenColor(PD6_Anim.hDC, RGB(0, 0, 0));

    PD6_Anim.Units[i]->Render(PD6_Anim.Units[i], &PD6_Anim);
  }

  TextOut(PD6_Anim.hDC, PD6_Anim.W / 30, PD6_Anim.H / 30, Buf,
      sprintf(Buf, "FPS: %.5f", PD6_Anim.FPS));

  FrameCounter++;
} /* End of 'PD6_AnimRender' function */

/* Функция вывода кадра анимации.
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID PD6_AnimCopyFrame( VOID )
{
  HDC hDC;

  /* вывод кадра */
  hDC = GetDC(PD6_Anim.hWnd);
  BitBlt(hDC, 0, 0, PD6_Anim.W, PD6_Anim.H, PD6_Anim.hDC, 0, 0, SRCCOPY);
  ReleaseDC(PD6_Anim.hWnd, hDC);
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
