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

// Pass an IDL file through the C preprocessor

#include "idl_bool.h"
#include "idl_defines.h"
#include "global_extern.h"
#include "fe_extern.h"
#include "drv_extern.h"
#include "utl_string.h"
#include "ace/Version.h"
#include "ace/Process_Manager.h"
#include "ace/SString.h"
#include "ace/Env_Value_T.h"
#include "ace/ARGV.h"

ACE_RCSID (driver, 
           drv_preproc, 
           "$Id$")

static long argcount = 0;
static long max_argcount = 128;
static const char *arglist[128];
static const char *output_arg_format = 0;
static long output_arg_index = 0;

// Push the new CPP location if we got a -Yp argument.
void
DRV_cpp_new_location (const char *new_loc)
{
  arglist[0] = new_loc;
}

// Push an argument into the arglist.
void
DRV_cpp_putarg (const char *str)
{
  if (argcount >= max_argcount)
    {
      ACE_ERROR ((LM_ERROR,
                  "%s%s %d %s\n",
                  idl_global->prog_name (),
                  ": More than",
                  max_argcount,
                  "arguments to preprocessor"));

      ACE_OS::exit (99);
    }

  arglist[argcount++] = ACE::strnew (str);
}

// Expand the output argument with the given filename.
void
DRV_cpp_expand_output_arg (const char *filename)
{
  if (output_arg_format != 0)
    {
      delete [] ACE_const_cast (char *, arglist[output_arg_index]);
      arglist[output_arg_index] = 0;

      char *output_arg = 0;
      ACE_NEW (output_arg,
               char [ACE_OS::strlen (output_arg_format)
                     + ACE_OS::strlen (filename)
                     + 1]);

      ACE_OS::sprintf (output_arg,
                       output_arg_format,
                       filename);

      arglist[output_arg_index] = output_arg;
    }
}

// Lines can be 1024 chars long.
#define LINEBUF_SIZE 1024
static char drv_line[LINEBUF_SIZE + 1];

// Get a line from stdin.
static long
DRV_get_line (FILE *f)
{
    char *l = fgets (drv_line,
                     LINEBUF_SIZE,
                     f);
    size_t i = 0;

    if (l == 0)
      {
        return I_FALSE;
      }

    if (*l == '\0' && feof (f))
      {
        return I_FALSE;
      }

    if (*l == '\0')
      {
        return I_TRUE;
      }

    i = strlen(l) - 1;

    if (l[i] == '\n')
      {
        l[i] = '\0';
      }

    return I_TRUE;
}

// Store include paths from the environment variable, if any.
void
DRV_store_env_include_paths (void)
{
  ACE_Env_Value<char*> incl_paths ("INCLUDE",
                                   (char *) 0);
  const char *aggr_str = incl_paths;

  if (aggr_str != 0)
    {
      char separator;
#if defined (ACE_WIN32)
      separator = ';';
#else
      separator = ':';
#endif
      ACE_CString aggr_cstr (aggr_str);
      ssize_t pos;

      do 
        {
          pos = aggr_cstr.find (separator);
          idl_global->add_include_path (aggr_cstr.substr (0, pos).fast_rep ());
          aggr_cstr = aggr_cstr.substr (pos + 1);
        } while (pos != ACE_String_Base_Const::npos);
    }
}

