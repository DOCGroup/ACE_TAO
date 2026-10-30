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

#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include "idl_defines.h"
#include "global_extern.h"
#include "fe_extern.h"
#include "drv_extern.h"
#include "utl_string.h"
#include "utl_err.h"

#include "ace/Version.h"
#include "ace/Process.h"
#include "ace/Env_Value_T.h"
#include "ace/ARGV.h"
#include "ace/Dirent.h"
#include "ace/OS_NS_sys_stat.h"
#include "ace/Truncate.h"

// FUZZ: disable check_for_streams_include
#include "ace/streams.h"

#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_unistd.h"
#include "ace/OS_NS_fcntl.h"
#include "ace/OS_NS_sys_stat.h"

// Storage for preprocessor args.
constexpr unsigned long DRV_MAX_ARGCOUNT = 1024;
std::vector<std::basic_string<ACE_TCHAR>> DRV_arglist;

static std::optional<std::basic_string<ACE_TCHAR>> output_arg_format;
static size_t output_arg_index = 0;

constexpr ACE_TCHAR TCHAR_DIR_DOT[] = ACE_TEXT(".");

#if defined (ACE_HAS_TCHAR_DIRENT)
// ACE_TCHAR const DIR_DOT[] = ACE_TEXT(".");
constexpr ACE_TCHAR DIR_DOT_DOT[] = ACE_TEXT("..");
#else
// char const DIR_DOT[] = ".";
constexpr char DIR_DOT_DOT[] = "..";
#endif /* ACE_HAS_TCHAR_DIRENT */

#if defined (ACE_CC_PREPROCESSOR_DIRECT_INPUT)
static constexpr bool copy_input_file = false;
#else
static constexpr bool copy_input_file = true;
#endif

// Lines can be 1024 chars long intially -
// it will expand as required.
constexpr size_t LINEBUF_SIZE = 1024;
std::string drv_line;

static void
DRV_append_version_component (std::string &version,
                              int const component)
{
  std::string const component_string = std::to_string (component);
  if (component_string.size () < 2)
    {
      version.push_back ('0');
    }
  version += component_string;
}

// Push the new CPP location if we got a -Yp argument.
void
DRV_cpp_new_location (char const * new_loc)
{
  DRV_arglist[0] = ACE_TEXT_CHAR_TO_TCHAR (new_loc);
}

// Push an argument into the DRV_arglist.
void
DRV_cpp_putarg (char const *str)
{
  if (DRV_arglist.size () >= DRV_MAX_ARGCOUNT)
    {
      ACE_ERROR ((LM_ERROR,
                  "%C: More than %d arguments to preprocessor\n",
                  idl_global->prog_name ().data (),
                  DRV_MAX_ARGCOUNT ));

      throw Bailout ();
    }

  std::string argument (str);

#ifdef ACE_WIN32
  if (argument.find ('"') != std::string::npos)
    {
      std::string escaped;
      escaped.reserve (argument.size () + 4);
      for (char const character : argument)
        {
          if (character == '"')
            {
              escaped += '\\';
            }
          escaped += character;
        }
      argument = escaped;
    }
#endif

  if (argument.find (' ') != std::string::npos)
    {
      argument.insert (argument.begin (), '"');
      argument += '"';
    }

  DRV_arglist.emplace_back (ACE_TEXT_CHAR_TO_TCHAR (argument.c_str ()));
}

// Expand the output argument with the given filename.
void
DRV_cpp_expand_output_arg (char const *filename)
{
  if (output_arg_format)
    {
      std::basic_string<ACE_TCHAR> tchar_filename (
        ACE_TEXT_CHAR_TO_TCHAR (filename));
      std::basic_string<ACE_TCHAR> &output_arg =
        DRV_arglist[output_arg_index];
      output_arg.resize (output_arg_format->size () + tchar_filename.size () + 1);

#ifdef __GNUC__
#  pragma GCC diagnostic push
#  pragma GCC diagnostic ignored "-Wformat-nonliteral"
#endif
      ACE_OS::sprintf (output_arg.data (),
                       output_arg_format->c_str (),
                       tchar_filename.c_str ());
#ifdef __GNUC__
#  pragma GCC diagnostic pop
#endif
      output_arg.resize (ACE_OS::strlen (output_arg.c_str ()));
    }
}

// calculate the total size of all commandline arguments
unsigned int
DRV_cpp_calc_total_argsize()
{
  unsigned long size = 0;
  for (std::basic_string<ACE_TCHAR> const &argument : DRV_arglist)
    {
      size += ACE_Utils::truncate_cast<unsigned long> (argument.size () + 1);
    }
  return size;
}

