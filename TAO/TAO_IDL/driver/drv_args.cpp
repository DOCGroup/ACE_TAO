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

// drv_args.cpp - Argument parsing for IDL compiler main driver

#include "idl_defines.h"
#include "global_extern.h"
#include "drv_extern.h"
#include "fe_extern.h"

#include "be_global.h"
#include "be_extern.h"
#include "be_util.h"

#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_unistd.h"
#include "ace/os_include/os_ctype.h"

extern long DRV_nfiles;
extern char *DRV_files[];

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
  char *newarg = 0;
  ACE_NEW (newarg,
           char[512]);
  char *farg;

  newarg[0] = '\0';

  for (farg = ACE_OS::strtok (s, ",");
       farg != 0;
       farg = ACE_OS::strtok (0, ","))
    {
      ACE_OS::strcat (newarg,
                      farg);
    }

  DRV_cpp_putarg (newarg);
}

// Print a usage message and exit.
void
DRV_usage (void)
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("%s: usage: %s [flag | file]*\n"),
              ACE_TEXT_CHAR_TO_TCHAR (idl_global->prog_name ()),
              ACE_TEXT_CHAR_TO_TCHAR (idl_global->prog_name ())));

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("Legal flags:\n")));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -A...\t\t\tlocal implementation-specific escape\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -Cw\t\t\tWarning if identifier spellings differ ")
      ACE_TEXT ("only in case (default is error)\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -Ce\t\t\tError if identifier spellings differ ")
      ACE_TEXT ("only in case (default)\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -ae\t\t\tError if anonymous type is seen ")
      ACE_TEXT ("(default is silent)\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -aw\t\t\tWarning if anonymous type is seen ")
      ACE_TEXT ("(default is silent)\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -as\t\t\tSilences the anonymous type diagnostic ")
      ACE_TEXT ("(default)\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -d\t\t\tOutputs (to stdout) a dump of the AST\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -Dname[=value]\t\tdefines name for preprocessor\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -E\t\t\truns preprocessor only, prints on stdout\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -Idir\t\t\tincludes dir in search path for preprocessor\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -t\t\t\tTemporary directory to be used")
      ACE_TEXT (" by the IDL compiler.\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -u\t\t\tprints usage message and exits\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -Uname\t\t\tundefines name for preprocessor\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -v\t\t\ttraces compilation stages\n")
   ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -V\t\t\tprints version info then exits\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -w\t\t\tsuppresses IDL compiler warning messages\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -Wp,<arg1,...,argn>\tpasses args to preprocessor\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -Yp,path\t\tdefines location of preprocessor\n")
    ));

  be_util::usage ();
}

