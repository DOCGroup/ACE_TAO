/*

COPYRIGHT

Copyright 1992, 1993, 1994 Sun Microsystems, Inc.  Printed in the United
States of America.  All Rights Reserved.

This product is protected by copyright and distributed under the following
license restricting its use.

The Interface Definition Language Compiler Front End (CFE) is made
available for your use provided that you include this license and copyright
notice on all media and documentation and the software program in which
this product is incorporated in whole or part. You may copy and extend
functionality (but may not remove functionality) of the Interface
Definition Language CFE without charge, but you are not authorized to
license or distribute it to anyone else except as part of a product or
program developed by you or with the express written consent of Sun
Microsystems, Inc. ("Sun").

The names of Sun Microsystems, Inc. and any of its subsidiaries or
affiliates may not be used in advertising or publicity pertaining to
distribution of Interface Definition Language CFE as permitted herein.

This license is effective until terminated by Sun for failure to comply
with this license.  Upon termination, you shall destroy or return all code
and documentation for the Interface Definition Language CFE.

INTERFACE DEFINITION LANGUAGE CFE IS PROVIDED AS IS WITH NO WARRANTIES OF
ANY KIND INCLUDING THE WARRANTIES OF DESIGN, MERCHANTIBILITY AND FITNESS
FOR A PARTICULAR PURPOSE, NONINFRINGEMENT, OR ARISING FROM A COURSE OF
DEALING, USAGE OR TRADE PRACTICE.

INTERFACE DEFINITION LANGUAGE CFE IS PROVIDED WITH NO SUPPORT AND WITHOUT
ANY OBLIGATION ON THE PART OF Sun OR ANY OF ITS SUBSIDIARIES OR AFFILIATES
TO ASSIST IN ITS USE, CORRECTION, MODIFICATION OR ENHANCEMENT.

SUN OR ANY OF ITS SUBSIDIARIES OR AFFILIATES SHALL HAVE NO LIABILITY WITH
RESPECT TO THE INFRINGEMENT OF COPYRIGHTS, TRADE SECRETS OR ANY PATENTS BY
INTERFACE DEFINITION LANGUAGE CFE OR ANY PART THEREOF.

IN NO EVENT WILL SUN OR ANY OF ITS SUBSIDIARIES OR AFFILIATES BE LIABLE FOR
ANY LOST REVENUE OR PROFITS OR OTHER SPECIAL, INDIRECT AND CONSEQUENTIAL
DAMAGES, EVEN IF SUN HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.

Use, duplication, or disclosure by the government is subject to
restrictions as set forth in subparagraph (c)(1)(ii) of the Rights in
Technical Data and Computer Software clause at DFARS 252.227-7013 and FAR
52.227-19.

Sun, Sun Microsystems and the Sun logo are trademarks or registered
trademarks of Sun Microsystems, Inc.

SunSoft, Inc.  
2550 Garcia Avenue 
Mountain View, California  94043

NOTE:

SunOS, SunSoft, Sun, Solaris, Sun Microsystems or the Sun logo are
trademarks or registered trademarks of Sun Microsystems, Inc.

 */

#pragma ident "%@(#)drv_preproc.cc	1.16% %92/06/10% Sun Microsystems"

/*
 * DRV_pre_proc.cc - pass an IDL file through the C preprocessor
 */

#include	"idl.h"
#include	"idl_extern.h"

#include	"drv_private.h"
#include	"drv_link.h"

#include	<stdio.h>
#include	<fcntl.h>

#ifdef		SOLARIS2

#include	<unistd.h>		// POSIX standard types
#include	<wait.h>		// POSIX definition of wait()

#endif		// SOLARIS2

#ifdef		apollo
#include	<sysent.h>
#endif		// apollo

#if defined(hpux) || defined(__hpux) || defined(SUNOS4)
#include	<unistd.h>		// POSIX definitions
#include	<sys/wait.h>		// POSIX definition of wait()
#endif		// defined(hpux) || defined(__hpux) || defined(SUNOS4)

#undef	MAX_ARGLIST
#define	MAX_ARGLIST	128

static	char	*arglist[MAX_ARGLIST];
static	long	argcount = 0;

/*
 * Push the new CPP location if we got a -Yp argument
 */
