//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    interface_ch.cpp
//
// = DESCRIPTION
//    Visitor generating code for Interfaces in the client header
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

ACE_RCSID (be_visitor_interface,
           interface_ch,
           "$Id$")
// ******************************************************
// Interface visitor for client header
// ******************************************************

be_visitor_interface_ch::be_visitor_interface_ch (be_visitor_context *ctx)
  : be_visitor_interface (ctx)
{
}

be_visitor_interface_ch::~be_visitor_interface_ch (void)
{
}

int
be_visitor_interface_ch::visit_interface (be_interface *node)
{
  if (node->cli_hdr_gen () || node->imported ())
    {
      return 0;
    }

  // This will be a no-op if it has already been done by a forward
  // declaration.
  node->gen_var_out_seq_decls ();

  TAO_OutStream *os = this->ctx_->stream ();
  long i;

  *os << be_nl << be_nl << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__;

  // Now the interface definition itself.
  os->gen_ifdef_macro (node->flat_name ());

  if (!node->is_local () && !node->is_abstract ())
    {
      // Forward class declarations.
      *os << be_nl << be_nl
          << "class " << node->base_proxy_impl_name () << ";" << be_nl
          << "class " << node->remote_proxy_impl_name () << ";" << be_nl
          << "class " << node->base_proxy_broker_name () << ";" << be_nl
          << "class " << node->remote_proxy_broker_name () << ";";
    }

  // Now generate the class definition.
  *os << be_nl << be_nl
      << "class " << be_global->stub_export_macro ()
      << " " << node->local_name () << be_idt_nl
      << ": " ;

  long nparents = node->n_inherits ();
  int has_concrete_parent = 0;

  // If node interface inherits from other interfaces.
  if (nparents > 0)
    {
      *os << be_idt;

      for (i = 0; i < nparents; ++i)
        {
          if (! node->inherits ()[i]->is_abstract ())
            {
              has_concrete_parent = 1;
            }

          *os << "public virtual "
              << node->inherits ()[i]->name ();

          if (i < nparents - 1)
            {
              // Node has multiple inheritance, so put a comma.
              *os << "," << be_nl;
            }
        }

      if (has_concrete_parent == 1 || node->is_abstract ())
        {
          *os << be_uidt << be_uidt_nl;
        }
      else if (! node->is_abstract ())
        {
          *os << "," << be_nl;
        }
    }

  if (node->is_abstract () && nparents == 0)
    {
      *os << "public virtual CORBA::AbstractBase" << be_uidt_nl;
    }

  if (has_concrete_parent == 0 && ! node->is_abstract ())
    {
      *os << "public virtual CORBA::Object";

      if (nparents > 0)
        {
          *os << be_uidt;
        }

      *os << be_uidt_nl;
    }

  // Generate the body.

  *os << "{" << be_nl
      << "public:" << be_idt_nl

      // Generate the _ptr_type and _var_type typedefs.
      << "typedef " << node->local_name () << "_ptr _ptr_type;"
      << be_nl
      << "typedef " << node->local_name () << "_var _var_type;"
      << be_nl;

  // Generate the static variable that we use for narrowing.
  *os << "static int _tao_class_id;" << be_nl << be_nl;

  // Generate the static _duplicate, _narrow, and _nil operations.
  *os << "// The static operations." << be_nl
      << "static " << node->local_name () << "_ptr " << "_duplicate ("
      << node->local_name () << "_ptr obj);" << be_nl << be_nl
      << "static " << node->local_name () << "_ptr "
      << "_narrow (" << be_idt << be_idt_nl;

  if (node->is_abstract ())
    {
      *os << "CORBA::AbstractBase_ptr obj" << be_nl;
    }
  else
    {
      *os << "CORBA::Object_ptr obj" << be_nl;
    }

  *os << "ACE_ENV_ARG_DECL_WITH_DEFAULTS" << be_uidt_nl
      << ");" << be_uidt_nl << be_nl;

  // There's no need for an _unchecked_narrow for locality
  // constrained object.
  *os << "static " << node->local_name () << "_ptr "
      << "_unchecked_narrow (" << be_idt << be_idt_nl;

  if (node->is_abstract ())
    {
      *os << "CORBA::AbstractBase_ptr obj" << be_nl;
    }
  else
    {
      *os << "CORBA::Object_ptr obj" << be_nl;
    }

  *os << "ACE_ENV_ARG_DECL_WITH_DEFAULTS" << be_uidt_nl
      << ");" << be_uidt_nl << be_nl;

  // This method is defined in the header file to workaround old
  // g++ problems.
  *os << "static " << node->local_name () << "_ptr _nil (void)"
      << be_idt_nl << "{" << be_idt_nl
      << "return (" << node->local_name ()
      << "_ptr)0;" << be_uidt_nl
      << "}" << be_uidt_nl << be_nl;

  if (node->is_abstract ())
    {
      *os << "static " << node->local_name ()
          << "_ptr _downcast (CORBA::AbstractBase_ptr abs);"
          << be_nl << be_nl;
    }

  if (be_global->any_support ())
    {
      *os << "static void _tao_any_destructor (void *);";
    }

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

  *os << be_nl << be_nl << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__ << be_nl << be_nl;

  if (! node->is_abstract ())
    {
      node->analyze_parentage ();
    }

  // If we inherit from both CORBA::Object and CORBA::AbstractBase,
  // we have to override _add_ref() to avoid ambiguity, because it is
  // called in _tao_Queryinterface().
  if (node->has_mixed_parentage ())
    {
      *os << "virtual void _add_ref (void);" << be_nl << be_nl;
    }

  // The _is_a method
  if (! node->is_local ())
    {
      *os << "virtual CORBA::Boolean _is_a (" << be_idt << be_idt_nl
          << "const char *type_id" << be_nl
          << "ACE_ENV_ARG_DECL_WITH_DEFAULTS" << be_uidt_nl
          << ");" << be_uidt_nl << be_nl;
    }

  // The _tao_QueryInterface method.
  *os << "virtual void *_tao_QueryInterface (ptrdiff_t type);"
      << be_nl << be_nl;

  // The _interface_repository_id method.
  *os << "virtual const char* _interface_repository_id (void) const;";

  // The virtual marshal method, to prevent marshal of local iterfaces.
  *os << be_nl << "virtual CORBA::Boolean marshal (TAO_OutputCDR &cdr);";

  if (node->is_abstract ())
    {
      *os << be_nl << be_nl
          << "virtual void *_tao_obv_narrow (ptrdiff_t type_id);"
          << "\n#if defined (_MSC_VER)" << be_nl
          << "virtual void *" << node->flat_name ()
          << "_tao_obv_narrow (ptrdiff_t type_id);"
          << "\n#endif /* _MSC_VER */";
    }

  if (! node->is_local () && ! node->is_abstract ())
    {
      // Add the Proxy Broker member variable.
      *os << be_uidt_nl
          << "private:" << be_idt_nl
          << node->base_proxy_broker_name () << " *"
          << "the" << node->base_proxy_broker_name ()
          << "_;";
    }

  *os << be_uidt_nl << be_nl
      << "protected:" << be_idt_nl;

  if (! node->is_local () && ! node->is_abstract ())
    {
      // Generate the "protected" constructor so that users cannot
      // instantiate us.

      *os << node->local_name () << " (int collocated = 0);"
          << be_nl << be_nl;

      *os << "// These methods travese the inheritance tree and set the"
          << be_nl
          << "// parents piece of the given class in the right mode."
          << be_nl
          << "virtual void " << node->flat_name ()
          << "_setup_collocation (int collocated);" << be_nl << be_nl;
    }

  if (node->is_abstract () || node->is_local ())
    {
      // Protected default constructor for abstract interfaces.
      *os << node->local_name () << " (void);" << be_nl;
    }

  if (node->is_abstract ())
    {
      // Protected copy constructor for abstract interfaces.
      *os << node->local_name () << " (const "
          << node->local_name () << " &);" << be_nl;
    }


  // Local interfaces don't support stub objects.
  if (! node->is_local ())
    {
      *os << node->local_name ()
          << " (IOP::IOR *ior," << be_idt_nl
          << be_idt << "TAO_ORB_Core *orb_core = 0);" << be_uidt_nl
          << be_uidt_nl;

      *os << node->local_name ()
          << " (" << be_idt << be_idt_nl << "TAO_Stub *objref, " << be_nl
          << "CORBA::Boolean _tao_collocated = 0," << be_nl
          << "TAO_Abstract_ServantBase *servant = 0," <<  be_nl
          << "TAO_ORB_Core *orb_core = 0" << be_uidt_nl
          << ");" << be_uidt_nl;
    }

  // Protected destructor.
  *os << "virtual ~" << node->local_name () << " (void);";

  if (! node->is_abstract () && ! node->is_local ())
    {
      // Friends declarations.
      *os << be_nl << be_nl
          << "friend class " << node->remote_proxy_impl_name () << ";"
          << be_nl
          << "friend class " << node->thru_poa_proxy_impl_name () << ";"
          << be_nl
          << "friend class " << node->direct_proxy_impl_name () << ";";
    }

  // Private copy constructor and assignment operator. These are not
  // allowed, hence they are private.
  *os << be_uidt_nl << be_nl
      << "private:" << be_idt_nl;

  // Abstract interfaces have a *protected* copy constructor.
  if (! node->is_abstract ())
    {
      *os << node->local_name () << " (const "
          << node->local_name () << " &);" << be_nl;
    }

  *os << "void operator= (const " << node->local_name () << " &);";

  // Generate the embedded RequestInfo classes per operation.
  // This is to be used by interceptors.
  be_visitor_context ctx (*this->ctx_);

  ctx = *this->ctx_;

  *os << be_uidt_nl;
  *os << "};";

  // Don't support smart proxies for local interfaces.
  // @@@ (JP) This is TODO for abstract interfaces.
  if (! node->is_local () && ! node->is_abstract ())
    {
      *os << be_nl << be_nl;

      // Smart Proxy related classes.
      ctx.state (TAO_CodeGen::TAO_INTERFACE_SMART_PROXY_CH);
      be_visitor_interface_smart_proxy_ch sp_visitor (&ctx);

      if (node->accept (&sp_visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "be_visitor_interface_ch::"
                             "visit_interface - "
                             "codegen for smart proxy classes failed\n"),
                            -1);
        }

      // Proxy Implementation Declaration.
      ctx = *this->ctx_;
      be_visitor_interface_proxy_impls_ch spi_visitor (&ctx);

      if (node->accept (&spi_visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "be_visitor_interface_ch::"
                             "visit_interface - "
                             "codegen for Proxy Broker classes failed\n"),
                            -1);
        }

      // Proxy Broker Declaration.
      ctx = *this->ctx_;
      be_visitor_interface_proxy_brokers_ch pb_visitor (&ctx);

      if (node->accept (&pb_visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "be_visitor_interface_ch::"
                             "visit_interface - "
                             "codegen for Proxy Broker classes failed\n"),
                            -1);
        }
    }

  os->gen_endif ();

  if (be_global->tc_support ())
    {
      be_visitor_typecode_decl td_visitor (&ctx);

      if (node->accept (&td_visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_interface_ch::"
                             "visit_interface - "
                             "TypeCode declaration failed\n"),
                            -1);
        }
    }

  node->cli_hdr_gen (I_TRUE);
  return 0;
}

int
be_visitor_interface_ch::gen_abstract_ops_helper (be_interface *node,
                                                  be_interface *base,
                                                  TAO_OutStream *os)
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
                             "(%N:%l) be_interface::"
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
          ctx.state (TAO_CodeGen::TAO_OPERATION_CH);
          be_visitor_operation_ch op_visitor (&ctx);
          op_visitor.visit_operation (&new_op);
        }
    }

  return 0;
}
