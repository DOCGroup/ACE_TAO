
//=============================================================================
/**
 *  @file    facet_exs.cpp
 *
 *  $Id$
 *
 *  Visitor generating code for facets in the exec impl source.
 *
 *
 *  @author Jeff Parsons
 */
//=============================================================================

be_visitor_facet_exs::be_visitor_facet_exs (
      be_visitor_context *ctx)
  : be_visitor_component_scope (ctx),
    op_scope_ (0),
    comment_border_ ("//=============================="
                     "==============================")
{
}

be_visitor_facet_exs::~be_visitor_facet_exs (void)
{
}

int
be_visitor_facet_exs::visit_operation (be_operation *node)
{
  AST_Decl::NodeType nt =
    ScopeAsDecl (node->defined_in ())->node_type ();

  // Components have implied IDL operations added to the AST, but
  // we are interested only in supported interface operations.
  if (nt == AST_Decl::NT_component || nt == AST_Decl::NT_connector)
    {
      return 0;
    }
    
  be_visitor_operation_exs v (this->ctx_);
  v.scope (op_scope_);
  return v.visit_operation (node);
}

int
be_visitor_facet_exs::visit_attribute (be_attribute *node)
{
  AST_Decl::NodeType nt =
    ScopeAsDecl (node->defined_in ())->node_type ();

  // Components have implied IDL operations added to the AST, but
  // we are interested only in supported interface operations.
  if (nt == AST_Decl::NT_component || nt == AST_Decl::NT_connector)
    {
      return 0;
    }
    
  be_visitor_attribute v (this->ctx_);
  v.op_scope (op_scope_);
  return v.visit_attribute (node);
}

int
be_visitor_facet_exs::visit_provides (be_provides *node)
{
  be_type *impl = node->provides_type ();

  /// For the moment, we are generating multiple facet
  /// executor classes if the same interface is used in
  /// multiple components. I'm leaving the code here in
  /// case we change our minds later.
/*
  if (impl->exec_src_facet_gen ())
    {
      return 0;
    }
*/    
  // We don't want any '_cxx_' prefix here.
  const char *lname =
    impl->original_local_name ()->get_string ();

  os_ << be_nl << be_nl
      << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__;
      
  os_ << be_nl
      << comment_border_ << be_nl
      << "// Facet Executor Implementation Class: "
      << lname << "_exec_i" << be_nl
      << comment_border_;

  AST_Decl *c_scope = ScopeAsDecl (this->node_->defined_in ());
  bool is_global = (c_scope->node_type () == AST_Decl::NT_root);
  const char *smart_scope = (is_global ? "" : "::");
      
  os_ << be_nl << be_nl
      << lname << "_exec_i::" << lname
      << "_exec_i (" << be_idt << be_idt << be_idt_nl
      << smart_scope << c_scope->full_name () << "::CCM_"
      << this->node_->local_name () << "_Context_ptr ctx)"
      << be_uidt << be_uidt_nl
      << ": ciao_context_ (" << be_idt << be_idt_nl
      << smart_scope << c_scope->full_name ()
      << "::CCM_" << this->node_->local_name ()
      << "_Context::_duplicate (ctx))"
      << be_uidt << be_uidt << be_uidt_nl
      << "{" << be_nl
      << "}";

  os_ << be_nl << be_nl
      << lname << "_exec_i::~" << lname
      << "_exec_i (void)" << be_nl
      << "{" << be_nl
      << "}";

  if (impl->node_type () == AST_Decl::NT_interface)
    {
      be_interface *intf =
        be_interface::narrow_from_decl (impl);

      op_scope_ = intf;

      os_ << be_nl << be_nl
          << "// Operations from ::" << intf->full_name ();

      /// The overload of traverse_inheritance_graph() used here
      /// doesn't automatically prime the queues.
      intf->get_insert_queue ().reset ();
      intf->get_del_queue ().reset ();
      intf->get_insert_queue ().enqueue_tail (intf);

      Component_Exec_Op_Attr_Generator op_attr_gen (this);

      int status =
        intf->traverse_inheritance_graph (op_attr_gen,
                                          &os_,
                                          false,
                                          false);

      if (status == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("be_visitor_facet_exs::")
                             ACE_TEXT ("visit_provides - ")
                             ACE_TEXT ("traverse_inheritance_graph() ")
                             ACE_TEXT ("failed\n")),
                            -1);
        }
    }

//  impl->exec_src_facet_gen (true);

  return 0;
}

