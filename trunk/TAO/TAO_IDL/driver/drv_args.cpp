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

#include        "idl.h"
#include        "idl_extern.h"

#include        "drv_private.h"
#include        "drv_link.h"

#include        "be.h"
#include        <stdio.h>

ACE_RCSID(driver, drv_args, "$Id$")

  /*
 * Push a file into the list of files to be processed
 */
  static void
DRV_push_file(char *s)
{
  DRV_files[DRV_nfiles++] = s;
}

/*
 * Prepare a CPP argument
 */
static void
DRV_prep_cpp_arg (char *s)
{
  char *newarg = new char[512];
  char *farg;

  newarg[0] = '\0';
  for (farg = ACE_OS::strtok (s, ","); farg != NULL; farg = ACE_OS::strtok (0, ","))
    ACE_OS::strcat (newarg, farg);
  DRV_cpp_putarg (newarg);
}

/*
 * Print a usage message and exit
 */
void
DRV_usage (void)
{
  cerr << idl_global->prog_name ()
       << GTDEVEL (": usage: ")
       << idl_global->prog_name ()
       << GTDEVEL (" [flag | file]*\n");
  cerr << GTDEVEL ("Legal flags:\n");
  cerr << GTDEVEL (" -A...\t\t\tlocal implementation-specific escape\n");
  cerr << GTDEVEL (" -bback_end\t\tcauses specified back end to be used\n");
  cerr << GTDEVEL (" -ci\t\t\tClient inline file name ending. Default is C.i\n");
  cerr << GTDEVEL (" -cs\t\t\tClient stub's file name ending. Default is C.cpp\n");
  cerr << GTDEVEL (" -Dname[=value]\t\tdefines name for preprocessor\n");
  cerr << GTDEVEL (" -E\t\t\truns preprocessor only, prints on stdout\n");
  cerr << GTDEVEL (" -g <gperf_path>\tPath for the GPERF program. Default is $ACE_ROOT/bin/gperf\n");
  cerr << GTDEVEL (" -Gc\t\t\tenable Compiled marshaling (default is interpretive)\n");
  cerr << GTDEVEL (" -Gi\t\t\tenable Interpretive marshaling (default)\n");
  cerr << GTDEVEL (" -Ge\t\t\tenable C++ Exception support (suppressed by default)\n");
  cerr << GTDEVEL (" -Gt\t\t\tenable optimized TypeCode support (unopt by default)\n");
  cerr << GTDEVEL (" -GI[h|s|b|e|c]\tGenerate Implemenation Files \n");
  cerr << GTDEVEL ("  \t\t\th - Implementation header file name ending. Default is I.h \n");
  cerr << GTDEVEL ("  \t\t\ts - Implementation skeleton file name ending. Default is I.cpp\n");
  cerr << GTDEVEL ("  \t\t\tb - Prefix to the implementation class names. Default is 'no prefix' \n");
  cerr << GTDEVEL ("  \t\t\te - Suffix to the implementation class names. Default is _i\n");
  cerr << GTDEVEL ("  \t\t\tc - Generate copy construtors\n");
  //  cerr << GTDEVEL ("  \t\t\ta - Generate copy assigment operator\n");
  cerr << GTDEVEL (" -hc\t\t\tClient's header file name ending. Default is C.h\n");
  cerr << GTDEVEL (" -hs\t\t\tServer's header file name ending. Default is S.h\n");
  cerr << GTDEVEL (" -hT\t\t\tServer's template hdr file name ending. Default is S_T.h\n");
  cerr << GTDEVEL (" -H dynamic_hash\t\tTo force dynamic hashed operation lookup strategy. Default is perfect hashing\n");
  cerr << GTDEVEL (" -H linear_search\t\tTo force linear searchoperation lookup strategy\n");
  cerr << GTDEVEL (" -H binary_search\t\tTo force binary search operation lookup strategy\n");
  cerr << GTDEVEL (" -in \t\t\tTo generate <>s for standard #include'd files (non-changing files)\n");
  cerr << GTDEVEL (" -ic \t\t\tTo generate \"\"s for standard #include'd files (changing files) <\n");
  cerr << GTDEVEL (" -Idir\t\t\tincludes dir in search path for preprocessor\n");
  cerr << GTDEVEL (" -o <output_dir>\tOutput directory for the generated files. Default is current directory\n");
  cerr << GTDEVEL (" -si\t\t\tServer's inline file name ending. Default is S.i\n");
  cerr << GTDEVEL (" -ss\t\t\tServer's skeleton file name ending. Default is S.cpp\n");
  cerr << GTDEVEL (" -st\t\t\tServer's template inline file name ending. Default S_T.i\n");
  cerr << GTDEVEL (" -sT\t\t\tServer's template skeleton file name ending. Default is S_T.cpp\n");
  cerr << GTDEVEL (" -Sa\t\t\tsuppress Any support (support enabled by default)\n");
  cerr << GTDEVEL (" -St\t\t\tsuppress TypeCode support (support enabled by default)\n");
  cerr << GTDEVEL (" -u\t\t\tprints usage message and exits\n");
  cerr << GTDEVEL (" -Uname\t\t\tundefines name for preprocessor\n");
  cerr << GTDEVEL (" -v\t\t\ttraces compilation stages\n");
  cerr << GTDEVEL (" -V\t\t\tprints version info then exits\n");
  cerr << GTDEVEL (" -w\t\t\tsuppresses IDL compiler warning messages\n");
  cerr << GTDEVEL (" -W[p|b],arg1,argn\tpasses args to preprocessor or BE\n");
  cerr << GTDEVEL (" -Yp,path\t\tdefines location of preprocessor\n");
  cerr << GTDEVEL ("    \t\t\tNo effect if TypeCode generation is suppressed\n");

}

