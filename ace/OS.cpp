// $Id$

#include "ace/OS.h"
//#include "ace/Sched_Params.h"
//#include "ace/OS_Thread_Adapter.h"

#if !defined (ACE_HAS_WINCE)
# include "ace/OS_QoS.h"
#endif  // ACE_HAS_WINCE

// Perhaps we should *always* include ace/OS.i in order to make sure
// we can always link against the OS symbols?
#if !defined (ACE_HAS_INLINED_OSCALLS)
# include "ace/OS.i"
#endif /* ACE_HAS_INLINED_OS_CALLS */

ACE_RCSID(ace, OS, "$Id$")

#if defined (ACE_WIN32)

// = Static initialization.

// Keeps track of whether we've initialized the WinSock DLL.
int ACE_OS::socket_initialized_;

#endif /* WIN32 */

#if !defined(ACE_WIN32) && defined (__IBMCPP__) && (__IBMCPP__ >= 400)
# define ACE_BEGINTHREADEX(STACK, STACKSIZE, ENTRY_POINT, ARGS, FLAGS, THR_ID) \
       (*THR_ID = ::_beginthreadex ((void(_Optlink*)(void*))ENTRY_POINT, STACK, STACKSIZE, ARGS), *THR_ID)
#elif defined(ACE_WIN32) && defined (__IBMCPP__) && (__IBMCPP__ >= 400)

struct __IBMCPP__thread_params {
  __IBMCPP__thread_params(ACE_THR_C_FUNC e, LPVOID a)
    :entry_point(e),args(a) {}
  ACE_THR_C_FUNC entry_point;
  LPVOID args;
};

# pragma handler(initThread)
extern "C" DWORD __stdcall __IBMCPP__initThread(void *arg)
{
  // Must reset 387 since using CreateThread
  _fpreset();

  // Dispatch user function...
  auto_ptr<__IBMCPP__thread_params> parms((__IBMCPP__thread_params *)arg);
  (*parms->entry_point)(parms->args);
  _endthread();
  return 0;
}

HANDLE WINAPI __IBMCPP__beginthreadex(void *stack,
                                      DWORD stacksize,
                                      ACE_THR_C_FUNC entry_point,
                                      LPVOID args,
                                      DWORD flags,
                                      LPDWORD thr_id)
{
  return  CreateThread(0,
                       stacksize,
                       (LPTHREAD_START_ROUTINE)__IBMCPP__initThread,
                       new __IBMCPP__thread_params(entry_point, args),
                       flags,
                       thr_id);
}

# define ACE_BEGINTHREADEX(STACK, STACKSIZE, ENTRY_POINT, ARGS, FLAGS, THR_ID) \
             __IBMCPP__beginthreadex(STACK, STACKSIZE, ENTRY_POINT, ARGS, FLAGS, THR_ID)

#elif defined (ACE_HAS_WINCE) && defined (UNDER_CE) && (UNDER_CE >= 211)
# define ACE_BEGINTHREADEX(STACK, STACKSIZE, ENTRY_POINT, ARGS, FLAGS, THR_ID) \
      CreateThread (0, STACKSIZE, (unsigned long (__stdcall *) (void *)) ENTRY_POINT, ARGS, (FLAGS) & CREATE_SUSPENDED, (unsigned long *) THR_ID)
#elif defined(ACE_HAS_WTHREADS)
  // Green Hills compiler gets confused when __stdcall is imbedded in
  // parameter list, so we define the type ACE_WIN32THRFUNC_T and use it
  // instead.
  typedef unsigned (__stdcall *ACE_WIN32THRFUNC_T)(void*);
# define ACE_BEGINTHREADEX(STACK, STACKSIZE, ENTRY_POINT, ARGS, FLAGS, THR_ID) \
      ::_beginthreadex (STACK, STACKSIZE, (ACE_WIN32THRFUNC_T) ENTRY_POINT, ARGS, FLAGS, (unsigned int *) THR_ID)
#endif /* defined (__IBMCPP__) && (__IBMCPP__ >= 400) */

#if defined (VXWORKS)
# include /**/ <usrLib.h>   /* for ::sp() */

