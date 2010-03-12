
//=============================================================================
/**
 *  @file    executor_ami_exs.cpp
 *
 *  $Id$
 *
 *  Visitor generating code for executors in the AMI connector
 *  impl source.
 *
 *
 *  @author Jeff Parsons
 */
//=============================================================================

be_visitor_executor_ami_exs::be_visitor_executor_ami_exs (
      be_visitor_context *ctx)
  : be_visitor_component_scope (ctx)
{
}

be_visitor_executor_ami_exs::~be_visitor_executor_ami_exs (void)
{
}

int
be_visitor_executor_ami_exs::visit_connector (be_connector *node)
{
  node_ = node;

  const char *suffix = "_exec_i";
  AST_Decl *scope = ScopeAsDecl (node->defined_in ());
  const char *scope_name = scope->full_name ();
  bool global = (scope->node_type () == AST_Decl::NT_root);
  const char *smart_scope = (global ? "" : "::");
  ACE_CString class_name_str (node->local_name ());
  class_name_str += suffix;
  const char *class_name = class_name_str.c_str ();
  
  os_ << be_nl << be_nl
      << class_name << "::"
      << class_name << " (void)" << be_idt_nl
      << ": facet_exec_ (new ";

  for (UTL_ScopeActiveIterator i (node, UTL_Scope::IK_decls);
       !i.is_done ();
       i.next ())
    {
      AST_Decl *d = i.item ();
      AST_Provides *p = AST_Provides::narrow_from_decl (d);
      
      if (p != 0)
        {
          os_ << p->provides_type ()->local_name ();
        }
    }
    
  os_ << suffix << " ())" << be_uidt_nl
      << "{" << be_nl
      << "}";
      
  os_ << be_nl << be_nl
      << class_name << "::~"
      << class_name << " (void)" << be_nl
      << "{" << be_idt_nl
      << "delete facet_exec_;" << be_uidt_nl
      << "}";
      
  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_executor_ami_exs::")
                         ACE_TEXT ("visit_connector - ")
                         ACE_TEXT ("visit_scope() failed\n")),
                        -1);
    }
    
  os_ << be_nl << be_nl
      << "void" << be_nl
      << class_name << "::set_session_context (" << be_idt_nl
      << "::Components::SessionContext_ptr ctx)"
      << be_uidt << be_uidt_nl
      << "{" << be_idt_nl
      << "this->context_ =" << be_idt_nl
      << smart_scope << scope_name
      << "::CCM_" << node->local_name ()
      << "_Context::_narrow (ctx);" << be_nl << be_nl
      << "if ( ::CORBA::is_nil (this->context_.in ()))"
      << be_idt_nl
      << "{" << be_idt_nl
      << "throw ::CORBA::INTERNAL ();" << be_uidt_nl
      << "}" << be_uidt_nl << be_nl
      << "this->facet_exec_->set_session_context "
      << "(this->context_.in ());" << be_uidt_nl
      << "}";
      
  os_ << be_nl << be_nl
      << "void" << be_nl
      << class_name << "::configuration_complete (void)" << be_nl
      << "{" << be_nl
      << "}";
      
  os_ << be_nl << be_nl
      << "void" << be_nl
      << class_name << "::ccm_activate (void)" << be_nl
      << "{" << be_nl
      << "}";
      
  os_ << be_nl << be_nl
      << "void" << be_nl
      << class_name << "::ccm_passivate (void)" << be_nl
      << "{" << be_nl
      << "}";
      
  os_ << be_nl << be_nl
      << "void" << be_nl
      << class_name << "::ccm_remove (void)" << be_nl
      << "{" << be_nl 
      << "}";
  
  return 0;
}

int
be_visitor_executor_ami_exs::visit_provides (be_provides *node)
{
  AST_Type *t = node->provides_type ();
  AST_Decl *scope = ScopeAsDecl (t->defined_in ());
  bool global = (scope->node_type () == AST_Decl::NT_root);
  const char *smart_scope = (global ? "" : "::");
  
  os_ << be_nl << be_nl
      << smart_scope << scope->full_name () << "::CCM_"
      << t->local_name () << "_ptr" << be_nl
      << this->node_->local_name () << "_exec_i::get_"
      << node->local_name () << " (void)" << be_nl
      << "{" << be_idt_nl
      << "return this->facet_exec_;" << be_uidt_nl
      << "}";
  
  return 0;
}

