/* -*- C++ -*- */
// $Id$

#include "idl_defines.h"
#include "global_extern.h"
#include "fe_extern.h"
#include "ast_generator.h"
#include "ast_root.h"
#include "ast_extern.h"
#include "utl_string.h"
#include "drv_extern_ifr.h"
#include "be_extern.h"
#include "ace/Process.h"

ACE_RCSID (IFR_Service, 
           tao_ifr, 
           "$Id$")

#if !defined (NFILES)
# define NFILES 1024
#endif /* ! NFILES */

const char *DRV_files[NFILES];
long DRV_nfiles = 0;
long DRV_file_index = -1;

const size_t LOCAL_ESCAPES_BUFFER_SIZE = 1024;

extern TAO_IFR_BE_Export void BE_produce (void);
extern TAO_IFR_BE_Export void BE_abort (void);
extern TAO_IFR_BE_Export int BE_ifr_init (int &,
                                          char *[]);

// Initialize the node generator. The IfR administration BE visitors
// will visit the AST nodes, so be_generator is not required here.
AST_Generator *
gen_init (void)
{
  AST_Generator *g = 0;
  ACE_NEW_RETURN (g,
                  AST_Generator,
                  0);

  return g;
}

void
DRV_save_orb_args (ACE_CString &arg_holder,
                   int argc,
                   char *argv[])
{
  int i = 1;

  while (i < argc)
    {
      if (ACE_OS::strncmp (argv[i], "-ORB", 4) == 0)
        {
          arg_holder += ACE_CString (argv[i]);
          arg_holder += " ";

          // Could be another -ORBxxx arg or an IDL compiler arg.
          if (*argv[i + 1] == '-')
            {
              ++i;
              continue;
            }

          // No-copy constructor.
          ACE_CString tmp (argv[i + 1],
                           0,
                           0);

          // If the arg ends with either .idl or .pidl, we're done.

          int len = ACE_static_cast (int, tmp.length ());
          int pos = tmp.find (".idl");

          if (len - pos == 4)
            {
              return;
            }

          pos = tmp.find (".pidl");

          if (len - pos == 5)
            {
              return;
            }

          // If we're here, the next arg goes with the preceding -ORBxxx.
          arg_holder += tmp;
          arg_holder += " ";
          i += 2;
        }
      else
        {
          ++i;
        }
    }
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
  // Just default to cc.
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

  // Initialize BE global data object.
  ACE_NEW (be_global,
           BE_GlobalData);

  // Initialize driver private data.
  DRV_nfiles = 0;
  DRV_file_index = 0;  
}

// DRV_drive LOGIC:

// 1. Initialize the CFE, stage 1. This builds the scope stack
// 2. Initialize the BE. This builds an instance of the generator
// 3. Initialize the CFE, stage 2. This builds the global scope
//    and populates it with the predefined types
// 4. Invoke FE_yyparse
// 5. Check for errors from FE_yyparse. If any, exit now
// 6. Check if asked to dump AST. If so, do.
// 7. Invoke BE.

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
  AST_Generator *gen = gen_init ();

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
DRV_fork (ACE_CString &arg_string)
{
  // Append the IDL command line args to the -ORBxxx args
  // in arg_string.
  arg_string += idl_global->idl_flags ();

  for (DRV_file_index = 0;
       DRV_file_index < DRV_nfiles;
       ++DRV_file_index)
    {
      ACE_Process_Options options (1, 
                                   TAO_IDL_COMMAND_LINE_BUFFER_SIZE);
      options.creation_flags (ACE_Process_Options::NO_EXEC);
      options.command_line ("%s %s %s", 
                            idl_global->prog_name (), 
                            arg_string.c_str (),
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

// main LOGIC:

// 1. Initialize compiler driver.
// 2. Initialize ORB and resolve Interface Repository.
// 3. Parse command line args.
// 4. If more than one file to parse, fork.
// 5. Otherwise, for the single file, invoke DRV_drive.

int
main (int argc, char *argv[])
{
  // Initialize driver and global variables.
  DRV_init ();

  // We don't know yet if we're going to fork, but if so,
  // we need to save the -ORBxxx args before they're
  // stripped away by BE_ifr_init (which calls ORB_init),
  // so they can be passed through DRV_fork to the child
  // process(es).
  ACE_CString orb_args;
  DRV_save_orb_args (orb_args,
                     argc,
                     argv);

  // Initialize our ORB and resolve the Interface Repository.
  // Since CORBA::ORB_init strips any -ORBxxx xxx arguments
  // it finds, we must do this first. DRV_parse_args cannot
  // deal with them.
  int init_status = BE_ifr_init (argc, 
                                 argv);

  if (init_status != 0)
    {
      ACE_OS::exit (init_status);
    }

  // Parse arguments.
  DRV_parse_args (argc, 
                  argv);

  // If a usage message is requested, give it and exit.
  if (idl_global->compile_flags () & IDL_CF_ONLY_USAGE)
    {
      DRV_usage ();
      ACE_OS::exit (0);
    }

  // Fork off a process for each file to process, only if
  // there is more than one.
  if (DRV_nfiles > 1)
    {
      // DRV_fork never returns.
      DRV_fork (orb_args);
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

  // NOT REACHED.
  return 0;
}