/*
 * Parse arguments on command line
 */
void
DRV_parse_args (long ac, char **av)
{
  char  *buffer;
  char  *s = 0;
  long  i;

  // Retrieve the singleton instance of the code generator.
  TAO_CodeGen *cg = TAO_CODEGEN::instance ();

  DRV_cpp_init ();
  idl_global->set_prog_name (av[0]);
  //i = 1;
  /*
  while (a[i][0] == '-')
    {
  */
  for (i = 1; i < ac; i++)
    {
      if (av[i][0] == '-')
        {
          switch (av[i][1])
            {
            case 0:
              DRV_push_file ("standard input");
              break;
            case 'A':
              if (av[i][2] == '\0')
                {
                  if (i < ac - 1)
                    {
                      i++;
                      s = av[i];
                    }
                  else
                    ACE_OS::exit (99);
                }
              else
                s = av[i] + 2;
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
                  idl_global->client_hdr_ending (av[i+1]);
                  i++;
                }
              else if (av[i][2] == 's')
                {
                  // Server skeleton's header file.
                  idl_global->server_hdr_ending (av[i+1]);
                  i++;
                }
              else if (av[i][2] == 'T')
                {
                  // Server Template header ending.
                  idl_global->server_template_hdr_ending (av[i+1]);
                  i++;
                }
              else if (av[i][2] == 'I')
                {
                  // Server Template header ending.
                  idl_global->implementation_hdr_ending (av[i+1]);
                  i++;
                }
              else
                {
                  // I expect 'c' or 's' or 'T' after this.
                  cerr << GTDEVEL ("Incomplete Flag : ")
                       << av[i];
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
                  idl_global->client_stub_ending (av[i+1]);
                  i++;
                }

              else if (av[i][2] == 'i')
                {
                  idl_global->client_inline_ending (av[i+1]);
                  i++;
                }

              else
                {
                  // I expect 's' or 'i' after 'c'.
                  cerr << GTDEVEL("Incomplete Flag : ")
                       << av[i];
                  exit(99);
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
                  idl_global->server_skeleton_ending (av[i+1]);
                  i++;
                }
              else if (av[i][2] == 'T')
                {
                  idl_global->server_template_skeleton_ending (av[i+1]);
                  i++;
                }
              else if (av[i][2] == 'i')
                {
                  idl_global->server_inline_ending (av[i+1]);
                  i++;
                }
              else if (av[i][2] == 't')
                {
                  idl_global->server_template_inline_ending (av[i+1]);
                  i++;
                }

              else if (av[i][2] == 'I')
                {
                  idl_global->implementation_skel_ending (av[i+1]);
                  i++;
                }

              else
                {
                  // I expect 's' or 'T' or 'i' or 't' after 's'.
                  cerr << GTDEVEL ("Incomplete Flag : ")
                       << av[i];
                  ACE_OS::exit (99);
                }
              break;

              // Operation lookup strategy.
              // <perfect_hash>, <dynamic_hash> or <binary_search>
              // Default is perfect.
            case 'H':
              if (ACE_OS::strcmp (av[i+1], "dynamic_hash") == 0)
                cg->lookup_strategy (TAO_CodeGen::TAO_DYNAMIC_HASH);
              else if (ACE_OS::strcmp (av[i+1], "perfect_hash") == 0)
                cg->lookup_strategy (TAO_CodeGen::TAO_PERFECT_HASH);
              else if (ACE_OS::strcmp (av[i+1], "binary_search") == 0)
                cg->lookup_strategy (TAO_CodeGen::TAO_BINARY_SEARCH);
              else if (ACE_OS::strcmp (av[i+1], "linear_search") == 0)
                cg->lookup_strategy (TAO_CodeGen::TAO_LINEAR_SEARCH);
              else
                ACE_ERROR ((LM_ERROR,
                            "%s: unknown operation lookup <%s>\n",
                            av[0], av[i+1]));
              i++;
              break;

              // Switching between ""s and <>'s when we generate
              // #include statements for the standard files (e.g. tao/corba.h)
            case 'i':
              if (av[i][2] == 'c')
                idl_global->changing_standard_include_files (1);
              else if (av[i][2] == 'n')
                idl_global->changing_standard_include_files (0);
              else
                ACE_ERROR ((LM_ERROR,
                            "%s: unknown -i modifier <%c>\n",
                            av[0], av[i][2]));
              break;

              // Path for the perfect hash generator(gperf) program. Default
              // is $ACE_ROOT/bin/gperf.
            case 'g':
              idl_global->gperf_path (av[i+1]);
              i++;
              break;

              // Directory where all the IDL-Compiler-Generated files are to
              // be kept. Default is the current directory from which the
              // <tao_idl> is called.
            case 'o':
              idl_global->output_dir (av [i+1]);
              i++;
              break;
            
              // Temp directory for the IDL compiler to keep its files.               
            case 't':
              idl_global->temp_dir (av [i+1]);
              i++;
              break;
              
            case 'D':
            case 'U':
            case 'I':
              if (av[i][2] == '\0')
                {
                  if (i < ac - 1)
                    {
                      buffer = new char[ACE_OS::strlen (av[i]) + ACE_OS::strlen (av[i + 1]) + 2];
                      ACE_OS::sprintf (buffer, "%s%s", av[i], av[i+1]);
                      DRV_cpp_putarg (buffer);
                      i++;
                    }
                  else
                    {
                      cerr << GTDEVEL ("IDL: missing argument after '")
                           << av[i]
                           << GTDEVEL ("' flag\n");
                      ACE_OS::exit (99);
                    }
                }
              else
                DRV_cpp_putarg (av[i]);
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
                      cerr << GTDEVEL ("IDL: missing argument after '")
                           << av[i]
                           << GTDEVEL ("' flag\n");
                      ACE_OS::exit (99);
                    }
                }
              else
                s = av[i] + 2;
              switch (*s)
                {
                default:
                  cerr << GTDEVEL ("IDL: -W must be followed by 'p' or 'b'\n");
                  ACE_OS::exit (99);
                case 'p':
                  if (*(s + 1) == ',')
                    DRV_prep_cpp_arg (s + 2);
                  break;
                case 'b':
                  if (*(s + 1) == ',')
                    (*DRV_BE_prep_arg) (s + 2, I_TRUE);
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
                      cerr << GTDEVEL ("IDL: missing argument after '")
                           << av[i]
                           << GTDEVEL ("' flag\n");
                      ACE_OS::exit (99);
                    }
                }
              else
                s = av[i] + 2;
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
                      cerr << GTDEVEL("IDL: missing argument after '")
                           << av[i]
                           << GTDEVEL("' flag\n");
                      ACE_OS::exit (99);
                    }
                }
              else
                s = av[i] + 2;
              idl_global->set_be (s);
              break;

            case 'd':
              idl_global->set_compile_flags (idl_global->compile_flags () |
                                             IDL_CF_DUMP_AST);
              break;
            case 'u':
              idl_global->set_compile_flags (idl_global->compile_flags () |
                                             IDL_CF_ONLY_USAGE);
              break;
            case 'v':
              idl_global->set_compile_flags (idl_global->compile_flags () |
                                             IDL_CF_INFORMATIVE);
              break;
            case 'w':
              idl_global->set_compile_flags (idl_global->compile_flags () |
                                             IDL_CF_NOWARNINGS);
              break;
            case 'S':
              // suppress generation of ...
              if (av[i][2] == 'a')
                {
                  // suppress Any support
                  idl_global->any_support (0);
                }
              else if (av[i][2] == 't')
                {
                  // supress typecode support
                  idl_global->tc_support (0);
                }
              else
                {
                  cerr << GTDEVEL("IDL: missing argument after '")
                       << av[i]
                       << GTDEVEL("' flag\n");
                  ACE_OS::exit (99);
                }
              break;

            case 'G':
              // enable generation of ...
              if (av[i][2] == 'c')
                {
                  // compiled marshaling support
                  idl_global->compiled_marshaling (1);
                }
              else if (av[i][2] == 'i')
                {
                  // interpretive marshaling support
                  idl_global->compiled_marshaling (0);
                }
              else if (av[i][2] == 'e')
                {
                  // exception support
                  idl_global->exception_support (1);
                }
              else if (av[i][2] == 'o')
                {
                  // optimized typecode support
                  idl_global->opt_tc (1);
                }
              else if (av[i][2] == 'I')

                {
                  int options = ACE_OS::strlen(av[i]) - 3;
                  int j;
                  int k=i;
                  // optimized typecode support
                  idl_global->gen_impl_files (1);
                  for (j=0;j<options;j++)
                    {

                      if (av[k][j+3] == 's')
                        {

                          idl_global->implementation_skel_ending (av[i+1]);
                          i++;
                        }
                      else if (av[k][j+3] == 'h')
                        {
                          idl_global->implementation_hdr_ending (av[i+1]);
                          i++;
                        }
                      else if (av[k][j+3] == 'b')
                        {
                          idl_global->impl_class_prefix(av[i+1]);
                          i++;
                        }
                      else if (av[k][j+3] == 'e')
                        {
                          idl_global->impl_class_suffix(av[i+1]);
                          i++;
                        }
                      else if (av[k][j+3] == 'c')
                        {
                          idl_global->gen_copy_ctor(1);
                        }
                      else if (av[k][j+3] == 'a')
                        {
                          idl_global->gen_assign_op(1);
                        }
                      else if (isalpha(av[k][j+3] ))
                        {
                          cerr << GTDEVEL ("IDL: Illegal option '") << av[i] << "'\n";
                          idl_global->set_compile_flags (idl_global->compile_flags () |
                                                     IDL_CF_ONLY_USAGE);
                        }
                    }
                }
              else
                {

                  cerr << GTDEVEL("IDL: missing argument after '")
                       << av[i]
                       << GTDEVEL("' flag\n");
                  ACE_OS::exit (99);
                }
              break;

            default:
              cerr << GTDEVEL ("IDL: Illegal option '") << av[i] << "'\n";
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
  if ((cg->lookup_strategy () == TAO_CodeGen::TAO_PERFECT_HASH) || \
      (cg->lookup_strategy () == TAO_CodeGen::TAO_BINARY_SEARCH) || \
      (cg->lookup_strategy () == TAO_CodeGen::TAO_LINEAR_SEARCH))
    {
      // Testing whether GPERF works or no.
      int return_value = DRV_check_gperf ();
      if (return_value == -1)
        {
          // If gperf_path is an absolute path, try to call this
          // again with
          ACE_DEBUG ((LM_DEBUG,
                      "TAO_IDL: warning, GPERF could not be executed\n"
                      "Perfect Hashing or Binary/Linear Search cannot be done without GPERF\n"
                      "Now, using Dynamic Hashing..\n"
                      "To use Perfect Hashing or Binary/Linear Search strategy\n"
                      "\t-Build gperf at $ACE_ROOT/apps/gperf/src\n"
                      "\t-Set the environment variable $ACE_ROOT appropriately or add $ACE_ROOT/bin to the PATH\n"
                      "\t-Refer to Operation Lookup section in the TAO IDL User Guide ($TAO_ROOT/docs/compiler.html) for more details\n"));

          // Switching over to Dynamic Hashing.
          cg->lookup_strategy (TAO_CodeGen::TAO_DYNAMIC_HASH);
        }
    }
