/*  $Id$  */
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

/*
 * DRV_pre_proc.cc - pass an IDL file through the C preprocessor
 */

#include "idl.h"
#include "idl_extern.h"

#include "drv_private.h"
#include "drv_link.h"

#include "ace/Process_Manager.h"
#include "ace/Env_Value_T.h"

#undef  MAX_ARGLIST
#define MAX_ARGLIST     128

static  char    *arglist[MAX_ARGLIST];
static  long    argcount = 0;

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

  char *cpp_loc;

  // DRV_cpp_putarg("\\cygnus\\H-i386-cygwin32\\bin\\echo");
  ACE_Env_Value<char*> cpp_path ("CPP_LOCATION", (char *) 0);

  if (cpp_path != 0)
      cpp_loc = cpp_path;
  else
      cpp_loc = idl_global->cpp_location();
  DRV_cpp_putarg(cpp_loc);
  DRV_cpp_putarg("-E");
  DRV_cpp_putarg("-DIDL");
  DRV_cpp_putarg("-I.");
}

/*
 * lines can be 1024 chars long
 */
#define LINEBUF_SIZE    1024
static  char    drv_line[LINEBUF_SIZE + 1];

/*
 * Get a line from stdin
 */
static long
DRV_get_line(FILE *f)
{
    char        *l = fgets(drv_line, LINEBUF_SIZE, f);
    long        i;

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
  FILE  *f = fopen(fn, "w");

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
    char        *n = fn;
    long        l;

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
static char     tmp_file[128];
static char     tmp_ifile[128];

/*
 * Pass input through preprocessor
 */
void
DRV_pre_proc(char *myfile)
{
  long  readfromstdin = I_FALSE;
  char  catbuf[512];

  // Macro to avoid "warning: unused parameter" type warning.
  ACE_UNUSED_ARG (readfromstdin);

  const char* tmpdir = getenv("TMP");
  if (tmpdir != 0)
    {
      ACE_OS::strcpy (tmp_file, tmpdir);
      ACE_OS::strcpy (tmp_ifile, tmpdir);
    }
  else
    {
      ACE_OS::strcpy (tmp_file, ACE_DIRECTORY_SEPARATOR_STR_A);
      ACE_OS::strcat (tmp_file, "tmp");
      ACE_OS::strcpy (tmp_ifile, ACE_DIRECTORY_SEPARATOR_STR_A);
      ACE_OS::strcat (tmp_ifile, "tmp");
    }

  ACE_OS::strcat (tmp_file, ACE_DIRECTORY_SEPARATOR_STR_A);
  ACE_OS::strcat (tmp_file, "idlf_XXXXXX");

  ACE_OS::strcat (tmp_ifile, ACE_DIRECTORY_SEPARATOR_STR_A);
  ACE_OS::strcat (tmp_ifile, "idli_XXXXXX");

  (void) ACE_OS::mktemp (tmp_file); ACE_OS::strcat (tmp_file, ".cc");
  (void) ACE_OS::mktemp (tmp_ifile); ACE_OS::strcat (tmp_ifile, ".cc");
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

  // We use ACE instead of the (low level) fork facilities, this also
  // work on NT.
  ACE_Process manager;
  ACE_Process_Options cpp_options;
  DRV_cpp_putarg (tmp_ifile);
  DRV_cpp_putarg (0); // Null terminate the arglist.
  cpp_options.command_line (arglist);
  ACE_HANDLE fd = ACE_OS::open(tmp_file, O_WRONLY | O_CREAT | O_TRUNC, 0777);
  if (fd == ACE_INVALID_HANDLE) {
    cerr << idl_global->prog_name()
         << GTDEVEL(": cannot open temp file ")
         << tmp_file << " for writing\n";
    return;
  }
  cpp_options.set_handles (ACE_INVALID_HANDLE, fd);
  if (manager.spawn (cpp_options) == -1)
    {
      cerr << idl_global->prog_name()
           << GTDEVEL(": spawn of ")
           << arglist[0]
           << GTDEVEL(" failed\n");
      return;
    }

  // Close the output file on the parent process.
  if (ACE_OS::close (fd) == -1)
    {
      cerr << idl_global->prog_name ()
           << GTDEVEL(": cannot close temp file")
           << tmp_file << " on parent\n";
      return;
    }

  // Remove the null termination and the input file from the arglist,
  // the next file will the previous args.
  argcount -= 2;

  if (manager.wait () == -1)
    {
      cerr << idl_global->prog_name ()
           << GTDEVEL(": wait for child process failed\n");
      return;
    }
  // TODO: Manage problems in the pre-processor, in the previous
  // version the current process would exit if the pre-processor
  // returned with error.


  FILE * yyin = fopen(tmp_file, "r");
  if (yyin == NULL) {
    cerr << idl_global->prog_name()
         << GTDEVEL(": Could not open cpp output file ")
         << tmp_file
         << "\n";
    exit(99);
  }
  (*DRV_FE_set_yyin)((File *) yyin);

  // @@ TODO: This is not protable, cat(1) is a UNIX tool.
  if (idl_global->compile_flags() & IDL_CF_ONLY_PREPROC) {
    sprintf(catbuf, "cat < %s", tmp_file);
    system(catbuf);
  }

  if (ACE_OS::unlink(tmp_ifile) == -1) {
    cerr << idl_global->prog_name()
         << GTDEVEL(": Could not remove cpp input file ")
         << tmp_ifile
         << "\n";
    exit(99);
  }
#if !defined (ACE_WIN32)
  // TODO: This unlink fails every time under NT, it seems that you
  // cannot remove an open file under that OS?
  if (ACE_OS::unlink(tmp_file) == -1) {
    cerr << idl_global->prog_name()
         << GTDEVEL(": Could not remove cpp output file ")
         << tmp_file
         << "\n";
    exit(99);
  }
#endif /* !ACE_WIN32 */
  if (idl_global->compile_flags() & IDL_CF_ONLY_PREPROC)
    exit(0);
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
  template class ACE_Env_Value<char*>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
# pragma instantiate  ACE_Env_Value<char*>
#endif
