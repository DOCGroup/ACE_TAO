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
#include "ace/UUID.h"
#include "ace/Dirent.h"
#include "ace/OS_NS_sys_stat.h"

// FUZZ: disable check_for_streams_include
#include "ace/streams.h"

#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_unistd.h"
#include "ace/OS_NS_fcntl.h"

ACE_RCSID (driver,
           drv_preproc,
           "$Id$")

static long max_argcount = 128;
static long argcount = 0;
static const char *arglist[128];
static const char *output_arg_format = 0;
static long output_arg_index = 0;

const char *DIR_DOT = ".";
const char *DIR_DOT_DOT = "..";

// File names.
static char tmp_file [MAXPATHLEN + 1] = { 0 };
static char tmp_ifile[MAXPATHLEN + 1] = { 0 };

// Lines can be 1024 chars long.
#define LINEBUF_SIZE 1024
static char drv_line[LINEBUF_SIZE + 1];

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
      delete [] const_cast<char *> (arglist[output_arg_index]);
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
        return false;
      }

    if (*l == '\0' && feof (f))
      {
        return false;
      }

    if (*l == '\0')
      {
        return true;
      }

    i = strlen(l) - 1;

    if (l[i] == '\n')
      {
        l[i] = '\0';
      }

    return true;
}

// Initialize the cpp argument list.
void
DRV_cpp_init (void)
{
  const char *cpp_loc = FE_get_cpp_loc_from_env ();
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
  const char *cpp_args = FE_get_cpp_args_from_env ();

  if (cpp_args == 0)
    {
      // If no cpp flag was defined by the user, we define some
      // platform specific flags here.
      char option1[BUFSIZ];
      char option2[BUFSIZ];

#if defined (TAO_IDL_PREPROCESSOR_ARGS)
      cpp_args = TAO_IDL_PREPROCESSOR_ARGS;
#elif defined (ACE_CC_PREPROCESSOR_ARGS)
      cpp_args = ACE_CC_PREPROCESSOR_ARGS;
#else
      cpp_args = "-E";
#endif /* TAO_IDL_PREPROCESSOR_ARGS */

      // So we can find OMG IDL files, such as `orb.idl'.

      ACE_OS::strcpy (option1, "-I");
      ACE_OS::strcpy (option2, "-I");
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

          ACE_OS::strcat (option1, TAO_ROOT);
          ACE_OS::strcat (option2, TAO_ROOT);
#if defined (ACE_WIN32)
          ACE_OS::strcat (option2, "\\tao");
#else
          ACE_OS::strcat (option2, "/tao");
#endif
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

              ACE_OS::strcat (option1, ACE_ROOT);
              ACE_OS::strcat (option2, ACE_ROOT);
#if defined (ACE_WIN32)
              ACE_OS::strcat (option1, "\\TAO");
              ACE_OS::strcat (option2, "\\TAO\\tao");
#else
              ACE_OS::strcat (option1, "/TAO");
              ACE_OS::strcat (option2, "/TAO/tao");
#endif
            }
          else
            {
#if defined (TAO_IDL_INCLUDE_DIR)
              // TAO_IDL_INCLUDE_DIR should be in quotes,
              // e.g. "/usr/local/include/tao"
              ACE_OS::strcat (option1, TAO_IDL_INCLUDE_DIR);
#else
              ACE_ERROR ((LM_WARNING,
                          "NOTE: The environment variables "
                          "TAO_ROOT and ACE_ROOT are not defined.\n"
                          "      TAO_IDL may not be able to "
                          "locate orb.idl\n"));

              ACE_OS::strcat (option1, ".");
#endif  /* TAO_IDL_INCLUDE_DIR */
            }
        }

      DRV_cpp_putarg (option1);
      idl_global->add_include_path (ACE_CString (option1 + 2).c_str ());
      idl_global->tao_root (option1 + 2);
#if !defined (TAO_IDL_INCLUDE_DIR)
      DRV_cpp_putarg (option2);
      idl_global->add_include_path (ACE_CString (option2 + 2).c_str ());
#endif  /* TAO_IDL_INCLUDE_DIR */
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

