
//=============================================================================
/**
 *  @file    executor_ex_idl.cpp
 *
 *  $Id$
 *
 *  Visitor generating code for components
 *  in the CIAO executor IDL.
 *
 *
 *  @author Jeff Parsons
 */
//=============================================================================

be_visitor_executor_ex_idl::be_visitor_executor_ex_idl (
      be_visitor_context *ctx)
  : be_visitor_component_scope (ctx)
{
}

be_visitor_executor_ex_idl::~be_visitor_executor_ex_idl (
  void)
{
}

int
be_visitor_executor_ex_idl::visit_component (
  be_component *node)
{
  node_ = node;

  AST_Component *base = node->base_component ();

  os_ << be_nl
      << "local interface CCM_"
      << node->original_local_name ()->get_string ()
      << be_idt_nl
      << ": ";

  if (base == 0)
    {
      os_ << "::Components::EnterpriseComponent";

      this->gen_supported ();
    }
  else
    {
      ACE_CString sname_str =
        IdentifierHelper::orig_sn (
          ScopeAsDecl (base->defined_in ())->name ());

      const char *sname = sname_str.c_str ();
      const char *global = (sname_str == "" ? "" : "::");

      os_ << global << sname << "::CCM_"
          << base->original_local_name ()->get_string ();
    }

  os_ << be_uidt_nl
      << "{" << be_idt;

  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_executor_ex_idl::")
                         ACE_TEXT ("visit_component - ")
                         ACE_TEXT ("visit_scope() failed\n")),
                        -1);
    }

  os_ << be_uidt_nl
      << "};";

  return 0;
}

int
be_visitor_executor_ex_idl::visit_connector (be_connector *node)
{
  return this->visit_component (node);
}

int
be_visitor_executor_ex_idl::visit_attribute (be_attribute *node)
{
  AST_Decl::NodeType snt = this->node_->node_type ();
  AST_Decl::NodeType ant =
    ScopeAsDecl (node->defined_in ())->node_type ();

  /// For now, we want porttype attributes generated only for
  /// connectors.
  if (snt == AST_Decl::NT_component
      && ant == AST_Decl::NT_porttype)
    {
      return 0;
    }

  bool rd_only = node->readonly ();

  // Keep output statements separate because of side effects.
  // No need to check for anonymous array - anonymous types not
  // accepted by parser for attributes.
  os_ << be_nl
     << (rd_only ? "readonly " : "") << "attribute ";

  be_type *ft = node->field_type ();

  os_ << IdentifierHelper::type_name (ft, this);
  os_ << " " << this->ctx_->port_prefix ().c_str ()
     << IdentifierHelper::try_escape (node->original_local_name ()).c_str ();

  this->gen_exception_list (node->get_get_exceptions (),
                           rd_only ? "" : "get");

  this->gen_exception_list (node->get_set_exceptions (),
                           "set");

  os_ << ";";

  return 0;
}

int
be_visitor_executor_ex_idl::visit_sequence (
  be_sequence *node)
{
  // Keep output statements separate because of side effects.
  os_ << "sequence<";

  be_type *bt = node->base_type ();

  os_ << IdentifierHelper::type_name (bt, this);

  if (!node->unbounded ())
    {
      os_ << ", " << node->max_size ()->ev ()->u.ulval;
    }

  os_ << "> ";

  return 0;
}

int
be_visitor_executor_ex_idl::visit_string (be_string *node)
{
  bool wide =
    static_cast<size_t> (node->width ()) > sizeof (char);

  os_ << (wide ? "w" : "") << "string";

  ACE_CDR::ULong const bound =
    node->max_size ()->ev ()->u.ulval;

  if (bound > 0UL)
    {
      os_ << "<" << bound << ">";
    }

  return 0;
}

int
be_visitor_executor_ex_idl::visit_provides (
  be_provides *node)
{
  be_type *impl = node->provides_type ();

  AST_Decl *scope = ScopeAsDecl (impl->defined_in ());

  ACE_CString sname_str =
    IdentifierHelper::orig_sn (scope->name ());
  const char *sname = sname_str.c_str ();

  const char *lname =
    impl->original_local_name ()->get_string ();

  const char *global = (sname_str == "" ? "" : "::");

  ACE_CString prefix (this->ctx_->port_prefix ());
  prefix += node->local_name ()->get_string ();
  const char *port_name = prefix.c_str ();

  os_ << be_nl
      << global << sname << "::CCM_" << lname << " get_"
      << port_name << " ();";

  return 0;
}

int
be_visitor_executor_ex_idl::visit_consumes (
  be_consumes *node)
{
  be_eventtype *impl = node->consumes_type ();

  os_ << be_nl
      << "void push_" << node->local_name ()->get_string ()
      << " (in ::"
      << IdentifierHelper::orig_sn (impl->name ()).c_str ()
      << " e);";

  return 0;
}

void
be_visitor_executor_ex_idl::gen_supported (void)
{
  os_ << be_idt;

  AST_Type **supported = node_->supports ();

  for (long i = 0; i < node_->n_supports (); ++i)
    {
      os_ << "," << be_nl
          << "::"
          << IdentifierHelper::orig_sn (supported[i]->name ()).c_str ();
    }

  os_ << be_uidt;
}

void
be_visitor_executor_ex_idl::gen_exception_list (
  UTL_ExceptList *exceptions,
  const char *prefix,
  bool closed)
{
  if (exceptions != 0 && exceptions->length () > 0)
    {
      os_ << be_idt_nl
          << prefix << "raises ( ";

      for (UTL_ExceptlistActiveIterator ei (exceptions);
           !ei.is_done ();)
        {
          os_ << "::"
              << IdentifierHelper::orig_sn (ei.item ()->name ()).c_str ();

          ei.next ();

          if (!ei.is_done () || !closed)
            {
              os_ << ", ";
            }
        }

      if (closed)
        {
          os_ << ")" << be_uidt;
        }
    }
}