void
DRV_cpp_new_location(char *new_loc)
{
  arglist[0] = new_loc;
}

/*
 * Push an argument into the arglist
 */
void
DRV_cpp_putarg(char *str)
{
  if (argcount >= MAX_ARGLIST) {
    cerr << idl_global->prog_name()
         << GTDEVEL(": More than ")
	 << MAX_ARGLIST
	 << GTDEVEL(" arguments to preprocessor\n");
    exit (99);
  }
  arglist[argcount++] = str;
}

/*
 * Initialize the cpp argument list
 */
void
DRV_cpp_init()
{
  DRV_cpp_putarg(idl_global->cpp_location());
  DRV_cpp_putarg("-E");
  DRV_cpp_putarg("-DIDL");
  DRV_cpp_putarg("-I.");
}

/*
 * lines can be 1024 chars long
 */
#define	LINEBUF_SIZE	1024
static	char	drv_line[LINEBUF_SIZE + 1];

/*
 * Get a line from stdin
 */
static long
DRV_get_line(FILE *f)
{
    char	*l = fgets(drv_line, LINEBUF_SIZE, f);
    long	i;

    if (l == NULL)
	return I_FALSE;
    if (*l == '\0' && feof(f))
	return I_FALSE;
    if (*l == '\0')
	return I_TRUE;
    i = strlen(l) - 1;
    if (l[i] == '\n')
	l[i] = '\0';
    return I_TRUE;
}

/*
 * Copy from stdin to a file
 */
static void
DRV_copy_input(FILE *fin, char *fn)
{
  FILE	*f = fopen(fn, "w");

  if (f == NULL) {
    cerr << idl_global->prog_name()
	 << GTDEVEL(": cannot open temp file ")
	 << fn
	 << GTDEVEL(" for writing\n");
    exit(99);
  }
  if (fin == NULL) {
      cerr << idl_global->prog_name()
           << GTDEVEL(": cannot open input file\n");
      exit(99);
  }
  while (DRV_get_line(fin))
    fprintf(f, "%s\n", drv_line);
  fclose(f);
}

/*
 * Strip down a name to the last component, i.e. everything after the last
 * '/' character
 */
static char *
DRV_stripped_name(char *fn)
{
    char	*n = fn;
    long	l;

    if (n == NULL)
	return NULL;
    l = strlen(n);
    for (n += l; l > 0 && *n != '/'; l--, n--);
    if (*n == '/') n++;
    return n;
}

/*
 * File names
 */
static char	tmp_file[128];
static char	tmp_ifile[128];

/*
 * Pass input through preprocessor
 */
