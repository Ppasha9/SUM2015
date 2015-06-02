#include <windows.h>

void main( void )
{
  if ((MessageBox(NULL, "Please choose: Y\\N", "Question", MB_YESNO | MB_ICONQUESTION)) == IDYES)
    MessageBox(NULL, "You choose 'YES'", "Info", MB_OK | MB_ICONINFORMATION);
  else
    MessageBox(NULL, "You choose 'NO'", "Info", MB_OK | MB_ICONINFORMATION);
}