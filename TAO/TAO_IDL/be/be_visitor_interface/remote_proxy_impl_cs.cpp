//
// $Id$
//

ACE_RCSID (be_visitor_interface, 
           remote_proxy_impl_cs, 
           "$Id$")

be_visitor_interface_remote_proxy_impl_cs::
be_visitor_interface_remote_proxy_impl_cs (be_visitor_context *ctx)
  : be_visitor_interface (ctx)
{
  // No-Op.
}

be_visitor_interface_remote_proxy_impl_cs::
~be_visitor_interface_remote_proxy_impl_cs (void)
{
  // No-Op.
}

int
be_visitor_interface_remote_proxy_impl_cs::visit_interface (
    be_interface *node
  )
{
  TAO_OutStream *os = this->ctx_->stream ();
  os->decr_indent (0);

  *os << be_nl << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__ << be_nl;

  *os << be_nl
      << "///////////////////////////////////////////////////////////////////////" 
      << be_nl
      << "//                Base & Remote Proxy  Implementation. " << be_nl
      << "//" << be_nl << be_nl;
  // Create the destructor implementation for the base
  // proxy implementation.

  // Ctor Impl.
  *os << node->full_base_proxy_impl_name () << "::"
      << node->base_proxy_impl_name () << " (void)" << be_nl
      << "{}" << be_nl << be_nl;

  // Create the destructor implementation for the remote
  // proxy implementation.

  // Ctor Impl
  *os << node->full_remote_proxy_impl_name () << "::"
      << node->remote_proxy_impl_name () << " (void)" << be_nl
      << "{}" << be_nl << be_nl;

  // Generate the code for the Remote Proxy Impl.
  // operations
  *os << "// Remote Implementation of the IDL interface methods";

  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_interface_cs::"
                         "visit_interface - "
                         "codegen for Base Proxy Broker class failed\n"),
                        -1);
    }

  *os << be_nl << be_nl
      << "//" << be_nl
      << "//            End  Base & Remote  Proxy Implemeentation. " << be_nl
      << "///////////////////////////////////////////////////////////////////////"
      << be_nl << be_nl;

  return 0;
}

int 
be_visitor_interface_remote_proxy_impl_cs::gen_abstract_ops_helper (
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
                             "impl_cs::abstract_base_ops_helper - "
                             "bad node in this scope\n"),
                            -1);
        }

      if (d->node_type () == AST_Decl::NT_op)
        {
          UTL_ScopedName *item_new_name  = 0;
          ACE_NEW_RETURN (item_new_name,
                          UTL_ScopedName (d->local_name ()->copy (),
                                          0),
                          -1);

          UTL_ScopedName *base = (UTL_ScopedName *)node->name ()->copy ();
          base->nconc (item_new_name);

          AST_Operation *op = AST_Operation::narrow_from_decl (d);
          be_operation new_op (op->return_type (),
                               op->flags (),
                               0,
                               op->is_local (),
                               op->is_abstract ());
          new_op.set_defined_in (node);
          be_visitor_interface::add_abstract_op_args (op,
                                                      new_op);
          new_op.set_name (base);
          be_visitor_operation_remote_proxy_impl_cs op_visitor (&ctx);
          op_visitor.visit_operation (&new_op);

          base->destroy ();
          delete base;
          base = 0;
        }
    }

  return 0;
}

int be_visitor_interface_remote_proxy_impl_cs::visit_component (
    be_component *node
  )
{
  return this->visit_interface (node);
}


