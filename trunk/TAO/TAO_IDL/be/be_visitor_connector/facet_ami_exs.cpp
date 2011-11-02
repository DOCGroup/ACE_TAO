
//=============================================================================
/**
 *  @file    facet_ami_exs.cpp
 *
 *  $Id$
 *
 *  Visitor generating code for facets in the AMI connector
 *  impl source.
 *
 *
 *  @author Jeff Parsons
 */
//=============================================================================

be_visitor_facet_ami_exs::be_visitor_facet_ami_exs (
      be_visitor_context *ctx)
  : be_visitor_component_scope (ctx),
    for_reply_handler_ (true)
{
}

be_visitor_facet_ami_exs::~be_visitor_facet_ami_exs (void)
{
}

int
be_visitor_facet_ami_exs::visit_component (be_component *node)
{
  this->node_ = node;

  /// Not sure if this bulletproof. The contents of *A.idl come
  /// from iteration over the AMI4CCM interfaces list, which is
  /// in turn populated by the corresponding pragmas. We may
  /// eventually have to do the same for this code generation,
  /// as well as that of be_visitor_connector_ami_exs.
  return this->visit_scope (node);
}

int
be_visitor_facet_ami_exs::visit_provides (be_provides *node)
{
  this->iface_ =
    be_interface::narrow_from_decl (node->provides_type ());

  if (this->gen_reply_handler_class () == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_facet_ami_exs")
                         ACE_TEXT ("::visit_provides - ")
                         ACE_TEXT ("gen_reply_handler_class() ")
                         ACE_TEXT ("failed\n")),
                        -1);
    }

  if (this->gen_facet_executor_class () == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_facet_ami_exs")
                         ACE_TEXT ("::visit_provides - ")
                         ACE_TEXT ("gen_facet_executor_class() ")
                         ACE_TEXT ("failed\n")),
                        -1);
    }

  return 0;
}

int
be_visitor_facet_ami_exs::visit_operation (be_operation *node)
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

  if (this->for_reply_handler_)
    {
      return this->gen_reply_hander_op (node);
    }
  else
    {
      return this->gen_facet_executor_op (node);
    }
}

int
be_visitor_facet_ami_exs::visit_argument (be_argument *node)
{
  os_ << node->local_name ();

  return 0;
}

int
be_visitor_facet_ami_exs::post_process (be_decl *node)
{
  if (node->node_type () != AST_Decl::NT_argument)
    {
      return 0;
    }

  if (this->last_node (node))
    {
      os_ << ");" << be_uidt;
    }
  else
    {
      os_ << "," << be_nl;
    }

  return 0;
}