// Get a line from stdin.
static bool
DRV_get_line (FILE *file)
{
  char buffer[LINEBUF_SIZE + 1];
  char *line = ACE_OS::fgets (buffer,
                              static_cast<int> (sizeof (buffer)),
                              file);
  if (!line || (!*line && feof (file)))
    {
      // End of file, nothing else read in.
      return false;
    }

  drv_line.clear ();

  do
    {
      drv_line.append (buffer);

      // Check for line continuation escape...
      size_t len = drv_line.size ();
      if (2 <= len &&
          '\n' == drv_line [len - 1] &&
          '\\' == drv_line [len - 2])
        {
          // This is a "false" end-of-line, remove token
          drv_line.resize (len - 2);
          len -= 2;
        }

      // Check for end-of-line
      if (len && '\n' == drv_line [len - 1])
        {
          drv_line.pop_back ();
          return true;
        }

      line = ACE_OS::fgets (buffer,
                            static_cast<int> (sizeof (buffer)),
                            file);
    } while (line && *line);

  return true;
}

// Initialize the cpp argument list.
void
DRV_cpp_init ()
{
  DRV_arglist.clear ();
  output_arg_format.reset ();
  output_arg_index = 0;

  std::string const cpp_loc = FE_get_cpp_loc_from_env ();
  DRV_cpp_putarg (cpp_loc.c_str ());

  // Add an option to the IDL compiler to make the TAO version
  // available to the user. A XX.YY.ZZ release gets
  // version 0xXXYYZZ, for example, 5.1.14 gets 0x050114.
  std::string version_option ("-D__TAO_IDL=0x");
  DRV_append_version_component (version_option, ACE_MAJOR_VERSION);
  DRV_append_version_component (version_option, ACE_MINOR_VERSION);
  DRV_append_version_component (version_option, ACE_MICRO_VERSION);

  DRV_cpp_putarg (version_option.c_str ());
  DRV_cpp_putarg ("-I.");

#if defined (ACE_CC_PREPROCESSOR_DIRECT_INPUT_ARGS)
  ACE_ARGV direct_input_arglist (
    ACE_TEXT_CHAR_TO_TCHAR (ACE_CC_PREPROCESSOR_DIRECT_INPUT_ARGS));

  for (size_t i = 0; i < static_cast<size_t> (direct_input_arglist.argc ()); ++i)
    {
      DRV_cpp_putarg (ACE_TEXT_ALWAYS_CHAR (direct_input_arglist[i]));
    }
#endif

  std::optional<std::string> const env_cpp_args =
    FE_get_cpp_args_from_env ();
  std::string platform_cpp_args;

  if (!env_cpp_args.has_value ())
    {
      // If no cpp flag was defined by the user, we define some
      // platform specific flags here.

#if defined (TAO_IDL_PREPROCESSOR_ARGS)
      platform_cpp_args = TAO_IDL_PREPROCESSOR_ARGS;
#elif defined (ACE_CC_PREPROCESSOR_ARGS)
      platform_cpp_args = ACE_CC_PREPROCESSOR_ARGS;
#else
      platform_cpp_args = "-E";
#endif /* TAO_IDL_PREPROCESSOR_ARGS */

      // So we can find OMG IDL files, such as `orb.idl'.
      std::string include_path1, include_path2;

      char* TAO_ROOT = ACE_OS::getenv ("TAO_ROOT");

      if (TAO_ROOT != nullptr)
        {
            DRV_add_include_path (include_path1,
                                  TAO_ROOT,
                                  {},
                                  true);

            DRV_add_include_path (include_path2,
                                  TAO_ROOT,
                                  "/tao",
                                  true);
        }
      else
        {
          char* ACE_ROOT = ACE_OS::getenv ("ACE_ROOT");

          if (ACE_ROOT != nullptr)
            {
              DRV_add_include_path (include_path1,
                                    ACE_ROOT,
                                    "/TAO",
                                    true);

              DRV_add_include_path (include_path2,
                                    ACE_ROOT,
                                    "/TAO/tao",
                                    true);
            }
          else
            {
#if defined (TAO_IDL_INCLUDE_DIR)
              // TAO_IDL_INCLUDE_DIR may be in quotes,
              // e.g. "/usr/local/include/tao"
              // We deal with a case like this below ...
              DRV_add_include_path (include_path1,
                                    TAO_IDL_INCLUDE_DIR,
                                    {},
                                    true);
              DRV_add_include_path (include_path2,
                                    TAO_IDL_INCLUDE_DIR,
                                    "/tao",
                                    true);
#else
              DRV_add_include_path (include_path1,
                                    ".",
                                    {},
                                    true);
#endif  /* TAO_IDL_INCLUDE_DIR */
            }
        }

      idl_global->tao_root (include_path1.c_str ());
    }
  else
    {
      platform_cpp_args = *env_cpp_args;
    }

  // Add any flags in platform_cpp_args to cpp's DRV_arglist.
  ACE_ARGV platform_arglist (
    ACE_TEXT_CHAR_TO_TCHAR (platform_cpp_args.c_str ()));

  for (size_t i = 0; i < static_cast<size_t> (platform_arglist.argc ()); ++i)
    {
      // Check for an argument that specifies
      // the preprocessor's output file.
      if (ACE_OS::strstr (platform_arglist[i], ACE_TEXT ("%s")) != nullptr
          && !output_arg_format)
        {
          output_arg_format = platform_arglist[i];
          output_arg_index = DRV_arglist.size ();
          DRV_cpp_putarg ("");
        }
      else
        {
          DRV_cpp_putarg (ACE_TEXT_ALWAYS_CHAR (platform_arglist[i]));
        }
    }
}

