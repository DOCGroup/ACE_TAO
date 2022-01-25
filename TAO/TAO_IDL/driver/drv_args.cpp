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

// drv_args.cpp - Argument parsing for IDL compiler main driver

#include <string>

#include "idl_defines.h"
#include "global_extern.h"
#include "drv_extern.h"
#include "fe_extern.h"

#include "be_global.h"
#include "be_extern.h"
#include "be_util.h"

#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_stdlib.h"
#include "ace/OS_NS_unistd.h"
#include "ace/os_include/os_ctype.h"
#include "ace/Lib_Find.h"

extern long DRV_nfiles;
extern char *DRV_files[];

void process_long_option(long ac, char **av, long &i);

// Push a file into the list of files to be processed
void
DRV_push_file (const char *s)
{
  // If filenames come from the command line, there is no
  // need to duplicate the string, but some backends have
  // an option to recurse over a directory and add all
  // IDL files found. In this latter case we have to
  // duplicate the file name string.
  DRV_files[DRV_nfiles++] = ACE::strnew (s);
}

// Prepare a CPP argument
static void
DRV_prep_cpp_arg (char *s)
{
  std::string instr (s);
  std::string newarg;
  newarg.reserve (512);

  for (std::string::size_type pos = instr.find_first_of (',');
       pos != std::string::npos;
       instr = instr.substr (pos + 1), pos = instr.find_first_of (','))
    {
      newarg.append (instr.substr (0, pos));
    }

  DRV_cpp_putarg (newarg.c_str ());
}

// Print a usage message and exit.
void
DRV_usage ()
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("%C: usage: %C [[flag|file] ...] [-- file ...]\n"),
              idl_global->prog_name (),
              idl_global->prog_name ()));

  ACE_DEBUG ((LM_DEBUG,
    ACE_TEXT ("Legal flags:\n")
    ACE_TEXT (" -h | --help | -u\tPrint this list and exit successfully\n")
    ACE_TEXT (" -A...\t\t\tlocal implementation-specific escape\n")
    ACE_TEXT (" -Cw\t\t\tWarning if identifier spellings differ ")
    ACE_TEXT ("only in case (default is error)\n")
    ACE_TEXT (" -Ce\t\t\tError if identifier spellings differ ")
    ACE_TEXT ("only in case (default)\n")
    ACE_TEXT (" -ae\t\t\tError if anonymous type is seen ")
    ACE_TEXT ("(default)\n")
    ACE_TEXT (" -aw\t\t\tWarning if anonymous type is seen ")
    ACE_TEXT ("(default is error)\n")
    ACE_TEXT (" -as\t\t\tSilences the anonymous type diagnostic ")
    ACE_TEXT ("(default is error)\n")
    ACE_TEXT (" -d | --dump\t\tPrints a dump of the AST and exits\n")
    ACE_TEXT (" -Dname[=value]\t\tdefines name for preprocessor\n")
    ACE_TEXT (" -E\t\t\truns preprocessor only, prints on stdout\n")
    ACE_TEXT (" -Idir\t\t\tincludes dir in search path for preprocessor\n")
    ACE_TEXT (" -t\t\t\tTemporary directory to be used")
    ACE_TEXT (" by the IDL compiler.\n")
    ACE_TEXT (" -Uname\t\t\tundefines name for preprocessor\n")
    ACE_TEXT (" -v\t\t\ttraces compilation stages\n")
    ACE_TEXT (" -V | --version\t\tprints version info then exits\n")
    ACE_TEXT (" -w\t\t\tsuppresses IDL compiler warning messages\n")
    ACE_TEXT (" -Wp,<arg1,...,argn>\tpasses args to preprocessor\n")
    ACE_TEXT (" -Yp,path\t\tdefines location of preprocessor\n")
    ACE_TEXT (" --idl-version VERSION\tSet the version of IDL to use\n")
    ACE_TEXT (" --default-idl-version\tPrint the default IDL version and exit\n")
    ACE_TEXT (" --list-idl-versions\tPrint IDL versions supported and exit\n")
    ACE_TEXT (" --syntax-only\t\tJust check the syntax, do not create files\n")
    ACE_TEXT (" --bison-trace\t\tEnable Bison Tracing (sets yydebug to 1)\n")
    ACE_TEXT (" --dump-builtins\tDump the compiler and user defined IDL.\n")
    ACE_TEXT (" --just-dump-builtins\tJust dump the compiler defined IDL and exit.\n")
    ACE_TEXT (" --unknown-annotations ARG\t")
      ACE_TEXT ("Set reaction to unknown annotations. ARG must be one of the following:\n")
    ACE_TEXT ("\t\t\t\twarn-once\tThe default, warn once per unique local name\n")
    ACE_TEXT ("\t\t\t\twarn-all\tWarn for all unknown annotations\n")
    ACE_TEXT ("\t\t\t\terror\t\tCauses the process to exit with a failed return status\n")
    ACE_TEXT ("\t\t\t\tignore\t\tDon't report unknown annotations\n")
  ));

  be_util::usage ();
}