int
DRV_sweep_dirs (const char *rel_path,
                const char *base_path)
{
  // Zero rel_path means we're not using this option, and
  // so we become a no-op.
  if (rel_path == 0)
    {
      return 0;
    }

  if (ACE_OS::chdir (rel_path) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "DRV_sweep_dirs: chdir %s failed\n",
                         rel_path),
                        -1);
    }

  ACE_Dirent dir (DIR_DOT);
  ACE_CString bname (base_path);
  bname += (bname.length () > 0 ? "/" : "");
  bname += rel_path;
  bool include_added = false;

  for (dirent *dir_entry; (dir_entry = dir.read ()) != 0;)
    {
      // Skip the ".." and "." files in each directory.
      if (ACE_OS::strcmp (dir_entry->d_name, DIR_DOT) == 0
          || ACE_OS::strcmp (dir_entry->d_name, DIR_DOT_DOT) == 0)
        {
          continue;
        }

      ACE_CString lname (dir_entry->d_name);
      ACE_stat stat_buf;

      if (ACE_OS::lstat (lname.c_str (), &stat_buf) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "DRV_sweep_dirs: ACE_OS::lstat (%s) failed\n",
                             lname.c_str ()),
                            -1);
        }

      size_t len = 0;

      switch (stat_buf.st_mode & S_IFMT)
        {
        case S_IFREG: // Either a regular file or an executable.
          len = lname.length ();

          if (len > 4 && lname.substr (len - 4) == ".idl")
            {
              if (!include_added)
                {
                  ACE_CString incl_arg ("-I");
                  incl_arg += bname;
                  DRV_cpp_putarg (incl_arg.c_str ());
                  include_added = true;
                }

              ACE_CString fname (bname);
              fname += "/";
              fname += lname;
              DRV_push_file (fname.c_str ());
            }

          break;
        case S_IFDIR: // Subdirectory.
          DRV_sweep_dirs (lname.c_str (), bname.c_str ());
          break;
        case S_IFLNK: // Either a file link or directory link.
        default: // Some other type of file (PIPE/FIFO/device).
          break;
        }
    }

  // Move back up a level.
  if (ACE_OS::chdir (DIR_DOT_DOT) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "DRV_sweep_dirs: chdir .. (from %s) failed\n",
                         rel_path),
                        -1);
    }

  return 0;
}