int
DRV_sweep_dirs (char const *rel_path,
                char const *base_path)
{
  // Zero rel_path means we're not using this option, and
  // so we become a no-op.
  if (rel_path == nullptr)
    {
      return 0;
    }

  if (ACE_OS::chdir (rel_path) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "DRV_sweep_dirs: chdir %C failed\n",
                         rel_path),
                        -1);
    }

  ACE_Dirent dir (TCHAR_DIR_DOT);
  std::string bname (base_path);
  bname += (bname.length () > 0 ? "/" : "");
  bname += rel_path;
  bool include_added = false;
  char abspath[MAXPATHLEN] = {};
  for (ACE_DIRENT *dir_entry; (dir_entry = dir.read ()) != nullptr;)
    {
      // Skip the ".." and "." files in each directory.
      if (ACE::isdotdir (dir_entry->d_name))
        {
          continue;
        }

#if defined (ACE_HAS_TCHAR_DIRENT)
      std::string lname (ACE_TEXT_ALWAYS_CHAR (dir_entry->d_name));
#else
      std::string lname (dir_entry->d_name);
#endif
      ACE_stat stat_buf;

      if (ACE_OS::lstat (lname.c_str (), &stat_buf) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "DRV_sweep_dirs: ACE_OS::lstat"
                             " (%C) failed\n",
                             lname.c_str ()),
                            -1);
        }

      size_t len = 0;

      switch (stat_buf.st_mode & S_IFMT)
        {
        case S_IFREG: // Either a regular file or an executable.
          len = lname.size ();

          if (len > 4 && lname.substr (len - 4) == ".idl")
            {
              if (!include_added)
                {
                  std::string incl_arg ("-I");
                  incl_arg += bname;
                  DRV_cpp_putarg (incl_arg.c_str ());
                  idl_global->add_rel_include_path (bname.c_str ());
                  char const * const full_path = ACE_OS::realpath ("", abspath);

                  if (full_path != nullptr)
                    {
                      idl_global->add_include_path (full_path,
                                                    false);
                    }

                  include_added = true;
                }

              std::string fname (bname);
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
                         "DRV_sweep_dirs: chdir "
                         ".. (from %C) failed\n",
                         rel_path),
                        -1);
    }

  return 0;
}

std::string&
DRV_add_include_path (std::string& include_path,
                      std::string_view path,
                      std::string_view suffix,
                      bool is_system)
{
  if (path.empty ())
    {
      return include_path;
    }

  std::string_view path_view (path);
  bool const needToQuote =
    (!path_view.empty () && path_view.front () == '"')
    || path_view.find (' ') != std::string_view::npos;
  char const
#if defined (ACE_WIN32)
    nativeDir =  '\\',
    foreignDir = '/';
#else
    nativeDir =  '/',
    foreignDir = '\\';
#endif

  // Eliminate possible enclosing quotes and a trailing directory separator.
  if (!path_view.empty () && path_view.front () == '"')
    {
      path_view.remove_prefix (1);
    }
  if (!path_view.empty () && path_view.back () == '"')
    {
      path_view.remove_suffix (1);
    }
  if (path_view.size () > 1
      && (path_view.back () == nativeDir || path_view.back () == foreignDir))
    {
      path_view.remove_suffix (1);
    }
  include_path.assign (path_view);

  if (!suffix.empty ())
    {
      std::string_view suffix_view (suffix);
      if (include_path.empty () && !suffix_view.empty ()
          && (nativeDir == suffix_view.front () || foreignDir == suffix_view.front ()))
        {
          // Path is empty, don't add the
          // suffix's leading directory separator.
          suffix_view.remove_prefix (1);
        }

      if (!include_path.empty () && !suffix_view.empty ()
          && nativeDir != suffix_view.front ()
          && foreignDir != suffix_view.front ())
        {
          // Force a directory separator.
          include_path += nativeDir;
        }

      // Add the suffix string to the path,
      // ensuring that foreign directory slashes
      // are added as the native type.
      for (char const character : suffix_view)
        {
          include_path += (foreignDir == character)
                             ? nativeDir
                             : character;
        }
    }

  // Build up the include string from the new path+suffix
  std::string include_option ("-I");
  if (needToQuote)
    {
      include_option += '"';
    }
  include_option += include_path;

  if (needToQuote)
    {
      include_option += '"';
    }

  DRV_cpp_putarg (include_option.c_str ());

  idl_global->add_include_path (include_path.c_str (),
                                is_system);

  return include_path;
}

