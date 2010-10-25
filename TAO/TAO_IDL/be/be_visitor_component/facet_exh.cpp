
//=============================================================================
/**
 *  @file    facet_exh.cpp
 *
 *  $Id$
 *
 *  Visitor generating code for facets in the exec impl header.
 *
 *
 *  @author Jeff Parsons
 */
//=============================================================================

be_visitor_facet_exh::be_visitor_facet_exh (
      be_visitor_context *ctx)
  : be_visitor_component_scope (ctx),
    comment_start_border_ ("/**"),
    comment_end_border_ (" */")
{
  // This is initialized in the base class to svnt_export_macro()
  // or skel_export_macro(), since there are many more visitor
  // classes generating servant code. So we can just override
  // all that here.
  export_macro_ = be_global->exec_export_macro ();
}

be_visitor_facet_exh::~be_visitor_facet_exh (void)
{
}

int
be_visitor_facet_exh::visit_provides (be_provides *node)
{
  be_type *impl = node->provides_type ();
  const char *iname =
    impl->original_local_name ()->get_string ();

  ACE_CString lname_str (this->ctx_->port_prefix ());
  lname_str += node->original_local_name ()->get_string ();
  const char *lname = lname_str.c_str ();

  AST_Decl *s = ScopeAsDecl (impl->defined_in ());
  ACE_CString sname_str =
    IdentifierHelper::orig_sn (s->name (), false);
  const char *sname = sname_str.c_str ();
  const char *global = (sname_str == "" ? "" : "::");

  AST_Decl *c_scope = ScopeAsDecl (this->node_->defined_in ());
  bool is_global = (c_scope->node_type () == AST_Decl::NT_root);
  const char *smart_scope = (is_global ? "" : "::");

  os_ << be_nl_2
      << comment_start_border_ << be_nl
      << " * Provider Executor Implementation Class: "
      << lname << "_exec_i" << be_nl
      << comment_end_border_;

  os_ << be_nl_2
      << "class "
      << lname << "_exec_i" << be_idt_nl
      << ": public virtual " << global << sname << "::CCM_"
      << iname << "," << be_idt_nl
      << "public virtual ::CORBA::LocalObject"
      << be_uidt << be_uidt_nl
      << "{" << be_nl
      << "public:" << be_idt_nl
      << lname << "_exec_i (" << be_idt_nl
      << smart_scope << c_scope->full_name () << "::CCM_"
      << this->node_->local_name ()
      << "_Context_ptr ctx);" << be_uidt_nl
      << "virtual ~" << lname << "_exec_i (void);";

  if (impl->node_type () == AST_Decl::NT_interface)
    {
      be_interface *intf =
        be_interface::narrow_from_decl (impl);

      os_ << be_nl_2
          << "//@{" << be_nl
          << "/** Operations and attributes from ::"
          << intf->full_name () << ". */";

      int const status =
        intf->traverse_inheritance_graph (
          be_visitor_facet_exh::method_helper,
          &os_);

      if (status == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("be_visitor_facet_exh::")
                             ACE_TEXT ("visit_provides - ")
                             ACE_TEXT ("traverse_inheritance_graph() ")
                             ACE_TEXT ("failed\n")),
                            -1);
        }
        
      os_ << be_nl << "//@}";
    }

  os_ << be_uidt << be_nl_2
      << "private:" << be_idt_nl
      << smart_scope << c_scope->full_name () << "::CCM_"
      << this->node_->local_name ()
      << "_Context_var ciao_context_;" << be_uidt_nl
      << "};";

  return 0;
}

int
be_visitor_facet_exh::method_helper (be_interface *derived,
                                     be_interface *node,
                                     TAO_OutStream *os)
{
  be_visitor_context ctx;
  ctx.state (TAO_CodeGen::TAO_ROOT_IH);
  ctx.interface (derived);
  ctx.stream (os);
  be_visitor_interface_ih visitor (&ctx);

  if (visitor.visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_facet_exh::method_helper "
                         "- visit_scope() failed\n"),
                        -1);
    }

  return 0;
}

