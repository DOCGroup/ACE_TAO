#include "be_util.h"
#include "be_extern.h"

#include "ast_generator.h"

#include "ace/OS_NS_strings.h"

void
be_util::prep_be_arg (char * /*arg*/)
{
}

void
be_util::arg_post_proc ()
{
}

void
be_util::usage ()
{
}

AST_Generator *
be_util::generator_init ()
{
  AST_Generator *gen = 0;
  ACE_NEW_RETURN(gen, AST_Generator, 0);
  return gen;
}