#define UNKNOWN_OPTION \
  ACE_ERROR (( \
      LM_ERROR, \
      ACE_TEXT ("IDL: I don't understand") \
      ACE_TEXT (" the '%s' option\n"), \
      ACE_TEXT_CHAR_TO_TCHAR (av[i]) \
    )); \
  idl_global->parse_args_exit (1); \
  return;

// Parse arguments on command line
void
DRV_parse_args (long ac, char **av)
{
  ACE_CString buffer;
  char *s = nullptr;
  long i;

  // After -- process all arguments as files
  bool just_files = false;

  FE_store_env_include_paths ();
  DRV_cpp_init ();
  idl_global->set_prog_name (av[0]);

  for (i = 1; i < ac; i++)
    {
      if (!just_files && av[i][0] == '-')
        {
          idl_global->append_idl_flag (av[i]);

          switch (av[i][1])
            {
            case 0:
              // One or more letters expected after the dash.
              ACE_ERROR ((
                  LM_ERROR,
                  ACE_TEXT ("IDL: Space between dash and option ")
                  ACE_TEXT ("letters not allowed\n")
                ));
              idl_global->parse_args_exit (1);
              return;

            case 'A':
              if (av[i][2] == '\0')
                {
                  if (i < ac - 1)
                    {
                      s = av[i + 1];
                      ++i;
                    }
                  else
                    {
                      ACE_ERROR ((
                          LM_ERROR,
                          ACE_TEXT ("IDL: incorrect use of ")
                          ACE_TEXT ("the -A option\n")
                        ));
                      idl_global->parse_args_exit (1);
                      return;

                    }
                }
              else
                {
                  s = av[i] + 2;
                }

              ACE_OS::strcat (idl_global->local_escapes (), s);
              ACE_OS::strcat (idl_global->local_escapes (), " ");
              break;
            case 'a':
              if (av[i][2] == 'e')
                {
                  idl_global->anon_type_diagnostic (
                    IDL_GlobalData::ANON_TYPE_ERROR);
                }
              else if (av[i][2] == 'w')
                {
                  idl_global->anon_type_diagnostic (
                    IDL_GlobalData::ANON_TYPE_WARNING);
                }
              else if (av[i][2] == 's')
                {
                  idl_global->anon_type_diagnostic (
                    IDL_GlobalData::ANON_TYPE_SILENT);
                }
              else
                {
                  UNKNOWN_OPTION;
                }

               break;
            // Temp directory for the IDL compiler to keep its files.
            case 't':
              if ((av[i][2] == '\0') && (i < ac - 1))
                {
                  idl_global->append_idl_flag (av[i + 1]);
                  idl_global->temp_dir (av[i + 1]);
                  ++i;
                }
              else
                {
                  UNKNOWN_OPTION;
                }

              break;
            case 'D':
            case 'U':
            case 'I':
              if (av[i][2] == '\0')
                {
                  if (i < ac - 1)
                    {
                      idl_global->append_idl_flag (av[i + 1]);

                      ACE_CString arg = av[i];
                      arg += av[i + 1];

                      DRV_cpp_putarg (arg.c_str ());
                      idl_global->add_include_path (arg.substr (2).c_str (), false);
                      ++i;
                    }
                  else
                    {
                      UNKNOWN_OPTION;
                    }
                }
              else
                {
                  ACE_CString arg (av[i], 2);
                  arg += av[i] + 2;

                  idl_global->add_include_path (arg.substr (2).c_str (), false);
                  DRV_cpp_putarg (arg.c_str ());
                }

              break;
            case 'E':
              if (av[i][2] != '\0')
                {
                  UNKNOWN_OPTION;
                }
              idl_global->set_compile_flags (idl_global->compile_flags () |
                                             IDL_CF_ONLY_PREPROC);
              break;
            case 'V':
              if (av[i][2] != '\0')
                {
                  UNKNOWN_OPTION;
                }
              idl_global->print_version ();
              break;
            case 'W':
              if (av[i][2] == '\0')
                {
                  if (i < ac - 1)
                    {
                      s = av[i + 1];
                      ++i;
                    }
                  else
                    {
                      UNKNOWN_OPTION;
                    }
                }
              else
                {
                  s = av[i] + 2;
                }

              switch (*s)
                {
                default:
                  ACE_ERROR ((
                      LM_ERROR,
                      ACE_TEXT ("IDL: Incorrect use of -W option\n")
                    ));
                  idl_global->parse_args_exit (1);
                  return;

                case 'p':
                  if (*(s + 1) == ',')
                    {
                      DRV_prep_cpp_arg (s + 2);
                    }

                  break;
                case 'b':
                  if (*(s + 1) == ',')
                    {
                      be_util::prep_be_arg (s + 2);
                    }

                  break;
                }

              break;
            case 'Y':
              if (av[i][2] == '\0')
                {
                  if (i < ac - 1)
                    {
                      s = av[i + 1];
                      ++i;
                    }
                  else
                    {
                      UNKNOWN_OPTION;
                    }
                }
              else
                {
                  s = av[i] + 2;
                }

              switch (*s)
                {
                  case 'p':
                    if (*(s + 1) == ',')
                      {
                        idl_global->set_cpp_location (s + 2);
                        DRV_cpp_new_location (s + 2);
                      }
                    else
                      {
                        ACE_ERROR ((
                            LM_ERROR,
                            ACE_TEXT ("IDL: I don't understand")
                            ACE_TEXT (" the '-Y' option\n")
                          ));
                        idl_global->parse_args_exit (1);
                        return;
                      }

                    break;
                  default:
                    ACE_ERROR ((
                        LM_ERROR,
                        ACE_TEXT ("IDL: I dont' understand the use of")
                        ACE_TEXT (" %s with the '-Y' option\n"),
                        ACE_TEXT_CHAR_TO_TCHAR (s)
                      ));
                    idl_global->parse_args_exit (1);
                    return;
                }
              break;
            case 'd':
              if (av[i][2] != '\0')
                {
                  UNKNOWN_OPTION;
                }
              idl_global->dump_ast ();
              break;
            case 'v':
              if (av[i][2] != '\0')
                {
                  UNKNOWN_OPTION;
                }
              idl_global->set_compile_flags (idl_global->compile_flags ()
                                             | IDL_CF_INFORMATIVE);
              break;
            case 'w':
              if (av[i][2] != '\0')
                {
                  UNKNOWN_OPTION;
                }
              idl_global->set_compile_flags (idl_global->compile_flags ()
                                             | IDL_CF_NOWARNINGS);
              break;
            case 'C':
              // If identifiers in the same scope differ only by case...
              if (av[i][2] == 'e')
                {
                  // ...report an error.
                  idl_global->case_diff_error (true);
                }
              else if (av[i][2] == 'w')
                {
                  // ...report a warning (default for now)
                  idl_global->case_diff_error (false);
                }
              else
                {
                  UNKNOWN_OPTION;
                }

              break;

            case 'u': // Old Help Option
              if (av[i][2] != '\0')
                {
                  UNKNOWN_OPTION;
                }
              idl_global->print_help ();
              break;

            case 'h': // Short Help Option, else let be_global process it
              if (av[i][2] == '\0') {
                idl_global->print_help ();
              } else {
                be_global->parse_args (i, av);
              }
              break;

            case '-': // -- or Long Options
              if (av[i][2] == '\0')
                {
                  just_files = true; // Treat the following arguments as files
                }
              else
                {
                  process_long_option(ac, av, i);
                }
              break;

            default:
              be_global->parse_args (i, av);
            } // End of switch (av[i][1])
        } // End of IF (av[i][0] == '-')
      else
        {
          DRV_push_file (av[i]);
        }

      if (idl_global->parse_args_exit_) return; // Catch Exits
    } // End of FOR (i = 1; i < ac; i++)

  be_util::arg_post_proc ();

  // Make sure the output directory is valid.
  if (idl_global->temp_dir () == nullptr)
    {
      ACE_TCHAR tmpdir[MAXPATHLEN + 1];

      if (ACE::get_temp_dir (tmpdir, MAXPATHLEN) == -1)
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("Temporary path too long, ")
                      ACE_TEXT ("defaulting to current directory\n")));

          ACE_OS::strcpy (tmpdir, ACE_TEXT ("."));
        }

