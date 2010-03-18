
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

  const char *suffix = "_reply_hander";
  AST_Decl *scope = ScopeAsDecl (this->iface_->defined_in ());
  const char *scope_name = scope->full_name ();
  bool global = (scope->node_type () == AST_Decl::NT_root);
  const char *smart_scope = (global ? "" : "::");
  const char *iface_name = this->iface_->local_name ();
  
  os_ << be_nl
      << iface_name << suffix << "::"
      << iface_name << suffix << " ("
      << be_idt << be_idt << be_idt_nl
      << smart_scope << scope_name << "::" << iface_name
      << "Callback_ptr callback)" << be_uidt << be_uidt_nl
      << ": callback_ (" << be_idt << be_idt_nl
      << smart_scope << scope_name << "::" << iface_name
      << "Callback::_duplicate (callback))"
      << be_uidt << be_uidt << be_uidt_nl
      << "{" << be_nl
      << "}";
      
  os_ << be_nl << be_nl
      << iface_name << suffix << "::~"
      << iface_name << suffix << " (void)" << be_nl
      << "{" << be_nl
      << "}";

  ACE_CString handler_str (iface_name);
  handler_str += "Callback";
  Identifier id (handler_str.c_str ());
  
  /// The connector is defined in the template module instantiation,
  /// the callback interface is created in that module's
  /// containing scope.
  AST_Decl *m = ScopeAsDecl (this->node_->defined_in ());
  AST_Decl *d =
    m->defined_in ()->lookup_by_name_local (&id, 0);
    
  be_interface *callback_iface =
    be_interface::narrow_from_decl (d);
    
  if (this->visit_scope (callback_iface) == -1)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("be_visitor_connector_ami_exs")
                  ACE_TEXT ("::gen_reply_handler_class - ")
                  ACE_TEXT ("visit_scope() on callback ")
                  ACE_TEXT ("interface failed\n")));
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
  
  os_ << be_nl << be_nl
      << iface_name << suffix << "::"
      << iface_name << suffix << " (void)" << be_nl
      << "{" << be_nl
      << "}";
      
  os_ << be_nl << be_nl
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
  const char *smart_scope =
    (scope_str.empty () ? "" : "::"); 
    
  os_ << be_nl << be_nl
      << "void" << be_nl
      << iface_name << "_exec_i::set_session_context ("
      << be_idt_nl
      << "::Components::SessionContext_ptr ctx)" << be_uidt_nl
      << "{" << be_idt_nl
      << "this->context_ =" << be_idt_nl
      << smart_scope << scope_name << "::CCM_"
      << this->node_->local_name () << "_Context::_narrow (ctx);"
      << be_uidt_nl << be_nl
      << "if ( ::CORBA::is_nil (this->context_.in ()))"
      << be_idt_nl
      << "{" << be_idt_nl
      << "throw ::CORBA::INTERNAL ();" << be_uidt_nl
      << "}" << be_uidt_nl << be_nl
      << "this->receptacle_objref_ =" << be_idt_nl
      << "this->context_->get_connection_";
      
  for (UTL_ScopeActiveIterator i (this->node_, UTL_Scope::IK_decls);
       !i.is_done ();
       i.next ())
    {
      AST_Decl *d = i.item ();
      AST_Uses *u = AST_Uses::narrow_from_decl (d);
      
      if (u != 0)
        {
          os_ << u->local_name ();
        }
    }
      
  os_ << " ();"
      << be_uidt << be_uidt_nl
      << "}";
      
  return 0;
}

int
be_visitor_facet_ami_exs::gen_reply_hander_op (be_operation *node)
{
  os_ << be_nl << be_nl
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
    
  int c = node->argument_count ();
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

  os_ << be_nl
      << "this->_remove_ref ();" << be_uidt_nl
      << "}";

  return 0;
}

int
be_visitor_facet_ami_exs::gen_facet_executor_op (be_operation *node)
{
  os_ << be_nl << be_nl
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
    
  os_ << be_nl
      << "{" << be_idt_nl
      << "if (! ::CORBA::is_nil (this->receptacle_objref_.in ()))"
      << be_idt_nl
      << "{" << be_idt_nl
      << "::" << this->iface_->full_name ()
      << "Handler_var the_handler_var;" << be_nl << be_nl
      << "if (! ::CORBA::is_nil (ami_handler))" << be_idt_nl
      << "{" << be_idt_nl
      << this->iface_->local_name () << "_reply_handler *handler ="
      << be_idt_nl
      << "new " << this->iface_->local_name ()
      << "_reply_handler (ami_handler);" << be_uidt_nl
      << "the_handler_var = handler->_this ();" << be_uidt_nl
      << "}" << be_uidt_nl << be_nl
      << "this->receptacle_objref_->" << node->local_name ()
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
      << "}" << be_uidt << be_uidt_nl
      << "}";
     
  return 0;
}

