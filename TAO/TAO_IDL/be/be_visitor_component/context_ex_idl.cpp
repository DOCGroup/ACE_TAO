
//=============================================================================
/**
 *  @file    context_ex_idl.cpp
 *
 *  $Id$
 *
 *  Visitor generating code for the contect class
 *  in the CIAO executor IDL.
 *
 *
 *  @author Jeff Parsons
 */
//=============================================================================


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
  be_type *impl = node->uses_type ();

  os_ << be_nl
      << "::";

  ACE_CString port_name (this->port_prefix_);
  port_name += node->local_name ()->get_string ();
  const char *lname = port_name.c_str ();

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

  if (be_global->ami4ccm_call_back ())
    {
      if (node->is_multiple ())
        {
          os_ << IdentifierHelper::orig_sn (node_->name ()).c_str ()
              << "::" << lname << "Connections get_connections_"
              << lname << " ();";
        }
      else
        {
          ACE_CString original_op_name (
            impl->name ()->last_component ()->get_string ());
          ACE_CString new_op_name = ACE_CString ("AMI_") + original_op_name;
          UTL_ScopedName *op_name =
            static_cast<UTL_ScopedName *> (impl->name ()->copy ());
          op_name->last_component ()->replace_string (new_op_name.c_str ());
          os_ << be_nl << "::" << IdentifierHelper::orig_sn (op_name).c_str ()
              << " get_connection_sendc_" << lname << " ();";
        }
    }

  return 0;
}

int
be_visitor_context_ex_idl::visit_publishes (
  be_publishes *node)
{
  be_eventtype *impl = node->publishes_type ();

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
  be_eventtype *impl = node->emits_type ();

  os_ << be_nl
      << "void push_" << node->local_name ()->get_string ()
      << " (in ::"
      << IdentifierHelper::orig_sn (impl->name ()).c_str ()
      << " e);";

  return  0;
}
