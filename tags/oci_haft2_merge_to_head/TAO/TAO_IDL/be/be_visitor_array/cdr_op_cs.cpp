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

ACE_RCSID (be_visitor_array, 
           cdr_op_cs, 
           "$Id$")

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
  AST_Decl::NodeType nt = bt->node_type ();

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

  if (nt == AST_Decl::NT_sequence)
    {
      if (this->gen_anonymous_base_type (bt,
                                         TAO_CodeGen::TAO_ROOT_CDR_OP_CS)
          == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_array_cdr_op_cs::"
                             "visit_array - "
                             "gen_anonymous_base_type failed\n"),
                            -1);
        }
    }

  // If the array is an anonymous member and if its element type
  // is a declaration (not a reference), we must generate code for
  // the declaration.
  if (this->ctx_->alias () == 0 // Not a typedef.
      && bt->is_child (this->ctx_->scope ()))
    {
      int status = 0;
      be_visitor_context ctx (*this->ctx_);

      switch (nt)
      {
        case AST_Decl::NT_struct:
          {
            be_visitor_structure_cdr_op_cs sc_visitor (&ctx);
            status = bt->accept (&sc_visitor);
            break;
          }
        case AST_Decl::NT_union:
          {
            be_visitor_union_cdr_op_cs uc_visitor (&ctx);
            status = bt->accept (&uc_visitor);
            break;
          }
        default:
          break;
      }

      if (status == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_array_ch::"
                             "visit_array - "
                             "array base type codegen failed\n"),
                            -1);
        }
    }

  return 0;
}