#if defined(ACE_MVS)
      if (ACE_OS::access (tmpdir, F_OK) == -1
          || ACE_OS::access (tmpdir, R_OK) == -1
          || ACE_OS::access (tmpdir, W_OK) == -1)
#else
      if (ACE_OS::access (tmpdir, F_OK | R_OK | W_OK) == -1)
#endif /* ACE_MVS */
        {
          ACE_ERROR ((
              LM_ERROR,
              ACE_TEXT ("Can't access temporary directory (%s),")
              ACE_TEXT (" using current directory for temp files.\n"),
              tmpdir
            ));

          ACE_OS::strcpy (tmpdir, ACE_TEXT ("."));
#if defined(ACE_MVS)
          if (ACE_OS::access (tmpdir, F_OK) == -1
              || ACE_OS::access (tmpdir, R_OK) == -1
              || ACE_OS::access (tmpdir, W_OK) == -1)
#else
          if (ACE_OS::access (tmpdir, F_OK | R_OK | W_OK) == -1)
#endif /* ACE_MVS */
            {
              ACE_ERROR ((LM_ERROR,
                          "Error: Can't access temporary directory %s\n",
                          tmpdir));

              throw Bailout ();
            }
        }

      idl_global->temp_dir (ACE_TEXT_ALWAYS_CHAR (tmpdir));
    }

  DRV_cpp_post_init ();
}