// Adds additional include paths, but after parse_args() has
// added user-defined include paths.
void
DRV_cpp_post_init ()
{
  std::string idl_version_arg ("-D__TAO_IDL_IDL_VERSION=");
  idl_version_arg += idl_global->idl_version_.to_macro ();
  DRV_cpp_putarg (idl_version_arg.c_str ());

  DRV_cpp_putarg ("-D__TAO_IDL_FEATURES="
#ifdef TAO_IDL_FEATURES
    TAO_IDL_FEATURES
#else
    "\"tao/idl_features.h\""
#endif
  );

  // Add include path for TAO_ROOT/orbsvcs.
  char* TAO_ROOT = ACE_OS::getenv ("TAO_ROOT");

  std::string include_path3, include_path4, include_path5;

  // When adding new dirs here don't forget to update
  // FE_Utils::validate_orb_include accordingly.
  if (TAO_ROOT != nullptr)
    {
      DRV_add_include_path (include_path3,
                            TAO_ROOT,
                            "/orbsvcs",
                            true);
    }
  else
    {
      // If TAO_ROOT isn't defined, assume it's under ACE_ROOT.
      char* ACE_ROOT = ACE_OS::getenv ("ACE_ROOT");

      if (ACE_ROOT != nullptr)
        {
          DRV_add_include_path (include_path3,
                                ACE_ROOT,
                                "/TAO/orbsvcs",
                                true);
        }
      else
        {
#if defined (TAO_IDL_INCLUDE_DIR)
          DRV_add_include_path (include_path3,
                                TAO_IDL_INCLUDE_DIR,
                                "/orbsvcs",
                                true);
#else
          // If ACE_ROOT isn't defined either, there will already
          // be a warning from DRV_preproc().
          DRV_add_include_path (include_path3,
                                ".",
                                {},
                                true);
#endif  /* TAO_IDL_INCLUDE_DIR */
        }
    }

  // Add include paths for CIAO_ROOT and CIAO_ROOT/ciao.
  char* CIAO_ROOT = ACE_OS::getenv ("CIAO_ROOT");

  // When adding new dirs here don't forget to update
  // FE_Utils::validate_orb_include accordingly.
  if (CIAO_ROOT != nullptr)
    {
      DRV_add_include_path (include_path4,
                            CIAO_ROOT,
                            {},
                            true);

      DRV_add_include_path (include_path5,
                            CIAO_ROOT,
                            "/ciao",
                            true);

      DRV_add_include_path (include_path5,
                            CIAO_ROOT,
                            "/ccm",
                            true);
    }

  // Save path of current directory, in case
  // the call to DRV_sweep_dirs()
  // below is not a no-op - then the current
  // working directory will
  // have to be restored.
  char cwd_path[MAXPATHLEN];

  if (ACE_OS::getcwd (cwd_path, sizeof (cwd_path)) == nullptr)
    {
      ACE_ERROR ((LM_ERROR,
                  "DRV_cpp_post_init: ACE_OS::getcwd failed\n"));

      throw Bailout ();
    }

  // If first arg is non-zero, adds an include path and filename
  // for every IDL file found in all subdirectories. This is a
  // no-op for most backends.
  std::optional<std::string> const& recursion_start =
    idl_global->recursion_start ();

  if (recursion_start && DRV_sweep_dirs (recursion_start->c_str (), "") == -1)
    {
      ACE_ERROR ((LM_ERROR,
                  "DRV_cpp_post_init: DRV_sweep_dirs (%C) failed\n",
                  recursion_start->c_str ()));

      throw Bailout ();
    }

  // This is redundant for most backends, but not if the call to
  // DRV_sweep_dirs() above is more than a no-op.
  if (ACE_OS::chdir (cwd_path) == -1)
    {
      ACE_ERROR ((LM_ERROR,
                  "DRV_cpp_post_init: ACE_OS::chdir (%C) failed\n",
                  cwd_path));

      throw Bailout ();
    }
}

// Local/internal helper function.
namespace
{
  // Advances the input char buffer to the first non-white
  // space character, handles /**/ comments as well.
  char
  DRV_skip_over_white_spaces (char const *&input)
  {
    while (*input != '\0')
      {
        // Skip the spaces, tabs, vertical-tabs and form feeds.
        while (' ' == *input ||
              '\t' == *input ||
              '\v' == *input ||
              '\f' == *input)
          {
            ++input;
          }

        // Skip any "/*...*/" inline comments
        // (Note we can't cope with fully multi-line comments since we
        //  are scanning only single lines at a time here.)
        if ('/' != *input)
          {
            // This is the first non-white space character (could be
            // end of line).
            return *input;
          }

        if ('*' != *++input)
          {
            // Wasn't the start of a comment so / was the first non-white space character.
            return *--input;
          }

        // Skip over the contents of the comment (if we reach the end of the input
        // line we have no option but to concider the comment closed, bummer).
        do
          {
            // Looking for the closing "*/" characters
            while ('*' != *++input && *input)
              {}

            while ('*' == *input)
              {
                ++input;
              }
          } while ('/' != *input && *input);

          if ('/' == *input)
            {
              ++input;
            }
      }

    return '\0'; // Reached end of line.
  }

