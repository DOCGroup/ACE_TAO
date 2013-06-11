
//=============================================================================
/**
 *  @file    be_util.cpp
 *
 *  $Id$
 *
 *  Static helper methods used by multiple visitors.
 *
 *
 *  @author Jeff Parsons
 */
//=============================================================================

#include "orbsvcs/Log_Macros.h"
#include "be_util.h"
#include "be_extern.h"

#include "ast_generator.h"

#include "orbsvcs/Log_Macros.h"

// Prepare an argument for a BE.
void
be_util::prep_be_arg (char *)
{
}

void
be_util::arg_post_proc (void)
{
}

void
be_util::usage (void)
{
  ORBSVCS_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -o <dir>\t\tOutput directory for the generated file.")
      ACE_TEXT (" Default is current directory\n")
    ));
  ORBSVCS_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -e\t\t\tGenerate just an include of original IDL file")
      ACE_TEXT (" if no IDL3 declarations are found\n")
    ));
}

AST_Generator *
be_util::generator_init (void)
{
  AST_Generator *gen = 0;
  ACE_NEW_RETURN (gen,
                  AST_Generator,
                  0);
  return gen;
}

