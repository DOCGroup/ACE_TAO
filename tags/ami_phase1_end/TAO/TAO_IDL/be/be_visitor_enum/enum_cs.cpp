//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    enum_cs.cpp
//
// = DESCRIPTION
//    Visitor generating code for Enums in the client stubs
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#include	"idl.h"
#include	"idl_extern.h"
#include	"be.h"

#include "be_visitor_enum.h"

ACE_RCSID(be_visitor_enum, enum_cs, "$Id$")


// ********************************************************************
// Visitor implementation for the Enum type
// This one for the client stubs file
// ********************************************************************

be_visitor_enum_cs::be_visitor_enum_cs (be_visitor_context *ctx)
  : be_visitor_scope (ctx)
{
}

be_visitor_enum_cs::~be_visitor_enum_cs (void)
{
}

// visit the Enum_cs node and its scope
int
be_visitor_enum_cs::visit_enum (be_enum *node)
{
  if (!node->cli_stub_gen () && !node->imported ())
    {
      // by using a visitor to declare and define the TypeCode, we have the
      // added advantage to conditionally not generate any code. This will be
      // based on the command line options. This is still TO-DO
      be_visitor *visitor;
      be_visitor_context ctx (*this->ctx_);
      ctx.state (TAO_CodeGen::TAO_TYPECODE_DEFN);
      visitor = tao_cg->make_visitor (&ctx);
      if (!visitor || (node->accept (visitor) == -1))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_enum_cs::"
                             "visit_enum - "
                             "TypeCode definition failed\n"
                             ), -1);
        }

      node->cli_stub_gen (I_TRUE);
    }
  return 0;
}
