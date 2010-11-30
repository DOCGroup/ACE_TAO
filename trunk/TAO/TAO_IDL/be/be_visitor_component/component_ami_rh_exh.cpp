
//=============================================================================
/**
 *  @file    component_ami_rh_exh.cpp
 *
 *  $Id$
 *
 *  Visitor generating code for AMI4CCM reply handler impl headers.
 *
 *
 *  @author Jeff Parsons
 */
//=============================================================================

be_visitor_component_ami_rh_exh::be_visitor_component_ami_rh_exh (
      be_visitor_context *ctx)
  : be_visitor_scope (ctx),
    iface_ (0),
    callback_iface_ (0),
    scope_name_ (0),
    iface_name_ (0),
    callback_name_ (0),
    smart_scope_ (0),
    export_macro_ (be_global->exec_export_macro ())
{
}

be_visitor_component_ami_rh_exh::~be_visitor_component_ami_rh_exh (void)
{
}

int
be_visitor_component_ami_rh_exh::visit_uses (be_uses *node)
{
  this->iface_ =
    be_interface::narrow_from_decl (node->uses_type ());

  this->init ();

//  const char *suffix = "ReplyHandler";

  os_ << be_nl_2
      << "class " << this->export_macro_.c_str () << " "
      << this->callback_name_ << "_i" << be_idt_nl
      << ": public ::" << this->scope_name_ << this->smart_scope_
      << this->callback_name_ << be_uidt_nl
      << "{" << be_nl
      << "public:" << be_idt_nl
      << this->callback_name_ << "_i (void);" << be_nl
      << "virtual ~" << this->callback_name_ << "_i (void);";

  /// This overload of traverse_inheritance_graph() used here
  /// doesn't automatically prime the queues.
  this->iface_->get_insert_queue ().reset ();
  this->iface_->get_del_queue ().reset ();
  this->iface_->get_insert_queue ().enqueue_tail (this->iface_);

  Exec_ReplyHandler_Op_Attr_Decl_Generator op_attr_gen (this);

  int status =
    this->iface_->traverse_inheritance_graph (op_attr_gen,
                                              &os_,
                                              false,
                                              false);

  if (status == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_component_ami_rh_exh")
                         ACE_TEXT ("::visit_uses - ")
                         ACE_TEXT ("traverse_inheritance_graph() on ")
                         ACE_TEXT ("callback interface failed\n")),
                        -1);
    }

  os_ << be_uidt_nl
      << "};";

  return 0;
}

int
be_visitor_component_ami_rh_exh::visit_attribute (
  be_attribute *node)
{
  os_ << be_nl_2
      << "virtual void" << be_nl
      << "get_" << node->local_name ()->get_string ()
      << " (" << be_idt_nl;

  be_argument arg (AST_Argument::dir_IN,
                   node->field_type (),
                   node->name ());

  be_visitor_args_arglist arg_visitor (this->ctx_);
  arg_visitor.set_fixed_direction (AST_Argument::dir_IN);

  if (arg_visitor.visit_argument (&arg) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_component_ami_rh_exh")
                         ACE_TEXT ("::visit_attribute - ")
                         ACE_TEXT ("get_*() arg gen failed\n")),
                        -1);
    }

  os_ << ");" << be_uidt;

  arg.destroy ();

  this->gen_excep_op ("get_", node);

  if (! node->readonly ())
    {
      os_ << be_nl_2
          << "virtual void" << be_nl
          << "set_" << node->local_name ()->get_string ()
          << " (void);";

      this->gen_excep_op ("set_", node);
    }

  return 0;
}

