
//=============================================================================
/**
 *  @file    be_visitor_component_scope.cpp
 *
 *  $Id$
 *
 *  Abstract visitor providing ancestor scope visitation.
 *
 *
 *  @author Jeff Parsons
 */
//=============================================================================

#include "be_visitor_component_scope.h"
#include "be_visitor_context.h"

#include "be_mirror_port.h"
#include "be_component.h"
#include "be_provides.h"
#include "be_uses.h"

#include "be_helper.h"
#include "be_extern.h"

#include "utl_identifier.h"
#include "nr_extern.h"

be_visitor_component_scope::be_visitor_component_scope (
      be_visitor_context *ctx)
  : be_visitor_scope (ctx),
    node_ (0),
    os_ (*ctx->stream ()),
    export_macro_ (be_global->svnt_export_macro ())
{
  /// All existing CIAO examples set the servant export values in the CIDL
  /// compiler to equal the IDL compiler's skel export values. Below is a
  /// partial effort to decouple them, should be completely decoupled
  /// sometime. See comment in codegen.cpp, line 1173.
  if (export_macro_ == "")
    {
      export_macro_ = be_global->skel_export_macro ();
    }
}

be_visitor_component_scope::~be_visitor_component_scope (
  void)
{
}

int
be_visitor_component_scope::visit_extended_port (
  be_extended_port *node)
{
  this->ctx_->interface (this->node_);

  AST_Decl::NodeType nt =
    ScopeAsDecl (node->defined_in ())->node_type ();

  /// Skip if we are defined inside a porttype.
  /// Depends on nested ports not being allowed.
  if (nt == AST_Decl::NT_component || nt == AST_Decl::NT_connector)
    {
      this->ctx_->port_prefix () = node->local_name ()->get_string ();
      this->ctx_->port_prefix () += '_';
    }

  int status =
    this->visit_porttype_scope (node->port_type ());

  if (status == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_component_scope")
                         ACE_TEXT ("::visit_extended_port - ")
                         ACE_TEXT ("visit_porttype_scope failed\n")),
                        -1);
    }

  /// Reset port prefix string.
  this->ctx_->port_prefix () = "";
  return 0;
}

int
be_visitor_component_scope::visit_mirror_port (
  be_mirror_port *node)
{
  AST_Decl::NodeType nt =
    ScopeAsDecl (node->defined_in ())->node_type ();

  /// Skip if we are defined inside a porttype.
  /// Depends on nested ports not being allowed.
  if (nt == AST_Decl::NT_component || nt == AST_Decl::NT_connector)
    {
      this->ctx_->port_prefix () = node->local_name ()->get_string ();
      this->ctx_->port_prefix () += '_';
    }

  int status =
    this->visit_porttype_scope_mirror (node->port_type ());

  if (status == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_component_scope")
                         ACE_TEXT ("::visit_mirror_port - ")
                         ACE_TEXT ("visit_porttype_scope")
                         ACE_TEXT ("_mirror failed\n")),
                        -1);
    }

  /// Reset port prefix string.
  this->ctx_->port_prefix () = "";
  return 0;
}

int
be_visitor_component_scope::visit_component_scope (
  be_component *node)
{
  if (node == 0)
    {
      return 0;
    }

  this->ctx_->interface (node);

  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_component_scope")
                         ACE_TEXT ("::visit_component_scope - ")
                         ACE_TEXT ("visit_scope() ")
                         ACE_TEXT ("failed\n")),
                        -1);
    }

  return this->visit_component_scope (node->base_component ());
}

int
be_visitor_component_scope::visit_porttype_scope (
  be_porttype *node)
{
  return this->visit_scope (node);
}

