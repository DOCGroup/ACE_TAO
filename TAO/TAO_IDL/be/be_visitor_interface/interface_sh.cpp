//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    interface_sh.cpp
//
// = DESCRIPTION
//    Visitor generating code for Interfaces in the server header
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

ACE_RCSID (be_visitor_interface, 
           interface_sh, 
           "$Id$")

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
                         "be_visitor_interface_sh::"
                         "visit_interface - "
                         "codegen for AMH classes failed\n"),
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

  *os << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__ << be_nl << be_nl;

  // Generate the skeleton class name.
  *os << "class " << class_name.c_str () << ";" << be_nl;

  // Generate the _ptr declaration.
  *os << "typedef " << class_name.c_str () << " *" << class_name.c_str ()
      << "_ptr;" << be_nl << be_nl;

  // Forward class declarations.
  if (be_global->gen_thru_poa_collocation ())
    {
      *os << "class " << node->thru_poa_proxy_impl_name () << ";" << be_nl;
    }

  if (be_global->gen_direct_collocation ())
    {
      *os << "class " << node->direct_proxy_impl_name () << ";" << be_nl;
    }

  if (be_global->gen_thru_poa_collocation ()
      || be_global->gen_direct_collocation ())
    {
      *os << "class " << node->strategized_proxy_broker_name ()
          << ";" << be_nl;
    }

  *os << be_nl;

  // Now generate the class definition.
  *os << "class " << be_global->skel_export_macro ()
      << " " << class_name.c_str () << be_idt_nl << ": " << be_idt;

  long n_parents = node->n_inherits ();
  AST_Interface *parent = 0;
  int has_concrete_parent = 0;

  for (int i = 0; i < n_parents; ++i)
    {
      parent = node->inherits ()[i];
      
      if (parent->is_abstract ())
        {
          continue;
        }

      if (has_concrete_parent == 1)
        {
          *os << "," << be_nl;
        }

      *os << "public virtual " << "POA_"
          << parent->name ();

      has_concrete_parent = 1;
    }

  if (has_concrete_parent == 0)
    {
      // We don't inherit from another user defined object, hence our
      // base class is the ServantBase class.
      *os << "public virtual PortableServer::ServantBase";
    }

  *os << be_uidt << be_uidt_nl
      << "{" << be_nl
      << "protected:" << be_idt_nl;

  // Default constructor.
  *os << class_name.c_str () << " (void);" << be_uidt_nl << be_nl
      << "public:" << be_idt_nl;

  // Copy constructor and destructor.
  *os << class_name.c_str () << " (const " 
      << class_name.c_str () << "& rhs);" << be_nl
      << "virtual ~" << class_name.c_str () << " (void);" << be_nl << be_nl;

  // _is_a
  *os << "virtual CORBA::Boolean _is_a (" << be_idt << be_idt_nl
      << "const char* logical_type_id" << be_nl
      << "ACE_ENV_ARG_DECL_WITH_DEFAULTS" << be_uidt_nl
      << ");" << be_uidt_nl << be_nl;

  // _downcast
  *os << "virtual void* _downcast (" << be_idt << be_idt_nl
      << "const char* logical_type_id" << be_uidt_nl
      << ");" << be_uidt_nl << be_nl;

  // Add a skeleton for our _is_a method.
  *os << "static void _is_a_skel (" << be_idt << be_idt_nl
      << "TAO_ServerRequest &req," << be_nl
      << "void *servant," << be_nl
      << "void *servant_upcall" << be_nl
      << "ACE_ENV_ARG_DECL" << be_uidt_nl
      << ");" << be_uidt_nl << be_nl;

  // Add a skeleton for our _non_existent method.
  *os << "static void _non_existent_skel (" << be_idt << be_idt_nl
      << "TAO_ServerRequest &req," << be_nl
      << "void *servant," << be_nl
      << "void *servant_upcall" << be_nl
      << "ACE_ENV_ARG_DECL" << be_uidt_nl
      << ");" << be_uidt_nl << be_nl;

  // Add a skeleton for our _interface method.
  *os << "static void _interface_skel (" << be_idt << be_idt_nl
      << "TAO_ServerRequest &req," << be_nl
      << "void *servant," << be_nl
      << "void *servant_upcall" << be_nl
      << "ACE_ENV_ARG_DECL" << be_uidt_nl
      << ");" << be_uidt_nl << be_nl;

  // Add a skeleton for our _component method.
  *os << "static void _component_skel (" << be_idt << be_idt_nl
      << "TAO_ServerRequest &req," << be_nl
      << "void *obj," << be_nl
      << "void *servant_upcall" << be_nl
      << "ACE_ENV_ARG_DECL" << be_uidt_nl
      << ");" << be_uidt_nl << be_nl;

  // Add the dispatch method.
  *os << "virtual void _dispatch (" << be_idt << be_idt_nl
      << "TAO_ServerRequest &req," << be_nl
      << "void *_servant_upcall" << be_nl
      << "ACE_ENV_ARG_DECL" << be_uidt_nl
      << ");" << be_uidt_nl << be_nl;

  this->this_method (node);

  // The _interface_repository_id method.
  *os << be_nl
      << "virtual const char* _interface_repository_id "
      << "(void) const;";

  // Generate code for elements in the scope (e.g., operations).
  if (this->visit_scope (node) ==  -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_interface_sh::"
                         "visit_interface - "
                         "codegen for scope failed\n"),
                        -1);
    }

  // Generate skeletons for operations of our base classes. These
  // skeletons just cast the pointer to the appropriate type
  // before invoking the call.
  int status = 
    node->traverse_inheritance_graph (
              be_interface::gen_skel_helper, 
              os
            );

  if (status == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_interface_sh::"
                         "visit_interface - "
                         "inheritance graph traversal failed\n"),
                        -1);
    }

  *os << be_uidt_nl << "};";

  // Generate the embedded RequestInfo classes per operation.
  // This is to be used by interceptors.
  be_visitor_context ctx (*this->ctx_);

  if (be_global->gen_thru_poa_collocation ()
      || be_global->gen_direct_collocation ())
    {
      ctx = *this->ctx_;
      // Generate strategized proxy broker.
      be_visitor_interface_strategized_proxy_broker_sh ispb_visitor (&ctx);

      if (node->accept (&ispb_visitor) == -1)
        {
          ACE_ERROR_RETURN ((
              LM_ERROR,
              "be_visitor_interface_sh::"
              "visit_interface - "
              "codegen for thru_poa_collocated class failed\n"
            ),
            -1
          );
        }
    }

  ctx = *this->ctx_;

  // Generate the collocated class.
  if (be_global->gen_thru_poa_collocation ())
    {
      ctx = *this->ctx_;
      ctx.state (TAO_CodeGen::TAO_INTERFACE_THRU_POA_PROXY_IMPL_SH);
      be_visitor_interface_thru_poa_proxy_impl_sh itppi_visitor (&ctx);

      if (node->accept (&itppi_visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "be_visitor_interface_sh::"
                             "visit_interface - "
                             "codegen for thru_poa_collocated class failed\n"),
                            -1);
        }
    }

  ctx = *this->ctx_;

  if (be_global->gen_direct_collocation ())
    {
      ctx = *this->ctx_;
      ctx.state (TAO_CodeGen::TAO_INTERFACE_DIRECT_PROXY_IMPL_SH);
      be_visitor_interface_direct_proxy_impl_sh idpi_visitor (&ctx);

      if (node->accept (&idpi_visitor) == -1)
        {
          ACE_ERROR_RETURN ((
              LM_ERROR,
              "be_visitor_interface_sh::"
              "visit_interface - "
              "codegen for thru_poa_collocated class failed\n"
            ),
            -1
          );
        }
    }

  return 0;
}

int 
be_visitor_interface_sh::gen_abstract_ops_helper (
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
                             "(%N:%l) be_visitor_interface_sh::"
                             "abstract_base_ops_helper - "
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
          be_visitor_operation_sh op_visitor (&ctx);
          op_visitor.visit_operation (&new_op);
        }
    }

  return 0;
}

void
be_visitor_interface_sh::this_method (be_interface *node)
{
  TAO_OutStream *os = this->ctx_->stream ();

  // Print out the _this() method.
  *os << "::" << node->full_name () << " *_this (" << be_idt << be_idt_nl
      << "ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS" << be_uidt_nl
      << ");" << be_uidt << be_nl;
}

int
be_visitor_interface_sh::generate_amh_classes (be_interface *node)
{
   if (be_global->gen_amh_classes ())
    {
      be_visitor_amh_interface_sh amh_intf (this->ctx_);
      return amh_intf.visit_interface (node);
    }

  return 0;
}