#else /* Not ACE_HAS_GPERF */
  // If GPERF is not there, we cannot use PERFECT_HASH strategy. Let
  // us go for DYNAMIC_HASH.
  if ((cg->lookup_strategy () == TAO_CodeGen::TAO_PERFECT_HASH) ||
      (cg->lookup_strategy () == TAO_CodeGen::TAO_BINARY_SEARCH) ||
      (cg->lookup_strategy () == TAO_CodeGen::TAO_LINEAR_SEARCH))
      cg->lookup_strategy (TAO_CodeGen::TAO_DYNAMIC_HASH);
#endif /* ACE_HAS_GPERF */

  // make sure that we are not suppressing TypeCode generation and asking for
  // optimized typecode support at the same time
  if (!idl_global->tc_support () && idl_global->opt_tc ())
    {
      cerr << GTDEVEL ("Bad Combination -St and -Go \n");
      ACE_OS::exit (99);
    }
  
  // Make sure the output directory is valid.
  if (idl_global->temp_dir () == 0)
    {
      const char* tmpdir = getenv (ACE_DEFAULT_TEMP_DIR_ENV);
              
      if (tmpdir != 0)
        idl_global->temp_dir (tmpdir);
      else
        idl_global->temp_dir (ACE_DIRECTORY_SEPARATOR_STR_A
                              "tmp"
                              ACE_DIRECTORY_SEPARATOR_STR_A);
    }
}

