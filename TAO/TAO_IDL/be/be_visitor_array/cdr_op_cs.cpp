//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    cdr_op_cs.cpp
//
// = DESCRIPTION
//    Visitor for code generation of Arrays for the CDR operators in the client
//    stubs.
//
// = AUTHOR
//    Jeff Parsons <parsons@cs.wustl.edu>
//
// ============================================================================

#include        "idl.h"
#include        "idl_extern.h"
#include        "be.h"

#include "be_visitor_array.h"

// ***************************************************************************
// Array visitor for generating CDR operator declarations in the client
// stubs file
// ***************************************************************************

be_visitor_array_cdr_op_cs::be_visitor_array_cdr_op_cs (
    be_visitor_context *ctx
  )
  : be_visitor_decl (ctx)
{
}

be_visitor_array_cdr_op_cs::~be_visitor_array_cdr_op_cs (void)
{
}

int
be_visitor_array_cdr_op_cs::visit_array (be_array *node)
{
  if (node->is_local ())
    {
      return 0;
    }

  // Retrieve the base type.
  be_type *bt = be_type::narrow_from_decl (node->base_type ());

  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_array_cdr_op_cs::"
                         "visit_array - "
                         "Bad base type\n"),
                        -1);
    }

  // If we contain an anonymous sequence,
  // generate code for the sequence here.

  if (bt->node_type () == AST_Decl::NT_sequence)
    {
      if (this->gen_anonymous_base_type (bt,
                                         TAO_CodeGen::TAO_SEQUENCE_CDR_OP_CS)
          == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_array_cdr_op_cs::"
                             "visit_array - "
                             "gen_anonymous_base_type failed\n"),
                            -1);
        }
    }

  return 0;
}
