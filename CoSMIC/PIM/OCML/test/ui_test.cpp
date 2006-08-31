/**
 * $Id$
 */

#include "windows.h"
#include "assert.h"

#include <string>
#include <fstream>
#include <algorithm>
#include <iterator>
#include <cstdlib>
#include <sstream>

#include "../DllEntry.hpp"
#include "../LoadLibrary.hpp"

LRESULT CALLBACK MainWndProc(HWND, UINT, WPARAM, LPARAM); 

bool pause = false;

std::string p;  // @todo delete this and occurences.

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
  p = "";
  for (size_t i = 0; lpCmdLine[i]; ++i)
  {
    if (i)
      p += ", ";
    p+= lpCmdLine[i];
  }

  WNDCLASSEX wcx; 

  wcx.cbSize = sizeof(wcx);
  wcx.style = CS_HREDRAW | CS_VREDRAW;
  wcx.lpfnWndProc = MainWndProc;
  wcx.cbClsExtra = 0;
  wcx.cbWndExtra = 0;
  wcx.hInstance = hInstance;
  wcx.hIcon = LoadIcon(NULL, IDI_APPLICATION);
  wcx.hCursor = LoadCursor(NULL, IDC_ARROW);
  wcx.hbrBackground = (HBRUSH)( COLOR_WINDOW+1 );
  wcx.lpszMenuName =  "MainMenu";
  wcx.lpszClassName = "MainWClass";
  wcx.hIconSm = NULL;

  ATOM a = RegisterClassEx(&wcx); 
  assert(a);

  HWND hwnd = CreateWindow("MainWClass",
                           "Test DLL",
                           WS_OVERLAPPEDWINDOW,
                           CW_USEDEFAULT,
                           CW_USEDEFAULT,
                           CW_USEDEFAULT,
                           CW_USEDEFAULT,
                           (HWND) NULL,
                           (HMENU) NULL,
                           hInstance,
                           (LPVOID) NULL);
  assert(hwnd);

  ShowWindow(hwnd, nCmdShow); 
  UpdateWindow(hwnd); 

  long bRet;
  MSG msg;
  while((bRet = GetMessage( &msg, NULL, 0, 0 )) != 0 ) 
  {
    if (bRet != -1)
    {
      TranslateMessage( &msg );
      DispatchMessage( &msg );
    }
  }

  return 0;
}

// Windows Callback Procedure
LRESULT CALLBACK MainWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
  PAINTSTRUCT ps;
  HDC hDC;

  switch( msg ) {
  case WM_PAINT:
  {
    hDC = BeginPaint( hWnd, &ps );

    std::stringstream ss;
    char *picml_root = getenv("COSMIC_ROOT");
    ss << "Click on this form to launch dll. "
       << "COSMIC_ROOT environment variable points to: "
       << picml_root
       << " and the command line parameters are: "
       << p.c_str();
  
    TextOut( hDC, 10, 10, ss.str().c_str(), ss.str().size());
    EndPaint( hWnd, &ps );
    break;
  }

  case WM_DESTROY:
    {
      PostQuitMessage( 0 );
      break;
    }

  case WM_LBUTTONUP:
    if (pause)
      return 0;
    else
      {
        pause = true;

        // Read the values from file
        std::string values;
        std::ifstream values_file("values.xml");
        values_file.unsetf(std::ios_base::skipws);
        std::copy(std::istream_iterator<char>(values_file),
                  std::istream_iterator<char>(),
                  std::back_inserter(values));
        
        // Load up the DLL and call DLLFunction
        OCML_Configurator_Library lib;

        char* new_values = lib.call_function(values);
        if (new_values)
          {
            std::ofstream of("values.xml");
            of << new_values;
          }
      }
    pause = false;
    break;

  default:
    return( DefWindowProc( hWnd, msg, wParam, lParam ));
  }
  return 0;
}
