
//=============================================================================
/**
 *  @file    facet_ami_exh.cpp
 *
 *  $Id$
 *
 *  Visitor generating code for facets in the AMI connector
 *  impl header.
 *
 *
 *  @author Jeff Parsons
 */
//=============================================================================

be_visitor_facet_ami_exh::be_visitor_facet_ami_exh (
      be_visitor_context *ctx)
  : be_visitor_component_scope (ctx),
    iface_ (0),
    callback_iface_ (0),
    scope_name_ (0),
    iface_name_ (0)
{
  // This is initialized in the base class to svnt_export_macro()
  // or skel_export_macro(), since there are many more visitor
  // classes generating servant code. So we can just override
  // all that here.
  export_macro_ = be_global->conn_export_macro ();
}

be_visitor_facet_ami_exh::~be_visitor_facet_ami_exh (void)
{
}

int
be_visitor_facet_ami_exh::visit_component (be_component *node)
{
  this->node_ = node;

  /// Not sure if this bulletproof. The contents of *A.idl come
  /// from iteration over the AMI4CCM interfaces list, which is
  /// in turn populated by the corresponding pragmas. We may
  /// eventually have to do the same for this code generation,
  /// as well as that of be_visitor_connector_ami_exh.
  return this->visit_scope (node);
}

int
be_visitor_facet_ami_exh::visit_provides (be_provides *node)
{
  this->iface_ =
    be_interface::narrow_from_decl (node->provides_type ());

  if (this->gen_reply_handler_class () == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_facet_ami_exh")
                         ACE_TEXT ("::visit_provides - ")
                         ACE_TEXT ("gen_reply_handler_class() ")
                         ACE_TEXT ("failed\n")),
                        -1);
    }

  if (this->gen_facet_executor_class () == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_facet_ami_exh")
                         ACE_TEXT ("::visit_provides - ")
                         ACE_TEXT ("gen_facet_executor_class() ")
                         ACE_TEXT ("failed\n")),
                        -1);
    }

  return 0;
}

int
be_visitor_facet_ami_exh::visit_operation (be_operation *node)
{
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

  /// We're generating implementation operation declarations,
  /// so we can just use this visitor.
  be_visitor_operation_ih v (this->ctx_);

  if (v.visit_operation (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_facet_ami_exh")
                         ACE_TEXT ("::visit_operation - ")
                         ACE_TEXT ("be_visitor_operation_ih ")
                         ACE_TEXT ("failed\n")),
                        -1);
    }

  return 0;
}

void
be_visitor_facet_ami_exh::init (bool for_impl)
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

  this->handler_str_ += (for_impl ? "" : "AMI_");
  ACE_CString tmp (this->iface_name_);
  this->handler_str_ +=
    (for_impl ? tmp : tmp.substr (ACE_OS::strlen ("AMI4CCM_")));
  this->handler_str_ += (for_impl ? "Reply" : "");
  this->handler_str_ += "Handler";

  /// Look up the AMI_xxxCallback class (see comment above)
  /// so we can traverse its inheritance graph below.
  UTL_ScopedName *sn =
    FE_Utils::string_to_scoped_name (this->handler_str_.c_str ());
  AST_Decl *d = s->lookup_by_name (sn, true, false);
  this->callback_iface_ = be_interface::narrow_from_decl (d);

  sn->destroy ();
  delete sn;
  sn = 0;
}

int
be_visitor_facet_ami_exh::gen_reply_handler_class (void)
{
  const char *suffix = "_reply_handler";
  this->init (false);

  os_ << be_nl
      << "class " << this->export_macro_.c_str () << " "
      << this->iface_name_ << suffix << be_idt_nl
      << ": public ::POA_" << this->handler_str_.c_str ()
      << be_uidt_nl
      << "{" << be_nl
      << "public:" << be_idt_nl
      << this->iface_name_ << suffix << " (" << be_idt_nl
      << "::" << this->scope_name_ << this->smart_scope_
      << this->iface_name_
      << "ReplyHandler_ptr callback," << be_nl
      << "::PortableServer::POA_ptr poa);" << be_uidt_nl << be_nl
      << "virtual ~" << this->iface_name_ << suffix << " (void);";

  int const status =
    this->callback_iface_->traverse_inheritance_graph (
      be_interface::op_attr_decl_helper,
      &os_,
      false,
      false);

  if (status == -1)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("be_visitor_facet_ami_exh")
                  ACE_TEXT ("::gen_reply_handler_class - ")
                  ACE_TEXT ("traverse_inheritance_graph() on ")
                  ACE_TEXT ("callback interface failed\n")));
    }

  os_ << be_uidt_nl << be_nl
      << "private:" << be_idt_nl
      << "::" << this->scope_name_ << this->smart_scope_
      << this->iface_name_
      << "ReplyHandler_var callback_;" << be_nl
      << "::PortableServer::POA_var poa_;" << be_uidt_nl
      << "};";

  return 0;
}

int
be_visitor_facet_ami_exh::gen_facet_executor_class (void)
{
  const char *suffix = "_exec_i";
  const char *scope_name =
    ScopeAsDecl (this->iface_->defined_in ())->full_name ();
  const char *iface_name = this->iface_->local_name ();

  os_ << be_nl << be_nl
      << "class " << export_macro_.c_str () << " "
      << iface_name << suffix << be_idt_nl
      << ": public virtual ::" << scope_name << "::CCM_"
      << iface_name << "," << be_idt_nl
      << "public virtual ::CORBA::LocalObject"
      << be_uidt << be_uidt_nl
      << "{" << be_nl
      << "public:" << be_idt_nl
      << iface_name << suffix << " (void);" << be_nl << be_nl
      << "virtual ~" << iface_name << suffix
      << " (void);";

  if (this->visit_scope (this->iface_) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_connector_ami_exh")
                         ACE_TEXT ("::gen_facet_executor_class - ")
                         ACE_TEXT ("visit_scope() on sendc ")
                         ACE_TEXT ("interface failed\n")),
                        -1);
    }

  const char *container_type = be_global->ciao_container_type ();

  os_ << be_nl << be_nl
      << "virtual void set_"
      << tao_cg->downcase (container_type)
      << "_context ("
      << "::Components::"
      << be_global->ciao_container_type () << "Context_ptr ctx);";

  AST_Decl *s = ScopeAsDecl (this->node_->defined_in ());
  bool is_global =
   (s->node_type () == AST_Decl::NT_root);
  const char *smart_scope = (is_global ? "" : "::");

  os_ << be_nl << be_nl
      << "virtual ::CORBA::Object_ptr _get_component (void);";

  os_ << be_nl << be_nl
      << "virtual void _set_component (" << be_idt_nl
      << "::" << s->name () << smart_scope
      << "CCM_" << this->node_->local_name ()
      << "_ptr);" << be_uidt;

  os_ << be_uidt_nl << be_nl
      << "private:" << be_idt_nl
      << "::" << s->name () << smart_scope
      << "CCM_" << this->node_->local_name ()
      << "_Context_var context_;" << be_nl
      << "::" << s->name () << smart_scope
      << "CCM_" << this->node_->local_name ()
      << "_var component_;" << be_uidt_nl
      << "};";

  return 0;
}

