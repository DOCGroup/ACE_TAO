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

#include "be_extern.h"
#include "be_util.h"

#include "idl_defines.h"
#include "global_extern.h"

#include "fe_extern.h"

#include "ast_root.h"
#include "ast_extern.h"

#include "utl_string.h"
#include "utl_identifier.h"

#include "drv_extern.h"

#include "tao/Version.h"
#include "ace/Argv_Type_Converter.h"
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_unistd.h"

#if !defined (ACE_LACKS_IOSTREAM_TOTALLY)
// FUZZ: disable check_for_streams_include
#  include "ace/streams.h"
#endif /* ! ACE_LACKS_IOSTREAM_TOTALLY */

extern const ACE_TCHAR *DRV_arglist[];
extern unsigned long DRV_argcount;

char *DRV_files[NFILES];
long DRV_nfiles = 0;
long DRV_file_index = -1;

void
DRV_version (void)
{
  ACE_DEBUG ((LM_DEBUG,
              "%s\n"
              "TAO_IDL_FE, version %s (Based on Sun IDL FE, version %s)\n",
              ACE_TEXT_CHAR_TO_TCHAR (idl_global->prog_name ()),
              ACE_TEXT (TAO_VERSION),
              ACE_TEXT (SUN_IDL_FE_VERSION)));

  BE_version ();
}

int
DRV_init (int &argc, ACE_TCHAR *argv[])
{
  // Initialize front end.
  FE_init ();

  // Initialize driver private data. DRV_nfiles will be updated
  // by DRV_parse_args().
  DRV_nfiles = 0;
  DRV_file_index = 0;

#if defined (TAO_IDL_PREPROCESSOR)
  idl_global->set_cpp_location (TAO_IDL_PREPROCESSOR);
#elif defined (ACE_CC_PREPROCESSOR)
  idl_global->set_cpp_location (ACE_CC_PREPROCESSOR);
#else
  // Just default to cc
  idl_global->set_cpp_location ("cc");
#endif /* TAO_IDL_PREPROCESSOR */

  // Does nothing for IDL compiler, stores -ORB args, initializes
  // ORB and IFR for IFR loader.
  return BE_init (argc, argv);
}

void
DRV_refresh (void)
{
  idl_global->set_err_count (0);
  idl_global->set_filename (0);
  idl_global->set_main_filename (0);
  idl_global->set_real_filename (0);
  idl_global->set_stripped_filename (0);
  idl_global->set_lineno (-1);
  idl_global->reset_flag_seen ();
}

void
DRV_cleanup (void)
{
  // In case we got here via an init error or
  // usage/version option - otherwise it's idempotent.
  BE_cleanup ();

  be_global->destroy ();
  delete be_global;
  be_global = 0;

  idl_global->fini ();
  delete idl_global;
  idl_global = 0;

  for (DRV_file_index = 0;
       DRV_file_index < DRV_nfiles;
       ++DRV_file_index)
    {
      ACE::strdelete (DRV_files[DRV_file_index]);
    }

  for (unsigned long i = 0; i < DRV_argcount; ++i)
    {
      ACE::strdelete (const_cast<ACE_TCHAR *> (DRV_arglist[i]));
    }
}

/*
** Drive the compilation
**
** LOGIC:
**
** 2. Initialize the BE. This builds an instance of the generator
** 3. Initialize the FE. This builds the global scope
**    and populates it with the predefined types
** 4. Invoke FE_yyparse
** 5. Check for errors from FE_yyparse. If any, exit now
** 6. Check for undefined forward declared interfaces. If any, exit now
** 7. Check if asked to dump AST. If so, do.
** 8. Invoke BE.
*/