int
be_visitor_component_scope::visit_porttype_scope_mirror (
  be_porttype *node)
{
  for (UTL_ScopeActiveIterator si (node, UTL_Scope::IK_decls);
       !si.is_done ();
       si.next ())
    {
      be_decl *d = be_decl::narrow_from_decl (si.item ());

      switch (d->node_type ())
        {
          case AST_Decl::NT_provides:
            {
              be_provides *p =
                be_provides::narrow_from_decl (d);

              be_uses mirror_node (p->name (),
                                   p->provides_type (),
                                   false);

              if (this->visit_uses (&mirror_node) == -1)
                {
                  ACE_ERROR_RETURN ((LM_ERROR,
                                     ACE_TEXT ("be_visitor_component_scope")
                                     ACE_TEXT ("::visit_porttype_mirror - ")
                                     ACE_TEXT ("visit_uses() failed\n")),
                                    -1);
                }

              mirror_node.destroy ();
              break;
            }
          case AST_Decl::NT_uses:
            {
              be_uses *u =
                be_uses::narrow_from_decl (d);

              be_provides mirror_node (u->name (),
                                       u->uses_type ());

              if (this->visit_provides (&mirror_node) == -1)
                {
                  ACE_ERROR_RETURN ((LM_ERROR,
                                     ACE_TEXT ("be_visitor_component_scope")
                                     ACE_TEXT ("::visit_porttype_mirror - ")
                                     ACE_TEXT ("visit_provides() failed\n")),
                                    -1);
                }

              mirror_node.destroy ();
              break;
            }
          default:
            if (d->accept (this) == -1)
              {
                ACE_ERROR_RETURN ((LM_ERROR,
                                   ACE_TEXT ("be_visitor_component_scope")
                                   ACE_TEXT ("::visit_porttype_mirror - ")
                                   ACE_TEXT ("%s->accept() failed\n"),
                                   d->full_name ()),
                                  -1);
              }

            break;
        }
    }

  return 0;
}

void
be_visitor_component_scope::node (be_component *c)
{
  this->node_ = c;
}

void
be_visitor_component_scope::gen_svnt_entrypoint_decl (void)
{
  os_ << be_nl << be_nl
      << "extern \"C\" " << export_macro_.c_str ()
      << " ::PortableServer::Servant" << be_nl
      << "create_" << node_->flat_name ()
      << "_Servant (" << be_idt_nl
      << "::Components::EnterpriseComponent_ptr p," << be_nl
      << "::CIAO::Container_ptr c," << be_nl
      << "const char * ins_name);" << be_uidt;
}

void
be_visitor_component_scope::gen_svnt_entrypoint_defn (void)
{
  ACE_CString sname_str (
    ScopeAsDecl (node_->defined_in ())->full_name ());
  const char *sname = sname_str.c_str ();
  const char *lname = node_->local_name ();
  const char *global = (sname_str == "" ? "" : "::");

  os_ << be_nl << be_nl
      << "extern \"C\" " << export_macro_.c_str ()
      << " ::PortableServer::Servant" << be_nl
      << "create_" << node_->flat_name ()
      << "_Servant (" << be_idt_nl
      << "::Components::EnterpriseComponent_ptr p," << be_nl
      << "::CIAO::Container_ptr c," << be_nl
      << "const char * ins_name)" << be_uidt_nl
      << "{" << be_idt_nl
      << global << sname << "::CCM_" << lname
      << "_var x =" << be_idt_nl
      << global << sname << "::CCM_" << lname
      << "::_narrow (p);" << be_uidt_nl << be_nl
      << "if ( ::CORBA::is_nil (x.in ()))" << be_idt_nl
      << "{" << be_idt_nl
      << "return 0;" << be_uidt_nl
      << "}" << be_uidt_nl << be_nl
      << "::PortableServer::Servant retval = 0;" << be_nl
      << "ACE_NEW_RETURN (retval," << be_nl
      << "                " << lname << "_Servant (" << be_idt_nl
      << "                x.in ()," << be_nl
      << "                ::Components::CCMHome::_nil ()," << be_nl
      << "                ins_name," << be_nl
      << "                0," << be_nl
      << "                c)," << be_uidt_nl
      << "                0);" << be_nl << be_nl
      << "return retval;" << be_uidt_nl
      << "}";
}

void
be_visitor_component_scope::gen_exec_entrypoint_decl (void)
{
  os_ << be_nl << be_nl
      << "extern \"C\" " << export_macro_.c_str ()
      << " ::Components::EnterpriseComponent_ptr" << be_nl
      << "create_" << node_->flat_name ()
      << "_Impl (void);";
}

void
be_visitor_component_scope::gen_exec_entrypoint_defn (void)
{
  os_ << be_nl << be_nl
      << "extern \"C\" " << export_macro_.c_str ()
      << " ::Components::EnterpriseComponent_ptr" << be_nl
      << "create_" << node_->flat_name ()
      << "_Impl (void)" << be_nl
      << "{" << be_idt_nl
      << "::Components::EnterpriseComponent_ptr retval ="
      << be_idt_nl
      << "::Components::EnterpriseComponent::_nil ();"
      << be_uidt_nl << be_nl
      << "ACE_NEW_NORETURN (" << be_idt_nl
      << "retval," << be_nl
      << node_->local_name () << "_exec_i);"
      << be_uidt_nl << be_nl
      << "return retval;" << be_uidt_nl
      << "}";
}

