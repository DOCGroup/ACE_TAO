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

#include "idl_defines.h"
#include "be_codegen.h"
#include "be_generator.h"
#include "be_extern.h"
#include "global_extern.h"
#include "fe_extern.h"
#include "ast_root.h"
#include "ast_extern.h"
#include "utl_string.h"
#include "drv_extern.h"
#include "ace/Process.h"
#include "../tao/Version.h"

ACE_RCSID (TAO_IDL,
           tao_idl,
           "$Id$")

#define IDL_CFE_VERSION "1.3.0"

#if !defined (NFILES)
# define NFILES 1024
#endif /* ! NFILES */

const char *DRV_files[NFILES];
long DRV_nfiles = 0;
long DRV_file_index = -1;

const size_t LOCAL_ESCAPES_BUFFER_SIZE = 1024;

extern TAO_IDL_BE_Export void BE_produce (void);
extern TAO_IDL_BE_Export void BE_abort (void);

// Initialize the BE. The protocol requires only that this routine
// return an instance of AST_Generator (or a subclass thereof).
AST_Generator *
BE_init (void)
{
  tao_cg = TAO_CODEGEN::instance ();

  AST_Generator *g = 0;
  ACE_NEW_RETURN (g,
                  be_generator (),
                  0);

  return g;
}

// Print out a version string for the BE.
void
BE_version (void)
{
  ACE_DEBUG ((LM_DEBUG,
              "%s %s\n",
              ACE_TEXT ("TAO C++ BE, version"),
              ACE_TEXT (TAO_VERSION)));
}

void
DRV_version (void)
{
  ACE_DEBUG ((LM_DEBUG,
              "%s\n"
              "%s %s\n",
              idl_global->prog_name (),
              ACE_TEXT ("FE: Based on Sun IDL CFE version"),
              ACE_TEXT (IDL_CFE_VERSION)));

  BE_version ();
}

void
DRV_init (void)
{
  // Initialize FE global data object.
  ACE_NEW (idl_global,
           IDL_GlobalData);

  // Initialize some of its data.
  idl_global->set_root (0);
  idl_global->set_gen (0);
  idl_global->set_err (FE_new_UTL_Error ());
  idl_global->set_err_count (0);
  idl_global->set_indent (FE_new_UTL_Indenter ());
  idl_global->set_filename (0);
  idl_global->set_main_filename (0);
  idl_global->set_real_filename (0);
  idl_global->set_stripped_filename (0);
  idl_global->set_import (I_TRUE);
  idl_global->set_in_main_file (I_FALSE);
  idl_global->set_lineno (-1);
  idl_global->set_prog_name (0);

#if defined (TAO_IDL_PREPROCESSOR)
  idl_global->set_cpp_location (TAO_IDL_PREPROCESSOR);
#elif defined (ACE_CC_PREPROCESSOR)
  idl_global->set_cpp_location (ACE_CC_PREPROCESSOR);
#else
  // Just default to cc
  idl_global->set_cpp_location ("cc");
#endif /* TAO_IDL_PREPROCESSOR */

  char local_escapes[LOCAL_ESCAPES_BUFFER_SIZE];
  ACE_OS::memset (&local_escapes,
                  0,
                  LOCAL_ESCAPES_BUFFER_SIZE);

  idl_global->set_local_escapes (local_escapes);
  idl_global->set_be ("");
  idl_global->set_compile_flags (0);
  idl_global->set_read_from_stdin (I_FALSE);
  idl_global->set_include_file_names (0);
  idl_global->set_n_include_file_names (0);
  idl_global->set_parse_state (IDL_GlobalData::PS_NoState);

  // Put an empty prefix on the stack for the global scope.
  idl_global->pragma_prefixes ().push (ACE::strnew (""));

  // Initialize BE global data object.
  ACE_NEW (be_global,
           BE_GlobalData);

  // Initialize driver private data
  DRV_nfiles = 0;
  DRV_file_index = 0;
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
  UTL_String *src_file = 0;
  ACE_NEW (src_file,
           UTL_String (s));

  idl_global->idl_src_file (src_file);

  // Pass through CPP.
  if (idl_global->compile_flags () & IDL_CF_INFORMATIVE)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "%s%s %s\n",
                  idl_global->prog_name (),
                  ACE_TEXT (": preprocessing"),
                  s));
    }

  DRV_pre_proc (s);

  // Initialize BE.
  AST_Generator *gen = BE_init ();

  if (gen == 0)
    {
      ACE_ERROR ((
          LM_ERROR,
          ACE_TEXT ("IDL: BE init failed to create generator, exiting\n")
        ));

      ACE_OS::exit (99);
    }
  else
    {
      idl_global->set_gen (gen);
    }

  // Initialize FE.
  FE_init ();

  // Parse.
  if (idl_global->compile_flags () & IDL_CF_INFORMATIVE)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "%s%s %s\n",
                  idl_global->prog_name(),
                  ACE_TEXT (": parsing"),
                  s));
    }

  FE_yyparse ();

  // If there were any errors, stop.
  if (idl_global->err_count () > 0)
    {
      ACE_ERROR ((LM_ERROR,
                  "%s%s %s%s %d %s%s\n",
                  idl_global->prog_name (),
                  ACE_TEXT (":"),
                  s,
                  ACE_TEXT (": found"),
                  idl_global->err_count (),
                  ACE_TEXT ("error"),
                  ACE_TEXT ((idl_global->err_count () > 1
                    ? ACE_TEXT ("s")
                    : ACE_TEXT ("")))));

      // Call BE_abort to allow a BE to clean up after itself.
      BE_abort ();

      ACE_OS::exit (ACE_static_cast (int, idl_global->err_count ()));
    }

  // Dump the code.
  if ((idl_global->compile_flags () & IDL_CF_INFORMATIVE)
      && (idl_global->compile_flags () & IDL_CF_DUMP_AST))
    {
      ACE_DEBUG ((LM_DEBUG,
                  "%s%s %s\n",
                  idl_global->prog_name (),
                  ACE_TEXT (": dump"),
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
                  "%s%s %s\n",
                  idl_global->prog_name (),
                  ACE_TEXT (": BE processing on"),
                  s));
    }

  // Make sure all forward declared structs and unions are defined
  // before proceeding to code generation.
  AST_check_fwd_decls ();
  long error_count = idl_global->err_count ();

  if (error_count == 0)
    {
      BE_produce ();
    }
  else
    {
      ACE_OS::exit (ACE_static_cast (int,
                                     error_count));
    }

  // Exit cleanly.
  ACE_OS::exit (0);
}

