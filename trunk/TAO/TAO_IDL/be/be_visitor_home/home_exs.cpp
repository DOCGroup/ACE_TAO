//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO_IDL_BE
//
// = FILENAME
//    home_exs.cpp
//
// = DESCRIPTION
//    Visitor generating code for homes in the exec impl source.
//
// = AUTHOR
//    Jeff Parsons
//
// ============================================================================

ACE_RCSID (be_visitor_home,
           home_exs,
           "$Id$")

// ******************************************************
// Home visitor for exec impl source
// ******************************************************

be_visitor_home_exs::be_visitor_home_exs (be_visitor_context *ctx)
  : be_visitor_scope (ctx),
    node_ (0),
    comp_ (0),
    os_ (*ctx->stream ()),
    comment_border_ ("//=============================="
                     "=============================="),
    your_code_here_ ("/* Your code here. */"),
    export_macro_ (be_global->exec_export_macro ())
{
}

be_visitor_home_exs::~be_visitor_home_exs (void)
{
}

int
be_visitor_home_exs::visit_home (be_home *node)
{
  if (node->imported ())
    {
      return 0;
    }

  node_ = node;
  comp_ = node_->managed_component ();

  /// CIDL-generated namespace used 'CIDL_' + composition name.
  /// Now we use 'CIAO_' + component's flat name.
  os_ << be_nl << be_nl
      << "namespace CIAO_" << comp_->flat_name () << "_Impl" << be_nl
      << "{" << be_idt;

  if (this->gen_exec_class () == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_home_exs::")
                         ACE_TEXT ("visit_home - ")
                         ACE_TEXT ("gen_servant_class() failed\n")),
                        -1);
    }

  this->gen_entrypoint ();

  os_ << be_uidt_nl
      << "}";

  return 0;
}

int
be_visitor_home_exs::visit_operation (be_operation *node)
{
  be_visitor_operation_exs v (this->ctx_);
  v.scope (node_);
  return v.visit_operation (node);
}

int
be_visitor_home_exs::visit_attribute (be_attribute *node)
{
  be_visitor_attribute v (this->ctx_);
  v.op_scope (node_);
  return v.visit_attribute (node);
}

int
be_visitor_home_exs::gen_exec_class (void)
{
  // No '_cxx_' prefix.
  const char *lname =
    node_->original_local_name ()->get_string ();

  os_ << be_nl
      << comment_border_ << be_nl
      << "// Home Executor Implementation Class: "
      << lname << "_exec_i" << be_nl
      << comment_border_;

  os_ << be_nl << be_nl
      << lname << "_exec_i::" << lname << "_exec_i (void)" << be_nl
      << "{" << be_nl
      << "}";

  os_ << be_nl << be_nl
      << lname << "_exec_i::~" << lname << "_exec_i (void)" << be_nl
      << "{" << be_nl
      << "}";

  this->gen_ops_attrs ();

  this->gen_factories ();

  this->gen_finders ();

  os_ << be_nl << be_nl
      << "// Implicit operations.";

  os_ << be_nl << be_nl
      << "::Components::EnterpriseComponent_ptr" << be_nl
      << lname << "_exec_i::create (void)" << be_nl
      << "{" << be_idt_nl
      << "::Components::EnterpriseComponent_ptr retval ="
      << be_idt_nl
      << "::Components::EnterpriseComponent::_nil ();"
      << be_uidt_nl << be_nl
      << "ACE_NEW_THROW_EX (" << be_idt_nl
      << "retval," << be_nl
      << comp_->original_local_name ()->get_string ()
      << "_exec_i," << be_nl
      << "::CORBA::NO_MEMORY ());"
      << be_uidt_nl << be_nl
      << "return retval;" << be_uidt_nl
      << "}";

  return 0;
}

int
be_visitor_home_exs::gen_ops_attrs (void)
{
  os_ << be_nl << be_nl
      << "// All operations and attributes.";

  node_->get_insert_queue ().reset ();
  node_->get_del_queue ().reset ();
  node_->get_insert_queue ().enqueue_tail (node_);

  Home_Exec_Op_Attr_Generator op_attr_gen (this);

  int status =
    node_->traverse_inheritance_graph (op_attr_gen,
                                       &os_,
                                       false,
                                       false);

  if (status == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_home_exs::")
                         ACE_TEXT ("gen_ops_attrs - ")
                         ACE_TEXT ("traverse_inheritance_graph() ")
                         ACE_TEXT ("failed\n")),
                        -1);
    }

  return 0;
}

