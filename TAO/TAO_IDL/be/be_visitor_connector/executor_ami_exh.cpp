
//=============================================================================
/**
 *  @file    executor_ami_exh.cpp
 *
 *  $Id$
 *
 *  Visitor generating code for executors in the AMI connector
 *  impl header.
 *
 *
 *  @author Jeff Parsons
 */
//=============================================================================


be_visitor_executor_ami_exh::be_visitor_executor_ami_exh (
      be_visitor_context *ctx)
  : be_visitor_component_scope (ctx)
{
  // This is initialized in the base class to svnt_export_macro()
  // or skel_export_macro(), since there are many more visitor
  // classes generating servant code. So we can just override
  // all that here.
  export_macro_ = be_global->conn_export_macro ();
}

be_visitor_executor_ami_exh::~be_visitor_executor_ami_exh (void)
{
}

int
be_visitor_executor_ami_exh::visit_connector (be_connector *node)
{
  node_ = node;

  const char *suffix = "_exec_i";
  AST_Decl *scope = ScopeAsDecl (node->defined_in ());
  bool global = (scope->node_type () == AST_Decl::NT_root);
  const char *smart_scope = (global ? "" : "::");

  os_ << be_nl << be_nl
      << "class " << this->export_macro_.c_str ()
      << node->local_name () << suffix << be_idt_nl
      << ": public virtual " << smart_scope
      << scope->full_name () << "::CCM_"
      << node->local_name () << "," << be_idt_nl
      << "public virtual ::CORBA::LocalObject"
      << be_uidt << be_uidt_nl
      << "{" << be_nl
      << "public:" << be_idt_nl
      << node->local_name () << suffix << " (void);" << be_nl
      << "virtual ~" << node->local_name () << suffix
      << " (void);";

  // TODO - visit scope and generate get_provides_*

  os_ << be_nl << be_nl
      << "virtual void" << be_nl
      << "set_session_context (" << be_idt_nl
      << "::Components::SessionContext_ptr ctx);"
      << be_uidt_nl << be_nl
      << "virtual void configuration_complete (void);"
      << be_nl << be_nl
      << "virtual void ccm_activate (void);" << be_nl
      << "virtual void ccm_passivate (void);" << be_nl
      << "virtual void ccm_remove (void);";

  os_ << be_uidt_nl << be_nl
      << "private:" << be_idt_nl
      << smart_scope << scope->full_name () << "::CCM_"
      << node->local_name () << "_Context_var context_;" << be_nl;

      // TODO - rest of members.

  os_ << be_uidt_nl
      << "};";

  return 0;
}

int
be_visitor_executor_ami_exh::visit_provides (be_provides *)
{
  return 0;
}

