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

          *os << "public virtual ::"
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
      *os << "public virtual ::CORBA::AbstractBase" << be_uidt_nl;
    }

  if (has_concrete_parent == 0 && ! node->is_abstract ())
    {
      *os << "public virtual ::CORBA::Object";

      if (nparents > 0)
        {
          *os << be_uidt;
        }

      *os << be_uidt_nl;
    }

  // Generate the body.

  *os << "{" << be_nl
      << "public:" << be_idt_nl;

  if (!node->is_local () && !node->is_abstract ())
    {
      *os << "friend class TAO::Narrow_Utils<"
          << node->local_name () << ">;" << be_nl;
    }
  else if (!node->is_local () && node->is_abstract ())
    {
      *os << "friend class TAO::AbstractBase_Narrow_Utils<"
          << node->local_name () << ">;" << be_nl;
    }

  *os << "typedef " << node->local_name () << "_ptr _ptr_type;"
      << be_nl
      << "typedef " << node->local_name () << "_var _var_type;"
      << be_nl
      << "typedef " << node->local_name () << "_out _out_type;"
      << be_nl << be_nl;

  // Generate the static _duplicate, _narrow, _unchecked_narrow and
  // _nil operations.
  *os << "// The static operations." << be_nl
      << "static " << node->local_name () << "_ptr " << "_duplicate ("
      << node->local_name () << "_ptr obj);" << be_nl << be_nl
      << "static void _tao_release ("
      << node->local_name () << "_ptr obj);" << be_nl << be_nl;

  if (this->gen_xxx_narrow ("_narrow",
                            node,
                            os) == false)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%P|%t) Error in "
                         "be_visitor_interface_ch::"
                         "visit_interface while generating "
                         "_narrow () declaration\n"),
                        -1);
    }

  if (this->gen_xxx_narrow ("_unchecked_narrow",
                            node,
                            os) == false)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%P|%t) Error in "
                         "be_visitor_interface_ch::"
                         "visit_interface while generating "
                         "_unchecked_narrow () declaration\n"),
                        -1);
    }

  // This method is defined in the header file to workaround old
  // g++ problems.
  *os << "static " << node->local_name () << "_ptr _nil (void)"
      << be_nl
      << "{" << be_idt_nl
      << "return static_cast<" << node->local_name ()
      << "_ptr> (0);" << be_uidt_nl
      << "}" << be_nl << be_nl;

  bool gen_any_destructor =
    be_global->any_support ()
    && (!node->is_local () || be_global->gen_local_iface_anyops ());

  if (gen_any_destructor)
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

  if (node->is_local ())
    {
      if (node->convert_parent_ops (this) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                            "(%N:%l) be_visitor_interface_ch::"
                            "visit_interface - "
                            "converting parent ops failed\n"),
                            -1);
        }
    }

  *os << be_nl << be_nl << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__ << be_nl << be_nl;

  // If we inherit from both CORBA::Object and CORBA::AbstractBase,
  // we have to override _add_ref() to avoid ambiguity.
  if (node->has_mixed_parentage ())
    {
      *os << "virtual void _add_ref (void);" << be_nl << be_nl;
    }

  // The _is_a method
  *os << "virtual ::CORBA::Boolean _is_a (const char *type_id);" << be_nl;

  // The _interface_repository_id method.
  *os << "virtual const char* _interface_repository_id (void) const;";

  // The virtual marshal method, to prevent marshal of local iterfaces.
  *os << be_nl
      << "virtual ::CORBA::Boolean marshal (TAO_OutputCDR &cdr);";

  if (be_global->gen_ostream_operators ())
    {
      *os << be_nl
          << "virtual std::ostream &_tao_stream_v (std::ostream &) const;";
    }

  if (! node->is_local () &&
        (be_global->gen_direct_collocation() || be_global->gen_thru_poa_collocation ()))
    {
      // Add the Proxy Broker member variable.
      *os << be_uidt_nl
          << "private:" << be_idt_nl
          << "TAO::Collocation_Proxy_Broker *"
          << "the" << node->base_proxy_broker_name ()
          << "_;";
    }

  *os << be_uidt_nl << be_nl
      << "protected:" << be_idt_nl;

  if (! node->is_local ())
    {
      // Generate the "protected" constructor so that users cannot
      // instantiate us.

      if (! node->is_abstract ())
        {
          *os << "// Concrete interface only." << be_nl
              << node->local_name () << " (void);"
              << be_nl << be_nl;
        }

      if (be_global->gen_direct_collocation() || be_global->gen_thru_poa_collocation ())
        {
          *os << "// These methods travese the inheritance tree and set the"
              << be_nl
              << "// parents piece of the given class in the right mode."
              << be_nl
              << "virtual void " << node->flat_name ()
              << "_setup_collocation (void);" << be_nl << be_nl;
        }
    }

  if (node->is_abstract () || node->is_local ())
    {
      // Protected default constructor for abstract interfaces.
      *os << "// Abstract or local interface only." << be_nl
          << node->local_name () << " (void);" << be_nl << be_nl;
    }

  if (node->is_abstract ())
    {
      // Protected copy constructor for abstract interfaces.
      *os << "// Protected for abstract interfaces." << be_nl
          << node->local_name () << " (const "
          << node->local_name () << " &);" << be_nl << be_nl;
    }

  // Local interfaces don't support stub objects.
  if (! node->is_local ())
    {
      if (! node->is_abstract ())
        {
          *os << "// Concrete non-local interface only." << be_nl
              << node->local_name () << " (" << be_idt << be_idt_nl
              << "::IOP::IOR *ior," << be_nl
              << "TAO_ORB_Core *orb_core);" << be_uidt
              << be_uidt_nl << be_nl;
        }

      *os << "// Non-local interface only." << be_nl
          << node->local_name () << " (" << be_idt << be_idt_nl
          << "TAO_Stub *objref," << be_nl
          << "::CORBA::Boolean _tao_collocated = false," << be_nl
          << "TAO_Abstract_ServantBase *servant = 0," <<  be_nl
          << "TAO_ORB_Core *orb_core = 0);" << be_uidt
          << be_uidt_nl << be_nl;
    }

  // Protected destructor.
  *os << "virtual ~" << node->local_name () << " (void);";

  // Private copy constructor and assignment operator. These are not
  // allowed, hence they are private.
  *os << be_uidt_nl << be_nl
      << "private:" << be_idt_nl;

  // Abstract interfaces have a *protected* copy constructor.
  if (! node->is_abstract ())
    {
      *os << "// Private and unimplemented for concrete interfaces." << be_nl
          << node->local_name () << " (const "
          << node->local_name () << " &);" << be_nl << be_nl;
    }

  *os << "void operator= (const " << node->local_name () << " &);";

  // Generate the embedded RequestInfo classes per operation.
  // This is to be used by interceptors.
  be_visitor_context ctx (*this->ctx_);

  ctx = *this->ctx_;

  *os << be_uidt_nl;
  *os << "};";

  // Don't support smart proxies for local interfaces.
  if (! node->is_local ())
    {
      // List that generates proxy broker factory function pointer.
      be_global->non_local_interfaces.enqueue_tail (node);

      if (be_global->gen_smart_proxies ())
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
        }
    }

  if (be_global->tc_support () && !node->home_equiv ())
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

  node->cli_hdr_gen (true);
  return 0;
}

