//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    structure.cpp
//
// = DESCRIPTION
//    Visitor generating code for Structures. This is a generic visitor.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#include	"idl.h"
#include	"idl_extern.h"
#include	"be.h"

#include "be_visitor_structure.h"

ACE_RCSID(be_visitor_structure, structure, "$Id$")


// generic struct visitor
be_visitor_structure::be_visitor_structure (be_visitor_context *ctx)
  : be_visitor_scope (ctx)
{
}

be_visitor_structure::~be_visitor_structure (void)
{
}

// visit the Structure node and its scope
int
be_visitor_structure::visit_structure (be_structure *)
{
  return -1; // must be overriden
}

int
be_visitor_structure::visit_field (be_field *node)
{
  // instantiate a visitor context with a copy of our context. This info
  // will be modified based on what type of node we are visiting
  be_visitor_context ctx (*this->ctx_);
  ctx.node (node); // set the node to be the node being visited. The scope is
                   // still the same

  // this switch is acceptable rather than having derived visitors overriding
  // this method and differing only in what state they set

  switch (this->ctx_->state ())
    {
    case TAO_CodeGen::TAO_STRUCT_CH:
      ctx.state (TAO_CodeGen::TAO_FIELD_CH);
      break;
    case TAO_CodeGen::TAO_STRUCT_CI:
      ctx.state (TAO_CodeGen::TAO_FIELD_CI);
      break;
    case TAO_CodeGen::TAO_STRUCT_CS:
      ctx.state (TAO_CodeGen::TAO_FIELD_CS);
      break;
    case TAO_CodeGen::TAO_STRUCT_CDR_OP_CH:
      ctx.state (TAO_CodeGen::TAO_FIELD_CDR_OP_CH);
      break;
    case TAO_CodeGen::TAO_STRUCT_CDR_OP_CS:
      ctx.state (TAO_CodeGen::TAO_FIELD_CDR_OP_CS);
      break;
    default:
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%N:%l) be_visitor_structure::"
                           "visit_field - "
                           "Bad context state\n"
                           ), -1);
      }
    }

  be_visitor *visitor = tao_cg->make_visitor (&ctx);
  if (!visitor)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_structure::"
                         "visit_field - "
                         "NUL visitor\n"
                         ),  -1);
    }

  // let the node accept this visitor
  if (node->accept (visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_structure::"
                         "visit_field - "
                         "failed to accept visitor\n"
                         ),  -1);
    }
  delete visitor;
  return 0;
}
