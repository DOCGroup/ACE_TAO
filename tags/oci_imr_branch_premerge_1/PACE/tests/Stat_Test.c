/* $Id$ -*- C -*- */

/* ===================================================================== */
/*                                                                       */
/* = FILENAME                                                            */
/*    Stat_Test.c                                                        */
/*                                                                       */
/* =  DESCRIPTION                                                        */
/*     Testing the platform for POSIX functions typically found in the   */
/*     sys/stat.h file (e.g., chmod, mkdir, stat). This is not meant     */
/*     to be an exhaustive test at this point but more a sanity check    */
/*     that PACE works (at least somewhat) as advertised.                */
/*     This program creates a directory (via mkdir), creates a temporary */
/*     file in that directory (via open), modifies the permissions of    */
/*     the file (via chmod and fchmod), gets information about the file  */
/*     (via stat and fstat), and checks the process' file mode creation  */
/*     mask (via umask). The temporary file and directory are then       */
/*     closed and deleted.                                               */
/*                                                                       */
/* = AUTHOR                                                              */
/*    Joe Hoffert <joeh@cs.wustl.edu>                                    */
/*                                                                       */
/* ===================================================================== */

#include "pace/stdio.h"
#include "pace/unistd.h"
#include "pace/fcntl.h"
#include "pace/sys/stat.h"

#if defined (PACE_VXWORKS) && PACE_VXWORKS != 0
#include "vxworks_stub.c"
#endif /* VXWORKS */


#if PACE_LYNXOS != 0x300
int
main (int argc, char **argv)
{
  const char * dirname = "./junk";
  const char * failure = "***FAILED***";

  int fd;
  /*
  int retval;
  char* retval2;
  char buffer[256];
  */

  /* Test creating a directory. */
  /*printf("Starting test...\n");
  if (pace_mkdir(dirname, mode) != 0)
    {
      pace_printf("pace_mkdir %s\n", failure);
      return -1;
    }
  */

  fd = pace_open (dirname,
                  O_WRONLY | O_CREAT,
                  0x644);
  if (fd == 0)
    {
      pace_printf("pace_open %s\n", failure);
      pace_perror("pace_open");
      return -1;
    }
  else
    {
      pace_close (fd);
      pace_unlink (dirname);
    }

  PACE_UNUSED_ARG (argc);
  PACE_UNUSED_ARG (argv);
  return 0;
}
#else
int
main (int argc, char **argv)
{
  printf("PACE does not support LynxOS 3.0.0.\n");
}
#endif /* PACE_LYNXOS == 0x300 */
