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

#include        "idl.h"
#include        "idl_extern.h"
#include        "be.h"

#include "be_visitor_interface.h"

ACE_RCSID(be_visitor_interface, interface_ch, "$Id$")


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
  TAO_OutStream *os; // output stream
  long i;            // loop index

  if (!node->cli_hdr_gen () && !node->imported ()) // not already generated and
                                                   // not imported
    {
      // Grab the stream.
      os = this->ctx_->stream ();

      // == STEP 1:  generate the class name and class names we inherit ==

      // generate the ifdefined macro for  the _ptr type
      os->gen_ifdef_macro (node->flat_name (), "_ptr");


      // the following two are required to be under the ifdef macro to avoid
      // multiple declarations

      os->indent (); // start with whatever indentation level we are at
      // forward declaration
      *os << "class " << node->local_name () << ";" << be_nl;
      // generate the _ptr declaration
      *os << "typedef " << node->local_name () << " *" << node->local_name ()
          << "_ptr;" << be_nl;

      os->gen_endif ();

      // generate the ifdefined macro for the var type
      os->gen_ifdef_macro (node->flat_name (), "_var");

      // generate the _var declaration
      if (node->gen_var_defn () == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_interface_ch::"
                             "visit_interface - "
                             "codegen for _var failed\n"), -1);
        }
      os->gen_endif ();

      // generate the ifdef macro for the _out class
      os->gen_ifdef_macro (node->flat_name (), "_out");

      // generate the _out declaration - ORBOS/97-05-15 pg 16-20 spec
      if (node->gen_out_defn () == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_interface_ch::"
                             "visit_interface - "
                             "codegen for _out failed\n"), -1);
        }
      // generate the endif macro
      os->gen_endif ();

      // now the interface definition itself
      os->gen_ifdef_macro (node->flat_name ());

      if (!node->is_local ())
        {
          // Forward class declaration
          *os << "// Forward Classes Declaration" << be_nl
              << "class " << node->base_proxy_impl_name () << ";" << be_nl
              << "class " << node->remote_proxy_impl_name () << ";" << be_nl
              << "class " << node->base_proxy_broker_name () << ";" << be_nl
              << "class " << node->remote_proxy_broker_name () << ";" << be_nl
              << be_nl;
        }
      // now generate the class definition
      *os << "class " << be_global->stub_export_macro ()
          << " " << node->local_name ();

      idl_bool base_class_name_clash = 0;
      be_interface *clashing_parent = 0;

      if (node->n_inherits () > 0)  // node interface inherits from other
                                    // interfaces
        {
          *os << ": ";
          for (i = 0; i < node->n_inherits (); i++)
            {
              be_interface *inherited =
                be_interface::narrow_from_decl (node->inherits ()[i]);

              be_decl *scope = 0;

              if (inherited->is_nested ())
                {
                  // inherited node is used in the scope of "node" node
                  scope =
                    be_scope::narrow_from_scope (node->defined_in ())->decl ();

                  if (!ACE_OS::strcmp (inherited->local_name (), 
                                       node->local_name ()))
                    {
                      base_class_name_clash = 1;
                      clashing_parent = inherited;
                    }              
                }

              *os << "public virtual ";
              *os << inherited->nested_type_name (scope);
              if (i < node->n_inherits () - 1) // node is the case of multiple
                                               // inheritance, so put a comma
                {
                  *os << ", ";
                }
            }  // end of for loop
          *os << be_nl;
        }
      else
        {
          // we do not inherit from anybody, hence we do so from the base
          // CORBA::Object class
          // Generate code that uses the macro. This is required to deal with
          // the MSVC++ insanity
          *os << " : public virtual CORBA_Object" << be_nl;
        }

      // generate the body

      *os << "{" << be_nl
          << "public:" << be_nl

          // generate the _ptr_type and _var_type typedefs
          // but we must protect against certain versions of g++
          << "#if !defined(__GNUC__) || !defined (ACE_HAS_GNUG_PRE_2_8)"
          << be_idt_nl
          << "typedef " << node->local_name () << "_ptr _ptr_type;" 
          << be_nl
          << "typedef " << node->local_name () << "_var _var_type;" 
          << be_uidt_nl
          << "#endif /* ! __GNUC__ || g++ >= 2.8 */\n" << be_idt_nl;

          if (base_class_name_clash)
            {
              *os << "typedef " << clashing_parent->full_name ()
                  << " " << node->flat_name () << "_base_class_name;"
                  << be_nl << be_nl;
            }

          // generate the static _duplicate, _narrow, and _nil operations
      *os << "// the static operations" << be_nl
          << "static " << node->local_name () << "_ptr " << "_duplicate ("
          << node->local_name () << "_ptr obj);" << be_nl
          << "static " << node->local_name () << "_ptr "
          << "_narrow (" << be_idt << be_idt_nl
          << "CORBA::Object_ptr obj," << be_nl
          << "CORBA::Environment &ACE_TRY_ENV = " << be_idt_nl
          << "TAO_default_environment ()"
          << be_uidt << be_uidt_nl
          << ");" << be_uidt_nl;

      // There's no need for an _unchecked_narrow for locality 
      // constrained object.
      *os << "static " << node->local_name () << "_ptr "
          << "_unchecked_narrow (" << be_idt << be_idt_nl
          << "CORBA::Object_ptr obj," << be_nl
          << "CORBA::Environment &ACE_TRY_ENV = " << be_idt_nl
          << "TAO_default_environment ()"
          << be_uidt << be_uidt_nl
          << ");" << be_uidt_nl;

      // This method is defined in the header file to workaround old
      // g++ problems.
      *os << "static " << node->local_name () << "_ptr _nil (void)"
          << be_idt_nl << "{" << be_idt_nl
          << "return (" << node->local_name ()
          << "_ptr)0;" << be_uidt_nl
          << "}" << be_uidt << "\n\n";

      // No Any operator for local interfaces.
      if (! node->is_local ())
        {
          os->indent ();
          *os << "static void _tao_any_destructor (void*);\n\n";
        }

      // generate code for the interface definition by traversing thru the
      // elements of its scope. We depend on the front-end to have made sure
      // that only legal syntactic elements appear in our scope.

      if (this->visit_scope (node) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_interface_ch::"
                             "visit_interface - "
                             "codegen for scope failed\n"), -1);
        }
      // the _is_a method
      os->indent ();

      if (! node->is_local ())
        *os << "virtual CORBA::Boolean _is_a (" << be_idt << be_idt_nl
            << "const CORBA::Char *type_id, " << be_nl
            << "CORBA::Environment &ACE_TRY_ENV = " << be_idt_nl
            << "TAO_default_environment ()"
            << be_uidt << be_uidt_nl
            << ");" << be_uidt << be_nl;

      // the _tao_QueryInterface method
      *os << "virtual void *_tao_QueryInterface (ptr_arith_t type);"
          << be_nl << be_nl;

      // the _interface_repository_id method
      *os << "virtual const char* _interface_repository_id (void) const;\n"
          << be_uidt_nl;

      if (!node->is_local ())
        {
          // Add the Proxy Broker member variable.
          *os << "private:" << be_idt_nl
              << node->base_proxy_broker_name () << " *" 
              << "the" << node->base_proxy_broker_name ()
              << "_;" << be_nl <<  be_uidt_nl;
        }
      *os << "protected:" << be_idt_nl;
      if (!node->is_local ())
        {
          // Generate the "protected" constructor so that users cannot
          // instantiate us.

          *os << node->local_name () << " (int collocated = 0);" 
              << be_nl << be_nl;

          *os << "protected:" << be_idt_nl
              << "// This methods travese the inheritance tree and set the" 
              << be_nl
              << "// parents piece of the given class in the right mode" 
              << be_nl
              << "virtual void _tao_setup_collocation (int collocated);" 
              << be_nl << be_nl;
        }
      else
        *os << node->local_name () << " ();" << be_nl << be_nl;

      // Local interfaces don't support stub objects.
      if (! node->is_local ())
        {
          *os << node->local_name ()
              << " (" << be_idt << be_nl << "TAO_Stub *objref, " << be_nl
              << "CORBA::Boolean _tao_collocated = 0," << be_nl
              << "TAO_Abstract_ServantBase *servant = 0" <<  be_nl
              << ");" << be_uidt_nl << be_nl;

          // Friends declarations
          *os << "friend class " << node->remote_proxy_impl_name () << ";" 
              << be_nl
              << "friend class " << node->thru_poa_proxy_impl_name () << ";" 
              << be_nl
              << "friend class " << node->direct_proxy_impl_name () << ";" 
              << be_uidt_nl << be_nl;
        }
      // Protected destructor.
      *os << "virtual ~" << node->local_name () << " (void);" << be_uidt_nl;

      // private copy constructor and assignment operator. These are not
      // allowed, hence they are private.
      *os << "private:" << be_idt_nl;
      *os << node->local_name () << " (const " 
          << node->local_name () << " &);"
          << be_nl
          << "void operator= (const " << node->local_name () << " &);";

      // Generate the embedded RequestInfo classes per operation.
      // This is to be used by interceptors.
      be_visitor *visitor = 0;
      be_visitor_context ctx (*this->ctx_);

      // Interceptor related classes.
      ctx.state (TAO_CodeGen::TAO_INTERFACE_INTERCEPTORS_CH);
      visitor = tao_cg->make_visitor (&ctx);
      if (!visitor || (node->accept (visitor) == -1))
        {
          delete visitor;
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "be_visitor_interface_ch::"
                                 "visit_interface - "
                                 "codegen for interceptor classes failed\n"),
                                -1);
        }
      delete visitor;
      visitor = 0;

      ctx = *this->ctx_;

      *os << be_uidt_nl;
      *os << "};\n\n";

      // Don't support smart proxies for local interfaces.
      if (! node->is_local ())
        {
          // Smart Proxy related classes.
          ctx.state (TAO_CodeGen::TAO_INTERFACE_SMART_PROXY_CH);
          visitor = tao_cg->make_visitor (&ctx);
          if (!visitor || (node->accept (visitor) == -1))
            {
              delete visitor;
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "be_visitor_interface_ch::"
                                 "visit_interface - "
                                 "codegen for smart proxy classes failed\n"),
                                -1);
            }
          delete visitor;
          visitor = 0;
        }
      if (!node->is_local ())
        {
          // Proxy Implementation Declaration
          visitor = 0;
          ctx = *this->ctx_;
          ctx.state (TAO_CodeGen::TAO_INTERFACE_PROXY_IMPLS_CH);
          visitor = tao_cg->make_visitor (&ctx);

          if (!visitor || (node->accept (visitor) == -1))
            {
              delete visitor;
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "be_visitor_interface_ch::"
                                 "visit_interface - "
                                 "codegen for Proxy Broker classes failed\n"),
                                -1);
            }
          delete visitor;
        }

      if (!node->is_local ())
        {
          // Proxy Broker Declaration
          visitor = 0;
          ctx = *this->ctx_;
          ctx.state (TAO_CodeGen::TAO_INTERFACE_PROXY_BROKERS_CH);
          visitor = tao_cg->make_visitor (&ctx);

          if (!visitor || (node->accept (visitor) == -1))
            {
              delete visitor;
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "be_visitor_interface_ch::"
                                 "visit_interface - "
                                 "codegen for Proxy Broker classes failed\n"),
                                -1);
            }
          delete visitor;
        }

      os->gen_endif ();

      if (! node->is_local ())
        {
          // by using a visitor to declare and define the TypeCode, we
          // have the added advantage to conditionally not generate
          // any code. This will be based on the command line
          // options. This is still TO-DO be_visitor *visitor;
          visitor = 0;
          ctx.state (TAO_CodeGen::TAO_TYPECODE_DECL);
          visitor = tao_cg->make_visitor (&ctx);
          if (!visitor || (node->accept (visitor) == -1))
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "(%N:%l) be_visitor_interface_ch::"
                                 "visit_interface - "
                                 "TypeCode declaration failed\n"
                                 ), -1);
            }

        }

      node->cli_hdr_gen (I_TRUE);
    } // if !cli_hdr_gen

  return 0;
}
