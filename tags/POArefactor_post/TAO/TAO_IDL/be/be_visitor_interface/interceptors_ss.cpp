//
// $Id$
//
// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    interceptors_ss.cpp
//
// = DESCRIPTION
//    This provides code generation for interceptor classes for an
//    interface in the server source.
//
// = AUTHOR
//    Kirthika Parameswaran  <kirthika@cs.wustl.edu>
//
// ============================================================================

ACE_RCSID (be_visitor_interface, 
           interceptors_ss, 
           "$Id$")

// ************************************************************
//  interceptor class in header
// ************************************************************

be_visitor_interface_interceptors_ss::be_visitor_interface_interceptors_ss (
    be_visitor_context *ctx
  )
  : be_visitor_interface (ctx)
{
}

be_visitor_interface_interceptors_ss::~be_visitor_interface_interceptors_ss (
    void
  )
{
}

int be_visitor_interface_interceptors_ss::visit_interface (be_interface *node)
{
  if (node->is_local ())
    {
      return 0;
    }

  TAO_OutStream *os = this->ctx_->stream ();

  *os << be_nl << be_nl
      << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__;

  // Generate code for the interface definition by traversing thru the
  // elements of its scope. We depend on the front-end to have made sure
  // that only legal syntactic elements appear in our scope.
  *os << be_nl << be_nl 
      << "#if (TAO_HAS_INTERCEPTORS == 1)";

  os->reset ();

  // Generate code for the interface definition by traversing thru the
  // elements of its scope. We depend on the front-end to have made sure
  // that only legal syntactic elements appear in our scope.
  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_interface_ch::"
                         "visit_interface - "
                         "codegen for scope failed\n"), 
                        -1);
    }

  *os  << "\n\n#endif /* TAO_HAS_INTERCEPTORS */\n";

  return 0;
}

int 
be_visitor_interface_interceptors_ss::visit_component (be_component *node)
{
  return this->visit_interface (node);
}

int 
be_visitor_interface_interceptors_ss::gen_abstract_ops_helper (
    be_interface *node,
    be_interface *base,
    TAO_OutStream *os
  )
{
  if (!base->is_abstract ())
    {
      return 0;
    }

  AST_Decl *d = 0;
  be_visitor_context ctx;
  ctx.stream (os);
  ctx.state (TAO_CodeGen::TAO_INTERFACE_INTERCEPTORS_SS);

  for (UTL_ScopeActiveIterator si (base, UTL_Scope::IK_decls);
       !si.is_done ();
       si.next ())
    {
      d = si.item ();

      if (d == 0)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_interface_interceptors_ss::"
                             "gen_abstract_ops_helper - "
                             "bad node in this scope\n"),
                            -1);
        }

      UTL_ScopedName *item_new_name  = 0;
      ACE_NEW_RETURN (item_new_name,
                      UTL_ScopedName (d->local_name ()->copy (),
                                      0),
                      -1);

      UTL_ScopedName *base = (UTL_ScopedName *)node->name ()->copy ();
      base->nconc (item_new_name);

      if (d->node_type () == AST_Decl::NT_op)
        {
          // We pass the node's is_abstract flag to the operation
          // constructor so we will get the right generated operation
          // body if we are regenerating an operation from an
          // abstract interface in a concrete interface or component.
          AST_Operation *op = AST_Operation::narrow_from_decl (d);
          be_operation new_op (op->return_type (),
                               op->flags (),
                               0,
                               op->is_local (),
                               node->is_abstract ());
          new_op.set_defined_in (node);
          be_visitor_interface::add_abstract_op_args (op,
                                                      new_op);
          new_op.set_name (base);
          be_visitor_operation_interceptors_ss op_visitor (&ctx);
          op_visitor.visit_operation (&new_op);
        }
      else if (d->node_type () == AST_Decl::NT_attr)
        {
          AST_Attribute *attr = AST_Attribute::narrow_from_decl (d);
          be_attribute new_attr (attr->readonly (),
                                 attr->field_type (),
                                 0,
                                 attr->is_local (),
                                 attr->is_abstract ());
          new_attr.set_defined_in (node);
          new_attr.set_name (base);
          new_attr.be_add_get_exceptions (attr->get_get_exceptions ());
          new_attr.be_add_set_exceptions (attr->get_set_exceptions ());
          be_visitor_attribute attr_visitor (&ctx);
          attr_visitor.visit_attribute (&new_attr);
          ctx.attribute (0);
        }

      base->destroy ();
      delete base;
      base = 0;
    }

  return 0;
}

