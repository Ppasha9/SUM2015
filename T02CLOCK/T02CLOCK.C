/* FILE NAME: T02CLOCK.C
 * PROGRAMMER: PD6
 * DATE: 02.06.2015
 * PURPOSE: WinAPI clock visualization.
 */

#pragma warning(disable: 4244)

#include <time.h>
#include <string.h>
#include <math.h>
#include <stdio.h>

#include <windows.h>

/* Имя класса окна */
#define WND_CLASS_NAME "My window class"

#define PD6_PI 3.141592653589793238462643383278502884197169399375105820974944

/* Ссылка вперед */
LRESULT CALLBACK MyWindowFunc( HWND hWnd, UINT Msg,
                               WPARAM wParam, LPARAM lParam );

/* Главная функция программы.
 *   - дескриптор экземпляра приложения:
 *       HINSTANCE hInstance;
 *   - дескриптор предыдущего экземпляра приложения
 *     (не используется и должно быть NULL):
 *       HINSTANCE hPrevInstance;
 *   - командная строка:
 *       CHAR *CmdLine;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (INT) код возврата в операционную систему.
 *   0 - при успехе.
 */
INT WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    CHAR *CmdLine, INT ShowCmd )
{
  WNDCLASS wc;
  HWND hWnd;
  MSG msg;

  /* Регистрация класса окна */
  wc.style = CS_VREDRAW | CS_HREDRAW;                   /* Стиль окна: полностью перерисовывать
                                                             * при изменении вертикального или
                                                             * горизонтального размеров
                                                             * еще можно CS_DBLCLKS для добавления
                                                             * отработки двойного нажатия */
  wc.cbClsExtra = 0;                                    /* Дополнительное количество байт для класса */
  wc.cbWndExtra = 0;                                    /* Дополнительное количество байт для окна */
  wc.hbrBackground = CreateSolidBrush(RGB(255, 255, 0));
  wc.hCursor = LoadCursor(NULL, IDC_ARROW);             /* Загрузка курсора (системного) */
  wc.hIcon = LoadIcon(NULL, IDI_ASTERISK);              /* Загрузка пиктограммы (системной) */
  wc.hInstance = hInstance;                             /* Дескриптор приложения, регистрирующего класс */
  wc.lpszMenuName = NULL;                               /* Имя ресурса меню */
  wc.lpfnWndProc = MyWindowFunc;                        /* Указатель на функцию обработки */
  wc.lpszClassName = WND_CLASS_NAME;

  /* Регистрация класса в системе */
  if (!RegisterClass(&wc))
  {
    MessageBox(NULL, "Error register window class", "ERROR", MB_OK);
    return 0;
  }

  /* Создание окна */
  hWnd =
    CreateWindow(WND_CLASS_NAME,    /* Имя класса окна */
      "Title",                      /* Заголовок окна */
      WS_OVERLAPPEDWINDOW,          /* Стили окна - окно общего вида */
      CW_USEDEFAULT, CW_USEDEFAULT, /* Позиция окна (x, y) - по умолчанию */
      500, 500,                     /* Размеры окна (w, h) - по умолчанию */
      NULL,                         /* Дескриптор родительского окна */
      NULL,                         /* Дескриптор загруженного меню */
      hInstance,                    /* Дескриптор приложения */
      NULL);                        /* Указатель на дополнительные параметры */

  ShowWindow(hWnd, ShowCmd);
  UpdateWindow(hWnd);

  /* Запуск цикла сообщений окна */
  while (GetMessage(&msg, NULL, 0, 0))
    /* Передача сообщений в функцию окна */
    DispatchMessage(&msg);

  return 30;
} /* End of 'WinMain' function */


/* Функция переключения в/из полноэкранного режима
 * с учетом нескольких мониторов.
 * АРГУМЕНТЫ: 
 *   - дескриптор окна:
 *       HWND hWnd;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID FlipFullScreen( HWND hWnd )
{
  static BOOL IsFullScreen = FALSE;
  static RECT SaveRC;

  if (!IsFullScreen)
  {
    RECT rc;
    HMONITOR hmon;
    MONITORINFOEX moninfo;

    /* Save old size of window */
    GetWindowRect(hWnd, &SaveRC);

    hmon = MonitorFromWindow(hWnd, MONITOR_DEFAULTTONEAREST);

    moninfo.cbSize = sizeof(moninfo);
    GetMonitorInfo(hmon, (MONITORINFO *)&moninfo);

    /* переходим в полный экран */
    /* для одного монитора:
    rc.left = 0;
    rc.top = 0;
    rc.right = GetSystemMetrics(SM_CXSCREEN);
    rc.bottom = GetSystemMetrics(SM_CXSCREEN);
    */

    rc = moninfo.rcMonitor;

    AdjustWindowRect(&rc, GetWindowLong(hWnd, GWL_STYLE), FALSE);

    SetWindowPos(hWnd, HWND_TOPMOST, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top + 201,
      SWP_NOOWNERZORDER);
    IsFullScreen = TRUE;
  }
  else
  {
    SetWindowPos(hWnd, HWND_TOPMOST, SaveRC.left, SaveRC.top,
      SaveRC.right - SaveRC.left, SaveRC.bottom - SaveRC.top,
      SWP_NOOWNERZORDER);
    IsFullScreen = FALSE;
  }
} /* End of 'FlipFullScreen' function */


