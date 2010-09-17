
//=============================================================================
/**
 *  @file    connector_ami_rh_exh.cpp
 *
 *  $Id$
 *
 *  Visitor generating code for AMI4CCM reply handler impl headers.
 *
 *
 *  @author Jeff Parsons
 */
//=============================================================================

be_visitor_connector_ami_rh_exh::be_visitor_connector_ami_rh_exh (
      be_visitor_context *ctx)
  : be_visitor_facet_ami_exh (ctx)
{
}

be_visitor_connector_ami_rh_exh::~be_visitor_connector_ami_rh_exh (void)
{
}

int
be_visitor_connector_ami_rh_exh::visit_component (be_component *node)
{
  if (node->imported ())
    {
      return 0;
    }

  this->node_ = node;

  /// CIDL-generated namespace used 'CIDL_' + composition name.
  /// Now we use 'CIAO_' + component's flat name.
  os_ << be_nl << be_nl
      << "namespace CIAO_" << node->flat_name ()
      << "_Impl" << be_nl
      << "{" << be_idt;

  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_facet_ami_rh_exh")
                         ACE_TEXT ("::visit_component - ")
                         ACE_TEXT ("visit_scope() ")
                         ACE_TEXT ("failed\n")),
                        -1);
    }

  os_ << be_uidt_nl
      << "}";

  return 0;
}

int
be_visitor_connector_ami_rh_exh::visit_provides (be_provides *node)
{
  this->iface_ =
    be_interface::narrow_from_decl (node->provides_type ());

  this->init (true);

  const char *suffix = "ReplyHandler";

  os_ << be_nl
      << "class " << this->export_macro_.c_str () << " "
      << this->iface_name_ << suffix << "_i" << be_idt_nl
      << ": public ::" << this->scope_name_ << this->smart_scope_
      << this->iface_name_ << suffix << be_uidt_nl
      << "{" << be_nl
      << "public:" << be_idt_nl
      << this->iface_name_ << suffix << "_i (void);" << be_nl
      << "virtual ~" << this->iface_name_ << suffix << "_i (void);";

  /// This overload of traverse_inheritance_graph() used here
  /// doesn't automatically prime the queues.
  this->callback_iface_->get_insert_queue ().reset ();
  this->callback_iface_->get_del_queue ().reset ();
  this->callback_iface_->get_insert_queue ().enqueue_tail (
    this->callback_iface_);

  ReplyHandler_Op_Attr_Decl_Generator op_attr_gen (this);

  int status =
    this->callback_iface_->traverse_inheritance_graph (op_attr_gen,
                                                       &os_,
                                                       false,
                                                       false);

  if (status == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_connector_ami_rh_exh")
                         ACE_TEXT ("::visit_provides - ")
                         ACE_TEXT ("traverse_inheritance_graph() on ")
                         ACE_TEXT ("callback interface failed\n")),
                        -1);
    }

  os_ << be_uidt_nl
      << "};";

  return 0;
}

int
be_visitor_connector_ami_rh_exh::visit_attribute (
  be_attribute *node)
{
  this->ctx_->interface (this->callback_iface_);
  TAO_CodeGen::CG_STATE state = this->ctx_->state ();
  this->ctx_->state (TAO_CodeGen::TAO_ROOT_IH);

  be_visitor_attribute v (this->ctx_);

  if (v.visit_attribute (node) == -1)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("be_visitor_connector_ami_rh_exh")
                  ACE_TEXT ("::visit_attribute - ")
                  ACE_TEXT ("be_visitor_attribute:: ")
                  ACE_TEXT ("visit_attribute failed\n")));
    }

  this->ctx_->state (state);

  return 0;
}

// ======================================================

ReplyHandler_Op_Attr_Decl_Generator::ReplyHandler_Op_Attr_Decl_Generator (
    be_visitor_scope * visitor)
  : visitor_ (visitor)
{
}

int
ReplyHandler_Op_Attr_Decl_Generator::emit (
  be_interface * /* derived_interface */,
  TAO_OutStream * /* os */,
  be_interface * base_interface)
{
  return visitor_->visit_scope (base_interface);
}