int
be_visitor_interface_ch::gen_abstract_ops_helper (be_interface *node,
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

  for (UTL_ScopeActiveIterator si (base, UTL_Scope::IK_decls);
       !si.is_done ();
       si.next ())
    {
      d = si.item ();

      if (d == 0)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_interface::"
                             "gen_abstract_ops_helper - "
                             "bad node in this scope\n"),
                            -1);
        }

      if (d->node_type () == AST_Decl::NT_op)
        {

          be_operation *op = be_operation::narrow_from_decl (d);
          op->set_local (node->is_local ());
          ctx.state (TAO_CodeGen::TAO_OPERATION_CH);
          be_visitor_operation_ch op_visitor (&ctx);
          op_visitor.visit_operation (op);
          op->set_local (base->is_local ());
        }
    }

  return 0;
}

bool
be_visitor_interface_ch::gen_xxx_narrow (const char *nar,
                                         be_interface *node,
                                         TAO_OutStream *os)
{
  *os << "static " << node->local_name () << "_ptr "
      << nar << " (";

  if (node->is_abstract ())
    {
      *os << "::CORBA::AbstractBase_ptr obj";
    }
  else
    {
      *os << "::CORBA::Object_ptr obj";
    }

  *os << ");" << be_nl;

  return true;
}