/* Функция рисования часовых стрелок.
 * АРГУМЕНТЫ:
 *   - дескриптор контекста окна:
 *       HDC hDC;
 *   - абсцисса точки, из которой рисуется стрелка:
 *       INT X1
 *   - ордината точки, из которой рисуется стрелка:
 *       INT Y1
 *   - длина стрелки:
 *       INT Len;
 *   - угол, на который поворачивается сттрелка:
 *       DOUBLE Angle;
 * ВОЗРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID DrawArrow( HDC hDC, INT X1, INT Y1, INT Len, DOUBLE Angle )
{
  DOUBLE
    si = sin(PD6_PI * Angle / 180),
    co = cos(PD6_PI * Angle / 180);

  MoveToEx(hDC, X1, Y1, NULL);
  LineTo(hDC, X1 + si * Len, Y1 - co * Len);
} /* End of 'DrawArrow' function */


VOID DrawHandPoly( HDC hDC, INT Xc, INT Yc, INT L, INT W, FLOAT Angle )
{
  INT i;
  POINT pnts[] =
  {
    {0, W}, {-W, 0}, {0, L}, {W, 0}
  }, pntdraw[sizeof pnts / sizeof pnts[0]];
  FLOAT si = sin(Angle), co = cos(Angle);

  for (i = 0; i < sizeof pnts / sizeof pnts[0]; i++)
  {
    pntdraw[i].x = Xc + pnts[i].x * co - pnts[i].y * si;
    pntdraw[i].y = Yc + pnts[i].y * co + pnts[i].x * si;
  }

  Polygon(hDC, pntdraw, sizeof pnts / sizeof pnts[0]);
}

/* Функция обработки сообщения окна.
 * АРГУМЕНТЫ:
 *   - дескриптор окна:
 *       HWND hWnd;
 *   - номер сообщения (см. WM_***):
 *       UINT Msg;
 *   - параметр сообшения ('word parameter'):
 *       WPARAM wParam;
 *   - параметр сообшения ('long parameter'):
 *       LPARAM lParam;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (LRESULT) - в зависимости от сообщения.
 */
