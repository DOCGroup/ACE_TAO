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

#include "be_visitor_typecode/typecode_decl.h"

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

  TAO_OutStream *os = this->ctx_->stream ();
  long i;

  // == STEP 1:  generate the class name and class names we inherit ==

  *os << be_nl << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__ << be_nl;

  // Generate the ifdefined macro for  the _ptr type.
  os->gen_ifdef_macro (node->flat_name (),
                       "_ptr");


  // The following two are required to be under the ifdef macro to avoid
  // multiple declarations.

  // Forward declaration.
  *os << "class " << node->local_name () << ";" << be_nl;
  // Generate the _ptr declaration.
  *os << "typedef " << node->local_name () << " *"
      << node->local_name () << "_ptr;" << be_nl;

  os->gen_endif ();

  // Generate the ifdefined macro for the var type.
  os->gen_ifdef_macro (node->flat_name (), "_var");

  // Generate the _var declaration.
  if (node->gen_var_defn () == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_interface_ch::"
                         "visit_interface - "
                         "codegen for _var failed\n"),
                        -1);
    }

  os->gen_endif ();

  // Generate the ifdef macro for the _out class.
  os->gen_ifdef_macro (node->flat_name (),
                       "_out");

  // Generate the _out declaration.
  if (node->gen_out_defn () == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_interface_ch::"
                         "visit_interface - "
                         "codegen for _out failed\n"), 
                        -1);
    }

  // Generate the endif macro.
  os->gen_endif ();

  // The above code could have been executed by the forward declaration
  // as long as it wasn't imported. The code below can only be
  // executed by an interface definition, also non-imported.
  if (node->imported ())
    {
      return 0;
    }

  *os << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__ << be_nl << be_nl;

  // Now the interface definition itself.
  os->gen_ifdef_macro (node->flat_name ());

  if (!node->is_local () && !node->is_abstract ())
    {
      // Forward class declaration
      *os << "// Forward Classes Declaration." << be_nl
          << "class " << node->base_proxy_impl_name () << ";" << be_nl
          << "class " << node->remote_proxy_impl_name () << ";" << be_nl
          << "class " << node->base_proxy_broker_name () << ";" << be_nl
          << "class " << node->remote_proxy_broker_name () << ";"
          << be_nl << be_nl;
    }

  // Now generate the class definition.
  *os << "class " << be_global->stub_export_macro ()
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
  else if (node->is_abstract ())
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

  // No Any operator for local interfaces.
  if (! node->is_local () && be_global->any_support ())
    {
      *os << "static void _tao_any_destructor (void*);" << be_nl << be_nl;
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

  *os << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__ << be_nl << be_nl;

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
  *os << "virtual void *_tao_QueryInterface (ptr_arith_t type);"
      << be_nl << be_nl;

  // The _interface_repository_id method.
  *os << "virtual const char* _interface_repository_id (void) const;\n"
      << be_uidt_nl;

  if (! node->is_local () && ! node->is_abstract ())
    {
      // Add the Proxy Broker member variable.
      *os << "private:" << be_idt_nl
          << node->base_proxy_broker_name () << " *"
          << "the" << node->base_proxy_broker_name ()
          << "_;" << be_nl <<  be_uidt_nl;
    }

  *os << "protected:" << be_idt_nl;

  if (! node->is_local () && ! node->is_abstract ())
    {
      // Generate the "protected" constructor so that users cannot
      // instantiate us.

      *os << node->local_name () << " (int collocated = 0);"
          << be_nl << be_nl;

      *os << "// These methods travese the inheritance tree and set the"
          << be_nl
          << "// parents piece of the given class in the right mode"
          << be_nl
          << "virtual void " << node->flat_name ()
          << "_setup_collocation (int collocated);" << be_nl << be_nl;
    }
  else
    {
      *os << node->local_name () << " (void);" << be_nl << be_nl;
    }

  // Local and abstract interfaces don't support stub objects.
  if (! node->is_local () && ! node->is_abstract ())
    {
      *os << node->local_name ()
          << " (" << be_idt << be_idt_nl << "TAO_Stub *objref, " << be_nl
          << "CORBA::Boolean _tao_collocated = 0," << be_nl
          << "TAO_Abstract_ServantBase *servant = 0" <<  be_uidt_nl
          << ");" << be_uidt_nl << be_nl;

      // Friends declarations.
      *os << "friend class " << node->remote_proxy_impl_name () << ";"
          << be_nl
          << "friend class " << node->thru_poa_proxy_impl_name () << ";"
          << be_nl
          << "friend class " << node->direct_proxy_impl_name () << ";"
          << be_nl << be_nl;
    }

  // Protected copy constructor for abstract interfaces.
  if (node->is_abstract ())
    {
      *os << node->local_name () << " (const " 
          << node->local_name () << " &);" << be_nl << be_nl;
    }

  // Protected destructor.
  *os << "virtual ~" << node->local_name () << " (void);"
      << be_uidt_nl << be_nl;

  // Private copy constructor and assignment operator. These are not
  // allowed, hence they are private.
  *os << "private:" << be_idt_nl;

  // Abstract interfaces have a *protected* copy constructor.
  if (! node->is_abstract ())
    {
      *os << node->local_name () << " (const "
          << node->local_name () << " &);" << be_nl;
    }

  *os << "void operator= (const " << node->local_name () << " &);";

  // Generate the embedded RequestInfo classes per operation.
  // This is to be used by interceptors.
  be_visitor *visitor = 0;
  be_visitor_context ctx (*this->ctx_);

  ctx = *this->ctx_;

  *os << be_uidt_nl;
  *os << "};" << be_nl << be_nl;

  // Don't support smart proxies for local interfaces.
  if (! node->is_local () && ! node->is_abstract ())
    {
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
      ctx.state (TAO_CodeGen::TAO_INTERFACE_PROXY_IMPLS_CH);
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
      ctx.state (TAO_CodeGen::TAO_INTERFACE_PROXY_BROKERS_CH);
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
      ctx.state (TAO_CodeGen::TAO_TYPECODE_DECL);
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
