// -*- C++ -*-
// FUZZ: disable check_for_tchar

//=============================================================================
/**
 *  @file    FaCE_OS.h
 *
 *  $Id$
 *
 *  @author Si Mong Park <spark@ociweb.com>
 */
//=============================================================================

#ifndef FaCE_OS_h
#define FaCE_OS_h

// This definition is for the "int FaCE_MAIN(int, wchar_t**)" using FaCE.
#     define FaCE_MAIN \
ace_main_i (int, ACE_TCHAR**); \
extern BOOL InitInstance (HINSTANCE, int); \
extern void InitSetup(); \
int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow) \
{ \
    MSG msg; \
    HACCEL hAccelTable; \
    if (!InitInstance (hInstance, nCmdShow)) return FALSE; \
    hAccelTable = LoadAccelerators(hInstance, (LPCTSTR)IDC_FACE); \
    InitSetup(); \
    while (GetMessage(&msg, NULL, 0, 0)) { \
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) { \
            TranslateMessage(&msg); \
            DispatchMessage(&msg); \
        } \
    } \
    return msg.wParam; \
} \
int ace_main_i

#endif  // FaCE_OS_h