void
print_idl_versions()
{
  ACE_DEBUG ((LM_INFO,
    ACE_TEXT ("These are the valid IDL versions this compiler will accept:\n")
    ));
  for (int v = 1; v < IDL_VERSION_COUNT; v++)
    {
      ACE_DEBUG ((LM_INFO, ACE_TEXT ("%C\n"),
        IdlVersion (static_cast<SpecificIdlVersion>(v)).to_string ()
        ));
    }
}

void
process_long_option(long ac, char **av, long &i)
{
  const char *long_option = av[i] + 2;
  bool no_more_args = i + 1 >= ac;
  if (!ACE_OS::strcmp (long_option, "idl-version"))
    {
      bool invalid_version = no_more_args;
      if (no_more_args)
        {
          ACE_ERROR ((LM_ERROR,
            ACE_TEXT ("--idl-version is missing a required argument, ")
            ACE_TEXT ("the IDL version to use.\n")
            ));
        }
      else
        {
          idl_global->idl_version_.from_string (av[++i]);
          invalid_version = !idl_global->idl_version_.is_valid ();
          if (invalid_version)
            {
              ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("\"%C\" is not a valid IDL version supported\n"),
                av[i]
                ));
            }
        }
      if (invalid_version)
        {
          print_idl_versions ();
          idl_global->parse_args_exit (1);
        }
    }
  else if (!ACE_OS::strcmp (long_option, "syntax-only"))
    {
      idl_global->syntax_only_ = true;
    }
  else if (!ACE_OS::strcmp (long_option, "default-idl-version"))
    {
      ACE_DEBUG ((LM_INFO, ACE_TEXT ("%C\n"),
        idl_global->default_idl_version_.to_string ()));
      idl_global->parse_args_exit (0);
    }
  else if (!ACE_OS::strcmp (long_option, "list-idl-versions"))
    {
      print_idl_versions ();
      idl_global->parse_args_exit (0);
    }
  else if (!ACE_OS::strcmp (long_option, "help"))
    {
      idl_global->print_help ();
    }
  else if (!ACE_OS::strcmp (long_option, "bison-trace"))
    {
      FE_yydebug (true);
    }
  else if (!ACE_OS::strcmp (long_option, "version"))
    {
      idl_global->print_version ();
    }
  else if (!ACE_OS::strcmp (long_option, "dump-builtins"))
    {
      idl_global->dump_builtins_ = true;
      idl_global->dump_ast ();
    }
  else if (!ACE_OS::strcmp (long_option, "just-dump-builtins"))
    {
      // Don't use dump_ast because we need to dump before parsing any files.
      idl_global->dump_builtins_ = true;
      idl_global->just_dump_builtins_ = true;
    }
  else if (!ACE_OS::strcmp (long_option, "dump"))
    {
      idl_global->dump_ast ();
    }
  else if (!ACE_OS::strcmp (long_option, "unknown-annotations"))
    {
      bool invalid_argument = no_more_args;
      if (no_more_args)
        {
          ACE_ERROR ((LM_ERROR,
            ACE_TEXT ("--unknown-annotations is missing its required argument.")
            ));
        }
      else
        {
          i++;
          if (!ACE_OS::strcmp (av[i], "warn-once"))
            {
              idl_global->unknown_annotations_ =
                IDL_GlobalData::UNKNOWN_ANNOTATIONS_WARN_ONCE;
            }
          else if (!ACE_OS::strcmp (av[i], "warn-all"))
            {
              idl_global->unknown_annotations_ =
                IDL_GlobalData::UNKNOWN_ANNOTATIONS_WARN_ALL;
            }
          else if (!ACE_OS::strcmp (av[i], "error"))
            {
              idl_global->unknown_annotations_ =
                IDL_GlobalData::UNKNOWN_ANNOTATIONS_ERROR;
            }
          else if (!ACE_OS::strcmp (av[i], "ignore"))
            {
              idl_global->unknown_annotations_ =
                IDL_GlobalData::UNKNOWN_ANNOTATIONS_IGNORE;
            }
          else
            {
              invalid_argument = true;
              ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("\"%C\" is not a valid argument.\n"),
                av[i]
                ));
            }
        }
      if (invalid_argument)
        {
          ACE_ERROR ((LM_ERROR,
            ACE_TEXT ("Use either \"warn-once\", \"warn-all\", ")
            ACE_TEXT ("\"error\" or \"ignore\".\n"),
            av[i]
            ));
          idl_global->parse_args_exit (1);
        }
    }
  else
    {
      be_global->parse_args (i, av);
    }
}