// Adds additional include paths, but after parse_args() has
// added user-defined include paths.
void
DRV_cpp_post_init (void)
{
  char option3[BUFSIZ];
  char option4[BUFSIZ];
  char option5[BUFSIZ];

  ACE_OS::strcpy (option3, "-I");
  ACE_OS::strcpy (option4, "-I");
  ACE_OS::strcpy (option5, "-I");

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

      ACE_OS::strcat (option3, TAO_ROOT);
      ACE_OS::strcat (option4, TAO_ROOT);
      ACE_OS::strcat (option5, TAO_ROOT);
#if defined (ACE_WIN32)
      ACE_OS::strcat (option3, "\\orbsvcs");
      ACE_OS::strcat (option4, "\\CIAO");
      ACE_OS::strcat (option5, "\\CIAO\\ciao");
#else
      ACE_OS::strcat (option3, "/orbsvcs");
      ACE_OS::strcat (option4, "/CIAO");
      ACE_OS::strcat (option5, "/CIAO/ciao");
#endif
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

          ACE_OS::strcat (option3, ACE_ROOT);
          ACE_OS::strcat (option4, ACE_ROOT);
          ACE_OS::strcat (option5, ACE_ROOT);
#if defined (ACE_WIN32)
          ACE_OS::strcat (option3, "\\TAO\\orbsvcs");
          ACE_OS::strcat (option4, "\\TAO\\CIAO");
          ACE_OS::strcat (option5, "\\TAO\\CIAO\\ciao");
#else
          ACE_OS::strcat (option3, "/TAO/orbsvcs");
          ACE_OS::strcat (option4, "/TAO/CIAO");
          ACE_OS::strcat (option5, "/TAO/CIAO/ciao");
#endif
        }
      else
        {
#if defined (TAO_IDL_INCLUDE_DIR)
          // TAO_IDL_INCLUDE_DIR should be in quotes,
          // e.g. "/usr/local/include/tao"
          ACE_OS::strcat (option3, TAO_IDL_INCLUDE_DIR);
          ACE_OS::strcat (option4, TAO_IDL_INCLUDE_DIR);
          ACE_OS::strcat (option5, TAO_IDL_INCLUDE_DIR);
          ACE_OS::strcat (option3, "/orbsvcs");
          ACE_OS::strcat (option4, "/CIAO");
          ACE_OS::strcat (option5, "/CIAO/ciao");
#else
          ACE_OS::strcat (option3, ".");
          ACE_OS::strcat (option4, ".");
          ACE_OS::strcat (option5, ".");
#endif
        }
    }

  DRV_cpp_putarg (option3);
  DRV_cpp_putarg (option4);
  DRV_cpp_putarg (option5);

  idl_global->add_include_path (ACE_CString (option3 + 2).c_str ());
  idl_global->add_include_path (ACE_CString (option4 + 2).c_str ());
  idl_global->add_include_path (ACE_CString (option5 + 2).c_str ());

  // Save path of current directory, in case the call to DRV_sweep_dirs()
  // below is not a no-op - then the current working directory will
  // have to be restored.
  char cwd_path[MAXPATHLEN];
  if (ACE_OS::getcwd (cwd_path, sizeof (cwd_path)) == 0)
    {
      ACE_ERROR ((LM_ERROR,
                  "DRV_cpp_post_init: ACE_OS::getcwd failed\n"));
      return;
    }

  // If first arg is non-zero, adds an include path and filename
  // for every IDL file found in all subdirectories. This is a
  // no-op for most backends.
  if (DRV_sweep_dirs (idl_global->recursion_start (), "") == -1)
    {
      ACE_ERROR ((LM_ERROR,
                  "DRV_cpp_post_init: DRV_sweep_dirs (%s) failed\n",
                  idl_global->recursion_start ()));

      return;
    }

  // This is redundant for most backends, but not if the call to
  // DRV_sweep_dirs() above is more than a no-op.
  if (ACE_OS::chdir (cwd_path) == -1)
    {
      ACE_ERROR ((LM_ERROR,
                  "DRV_cpp_post_init: ACE_OS::chdir (%s) failed\n",
                  cwd_path));
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

  // Skip the tabs and spaces.
  while (*r == ' ' || *r == '\t')
    {
      ++r;
    }

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

  // We're not handling redirection from stdin.
  if (*h == '\0')
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("IDL: No friggin' input files\n")));

      ACE_OS::exit (99);
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

  size_t len = ACE_OS::strlen (file_name);
  ACE_CString name_str (file_name);
  ACE_CString simple ("orb.idl");
  ACE_CString nix_path ("tao/orb.idl");
  ACE_CString win_path ("tao\\orb.idl");

  // Some backends pass this file through, others don't.
  if (name_str == simple || name_str == nix_path || name_str == win_path)
    {
      if (idl_global->pass_orb_idl ())
        {
          idl_global->add_to_included_idl_files (file_name);
        }
      else
        {
          DRV_get_orb_idl_includes ();
        }
    }
  // We have special lookup for orb.idl (TAO_ROOT/tao) that
  // also kicks in for .pidl files. If one of the latter is
  // included as a local name only, we add the 'tao/' prefix
  // so the generated C++ include files will be correct.
  else if (ACE_OS::strcmp (file_name + len - 5, ".pidl") == 0
           && ACE_OS::strchr (file_name, '/') == 0
           && ACE_OS::strchr (file_name, '\\') == 0)
    {
      ACE_CString fixed_name ("tao/");
      fixed_name += file_name;
      idl_global->add_to_included_idl_files (fixed_name.rep ());
    }
  else
    {
      idl_global->add_to_included_idl_files (file_name);
    }
}

// This method turns a line like '#include "a.idl"' into the
// line '#include <a.idl>'
void
DRV_convert_includes (const char* buf)
{
  // Remove constness
  char* r = const_cast<char*> (buf);

  // Skip the tabs and spaces.
  while (*r == ' ' || *r == '\t')
    {
      ++r;
    }

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
  // all the blanks and tabs and reach the startng " character.
  for (; (*r != '"'); ++r)
    {
      if (*r == '\n' || *r == '\0')
        {
          return;
        }
    }

  // Replace the opening quote with an angle bracket.
  *r = '<';

  // We're not handling redirection from stdin.
  // n.b. I neither know nor care what all this is about.
  // It came free with the cut-and-paste of DRV_check_for_include. sm.
  if (*r == '\0')
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("IDL: No friggin' input files\n")));

      ACE_OS::exit (99);
    }

  // Find the closing '"' character.
  for (; *r != '"'; ++r)
    {
      continue;
    }

  // Swap it for a '>'
  if (*r == '"')
    {
      *r = '>';
    }
}

void
DRV_get_orb_idl_includes (void)
{
  ACE_CString orb_idl_path (idl_global->tao_root ());
  orb_idl_path += "/tao/orb.idl";
  FILE *fd = ACE_OS::fopen (orb_idl_path.fast_rep (), "r");

  if (fd == 0) {
    ACE_ERROR ((LM_ERROR,
                "%s%s",
                orb_idl_path.fast_rep (),
                ": cannot open input file\n"));

    ACE_OS::exit (99);
  }

  while (DRV_get_line (fd))
    {
      // Find the included .pidl files and add them to
      // the included IDL file list.
      DRV_check_for_include (drv_line);
    }

  ACE_OS::fclose (fd);
}

