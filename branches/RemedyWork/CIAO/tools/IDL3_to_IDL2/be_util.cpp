
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

#include "be_util.h"
#include "be_extern.h"

#include "ast_generator.h"

#include "ace/Log_Msg.h"

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
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -L\t\t\tEnable locking at the IDL file level\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -r\t\t\tRemove contents of IDL file(s) from repository\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -Si\t\t\tSuppress processing of included IDL files\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -T\t\t\tAllow duplicate typedefs in IDL files\n")
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

