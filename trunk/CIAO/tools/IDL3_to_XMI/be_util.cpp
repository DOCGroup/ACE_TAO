
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
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT (" -f\t\t\tsimulates IFR type filtering on included types\n")
              ));
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT (" -od <dir>\t\toutput directory for the generated file ")
              ACE_TEXT ("(default is current directory)\n")
              ));
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT (" -of <dir>\t\toutput file for the generated XMI ")
              ACE_TEXT ("(default for example.idl is example.xmi)\n")
              ));
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT (" -xd <dtd_file>\t\tfull path to XMI dtd file to include in generated file ")
              ACE_TEXT ("(default is XMI.dtd)\n")
              ));
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT (" -n\t\t\texports XMI for non-local entities\n")
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

