/* $Id$ -*- C -*- */

/* ===================================================================== */
/*                                                                       */
/* = FILENAME                                                            */
/*    vxworks_stub.c                                                     */
/*                                                                       */
/* =  DESCRIPTION                                                        */
/*     The file includes the special machinations that are needed to     */
/*     kick off a C program in VxWorks. Every test run on VxWorks needs  */
/*     to have this so it is put into this common file.                  */
/*                                                                       */
/* = AUTHOR                                                              */
/*    Joe Hoffert <joeh@cs.wustl.edu>                                    */
/*                                                                       */
/* ===================================================================== */

# define main post_pace_main
# include /**/ <usrLib.h>   /* for ::sp() */
# include /**/ "pace/pthread.h"

int post_pace_main();

/* This global function can be used from the VxWorks shell to pass
 * arguments to a C main () function.
 *
 * usage: -> spa main, "arg1", "arg2"
 *
 * All arguments must be quoted, even numbers.
 */
int
spa (FUNCPTR entry, ...)
{
  const unsigned int MAX_ARGS = 10;
  char *argv[MAX_ARGS];
  va_list pvar;
  unsigned int argc;
  int ret;

  /* Hardcode a program name because the real one isn't available
   * through the VxWorks shell.
   */
  argv[0] = "pace_main";

  /* Peel off arguments to spa () and put into argv.  va_arg () isn't
   * necessarily supposed to return 0 when done, though since the
   * VxWorks shell uses a fixed number (10) of arguments, it might 0
   * the unused ones.  This function could be used to increase that
   * limit, but then it couldn't depend on the trailing 0.  So, the
   * number of arguments would have to be passed.
   */
  va_start (pvar, entry);

  for (argc = 1; argc <= MAX_ARGS; ++argc)
    {
      argv[argc] = va_arg (pvar, char *);

      if (argv[argc] == 0)
        break;
    }

  if (argc > MAX_ARGS  &&  argv[argc-1] != 0)
    {
      /* try to read another arg, and warn user if the limit was exceeded */
      if (va_arg (pvar, char *) != 0)
        pace_fprintf (stderr, "spa(): number of arguments limited to %d\n",
                      MAX_ARGS);
    }
  else
    {
      /* fill unused argv slots with 0 to get rid of leftovers
       * from previous invocations
       */
      unsigned int i;
      for (i = argc; i <= MAX_ARGS; ++i)
        argv[i] = 0;
    }

  /* The hard-coded options are what ::sp () uses, except for the
   * larger stack size (instead of ::sp ()'s 20000).
   */
  ret = taskSpawn (argv[0],    /* task name */
                   100,        /* task priority */
                   VX_FP_TASK, /* task options */
                   PTHREAD_DEFAULT_STACK_SIZE, /* stack size */
                   entry,      /* entry point */
                   argc,       /* first argument to main () */
                   (int) argv, /* second argument to main () */
                   0, 0, 0, 0, 0, 0, 0, 0);
  va_end (pvar);

  /* taskSpawn () returns the taskID on success: return 0 instead if
   * successful
   */
  return ret > 0 ? 0 : ret;
}

int
pace_main(int argc, char* argv[])
{
  /* Setup information for VxWorks emulation */ 
  if (pacevx_vxworks_init() == ERROR)
    return ERROR;

  /* Call the "normal" main function now that we've done
   * our bookkeeping.
   */
  return post_pace_main(argc, argv);
}
