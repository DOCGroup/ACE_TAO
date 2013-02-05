
//=============================================================================
/**
 *  @file    enum_cs.cpp
 *
 *  $Id$
 *
 *  Visitor generating code for Enums in the client stubs
 *
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================

#include "enum.h"
#include "be_visitor_typecode/enum_typecode.h"

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
  if (node->cli_stub_gen ()
      || node->imported ())
    {
      return 0;
    }

  if (be_global->tc_support ())
    {
      be_visitor_context ctx (*this->ctx_);
      // ctx.sub_state (TAO_CodeGen::TAO_TC_DEFN_TYPECODE);
      TAO::be_visitor_enum_typecode visitor (&ctx);

      if (visitor.visit_enum (node) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_enum_cs::"
                             "visit_enum - "
                             "TypeCode definition failed\n"),
                            -1);
        }
    }

  node->cli_stub_gen (true);
  return 0;
}
