// $Id$

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

ACE_RCSID(driver, drv_preproc, "$Id$")

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
  DRV_cpp_putarg (cpp_loc);
#if defined (ACE_WIN32)
  DRV_cpp_putarg ("-nologo");
#endif /* ACE_WIN32 */
  DRV_cpp_putarg ("-E");
  DRV_cpp_putarg("-DIDL");
  DRV_cpp_putarg ("-I.");
  DRV_cpp_putarg (ACE_OS::strcat (ACE_OS::strcat ("-I",
                                                  ACE_OS::getenv ("TAO_ROOT")),
                                  "/tao"));
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
DRV_copy_input(FILE *fin, char *fn, const char *orig_filename)
{
  FILE  *f = ACE_OS::fopen(fn, "w");

  if (f == NULL) {
    cerr << idl_global->prog_name()
         << GTDEVEL(": cannot open temp file ")
         << fn
         << GTDEVEL(" for writing\n");
    ACE_OS::exit (99);
  }
  if (fin == NULL) {
      cerr << idl_global->prog_name()
           << GTDEVEL(": cannot open input file\n");
      ACE_OS::exit (99);
  }
#if !defined (ACE_WIN32)
  fprintf (f, "#line 1 \"%s\"\n", orig_filename);
#else
  // Convert single \ into double \ otherwise MSVC++ pre-processor
  // gets awfully confused.
  char buf[2*MAXPATHLEN];
  char *d = buf;
  for (const char *s = orig_filename; *s != 0; ++s)
    {
      if (*s == '\\')
        {
          *d = '\\';
          d++;
        }
      *d = *s;
      d++;
    }
  *d = 0;
  ACE_OS::fprintf (f, "#line 1 \"%s\"\n", buf);
#endif /* ! ACE_WIN32 */

  while (DRV_get_line(fin))
    {
      // We really need to know whether this line is a "#include
      // ...". If so, we would like to separate the "file name" and
      // keep that in the idl_global. We need them to produce
      // "#include's in the stubs and skeletons.
      DRV_check_for_include (drv_line);

      // Print the line to the temp file.
      ACE_OS::fprintf (f, "%s\n", drv_line);
    }

  // Close the temp file.
  ACE_OS::fclose(f);
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

  const char* tmpdir = idl_global->temp_dir ();

  ACE_OS::strcpy (tmp_file, tmpdir);
  ACE_OS::strcpy (tmp_ifile, tmpdir);

  ACE_OS::strcat (tmp_file, "idlf_XXXXXX");
  ACE_OS::strcat (tmp_ifile, "idli_XXXXXX");

  (void) ACE_OS::mktemp (tmp_file); ACE_OS::strcat (tmp_file, ".cc");
  (void) ACE_OS::mktemp (tmp_ifile); ACE_OS::strcat (tmp_ifile, ".cc");

  if (strcmp(myfile, "standard input") == 0)
    {
      idl_global->set_filename((*DRV_FE_new_UTL_String)(tmp_ifile));
      idl_global->set_main_filename((*DRV_FE_new_UTL_String)(tmp_ifile));
      idl_global->set_stripped_filename((*DRV_FE_new_UTL_String)(DRV_stripped_name(tmp_ifile)));
      idl_global->set_real_filename((*DRV_FE_new_UTL_String)(tmp_ifile));
      DRV_copy_input(stdin, tmp_ifile, "standard input");
      idl_global->set_read_from_stdin(I_TRUE);
    }
  else
    {
      FILE *fd = fopen(myfile, "r");
      DRV_copy_input(fd, tmp_ifile, myfile);
      fclose(fd);
      idl_global->set_read_from_stdin(I_FALSE);
      idl_global->set_filename((*DRV_FE_new_UTL_String)(myfile));
      idl_global->set_main_filename((*DRV_FE_new_UTL_String)(myfile));
      idl_global->set_stripped_filename((*DRV_FE_new_UTL_String)(DRV_stripped_name(myfile)));
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

  int status = 0;
  if (manager.wait (&status) == -1)
    {
      cerr << idl_global->prog_name ()
           << GTDEVEL(": wait for child process failed\n");
      return;
    }
  if (status != 0)
    {
      cerr << idl_global->prog_name()
           << GTDEVEL(": preprocessor ")
           << arglist[0]
           << GTDEVEL(" returned with an error\n");
      ACE_OS::exit (1);
    }
  // TODO: Manage problems in the pre-processor, in the previous
  // version the current process would exit if the pre-processor
  // returned with error.

  FILE * yyin = ACE_OS::fopen(tmp_file, "r");
  if (yyin == NULL) {
    cerr << idl_global->prog_name()
         << GTDEVEL(": Could not open cpp output file ")
         << tmp_file
         << "\n";
    ACE_OS::exit (99);
  }
  (*DRV_FE_set_yyin)(ACE_reinterpret_cast(File *, yyin));

  // @@ TODO: This is not portable, cat(1) is a UNIX tool.
  if (idl_global->compile_flags() & IDL_CF_ONLY_PREPROC) {
    sprintf(catbuf, "cat < %s", tmp_file);
    system(catbuf);
  }

  if (ACE_OS::unlink(tmp_ifile) == -1) {
    cerr << idl_global->prog_name()
         << GTDEVEL(": Could not remove cpp input file ")
         << tmp_ifile
         << "\n";
    ACE_OS::exit (99);
  }

#if !defined (ACE_WIN32) || defined (ACE_HAS_WINNT4) && (ACE_HAS_WINNT4 != 0)
  if (ACE_OS::unlink(tmp_file) == -1) {
    cerr << idl_global->prog_name()
         << GTDEVEL(": Could not remove cpp output file ")
         << tmp_file
         << "\n";
    ACE_OS::exit (99);
  }
#endif /* ACE_HAS_WINNT4 && ACE_HAS_WINNT4 != 0 */

  if (idl_global->compile_flags() & IDL_CF_ONLY_PREPROC)
    ACE_OS::exit (0);
}

// We really need to know whether this line is a "#include ...". If
// so, we would like to separate the "file name" and keep that in the
// idl_global. We need them to produce "#include's in the stubs and
// skeletons.
void
DRV_check_for_include (const char* buf)
{
  const char* r = buf;
  const char* h;

  // Skip initial '#'.
  if (*r != '#')
    return;
  else
    r++;

  // Skip the tabs and spaces.
  while (*r == ' ' || *r == '\t')
    r++;

  // Probably we are at the word `include`. If not return.
  if (*r != 'i')
    return;

  // Check whether this word is `include` or no.
  char* include_str = "include";
  for (size_t ii = 0; ii < strlen ("include") && *r != '\0' && *r != ' ' && *r != '\t'; r++, ii++)
    // Return if it doesn't match.
    if (include_str [ii] != *r)
      return;

  // Next thing is finding the file that has been `#include'd. Skip
  // all the blanks and tabs and reach the startng " or < character.
  for (; (*r != '"') && (*r != '<'); r++)
    if (*r == '\n' || *r == '\0')
      return;

  // Decide on the end char.
  char end_char = '"';
  if (*r == '<')
    end_char = '>';

  // Skip this " or <.
  r++;

  // Store this position.
  h = r;

  // Found this in idl.ll. Decides the file to be standard input.
  if (*h == '\0')
    return;

  // Find the closing " or < character.
  for (; *r != end_char; r++)
    continue;

  // Make a new string for this file name.
  char* file_name = 0;
  ACE_NEW (file_name,
           char [r - h + 1]);

  // Copy the char's.
  size_t fi = 0;
  for (; h != r; fi++, h++)
    file_name [fi] = *h;

  // Terminate the string.
  file_name [fi] = '\0';

  // Put Microsoft-style pathnames into a canonical form.
  size_t i = 0;
  for (size_t j = 0; file_name [j] != '\0'; i++, j++)
    {
      if (file_name [j] == '\\' && file_name [j + 1] == '\\')
        j++;

      file_name [i] = file_name [j];
    }

  // Terminate this string.
  file_name [i] = '\0';

  // Store in the idl_global, unless it's "orb.idl" -
  // we don't want to generate header includes for that.
  if (ACE_OS::strcmp (file_name, "orb.idl"))
    idl_global->add_to_included_idl_files (file_name);
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
  template class ACE_Env_Value<char*>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
# pragma instantiate  ACE_Env_Value<char*>
#endif