LRESULT CALLBACK MyWindowFunc( HWND hWnd, UINT Msg,
                               WPARAM wParam, LPARAM lParam )
{
  HDC hDC;
  INT RS = 300, RM = 270, RH = 150;
  CREATESTRUCT *cs;
  SYSTEMTIME st, TimeLine;
  CHAR Buf[100];
  HFONT hFnt, hOldFnt;
  HPEN hPen;
  static BITMAP bm;
  static HBITMAP hBm, hBmLogo;
  static HDC hMemDC, hMemDCLogo;
  static INT w, h;

  switch (Msg)
  {
  case WM_CREATE:
    cs = (CREATESTRUCT *)lParam;
    SetTimer(hWnd, 111, 50, NULL);

    hBmLogo = LoadImage(NULL, "ClockFace.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    GetObject(hBmLogo, sizeof(bm), &bm);

    /* создаем контекст в памяти */
    hDC = GetDC(hWnd);
    hMemDC = CreateCompatibleDC(hDC);
    hMemDCLogo = CreateCompatibleDC(hDC);
    ReleaseDC(hWnd, hDC);

    SelectObject(hMemDCLogo, hBmLogo);
    SetPixel(hMemDCLogo, 10, 10, RGB(255, 0, 0));
    return 0;

  case WM_SIZE:
    w = LOWORD(lParam);
    h = HIWORD(lParam);

    /* создаем картинку размером с окно */
    if (hBm != NULL)
      DeleteObject(hBm);

    hDC = GetDC(hWnd);
    hBm = CreateCompatibleBitmap(hDC, w, h);
    ReleaseDC(hWnd, hDC);

    SelectObject(hMemDC, hBm);
    SendMessage(hWnd, WM_TIMER, 111, 0);
    return 0;

  case WM_TIMER:
    /* Clear Background */
    SelectObject(hMemDC, GetStockObject(NULL_PEN));
    SelectObject(hMemDC, GetStockObject(DC_BRUSH));
    SetDCBrushColor(hMemDC, RGB(255, 255, 255));
    Rectangle(hMemDC, 0, 0, w + 1, h + 1);

    StretchBlt(hMemDC, w / 2 - bm.bmWidth / 2, h / 2 - bm.bmHeight / 2, bm.bmWidth, bm.bmHeight,
      hMemDCLogo, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);

    /* Draw clock's arrows */
    GetLocalTime(&TimeLine);

    hPen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
    SelectObject(hMemDC, hPen);
    DrawArrow(hMemDC, w / 2, h / 2, RS, TimeLine.wSecond * 6);
    ///DrawHandPoly(hMemDC, w / 2, h / 2, RS, 5, TimeLine.wSecond * PD6_PI / 30);
    DeleteObject(hPen);

    hPen = CreatePen(PS_SOLID, 2, RGB(0, 255, 0));
    SelectObject(hMemDC, hPen);
/*    SelectObject(hMemDC, GetStockObject(DC_BRUSH));
    SelectObject(hMemDC, GetStockObject(NULL_PEN));
    SetDCBrushColor(hMemDC, RGB(0, 255, 0));       */
    DrawArrow(hMemDC, w / 2, h / 2, RM, TimeLine.wMinute * 6);
    ///DrawHandPoly(hMemDC, w / 2, h / 2, RM, 20, TimeLine.wMinute * PD6_PI / 30);
    ///SelectObject(hMemDC, GetStockObject(NULL_BRUSH));
    DeleteObject(hPen);

    hPen = CreatePen(PS_SOLID, 2, RGB(0, 0, 255));
    SelectObject(hMemDC, hPen);
    DrawArrow(hMemDC, w / 2, h / 2, RH, ((TimeLine.wHour - 1) % 12) * 30);
    ///DrawHandPoly(hMemDC, w / 2, h / 2, -RH, 30, ((TimeLine.wHour - 1) % 12) * PD6_PI / 6);
    DeleteObject(hPen);

    /* Writing the time and the date at the moment */
    hFnt = CreateFont(60, 0, 0, 0, FW_BOLD, FALSE, FALSE,
      FALSE, RUSSIAN_CHARSET, OUT_DEFAULT_PRECIS,
      CLIP_DEFAULT_PRECIS, PROOF_QUALITY,
      VARIABLE_PITCH | FF_ROMAN, "");
    hOldFnt = SelectObject(hMemDC, hFnt);

    GetLocalTime(&st);
    SetTextColor(hMemDC, RGB(59, 255, 230));
    SetBkColor(hMemDC, RGB(255, 255, 0));
    SetBkMode(hMemDC, TRANSPARENT);

    TextOut(hMemDC, w / 2 - 210, h / 2 + 400, Buf,
      sprintf(Buf, "Время: %02d:%02d:%02d",
        st.wHour - 1, st.wMinute, st.wSecond));
    TextOut(hMemDC, w / 2 - 210, h / 2 + 450, Buf,
      sprintf(Buf, "Дата: %02d.%02d.%d",
        st.wDay, st.wMonth, st.wYear));


    DeleteObject(hFnt);

    SelectObject(hMemDC, hOldFnt);
    DeleteObject(hFnt);

    SelectObject(hMemDC, GetStockObject(DC_PEN));
    SelectObject(hMemDC, GetStockObject(DC_BRUSH));

    InvalidateRect(hWnd, NULL, TRUE);
    return 0;

  case WM_KEYDOWN:
    if (wParam == 'F')
      FlipFullScreen(hWnd);
    if (wParam == 27)
      SendMessage(hWnd, WM_CLOSE, 0, 0);
    return 0;

  case WM_CLOSE:
    if (MessageBox(hWnd, "Are you shure to exit from program?",
          "Exit", MB_YESNO | MB_ICONQUESTION) == IDNO)
      return 0;
    break;

  case WM_ERASEBKGND:
    BitBlt((HDC)wParam, 0, 0, w, h, hMemDC, 0, 0, SRCCOPY);
    return 0;

  case WM_DESTROY:
    DeleteDC(hMemDC);
    DeleteObject(hBm);
    DeleteDC(hMemDCLogo);
    DeleteObject(hBmLogo);
    KillTimer(hWnd, 111);
    PostQuitMessage(0);
    return 0;
  }
  return DefWindowProc(hWnd, Msg, wParam, lParam);
} /* End of 'MyWindowFunc' function */

/* END OF 'T02CLOCK.C' FILE */