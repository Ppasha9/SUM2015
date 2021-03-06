/* T01FWIN.C
 * ������ �������� ������ ��������� ��� WinAPI.
 * �������� � ��������� ����������� ����.
 *
 * ��������: ������� ����� 10-6
 * ���� ������: 01.06.2015
 */

#include <windows.h>
#include <math.h>

/* ��� ������ ���� */
#define WND_CLASS_NAME "My window class"

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
 *   (INT) ��� �������� � ������������ �������.
 *   0 - ��� ������.
 */
INT WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    CHAR *CmdLine, INT ShowCmd )
{
  WNDCLASS wc;
  HWND hWnd;
  MSG msg;
  /* HINSTANCE hIns = LoadLibrary("shell32"); */

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
      CW_USEDEFAULT, CW_USEDEFAULT, /* ������� ���� (w, h) - �� ��������� */
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


/* ������� ��������� �����.
 * ���������:
 *   - �������� ����:
 *       HDC hDC;
 *   - �������� ������ �������� ���� ��������������, ��������������� ����:
 *       int left;
 *   - �������� ������ �������� ���� ��������������, ��������������� ����:
 *       int top;
 *   - �������� ������� ������� ���� ��������������, ��������������� ����:
 *       int right;
 *   - �������� ������� ������� ���� ��������������, ��������������� ����:
 *       int bottom;
 * ������������ ��������:
 *   (VOID) ������
 */
void DrawEye( HDC hDC, int left, int top, int right, int bottom )
{
  SelectObject(hDC, GetStockObject(DC_BRUSH));
  SetDCBrushColor(hDC, RGB(0, 0, 0));
  Ellipse(hDC, left, top, right, bottom);
  SetDCBrushColor(hDC, RGB(255, 255, 255));
} /* End of 'DrawEye' function */


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
  HDC hDC;         /* Display context */
  static INT w, h, x, y;
  POINT pt;        /* The point of cursor */
  PAINTSTRUCT ps;
  INT CEyeF = 240, CEyeFy = 240, CEyeS = 560, CEyeSy = 240, R = 65;
  INT Rx1, Ry1, Rx2, Ry2; /* The coordinats of center */

  switch (Msg)
  {
  case WM_CREATE:
    SetTimer(hWnd, 111, 50, NULL);
    return 0;

  case WM_SIZE:
    w = LOWORD(lParam);
    h = HIWORD(lParam);
    return 0;

  case WM_TIMER:
    InvalidateRect(hWnd, NULL, FALSE);
    return 0;

  case WM_PAINT:
    hDC = BeginPaint(hWnd, &ps);

    GetCursorPos(&pt);
    ScreenToClient(hWnd, &pt);

    SelectObject(hDC, GetStockObject(DC_BRUSH));
    SetDCBrushColor(hDC, RGB(255, 255, 0));
    Rectangle(hDC, 0, 0, w, h);
    SetDCBrushColor(hDC, RGB(255, 255, 255));
/*    DrawEye(hDC, pt.x - 30, pt.y + 30, pt.x + 30, pt.y - 30);
    SelectObject(hDC, GetStockObject(NULL_BRUSH));*/

    x = pt.x;
    y = pt.y;

    Ellipse(hDC, 160, 160, 321, 321);
    Ellipse(hDC, 480, 160, 641, 321);

    Rx1 = R * abs(CEyeF - x) / (INT)sqrt((CEyeF - x) * (CEyeF - x) + (CEyeFy - y) * (CEyeFy - y));
    Ry1 = R * abs(CEyeFy - y) / (INT)sqrt((CEyeF - x) * (CEyeF - x) + (CEyeFy - y) * (CEyeFy - y));
    Rx2 = R * abs(CEyeS - x) / (INT)sqrt((CEyeS - x) * (CEyeS - x) + (CEyeSy - y) * (CEyeSy - y));
    Ry2 = R * abs(CEyeSy - y) / (INT)sqrt((CEyeS - x) * (CEyeS - x) + (CEyeSy - y) * (CEyeSy - y));

    if (x > CEyeF && x < CEyeS)
      Rx2 = -Rx2;
    if (x < CEyeF)
      Rx1 = -Rx1, Rx2 = -Rx2;
    if (y < CEyeFy)
      Ry1 = -Ry1, Ry2 = -Ry2;

    SelectObject(hDC, GetStockObject(DC_PEN));
    SelectObject(hDC, GetStockObject(DC_BRUSH));
    SetDCBrushColor(hDC, RGB(0, 0, 0));
    SetDCPenColor(hDC, RGB(0, 0, 0));
    DrawEye(hDC, CEyeF + Rx1 - 16, CEyeFy + Ry1 - 16, CEyeF + Rx1 + 16, CEyeFy + Ry1 + 16);
    DrawEye(hDC, CEyeS + Rx2 - 16, CEyeSy + Ry2 - 16, CEyeS + Rx2 + 16, CEyeSy + Ry2 + 16);
    SelectObject(hDC, GetStockObject(NULL_PEN));
    SelectObject(hDC, GetStockObject(NULL_BRUSH));

    SelectObject(hDC, GetStockObject(DC_PEN));
    SelectObject(hDC, GetStockObject(DC_BRUSH));
    SetDCBrushColor(hDC, RGB(255, 255, 254));
    SetDCPenColor(hDC, RGB(255, 255, 254));
    DrawEye(hDC, CEyeF + Rx1 - 5, CEyeFy + Ry1 - 5, CEyeF + Rx1 + 5, CEyeFy + Ry1 + 5);
    DrawEye(hDC, CEyeS + Rx2 - 5, CEyeSy + Ry2 - 5, CEyeS + Rx2 + 5, CEyeSy + Ry2 + 5);

    EndPaint(hWnd, &ps);
    return 0;

  case WM_KEYDOWN:
    if (wParam == 27)
      DestroyWindow(hWnd);
    return 0;

  case WM_MOUSEMOVE:
    x = LOWORD(lParam);
    y = HIWORD(lParam);

    hDC = GetDC(hWnd);

    SelectObject(hDC, GetStockObject(DC_PEN));
    SelectObject(hDC, GetStockObject(DC_BRUSH));
    SetDCBrushColor(hDC, RGB(255, 255, 255));
    SetDCPenColor(hDC, RGB(0, 0, 0));

    if (wParam & MK_LBUTTON)
      Ellipse(hDC, x - 5, y - 5, x + 5, y + 5);

    return 0;

  case WM_CLOSE:
    if (MessageBox(hWnd, "Are you sure to exit this program?", "Exit", MB_YESNO | MB_ICONQUESTION) == IDNO)
      return 0;
    break;

  case WM_DESTROY:
    KillTimer(hWnd, 111);
    PostMessage(hWnd, WM_QUIT, 0, 0);
    return 0;
  }
  return DefWindowProc(hWnd, Msg, wParam, lParam);
} /* End of 'MyWindowFunc' function */

/* END OF 'T01FWIN.C' FILE */
