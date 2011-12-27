
//=============================================================================
/**
 *  @file    component_ex_idl.cpp
 *
 *  $Id$
 *
 *  Visitor generating code for Components in the CIAO executor IDL.
 *
 *
 *  @author Jeff Parsons
 */
//=============================================================================

be_visitor_component_ex_idl::be_visitor_component_ex_idl (
      be_visitor_context *ctx)
  : be_visitor_decl (ctx),
    os_ (*ctx->stream ())
{
}

be_visitor_component_ex_idl::~be_visitor_component_ex_idl (void)
{
}

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

  ACE_CString sname_str =
    IdentifierHelper::orig_sn (scope->name ());
  const char *sname = sname_str.c_str ();

  const char *lname =
    node->original_local_name ()->get_string ();

  const char *global = (sname_str == "" ? "" : "::");

  os_ << be_nl_2
      << "module CIAO_" << node->flat_name () << "_Impl" << be_nl
      << "{" << be_idt_nl;

  os_ << "local interface " << lname << "_Exec" << be_idt_nl
      << ": " << global << sname << "::CCM_" << lname
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

