
//=============================================================================
/**
 *  @file    home_exs.cpp
 *
 *  $Id$
 *
 *  Visitor generating code for homes in the exec impl source.
 *
 *
 *  @author Jeff Parsons
 */
//=============================================================================

// ******************************************************
// Home visitor for exec impl source
// ******************************************************

be_visitor_home_exs::be_visitor_home_exs (be_visitor_context *ctx)
  : be_visitor_scope (ctx),
    node_ (0),
    comp_ (0),
    os_ (*ctx->stream ()),
    comment_start_border_ ("/**"),
    comment_end_border_ (" */"),
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
  os_ << be_nl_2
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
be_visitor_home_exs::visit_factory (be_factory *node)
{
  AST_Decl *scope = ScopeAsDecl (comp_->defined_in ());
  ACE_CString sname_str (scope->full_name ());
  const char *sname = sname_str.c_str ();
  const char *lname = comp_->local_name ()->get_string ();
  const char *global = (sname_str == "" ? "" : "::");

  os_ << be_nl_2
      << "::Components::EnterpriseComponent_ptr" << be_nl
      << node_->original_local_name ()->get_string ()
      << "_exec_i::" << node->local_name ();

  be_visitor_operation_arglist visitor (this->ctx_);
  visitor.unused (true);

  if (visitor.visit_factory (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_home_exs::")
                         ACE_TEXT ("visit_factory - ")
                         ACE_TEXT ("codegen for arglist failed\n")),
                        -1);
    }

  os_ << be_nl
      << "{" << be_idt_nl
      << your_code_here_ << be_nl
      << "return " << global << sname << "::CCM_"
      << lname << "::_nil ();" << be_uidt_nl
      << "}";

  return 0;
}

int
be_visitor_home_exs::gen_exec_class (void)
{
  // No '_cxx_' prefix.
  const char *lname =
    node_->original_local_name ()->get_string ();

  os_ << be_nl
      << comment_start_border_ << be_nl
      << " * Home Executor Implementation Class: "
      << lname << "_exec_i" << be_nl
      << comment_end_border_;

  os_ << be_nl_2
      << lname << "_exec_i::" << lname << "_exec_i (void)" << be_nl
      << "{" << be_nl
      << "}";

  os_ << be_nl_2
      << lname << "_exec_i::~" << lname << "_exec_i (void)" << be_nl
      << "{" << be_nl
      << "}";

  be_home *h = node_;

  while (h != 0)
    {
      if (this->visit_scope (h) != 0)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("be_visitor_home_exs::")
                             ACE_TEXT ("gen_exec_class - ")
                             ACE_TEXT ("codegen for scope ")
                             ACE_TEXT ("failed\n")),
                            -1);
        }

      for (long i = 0; i < h->n_inherits (); ++i)
        {
          // A closure of all the supported interfaces is stored
          // in the base class 'pd_inherits_flat' member.
          be_interface *bi =
            be_interface::narrow_from_decl (h->inherits ()[i]);

          bi->get_insert_queue ().reset ();
          bi->get_del_queue ().reset ();
          bi->get_insert_queue ().enqueue_tail (bi);

          Home_Exec_Op_Attr_Generator op_attr_gen (this);

          int status =
            bi->traverse_inheritance_graph (op_attr_gen,
                                            &os_,
                                            false,
                                            false);

          if (status == -1)
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 ACE_TEXT ("be_visitor_home_exs::")
                                 ACE_TEXT ("gen_exec_class - ")
                                 ACE_TEXT ("traverse_inheritance_graph() ")
                                 ACE_TEXT ("failed for %s\n"),
                                 bi->full_name ()),
                                -1);
            }
        }

      h = be_home::narrow_from_decl (h->base_home ());
    }

  os_ << be_nl_2
      << "// Implicit operations.";

  os_ << be_nl_2
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

void
be_visitor_home_exs::gen_entrypoint (void)
{
  os_ << be_nl_2
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