int
be_visitor_component_ami_rh_exh::visit_operation (
  be_operation *node)
{
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
      << "virtual void" << be_nl
      << node->local_name ()->get_string () << " (";

  int count =
    node->count_arguments_with_direction (
      AST_Argument::dir_INOUT | AST_Argument::dir_OUT);

  bool vrt = node->void_return_type ();

  if (count == 0 && vrt)
    {
      os_ << "void);";

      return  0;
    }

  os_ << be_idt_nl;

  if (!vrt)
    {
      be_visitor_operation_rettype rt_visitor (this->ctx_);
      be_decl *rt =
        be_decl::narrow_from_decl (node->return_type ());

      if (rt->accept (&rt_visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("be_visitor_component_ami_rh_exh")
                             ACE_TEXT ("::visit_operation - ")
                             ACE_TEXT ("return type arg gen failed\n")),
                            -1);
        }

      os_ << " ami_return_val";

      if (count != 0)
        {
          os_ << "," << be_nl;
        }
    }

  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_component_ami_rh_exh")
                         ACE_TEXT ("::visit_operation - ")
                         ACE_TEXT ("visit_scope() failed\n")),
                        -1);
    }

  os_ << ");" << be_uidt;

  this->gen_excep_op ("", node);

  return 0;
}

int
be_visitor_component_ami_rh_exh::visit_argument (
  be_argument *node)
{
  if (node->direction () == AST_Argument::dir_IN)
    {
      return 0;
    }

  be_visitor_args_arglist arg_visitor (this->ctx_);
  arg_visitor.set_fixed_direction (AST_Argument::dir_IN);

  if (arg_visitor.visit_argument (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_component_ami_rh_exh")
                         ACE_TEXT ("::visit_argument - ")
                         ACE_TEXT ("be_visitor_args_arglist failed\n")),
                        -1);
    }

  return 0;
}

int
be_visitor_component_ami_rh_exh::post_process (be_decl *bd)
{
  // Has to be (1) an argument, (2) not an IN arg, and
  // (3) not the last INOUT or OUT arg.
  if (AST_Decl::NodeType () == AST_Decl::NT_argument)
    {
      AST_Argument *arg =
        AST_Argument::narrow_from_decl (bd);

      if (arg->direction () != AST_Argument::dir_IN
          && ! this->last_inout_or_out_node (bd))
        {
          os_ << "," << be_nl;
        }
    }

  return 0;
}

void
be_visitor_component_ami_rh_exh::init (void)
{
  UTL_Scope *s = this->iface_->defined_in ();
  AST_Decl *scope = ScopeAsDecl (s);
  this->scope_name_ = scope->full_name ();
  bool global = (scope->node_type () == AST_Decl::NT_root);
  this->smart_scope_ = (global ? "" : "::");
  this->iface_name_ = this->iface_->local_name ();

  /// The reply handler class we are generating inherits from the
  /// CORBA AMI skeleton class, not the AMI_xxxCallback class
  /// generated from the corresponding interface in this IDL file.
  /// So to get the correct *_excep operation signatures, we
  /// visit the scope of the AMI_xxxHandler interface generated
  /// by -GC, which must be applied to this IDL file.
  this->handler_str_ = this->scope_name_;
  this->handler_str_ += this->smart_scope_;
  this->handler_str_ += "AMI4CCM_";

  ACE_CString tmp (this->iface_name_);
  this->handler_str_ += tmp;
  this->handler_str_ += "ReplyHandler";

  /// Look up the AMI_xxxCallback class (see comment above)
  /// so we can traverse its inheritance graph below.
  UTL_ScopedName *sn =
    FE_Utils::string_to_scoped_name (this->handler_str_.c_str ());
  AST_Decl *d = s->lookup_by_name (sn, true, false);
  this->callback_iface_ = be_interface::narrow_from_decl (d);
  this->callback_name_ = this->callback_iface_->local_name ();

  sn->destroy ();
  delete sn;
  sn = 0;
}

void
be_visitor_component_ami_rh_exh::gen_excep_op (
  const char *prefix,
  be_decl *node)
{
  os_ << be_nl_2
      << "virtual void" << be_nl
      << prefix << node->local_name ()->get_string ()
      << "_excep (" << be_idt_nl
      << "::CCM_AMI::ExceptionHolder_ptr excep_holder);"
      << be_uidt;
}

// ======================================================

Exec_ReplyHandler_Op_Attr_Decl_Generator::Exec_ReplyHandler_Op_Attr_Decl_Generator (
    be_visitor_scope * visitor)
  : visitor_ (visitor)
{
}

int
Exec_ReplyHandler_Op_Attr_Decl_Generator::emit (
  be_interface * /* derived_interface */,
  TAO_OutStream * /* os */,
  be_interface * base_interface)
{
  return visitor_->visit_scope (base_interface);
}