// Return 0 on success, -1 failure. The <errno> corresponding to the
// error that caused the GPERF execution is also set.
int
DRV_check_gperf (void)
{
  // If absolute path is not specified yet, let us call just
  // "gperf". Hopefully PATH is set up correctly to locate the gperf.
  if (idl_global->gperf_path () == 0)
    // If ACE_GPERF is defined then use that gperf program instead of "gperf."
#if defined (ACE_GPERF)
    idl_global->gperf_path (ACE_GPERF);
#else
    idl_global->gperf_path ("gperf");
#endif /* ACE_GPERF */

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

  // Just call gperf in silent mode. It will come and immly exit.

  // Using ACE_Process.
  ACE_Process process_manager;
  ACE_Process_Options process_options;

  // Set the command line for the gperf program.
  process_options.command_line ("%s"
                                " "
                                "-V",
                                idl_global->gperf_path ());

  // Spawn a process for gperf.
  if (process_manager.spawn (process_options) == -1)
    return -1;

#if defined (ACE_WIN32)
  // No wait or anything in Win32.
  return 0;
#endif /* ACE_WIN32 */

  // Wait for gperf to complete.
  int wait_status = 0;
  if (process_manager.wait (&wait_status) == -1)
    return -1;
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
            // <exec> has failed.
            return -1;
          else
            // Everything was alright.
            return 0;
        }
      else
        // Not a normal exit. No <errno> might be set.
        return -1;
    }
}
