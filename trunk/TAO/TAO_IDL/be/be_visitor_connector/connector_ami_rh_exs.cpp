
//=============================================================================
/**
 *  @file    connector_ami_rh_exs.cpp
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

be_visitor_connector_ami_rh_exs::be_visitor_connector_ami_rh_exs (
      be_visitor_context *ctx)
  : be_visitor_facet_ami_exh (ctx)
{
}

be_visitor_connector_ami_rh_exs::~be_visitor_connector_ami_rh_exs (void)
{
}

int
be_visitor_connector_ami_rh_exs::visit_component (be_component *node)
{
  if (node->imported ())
    {
      return 0;
    }

  this->node_ = node;

  /// CIDL-generated namespace used 'CIDL_' + composition name.
  /// Now we use 'CIAO_' + component's flat name.
  os_ << be_nl_2
      << "namespace CIAO_" << node->flat_name ()
      << "_Impl" << be_nl
      << "{" << be_idt;

  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_facet_ami_rh_exs")
                         ACE_TEXT ("::visit_component - ")
                         ACE_TEXT ("visit_component_scope() ")
                         ACE_TEXT ("failed\n")),
                        -1);
    }

  os_ << be_uidt_nl
      << "}";

  return 0;
}

int
be_visitor_connector_ami_rh_exs::visit_provides (be_provides *node)
{
  this->iface_ =
    be_interface::narrow_from_decl (node->provides_type ());

  this->init (true);

  const char *suffix = "ReplyHandler_i";

  os_ << be_nl
      << this->iface_name_ << suffix << "::"
      << this->iface_name_ << suffix << " (void)" << be_nl
      << "{" << be_nl
      << "}";

  os_ << be_nl_2
      << this->iface_name_ << suffix << "::~"
      << this->iface_name_ << suffix << " (void)" << be_nl
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

  ReplyHandler_Op_Attr_Defn_Generator op_attr_gen (this);

  int status =
    this->callback_iface_->traverse_inheritance_graph (op_attr_gen,
                                                       &os_,
                                                       false,
                                                       false);

  if (status == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_connector_ami_rh_exs")
                         ACE_TEXT ("::visit_provides - ")
                         ACE_TEXT ("traverse_inheritance_graph() on ")
                         ACE_TEXT ("callback interface failed\n")),
                        -1);
    }

  this->ctx_->port_prefix () = port_pfix;
  return 0;
}

int
be_visitor_connector_ami_rh_exs::visit_operation (
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
be_visitor_connector_ami_rh_exs::visit_attribute (
  be_attribute *node)
{
  be_visitor_attribute v (this->ctx_);
  v.op_scope (this->callback_iface_);
  v.exec_class_extension ("_i");
  return v.visit_attribute (node);
}

// ======================================================

ReplyHandler_Op_Attr_Defn_Generator::ReplyHandler_Op_Attr_Defn_Generator (
    be_visitor_scope * visitor)
  : visitor_ (visitor)
{
}

int
ReplyHandler_Op_Attr_Defn_Generator::emit (
  be_interface * /* derived_interface */,
  TAO_OutStream * /* os */,
  be_interface * base_interface)
{
  return visitor_->visit_scope (base_interface);
}

