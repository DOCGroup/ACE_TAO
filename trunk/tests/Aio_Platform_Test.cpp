// $Id$
// ============================================================================
//
// = FILENAME
//    aio_platform_test.cpp
//
// =  DESCRITPTION
//     Testing the platform for POSIX Asynchronous I/O.
//
// = AUTHOR
//    Programming for the Real World. Bill O. GallMeister.
//    Modified by Alexander Babu Arulanthu <alex@cs.wustl.edu>
//
// =====================================================================

#include "test_config.h"
#include "ace/OS.h"

// Define old style feature selector.
//#define _POSIX_SOURCE

// Use 9/93 POSIX.1 .2 and .4 definitions only.
//#define _POSIX_C_SOURCE 199309L

#include <unistd.h>
#include <stdio.h>
#include <errno.h>

#if defined _POSIX_ASYNCHRONOUS_IO

#if defined _POSIX_ASYNC_IO

#if _POSIX_ASYNC_IO == -1

int have_asynchio (void)
{
  ACE_DEBUG ((LM_DEBUG,
              "_POSIX_ASYNC_IO = -1.. Not supported at all\n"));
  return 0;
}

#else /* _POSIX_ASYNC_IO is != -1 */
// Supported everywhere.
int have_asynchio (void)
{
  ACE_DEBUG ((LM_DEBUG, "_POSIX_ASYNC_IO is != -1..AIO is supported everywhere\n"));
  ACE_DEBUG ((LM_DEBUG, "System claims to have  POSIX_ASYNCHRONOUS_IO\n"));
  ACE_DEBUG ((LM_DEBUG, "_POSIX_AIO_LISTIO_MAX = %d\n", _POSIX_AIO_LISTIO_MAX));
  ACE_DEBUG ((LM_DEBUG, "_POSIX_AIO_MAX = %d\n", _POSIX_AIO_MAX));

  // Call sysconf to find out runtime values.
  errno = 0;
  ACE_ERROR ((LM_ERROR,
              "(%p):Runtime value of AIO_LISTIO_MAX is %d, errno = %d\n",
              sysconf(_SC_AIO_LISTIO_MAX),
              errno));

  errno = 0;
  ACE_ERROR ((LM_ERROR,
              "(%p):Runtime value of AIO_MAX is %d, errno = %d\n",
              sysconf (_SC_AIO_MAX),
              errno));

  errno = 0;
  ACE_ERROR ((LM_ERROR,
              "(%p):Runtime value of _POSIX_ASYNCHRONOUS_IO is %d, errno = %d\n",
              sysconf (_SC_ASYNCHRONOUS_IO),
              errno));

  errno = 0;
  ACE_ERROR ((LM_ERROR,
              "(%p):Runtime value of _POSIX_REALTIME_SIGNALS is %d, errno = %d\n",
              sysconf (_SC_REALTIME_SIGNALS),
              errno));

  errno = 0;
  ACE_ERROR ((LM_ERROR,
              "(%p):Runtime value of RTSIG_MAX %d\n",
              sysconf (_SC_RTSIG_MAX)));
  return 1;
}

#endif /* _POSIX_ASYNC_IO == -1 */

#else /* _POSIX_ASYNC_IO_ is not defined */


#include <limits.h>
#include <fcntl.h>

char *asynch_io_files[] =
{
  "/tmp/fu",
  "/fu",
  "./fu",
  "/mnt/fu",
  "/dev/tty",
  "dev/dsk/c2t0ds0",
  NULL
};

int have_asynchio (void)
{
  int i, res, num_ok;

  ACE_ERROR ((LM_DEBUG,
              "_POSIX_ASYNC_IO_ is not defined.."));
  ACE_DEBUG ((LM_DEBUG,
              "AIO might *not* be supported everwhere.."));

  ACE_DEBUG ((LM_DEBUG, "System claims to have  POSIX_ASYNCHRONOUS_IO\n"));
  ACE_DEBUG ((LM_DEBUG, "_POSIX_AIO_LISTIO_MAX = %d\n", _POSIX_AIO_LISTIO_MAX));
  ACE_DEBUG ((LM_DEBUG, "_POSIX_AIO_MAX = %d\n", _POSIX_AIO_MAX));

  // Call sysconf to find out runtime values.
  errno = 0;
  ACE_ERROR ((LM_ERROR,
              "(%p):Runtime value of AIO_LISTIO_MAX is %d, errno = %d\n",
              sysconf(_SC_AIO_LISTIO_MAX),
              errno));

  errno = 0;
  ACE_ERROR ((LM_ERROR,
              "(%p):Runtime value of AIO_MAX is %d, errno = %d\n",
              sysconf (_SC_AIO_MAX),
              errno));

  errno = 0;
  ACE_ERROR ((LM_ERROR,
              "(%p):Runtime value of _POSIX_ASYNCHRONOUS_IO is %d\n",
              sysconf (_SC_ASYNCHRONOUS_IO)));

  errno = 0;
  ACE_ERROR ((LM_ERROR,
              "(%p):Runtime value of _POSIX_REALTIME_SIGNALS is %d\n",
              sysconf (_SC_REALTIME_SIGNALS)));

  errno = 0;
  ACE_ERROR ((LM_ERROR,
              "(%p):Runtime value of RTSIG_MAX %d\n",
              sysconf (_SC_RTSIG_MAX)));

  ACE_DEBUG ((LM_DEBUG,
              "Checking individual files using pathconf \n"));

  for (i = num_ok = 0;
       asynch_io_files[i];
       i++)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Checking on path %s\n", asynch_io_files [i]));
      errno = 0;
      res = pathconf (asynch_io_files [i], _PC_ASYNC_IO);
      if ((res == -1) && (!errno))
        {
          ACE_DEBUG ((LM_DEBUG,
                      "\tAsynch IO is allowed :-)\n"));
          num_ok++;
        }
      else if (res < 0)
        {
          ACE_ERROR ((LM_ERROR,
                      "(%p):Asynch I/O is not allowed >:-<"));
        }
      else
        {
          ACE_ERROR ((LM_ERROR,
                      "(%p):Asynch. I/O is allowed :-), res = %d, errno = %d\n",
                      res,
                      errno));
          num_ok++;
        }
    }
  if (num_ok == i)
    return 1;
  else
    return 0;
}
#endif /* _POSIX_ASYNC_IO_ */

#else  /* _POSIX_ASYNCHRONOUS_IO is not defined*/
int have_asynchio (void)
{
  ACE_DEBUG ((LM_DEBUG,
              "_POSIX_ASYNCHRONOUS_IO itself is not defined\n"));
  return 0;
}
#endif /* _POSIX_ASYNCHRONOUS_IO */

int
main ()
{
  int ret_val = have_asynchio ();

  if (ret_val == 1)
    ACE_DEBUG ((LM_DEBUG, "Supported\n"));
  else
    ACE_DEBUG ((LM_DEBUG, "Not Supported\n"));
  return 0;
}
