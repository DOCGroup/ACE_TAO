
//=============================================================================
/**
 *  @file    structure.cpp
 *
 *  $Id$
 *
 *  Visitor generating code for Structures. This is a generic visitor.
 *
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================

be_visitor_structure::be_visitor_structure (be_visitor_context *ctx)
  : be_visitor_scope (ctx)
{
}

be_visitor_structure::~be_visitor_structure (void)
{
}

int
be_visitor_structure::visit_structure (be_structure *)
{
  return -1; // Must be overriden.
}

int
be_visitor_structure::visit_field (be_field *node)
{
  // Instantiate a visitor context with a copy of our context. This info
  // will be modified based on what type of node we are visiting.
  be_visitor_context ctx (*this->ctx_);
  ctx.node (node);
  int status = 0;

  switch (this->ctx_->state ())
    {
    case TAO_CodeGen::TAO_ROOT_CH:
    case TAO_CodeGen::TAO_INTERFACE_CH:
    case TAO_CodeGen::TAO_UNION_PUBLIC_CH:
    case TAO_CodeGen::TAO_UNION_PRIVATE_CH:
    case TAO_CodeGen::TAO_ARRAY_CH:
      {
        be_visitor_field_ch visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_CI:
      {
        be_visitor_field_ci visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_CS:
    case TAO_CodeGen::TAO_UNION_PUBLIC_CS:
      {
        be_visitor_field_cs visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_CDR_OP_CH:
      {
        be_visitor_field_cdr_op_ch visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_CDR_OP_CS:
      {
        be_visitor_field_cdr_op_cs visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    default:
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%N:%l) be_visitor_structure::"
                           "visit_field - "
                           "Bad context state\n"),
                          -1);
      }
    }

  if (status == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_structure::"
                         "visit_field - "
                         "failed to accept visitor\n"),
                        -1);
    }

  return 0;
}