void
DRV_pre_proc(char *myfile)
{
#if defined(apollo) || defined(SUNOS4)
  union wait wait_status;
#else
  int	wait_status;
#endif	// defined(apollo) || defined(SUNOS4)
  long	readfromstdin = I_FALSE;
  pid_t	child_pid;
  char	catbuf[512];
  
  strcpy(tmp_file, "/tmp/idlf_XXXXXX");
  strcpy(tmp_ifile, "/tmp/idli_XXXXXX");

  (void) mktemp(tmp_file); strcat(tmp_file, ".cc");
  (void) mktemp(tmp_ifile); strcat(tmp_ifile, ".cc");
  if (strcmp(myfile, "standard input") == 0) {
    idl_global->set_filename((*DRV_FE_new_UTL_String)(tmp_ifile));
    idl_global->set_main_filename((*DRV_FE_new_UTL_String)(tmp_ifile));
    idl_global->
        set_stripped_filename(
            (*DRV_FE_new_UTL_String)(DRV_stripped_name(tmp_ifile))
        );
    idl_global->set_real_filename((*DRV_FE_new_UTL_String)(tmp_ifile));
    DRV_copy_input(stdin, tmp_ifile);
    idl_global->set_read_from_stdin(I_TRUE);
  } else {
    FILE *fd = fopen(myfile, "r");
    DRV_copy_input(fd, tmp_ifile);
    fclose(fd);
    idl_global->set_read_from_stdin(I_FALSE);
    idl_global->set_filename((*DRV_FE_new_UTL_String)(myfile));
    idl_global->set_main_filename((*DRV_FE_new_UTL_String)(myfile));
    idl_global->
        set_stripped_filename(
            (*DRV_FE_new_UTL_String)(DRV_stripped_name(myfile))
        );
    idl_global->set_real_filename((*DRV_FE_new_UTL_String)(tmp_ifile));
  }
  switch (child_pid = fork()) {
  case 0:	/* Child - call cpp */
    DRV_cpp_putarg(tmp_ifile);
    {
      int fd = open(tmp_file, O_WRONLY | O_CREAT | O_TRUNC, 0777);
      if (fd < 0) {
        cerr << idl_global->prog_name()
    	  << GTDEVEL(": cannot open temp file ")
 	  << tmp_file << " for writing\n";
        exit(99);
      }
      int result = dup2(fd, 1);
      if (result < 0) {
        cerr << idl_global->prog_name()
    	  << GTDEVEL(": temp file ")
  	  << tmp_file << " dup error\n";
        exit(99);
      }
      close(fd);
    }
    execvp(arglist[0], arglist);
    cerr << idl_global->prog_name() 
         << GTDEVEL(": execvp of ")
	 << arglist[0]
	 << GTDEVEL(" failed\n");
    exit(99);
  case -1:
    cerr << idl_global->prog_name() << GTDEVEL(": fork failed\n");
    exit(99);
  default:	/* Parent - wait */
#if defined(hpux) || defined(__hpux)
    sleep(1);  // try to get around libc_r defect
#endif
    while (child_pid != wait(&wait_status));
#if defined(WIFEXITED) && defined(WEXITSTATUS)
    if (WIFEXITED(wait_status)) {
      if (WEXITSTATUS(wait_status) != 0) {
        cerr << idl_global->prog_name()
	     << GTDEVEL(": Preprocessor returned non-zero status ")
	     << (int) WEXITSTATUS(wait_status)
	     << "\n";
	unlink(tmp_ifile);
	unlink(tmp_file);
        exit(WEXITSTATUS(wait_status));
      }
    } else {
      // child terminated abnormally - wait_status is meaningless
      cerr << idl_global->prog_name()
	   << GTDEVEL(": Preprocessor terminated abnormally")
	   << "\n";
      unlink(tmp_ifile);
      unlink(tmp_file);
      exit(1);
    }
#else
#if defined(apollo) || defined(SUNOS4)
    if (wait_status.w_status != 0) {
#else
    if (wait_status != 0) {
#endif	// defined(apollo) || defined(SUNOS4)
      cerr << idl_global->prog_name()
	   << GTDEVEL(": Preprocessor returned non-zero status ")
#if defined(apollo) || defined(SUNOS4)
	   << wait_status.w_status
#else
	   << wait_status
#endif	// defined(apollo) || defined(SUNOS4)
	   << "\n";
      unlink(tmp_ifile);
      unlink(tmp_file);
#if defined(apollo) || defined(SUNOS4)
      exit(wait_status.w_status);
#else
      exit((int) wait_status);
#endif	// defined(apollo) || defined(SUNOS4)
    }
#endif  // defined(WIFEXITED) && defined(WEXITSTATUS)
  }
  FILE * yyin = fopen(tmp_file, "r");
  if (yyin == NULL) {
    cerr << idl_global->prog_name()
	 << GTDEVEL(": Could not open cpp output file ")
	 << tmp_file
	 << "\n";
    exit(99);
  }
  (*DRV_FE_set_yyin)((File *) yyin);
  if (idl_global->compile_flags() & IDL_CF_ONLY_PREPROC) {
    sprintf(catbuf, "cat < %s", tmp_file);
    system(catbuf);
  }
  if (unlink(tmp_ifile) != 0) {
    cerr << idl_global->prog_name()
         << GTDEVEL(": Could not remove cpp input file ")
	 << tmp_ifile
	 << "\n";
    exit(99);
  }
  if (unlink(tmp_file) != 0) {
    cerr << idl_global->prog_name()
	 << GTDEVEL(": Could not remove cpp output file ")
	 << tmp_file
	 << "\n";
    exit(99);
  }
  if (idl_global->compile_flags() & IDL_CF_ONLY_PREPROC)
    exit(0);
}
