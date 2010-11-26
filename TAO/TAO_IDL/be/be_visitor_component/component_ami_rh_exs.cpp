
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
  : be_visitor_scope (ctx),
    iface_ (0),
    callback_iface_ (0),
    scope_name_ (0),
    iface_name_ (0),
    callback_name_ (0),
    smart_scope_ (0)
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
      << this->callback_name_ << "_i" << "::"
      << this->callback_name_ << "_i" << " (void)" << be_nl
      << "{" << be_nl
      << "}";

  os_ << be_nl_2
      << this->callback_name_ << "_i" << "::~"
      << this->callback_name_ << "_i" << " (void)" << be_nl
      << "{" << be_nl
      << "}";

  ACE_CString port_pfix = this->ctx_->port_prefix ();
  this->ctx_->port_prefix () = "";

  /// This overload of traverse_inheritance_graph() used here
  /// doesn't automatically prime the queues.
  this->callback_iface_->get_insert_queue ().reset ();
  this->callback_iface_->get_del_queue ().reset ();
  this->callback_iface_->get_insert_queue ().enqueue_tail (
    this->callback_iface_);

  Exec_ReplyHandler_Op_Attr_Defn_Generator op_attr_gen (this);

  int status =
    this->callback_iface_->traverse_inheritance_graph (op_attr_gen,
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
  AST_Decl::NodeType nt = node->defined_in ()->scope_node_type ();

  if (nt != AST_Decl::NT_interface)
    {
      return 0;
    }

  be_visitor_operation_exs v (this->ctx_);
  v.scope (this->callback_iface_);
  v.class_extension ("_i");
  return v.visit_operation (node);
}

int
be_visitor_component_ami_rh_exs::visit_attribute (
  be_attribute *node)
{
  be_visitor_attribute v (this->ctx_);
  v.op_scope (this->callback_iface_);
  v.exec_class_extension ("_i");
  return v.visit_attribute (node);
}

void
be_visitor_component_ami_rh_exs::init (void)
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

// ======================================================

Exec_ReplyHandler_Op_Attr_Defn_Generator::Exec_ReplyHandler_Op_Attr_Defn_Generator (
    be_visitor_scope * visitor)
  : visitor_ (visitor)
{
}

int
Exec_ReplyHandler_Op_Attr_Defn_Generator::emit (
  be_interface * /* derived_interface */,
  TAO_OutStream * /* os */,
  be_interface * base_interface)
{
  return visitor_->visit_scope (base_interface);
}