  // Checks for and skips over #include, positions buffer
  // at the leading quote character of the filename if
  // #include is found.
  bool
  DRV_find_include_filename (char const *&input)
  {
    // Must have initial # character
    if ('#' != DRV_skip_over_white_spaces (input))
      {
        return false;
      }

    // Only want #include to match
    char const *include_str = "include";

    if (*include_str != DRV_skip_over_white_spaces (++input))
      {
        return false;
      }

    while (*++include_str == *++input && *input)
      {}

    if (*include_str || !*input)
      {
        // Not #include (or it ends before filename given).
        return false;
      }

    // Next thing is finding the file that has been `#include'd. Skip
    // over to the starting " or < character.
    char const start_char = DRV_skip_over_white_spaces (input);
    return ('"' == start_char || '<' == start_char);
  }

  // We really need to know whether
  // this line is a "#include ...". If
  // so, we would like to separate the
  // "file name" and keep that in the
  // idl_global. We need them to produce
  // "#include's in the stubs and
  // skeletons.
  void
  DRV_check_for_include (char const *buf)
  {
    if (!DRV_find_include_filename (buf))
      {
        return;
      }

    // Skip over this leading " or < and
    // copy the filename upto the
    // closing " or > character.
    char const
      start_char = *buf++,
      end_char   = ('<' == start_char) ? '>' : start_char;

    std::string incl_file;

    while (*buf != '\0' && *buf != end_char)
      {
        // Put Microsoft-style pathnames into a canonical form.
        if ('\\' == buf[0] && '\\' == buf [1])
          {
            ++buf;
          }

        incl_file.push_back (*buf++);
      }

    if (incl_file.empty ())
      {
        return; // Null filename not allowed.
      }

    std::string const name_str (incl_file);
    std::string const simple ("orb.idl");
    std::string const nix_path ("tao/orb.idl");
    std::string const win_path ("tao\\orb.idl");

    // Some backends pass this file through, others don't.
    if (incl_file == simple
        || incl_file == nix_path
        || incl_file == win_path)
      {
        if (idl_global->pass_orb_idl ())
          {
            idl_global->add_to_included_idl_files (incl_file.c_str ());
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
    else if (incl_file.size () >= 5
             && std::string_view (incl_file).substr (incl_file.size () - 5)
                  == ".pidl"
             && incl_file.find ('/') == std::string::npos
             && incl_file.find ('\\') == std::string::npos)
      {
        std::string fixed_name ("tao/");
        fixed_name += incl_file;

        idl_global->add_to_included_idl_files (fixed_name.c_str ());
      }
    else
      {
        idl_global->add_to_included_idl_files (incl_file.c_str ());
      }
  }

  // This method turns a line like '#include "a.idl"' into the
  // line '#include <a.idl>'
  void
  DRV_convert_includes (char *buf)
  {
    char const *input = buf;

    if (!DRV_find_include_filename (input) || '"' != *input)
      {
        return; // Only interested in #include "" type
      }

    buf = const_cast<char *> (input);

    // Find the closing '"' character.
    char *open_quote= buf;

    while ('"' != *++buf && *buf != '\0')
      {
        if ('>' == *buf)
          {
            // Can't change to #include <> as it
            // has a > character in the filename!
            return;
          }
      }
    if ('"' == *buf)
      {
        // Replace the quotes with angle brackets.
        *open_quote = '<';
        *buf = '>';
      }
  }
} // End of local/internal namespace

void
DRV_get_orb_idl_includes ()
{
  static constexpr char orb_idl[] = "tao/orb.idl";

 // Search for orb.idl in supplied include file search paths.
  std::string directory;
  FILE * fp = FE_Utils::open_included_file (orb_idl, directory);

  if (fp == nullptr)
    {
      // Fall back on $TAO_ROOT/tao/orb.idl if orb.idl is not in the
      // include path.
      std::string orb_idl_path (idl_global->tao_root ());
      orb_idl_path += '/';
      orb_idl_path += orb_idl;

      fp = ACE_OS::fopen (orb_idl_path.c_str (), "r");

      if (fp == nullptr)
        {
          ACE_ERROR ((LM_ERROR,
                      "TAO_IDL: cannot open or find file: %C\n",
                      orb_idl_path.c_str ()));

          throw Bailout ();
        }
    }
  else
    {
      // Make sure the include directory containing orb.idl is passed
      // to the preprocessor.
      // This should go after user supplied include paths.
      std::string include_path_arg;
      DRV_add_include_path (include_path_arg, directory, "/tao", true);
    }

  while (DRV_get_line (fp))
    {
      // Find the included .pidl files in orb.idl and add them to the
      // included IDL file list.
      DRV_check_for_include (drv_line.c_str ());
    }

  ACE_OS::fclose (fp);
}

// Copy to a file.
static void
DRV_copy_input (FILE *fin,
                FILE *f,
                char const *fn,
                char const *orig_filename)
{
  if (f == nullptr)
    {
      ACE_ERROR ((LM_ERROR,
                  "%C: cannot open temp file \"%C\" for copying from \"%C\": %m\n",
                  idl_global->prog_name ().data (),
                  fn,
                  orig_filename));

      throw Bailout ();
    }

  if (fin == nullptr)
    {
      ACE_ERROR ((LM_ERROR,
                  "%C: cannot open input file\n",
                  idl_global->prog_name ().data ()));

      throw Bailout ();
    }

#if !defined (ACE_WIN32)
  ACE_OS::fprintf (f,
                   "#line 1 \"%s\"\n",
                   orig_filename);
#else
  // Convert single \ into double \ otherwise MSVC++ pre-processor
  // gets awfully confused.
  std::string escaped_filename;
  std::string_view const filename_view (orig_filename);
  escaped_filename.reserve (filename_view.size ());

  for (char const current_character : filename_view)
    {
      if (current_character == '\\')
        {
          escaped_filename.push_back ('\\');
        }

      escaped_filename.push_back (current_character);
    }

  ACE_OS::fprintf (f,
                   "#line 1 \"%s\"\n",
                   escaped_filename.c_str ());
#endif /* ! ACE_WIN32 */

  while (DRV_get_line (fin))
    {
      DRV_convert_includes (drv_line.data ());

      // Print the line to the temporary file.
      ACE_OS::fprintf (f,
                       "%s\n",
                       drv_line.c_str ());

      // We really need to know whether this line is a "#include
      // ...". If so, we would like to separate the "file name" and
      // keep that in the idl_global. We need them to produce
      // "#include's in the stubs and skeletons.
      DRV_check_for_include (drv_line.c_str ());
    }

  // Close the temporary file.
  ACE_OS::fclose (f);
}

// Strip down a name to the last component,
// i.e. everything after the last '/' or '\' character.
static std::string_view
DRV_stripped_name (std::string_view filename)
{
  size_t const separator = filename.find_last_of ("/\\");
  return separator == std::string_view::npos
    ? filename
    : filename.substr (separator + 1);
}

// Pass input through preprocessor.
void
DRV_pre_proc (char const *myfile)
{
  // Check to see that the file is a normal file. If we don't and the file is a
  // directory, then the copy would be blank and any error message later might
  // not be helpful.
  ACE_stat file_stat;
  if (ACE_OS::stat (myfile, &file_stat) == -1)
    {
      ACE_ERROR ((LM_ERROR,
                  "%C: ERROR: Unable to open file (stat) \"%C\": %m\n",
                  idl_global->prog_name ().data (),
                  myfile));
      throw Bailout ();
    }
  else if ((file_stat.st_mode & S_IFREG) != S_IFREG)
    {
      ACE_ERROR ((LM_ERROR,
                  "%C: ERROR: This is not a regular file: \"%C\"\n",
                  idl_global->prog_name ().data (),
                  myfile));
      throw Bailout ();
    }

  std::string const& temp_dir = idl_global->temp_dir ();
  if (temp_dir.empty ())
    {
      ACE_ERROR ((LM_ERROR,
                  "%C: temporary directory is not set\n",
                  idl_global->prog_name ()));

      throw Bailout ();
    }

  char const *tmpdir = temp_dir.c_str ();
  static constexpr char temp_file_extension[] = ".cpp";

  static constexpr char tao_idlf_template[] = "tao-idlf_XXXXXX";
  static constexpr char tao_idli_template[] = "tao-idli_XXXXXX";

  std::string tmp_file (tmpdir);
  tmp_file += tao_idlf_template;

  std::string tmp_ifile;
  if (copy_input_file)
    {
      tmp_ifile = tmpdir;
      tmp_ifile += tao_idli_template;
    }

  ACE_HANDLE ti_fd = ACE_INVALID_HANDLE;
  if (copy_input_file)
    {
      ti_fd = ACE_OS::mkstemp (tmp_ifile.data ());
      if (ti_fd == ACE_INVALID_HANDLE)
        {
          ACE_ERROR ((LM_ERROR,
                      "%C: Unable to create temporary file \"%C\": %m\n",
                      idl_global->prog_name ().data (),
                      tmp_ifile.c_str ()));

          throw Bailout ();
        }
    }

  ACE_HANDLE const tf_fd = ACE_OS::mkstemp (tmp_file.data ());

  if (tf_fd == ACE_INVALID_HANDLE)
    {
      ACE_ERROR ((LM_ERROR,
                  "%C: Unable to create temporary file \"%C\": %m\n",
                  idl_global->prog_name ().data (),
                  tmp_file.c_str ()));

      if (copy_input_file)
        {
          (void) ACE_OS::unlink (tmp_ifile.c_str ());
        }
      throw Bailout ();
    }

  // Append C++ source file extension. Temporary files will be renamed to
  // these filenames when the preprocessor requires an input extension.
  std::string tmp_cpp_file (tmp_file);
  tmp_cpp_file += temp_file_extension;

  std::string tmp_cpp_ifile;
  if (copy_input_file)
    {
      tmp_cpp_ifile = tmp_ifile;
      tmp_cpp_ifile += temp_file_extension;
    }

  std::string const &t_file = copy_input_file ? tmp_cpp_file : tmp_file;
  std::string const &t_ifile = tmp_cpp_ifile;
  char const * const input_file = copy_input_file ? t_ifile.c_str () : myfile;

  ACE_OS::close (tf_fd);

  // Rename temporary files so that they have extensions accepted
  // by the preprocessor.
  FILE * const file = ACE_OS::fopen (myfile, "r");
  if (file == nullptr)
    {
      ACE_ERROR ((LM_ERROR,
                  "%C: ERROR: Unable to open file (fopen) \"%C\": %m\n",
                  idl_global->prog_name ().data (),
                  myfile));
      if (copy_input_file)
        {
          (void) ACE_OS::unlink (tmp_ifile.c_str ());
        }
      (void) ACE_OS::unlink (tmp_file.c_str ());
      throw Bailout ();
    }

  if (copy_input_file)
    {
      DRV_copy_input (file,
                      ACE_OS::fdopen (ti_fd, ACE_TEXT("w")),
                      tmp_ifile.c_str (),
                      myfile);
    }
  else
    {
      while (DRV_get_line (file))
        {
          DRV_check_for_include (drv_line.c_str ());
        }
    }
  ACE_OS::fclose (file);

  idl_global->set_main_filename (myfile);
  std::string const stripped_filename (DRV_stripped_name (myfile));
  idl_global->set_stripped_filename (stripped_filename);
  idl_global->set_real_filename (input_file);

  // We use ACE instead of the (low level) fork facilities, this also
  // works on NT.
  ACE_Process process;

  DRV_cpp_expand_output_arg (t_file.c_str ());
  DRV_cpp_putarg (input_file);

  std::vector<ACE_TCHAR const *> cpp_arglist;
  cpp_arglist.reserve (DRV_arglist.size () + 1);
  for (std::basic_string<ACE_TCHAR> const &argument : DRV_arglist)
    {
      cpp_arglist.push_back (argument.c_str ());
    }
  cpp_arglist.push_back (nullptr);

  // For complex builds, the default
  // command line buffer size of 1024
  // is often not enough. We determine
  // the required space and arg nr
  // dynamically here.
  ACE_Process_Options cpp_options (true,       // Inherit environment.
                                   DRV_cpp_calc_total_argsize (),
                                   16 * 1024,
                                   512,
                                   ACE_Utils::truncate_cast<unsigned long> (
                                     cpp_arglist.size ()));

  if (cpp_options.command_line (cpp_arglist.data ()) != 0)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("%C: command line processing \"%s\" failed\n"),
                  idl_global->prog_name (),
                  DRV_arglist[0].c_str ()));

