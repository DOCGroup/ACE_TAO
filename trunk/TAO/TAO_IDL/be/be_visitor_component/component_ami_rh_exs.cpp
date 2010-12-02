
//=============================================================================
/**
 *  @file    component_ami_rh_exs.cpp
 *
 *  $Id$
 *
 *  Visitor generating code for AMI4CCM reply handler impl source
 *  files.
 *
 *
 *  @author Jeff Parsons
 */
//=============================================================================

be_visitor_component_ami_rh_exs::be_visitor_component_ami_rh_exs (
      be_visitor_context *ctx)
  : be_visitor_component_ami_rh_ex_base (ctx),
    your_code_here_ ("/* Your code here. */")
{
}

be_visitor_component_ami_rh_exs::~be_visitor_component_ami_rh_exs (void)
{
}

int
be_visitor_component_ami_rh_exs::visit_uses (be_uses *node)
{
  this->iface_ =
    be_interface::narrow_from_decl (node->uses_type ());

  this->init ();

  os_ << be_nl_2
      << this->prefix_ << this->iface_name_ << this->suffix_ << "_i" << "::"
      << this->prefix_ << this->iface_name_ << this->suffix_ << "_i" << " (void)" << be_nl
      << "{" << be_nl
      << "}";

  os_ << be_nl_2
      << this->prefix_ << this->iface_name_ << this->suffix_ << "_i" << "::~"
      << this->prefix_ << this->iface_name_ << this->suffix_ << "_i" << " (void)" << be_nl
      << "{" << be_nl
      << "}";

  ACE_CString port_pfix = this->ctx_->port_prefix ();
  this->ctx_->port_prefix () = "";

  /// This overload of traverse_inheritance_graph() used here
  /// doesn't automatically prime the queues.
  this->iface_->get_insert_queue ().reset ();
  this->iface_->get_del_queue ().reset ();
  this->iface_->get_insert_queue ().enqueue_tail (this->iface_);

  Exec_ReplyHandler_Op_Attr_Generator op_attr_gen (this);

  int status =
    this->iface_->traverse_inheritance_graph (op_attr_gen,
                                              &os_,
                                              false,
                                              false);

  if (status == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_component_ami_rh_exs")
                         ACE_TEXT ("::visit_provides - ")
                         ACE_TEXT ("traverse_inheritance_graph() on ")
                         ACE_TEXT ("callback interface failed\n")),
                        -1);
    }

  this->ctx_->port_prefix () = port_pfix;
  return 0;
}

int
be_visitor_component_ami_rh_exs::visit_operation (
  be_operation *node)
{
  /// Skip these for the reply handler.
  if (node->is_sendc_ami ())
    {
      return 0;
    }

  AST_Decl *d =
    ScopeAsDecl (node->defined_in ());

  /// We end up here also from the visit_scope() call on the
  /// connector. We want to skip the CCM-related operations
  /// that were added to the connector since it's a component.
  /// We want only the facet interface operations.
  if (d->node_type () != AST_Decl::NT_interface)
    {
      return  0;
    }

  os_ << be_nl_2
      << "void" << be_nl
      << this->class_name_ << "_i"
      << "::" << node->local_name ()->get_string ()
      << " (";

  int count =
    node->count_arguments_with_direction (
      AST_Argument::dir_INOUT | AST_Argument::dir_OUT);

  bool vrt = node->void_return_type ();

  if (count == 0 && vrt)
    {
      os_ << "void)";
    }
  else
    {
      os_ << be_idt_nl;
    }

  if (!vrt)
    {
      be_visitor_operation_rettype rt_visitor (this->ctx_);
      be_decl *rt =
        be_decl::narrow_from_decl (node->return_type ());

      if (rt->accept (&rt_visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("be_visitor_component_ami_rh_exs")
                             ACE_TEXT ("::visit_operation - ")
                             ACE_TEXT ("return type arg gen failed\n")),
                            -1);
        }

      os_ << " /* ami_return_val */";

      if (count != 0)
        {
          os_ << "," << be_nl;
        }
    }

  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_component_ami_rh_exs")
                         ACE_TEXT ("::visit_operation - ")
                         ACE_TEXT ("visit_scope() failed\n")),
                        -1);
    }

  if (count != 0 || vrt)
    {
      os_ << ")" << be_uidt;
    }

  this->gen_op_body ();

  this->gen_excep_op ("", node, true);

  return 0;
}

int
be_visitor_component_ami_rh_exs::visit_attribute (
  be_attribute *node)
{
  return this->gen_attr_op (node, true);
}

int
be_visitor_component_ami_rh_exs::visit_argument (
  be_argument *node)
{
  if (node->direction () == AST_Argument::dir_IN)
    {
      return 0;
    }

  be_visitor_args_arglist arg_visitor (this->ctx_);
  arg_visitor.set_fixed_direction (AST_Argument::dir_IN);
  arg_visitor.unused (true);

  if (arg_visitor.visit_argument (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_component_ami_rh_exs")
                         ACE_TEXT ("::visit_argument - ")
                         ACE_TEXT ("be_visitor_args_arglist failed\n")),
                        -1);
    }

  return 0;
}

void
be_visitor_component_ami_rh_exs::gen_op_body (void)
{
  os_ << be_nl
      << "{" << be_idt_nl
      << this->your_code_here_ << be_uidt_nl
      << "}";
}

