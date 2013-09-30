
//=============================================================================
/**
 *  @file    Unload_libACE.cpp
 *
 *  $Id$
 *
 *    This is a simple test of library unloading that uses
 *    an application which has _not_ been linked with libACE
 *    but uses dlopen() to dynamically load libACE
 *    and then uses dlclose() to unload it.
 *
 *
 *  @author David Smith <dts@prismtech.com> and Don Sharp <Donald.Sharp@prismtech.com>
 */
//=============================================================================


//FUZZ: disable check_for_lack_ACE_OS
//FUZZ: disable check_for_improper_main_declaration

#include <stdio.h>

#undef UNLOAD_LIBACE_TEST

#if defined (__GNUC__)
#if !defined (ACE_VXWORKS) && !defined (__MINGW32__) && !defined (__CYGWIN32__)
#define UNLOAD_LIBACE_TEST 1
#endif /* !ACE_VXWORKS && !__MINGW32__ && !CYGWIN32 */
#endif /* __GNUC__ */

#if defined (__hpux) || defined (__SUNPRO_CC)
#define UNLOAD_LIBACE_TEST 1
#endif /* (__hpux) || (__SUNPRO_CC) */

#if defined (ACE_AS_STATIC_LIBS)
#undef UNLOAD_LIBACE_TEST
#endif /* ACE_AS_STATIC_LIBS */

#ifdef UNLOAD_LIBACE_TEST

#include <errno.h>
#include <dlfcn.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define TIME_STAMP_FIELD_WIDTH  32

static char *
time_stamp (char date_and_time[], int date_and_timelen, int format)
{
  static char const *const month_name[] =
    {
      "Jan",
      "Feb",
      "Mar",
      "Apr",
      "May",
      "Jun",
      "Jul",
      "Aug",
      "Sep",
      "Oct",
      "Nov",
      "Dec"
    };

  static char const *const day_of_week_name[] =
    {
      "Sun",
      "Mon",
      "Tue",
      "Wed",
      "Thu",
      "Fri",
      "Sat"
    };

  char *ts = 0;

  if (date_and_timelen >= TIME_STAMP_FIELD_WIDTH)
    {
      time_t timeval;
      struct tm *now;

      time (&timeval);
      now = localtime (&timeval);     /* Get current local time. */

      if (format == 'Y')
        {
          sprintf (date_and_time,
                   "%3s %3s %2d %04d %02d:%02d:%02d.%06d",
                   day_of_week_name[now->tm_wday],
                   month_name[now->tm_mon],
                   (int) now->tm_mday,
                   (int) now->tm_year + 1900,
                   (int) now->tm_hour,
                   (int) now->tm_min, (int) now->tm_sec, (int) 0);
        }
      else                      /* 'T' */
        {
          sprintf (date_and_time,
                   "%3s %2d %02d:%02d:%02d.%03d %04d",
                   month_name[now->tm_mon],
                   (int) now->tm_mday,
                   (int) now->tm_hour,
                   (int) now->tm_min,
                   (int) now->tm_sec, (int) 0,
                   (int) now->tm_year + 1900);
        }

      ts = date_and_time;
    }
  return ts;
}

