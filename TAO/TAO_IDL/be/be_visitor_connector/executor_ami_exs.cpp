
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

#include "connector.h"

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
  ACE_CString class_name_str (scope->local_name ()->get_string ());
  class_name_str += suffix;
  const char *class_name = class_name_str.c_str ();

  os_ << be_nl_2
      << class_name << "::"
      << class_name << " (void)" << be_idt_nl;

  /// The port is the only item in the connector's scope.
  UTL_ScopeActiveIterator j (node, UTL_Scope::IK_decls);
  AST_Extended_Port *p =
    AST_Extended_Port::narrow_from_decl (j.item ());

  bool first = true;
  int port_nr = 0;
  for (UTL_ScopeActiveIterator i (p->port_type (), UTL_Scope::IK_decls);
       !i.is_done ();
       i.next ())
    {
      AST_Decl *d = i.item ();
      AST_Provides *p = AST_Provides::narrow_from_decl (d);

      if (p != 0)
        {
          if (first)
            {
             os_  << ": facet_exec_" << port_nr << "_ (new ";
             os_ << p->provides_type ()->local_name ();
             os_ << suffix << " ())" << be_nl;
             first = false;
            }
          else
            {
             os_  << ", facet_exec_" << port_nr << "_ (new ";
             os_ << p->provides_type ()->local_name ();
             os_ << suffix << " ())" << be_uidt_nl;
            }
          port_nr++;
        }
    }

  os_ << "{" << be_nl
      << "}";

  os_ << be_nl_2
      << class_name << "::~"
      << class_name << " (void)" << be_nl
      << "{" << be_idt_nl;
  for (int i = 0; i < port_nr; i ++)
    {
      os_ << "::CORBA::release (this->facet_exec_" << i <<"_);" << be_nl;
    }
  os_ << be_uidt_nl
      << "}";

  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_executor_ami_exs::")
                         ACE_TEXT ("visit_connector - ")
                         ACE_TEXT ("visit_scope() failed\n")),
                        -1);
    }

  const char *container_type = be_global->ciao_container_type ();

  os_ << be_nl_2
      << "void" << be_nl
      << class_name << "::set_"
      << tao_cg->downcase (container_type)
      << "_context (" << be_idt_nl
      << "::Components::" << be_global->ciao_container_type ()
      << "Context_ptr ctx)"
      << be_uidt_nl
      << "{" << be_idt_nl;
   for (int i = 0; i < port_nr; i ++)
    {
      os_ << "this->facet_exec_" << i << "_->set_"
      << tao_cg->downcase (container_type)
      << "_context "
      << "(ctx);" << be_nl;
   }
   os_  << be_uidt_nl << "}";

  os_ << be_nl_2
      << "void" << be_nl
      << class_name << "::configuration_complete (void)" << be_nl
      << "{" << be_idt_nl;
   for (int i = 0; i < port_nr; i ++)
    {
      os_ << "this->facet_exec_" << i << "_->_set_component (this);" << be_nl;
    }
  os_ << be_uidt_nl << "}";

  os_ << be_nl_2
      << "void" << be_nl
      << class_name << "::ccm_activate (void)" << be_nl
      << "{" << be_nl
      << "}";

  os_ << be_nl_2
      << "void" << be_nl
      << class_name << "::ccm_passivate (void)" << be_nl
      << "{" << be_nl
      << "}";

  AST_Decl *s = ScopeAsDecl (this->node_->defined_in ());
  bool is_global =
   (s->node_type () == AST_Decl::NT_root);
  const char *smart_scope = (is_global ? "" : "::");

  os_ << be_nl_2
      << "void" << be_nl
      << class_name << "::ccm_remove (void)" << be_nl
      << "{" << be_idt_nl;
   for (int i = 0; i < port_nr; i ++)
    {
      os_ << "this->facet_exec_" << i <<"_->_set_component (" << be_idt_nl
      << "::" << s->name () << smart_scope
      << "CCM_" << this->node_->local_name ()
      << "::_nil ());" << be_uidt_nl;
   }
  os_ << be_uidt_nl << "}";

  return 0;
}

int
be_visitor_executor_ami_exs::visit_provides (be_provides *node)
{
  AST_Type *t = node->provides_type ();
   AST_Decl *scope = ScopeAsDecl (t->defined_in ());
  bool global = (scope->node_type () == AST_Decl::NT_root);
  const char *smart_scope = (global ? "" : "::");

  os_ << be_nl_2
      << smart_scope << scope->full_name () << "::CCM_"
      << t->local_name () << "_ptr" << be_nl
      << ScopeAsDecl (this->node_->defined_in ())->local_name ()
      << "_exec_i::get_";

  /// The port is the only thing in the connector's scope, and
  /// we need to insert its name into the operation name.
  UTL_ScopeActiveIterator i (this->node_, UTL_Scope::IK_decls);
  AST_Decl *d = i.item ();

  const char * loc_name = node->local_name()->get_string();
  const char *exec_ext = (ACE_OS::strstr (loc_name, "sync") ? "1" : "0");

    os_ << d->local_name () << "_"
      << node->local_name () << " (void)" << be_nl
      << "{" << be_idt_nl
      << "return "
      << smart_scope << scope->full_name () << "::CCM_" << t->local_name () << "::_duplicate ("
      << "this->facet_exec_" << exec_ext << "_);" << be_uidt_nl
      << "}";

  return 0;
}