// Copy to a file.
static void
DRV_copy_input (FILE *fin,
                FILE *f,
                const char *fn,
                const char *orig_filename)
{
  if (f == 0)
    {
      ACE_ERROR ((LM_ERROR,
                  "%s: cannot open temp file \"%s\" for writing\n",
                  idl_global->prog_name (),
                  fn));

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
      DRV_convert_includes (drv_line);

      // Print the line to the temporary file.
      ACE_OS::fprintf (f,
                       "%s\n",
                       drv_line);

      // We really need to know whether this line is a "#include
      // ...". If so, we would like to separate the "file name" and
      // keep that in the idl_global. We need them to produce
      // "#include's in the stubs and skeletons.
      DRV_check_for_include (drv_line);
    }

  // Close the temporary file.
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

// Pass input through preprocessor.
void
DRV_pre_proc (const char *myfile)
{
  const char* tmpdir = idl_global->temp_dir ();
  static const char temp_file_extension[] = ".cpp";

  static char const tao_idlf_template[] = "tao-idlf_XXXXXX";
  static char const tao_idli_template[] = "tao-idli_XXXXXX";

  size_t const tlen =
    ACE_OS::strlen (tmpdir) + sizeof (temp_file_extension);

  // Prevent a buffer overrun.
  if (tlen + sizeof (tao_idlf_template) > sizeof (tmp_file)
      || tlen + sizeof (tao_idli_template) > sizeof (tmp_ifile))
    {
      ACE_ERROR ((LM_ERROR,
                  "%s: temporary path/filename length is greater than "
                  "length allowed by platform\n",
                  idl_global->prog_name ()));

      return;
    }

  ACE_OS::strcpy (tmp_file,  tmpdir);
  ACE_OS::strcpy (tmp_ifile, tmpdir);

  // Append temporary filename template to temporary directory.
  ACE_OS::strcat (tmp_file,  tao_idlf_template);
  ACE_OS::strcat (tmp_ifile, tao_idli_template);

  ACE_HANDLE const ti_fd = ACE_OS::mkstemp (tmp_ifile);
  ACE_HANDLE const tf_fd = ACE_OS::mkstemp (tmp_file);

  if (ti_fd == ACE_INVALID_HANDLE || tf_fd == ACE_INVALID_HANDLE)
    {
      ACE_ERROR ((LM_ERROR,
                  "%s: Unable to create temporary file: %m\n",
                  idl_global->prog_name ()));

      return;
    }

  char tmp_cpp_file [MAXPATHLEN + 1] = { 0 };
  char tmp_cpp_ifile[MAXPATHLEN + 1] = { 0 };

  // Append C++ source file extension.  Temporary files will be renamed
  // to these filenames.
  ACE_OS::strcpy (tmp_cpp_file,  tmp_file);
  ACE_OS::strcpy (tmp_cpp_ifile, tmp_ifile);
  ACE_OS::strcat (tmp_cpp_file,  temp_file_extension);
  ACE_OS::strcat (tmp_cpp_ifile, temp_file_extension);

  char * const t_file  = tmp_cpp_file;
  char * const t_ifile = tmp_cpp_ifile;

  ACE_OS::close (tf_fd);

  // Rename temporary files so that they have extensions accepted
  // by the preprocessor.

  FILE * const file = ACE_OS::fopen (myfile, "r");
  DRV_copy_input (file,
                  ACE_OS::fdopen (ti_fd, "w"),
                  tmp_ifile,
                  myfile);
  ACE_OS::fclose (file);

  UTL_String *tmp = 0;
  ACE_NEW (tmp,
           UTL_String (myfile));
  idl_global->set_main_filename (tmp);

  ACE_Auto_String_Free safety (ACE_OS::strdup (myfile));
  ACE_NEW (tmp,
           UTL_String (DRV_stripped_name (safety.get ())));
  idl_global->set_stripped_filename (tmp);

  ACE_NEW (tmp,
           UTL_String (t_ifile));
  idl_global->set_real_filename (tmp);

  // We use ACE instead of the (low level) fork facilities, this also
  // works on NT.
  ACE_Process process;

  // For complex builds, the default command line buffer size of 1024
  // is sometimes not enough. We use 4096 here.
  ACE_Process_Options cpp_options (1,       // Inherit environment.
                                   TAO_IDL_COMMAND_LINE_BUFFER_SIZE);

  DRV_cpp_expand_output_arg (t_file);
  DRV_cpp_putarg (t_ifile);
  DRV_cpp_putarg (0); // Null terminate the arglist.

  cpp_options.command_line (arglist);

  // Rename temporary files so that they have extensions accepted
  // by the preprocessor.  Renaming is (supposed to be) an atomic
  // operation so we shouldn't be susceptible to attack.
  if (ACE_OS::rename (tmp_file, t_file) != 0
      || ACE_OS::rename (tmp_ifile, t_ifile) != 0)
    {
      ACE_ERROR ((LM_ERROR,
                  "%s: Unable to rename temporary file: %m\n",
                  idl_global->prog_name ()));

      return;
    }

  // Remove any existing output file.
  (void) ACE_OS::unlink (t_file);

  ACE_HANDLE fd = ACE_INVALID_HANDLE;

  if (output_arg_format == 0)
    {
      // If the following open() fails, then we're either being hit with a
      // symbolic link attack, or another process opened the file before
      // us.
      fd = ACE_OS::open (t_file,
                         O_WRONLY | O_CREAT | O_EXCL,
                         ACE_DEFAULT_FILE_PERMS);

      if (fd == ACE_INVALID_HANDLE)
        {
          ACE_ERROR ((LM_ERROR,
                      "%s: cannot open temp file \"%s\" for writing\n",
                      idl_global->prog_name (),
                      t_file));

          return;
        }

      cpp_options.set_handles (ACE_INVALID_HANDLE, fd);
    }

  if (process.spawn (cpp_options) == ACE_INVALID_PID)
    {
      ACE_ERROR ((LM_ERROR,
                  "%s: spawn of \"%s\" failed\n",
                  idl_global->prog_name (),
                  arglist[0]));

      return;
    }

  if (fd != ACE_INVALID_HANDLE)
    {
      // Close the output file on the parent process.
      if (ACE_OS::close (fd) == -1)
        {
          ACE_ERROR ((LM_ERROR,
                      "%s: cannot close temp file \"%s\" on parent\n",
                      idl_global->prog_name (),
                      t_file));

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
                  "%s: wait for child process failed\n",
                  idl_global->prog_name ()));

      return;
    }

  if (WIFEXITED ((status)))
    {
      // child terminated normally?
      if (WEXITSTATUS ((status)) != 0)
        {
          errno = WEXITSTATUS ((status));

          ACE_ERROR ((LM_ERROR,
                      "%s: preprocessor \"%s\" returned with an error\n",
                      idl_global->prog_name (),
                      arglist[0]));

          ACE_OS::exit (1);
        }
    }
  else
    {
      // child didn't call exit(); perhaps it received a signal?
      errno = EINTR;

      ACE_ERROR ((LM_ERROR,
                  "%s: preprocessor \"%s\" appears to have been interrupted\n",
                  idl_global->prog_name (),
                  arglist[0]));

      ACE_OS::exit (1);
    }
  // TODO: Manage problems in the pre-processor, in the previous
  // version the current process would exit if the pre-processor
  // returned with error.

  FILE * const yyin = ACE_OS::fopen (t_file, "r");

  if (yyin == NULL)
    {
      ACE_ERROR ((LM_ERROR,
                  "%s: Could not open cpp output file \"%s\"\n",
                  idl_global->prog_name (),
                  t_file));

      ACE_OS::exit (99);
    }

  FE_set_yyin (reinterpret_cast<File *> (yyin));

  if (idl_global->compile_flags () & IDL_CF_ONLY_PREPROC)
    {
      FILE *preproc = ACE_OS::fopen (t_file, "r");
      char buffer[ACE_MAXLOGMSGLEN];
      size_t bytes;

      if (preproc == 0)
        {
          ACE_ERROR ((LM_ERROR,
                      "%s: Could not open cpp output file: %s\n",
                      idl_global->prog_name (),
                      t_file));

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

  if (ACE_OS::unlink (t_ifile) == -1)
    {
      ACE_ERROR ((LM_ERROR,
                  "%s: Could not remove cpp input file \"%s\"\n",
                  idl_global->prog_name (),
                  t_ifile));

      ACE_OS::exit (99);
    }

#if !defined (ACE_WIN32) || defined (ACE_HAS_WINNT4) && (ACE_HAS_WINNT4 != 0)
  if (ACE_OS::unlink (t_file) == -1)
    {
      ACE_ERROR ((LM_ERROR,
                  "%s: Could not remove cpp output file \"%s\"\n",
                  idl_global->prog_name (),
                  t_file));

      ACE_OS::exit (99);
    }
#endif /* ACE_HAS_WINNT4 && ACE_HAS_WINNT4 != 0 */

  if (idl_global->compile_flags () & IDL_CF_ONLY_PREPROC)
    {
      ACE_OS::exit (0);
    }
}
