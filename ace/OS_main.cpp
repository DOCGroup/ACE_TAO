// -*- C++ -*-
// $Id$

#include "ace/OS_main.h"

ACE_RCSID(ace, OS_main, "$Id$")

#if defined (ACE_HAS_NONSTATIC_OBJECT_MANAGER) && !defined (ACE_DOESNT_INSTANTIATE_NONSTATIC_OBJECT_MANAGER)

#  if !defined (ACE_HAS_MINIMAL_ACE_OS)
#    include "ace/Object_Manager.h"
#  endif /* ! ACE_HAS_MINIMAL_ACE_OS */

// Rename "main ()" on platforms that don't allow it to be called "main ()".

// Also, create ACE_Object_Manager static instance(s) in "main ()".
// ACE_MAIN_OBJECT_MANAGER defines the ACE_Object_Manager(s) that will
// be instantiated on the stack of main ().  Note that it is only used
// when compiling main ():  its value does not affect the contents of
// ace/OS.o.
#  if !defined (ACE_MAIN_OBJECT_MANAGER)
#    define ACE_MAIN_OBJECT_MANAGER \
        ACE_OS_Object_Manager ace_os_object_manager; \
        ACE_Object_Manager ace_object_manager;
#  endif /* ! ACE_MAIN_OBJECT_MANAGER */

#  if !defined (ACE_WIN32)

/* forward declaration */
extern int ace_main_i (int, char *[]);

#    if defined (ACE_PSOSIM)
// PSOSIM root lacks the standard argc, argv command line parameters,
// create dummy argc and argv in the "real" main  and pass to "user" main.
// NOTE: ACE_MAIN must be defined to give the return type as well as the
// name of the entry point.

ACE_MAIN ()   /* user's entry point, e.g., "main" w/out argc, argv */ 
{
  int argc = 1;                            /* dummy arg count */
  char *argv[] = {"psosim"};               /* dummy arg list */
  ACE_MAIN_OBJECT_MANAGER
  int ret_val = -1; /* assume the worst */
  if (ACE_PSOS_Time_t::init_simulator_time ()) /* init simulator time */
  {
    ACE_ERROR((LM_ERROR, "init_simulator_time failed\n"));  /* report */
  }
  else
  {
    ret_val = ace_main_i (argc, argv);   /* call user main, save result */
  }
  ACE_OS::exit (ret_val);                /* pass code to simulator exit */
}

#    elif defined (ACE_PSOS) && defined (ACE_PSOS_LACKS_ARGC_ARGV)
// PSOS root lacks the standard argc, argv command line parameters,
// create dummy argc and argv in the "real" main  and pass to "user" main.
// Ignore return value from user main as well.  NOTE: ACE_MAIN must be
// defined to give the return type as well as the name of the entry point

ACE_MAIN ()   /* user's entry point, e.g., "main" w/out argc, argv */ 
{
  int argc = 1;                           /* dummy arg count */
  char *argv[] = {"root"};                /* dummy arg list */
  ACE_MAIN_OBJECT_MANAGER
  ace_main_i (argc, argv);                /* call user main, ignore result */
}

#    endif /* ACE_PSOSIM */

int ace_os_main_i (int argc, char *argv[]) /* user's entry point, e.g., main */
{ 
  ACE_MAIN_OBJECT_MANAGER 
  return ace_main_i (argc, argv);           /* what the user calls "main" */ 
}

#  elif !defined (ACE_WINCE)

int xxx::run (int argc, ACE_TCHAR *argv[])  \
{ \
  return this->run_i (argc, argv); \
} \

int
ace_os_wmain_i (xxx &x, int argc, ACE_TCHAR *argv[]) /* user's entry point, e.g., main */ 
{
  ACE_MAIN_OBJECT_MANAGER
  return x.run (argc, argv);           /* what the user calls "main" */
} 

#  else /* ACE_WINCE */

#    if defined (ACE_TMAIN)  // Use WinMain on CE; others give warning/error.
#      undef ACE_TMAIN
#    endif  // ACE_TMAIN

// CE only gets a command line string;  no argv. So we need to convert it
// when the main entrypoint expects argc/argv. ACE_ARGV supports this.
#    include "ace/ARGV.h"

// Support for ACE_TMAIN, which is a recommended way. It would be nice if
// CE had CommandLineToArgvW()... but it's only on NT3.5 and up.

int WINAPI ace_os_wintmain_i (HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
  ACE_TCHAR cmdline[1024];
  ACE_OS::strcpy (cmdline, ACE_LIB_TEXT ("program "));
  ACE_OS::strcat (cmdline, lpCmdLine);
  ACE_ARGV ce_argv (cmdline);
  ACE::init ();
  ACE_MAIN_OBJECT_MANAGER
  int i = ace_main_i (ce_argv.argc (), ce_argv.argv ());
  ACE::fini ();
  return i;
}

// Support for wchar_t but still can't fit to CE because of the command
// line parameters.
int WINAPI ace_os_winwmain_i (HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
  ACE_TCHAR cmdline[1024];
  ACE_OS::strcpy (cmdline, ACE_LIB_TEXT ("program "));
  ACE_OS::strcat (cmdline, lpCmdLine);
  ACE_ARGV ce_argv (cmdline);
  ACE::init ();
  ACE_MAIN_OBJECT_MANAGER
  int i = ace_main_i (ce_argv.argc (), ce_argv.argv ());
  ACE::fini ();
  return i;
}

// Supporting legacy 'main' is A LOT easier for users than changing existing
// code on WinCE. Unfortunately, evc 3 can't grok a #include within the macro
// expansion, so it needs to go out here.
#    include "ace/Argv_Type_Converter.h"
int WINAPI ace_os_winmain_i (HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
  ACE_TCHAR cmdline[1024];
  ACE_OS::strcpy (cmdline, ACE_LIB_TEXT ("program "));
  ACE_OS::strcat (cmdline, lpCmdLine);
  ACE_ARGV ce_argv (cmdline);
  ACE::init ();
  ACE_MAIN_OBJECT_MANAGER
  ACE_Argv_Type_Converter command_line (ce_argv.argc (), ce_argv.argv ());
  int i = ace_main_i (command_line.get_argc(), command_line.get_ASCII_argv());
  ACE::fini ();
  return i;
}
int ace_main_i

#  endif   /* !ACE_WINCE */
# endif /* ACE_HAS_NONSTATIC_OBJECT_MANAGER && !ACE_HAS_WINCE && !ACE_DOESNT_INSTANTIATE_NONSTATIC_OBJECT_MANAGER */
