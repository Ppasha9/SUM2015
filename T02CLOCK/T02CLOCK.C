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

/* ��� ������ ���� */
#define WND_CLASS_NAME "My window class"

#define PD6_PI 3.141592653589793238462643383278502884197169399375105820974944

/* ������ ������ */
LRESULT CALLBACK MyWindowFunc( HWND hWnd, UINT Msg,
                               WPARAM wParam, LPARAM lParam );

/* ������� ������� ���������.
 *   - ���������� ���������� ����������:
 *       HINSTANCE hInstance;
 *   - ���������� ����������� ���������� ����������
 *     (�� ������������ � ������ ���� NULL):
 *       HINSTANCE hPrevInstance;
 *   - ��������� ������:
 *       CHAR *CmdLine;
 * ������������ ��������:
 *   (INT) ��� �������� � ������������ �� �����.
 *   0 - ��� ������.
 */
INT WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    CHAR *CmdLine, INT ShowCmd )
{
  WNDCLASS wc;
  HWND hWnd;
  MSG msg;

  /* ����������� ������ ���� */
  wc.style = CS_VREDRAW | CS_HREDRAW;                   /* ����� ����: ��������� ��������������
                                                             * ��� ��������� ������������� ���
                                                             * ��������������� ��������
                                                             * ��� ����� CS_DBLCLKS ��� ����������
                                                             * ��������� �������� ������� */
  wc.cbClsExtra = 0;                                    /* �������������� ���������� ���� ��� ������ */
  wc.cbWndExtra = 0;                                    /* �������������� ���������� ���� ��� ���� */
  wc.hbrBackground = CreateSolidBrush(RGB(255, 255, 0));
  wc.hCursor = LoadCursor(NULL, IDC_ARROW);             /* �������� ������� (����������) */
  wc.hIcon = LoadIcon(NULL, IDI_ASTERISK);              /* �������� ����������� (���������) */
  wc.hInstance = hInstance;                             /* ���������� ����������, ��������������� ����� */
  wc.lpszMenuName = NULL;                               /* ��� ������� ���� */
  wc.lpfnWndProc = MyWindowFunc;                        /* ��������� �� ������� ��������� */
  wc.lpszClassName = WND_CLASS_NAME;

  /* ����������� ������ � ������� */
  if (!RegisterClass(&wc))
  {
    MessageBox(NULL, "Error register window class", "ERROR", MB_OK);
    return 0;
  }

  /* �������� ���� */
  hWnd =
    CreateWindow(WND_CLASS_NAME,    /* ��� ������ ���� */
      "Title",                      /* ��������� ���� */
      WS_OVERLAPPEDWINDOW,          /* ����� ���� - ���� ������ ���� */
      CW_USEDEFAULT, CW_USEDEFAULT, /* ������� ���� (x, y) - �� ��������� */
      1920, 1080,                     /* ������� ���� (w, h) - �� ��������� */
      NULL,                         /* ���������� ������������� ���� */
      NULL,                         /* ���������� ������������ ���� */
      hInstance,                    /* ���������� ���������� */
      NULL);                        /* ��������� �� �������������� ��������� */

  ShowWindow(hWnd, ShowCmd);
  UpdateWindow(hWnd);

  /* ������ ����� ��������� ���� */
  while (GetMessage(&msg, NULL, 0, 0))
    /* �������� ��������� � ������� ���� */
    DispatchMessage(&msg);

  return 30;
} /* End of 'WinMain' function */


/* ������� ������������ �/�� �������������� ������
 * � ������ ���������� ���������.
 * ���������: 
 *   - ���������� ����:
 *       HWND hWnd;
 * ������������ ��������: ���.
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

    /* ��������� � ������ ����� */
    /* ��� ������ ��������:
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


/* ������� ��������� ������� �������.
 * ���������:
 *   - ���������� ��������� ����:
 *       HDC hDC;
 *   - �������� �����, �� ������� �������� �������:
 *       INT X1
 *   - �������� �����, �� ������� �������� �������:
 *       INT Y1
 *   - ����� �������:
 *       INT Len;
 *   - ����, �� ������� �������������� ��������:
 *       DOUBLE Angle;
 * ����������� ��������: ���.
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

/* ������� ��������� ��������� ����.
 * ���������:
 *   - ���������� ����:
 *       HWND hWnd;
 *   - ����� ��������� (��. WM_***):
 *       UINT Msg;
 *   - �������� ��������� ('word parameter'):
 *       WPARAM wParam;
 *   - �������� ��������� ('long parameter'):
 *       LPARAM lParam;
 * ������������ ��������:
 *   (LRESULT) - � ����������� �� ���������.
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

    /* ������� �������� � ������ */
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

    /* ������� �������� �������� � ���� */
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
      sprintf(Buf, "�����: %02d:%02d:%02d",
        st.wHour - 1, st.wMinute, st.wSecond));
    TextOut(hMemDC, w / 2 - 210, h / 2 + 450, Buf,
      sprintf(Buf, "����: %02d.%02d.%d",
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