int
be_visitor_home_exs::gen_factories (void)
{
  os_ << be_nl << be_nl
      << "// Factory operations.";

  return this->gen_factories_r (node_);
}

int
be_visitor_home_exs::gen_factories_r (AST_Home *node)
{
  if (node == 0)
    {
      return 0;
    }

  if (this->gen_init_ops_i (node->factories ()) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_home_exs::")
                         ACE_TEXT ("gen_factories_r - ")
                         ACE_TEXT ("gen_init_ops_i() failed\n")),
                        -1);
    }

  AST_Home *base = node->base_home ();

  return this->gen_factories_r (base);
}

int
be_visitor_home_exs::gen_finders (void)
{
  os_ << be_nl << be_nl
      << "// Finder operations.";

  return this->gen_finders_r (node_);
}

int
be_visitor_home_exs::gen_finders_r (AST_Home *node)
{
  if (node == 0)
    {
      return 0;
    }

  if (this->gen_init_ops_i (node->finders ()) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_home_exs::")
                         ACE_TEXT ("gen_finders_r - ")
                         ACE_TEXT ("gen_init_ops_i() failed\n")),
                        -1);
    }

  AST_Home *base = node->base_home ();

  return this->gen_finders_r (base);
}

int
be_visitor_home_exs::gen_init_ops_i (AST_Home::INIT_LIST & list)
{
  AST_Decl *scope = ScopeAsDecl (comp_->defined_in ());
  ACE_CString sname_str (scope->full_name ());
  const char *sname = sname_str.c_str ();
  const char *lname = comp_->local_name ()->get_string ();
  const char *global = (sname_str == "" ? "" : "::");

  AST_Operation **op = 0;

  for (AST_Home::INIT_LIST::ITERATOR i = list.begin ();
       !i.done ();
       i.advance ())
    {
      i.next (op);
      be_operation *bop = be_operation::narrow_from_decl (*op);

      /// Return type for home exec factories and finders is not
      /// the same as for the corresponding home servant, so we
      /// generate the return type and op name by hand, then finish
      /// the operation traversal with an arglist visitor.
      os_ << be_nl << be_nl
          << "::Components::EnterpriseComponent_ptr" << be_nl
          << node_->original_local_name ()->get_string ()
          << "_exec_i::"
          << bop->local_name ();

      be_visitor_operation_arglist visitor (this->ctx_);
      visitor.unused (true);

      if (visitor.visit_operation (bop) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("be_visitor_home_exs::")
                             ACE_TEXT ("gen_init_ops_i - ")
                             ACE_TEXT ("visit_operation() failed\n")),
                            -1);
        }

      os_ << be_nl
          << "{" << be_idt_nl
          << your_code_here_ << be_nl
          << "return " << global << sname << "::CCM_"
          << lname << "::_nil ();" << be_uidt_nl
          << "}";
    }

  return 0;
}

void
be_visitor_home_exs::gen_entrypoint (void)
{
  os_ << be_nl << be_nl
      << "extern \"C\" " << export_macro_.c_str ()
      << " ::Components::HomeExecutorBase_ptr" << be_nl
      << "create_" << node_->flat_name ()
      << "_Impl (void)" << be_nl
      << "{" << be_idt_nl
      << "::Components::HomeExecutorBase_ptr retval =" << be_idt_nl
      << "::Components::HomeExecutorBase::_nil ();"
      << be_uidt_nl << be_nl
      << "ACE_NEW_NORETURN (" << be_idt_nl
      << "retval," << be_nl
      << node_->original_local_name ()->get_string ()
      << "_exec_i);" << be_uidt_nl << be_nl
      << "return retval;" << be_uidt_nl
      << "}";
}

Home_Exec_Op_Attr_Generator::Home_Exec_Op_Attr_Generator (
    be_visitor_scope * visitor)
  : visitor_ (visitor)
{
}

int
Home_Exec_Op_Attr_Generator::emit (be_interface * /* derived_interface */,
                                   TAO_OutStream * /* os */,
                                   be_interface * base_interface)
{
  return visitor_->visit_scope (base_interface);
}