int
be_visitor_facet_ami_exs::gen_reply_handler_class (void)
{
  this->for_reply_handler_ = true;

  const char *suffix = "_reply_handler";
  UTL_Scope *s = this->iface_->defined_in ();
  AST_Decl *scope = ScopeAsDecl (s);
  const char *scope_name = scope->full_name ();
  bool global = (scope->node_type () == AST_Decl::NT_root);
  const char *smart_scope = (global ? "" : "::");
  const char *iface_name = this->iface_->local_name ();

  os_ << be_nl
      << iface_name << suffix << "::"
      << iface_name << suffix << " ("
      << be_idt << be_idt << be_idt_nl
      << smart_scope << scope_name << "::" << iface_name
      << "ReplyHandler_ptr callback," << be_nl
      << "::PortableServer::POA_ptr poa)" << be_uidt << be_uidt_nl
      << ": callback_ (" << be_idt << be_idt_nl
      << smart_scope << scope_name << "::" << iface_name
      << "ReplyHandler::_duplicate (callback))," << be_nl
      << "poa_ (::PortableServer::POA::_duplicate (poa))"

      << be_uidt << be_uidt << be_uidt_nl
      << "{" << be_nl
      << "}";

  os_ << be_nl_2
      << iface_name << suffix << "::~"
      << iface_name << suffix << " (void)" << be_nl
      << "{" << be_nl
      << "}";

  /// The reply handler class we are generating inherits from the
  /// CORBA AMI skeleton class, not the AMI_xxxCallback class
  /// generated from the corresponding interface in this IDl file.
  /// So to get the correct *_excep operation signatures, we
  /// visit the scope of the AMI_xxxHandler interface generated
  /// by -GC, which must be applied to this IDL file.
  ACE_CString handler_str (
    ScopeAsDecl (this->iface_->defined_in ())->full_name ());
  handler_str += "::AMI_";
  ACE_CString tmp (this->iface_->local_name ());
  handler_str += tmp.substr (ACE_OS::strlen ("AMI4CCM_"));
  handler_str += "Handler";

  UTL_ScopedName *sn =
    FE_Utils::string_to_scoped_name (handler_str.c_str ());
  AST_Decl *d = s->lookup_by_name (sn, true);

  sn->destroy ();
  delete sn;
  sn = 0;

  be_interface *callback_iface =
    be_interface::narrow_from_decl (d);

  /// The overload of traverse_inheritance_graph() used here
  /// doesn't automatically prime the queues.
  callback_iface->get_insert_queue ().reset ();
  callback_iface->get_del_queue ().reset ();
  callback_iface->get_insert_queue ().enqueue_tail (callback_iface);

  Facet_AMI_Exec_Op_Attr_Generator op_attr_gen (this);

  int status =
    callback_iface->traverse_inheritance_graph (
      op_attr_gen,
      &os_,
      false,
      false);

  if (status == -1)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("be_visitor_facet_ami_exs")
                  ACE_TEXT ("::gen_reply_handler_class - ")
                  ACE_TEXT ("traverse_inheritance_graph() on ")
                  ACE_TEXT ("callback interface failed\n")));
    }

  return 0;
}

int
be_visitor_facet_ami_exs::gen_facet_executor_class (void)
{
  this->for_reply_handler_ = false;

  const char *suffix = "_exec_i";
  const char *scope_name =
    ScopeAsDecl (this->iface_->defined_in ())->full_name ();
  const char *iface_name = this->iface_->local_name ();

  os_ << be_nl_2
      << iface_name << suffix << "::"
      << iface_name << suffix << " (void)" << be_nl
      << "{" << be_nl
      << "}";

  os_ << be_nl_2
      << iface_name << suffix << "::~"
      << iface_name << suffix << " (void)" << be_nl
      << "{" << be_nl
      << "}";


  if (this->visit_scope (this->iface_) == -1)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("be_visitor_connector_ami_exs")
                  ACE_TEXT ("::gen_reply_handler_class - ")
                  ACE_TEXT ("visit_scope() on sendc ")
                  ACE_TEXT ("interface failed\n")));
    }

  ACE_CString scope_str (scope_name, 0, false);

  const char *container_type = be_global->ciao_container_type ();

  os_ << be_nl_2
      << "void" << be_nl
      << iface_name << "_exec_i::set_"
      << tao_cg->downcase (container_type)
      << "_context ("
      << be_idt_nl
      << "::Components::" << be_global->ciao_container_type ()
      << "Context_ptr ctx)" << be_uidt_nl
      << "{" << be_idt_nl
      << "this->context_ =" << be_idt_nl
      << "::"
      << ScopeAsDecl (this->node_->defined_in ())->full_name ()
      << "::CCM_" << this->node_->local_name ()
      << "_Context::_narrow (ctx);" << be_uidt_nl << be_nl
      << "if ( ::CORBA::is_nil (this->context_.in ()))"
      << be_idt_nl
      << "{" << be_idt_nl
      << "throw ::CORBA::INTERNAL ();" << be_uidt_nl
      << "}" << be_uidt << be_uidt_nl
      << "}";

  AST_Decl *s = ScopeAsDecl (this->node_->defined_in ());
  bool is_global =
   (s->node_type () == AST_Decl::NT_root);
  const char *smart_scope = (is_global ? "" : "::");

  os_ << be_nl_2
      << "::CORBA::Object_ptr" << be_nl
      << iface_name << "_exec_i::_get_component (void)" << be_nl
      << "{" << be_idt_nl
      << "return" << be_idt_nl
      << "::" << s->name () << smart_scope
      << "CCM_" << this->node_->local_name ()
      << "::_duplicate (" << be_idt_nl
      << "this->component_.in ());"
      << be_uidt << be_uidt << be_uidt_nl
      << "}";

  os_ << be_nl_2
      << "void" << be_nl
      << iface_name << "_exec_i::_set_component (" << be_idt_nl
      << "::" << s->name () << smart_scope
      << "CCM_" << this->node_->local_name ()
      << "_ptr component)" << be_uidt_nl
      << "{" << be_idt_nl
      << "this->component_ =" << be_idt_nl
      << "::" << s->name () << smart_scope
      << "CCM_" << this->node_->local_name ()
      << "::_duplicate (" << be_idt_nl
      << "component);" << be_uidt << be_uidt << be_uidt_nl
      << "}";

  return 0;
}

