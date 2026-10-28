
//=============================================================================
/**
 *  @file    component_ex_idl.cpp
 *
 *  Visitor generating code for Components in the CIAO executor IDL.
 *
 *  @author Jeff Parsons
 */
//=============================================================================

#include "component.h"

#include <string>

be_visitor_component_ex_idl::be_visitor_component_ex_idl (
      be_visitor_context *ctx)
  : be_visitor_decl (ctx),
    os_ (*ctx->stream ())
{
}

be_visitor_component_ex_idl::~be_visitor_component_ex_idl () = default;

int
be_visitor_component_ex_idl::visit_component (
  be_component *node)
{
  if (node->imported ())
    {
      return 0;
    }

  be_util::gen_nesting_open (os_, node);

  be_visitor_executor_ex_idl exec_visitor (this->ctx_);

  if (exec_visitor.visit_component (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_component_ex_idl::")
                         ACE_TEXT ("visit_component - ")
                         ACE_TEXT ("executor visitor failed\n")),
                        -1);
    }

  be_visitor_context_ex_idl context_visitor (this->ctx_);

  if (context_visitor.visit_component (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_component_ex_idl::")
                         ACE_TEXT ("visit_component - ")
                         ACE_TEXT ("context visitor failed\n")),
                        -1);
    }

  be_util::gen_nesting_close (os_, node);

  AST_Decl *scope = ScopeAsDecl (node->defined_in ());

  std::string const sname_str (
    IdentifierHelper::orig_sn (scope->name ()).c_str ());
  char const * const sname = sname_str.c_str ();

  std::string_view const lname =
    node->original_local_name ()->get_string_view_or_empty ();

  std::string_view const global = sname_str.empty () ? "" : "::";

  os_ << be_nl_2
      << "module CIAO_" << node->flat_name () << "_Impl" << be_nl
      << "{" << be_idt_nl;

  os_ << "local interface " << lname << "_Exec" << be_idt_nl
      << ": " << global << sname_str << "::CCM_" << lname
      << "," << be_idt_nl;

  os_ << "::Components::" << be_global->ciao_container_type ()
      << "Component" << be_uidt << be_uidt_nl;

  os_ << "{" << be_nl
      << "};";

  os_ << be_uidt_nl
      << "};";

  return 0;
}

int
be_visitor_component_ex_idl::visit_connector (be_connector *node)
{
  return this->visit_component (node);
}