// This global function can be used from the VxWorks shell to pass
// arguments to a C main () function.
//
// usage: -> spa main, "arg1", "arg2"
//
// All arguments must be quoted, even numbers.
int
spa (FUNCPTR entry, ...)
{
  static const unsigned int MAX_ARGS = 10;
  static char *argv[MAX_ARGS];
  va_list pvar;
  unsigned int argc;

  // Hardcode a program name because the real one isn't available
  // through the VxWorks shell.
  argv[0] = "ace_main";

  // Peel off arguments to spa () and put into argv.  va_arg () isn't
  // necessarily supposed to return 0 when done, though since the
  // VxWorks shell uses a fixed number (10) of arguments, it might 0
  // the unused ones.  This function could be used to increase that
  // limit, but then it couldn't depend on the trailing 0.  So, the
  // number of arguments would have to be passed.
  va_start (pvar, entry);

  for (argc = 1; argc <= MAX_ARGS; ++argc)
    {
      argv[argc] = va_arg (pvar, char *);

      if (argv[argc] == 0)
        break;
    }

  if (argc > MAX_ARGS  &&  argv[argc-1] != 0)
    {
      // try to read another arg, and warn user if the limit was exceeded
      if (va_arg (pvar, char *) != 0)
        ACE_OS::fprintf (stderr, "spa(): number of arguments limited to %d\n",
                         MAX_ARGS);
    }
  else
    {
      // fill unused argv slots with 0 to get rid of leftovers
      // from previous invocations
      for (unsigned int i = argc; i <= MAX_ARGS; ++i)
        argv[i] = 0;
    }

  // The hard-coded options are what ::sp () uses, except for the
  // larger stack size (instead of ::sp ()'s 20000).
  const int ret = ::taskSpawn (argv[0],    // task name
                               100,        // task priority
                               VX_FP_TASK, // task options
                               ACE_NEEDS_HUGE_THREAD_STACKSIZE, // stack size
                               entry,      // entry point
                               argc,       // first argument to main ()
                               (int) argv, // second argument to main ()
                               0, 0, 0, 0, 0, 0, 0, 0);
  va_end (pvar);

  // ::taskSpawn () returns the taskID on success: return 0 instead if
  // successful
  return ret > 0 ? 0 : ret;
}

// A helper function for the extended spa functions
static void
add_to_argv (int& argc, char** argv, int max_args, char* string)
{
  char indouble   = 0;
  size_t previous = 0;
  size_t length   = ACE_OS::strlen (string);

  // We use <= to make sure that we get the last argument
  for (size_t i = 0; i <= length; i++)
    {
      // Is it a double quote that hasn't been escaped?
      if (string[i] == '\"' && (i == 0 || string[i - 1] != '\\'))
        {
          indouble ^= 1;
          if (indouble)
            {
              // We have just entered a double quoted string, so
              // save the starting position of the contents.
              previous = i + 1;
            }
          else
            {
              // We have just left a double quoted string, so
              // zero out the ending double quote.
              string[i] = '\0';
            }
        }
      else if (string[i] == '\\')  // Escape the next character
        {
          // The next character is automatically
          // skipped because of the strcpy
          ACE_OS::strcpy (string + i, string + i + 1);
          length--;
        }
      else if (!indouble &&
               (ACE_OS::ace_isspace (string[i]) || string[i] == '\0'))
        {
          string[i] = '\0';
          if (argc < max_args)
            {
              argv[argc] = string + previous;
              argc++;
            }
          else
            {
              ACE_OS::fprintf (stderr, "spae(): number of arguments "
                                       "limited to %d\n", max_args);
            }

          // Skip over whitespace in between arguments
          for(++i; i < length && ACE_OS::ace_isspace (string[i]); ++i)
            {
            }

          // Save the starting point for the next time around
          previous = i;

          // Make sure we don't skip over a character due
          // to the above loop to skip over whitespace
          i--;
        }
    }
}

// This global function can be used from the VxWorks shell to pass
// arguments to a C main () function.
//
// usage: -> spae main, "arg1 arg2 \"arg3 with spaces\""
//
// All arguments must be within double quotes, even numbers.
int
spae (FUNCPTR entry, ...)
{
  static const int WINDSH_ARGS = 10;
  static const int MAX_ARGS    = 128;
  static char* argv[MAX_ARGS]  = { "ace_main", 0 };
  va_list pvar;
  int argc = 1;

  // Peel off arguments to spa () and put into argv.  va_arg () isn't
  // necessarily supposed to return 0 when done, though since the
  // VxWorks shell uses a fixed number (10) of arguments, it might 0
  // the unused ones.
  va_start (pvar, entry);

  int i = 0;
  for (char* str = va_arg (pvar, char*);
       str != 0 && i < WINDSH_ARGS; str = va_arg (pvar, char*), ++i)
    {
      add_to_argv(argc, argv, MAX_ARGS, str);
    }

  // fill unused argv slots with 0 to get rid of leftovers
  // from previous invocations
  for (i = argc; i < MAX_ARGS; ++i)
    argv[i] = 0;

  // The hard-coded options are what ::sp () uses, except for the
  // larger stack size (instead of ::sp ()'s 20000).
  const int ret = ::taskSpawn (argv[0],    // task name
                               100,        // task priority
                               VX_FP_TASK, // task options
                               ACE_NEEDS_HUGE_THREAD_STACKSIZE, // stack size
                               entry,      // entry point
                               argc,       // first argument to main ()
                               (int) argv, // second argument to main ()
                               0, 0, 0, 0, 0, 0, 0, 0);
  va_end (pvar);

  // ::taskSpawn () returns the taskID on success: return 0 instead if
  // successful
  return ret > 0 ? 0 : ret;
}