// Fork off a process, wait for it to die.
void
DRV_fork (void)
{
  for (DRV_file_index = 0;
       DRV_file_index < DRV_nfiles;
       ++DRV_file_index)
    {
      ACE_Process_Options options (1,
                                   TAO_IDL_COMMAND_LINE_BUFFER_SIZE);
      options.creation_flags (ACE_Process_Options::NO_EXEC);
      options.command_line ("%s %s %s",
                            idl_global->prog_name (),
                            idl_global->idl_flags (),
                            DRV_files[DRV_file_index]);
      ACE_Process manager;
      pid_t child_pid = manager.spawn (options);

      if (child_pid == 0)
        {
          // OK, do it to this file (in the child).
          DRV_drive (DRV_files[DRV_file_index]);
          ACE_OS::exit (0);
        }

      if (child_pid == ACE_INVALID_PID)
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("IDL: spawn failed\n")));

          ACE_OS::exit (99);
        }

      // child_pid is the process id of something at this point.
      if (manager.wait () == -1)
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("IDL: wait failed\n")));

          ACE_OS::exit (99);
        }
    }

  // Now the parent process can exit.
  ACE_OS::exit (0);
}

/*
** LOGIC:
**
** 1. Initialize compiler driver
** 2. Parse command line args
** 3. If more than one file to parse, fork
** 4. Otherwise, for the single file, invoke DRV_drive
*/

int
main (int argc, char *argv[])
{
  // Initialize driver and global variables.
  DRV_init ();

  // Parse arguments.
  DRV_parse_args (argc, argv);

  // If a version message is requested, print it and exit.
  if (idl_global->compile_flags () & IDL_CF_VERSION)
    {
      DRV_version ();
      ACE_OS::exit (0);
    }

  // If a usage message is requested, give it and exit.
  if (idl_global->compile_flags () & IDL_CF_ONLY_USAGE)
    {
      DRV_usage ();
      ACE_OS::exit (0);
    }

  // Fork off a process for each file to process. Fork only if
  // there is more than one file to process.
  if (DRV_nfiles > 1)
    {
      // DRV_fork never returns.
      DRV_fork ();
    }
  else
    {
      // Do the one file we have to parse.
      // Check if stdin and handle file name appropriately.
      if (DRV_nfiles == 0)
        {
          DRV_files[0] = "standard input";
        }

      DRV_file_index = 0;
      DRV_drive (DRV_files[DRV_file_index]);
    }

  ACE_OS::exit (0);

  // NOT REACHED
  return 0;
}