// Initialize the cpp argument list.
void
DRV_cpp_init (void)
{
  const char *cpp_loc, *cpp_args;

  // See if TAO_IDL_PREPROCESSOR is defined.
  ACE_Env_Value<char*> preprocessor ("TAO_IDL_PREPROCESSOR", 
                                     (char *) 0);

  // Set cpp_loc to the built in location, unless it has been overriden by
  // environment variables.
  if (preprocessor != 0)
    {
      cpp_loc = preprocessor;
    }
  else
    {
      // Check for the deprecated CPP_LOCATION environment variable
      ACE_Env_Value<char*> cpp_path ("CPP_LOCATION",
                                     (char *) 0);

      if (cpp_path != 0)
        {
          ACE_ERROR ((LM_ERROR,
                      "Warning: The environment variable "
                      "CPP_LOCATION has been deprecated.\n"
                      "         Please use TAO_IDL_PREPROCESSOR "
                      "instead.\n"));

          cpp_loc = cpp_path;
        }
      else
        {
          cpp_loc = idl_global->cpp_location ();
        }
    }

  DRV_cpp_putarg (cpp_loc);

  // Add an option to the IDL compiler to make the TAO version
  // available to the user. A XX.YY.ZZ release gets version 0xXXYYZZ,
  // for example, 5.1.14 gets 0x050114.
  char version_option[128];
  ACE_OS::sprintf (version_option,
                   "-D__TAO_IDL=0x%2.2d%2.2d%2.2d",
                   ACE_MAJOR_VERSION,
                   ACE_MINOR_VERSION,
                   ACE_BETA_VERSION);
  DRV_cpp_putarg (version_option);

  DRV_cpp_putarg ("-I.");

  // Added some customizable preprocessor options
  ACE_Env_Value<char*> args1 ("TAO_IDL_PREPROCESSOR_ARGS",
                              (char *) 0);

  if (args1 != 0)
    {
      cpp_args = args1;
    }
  else
    {
      // Check for the deprecated TAO_IDL_DEFAULT_CPP_FLAGS environment 
      // variable.
      ACE_Env_Value<char*> args2 ("TAO_IDL_DEFAULT_CPP_FLAGS",
                                  (char *) 0);

      if (args2 != 0)
        {
          ACE_ERROR ((LM_ERROR,
                      "Warning: The environment variable "
                      "TAO_IDL_DEFAULT_CPP_FLAGS has been "
                      "deprecated.\n"
                      "         Please use "
                      "TAO_IDL_PREPROCESSOR_ARGS instead.\n"));

          cpp_args = args2;
        }
      else
        {
          // If no cpp flag was defined by the user, we define some
          // platform specific flags here.
          char option[BUFSIZ];

#if defined (TAO_IDL_PREPROCESSOR_ARGS)
          cpp_args = TAO_IDL_PREPROCESSOR_ARGS;
#elif defined (ACE_CC_PREPROCESSOR_ARGS)
          cpp_args = ACE_CC_PREPROCESSOR_ARGS;
#else
          cpp_args = "-E";
#endif /* TAO_IDL_PREPROCESSOR_ARGS */

          // So we can find OMG IDL files, such as `orb.idl'.
          ACE_OS::strcpy (option, "-I");

#if defined (TAO_IDL_INCLUDE_DIR)
          // TAO_IDL_INCLUDE_DIR should be in quotes,
          // e.g. "/usr/local/include/tao"

          ACE_OS::strcat (option, 
                          TAO_IDL_INCLUDE_DIR);
#else
          char* TAO_ROOT = ACE_OS::getenv ("TAO_ROOT");
          size_t len = 0;

          if (TAO_ROOT != 0)
            {
              len = ACE_OS::strlen (TAO_ROOT);

              // Some compilers choke on "//" separators.
              if (TAO_ROOT[len - 1] == '/')
                {
                  TAO_ROOT[len - 1] = '\0';
                }

              ACE_OS::strcat (option, TAO_ROOT);
              ACE_OS::strcat (option, "/tao");
            }
          else
            {
              char* ACE_ROOT = ACE_OS::getenv ("ACE_ROOT");

              if (ACE_ROOT != 0)
                {
                  len = ACE_OS::strlen (ACE_ROOT);

                  // Some compilers choke on "//" separators.
                  if (ACE_ROOT[len - 1] == '/')
                    {
                      ACE_ROOT[len - 1] = '\0';
                    }

                  ACE_OS::strcat (option, ACE_ROOT);
                  ACE_OS::strcat (option, "/TAO/tao");
                }
              else
                {
                  ACE_ERROR ((
                      LM_ERROR,
                      "Note: The environment variables "
                      "TAO_ROOT and ACE_ROOT are not defined.\n"
                      "      TAO_IDL may not be able to "
                      "locate orb.idl\n"
                    ));

                  ACE_OS::strcat (option, ".");
                }
            }
#endif  /* TAO_IDL_INCLUDE_DIR */

          DRV_cpp_putarg (option);
        }
    }

  // Add any flags in cpp_args to cpp's arglist.
  ACE_ARGV arglist (cpp_args);

  for (size_t arg_cnt = 0; arg_cnt < (size_t)arglist.argc (); ++arg_cnt)
    {
      // Check for an argument that specifies the preprocessor's output file.
      if (ACE_OS::strstr (arglist[arg_cnt], "%s") != 0 
          && output_arg_format == 0)
        {
          output_arg_format = ACE::strnew (arglist[arg_cnt]);
          output_arg_index = argcount;
          DRV_cpp_putarg (0);
        }
      else
        {
          DRV_cpp_putarg (arglist[arg_cnt]);
        }
    }
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
    {
      return;
    }
  else
    {
      r++;
    }

  // Skip the tabs and spaces.
  while (*r == ' ' || *r == '\t')
    {
      ++r;
    }

  // Probably we are at the word `include`. If not return.
  if (*r != 'i')
    {
      return;
    }

  // Check whether this word is `include` or no.
  const char* include_str = "include";

  for (size_t ii = 0;
       ii < strlen ("include") && *r != '\0' && *r != ' ' && *r != '\t';
       ++r, ++ii)
    {
      // Return if it doesn't match.
      if (include_str [ii] != *r)
        {
          return;
        }
    }

  // Next thing is finding the file that has been `#include'd. Skip
  // all the blanks and tabs and reach the startng " or < character.
  for (; (*r != '"') && (*r != '<'); ++r)
    {
      if (*r == '\n' || *r == '\0')
        {
          return;
        }
    }

  // Decide on the end char.
  char end_char = '"';

  if (*r == '<')
    {
      end_char = '>';
    }

  // Skip this " or <.
  ++r;

  // Store this position.
  h = r;

  // Found this in idl.ll. Decides the file to be standard input.
  if (*h == '\0')
    {
      return;
    }

  // Find the closing " or < character.
  for (; *r != end_char; ++r)
    {
      continue;
    }

  // Make a new string for this file name.
  char* file_name = 0;
  ACE_NEW (file_name,
           char [r - h + 1]);

  // Copy the char's.
  size_t fi = 0;

  for (; h != r; ++fi, ++h)
    {
      file_name [fi] = *h;
    }

  // Terminate the string.
  file_name [fi] = '\0';

  // Put Microsoft-style pathnames into a canonical form.
  size_t i = 0;

  for (size_t j = 0; file_name [j] != '\0'; ++i, ++j)
    {
      if (file_name [j] == '\\' && file_name [j + 1] == '\\')
        {
          j++;
        }

      file_name [i] = file_name [j];
    }

  // Terminate this string.
  file_name [i] = '\0';

  // Store in the idl_global, unless it's "orb.idl" -
  // we don't want to generate header includes for that.
  if (ACE_OS::strcmp (file_name, "orb.idl"))
    {
      idl_global->add_to_included_idl_files (file_name);
    }
}