      if (copy_input_file)
        {
          (void) ACE_OS::unlink (tmp_ifile.c_str ());
        }
      (void) ACE_OS::unlink (tmp_file.c_str ());
      throw Bailout ();
    }

  // Rename temporary files so that they have extensions accepted
  // by the preprocessor.  Renaming is (supposed to be) an atomic
  // operation so we shouldn't be susceptible to attack.
  if (copy_input_file
      && ACE_OS::rename (tmp_file.c_str (), t_file.c_str ()) != 0)
    {
      ACE_ERROR ((LM_ERROR,
                  "%C: Unable to rename temporary "
                  "file \"%C\" to \"%C\": %m\n",
                  idl_global->prog_name ().data (),
                  tmp_file.c_str (),
                  t_file.c_str ()));


      (void) ACE_OS::unlink (tmp_ifile.c_str ());
      (void) ACE_OS::unlink (tmp_file.c_str ());
      throw Bailout ();
    }

  if (copy_input_file
      && ACE_OS::rename (tmp_ifile.c_str (), t_ifile.c_str ()) != 0)
    {
      ACE_ERROR ((LM_ERROR,
                  "%C: Unable to rename temporary "
                  "file \"%C\" to \"%C\": %m\n",
                  idl_global->prog_name ().data (),
                  tmp_ifile.c_str (),
                  t_ifile.c_str ()));

      (void) ACE_OS::unlink (tmp_ifile.c_str ());
      (void) ACE_OS::unlink (t_file.c_str ());
      throw Bailout ();
    }

  // Remove any existing output file.
  (void) ACE_OS::unlink (t_file.c_str ());

  ACE_HANDLE fd = ACE_INVALID_HANDLE;

  if (!output_arg_format)
    {
      // If the following open() fails, then we're either being hit with a
      // symbolic link attack, or another process opened the file before
      // us.
      fd = ACE_OS::open (t_file.c_str (),
                         O_WRONLY | O_CREAT | O_EXCL,
                         ACE_DEFAULT_FILE_PERMS);

      if (fd == ACE_INVALID_HANDLE)
        {
          ACE_ERROR ((LM_ERROR,
                      "%C: cannot open temp file"
                      " \"%C\" for writing: %m\n",
                      idl_global->prog_name ().data (),
                      t_file.c_str ()));

          (void) ACE_OS::unlink (t_file.c_str ());
          (void) ACE_OS::unlink (t_ifile.c_str ());
          throw Bailout ();
        }

      if (cpp_options.set_handles (ACE_INVALID_HANDLE, fd) == -1)
        {
          ACE_ERROR ((LM_ERROR, "%C: cannot set stdout for child process: %m\n",
                      idl_global->prog_name ().data ()));

          throw Bailout ();
        }
    }

  if (idl_global->compile_flags () & IDL_CF_INFORMATIVE)
    {
      ACE_DEBUG ((LM_DEBUG, "%C: spawning: %s\n",
                  idl_global->prog_name ().data (),
                  cpp_options.command_line_buf ()));
    }

  if (process.spawn (cpp_options) == ACE_INVALID_PID)
    {
      ACE_ERROR ((LM_ERROR,
                  "%C: spawn of \"%s\" failed\n",
                  idl_global->prog_name (),
                  DRV_arglist[0].c_str ()));


      (void) ACE_OS::unlink (t_file.c_str ());
      (void) ACE_OS::unlink (t_ifile.c_str ());
      throw Bailout ();
    }

  if (fd != ACE_INVALID_HANDLE)
    {
      // Close the output file on the parent process.
      if (ACE_OS::close (fd) == -1)
        {
          ACE_ERROR ((LM_ERROR,
            "%C: cannot close temp file \"%C\" on parent: %m\n",
                      idl_global->prog_name ().data (),
                      t_file.c_str ()));

          (void) ACE_OS::unlink (t_file.c_str ());
          (void) ACE_OS::unlink (t_ifile.c_str ());
          throw Bailout ();
        }
    }

  // Remove the input file from the argument storage so the next file can use
  // the same preprocessor options.
  DRV_arglist.pop_back ();
  ACE_exitcode status = 0;

  if (process.wait (&status) == ACE_INVALID_PID)
    {
      ACE_ERROR ((LM_ERROR,
                  "%C: wait for child process failed\n",
                  idl_global->prog_name ().data ()));

      (void) ACE_OS::unlink (t_file.c_str ());
      (void) ACE_OS::unlink (t_ifile.c_str ());
      throw Bailout ();
    }

  if (WIFEXITED ((status)))
    {
      // Child terminated normally?
      if (WEXITSTATUS ((status)) != 0)
        {
          errno = WEXITSTATUS ((status));

          ACE_ERROR ((LM_ERROR,
                      "%C: preprocessor \"%s\" "
                      "returned with an error\n",
                      idl_global->prog_name (),
                      DRV_arglist[0].c_str ()));

          (void) ACE_OS::unlink (t_file.c_str ());
          (void) ACE_OS::unlink (t_ifile.c_str ());
          throw Bailout ();
        }
    }
  else
    {
      // Child didn't call exit(); perhaps it received a signal?
      errno = EINTR;

      ACE_ERROR ((LM_ERROR,
                  "%C: preprocessor \"%s\" appears "
                  "to have been interrupted\n",
                  idl_global->prog_name (),
                  DRV_arglist[0].c_str ()));

      (void) ACE_OS::unlink (t_file.c_str ());
      (void) ACE_OS::unlink (t_ifile.c_str ());
      throw Bailout ();
    }
  // TODO: Manage problems in the
  // pre-processor, in the previous
  // version the current process
  // would exit if the pre-processor
  // returned with error.
  FILE * const yyin = ACE_OS::fopen (t_file.c_str (), "r");

  if (yyin == nullptr)
    {
      ACE_ERROR ((LM_ERROR,
                  "%C: Could not open cpp "
                  "output file \"%C\": %m\n",
                  idl_global->prog_name ().data (),
                  t_file.c_str ()));

      (void) ACE_OS::unlink (t_file.c_str ());
      (void) ACE_OS::unlink (t_ifile.c_str ());
      throw Bailout ();
    }

  FE_set_yyin (yyin);

  if (idl_global->compile_flags () & IDL_CF_ONLY_PREPROC)
    {
      FILE *preproc = ACE_OS::fopen (t_file.c_str (), "r");
      char buffer[ACE_MAXLOGMSGLEN];
      size_t bytes;

      if (preproc == nullptr)
        {
          ACE_ERROR ((LM_ERROR,
                      "%C: Could not open cpp "
                      "output file \"%C\": %m\n",
                      idl_global->prog_name ().data (),
                      t_file.c_str ()));

          (void) ACE_OS::unlink (t_file.c_str ());
          (void) ACE_OS::unlink (t_ifile.c_str ());
          throw Bailout ();
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
          buffer[bytes] = '\0';
          ACE_DEBUG ((LM_DEBUG, buffer));
        }

      ACE_OS::fclose (preproc);
    }

  if (copy_input_file && ACE_OS::unlink (t_ifile.c_str ()) == -1)
    {
      ACE_ERROR ((LM_ERROR,
                  "%C: Could not remove cpp "
                  "input file \"%C\": %m\n",
                  idl_global->prog_name ().data (),
                  t_ifile.c_str ()));

      throw Bailout ();
    }

  if (ACE_OS::unlink (t_file.c_str ()) == -1)
    {
      ACE_ERROR ((LM_ERROR,
                  "%C: Could not remove cpp "
                  "output file \"%C\": %m\n",
                  idl_global->prog_name ().data (),
                  t_file.c_str ()));

      throw Bailout ();
    }
}