// Parse arguments on command line
void
DRV_parse_args (long ac, char **av)
{
  ACE_CString buffer;
  char *s = 0;
  long i;
  bool has_space = false;

  FE_store_env_include_paths ();
  DRV_cpp_init ();
  idl_global->set_prog_name (av[0]);

  for (i = 1; i < ac; i++)
    {
      if (av[i][0] == '-')
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

              ++i;
              idl_global->set_err_count (idl_global->err_count () + 1);
              break;
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

                      idl_global->set_compile_flags (
                                      idl_global->compile_flags ()
                                      | IDL_CF_ONLY_USAGE
                                    );
                      break;
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
                  ACE_ERROR ((
                      LM_ERROR,
                      ACE_TEXT ("IDL: I don't understand")
                      ACE_TEXT (" the '%s' option\n"),
                      ACE_TEXT_CHAR_TO_TCHAR (av[i])
                    ));
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
                  ACE_ERROR ((
                      LM_ERROR,
                      ACE_TEXT ("IDL: I don't understand")
                      ACE_TEXT (" the '%s' option\n"),
                      ACE_TEXT_CHAR_TO_TCHAR (av[i])
                    ));

                  idl_global->set_compile_flags (
                                  idl_global->compile_flags ()
                                  | IDL_CF_ONLY_USAGE
                                );
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
                      has_space = FE_Utils::hasspace (av[i + 1]);

                      // If the include path has a space, we need to
                      // add literal "s.
                      ACE_CString arg = av[i];
                      arg += (has_space ? "\"" : "");
                      arg += av[i + 1];
                      arg += (has_space ? "\"" : "");

                      DRV_cpp_putarg (arg.c_str ());
                      idl_global->add_include_path (arg.substr (2).c_str (), false);
                      ++i;
                    }
                  else
                    {
                      ACE_ERROR ((
                          LM_ERROR,
                          ACE_TEXT ("IDL: I don't understand")
                          ACE_TEXT (" the '%s' option\n"),
                          ACE_TEXT_CHAR_TO_TCHAR (av[i])
                        ));

                      idl_global->set_compile_flags (
                                      idl_global->compile_flags ()
                                      | IDL_CF_ONLY_USAGE
                                    );
                      break;
                    }
                }
              else
                {
                  has_space = FE_Utils::hasspace (av[i]);

                  // If the include path has a space, we need to
                  // add literal "s.
                  ACE_CString arg (av[i], 2);
                  arg += (has_space ? "\"" : "");
                  arg += av[i] + 2;
                  arg += (has_space? "\"" : "");

                  idl_global->add_include_path (arg.substr (2).c_str (), false);
                  DRV_cpp_putarg (arg.c_str ());
                }

              break;
            case 'E':
              idl_global->set_compile_flags (idl_global->compile_flags () |
                                             IDL_CF_ONLY_PREPROC);
              break;
            case 'V':
              idl_global->set_compile_flags (idl_global->compile_flags () |
                                             IDL_CF_VERSION);
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
                      ACE_ERROR ((
                          LM_ERROR,
                          ACE_TEXT ("IDL: I don't understand")
                          ACE_TEXT (" the '%s' option\n"),
                          ACE_TEXT_CHAR_TO_TCHAR (av[i])
                        ));

                      idl_global->set_compile_flags (
                                      idl_global->compile_flags ()
                                      | IDL_CF_ONLY_USAGE
                                    );
                      break;
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

                  idl_global->set_compile_flags (
                                  idl_global->compile_flags ()
                                  | IDL_CF_ONLY_USAGE
                                );
                  break;
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
                      ACE_ERROR ((
                          LM_ERROR,
                          ACE_TEXT ("IDL: I don't understand")
                          ACE_TEXT (" the '%s' option\n"),
                          ACE_TEXT_CHAR_TO_TCHAR (av[i])
                        ));

                      idl_global->set_compile_flags (
                                      idl_global->compile_flags ()
                                      | IDL_CF_ONLY_USAGE
                                    );
                      break;
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

                        idl_global->set_compile_flags (
                                        idl_global->compile_flags ()
                                        | IDL_CF_ONLY_USAGE
                                      );
                      }

                    break;
                  default:
                    ACE_ERROR ((
                        LM_ERROR,
                        ACE_TEXT ("IDL: I dont' understand the use of")
                        ACE_TEXT (" %s with the '-Y' option\n"),
                        ACE_TEXT_CHAR_TO_TCHAR (s)
                      ));

                    idl_global->set_compile_flags (
                                    idl_global->compile_flags ()
                                    | IDL_CF_ONLY_USAGE
                                  );
                   break;
                }
              break;
            case 'd':
              idl_global->set_compile_flags (idl_global->compile_flags ()
                                             | IDL_CF_DUMP_AST);
              break;
            case 'u':
              idl_global->set_compile_flags (idl_global->compile_flags ()
                                             | IDL_CF_ONLY_USAGE);
              break;
            case 'v':
              idl_global->set_compile_flags (idl_global->compile_flags ()
                                             | IDL_CF_INFORMATIVE);
              break;
            case 'w':
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
                  ACE_ERROR ((
                      LM_ERROR,
                      ACE_TEXT ("IDL: I don't understand the '%s' option\n"),
                      ACE_TEXT_CHAR_TO_TCHAR (av[i])
                    ));

                  idl_global->set_compile_flags (
                                  idl_global->compile_flags ()
                                  | IDL_CF_ONLY_USAGE
                                );
                }

              break;
            default:
              be_global->parse_args (i, av);
              break;
            } // End of switch (av[i][1])
        } // End of IF (av[i][0] == '-')
      else
        {
          DRV_push_file (av[i]);
        }
    } // End of FOR (i = 1; i < ac; i++)

  be_util::arg_post_proc ();

  // Make sure the output directory is valid.
  if (idl_global->temp_dir () == 0)
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