int
be_visitor_facet_ami_exs::gen_reply_hander_op (be_operation *node)
{
  os_ << be_nl_2
      << "void" << be_nl
      << this->iface_->local_name () << "_reply_handler::"
      << node->local_name ();

  be_visitor_operation_arglist al_visitor (this->ctx_);

  if (node->accept (&al_visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_facet_ami_exs::")
                         ACE_TEXT ("gen_reply_hander_op - ")
                         ACE_TEXT ("codegen for argument ")
                         ACE_TEXT ("list failed\n")),
                        -1);
    }

  int const c = node->argument_count ();
  bool is_excep = false;

  if (c == 1)
    {
      UTL_ScopeActiveIterator i (node, UTL_Scope::IK_decls);
      AST_Decl *d = i.item ();
      AST_Argument *arg =
        AST_Argument::narrow_from_decl (d);
      AST_Type *t = arg->field_type ();
      ACE_CString type_name = t->full_name ();

      if (type_name == "Messaging::ExceptionHolder")
        {
          is_excep = true;
        }
    }

  os_ << be_nl
      << "{" << be_idt_nl;

  os_ << "if (! ::CORBA::is_nil (this->callback_.in ()))"
      << be_idt_nl << "{" << be_idt_nl;

  if (is_excep)
    {
      os_ << "::CCM_AMI::ExceptionHolder_i holder (excep_holder);"
          << be_nl
          << "this->callback_->" << node->local_name ()
          << " (&holder);";
    }
  else
    {
      os_ << "this->callback_->" << node->local_name () << " (";

      if (c == 0)
        {
          os_ << ");";
        }
      else
        {
          os_ << be_idt_nl;

          if (this->visit_scope (node) == -1)
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 ACE_TEXT ("be_visitor_facet_ami_exs")
                                 ACE_TEXT ("::visit_operation - ")
                                 ACE_TEXT ("visit_scope() failed\n")),
                                -1);
            }
        }
    }

  os_ << be_uidt_nl << "}" << be_uidt_nl;

  os_ << be_nl
      << "::PortableServer::ObjectId_var oid =" << be_idt_nl
      << "this->poa_->servant_to_id (this);" << be_uidt_nl
      << "this->poa_->deactivate_object (oid.in ());" << be_uidt_nl
      << "}";

  return 0;
}