int
main (int, char **)
{
  char const *const program = "UnloadLibACE";

  int status = 0;
  void *handle = 0;
  char *ace_root = 0;
  char tbuf[BUFSIZ];
  char ybuf[BUFSIZ];
  FILE *logfp = 0;

  if ((logfp = fopen ("log/UnloadLibACE.log", "w")) != 0)
    {
      setvbuf (logfp, 0, _IONBF, 0);
      // reassign stdout/stderr to log file
      int fdno = fileno (logfp);

      dup2 (fdno, fileno (stdout));
      dup2 (fdno, fileno (stderr));
      setvbuf (stdout, 0, _IONBF, 0);
      setvbuf (stderr, 0, _IONBF, 0);
      fflush (stdout);
      fflush (stderr);

      printf ("%s@LM_DEBUG@ Starting %s test at %s\n",
              time_stamp (tbuf, BUFSIZ, 'T'),
              program, time_stamp (ybuf, BUFSIZ, 'Y'));

      if ((ace_root = getenv ("ACE_ROOT")) != 0)
        {
          char buf[BUFSIZ];

          strcpy (buf, ace_root);
          strcat (buf, "/lib/");
          const char *subdir_env = getenv ("ACE_EXE_SUB_DIR");
          if (subdir_env)
            {
              strcat (buf, subdir_env);
              strcat (buf, "/");
            }
          strcat (buf, "lib");
#if defined (ACE_LIB_NAME)
          strcat (buf, ACE_LIB_NAME);
#else
          strcat (buf, "ACE");
#endif /* ACE_LIB_NAME */
#if defined (__hpux) && !(defined (__ia64) && (__ia64 == 1))
          strcat (buf, ".sl");
#elif defined (__APPLE__)
          strcat (buf, ".dylib");
#else
          strcat (buf, ".so");
#endif /* (__hpux) */

          handle = dlopen (buf, RTLD_LAZY);
          if (handle == 0)
            {
              // is it because of "No such file or directory" ?
              if (errno != ENOENT)
                {
                  fprintf (stderr,
                           "%s@LM_ERROR@ dlopen() returned NULL\n",
                           time_stamp (tbuf, BUFSIZ, 'T'));
                  fprintf (stderr,
                           "%s@LM_ERROR@ dlerror() says: %s\n",
                           time_stamp (tbuf, BUFSIZ, 'T'), dlerror ());
                  status = 1;
                }
              else
                {
                  printf ("%s@LM_DEBUG@ dlopen() did not find %s\n",
                          time_stamp (tbuf, BUFSIZ, 'T'), buf);
                  status = 0;
                }
            }
          else if (dlclose (handle) != 0)
            {
              fprintf (stderr,
                       "%s@LM_ERROR@ dlclose() failed : %s\n",
                       time_stamp (tbuf, BUFSIZ, 'T'), strerror (errno));
              status = 1;
            }
        }
      else
        {
          fprintf (stderr,
                   "%s@LM_ERROR@ ACE_ROOT environment variable not set\n",
                   time_stamp (tbuf, BUFSIZ, 'T'));
          status = 1;
        }

      fflush (stdout);
      fflush (stderr);
      fflush (logfp);

      fclose (logfp);
    }
  else
    {
      // Couldn't go into the log file !!!
      printf ("%s@LM_DEBUG@ Starting %s test at %s\n",
              time_stamp (tbuf, BUFSIZ, 'T'),
              program, time_stamp (ybuf, BUFSIZ, 'Y'));

      fprintf (stderr,
               "%s@LM_ERROR@ Could not open log/UnloadLibACE.log : %s\n",
               time_stamp (tbuf, BUFSIZ, 'T'), strerror (errno));
      status = 1;
    }

  printf ("%s@LM_DEBUG@ Ending %s test at %s\n",
          time_stamp (tbuf, BUFSIZ, 'T'),
          program, time_stamp (ybuf, BUFSIZ, 'Y'));

  fflush (stderr);
  fflush (stdout);
  fclose (stdout);
  fclose (stderr);

  // Don't change this since we do NOT want to use ACE for this test!
  exit (status);
  return 0;
}
#else
# if defined (WIN32) && defined (ACE_USES_WCHAR)
// Borrow include list from ace_wchar.h
# if defined (ACE_HAS_WINCE)
#   include /**/ <wtypes.h>
# elif !defined (__BORLANDC__)
#   include /**/ <wchar.h>
# endif /* ACE_HAS_WINCE || __BORLANDC__ */

int
wmain (int, wchar_t **)
#else
int
main (int, char **)
#endif /* (WIN32) && (ACE_USES_WCHAR) */
{
  char const *const program = "UnloadLibACE";

  FILE *logfp = 0;

  if ((logfp = fopen ("log/UnloadLibACE.log", "w")) != 0)
    {
      fprintf (logfp, "@LM_DEBUG@ Starting %s test\n", program);
      fprintf (logfp, "@LM_DEBUG@ %s test not implemented for this platform\n",
               program);
      fprintf (logfp, "@LM_DEBUG@ Ending %s test\n", program);

      fflush (logfp);
      fclose (logfp);
    }
  return 0;
}
#endif /* UNLOAD_LIBACE_TEST */
