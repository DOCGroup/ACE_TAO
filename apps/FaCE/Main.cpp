// $Id$
// FUZZ: disable check_for_tchar

// ************************************************
// ** This file is NOT to be used for framework. **
// ************************************************

// This file defines the entry point for Windows CE, which is defined in OS.h for real applications.


#include "FaCE.h"

extern BOOL InitInstance (HINSTANCE, int);
extern void InitSetup();


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
    MSG msg;
    HACCEL hAccelTable;
    if (!InitInstance (hInstance, nCmdShow)) return FALSE;
    hAccelTable = LoadAccelerators(hInstance, (LPCTSTR)IDC_FACE);
    InitSetup();
    while (GetMessage(&msg, NULL, 0, 0)) {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
    return msg.wParam;
}


#ifdef NO_ACE

int main_i(int, ACE_TCHAR**)
{
    // this function will be replaced by user's main_ce function
    return 0;
}

#else

int ace_main_i(int, ACE_TCHAR**)
{
    // this function will be replaced by user's main_ce function
    return 0;
}

#endif  // NO_ACE
