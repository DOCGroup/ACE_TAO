//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    context_ex_idl.cpp
//
// = DESCRIPTION
//    Visitor generating code for the contect class
//    in the CIAO executor IDL.
//
// = AUTHOR
//    Jeff Parsons
//
// ============================================================================

be_visitor_context_ex_idl::be_visitor_context_ex_idl (
      be_visitor_context *ctx)
  : be_visitor_component_scope (ctx)
{
}

be_visitor_context_ex_idl::~be_visitor_context_ex_idl (
  void)
{
}

int
be_visitor_context_ex_idl::visit_component (
  be_component *node)
{
  node_ = node;

  os_ << be_nl << be_nl
      << "local interface CCM_"
      << node->original_local_name ()->get_string ()
      << "_Context" << be_idt_nl
      << ": ";

  AST_Component *base = node->base_component ();

  if (base == 0)
    {
      os_ << "::Components::SessionContext";
    }
  else
    {
      AST_Decl *scope =
        ScopeAsDecl (base->defined_in ());

      ACE_CString sname_str =
        IdentifierHelper::orig_sn (scope->name ());
      const char *sname = sname_str.c_str ();

      const char *lname =
        base->original_local_name ()->get_string ();

      const char *global = (sname_str == "" ? "" : "::");

      os_ << global << sname << "::CCM_"
          << lname << "_Context";
    }

  os_ << be_uidt_nl
      << "{" << be_idt;

  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_context_ex_idl::")
                         ACE_TEXT ("visit_component - ")
                         ACE_TEXT ("visit_scope() failed\n")),
                        -1);
    }

  os_ << be_uidt_nl
      << "};";
      
  return 0;
}

int
be_visitor_context_ex_idl::visit_uses (
  be_uses *node)
{
  be_type *impl =
    be_type::narrow_from_decl (node->uses_type ());

  os_ << be_nl
      << "::";
      
  const char *lname = node->local_name ()->get_string ();

  // Note that we don't strip off the possible '_cxx_' when
  // adding the 'Connections' suffix. The front end will
  // create this implied IDL node with the '_cxx_' so lookup
  // will fail (when processing the *E.idl file) if we
  // strip it off here.
  if (node->is_multiple ())
    {
      os_ << IdentifierHelper::orig_sn (node_->name ()).c_str ()
          << "::" << lname << "Connections get_connections_"
          << lname << " ();";
    }
  else
    {
      os_ << IdentifierHelper::orig_sn (impl->name ()).c_str ()
          << " get_connection_" << lname << " ();";
    }
    
  return 0;
}

int
be_visitor_context_ex_idl::visit_publishes (
  be_publishes *node)
{
  be_type *impl =
    be_type::narrow_from_decl (node->publishes_type ());

  os_ << be_nl
      << "void push_" << node->local_name ()->get_string ()
      << " (in ::"
      << IdentifierHelper::orig_sn (impl->name ()).c_str ()
      << " e);";
      
  return 0;
}

int
be_visitor_context_ex_idl::visit_emits (
  be_emits *node)
{
  be_type *impl =
    be_type::narrow_from_decl (node->emits_type ());

  os_ << be_nl
      << "void push_" << node->local_name ()->get_string ()
      << " (in ::"
      << IdentifierHelper::orig_sn (impl->name ()).c_str ()
      << " e);";
      
  return  0;
}