void
DRV_drive (const char *s)
{
  // Set the name of the IDL file we are parsing. This is useful to
  // the backend when it generates C++ headers and files.
  UTL_String *utl_string = 0;
  ACE_NEW (utl_string,
           UTL_String (s, true));

  idl_global->idl_src_file (utl_string);

  // Pass through CPP.
  if (idl_global->compile_flags () & IDL_CF_INFORMATIVE)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT("%C: preprocessing %C\n"),
                  idl_global->prog_name (),
                  s));
    }

  DRV_pre_proc (s);

  if (idl_global->compile_flags () & IDL_CF_ONLY_PREPROC)
    {
      // Go straight to cleanup, process the next file, if any.
      DRV_refresh ();
      return;
    }

  // Parse.
  if (idl_global->compile_flags () & IDL_CF_INFORMATIVE)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT("%C: parsing %C\n"),
                  idl_global->prog_name (),
                  s));
    }

  // Return value not used - error count stored in idl_global
  // and checked below.
  (void) FE_yyparse ();

  // This option creates a single IDL file that includes all
  // input files. The backend outputs their names individually.
  if (!idl_global->multi_file_input ())
    {
      // Filename set by FE_yyparse(), so we output it immediately after.
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT("processing %C\n"),
                  idl_global->filename ()->get_string ()));
    }

  // We must do this as late as possible to make sure any
  // forward declared structs or unions contained in a
  // primary key at some level have been fully defined.
  idl_global->check_primary_keys ();

  // If there were any errors, stop.
  if (idl_global->err_count () > 0)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT("%C: %C: found %d error%s\n"),
                  idl_global->prog_name (),
                  s,
                  idl_global->err_count (),
                  (idl_global->err_count () > 1
                    ? ACE_TEXT ("s")
                    : ACE_TEXT ("")) ));

      // Backend will be cleaned up after the exception is caught.
      throw Bailout ();
    }

  // Dump the code.
  if ((idl_global->compile_flags () & IDL_CF_INFORMATIVE)
      && (idl_global->compile_flags () & IDL_CF_DUMP_AST))
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT("%C: dump %C\n"),
                  idl_global->prog_name (),
                  s));
    }

  if (idl_global->compile_flags () & IDL_CF_DUMP_AST)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("Dump of AST:\n")));

      idl_global->root ()->dump (*ACE_DEFAULT_LOG_STREAM);
    }

  // Call the main entry point for the BE.
  if (idl_global->compile_flags () & IDL_CF_INFORMATIVE)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT("%C: BE processing on %C\n"),
                  idl_global->prog_name (),
                  s));
    }

  // Make sure all forward declared structs and unions are defined
  // before proceeding to code generation.
  AST_check_fwd_decls ();

  if (0 == idl_global->err_count ())
    {
      BE_produce ();
    }
  else
    {
      throw Bailout ();
    }

  DRV_refresh ();
}

/*
** LOGIC:
**
** 1. Initialize compiler driver
** 2. Parse command line args
** 3. Invoke DRV_drive iteratively on each file passed in
*/

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  ACE_Argv_Type_Converter atc (argc, argv);
  try
    {
      if (0 != DRV_init (atc.get_argc (), atc.get_TCHAR_argv ()))
        {
          throw Bailout ();
        }

      // Parse arguments.
      DRV_parse_args (atc.get_argc (), atc.get_ASCII_argv ());

      // If a version message is requested, print it and exit cleanly.
      if (idl_global->compile_flags () & IDL_CF_VERSION)
        {
          DRV_version ();
          DRV_cleanup ();
          return 0;
        }

      // If a usage message is requested, print it and exit cleanly.
      if (idl_global->compile_flags () & IDL_CF_ONLY_USAGE)
        {
          DRV_usage ();
          DRV_cleanup ();
          return 0;
        }

      // If there are no input files, and we are not using the
      // directory recursion option, there's no sense going any further.
      if (0 == DRV_nfiles && 0 == idl_global->recursion_start ())
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("IDL: No input files\n")));

          throw Bailout ();
        }

      AST_Generator *gen = be_util::generator_init ();

      if (0 == gen)
        {
          ACE_ERROR ((
              LM_ERROR,
              ACE_TEXT ("IDL: DRV_generator_init() failed to create ")
              ACE_TEXT ("generator, exiting\n")
            ));

          throw Bailout ();
        }
      else
        {
          idl_global->set_gen (gen);
        }

      // Initialize AST and load predefined types.
      FE_populate ();

      // Does various things in various backends.
      BE_post_init (DRV_files, DRV_nfiles);

      FILE *output_file = 0;

      if (idl_global->multi_file_input ())
        {
          output_file =
            ACE_OS::fopen (idl_global->big_file_name (), "w");
        }

      for (DRV_file_index = 0;
           DRV_file_index < DRV_nfiles;
           ++DRV_file_index)
        {
          if (idl_global->multi_file_input ())
            {
              ACE_OS::fprintf (output_file,
                               "#include \"%s\"\n",
                               DRV_files[DRV_file_index]);
            }
          else
            {
              DRV_drive (DRV_files[DRV_file_index]);
            }
        }

      if (idl_global->multi_file_input ())
        {
          ACE_OS::fclose (output_file);
          DRV_drive (idl_global->big_file_name ());
          ACE_OS::unlink (idl_global->big_file_name ());
        }
    }
  catch (Bailout)
    {
      // Incrementing here may be redundant, but the error count
      // is the exit value, and we want to make sure it isn't 0
      // if there was in fact an error. If a non-zero value is
      // off by 1, it's not so important.
      idl_global->set_err_count (idl_global->err_count () + 1);
    }

  int retval = idl_global->err_count ();
  DRV_cleanup ();

  return retval;
}
