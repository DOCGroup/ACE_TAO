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
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_unistd.h"
#include "ace/os_include/os_ctype.h"

ACE_RCSID (driver,
           drv_args,
           "$Id$")

extern long DRV_nfiles;
extern const char *DRV_files[];

// Push a file into the list of files to be processed
static void
DRV_push_file (const char *s)
{
  DRV_files[DRV_nfiles++] = s;
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
              ACE_TEXT ("%s%s%s%s"),
              idl_global->prog_name (),
              ACE_TEXT (": usage: "),
              idl_global->prog_name (),
              ACE_TEXT (" [flag | file]*\n")));

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
      ACE_TEXT (" -g <gperf_path>\tPath for the GPERF program.")
      ACE_TEXT (" Default is $ACE_ROOT/bin/gperf\n")));
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
      ACE_TEXT (" -W[p|b],arg1,argn\tpasses args to preprocessor or BE\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -Yp,path\t\tdefines location of preprocessor\n")
    ));
    
  be_global->usage ();
}

// Parse arguments on command line
void
DRV_parse_args (long ac, char **av)
{
  char *buffer;
  char *s = 0;
  long i;

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

              ACE_OS::exit (99);
            case 'A':
              if (av[i][2] == '\0')
                {
                  if (i < ac - 1)
                    {
                      i++;
                      s = av[i];
                    }
                  else
                    {
                      ACE_OS::exit (99);
                    }
                }
              else
                {
                  s = av[i] + 2;
                }

              ACE_OS::strcat (idl_global->local_escapes (), s);
              ACE_OS::strcat (idl_global->local_escapes (), " ");
              break;
            // Temp directory for the IDL compiler to keep its files.
            case 't':
              if (av[i][2] == '\0')
                {
                  idl_global->append_idl_flag (av[i + 1]);
                  idl_global->temp_dir (av [i + 1]);
                  i++;
                }
              else
                {
                  ACE_ERROR ((
                      LM_ERROR,
                      ACE_TEXT ("IDL: I don't understand")
                      ACE_TEXT (" the '%s' option\n"),
                      av[i]
                    ));

                  ACE_OS::exit (99);
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

                      ACE_NEW (buffer,
                               char[ACE_OS::strlen (av[i])
                                    + ACE_OS::strlen (av[i + 1])
                                    + 2]);

                      ACE_OS::sprintf (buffer,
                                       "%s%s",
                                       av[i],
                                       av[i + 1]);

                      DRV_cpp_putarg (buffer);
                      idl_global->add_include_path (buffer + 2);
                      i++;
                    }
                  else
                    {
                      ACE_ERROR ((
                          LM_ERROR,
                          ACE_TEXT ("IDL: I don't understand")
                          ACE_TEXT (" the '%s' option\n"),
                          av[i]
                        ));

                      ACE_OS::exit (99);
                    }
                }
              else
                {
                  idl_global->add_include_path (av[i] + 2);
                  DRV_cpp_putarg (av[i]);
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
                      i++;
                      s = av[i];
                    }
                  else
                    {
                      ACE_ERROR ((
                          LM_ERROR,
                          ACE_TEXT ("IDL: I don't understand")
                          ACE_TEXT (" the '%s' option\n"),
                          av[i]
                        ));

                      ACE_OS::exit (99);
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
                      ACE_TEXT ("IDL: -W must be followed by 'p' or 'b'\n")
                    ));
                  ACE_OS::exit (99);
                case 'p':
                  if (*(s + 1) == ',')
                    {
                      DRV_prep_cpp_arg (s + 2);
                    }
                  break;
                case 'b':
                  if (*(s + 1) == ',')
                    {
                      be_global->prep_be_arg (s + 2);
                    }
                  break;
                }
              break;
            case 'Y':
              if (av[i][2] == '\0')
                {
                  if (i < ac - 1)
                    {
                      i++;
                      s = av[i];
                    }
                  else
                    {
                      ACE_ERROR ((
                          LM_ERROR,
                          ACE_TEXT ("IDL: I don't understand")
                          ACE_TEXT (" the '%s' option\n"),
                          av[i]
                        ));

                      ACE_OS::exit (99);
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
                    break;
                  default:
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
                  idl_global->case_diff_error (I_TRUE);
                }
              else if (av[i][2] == 'w')
                {
                  // ...report a warning (default for now)
                  idl_global->case_diff_error (I_FALSE);
                }
              else
                {
                  ACE_ERROR ((
                      LM_ERROR,
                      ACE_TEXT ("IDL: I don't understand the '%s' option\n"),
                      av[i]
                    ));

                  ACE_OS::exit (99);
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

  be_global->arg_post_proc ();

  // Make sure the output directory is valid.
  if (idl_global->temp_dir () == 0)
    {
      char tmpdir[MAXPATHLEN + 1];

      if (ACE::get_temp_dir (tmpdir, MAXPATHLEN) == -1)
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("Temporary path too long, ")
                      ACE_TEXT ("defaulting to current directory\n")));

          ACE_OS::strcpy (tmpdir, ".");
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
              ACE_TEXT ("%s%s%s\n"),
              ACE_TEXT ("Can't access temporary directory ("),
              tmpdir,
              ACE_TEXT ("), using current directory for temp files.")
            ));

          ACE_OS::strcpy (tmpdir, ".");
#if defined(ACE_MVS)
          if (ACE_OS::access (tmpdir, F_OK) == -1
              || ACE_OS::access (tmpdir, R_OK) == -1
              || ACE_OS::access (tmpdir, W_OK) == -1)
#else
          if (ACE_OS::access (tmpdir, F_OK | R_OK | W_OK) == -1)
#endif /* ACE_MVS */
            {
              ACE_ERROR ((LM_ERROR,
                          "%s%s\n",
                          ACE_TEXT ("Error: Can't access temporary directory "),
                          tmpdir));

              ACE_OS::exit (99);
            }
        }

      idl_global->temp_dir (tmpdir);
    }
}