// This global function can be used from the VxWorks shell to pass
// arguments to a C main () function.  The function will be run
// within the shells task.
//
// usage: -> spaef main, "arg1 arg2 \"arg3 with spaces\""
//
// All arguments must be within double quotes, even numbers.
// Unlike the spae function, this fuction executes the supplied
// routine in the foreground, rather than spawning it in a separate
// task.
int
spaef (FUNCPTR entry, ...)
{
  static const int WINDSH_ARGS = 10;
  static const int MAX_ARGS    = 128;
  static char* argv[MAX_ARGS]  = { "ace_main", 0 };
  va_list pvar;
  int argc = 1;

  // Peel off arguments to spa () and put into argv.  va_arg () isn't
  // necessarily supposed to return 0 when done, though since the
  // VxWorks shell uses a fixed number (10) of arguments, it might 0
  // the unused ones.
  va_start (pvar, entry);

  int i = 0;
  for (char* str = va_arg (pvar, char*);
       str != 0 && i < WINDSH_ARGS; str = va_arg (pvar, char*), ++i)
    {
      add_to_argv(argc, argv, MAX_ARGS, str);
    }

  // fill unused argv slots with 0 to get rid of leftovers
  // from previous invocations
  for (i = argc; i < MAX_ARGS; ++i)
    argv[i] = 0;

  int ret = entry (argc, argv);

  va_end (pvar);

  // Return the return value of the invoked ace_main routine.
  return ret;
}
#endif /* VXWORKS */

#if defined (__DGUX) && defined (ACE_HAS_THREADS) && defined (_POSIX4A_DRAFT10_SOURCE)
extern "C" int __d6_sigwait (sigset_t *set);

extern "C" int __d10_sigwait (const sigset_t *set, int *sig)
{
  sigset_t unconst_set = *set;
  int caught_sig = __d6_sigwait (&unconst_set);

  if (caught == -1)
    return -1;

  *sig = caught_sig;
  return 0;
}
#endif /* __DGUX && PTHREADS && _POSIX4A_DRAFT10_SOURCE */


// include new cpps
#include "ace/Cleanup.cpp"
#include "ace/Object_Manager_Base.cpp"
#include "ace/OS_NS_arpa_inet.cpp"
#include "ace/OS_NS_ctype.cpp"
#include "ace/OS_NS_dirent.cpp"
#include "ace/OS_NS_dlfcn.cpp"
#include "ace/OS_NS_errno.cpp"
#include "ace/OS_NS_fcntl.cpp"
#include "ace/OS_NS_math.cpp"
#include "ace/OS_NS_netdb.cpp"
#include "ace/OS_NS_poll.cpp"
#include "ace/OS_NS_pwd.cpp"
#include "ace/OS_NS_signal.cpp"
#include "ace/OS_NS_stdio.cpp"
#include "ace/OS_NS_stdlib.cpp"
#include "ace/OS_NS_string.cpp"
#include "ace/OS_NS_strings.cpp"
#include "ace/OS_NS_stropts.cpp"
#include "ace/OS_NS_sys_mman.cpp"
#include "ace/OS_NS_sys_msg.cpp"
#include "ace/OS_NS_sys_resource.cpp"
#include "ace/OS_NS_sys_select.cpp"
#include "ace/OS_NS_sys_shm.cpp"
#include "ace/OS_NS_sys_socket.cpp"
#include "ace/OS_NS_sys_stat.cpp"
#include "ace/OS_NS_sys_time.cpp"
#include "ace/OS_NS_sys_uio.cpp"
#include "ace/OS_NS_sys_utsname.cpp"
#include "ace/OS_NS_sys_wait.cpp"
#include "ace/OS_NS_Thread.cpp"
#include "ace/OS_NS_time.cpp"
#include "ace/OS_NS_unistd.cpp"
#include "ace/OS_NS_wchar.cpp"
