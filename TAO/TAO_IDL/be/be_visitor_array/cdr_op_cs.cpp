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

be_visitor_array_cdr_op_cs::be_visitor_array_cdr_op_cs
(be_visitor_context *ctx)
  : be_visitor_decl (ctx)
{
}

be_visitor_array_cdr_op_cs::~be_visitor_array_cdr_op_cs (void)
{
}

int
be_visitor_array_cdr_op_cs::visit_array (be_array *node)
{
  // retrieve the base type
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

// Just do nothing for all the base types

int
be_visitor_array_cdr_op_cs::visit_enum (be_enum *)
{
  return 0;
}

int
be_visitor_array_cdr_op_cs::visit_interface (be_interface *)
{
  return 0;
}

int
be_visitor_array_cdr_op_cs::visit_interface_fwd (be_interface_fwd *)
{
  return 0;
}

int
be_visitor_array_cdr_op_cs::visit_predefined_type (be_predefined_type *)
{
  return 0;
}

int
be_visitor_array_cdr_op_cs::visit_sequence (be_sequence *)
{
  return 0;
}

int
be_visitor_array_cdr_op_cs::visit_string (be_string *)
{
  return 0;
}

int
be_visitor_array_cdr_op_cs::visit_structure (be_structure *)
{
  return 0;
}

int
be_visitor_array_cdr_op_cs::visit_union (be_union *)
{
  return 0;
}

int
be_visitor_array_cdr_op_cs::visit_typedef (be_typedef *)
{
  return 0;
}

// helper
int
be_visitor_array_cdr_op_cs::visit_node (be_type *bt)
{
  return 0;
}
