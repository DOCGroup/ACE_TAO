#include "be_util.h"
#include "be_extern.h"

#include "ast_generator.h"

#include "ace/OS_NS_strings.h"

#include <memory>
#include <new>

void
be_util::prep_be_arg (std::string const& /*arg*/)
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

std::unique_ptr<AST_Generator>
be_util::generator_init ()
{
  return std::unique_ptr<AST_Generator> (new (std::nothrow) AST_Generator);
}
