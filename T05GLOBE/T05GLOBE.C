/* FILE NAME: T05GLOBE.C
 * PROGRAMMER: PD6
 * DATE: 04.06.2015
 * PURPOSE: WinAPI sphere drawing.
 */

#include <time.h>
#include <string.h>
#include <math.h>
#include <stdio.h>

#include <windows.h>

#include "globe.h"
#include "image.h"

/* Имя класса окна */
#define WND_CLASS_NAME "My window class"

/* Рисовать в режиме WireFrame или нет */
BOOL IsWire = FALSE;

extern INT Radius;
BYTE *Pic;
INT PicH = 0, PicW = 0;

extern IMAGE GlobeImg;

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
 *   (INT) код возврата в операционную си стему.
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
      "Sphera",                      /* Заголовок окна */
      WS_OVERLAPPEDWINDOW,          /* Стили окна - окно общего вида */
      CW_USEDEFAULT, CW_USEDEFAULT, /* Позиция окна (x, y) - по умолчанию */
      CW_USEDEFAULT, CW_USEDEFAULT, /* Размеры окна (w, h) - по умолчанию */
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

    SetWindowPos(hWnd, HWND_TOP, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top,
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
  static HDC hMemDC;
  static INT w, h;
  static BITMAP bm;
  static HBITMAP hBm;
  //FILE *F;

  switch (Msg)
  {
  case WM_CREATE:
    SetTimer(hWnd, 111, 50, NULL);

    /* создаем контекст в памяти */
    hDC = GetDC(hWnd);
    hMemDC = CreateCompatibleDC(hDC);
    ReleaseDC(hWnd, hDC);

/*    F = fopen("VINNYPUH.g24", "rb");
    fread(&PicW, 2, 1, F);
    fread(&PicH, 2, 1, F);
    Pic = malloc(PicW * PicH * 3);
    fread(Pic, 3, PicW * PicH, F);
    fclose(F);*/

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

    SelectObject(hMemDC, GetStockObject(NULL_PEN));
    SelectObject(hMemDC, GetStockObject(DC_BRUSH));
    SetDCBrushColor(hMemDC, RGB(255, 0, 0));
    GlobeBuild();
    GlobeDraw(hMemDC, w, h);

    InvalidateRect(hWnd, NULL, TRUE);
    return 0;

  case WM_KEYDOWN:
    if (wParam == 'F')
      FlipFullScreen(hWnd);
    if (wParam == 27)
      SendMessage(hWnd, WM_CLOSE, 0, 0);
    if (wParam == 'W')
      IsWire = !IsWire;
    if (wParam == VK_UP)
      Radius += 19;
    if (wParam == VK_DOWN)
      Radius -= 19;
    return 0;

  case WM_CLOSE:
    if (MessageBox(hWnd, "Are you sure to exit from program?",
          "Exit", MB_YESNO | MB_ICONQUESTION) == IDNO)
      return 0;
    break;

  case WM_ERASEBKGND:
    BitBlt((HDC)wParam, 0, 0, w, h, hMemDC, 0, 0, SRCCOPY);
    return 0;

  case WM_DESTROY:
    DeleteDC(hMemDC);
    DeleteObject(hBm);
    ImageFree(&GlobeImg);
    KillTimer(hWnd, 111);
    PostQuitMessage(0);
    return 0;
  }
  return DefWindowProc(hWnd, Msg, wParam, lParam);
} /* End of 'MyWindowFunc' function */

/* END OF 'T05GLOBE.C' FILE */