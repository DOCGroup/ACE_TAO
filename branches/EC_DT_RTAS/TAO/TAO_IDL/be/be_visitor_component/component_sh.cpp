//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    component_sh.cpp
//
// = DESCRIPTION
//    Visitor generating code for Components in the server header.
//
// = AUTHOR
//    Jeff Parsons
//
// ============================================================================

ACE_RCSID (be_visitor_component,
           component_sh,
           "$Id$")

// ******************************************************
// Component visitor for server header
// ******************************************************

be_visitor_component_sh::be_visitor_component_sh (be_visitor_context *ctx)
  : be_visitor_component (ctx)
{
}

be_visitor_component_sh::~be_visitor_component_sh (void)
{
}

int
be_visitor_component_sh::visit_component (be_component *node)
{
  if (node->srv_hdr_gen () 
      || node->imported () 
      || node->is_abstract ())
    {
      return 0;
    }

   if (be_global->gen_amh_classes ())
    {
      be_visitor_amh_interface_sh amh_intf (this->ctx_);

      if (amh_intf.visit_interface (node) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "be_visitor_component_sh::"
                             "visit_component - "
                             "codegen for AMH classes failed\n"),
                            -1);
        }
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

  AST_Component *base = node->base_component ();

  if (base != 0)
    {
      *os << "public virtual POA_" << base->name ();
    }
  else
    {
      *os << "public virtual POA_Components::CCMObject";
    }

  long nsupports = node->n_inherits ();
  AST_Interface **supports = node->supports ();
  AST_Interface *supported = 0;

  for (long i = 0; i < nsupports; ++i)
    {
      supported = supports[i];

      if (supported->is_abstract ())
        {
          continue;
        }

      *os << "," << be_nl;
      *os << "public virtual POA_" << supported->name ();
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

  // _this
  *os << "::" << node->full_name () << " *_this (" << be_idt << be_idt_nl
      << "ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS" << be_uidt_nl
      << ");" << be_uidt_nl << be_nl;

  // _interface_repository_id
  *os << "virtual const char* _interface_repository_id "
      << "(void) const;";

  // Generate code for elements in the scope (e.g., operations).
  if (this->visit_scope (node) ==  -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_component_sh::"
                         "visit_component - "
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
                         "be_visitor_component_sh::"
                         "visit_component - "
                         "inheritance graph traversal failed\n"),
                        -1);
    }

  *os << be_uidt_nl << "};" << be_nl << be_nl;

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
              "be_visitor_component_sh::"
              "visit_component - "
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
                             "be_visitor_component_sh::"
                             "visit_component - "
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
              "be_visitor_component_sh::"
              "visit_component - "
              "codegen for thru_poa_collocated class failed\n"
            ),
            -1
          );
        }
    }

  return 0;
}

int
be_visitor_component_sh::generate_amh_classes (be_component *node)
{
   if (be_global->gen_amh_classes ())
    {
      be_visitor_amh_interface_sh amh_intf (this->ctx_);
      return amh_intf.visit_interface (node);
    }

  return 0;
}
