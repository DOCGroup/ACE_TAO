
//=============================================================================
/**
 *  @file    interface_sh.cpp
 *
 *  $Id$
 *
 *  Visitor generating code for Interfaces in the server header
 *
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================

// ************************************************************
// Interface visitor for server header.
// ************************************************************

be_visitor_interface_sh::be_visitor_interface_sh (be_visitor_context *ctx)
  : be_visitor_interface (ctx)
{
}

be_visitor_interface_sh::~be_visitor_interface_sh (void)
{
}

int
be_visitor_interface_sh::visit_interface (be_interface *node)
{
  if (node->srv_hdr_gen ()
      || node->imported ()
      || node->is_abstract ())
    {
      return 0;
    }

  // If this node is a AMH-RH node, then generate code for it
  if (node->is_local ())
    {
      if (this->is_amh_rh_node (node))
        {
          // Create amh_rh_visitors.
          be_visitor_amh_rh_interface_sh amh_rh_intf (this->ctx_);
          amh_rh_intf.visit_interface (node);
        }

      return 0;
    }

  if (this->generate_amh_classes (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                          ACE_TEXT ("be_visitor_interface_sh::")
                          ACE_TEXT ("visit_interface - ")
                          ACE_TEXT ("codegen for AMH classes failed\n")),
                        -1);
    }

  TAO_OutStream *os  = this->ctx_->stream ();
  ACE_CString class_name;

  // We shall have a POA_ prefix only if we are at the topmost level.
  if (!node->is_nested ())
    {
      // We are outermost.
      class_name += "POA_";
      class_name += node->local_name ();
    }
  else
    {
      class_name +=  node->local_name ();
    }

  *os << be_nl_2;

  *os << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__;

  // Generate the skeleton class name.
  *os << be_nl_2
      << "class " << class_name.c_str () << ";" << be_nl;

  // Generate the _ptr declaration.
  *os << "typedef " << class_name.c_str () << " *"
      << class_name.c_str () << "_ptr;";


  if (be_global->gen_direct_collocation ())
    {
      *os << be_nl_2
          << "class " << node->direct_proxy_impl_name ()
          << ";";
    }

  // Now generate the class definition.
  *os << be_nl_2
      << "class " << be_global->skel_export_macro ()
      << " " << class_name.c_str () << be_idt_nl
      << ": " << be_idt;

  node->gen_skel_inheritance (os);

  *os << be_uidt << be_uidt_nl
      << "{" << be_nl
      << "protected:" << be_idt_nl;

  // Default constructor.
  *os << class_name.c_str () << " (void);"
      << be_uidt_nl << be_nl
      << "public:" << be_idt_nl;

  // Some useful typedefs.
  *os << "/// Useful for template programming." << be_nl
      << "typedef ::" << node->name () << " _stub_type;"
      << be_nl
      << "typedef ::" << node->name () << "_ptr _stub_ptr_type;"
      << be_nl
      << "typedef ::" << node->name () << "_var _stub_var_type;"
      << be_nl_2;

  // Copy constructor and destructor.
  *os << class_name.c_str () << " (const "
      << class_name.c_str () << "& rhs);" << be_nl
      << "virtual ~" << class_name.c_str () << " (void);" << be_nl_2;

  // _is_a
  *os << "virtual ::CORBA::Boolean _is_a (const char* logical_type_id);" << be_nl_2;

  // Add the dispatch method.
  *os << "virtual void _dispatch (" << be_idt << be_idt_nl
      << "TAO_ServerRequest & req," << be_nl
      << "TAO::Portable_Server::Servant_Upcall *servant_upcall);" << be_uidt
      << be_uidt_nl << be_nl;

  this->this_method (node);

  // The _interface_repository_id method.
  *os << be_nl
      << "virtual const char* _interface_repository_id "
      << "(void) const;";

  // Generate code for elements in the scope (e.g., operations).
  if (this->visit_scope (node) ==  -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_interface_sh::")
                         ACE_TEXT ("visit_interface - ")
                         ACE_TEXT ("codegen for scope failed\n")),
                        -1);
    }

  *os << be_uidt_nl << "};";

  be_visitor_context ctx (*this->ctx_);

  // Generate the collocated class.
  ctx = *this->ctx_;

  if (be_global->gen_direct_collocation ())
    {
      ctx.state (TAO_CodeGen::TAO_INTERFACE_DIRECT_PROXY_IMPL_SH);
      be_visitor_interface_direct_proxy_impl_sh idpi_visitor (&ctx);

      if (node->accept (&idpi_visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("be_visitor_interface_sh::")
                             ACE_TEXT ("visit_interface - ")
                             ACE_TEXT ("codegen for direct ")
                             ACE_TEXT ("collocated class failed\n")),
                            -1);
        }
    }

  return 0;
}

int
be_visitor_interface_sh::visit_component (be_component *node)
{
  return this->visit_interface (node);
}

int
be_visitor_interface_sh::visit_connector (be_connector *node)
{
  return this->visit_interface (node);
}

int
be_visitor_interface_sh::gen_abstract_ops_helper (
  be_interface *node,
  be_interface *base,
  TAO_OutStream *os)
{
  if (!base->is_abstract ())
    {
      return 0;
    }

  AST_Decl *d = 0;
  be_visitor_context ctx;
  ctx.stream (os);
  ctx.state (TAO_CodeGen::TAO_ROOT_SH);

  for (UTL_ScopeActiveIterator si (base, UTL_Scope::IK_decls);
       !si.is_done ();
       si.next ())
    {
      d = si.item ();

      if (d == 0)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("be_visitor_interface_sh::")
                             ACE_TEXT ("gen_abstract_ops_helper - ")
                             ACE_TEXT ("bad node in this scope\n")),
                            -1);
        }

      UTL_ScopedName item_new_name (d->local_name (),
                                    0);

      if (d->node_type () == AST_Decl::NT_op)
        {
          be_operation *op = be_operation::narrow_from_decl (d);
          be_visitor_operation_sh op_visitor (&ctx);
          op_visitor.visit_operation (op);
        }
      else if (d->node_type () == AST_Decl::NT_attr)
        {
          AST_Attribute *attr = AST_Attribute::narrow_from_decl (d);
          be_attribute new_attr (attr->readonly (),
                                 attr->field_type (),
                                 &item_new_name,
                                 attr->is_local (),
                                 attr->is_abstract ());
          new_attr.set_defined_in (node);

          UTL_ExceptList *get_exceptions = attr->get_get_exceptions ();

          if (0 != get_exceptions)
            {
              new_attr.be_add_get_exceptions (get_exceptions->copy ());
            }

          UTL_ExceptList *set_exceptions = attr->get_set_exceptions ();

          if (0 != set_exceptions)
            {
              new_attr.be_add_set_exceptions (set_exceptions->copy ());
            }

          be_visitor_attribute attr_visitor (&ctx);
          attr_visitor.visit_attribute (&new_attr);
          ctx.attribute (0);
          new_attr.destroy ();
        }
    }

  return 0;
}

void
be_visitor_interface_sh::this_method (be_interface *node)
{
  TAO_OutStream *os = this->ctx_->stream ();

  // Print out the _this() method.
  *os << "::" << node->full_name () << " *_this (void);"
      << be_nl;
}

int
be_visitor_interface_sh::generate_amh_classes (be_interface *node)
{
  // We have to check for any abstract ancestor until AMH is integrated
  // with abstract interfaces. If the node itself is abstract, this
  // visitor would not be created.
  if (be_global->gen_amh_classes () && !node->has_mixed_parentage ())
  {
    be_visitor_amh_interface_sh amh_intf (this->ctx_);
    return amh_intf.visit_interface (node);
  }

  return 0;
}