int
be_visitor_facet_ami_exs::gen_facet_executor_op (be_operation *node)
{
  os_ << be_nl_2
      << "void" << be_nl
      << this->iface_->local_name () << "_exec_i::"
      << node->local_name ();

  be_visitor_operation_arglist al_visitor (this->ctx_);

  if (node->accept (&al_visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_facet_ami_exs::")
                         ACE_TEXT ("gen_facet_executor_op - ")
                         ACE_TEXT ("codegen for argument ")
                         ACE_TEXT ("list failed\n")),
                        -1);
    }

  AST_Decl *scope = ScopeAsDecl (this->iface_->defined_in ());
  bool global = (scope->node_type () == AST_Decl::NT_root);
  const char *smart_scope = (global ? "" : "::");

  const char *prefix = "AMI4CCM_";
  ACE_CString iface_str (this->iface_->local_name ());
  ACE_CString orig_iface_str (
    iface_str.substr (ACE_OS::strlen (prefix)));
  const char *orig_iface_name = orig_iface_str.c_str ();

  os_ << be_nl
      << "{" << be_idt_nl
      << "::" << scope->full_name () << smart_scope
      << orig_iface_name << "_var receptacle_objref =" << be_idt_nl
      << "this->context_->get_connection_ami4ccm_port_ami4ccm_uses ();"
      << be_uidt_nl << be_nl;

  os_ << "if (! ::CORBA::is_nil (receptacle_objref.in ()))"
      << be_idt_nl
      << "{" << be_idt_nl
      << "::" << scope->full_name () << smart_scope << "AMI_"
      << orig_iface_name << "Handler_var the_handler_var;"
      << be_nl_2;

  os_ << "if (! ::CORBA::is_nil (ami_handler))" << be_idt_nl
      << "{" << be_idt_nl
      << "::CORBA::Object_var objvar =" << be_idt_nl
      << "this->context_->resolve_service_reference (\"POA\");" << be_uidt_nl
      << "::PortableServer::POA_var poa =" << be_idt_nl
      << "::PortableServer::POA::_narrow (objvar.in ());" << be_uidt_nl
      << this->iface_->local_name () << "_reply_handler *handler = 0;"
      << be_nl
      << "ACE_NEW (handler," << be_nl
      << "         " << this->iface_->local_name ()
      << "_reply_handler (ami_handler, poa));" << be_nl
      << "::PortableServer::ServantBase_var owner_transfer (handler);"
      << be_nl
      << "::PortableServer::ObjectId_var oid = "
      << "poa->activate_object (handler);" << be_nl
      << "::CORBA::Object_var handler_obj = poa->id_to_reference (oid.in ());"
      << be_nl
      << "the_handler_var = ::" << scope->full_name ()
      << smart_scope << "AMI_" << orig_iface_name
      << "Handler::_narrow (handler_obj.in ());"
      << be_uidt_nl
      << "}" << be_uidt_nl << be_nl
      << "receptacle_objref->" << node->local_name ()
      << " (" << be_idt_nl
      << "the_handler_var.in ()";

  unsigned long index = 0UL;

  /// Quick scope iteration to catch all the args except the
  /// first one, which is replace above by the local _var.
  for (UTL_ScopeActiveIterator i (node, UTL_Scope::IK_decls);
       !i.is_done ();
       i.next (), ++index)
    {
      /// Skip the first arg, which is the callback reference.
      if (index == 0UL)
        {
          continue;
        }

      AST_Decl *d = i.item ();

      os_ << "," << be_nl
          << d->local_name ();
    }

  os_ << ");" << be_uidt << be_uidt_nl
      << "}" << be_uidt << be_nl;

  /// Throw an INV_OBJREF exception because there is no connection
  /// see AMI4CCM mars/11-xx-yy , 2011

  os_ << "else" << be_idt_nl
      << "{" << be_idt_nl
      << "throw ::CORBA::INV_OBJREF ();" << be_uidt_nl
      << "}" << be_uidt << be_uidt_nl;

  os_ << "}";

  return 0;
}

// ==================================================

Facet_AMI_Exec_Op_Attr_Generator::Facet_AMI_Exec_Op_Attr_Generator (
      be_visitor_scope * visitor)
  : visitor_ (visitor)
{
}

int
Facet_AMI_Exec_Op_Attr_Generator::emit (be_interface * /*derived_interface */,
                                        TAO_OutStream * /* os */,
                                        be_interface * base_interface)
{
  return visitor_->visit_scope (base_interface);
}

