
//=============================================================================
/**
 *  @file    operation_cs.cpp
 *
 *  $Id$
 *
 *  Visitor generating code for Operation in the stubs file.
 *
 *
 *  @author Aniruddha Gokhale & Angelo Corsaro
 */
//=============================================================================

// ************************************************************
// Operation visitor for client stubs
// ************************************************************

be_visitor_operation_cs::be_visitor_operation_cs (be_visitor_context *ctx)
  : be_visitor_operation (ctx)
{
}

be_visitor_operation_cs::~be_visitor_operation_cs (void)
{
}

// Processing to be done after every element in the scope is processed.
int
be_visitor_operation_cs::post_process (be_decl *bd)
{
  // All we do here is to insert a comma and a newline.
  TAO_OutStream *os = this->ctx_->stream ();

  if (!this->last_node (bd))
    {
      *os << "," << be_nl;
    }

  return 0;
}

int
be_visitor_operation_cs::visit_operation (be_operation *node)
{
  UTL_Scope *s =
    this->ctx_->attribute ()
      ? this->ctx_->attribute ()->defined_in ()
      : node->defined_in ();

  be_interface *intf = be_interface::narrow_from_scope (s);

  if (intf == 0)
    {
      be_porttype *pt = be_porttype::narrow_from_scope (s);

      if (pt == 0)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("be_visitor_operation_cs::")
                             ACE_TEXT ("visit_operation - ")
                             ACE_TEXT ("bad scope\n")),
                            -1);
        }
      else
        {
          intf = this->ctx_->interface ();

          if (intf == 0)
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 ACE_TEXT ("be_visitor_operation_cs::")
                                 ACE_TEXT ("visit_operation - ")
                                 ACE_TEXT ("bad scope\n")),
                                -1);
            }
        }
    }

  TAO_OutStream *os = this->ctx_->stream ();
  this->ctx_->node (node); // save the node for future use

  if (node->is_local ())
    {
      return 0;
    }

  *os << be_nl_2 << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__ << be_nl_2;

  // Retrieve the operation return type.
  be_type *bt = be_type::narrow_from_decl (node->return_type ());

  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_operation_cs::")
                         ACE_TEXT ("visit_operation - ")
                         ACE_TEXT ("bad return type\n")),
                        -1);
    }

  // Generate the return type mapping (same as in the header file)
  be_visitor_context ctx = *this->ctx_;
  be_visitor_operation_rettype rt_visitor = (&ctx);

  if (bt->accept (&rt_visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_operation_cs::")
                         ACE_TEXT ("visit_operation - ")
                         ACE_TEXT ("codegen for return type failed\n")),
                        -1);
    }

  // Generate the operation name
  *os << be_nl
      << intf->name () << "::" << node->local_name ();

  // Generate the argument list with the appropriate mapping (same as
  // in the header file)
  ctx = *this->ctx_;
  be_visitor_operation_arglist al_visitor (&ctx);

  if (node->accept (&al_visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_operation_cs::")
                         ACE_TEXT ("visit_operation - ")
                         ACE_TEXT ("codegen for argument list failed\n")),
                        -1);
    }

  if (this->gen_stub_operation_body (node, bt) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_operation_cs::")
                         ACE_TEXT ("visit_operation - ")
                         ACE_TEXT ("codegen for stub body failed\n")),
                        -1);
    }

  /// If we are in a reply handler, are not an execp_* operation,
  /// and have no native args, then generate the AMI static
  /// reply stub declaration.
  if (intf != 0
      && intf->is_ami_rh ()
      && !node->is_excep_ami ()
      && !node->has_native ())
    {
      be_visitor_operation_ami_handler_reply_stub_operation_cs v (
        this->ctx_);

      int status = v.visit_operation (node);

      if (status == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("be_visitor_operation_cs::")
                             ACE_TEXT ("visit_operation - ")
                             ACE_TEXT ("codegen for AMI reply stub failed\n")),
                            -1);
        }
    }

  return 0;
}

int
be_visitor_operation_cs::visit_argument (be_argument *node)
{
  // This method is used to generate the ParamData table entry.

  TAO_OutStream *os = this->ctx_->stream ();
  be_type *bt = be_type::narrow_from_decl (node->field_type ());

  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_operation_cs::")
                         ACE_TEXT ("visit_argument - ")
                         ACE_TEXT ("bad argument type\n")),
                        -1);
    }

  os->indent ();
  *os << "{" << bt->tc_name () << ", ";

  switch (node->direction ())
    {
    case AST_Argument::dir_IN:
      *os << "PARAM_IN, ";
      break;
    case AST_Argument::dir_INOUT:
      *os << "PARAM_INOUT, ";
      break;
    case AST_Argument::dir_OUT:
      *os << "PARAM_OUT, ";
      break;
    }

  *os << "0}";

  return 0;
}
