//
// $Id$
//

ACE_RCSID (be_visitor_interface, 
           base_proxy_impl_ch, 
           "$Id$")

be_visitor_interface_base_proxy_impl_ch::
be_visitor_interface_base_proxy_impl_ch (be_visitor_context *ctx)
  : be_visitor_interface (ctx)
{
  // No-Op.
}


be_visitor_interface_base_proxy_impl_ch::
~be_visitor_interface_base_proxy_impl_ch (void)
{
  // No-Op.
}

int
be_visitor_interface_base_proxy_impl_ch::visit_interface (be_interface *node)
{
  TAO_OutStream *os = this->ctx_->stream ();

  *os << be_nl
      << "///////////////////////////////////////////////////////////////////////" 
      << be_nl
      << "//                    Base Proxy Impl. Declaration" << be_nl
      << "//" << be_nl << be_nl;

  *os << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__ << be_nl << be_nl;

  // Generate Class Declaration.
  *os << "class " << be_global->stub_export_macro ()
      << " " << node->base_proxy_impl_name () << be_idt_nl
      << ": ";

  int n_parents = node->n_inherits ();
  int has_concrete_parent = 0;

  if (n_parents > 0)
    {
      *os << be_idt;

      for (int i = 0; i < n_parents; ++i)
        {
          be_interface *inherited =
            be_interface::narrow_from_decl (node->inherits ()[i]);

          if (inherited->is_abstract ())
            {
              continue;
            }

          if (has_concrete_parent == 1)
            {
               // Node is the case of multiple
               // inheritance, so put a comma.
              *os << ", " << be_nl;
            }

          *os << "public virtual ";
          *os << inherited->full_base_proxy_impl_name ();

          has_concrete_parent = 1;
        }

      if (has_concrete_parent == 1)
        {
          *os << be_uidt << be_uidt_nl;
        }
    }

  if (has_concrete_parent == 0)
    {
      *os << "public virtual TAO_Object_Proxy_Impl" << be_uidt << be_uidt_nl;
    }

  *os << "{" << be_nl << "public:"
      << be_idt_nl; // idt = 1

  // Destructor Declaration.
  *os << "virtual ~" << node->base_proxy_impl_name () << " (void) { }"
      << be_nl;

  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_interface_base_proxy_impl_ch::"
                         "visit_interface - "
                         "codegen for scope failed\n"), 
                        -1);
    }

  *os << be_uidt_nl;

  // Constructor Declaration.
  *os << "protected:" << be_idt_nl // idt = 1
      << node->base_proxy_impl_name () << " (void);"
      << be_uidt_nl;

  *os << "};" << be_nl << be_nl
      << "//" << be_nl
      << "//               End Base Proxy Impl. Declaration" << be_nl
      << "///////////////////////////////////////////////////////////////////////"
      << be_nl << be_nl;

  return 0;
}

int 
be_visitor_interface_base_proxy_impl_ch::gen_abstract_ops_helper (
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
                             "(%N:%l) be_visitor_interface_base_proxy_impl_ch"
                             "::abstract_base_ops_helper - "
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
          ctx.state (TAO_CodeGen::TAO_OPERATION_BASE_PROXY_IMPL_CH);
          be_visitor_operation_base_proxy_impl_ch op_visitor (&ctx);
          op_visitor.visit_operation (&new_op);
        }
    }

  return 0;
}