// Copy from stdin to a file
static void
DRV_copy_input (FILE *fin,
                char *fn,
                const char *orig_filename)
{
  FILE  *f = ACE_OS::fopen (fn, "w");

  if (f == 0)
    {
      ACE_ERROR ((LM_ERROR,
                  "%s%s%s%s",
                  idl_global->prog_name (),
                  ": cannot open temp file ",
                  fn,
                  " for writing\n"));

      ACE_OS::exit (99);
    }

  if (fin == 0)
    {
      ACE_ERROR ((LM_ERROR,
                  "%s%s",
                  idl_global->prog_name (),
                  ": cannot open input file\n"));

      ACE_OS::exit (99);
    }

#if !defined (ACE_WIN32)
  fprintf (f,
           "#line 1 \"%s\"\n",
           orig_filename);
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
  ACE_OS::fprintf (f,
                   "#line 1 \"%s\"\n",
                   buf);
#endif /* ! ACE_WIN32 */

  while (DRV_get_line (fin))
    {
      // We really need to know whether this line is a "#include
      // ...". If so, we would like to separate the "file name" and
      // keep that in the idl_global. We need them to produce
      // "#include's in the stubs and skeletons.
      DRV_check_for_include (drv_line);

      // Print the line to the temp file.
      ACE_OS::fprintf (f,
                       "%s\n",
                       drv_line);
    }

  // Close the temp file.
  ACE_OS::fclose (f);
}

// Strip down a name to the last component,
// i.e. everything after the last '/' or '\' character.
static char *
DRV_stripped_name (char *fn)
{
    char *n = fn;
    size_t l;

    if (n == 0)
      {
        return 0;
      }

    l = strlen (n);
    int slash_found = 0;

    for (n += l - 1; n >= fn && !slash_found; n--)
      {
        slash_found = (*n == '/' || *n == '\\');
      }

    n += 1;

    if (slash_found)
      {
        n += 1;
      }

    return n;
}

