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

// drv_args.c - Argument parsing for IDL compiler main driver

#include "idl_defines.h"
#include "global_extern.h"
#include "drv_extern.h"
#include "be_global.h"
#include "be_extern.h"
#include "ace/Process.h"

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

// Prepare an argument for a BE
void
DRV_prep_be_arg (char *s,
                 idl_bool)
{
  const char arg_macro[] = "export_macro=";
  const char arg_include[] = "export_include=";
  const char skel_arg_macro[] = "skel_export_macro=";
  const char skel_arg_include[] = "skel_export_include=";
  const char stub_arg_macro[] = "stub_export_macro=";
  const char stub_arg_include[] = "stub_export_include=";
  const char arg_pch_include[] = "pch_include=";
  const char arg_pre_include[] = "pre_include=";
  const char arg_post_include[] = "post_include=";
  const char obv_opt_accessor[] = "obv_opt_accessor";

  char* last = 0;

  for (char* arg = ACE_OS::strtok_r (s, ",", &last);
       arg != 0;
       arg = ACE_OS::strtok_r (0, ",", &last))
    {
      if (ACE_OS::strstr (arg, arg_macro) == arg)
        {
          char* val = arg + sizeof (arg_macro) - 1;
          be_global->skel_export_macro (val);
          be_global->stub_export_macro (val);
        }
      else if (ACE_OS::strstr (arg, arg_include) == arg)
        {
          char* val = arg + sizeof (arg_include) - 1;
          be_global->stub_export_include (val);
        }
      else if (ACE_OS::strstr (arg, skel_arg_macro) == arg)
        {
          char* val = arg + sizeof (skel_arg_macro) - 1;
          be_global->skel_export_macro (val);
        }
      else if (ACE_OS::strstr (arg, skel_arg_include) == arg)
        {
          char* val = arg + sizeof (skel_arg_include) - 1;
          be_global->skel_export_include (val);
        }
      else if (ACE_OS::strstr (arg, stub_arg_macro) == arg)
        {
          char* val = arg + sizeof (stub_arg_macro) - 1;
          be_global->stub_export_macro (val);
        }
      else if (ACE_OS::strstr (arg, stub_arg_include) == arg)
        {
          char* val = arg + sizeof (stub_arg_include) - 1;
          be_global->stub_export_include (val);
        }
      else if (ACE_OS::strstr (arg, arg_pch_include) == arg)
        {
          char* val = arg + sizeof (arg_pch_include) - 1;
          be_global->pch_include (val);
        }
      else if (ACE_OS::strstr (arg, arg_pre_include) == arg)
        {
          char* val = arg + sizeof (arg_pre_include) - 1;
          be_global->pre_include (val);
        }
      else if (ACE_OS::strstr (arg, arg_post_include) == arg)
        {
          char* val = arg + sizeof (arg_post_include) - 1;
          be_global->post_include (val);
        }
      else if (ACE_OS::strstr (arg, obv_opt_accessor) == arg)
        {
          be_global->obv_opt_accessor (1);
        }
      else
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("%s%s%s%s"),
                      idl_global->prog_name (),
                      ACE_TEXT (": invalid or unknown argument <"),
                      arg,
                      ACE_TEXT ("> to back end\n")));
        }
    }
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
      ACE_TEXT (" -bback_end\t\tcauses specified back end to be used\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -ci\t\t\tClient inline file name ending. Default is C.i\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -cs\t\t\tClient stub's file name ending.")
      ACE_TEXT (" Default is C.cpp\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -d\t\tOutputs (to stdout) a dump of the AST\n")
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
      ACE_TEXT (" -GC \t\tGenerate the code for using AMI Call back model\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -Gd \t\tGenerate the code for direct collocation. Default")
      ACE_TEXT ("is thru-POA collocation\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -Ge [0|1]\t\t\tDisable/Enable generation of")
      ACE_TEXT (" CORBA::Environment arguments (disabled by default")
      ACE_TEXT (" if ACE_HAS_EXCEPTIONS)\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -GI[h|s|b|e|c]\tGenerate Implemenation Files \n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT ("  \t\t\th - Implementation header file name ending.")
      ACE_TEXT (" Default is I.h \n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT ("  \t\t\ts - Implementation skeleton file name ending.")
      ACE_TEXT (" Default is I.cpp\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT ("  \t\t\tb - Prefix to the implementation class names.")
      ACE_TEXT (" Default is 'no prefix' \n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT ("  \t\t\te - Suffix to the implementation class names.")
      ACE_TEXT (" Default is _i\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -Gm \t\tEnable support for CORBA components\n")
      ACE_TEXT (" (default)\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -Gp \t\tGenerate the code for thru-POA collocation")
      ACE_TEXT (" (default)\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -Gsp \t\tGenerate the code for Smart Proxies\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -Gt\t\t\tenable optimized TypeCode support")
      ACE_TEXT (" (unopt by default)\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -Gv\t\t\tenable OBV (Valuetype) support")
      ACE_TEXT (" (disabled by default)\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -hc\t\t\tClient's header file name ending.")
      ACE_TEXT (" Default is C.h\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -hs\t\t\tServer's header file name ending.")
      ACE_TEXT (" Default is S.h\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -hT\t\t\tServer's template hdr file name ending.")
      ACE_TEXT (" Default is S_T.h\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -H perfect_hash\t\tTo force perfect hashed operation")
      ACE_TEXT (" lookup strategy (default)\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -H dynamic_hash\t\tTo force dynamic hashed operation")
      ACE_TEXT (" lookup strategy. Default is perfect hashing\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -H linear_search\t\tTo force linear search operation")
      ACE_TEXT (" lookup strategy\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -H binary_search\t\tTo force binary search operation")
      ACE_TEXT (" lookup strategy\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -in \t\t\tTo generate <>s for standard #include'd")
      ACE_TEXT (" files (non-changing files)\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -ic \t\t\tTo generate \"\"s for standard #include'd")
      ACE_TEXT (" files (changing files) <\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -Idir\t\t\tincludes dir in search path for preprocessor\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -o <output_dir>\tOutput directory for the generated files.")
      ACE_TEXT (" Default is current directory\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -si\t\t\tServer's inline file name ending.")
      ACE_TEXT (" Default is S.i\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -ss\t\t\tServer's skeleton file name ending.")
      ACE_TEXT (" Default is S.cpp\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -st\t\t\tServer's template inline file name ending.")
      ACE_TEXT (" Default S_T.i\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -sT\t\t\tServer's template skeleton file name ending.")
      ACE_TEXT (" Default is S_T.cpp\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -Sa\t\t\tsuppress Any support")
      ACE_TEXT (" (support enabled by default)\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -St\t\t\tsuppress TypeCode support")
      ACE_TEXT (" (support enabled by default)\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -Sc\t\t\tsuppress tie class (and file)")
      ACE_TEXT (" generation (enabled by default)\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -Sp\t\t\tsuppress generating Thru POA collocated")
      ACE_TEXT (" stubs (enabled by default)\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -Sd\t\t\tsuppress generating Direct collocated")
      ACE_TEXT (" stubs (disable by default)\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -Sv\t\t\tdisable OBV (Valuetype) support")
      ACE_TEXT (" (disabled by default)\n")
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
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT ("    \t\t\tNo effect if TypeCode generation is suppressed\n")
    ));
}

// Return 0 on success, -1 failure. The <errno> corresponding to the
// error that caused the GPERF execution is also set.
int
DRV_check_gperf (void)
{
  // If absolute path is not specified yet, let us call just
  // "gperf". Hopefully PATH is set up correctly to locate the gperf.
  if (idl_global->gperf_path () == 0)
    {
      // If ACE_GPERF is defined then use that gperf program instead of "gperf."
#if defined (ACE_GPERF)
      idl_global->gperf_path (ACE_GPERF);
#else
      idl_global->gperf_path ("gperf");
#endif /* ACE_GPERF */
    }

  // If we have absolute path for the <gperf> rather than just the
  // executable name <gperf>, make sure the file exists
  // firsts. Otherwise just call <gperf>. Probably PATH is set
  // correctly to take care of this.

  // If ACE_GPERF is defined then use that gperf program instead of "gperf."
#if defined (ACE_GPERF)
  if (ACE_OS::strcmp (idl_global->gperf_path (), ACE_GPERF) != 0)
#else
  if (ACE_OS::strcmp (idl_global->gperf_path (), "gperf") != 0)
#endif /* ACE_GPERF */
    {
      // It is absolute path. Check the existance, permissions and
      // the modes.
      if (ACE_OS::access (idl_global->gperf_path (),
                          F_OK | X_OK) == -1)
        {
          // Problem with the file. No point in having the absolute
          // path. Swith to "gperf".
          // If ACE_GPERF is defined then use that gperf program
          //instead of "gperf."
#if defined (ACE_GPERF)
          idl_global->gperf_path (ACE_GPERF);
#else
          idl_global->gperf_path ("gperf");
#endif /* ACE_GPERF */
        }
    }

  // Just call gperf in silent mode. It will come and immly exit.

  // Using ACE_Process.
  ACE_Process process;
  ACE_Process_Options process_options;

  // Set the command line for the gperf program.
  process_options.command_line ("%s"
                                " "
                                "-V",
                                idl_global->gperf_path ());

  // Spawn a process for gperf.
  if (process.spawn (process_options) == -1)
    {
      return -1;
    }

#if defined (ACE_WIN32)
  // No wait or anything in Win32.
  return 0;
#endif /* ACE_WIN32 */

  // Wait for gperf to complete.
  ACE_exitcode wait_status = 0;
  if (process.wait (&wait_status) == -1)
    {
      return -1;
    }
  else
    {
      // Wait is sucessful, we will check the exit code from the
      // spawned process.
      if (WIFEXITED (wait_status))
        {
          // Normal exit.

          // Check the exit value of the spawned process. ACE_Process
          // exits with <errno> as exit code, if it is not able to
          // exec gperf program, so get the exit code now and set that
          // to <errno> again, so that it can be used to print error
          // messages.
          errno = WEXITSTATUS (wait_status);
          if (errno)
            {
              // <exec> has failed.
              return -1;
            }
          else
            {
              // Everything was alright.
              return 0;
            }
        }
      else
        {
          // Not a normal exit. No <errno> might be set.
          return -1;
        }
    }
}

// Parse arguments on command line
void
DRV_parse_args (long ac, char **av)
{
  char *buffer;
  char *s = 0;
  long i;

  DRV_store_env_include_paths ();
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

              // = File name endings for all the IDL generated header files,
              //   stub files, skeleton files and inline files.

              // = Various 'h'eader_file_name_endings.
            case 'h':

              // <-hc Client's header file name ending>
              //      Default is "C.h".
              // <-hs Server's header file name ending>
              //       Default is "S.h".
              // <-hT Server's template hdr file name ending>
              //       Default is "S_T.h".
              // <-hI Server's implementation header file name ending>
              //       Default is "I.h".

              if (av[i][2] == 'c')
                {
                  // Client stub's header file ending.
                  // @@ No error handling done here.
                  idl_global->append_idl_flag (av[i + 1]);
                  be_global->client_hdr_ending (av[i + 1]);
                  i++;
                }
              else if (av[i][2] == 's')
                {
                  // Server skeleton's header file.
                  idl_global->append_idl_flag (av[i + 1]);
                  be_global->server_hdr_ending (av[i + 1]);
                  i++;
                }
              else if (av[i][2] == 'T')
                {
                  // Server Template header ending.
                  idl_global->append_idl_flag (av[i + 1]);
                  be_global->server_template_hdr_ending (av[i + 1]);
                  i++;
                }
              else if (av[i][2] == 'I')
                {
                  // Server Template header ending.
                  idl_global->append_idl_flag (av[i + 1]);
                  be_global->implementation_hdr_ending (av[i + 1]);
                  i++;
                }
              else
                {
                  // I expect 'c' or 's' or 'T' after this.
                  ACE_ERROR ((
                      LM_ERROR,
                      ACE_TEXT ("IDL: I don't understand the '%s' option\n"),
                      av[i]
                    ));

                  ACE_OS::exit (99);
                }
              break;

              // = Various 'c'lient side stub file_name_endings.
            case 'c':
              // <-cs Client stub's file name ending>
              //      Default is "C.cpp".
              // <-ci Client inline file name ending>
              //      Default is "C.i".

              if (av[i][2] == 's')
                {
                  idl_global->append_idl_flag (av[i + 1]);
                  be_global->client_stub_ending (av[i + 1]);
                  i++;
                }

              else if (av[i][2] == 'i')
                {
                  idl_global->append_idl_flag (av[i + 1]);
                  be_global->client_inline_ending (av[i + 1]);
                  i++;
                }

              else
                {
                  // I expect 's' or 'i' after 'c'.
                  ACE_ERROR ((
                      LM_ERROR,
                      ACE_TEXT ("IDL: I don't understand the '%s' option\n"),
                      av[i]
                    ));

                  ACE_OS::exit (99);
                }
              break;

              // = Various 's'erver side skeleton file name endings.
            case 's':
              // <-ss Server's skeleton file name ending>
              //      Default is "S.cpp".
              // <-sT Server's template skeleton file name ending>
              //      Default is "S_T.cpp".
              // <-si Server's inline file name ending>
              //      Default is "S.i".
              // <-st Server's template inline file name ending>
              //      Default is "S_T.i".
              // <-sI Server's implementation skeleton file name ending>
              //      Default is "I.cpp".

              if (av[i][2] == 's')
                {
                  idl_global->append_idl_flag (av[i + 1]);
                  be_global->server_skeleton_ending (av[i + 1]);
                  i++;
                }
              else if (av[i][2] == 'T')
                {
                  idl_global->append_idl_flag (av[i + 1]);
                  be_global->server_template_skeleton_ending (av[i + 1]);
                  i++;
                }
              else if (av[i][2] == 'i')
                {
                  idl_global->append_idl_flag (av[i + 1]);
                  be_global->server_inline_ending (av[i + 1]);
                  i++;
                }
              else if (av[i][2] == 't')
                {
                  idl_global->append_idl_flag (av[i + 1]);
                  be_global->server_template_inline_ending (av[i + 1]);
                  i++;
                }

              else if (av[i][2] == 'I')
                {
                  idl_global->append_idl_flag (av[i + 1]);
                  be_global->implementation_skel_ending (av[i + 1]);
                  i++;
                }

              else
                {
                  // I expect 's' or 'T' or 'i' or 't' after 's'.
                  ACE_ERROR ((
                      LM_ERROR,
                      ACE_TEXT ("IDL: I don't understand the '%s' option\n"),
                      av[i]
                    ));

                  ACE_OS::exit (99);
                }
              break;

              // Operation lookup strategy.
              // <perfect_hash>, <dynamic_hash> or <binary_search>
              // Default is perfect.
            case 'H':
              idl_global->append_idl_flag (av[i + 1]);

              if (av[i+1] == 0 || av[i+1][0] == '-')
                {
                  ACE_ERROR ((LM_ERROR,
                              ACE_TEXT ("no selection for -H option\n")));
                  ACE_OS::exit (99);
                }
              else if (ACE_OS::strcmp (av[i+1], "dynamic_hash") == 0)
                {
                  be_global->lookup_strategy (
                      BE_GlobalData::TAO_DYNAMIC_HASH
                    );
                }
              else if (ACE_OS::strcmp (av[i + 1], "perfect_hash") == 0)
                {
                  be_global->lookup_strategy (
                      BE_GlobalData::TAO_PERFECT_HASH
                    );
                }
              else if (ACE_OS::strcmp (av[i + 1], "binary_search") == 0)
                {
                  be_global->lookup_strategy (
                      BE_GlobalData::TAO_BINARY_SEARCH
                    );
                }
              else if (ACE_OS::strcmp (av[i + 1], "linear_search") == 0)
                {
                  be_global->lookup_strategy (
                      BE_GlobalData::TAO_LINEAR_SEARCH
                    );
                }
              else
                {
                  ACE_ERROR ((LM_ERROR,
                              ACE_TEXT ("%s: unknown operation lookup <%s>\n"),
                              av[0],
                              av[i + 1]));
                  ACE_OS::exit (99);
                }

              i++;
              break;

              // Switching between ""s and <>s when we generate
              // #include statements for the standard files (e.g. tao/corba.h)
            case 'i':
              if (av[i][2] == 'c')
                {
                  be_global->changing_standard_include_files (1);
                }
              else if (av[i][2] == 'n')
                {
                  be_global->changing_standard_include_files (0);
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

              // Path for the perfect hash generator(gperf) program. Default
              // is $ACE_ROOT/bin/gperf.
            case 'g':
              if (av[i][2] == '\0')
                {
                  idl_global->append_idl_flag (av[i + 1]);
                  idl_global->gperf_path (av[i + 1]);
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

              // Option to generate the features regarding the
              // Messaging Specification.

              // Directory where all the IDL-Compiler-Generated files are to
              // be kept. Default is the current directory from which the
              // <tao_idl> is called.
            case 'o':
              if (av[i][2] == '\0')
                {
                  idl_global->append_idl_flag (av[i + 1]);
                  be_global->output_dir (av [i + 1]);
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
                      DRV_prep_be_arg (s + 2, I_TRUE);
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

            case 'b':
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

              idl_global->set_be (s);
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
            case 'S':
              // suppress generation of ...
              if (av[i][2] == 'a')
                {
                  // suppress Any support
                  be_global->any_support (0);
                }
              else if (av[i][2] == 't')
                {
                  // suppress typecode support
                  // Anys must be suppressed as well
                  be_global->tc_support (0);
                  be_global->any_support (0);
                }
              else if (av[i][2] == 'p')
                {
                  // suppress generating Thru_POA collocated stubs
                  be_global->gen_thru_poa_collocation (0);
                }
              else if (av[i][2] == 'd')
                {
                  // suppress generating Direct collocated stubs
                  be_global->gen_direct_collocation (0);
                }
              else if (av[i][2] == 'c')
                {
                  // suppress generating tie classes and files
                  be_global->gen_tie_classes (0);
                }
              else if (av[i][2] == 'v')
                {
                  // disable OBV (Valuetype) support
#                 ifdef IDL_HAS_VALUETYPE
                  idl_global->obv_support (0);
#                 endif
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
            case 'G':
              // Enable generation of ...
              if (av[i][2] == 'C')
                {
                  // AMI with Call back.
                  be_global->ami_call_back (I_TRUE);
                }
              else if (av[i][2] == 'H')
                {
                  // AMH classes.
                  be_global->gen_amh_classes (I_TRUE);
                }
              else if (av[i][2] == 'e')
                {
                  idl_global->append_idl_flag (av[i+1]);
                  int option = ACE_OS::atoi (av[i+1]);

                  // exception support
                  be_global->exception_support (option == 0
                                                 || option == 2);

                  // use of raw 'throw'
                  be_global->use_raw_throw (option == 2);

                  i++;
                }
              else if (av[i][2] == 's')
                {
                  if (av[i][3] == 'p')
                    {
                      // smart proxies
                      be_global->gen_smart_proxies (I_TRUE);
                    }
                  else
                    {
                      ACE_ERROR ((
                          LM_ERROR,
                          ACE_TEXT ("IDL: I don't understand ")
                          ACE_TEXT ("the '%s' option\n"),
                          av[i]
                        ));

                      ACE_OS::exit (99);
                    }

                  break;
                }
              else if (av[i][2] == 'i')
                {
                  if (av[i][3] == 'c')
                    {
                      // inline constants
                      be_global->gen_inline_constants (I_TRUE);
                    }
                  else
                    {
                      ACE_ERROR ((
                          LM_ERROR,
                          ACE_TEXT ("IDL: I don't understand ")
                          ACE_TEXT ("the '%s' option\n"),
                          av[i]
                        ));

                      ACE_OS::exit (99);
                    }

                  break;
                }
              else if (av[i][2] == 't')
                {
                  // optimized typecode support
                  be_global->opt_tc (1);
                }
              else if (av[i][2] == 'p')
                {
                  // generating Thru_POA collocated stubs.
                  be_global->gen_thru_poa_collocation (1);
                }
              else if (av[i][2] == 'd')
                {
                  // generating Direct collocated stubs.
                  be_global->gen_direct_collocation (1);
                }
              else if (av[i][2] == 'v')
                {
                  // enable OBV (Valuetype) support
                  idl_global->obv_support (1);
                }
              else if (av[i][2] == 'I')
                {
                  int options = ACE_OS::strlen(av[i]) - 3;
                  int j;
                  int k = i;
                  // optimized typecode support
                  be_global->gen_impl_files (1);

                  for (j = 0; j < options; ++j)
                    {
                      if (av[k][j + 3] == 's')
                        {
                          idl_global->append_idl_flag (av[i + 1]);
                          be_global->implementation_skel_ending (av[i + 1]);
                          i++;
                        }
                      else if (av[k][j + 3] == 'h')
                        {
                          idl_global->append_idl_flag (av[i + 1]);
                          be_global->implementation_hdr_ending (av[i + 1]);
                          i++;
                        }
                      else if (av[k][j + 3] == 'b')
                        {
                          idl_global->append_idl_flag (av[i + 1]);
                          be_global->impl_class_prefix (av[i + 1]);
                          i++;
                        }
                      else if (av[k][j + 3] == 'e')
                        {
                          idl_global->append_idl_flag (av[i + 1]);
                          be_global->impl_class_suffix (av[i + 1]);
                          i++;
                        }
                      else if (av[k][j + 3] == 'c')
                        {
                          be_global->gen_copy_ctor (1);
                        }
                      else if (av[k][j + 3] == 'a')
                        {
                          be_global->gen_assign_op (1);
                        }
                      else if (isalpha (av[k][j + 3] ))
                        {
                          ACE_ERROR ((
                              LM_ERROR,
                              ACE_TEXT ("IDL: I don't understand")
                              ACE_TEXT (" the '%s' option\n"),
                              av[i]
                            ));

                          idl_global->set_compile_flags (
                              idl_global->compile_flags ()
                              | IDL_CF_ONLY_USAGE
                            );
                        }
                    }
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
              ACE_ERROR ((
                  LM_ERROR,
                  ACE_TEXT ("IDL: I don't understand the '%s' option\n"),
                  av[i]
                ));

              idl_global->set_compile_flags (idl_global->compile_flags () |
                                             IDL_CF_ONLY_USAGE);
              break;
            }
        }
      else
        DRV_push_file (av[i]);
    }


  // Let us try to use Perfect Hashing Operation Lookup Strategy. Let
  // us check whether things are fine with GPERF.
#if defined (ACE_HAS_GPERF)
  // If Perfect Hashing or Binary Search or Linear Search strategies
  // have been selected, let us make sure that it exists and will
  // work.
  if ((be_global->lookup_strategy () == BE_GlobalData::TAO_PERFECT_HASH) ||
      (be_global->lookup_strategy () == BE_GlobalData::TAO_BINARY_SEARCH) ||
      (be_global->lookup_strategy () == BE_GlobalData::TAO_LINEAR_SEARCH))
    {
      // Testing whether GPERF works or no.
      int return_value = DRV_check_gperf ();
      if (return_value == -1)
        {
          // If gperf_path is an absolute path, try to call this
          // again with
          ACE_DEBUG ((
              LM_DEBUG,
              ACE_TEXT ("TAO_IDL: warning, GPERF could not be executed\n")
              ACE_TEXT ("Perfect Hashing or Binary/Linear Search cannot be")
              ACE_TEXT (" done without GPERF\n")
              ACE_TEXT ("Now, using Dynamic Hashing..\n")
              ACE_TEXT ("To use Perfect Hashing or Binary/Linear")
              ACE_TEXT (" Search strategy\n")
              ACE_TEXT ("\t-Build gperf at $ACE_ROOT/apps/gperf/src\n")
              ACE_TEXT ("\t-Set the environment variable $ACE_ROOT")
              ACE_TEXT (" appropriately or add $ACE_ROOT/bin to the PATH\n")
              ACE_TEXT ("\t-Refer to Operation Lookup section in the TAO IDL")
              ACE_TEXT (" User Guide ($TAO_ROOT/docs/compiler.html)")
              ACE_TEXT (" for more details\n")
            ));

          // Switching over to Dynamic Hashing.
          be_global->lookup_strategy (BE_GlobalData::TAO_DYNAMIC_HASH);
        }
    }
#else /* Not ACE_HAS_GPERF */
  // If GPERF is not there, we cannot use PERFECT_HASH strategy. Let
  // us go for DYNAMIC_HASH.
  if ((be_global->lookup_strategy () == BE_GlobalData::TAO_PERFECT_HASH) ||
      (be_global->lookup_strategy () == BE_GlobalData::TAO_BINARY_SEARCH) ||
      (be_global->lookup_strategy () == BE_GlobalData::TAO_LINEAR_SEARCH))
    {
      be_global->lookup_strategy (BE_GlobalData::TAO_DYNAMIC_HASH);
    }
#endif /* ACE_HAS_GPERF */

  // make sure that we are not suppressing TypeCode generation and asking for
  // optimized typecode support at the same time
  if (!be_global->tc_support () && be_global->opt_tc ())
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("Bad Combination -St and -Go \n")));

      ACE_OS::exit (99);
    }

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
