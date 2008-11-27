//
// $Id$
//

ACE_RCSID (be_visitor_interface, 
           remote_proxy_impl_ch, 
           "$Id$")

be_visitor_interface_remote_proxy_impl_ch::
be_visitor_interface_remote_proxy_impl_ch (be_visitor_context *ctx)
  : be_visitor_interface (ctx)
{
  // No-Op.
}


be_visitor_interface_remote_proxy_impl_ch::
~be_visitor_interface_remote_proxy_impl_ch (void)
{
  // No-Op.
}

int
be_visitor_interface_remote_proxy_impl_ch::visit_interface (
    be_interface *node
  )
{
  TAO_OutStream *os = this->ctx_->stream ();

  *os << be_nl
      << "///////////////////////////////////////////////////////////////////////"
      << be_nl
      << "//                Remote Proxy Impl. Declaration" << be_nl
      << "//" << be_nl << be_nl;

  *os << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__ << be_nl << be_nl;

  // Generate Class Declaration.
  *os << "class " << be_global->stub_export_macro ()
      << " " << node->remote_proxy_impl_name () << be_idt_nl;
  *os << ": " << "public virtual " << node->base_proxy_impl_name ()
      << "," << be_idt_nl << "public virtual " 
      << "TAO_Remote_Object_Proxy_Impl";

  if (node->node_type () == AST_Decl::NT_component)
    {
      be_component *bc = be_component::narrow_from_decl (node);
      AST_Component *ac_base = bc->base_component ();

      if (ac_base != 0)
        {
          be_component *bc_base = be_component::narrow_from_decl (ac_base);

          *os << "," << be_nl
              << "public virtual "
              << bc_base->full_remote_proxy_impl_name ();
        }
    }

  int nparents = node->n_inherits ();

  if (nparents > 0)
    {
      for (int i = 0; i < nparents; ++i)
        {
          be_interface *inherited =
            be_interface::narrow_from_decl (node->inherits ()[i]);

          if (inherited->is_abstract ())
            {
              continue;
            }

          *os << "," << be_nl;
          *os << "public virtual ";
          *os << inherited->full_remote_proxy_impl_name ();
        }
    }

  *os << be_uidt << be_uidt_nl;
  *os << "{" << be_nl << "public:" << be_idt_nl;

  // Constructor Declaration.
  *os << node->remote_proxy_impl_name () << " (void);"
      << be_nl  << be_nl;

  // Destructor Declaration.
  *os << "virtual ~" << node->remote_proxy_impl_name () << " (void) {}";

  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_interface_ch::"
                         "visit_interface - "
                         "codegen for scope failed\n"),
                        -1);
    }

  *os << be_uidt_nl << "};" << be_nl;
  *os << be_nl
      << "//" << be_nl
      << "//             End Remote Proxy Impl. Declaration" << be_nl
      << "///////////////////////////////////////////////////////////////////////"
      << be_nl << be_nl;

  return 0;
}

int 
be_visitor_interface_remote_proxy_impl_ch::gen_abstract_ops_helper (
    be_interface *node,
    be_interface *base,
    TAO_OutStream *os
  )
{
  if (node == base)
    {
      return 0;
    }

  AST_Decl *d = 0;
  be_visitor_context ctx;
  ctx.stream (os);

  for (UTL_ScopeActiveIterator si (base, UTL_Scope::IK_decls);
       !si.is_done ();
       si.next ())
    {
      d = si.item ();

      if (d == 0)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_interface_remote_proxy_"
                             "impl_ch::abstract_base_ops_helper - "
                             "bad node in this scope\n"),
                            -1);
        }

      if (d->node_type () == AST_Decl::NT_op)
        {
          UTL_ScopedName item_new_name (d->local_name (),
                                        0);

          AST_Operation *op = AST_Operation::narrow_from_decl (d);
          be_operation new_op (op->return_type (),
                               op->flags (),
                               &item_new_name,
                               op->is_local (),
                               op->is_abstract ());
          new_op.set_defined_in (node);
          be_visitor_interface::add_abstract_op_args (op,
                                                      new_op);
          be_visitor_operation_proxy_impl_xh op_visitor (&ctx);
          op_visitor.visit_operation (&new_op);
        }
    }

  return 0;
}

int be_visitor_interface_remote_proxy_impl_ch::visit_component (
    be_component *node
  )
{
  return this->visit_interface (node);
}

