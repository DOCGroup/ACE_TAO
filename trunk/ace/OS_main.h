// -*- C++ -*-

//=============================================================================
/**
 *  @file   OS_main.h
 *
 *  $Id$
 *
 *  @author Douglas C. Schmidt <schmidt@cs.wustl.edu>
 *  @author Jesper S. M|ller<stophph@diku.dk>
 *  @author and a cast of thousands...
 *
 *  Originally in OS.h.
 */
//=============================================================================

#include "ace/ACE_export.h"

#ifndef ACE_OS_MAIN_H
# define ACE_OS_MAIN_H

# include /**/ "ace/pre.h"

# if !defined (ACE_LACKS_PRAGMA_ONCE)
#  pragma once
# endif /* ACE_LACKS_PRAGMA_ONCE */

# if !defined (ACE_MAIN)
#   define ACE_MAIN main
# endif /* ! ACE_MAIN */

# if !defined (ACE_WMAIN)
#   define ACE_WMAIN wmain
# endif /* ! ACE_WMAIN */

# if defined (ACE_WIN32) && defined (ACE_USES_WCHAR)
#   define ACE_TMAIN wmain
# else
#   define ACE_TMAIN main
# endif

# if defined (ACE_DOESNT_INSTANTIATE_NONSTATIC_OBJECT_MANAGER)
#   if !defined (ACE_HAS_NONSTATIC_OBJECT_MANAGER)
#     define ACE_HAS_NONSTATIC_OBJECT_MANAGER
#   endif /* ACE_HAS_NONSTATIC_OBJECT_MANAGER */
# endif /* ACE_DOESNT_INSTANTIATE_NONSTATIC_OBJECT_MANAGER */

# if defined (ACE_HAS_NONSTATIC_OBJECT_MANAGER) \
       && !defined (ACE_DOESNT_INSTANTIATE_NONSTATIC_OBJECT_MANAGER)

// Rename "main ()" on platforms that don't allow it to be called "main ()".

#   if defined (ACE_PSOSIM) 
         || (defined (ACE_PSOS) && defined (ACE_PSOS_LACKS_ARGC_ARGV))

#     define main \
ace_os_main_i (int, char *[]); \
ACE_MAIN ()   /* user's entry point, e.g., "main" w/out argc, argv */ \
{ \
  ace_os_main_i (argc, argv); /* what the user calls "main" */ \
} \
int \
ace_main_i

#   elif !defined (ACE_WIN32)
   
#     define main \
ace_os_main_i (int, char *[]); \
int \
ACE_MAIN (int argc, char *argv[])    /* user's entry point, e.g., main */ \
{ \
  return ace_os_main_i (argc, argv); /* what the user calls "main" */ \
} \
int \
ace_main_i

#   elif !defined (ACE_WINCE)

class ACE_Export xxx
{
public:
  xxx (void);
  ~xxx (void) {}
  int run (int, ACE_TCHAR *[]);
  virtual int run_i (int, ACE_TCHAR *[]) = 0;
};

#       define wmain \
ace_os_wmain_i (xxx&, int, ACE_TCHAR *[]); \
int xxx::run_i (int argc, ACE_TCHAR *argv[])  \
{ \
  return ace_os_wmain_i (argc, argv); \
} \
int \
ACE_WMAIN (int argc, ACE_TCHAR *argv[]) /* user's entry point, e.g., wmain */ \
{ \
  xxx x; \
  return ace_os_wmain_i (x, argc, argv);   /* what the user calls "main" */ \
} \
int \
ace_main_i

#   else /* ACE_HAS_WINCE */

#     if defined (ACE_TMAIN)  // Use WinMain on CE; others give warning/error.
#       undef ACE_TMAIN
#     endif  // ACE_TMAIN

// CE only gets a command line string;  no argv. So we need to convert it
// when the main entrypoint expects argc/argv. ACE_ARGV supports this.
#     include "ace/ARGV.h"

// Support for ACE_TMAIN, which is a recommended way. It would be nice if
// CE had CommandLineToArgvW()... but it's only on NT3.5 and up.

#     define ACE_TMAIN \
ace_os_wintmain_i (HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow);  /* forward declaration */ \
int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow) \
{ \
  return ace_os_wintmain_i (hInstance, hPrevInstance, lpCmdLine, nCmdShow); \
} \
int ace_main_i

// Support for wchar_t but still can't fit to CE because of the command
// line parameters.
#     define wmain \
ace_os_winwmain_i (int, ACE_TCHAR *[]);  /* forward declaration */ \
int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow) \
{ \
  return ace_os_winwmain_i (hInstance, hPrevInstance, lpCmdLine, nCmdShow); \
} \
int ace_main_i

// Supporting legacy 'main' is A LOT easier for users than changing existing
// code on WinCE. Unfortunately, evc 3 can't grok a #include within the macro
// expansion, so it needs to go out here.
#     include "ace/Argv_Type_Converter.h"
#     define main \
ace_os_winmain_i (int, char *[]);  /* forward declaration */ \
int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow) \
{ \
  return ace_os_winmain_i (hInstance, hPrevInstance, lpCmdLine, nCmdShow); \
} \
int ace_main_i

#   endif   /* ACE_PSOSIM */
# endif /* ACE_HAS_NONSTATIC_OBJECT_MANAGER && !ACE_HAS_WINCE && !ACE_DOESNT_INSTANTIATE_NONSTATIC_OBJECT_MANAGER */

# include /**/ "ace/post.h"

#endif /* ACE_OS_MAIN_H */