// File names.
static char     tmp_file[128];
static char     tmp_ifile[128];

// Pass input through preprocessor.
void
DRV_pre_proc (const char *myfile)
{
  long  readfromstdin = I_FALSE;

  // Macro to avoid "warning: unused parameter" type warning.
  ACE_UNUSED_ARG (readfromstdin);

  const char* tmpdir = idl_global->temp_dir ();

  ACE_OS::strcpy (tmp_file, 
                  tmpdir);
  ACE_OS::strcpy (tmp_ifile, 
                  tmpdir);

  ACE_OS::strcat (tmp_file, 
                  "idlf_XXXXXX");
  ACE_OS::strcat (tmp_ifile, 
                  "idli_XXXXXX");

  (void) ACE_OS::mktemp (tmp_file); 
  ACE_OS::strcat (tmp_file, 
                  ".cc");
  (void) ACE_OS::mktemp (tmp_ifile); 
  ACE_OS::strcat (tmp_ifile, 
                  ".cc");

  UTL_String *tmp = 0;

  if (strcmp (myfile, "standard input") == 0)
    {
      ACE_NEW (tmp,
               UTL_String (tmp_ifile));
      idl_global->set_main_filename (tmp);

      ACE_NEW (tmp,
               UTL_String (DRV_stripped_name (tmp_ifile)));
      idl_global->set_stripped_filename (tmp);

      ACE_NEW (tmp,
               UTL_String (tmp_ifile));
      idl_global->set_real_filename (tmp);

      DRV_copy_input (stdin,
                      tmp_ifile,
                      "standard input");

      idl_global->set_read_from_stdin (I_TRUE);
    }
  else
    {
      FILE *fd = ACE_OS::fopen (myfile, "r");
      DRV_copy_input (fd,
                      tmp_ifile,
                      myfile);
      ACE_OS::fclose (fd);

      idl_global->set_read_from_stdin (I_FALSE);

      ACE_NEW (tmp,
               UTL_String (myfile));
      idl_global->set_main_filename (tmp);

      ACE_Auto_String_Free safety (ACE_OS::strdup (myfile));
      ACE_NEW (tmp,
               UTL_String (DRV_stripped_name (safety.get ())));
      idl_global->set_stripped_filename (tmp);

      ACE_NEW (tmp,
               UTL_String (tmp_ifile));
      idl_global->set_real_filename (tmp);
    }

  // We use ACE instead of the (low level) fork facilities, this also
  // works on NT.
  ACE_Process process;

  // For complex builds, the default command line buffer size of 1024
  // is sometimes not enough. We use 4096 here.
  ACE_Process_Options cpp_options (1,       // Inherit environment.
                                   TAO_IDL_COMMAND_LINE_BUFFER_SIZE);

  DRV_cpp_expand_output_arg (tmp_file);
  DRV_cpp_putarg (tmp_ifile);
  DRV_cpp_putarg (0); // Null terminate the arglist.

  cpp_options.command_line (arglist);

  /// Remove any existing output file.
  (void) ACE_OS::unlink (tmp_file);

  ACE_HANDLE fd = ACE_INVALID_HANDLE;

  if (output_arg_format == 0)
    {
      // If the following open() fails, then we're either being hit with a
      // symbolic link attack, or another process opened the file before
      // us.
      fd = ACE_OS::open (tmp_file,
                         O_WRONLY | O_CREAT | O_EXCL,
                         ACE_DEFAULT_FILE_PERMS);

      if (fd == ACE_INVALID_HANDLE)
        {
          ACE_ERROR ((LM_ERROR,
                      "%s%s%s%s",
                      idl_global->prog_name (),
                      ": cannot open temp file ",
                      tmp_file,
                      " for writing\n"));

          return;
        }

      cpp_options.set_handles (ACE_INVALID_HANDLE, fd);
    }

  if (process.spawn (cpp_options) == ACE_INVALID_PID)
    {
      ACE_ERROR ((LM_ERROR,
                  "%s%s%s%s",
                  idl_global->prog_name (),
                  ": spawn of ",
                  arglist[0],
                  " failed\n"));

      return;
    }

  if (fd != ACE_INVALID_HANDLE)
    {
      // Close the output file on the parent process.
      if (ACE_OS::close (fd) == -1)
        {
          ACE_ERROR ((LM_ERROR,
                      "%s%s%s%s",
                      idl_global->prog_name (),
                      ": cannot close temp file",
                      tmp_file,
                      " on parent\n"));

          return;
        }
    }

  // Remove the null termination and the input file from the arglist,
  // the next file will the previous args.
  argcount -= 2;

  ACE_exitcode status = 0;

  if (process.wait (&status) == ACE_INVALID_PID)
    {
      ACE_ERROR ((LM_ERROR,
                  "%s%s",
                  idl_global->prog_name (),
                  ": wait for child process failed\n"));

      return;
    }

  if (WIFEXITED ((status)))
    {
      // child terminated normally?
      if (WEXITSTATUS ((status)) != 0)
        {
          errno = WEXITSTATUS ((status));

          ACE_ERROR ((LM_ERROR,
                      "%s%s%s%s",
                      idl_global->prog_name (),
                      ": preprocessor ",
                      arglist[0],
                      " returned with an error\n"));

          ACE_OS::exit (1);
        }
    }
  else
    {
      // child didn't call exit(); perhaps it received a signal?
      errno = EINTR;

      ACE_ERROR ((LM_ERROR,
                  "%s%s%s%s",
                  idl_global->prog_name (),
                  ": preprocessor ",
                  arglist[0],
                  " appears to have been interrupted\n"));

      ACE_OS::exit (1);
    }
  // TODO: Manage problems in the pre-processor, in the previous
  // version the current process would exit if the pre-processor
  // returned with error.

  FILE *yyin = ACE_OS::fopen (tmp_file, "r");

  if (yyin == NULL)
    {
      ACE_ERROR ((LM_ERROR,
                  "%s%s %s\n",
                  idl_global->prog_name (),
                  ": Could not open cpp output file",
                  tmp_file));

      ACE_OS::exit (99);
    }

  FE_set_yyin (ACE_reinterpret_cast (File *, yyin));

  if (idl_global->compile_flags () & IDL_CF_ONLY_PREPROC)
    {
      FILE *preproc = ACE_OS::fopen (tmp_file, "r");
      char buffer[ACE_MAXLOGMSGLEN];
      size_t bytes;

      if (preproc == 0)
        {
          ACE_ERROR ((LM_ERROR,
                      "%s: Could not open cpp output file: %s\n",
                      idl_global->prog_name (),
                      tmp_file));

          ACE_OS::exit (99);
        }

      // ACE_DEBUG sends to stderr - we want stdout for this dump
      // of the preprocessor output. So we modify the singleton that
      // was created in this process. Since IDL_CF_ONLY_PREPROC causes
      // an (almost) immediate exit below, we don't have to restore
      // the singleton's default parameters.
      ACE_Log_Msg *out = ACE_Log_Msg::instance ();
      out->msg_ostream (&cout);
      out->clr_flags (ACE_Log_Msg::STDERR);
      out->set_flags (ACE_Log_Msg::OSTREAM);

      while ((bytes = ACE_OS::fread (buffer,
                                     sizeof (char),
                                     ACE_MAXLOGMSGLEN - 1,
                                     preproc))
          != 0)
        {
          buffer[bytes] = 0;

          ACE_DEBUG ((LM_DEBUG,
                      buffer));
        }

      ACE_OS::fclose (preproc);
    }

  if (ACE_OS::unlink (tmp_ifile) == -1)
    {
      ACE_ERROR ((LM_ERROR,
                  "%s%s %s\n",
                  idl_global->prog_name (),
                  ": Could not remove cpp input file",
                  tmp_ifile));

      ACE_OS::exit (99);
    }

#if !defined (ACE_WIN32) || defined (ACE_HAS_WINNT4) && (ACE_HAS_WINNT4 != 0)
  if (ACE_OS::unlink (tmp_file) == -1)
    {
      ACE_ERROR ((LM_ERROR,
                  "%s%s %s\n",
                  idl_global->prog_name (),
                  ": Could not remove cpp output file",
                  tmp_file));

      ACE_OS::exit (99);
    }
#endif /* ACE_HAS_WINNT4 && ACE_HAS_WINNT4 != 0 */

  if (idl_global->compile_flags () & IDL_CF_ONLY_PREPROC)
    {
      ACE_OS::exit (0);
    }
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
  template class ACE_Env_Value<char*>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
# pragma instantiate  ACE_Env_Value<char*>
#endif
