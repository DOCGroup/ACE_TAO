//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    component_ch.cpp
//
// = DESCRIPTION
//    Visitor generating code for Components in the client header.
//
// = AUTHOR
//    Jeff Parsons
//
// ============================================================================

ACE_RCSID (be_visitor_component,
           component_ch,
           "$Id$")

// ******************************************************
// Component visitor for client header
// ******************************************************

be_visitor_component_ch::be_visitor_component_ch (be_visitor_context *ctx)
  : be_visitor_component (ctx)
{
}

be_visitor_component_ch::~be_visitor_component_ch (void)
{
}

int
be_visitor_component_ch::visit_component (be_component *node)
{
  if (node->cli_hdr_gen () || node->imported ())
    {
      return 0;
    }

  // This will be a no-op if it has already been done by a forward
  // declaration.
  node->gen_var_out_seq_decls ();

  TAO_OutStream *os = this->ctx_->stream ();

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
          << "class " << node->remote_proxy_broker_name () << ";"
          << be_nl << be_nl;
    }

  // Now generate the class definition.
  *os << "class " << be_global->stub_export_macro ()
      << " " << node->local_name () << be_idt_nl
      << ": public virtual ::" ;

  AST_Component *parent = node->base_component ();

  if (parent != 0)
    {
      *os << parent->name ();
    }
  else
    {
      *os << "Components::CCMObject";
    }

  long nsupports = node->n_inherits ();

  if (nsupports > 0)
    {
      *os << be_idt;

      for (long i = 0; i < nsupports; ++i)
        {
          *os << ", " << be_nl
              << "public virtual ::" << node->supports()[i]->name ();
        }

      *os << be_uidt;
    }

  *os << be_uidt_nl
      << "{" << be_nl
      << "public:" << be_idt_nl

      << "friend class TAO::Narrow_Utils<"
      << node->local_name () << ">;" << be_nl

    // Generate the _ptr_type and _var_type typedefs.
      << "typedef " << node->local_name () << "_ptr _ptr_type;"
      << be_nl
      << "typedef " << node->local_name () << "_var _var_type;"
      << be_nl
      << "typedef " << node->local_name () << "_out _out_type;"
      << be_nl;

  // Generate the static _duplicate, _narrow, and _nil operations.
  *os << "// The static operations." << be_nl
      << "static " << node->local_name () << "_ptr " << "_duplicate ("
      << node->local_name () << "_ptr obj);" << be_nl << be_nl
      << "static " << node->local_name () << "_ptr "
      << "_narrow (::CORBA::Object_ptr obj);" << be_nl;

  // This method is defined in the header file to workaround old
  // g++ problems.
  *os << "static " << node->local_name () << "_ptr _nil (void)"
      << be_idt_nl << "{" << be_idt_nl
      << "return static_cast<" << node->local_name ()
      << "_ptr> (0);" << be_uidt_nl
      << "}" << be_uidt_nl << be_nl;

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
                         "(%N:%l) be_visitor_component_ch::"
                         "visit_component - "
                         "codegen for scope failed\n"),
                        -1);
    }

  *os << be_nl << be_nl << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__;

  if (! node->is_abstract ())
    {
      node->analyze_parentage ();
    }

  // If we inherit from both CORBA::Object and CORBA::AbstractBase,
  // we have to override _add_ref() to avoid ambiguity. A comopnent
  // can have mixed parentage if it supports an abstract or mixed-
  // parentage interface.
  if (node->has_mixed_parentage ())
    {
      *os << be_nl << be_nl
          << "virtual void _add_ref (void);";
    }

  *os << be_nl << be_nl
      << "virtual ::CORBA::Boolean _is_a (const char *type_id);";

  // The _interface_repository_id method.
  *os << be_nl << be_nl
      << "virtual const char* _interface_repository_id (void) const;";

  // The virtual marshal method, to prevent marshal of local iterfaces.
  *os << be_nl << "virtual ::CORBA::Boolean marshal (TAO_OutputCDR &cdr);";

  if (be_global->gen_direct_collocation() || be_global->gen_thru_poa_collocation ())
    {
      // Add the Proxy Broker member variable.
      *os << be_uidt_nl << be_nl
          << "private:" << be_idt_nl
          << "TAO::Collocation_Proxy_Broker *"
          << "the" << node->base_proxy_broker_name ()
          << "_;";
    }

  *os << be_uidt_nl <<  be_nl
      << "protected:" << be_idt;

  // Generate the "protected" constructor so that users cannot
  // instantiate us.

  *os << be_nl
      << node->local_name () << " (void);";

  if (be_global->gen_direct_collocation() || be_global->gen_thru_poa_collocation ())
    {
      *os << be_nl << be_nl
          << "// These methods travese the inheritance tree and set the"
          << be_nl
          << "// parents piece of the given class in the right mode."
          << be_nl
          << "virtual void " << node->flat_name ()
          << "_setup_collocation (void);";
    }

  *os << be_nl << be_nl
      << node->local_name () << " (" << be_idt << be_idt_nl
      << "::IOP::IOR *ior," << be_nl
      << "TAO_ORB_Core *orb_core" << be_uidt_nl
      << ");" << be_uidt;

  *os << be_nl << be_nl
      << node->local_name ()
      << " (" << be_idt << be_idt_nl << "TAO_Stub *objref, " << be_nl
      << "::CORBA::Boolean _tao_collocated = false," << be_nl
      << "TAO_Abstract_ServantBase *servant = 0," << be_nl
      << "TAO_ORB_Core *oc = 0" << be_uidt_nl
      << ");" << be_uidt;

  // Friends declarations.
  *os << be_nl << be_nl
      << "friend class " << node->remote_proxy_impl_name () << ";"
      << be_nl
      << "friend class " << node->direct_proxy_impl_name () << ";";

  // Protected destructor.
  *os << be_nl << be_nl
      << "virtual ~" << node->local_name () << " (void);";

  // Private copy constructor and assignment operator. These are not
  // allowed, hence they are private.
  *os << be_uidt_nl << be_nl
      << "private:" << be_idt_nl;
  *os << node->local_name () << " (const "
      << node->local_name () << " &);"
      << be_nl
      << "void operator= (const " << node->local_name () << " &);";

  *os << be_uidt_nl
      << "};" << be_nl << be_nl;

  // List that generates proxy broker factory function pointer.
  be_global->non_local_interfaces.enqueue_tail (node);

  be_visitor_context ctx (*this->ctx_);
  if (be_global->gen_smart_proxies ())
    {
       *os << be_nl << be_nl;
       // Smart Proxy related classes.
       ctx.state (TAO_CodeGen::TAO_INTERFACE_SMART_PROXY_CH);
       be_visitor_interface_smart_proxy_ch sp_visitor (&ctx);

       if (node->accept (&sp_visitor) == -1)
         {
           ACE_ERROR_RETURN ((LM_ERROR,
                              "be_visitor_component_ch::"
                              "visit_component - "
                              "codegen for smart proxy classes failed\n"),
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
                             "(%N:%l) be_visitor_component_ch::"
                             "visit_component - "
                             "TypeCode declaration failed\n"),
                            -1);
        }
    }

  node->cli_hdr_gen (true);
  return 0;
}
