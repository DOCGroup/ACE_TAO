// $Id$

// ============================================================================
//
//
// = LIBRARY
//    TAO_IDL3_TO_IDL2L_BE_DLL
//
// = FILENAME
//    be_global.cpp
//
// = DESCRIPTION
//    Stores global data specific to the compiler back end.
//
// = AUTHOR
//    Jeff Parsons <j.parsons@vanderbilt.edu>
//
// ============================================================================

#include "be_global.h"
#include "ast_generator.h"
#include "global_extern.h"
#include "idl_defines.h"
#include "idl3_to_idl2_visitor.h"
#include "ace/OS_NS_stdio.h"

TAO_IDL3_TO_IDL2_BE_Export BE_GlobalData *be_global = 0;

BE_GlobalData::BE_GlobalData (void)
  : filename_ (0),
    output_dir_ (0)
{
}

BE_GlobalData::~BE_GlobalData (void)
{
}

const char *
BE_GlobalData::filename (void) const
{
  return this->filename_;
}

void
BE_GlobalData::filename (char *fname)
{
  this->filename_ = fname;
}

const char*
BE_GlobalData::output_dir (void) const
{
  return this->output_dir_;
}

void
BE_GlobalData::output_dir (const char* s)
{
  delete [] this->output_dir_;
  this->output_dir_ = ACE::strnew (s);
}

void
BE_GlobalData::parse_args (long &i, char **av)
{
  switch (av[i][1])
    {
      // Directory where the generated file is to
      // be kept. Default is the current directory from which 
      // <tao_idl3_to_idl2> is called.
      case 'o':
        if (av[i][2] == '\0')
          {
            be_global->output_dir (av [i + 1]);
            i++;
          }
        else
          {
            be_global->output_dir (av[i] + 2);
          }
        break;
      default:
        ACE_ERROR ((
            LM_ERROR,
            ACE_TEXT ("IDL: I don't understand the '%s' option\n"),
            av[i]
          ));

        idl_global->set_compile_flags (idl_global->compile_flags ()
                                       | IDL_CF_ONLY_USAGE);
        break;
    }
}

// Prepare an argument for a BE
void
BE_GlobalData::prep_be_arg (char *)
{
}

void
BE_GlobalData::arg_post_proc (void)
{
}

void
BE_GlobalData::usage (void) const
{
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -o <dir>\t\tOutput directory for the generated file.")
      ACE_TEXT (" Default is current directory\n")
    ));
}

AST_Generator *
BE_GlobalData::generator_init (void)
{
  AST_Generator *gen = 0;
  ACE_NEW_RETURN (gen,
                  AST_Generator,
                  0);             
  return gen;
}


void
BE_GlobalData::destroy (void)
{
}
