// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    component_cs.cpp
//
// = DESCRIPTION
//    Visitor generating code for Components in the client stub.
//
// = AUTHOR
//    Jeff Parsons
//
// ============================================================================

ACE_RCSID (be_visitor_component,
           component_cs,
           "$Id$")

// ******************************************************
// Component visitor for client stub
// ******************************************************

be_visitor_component_cs::be_visitor_component_cs (be_visitor_context *ctx)
  : be_visitor_component (ctx)
{
}

be_visitor_component_cs::~be_visitor_component_cs (void)
{
}

int
be_visitor_component_cs::visit_component (be_component *node)
{
  if (node->cli_stub_gen () || node->imported ())
    {
      return 0;
    }

  be_type *bt;

  // Set the right type.
  if (this->ctx_->alias ())
    {
      bt = this->ctx_->alias ();
    }
  else
    {
      bt = node;
    }

  TAO_OutStream *os = this->ctx_->stream ();

  *os << be_nl << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__ << be_nl << be_nl;

  // Initialize the static narrrowing helper variable.
  *os << "int " << node->full_name () << "::_tao_class_id = 0;"
      << be_nl << be_nl;

  AST_Decl *parent = ScopeAsDecl (node->defined_in ());

  // Helper functions generated in case this interface was
  // forward declared in some other IDL file and not defined there.
  *os << node->full_name () << "_ptr" << be_nl;

  if (parent != 0 && parent->node_type () != AST_Decl::NT_root)
    {
      *os << parent->name () << "::";
    }

  *os << "tao_" << node->local_name () << "_life::"
      << "tao_duplicate (" << be_idt << be_idt_nl
      << node->full_name () << "_ptr p" << be_uidt_nl
      << ")" << be_uidt_nl
      << "{" << be_idt_nl
      << "return " << node->full_name ()
      << "::_duplicate (p);" << be_uidt_nl
      << "}" << be_nl << be_nl;

  *os << "void" << be_nl;

  if (parent != 0 && parent->node_type () != AST_Decl::NT_root)
    {
      *os << parent->name () << "::";
    }

  *os << "tao_" << node->local_name () << "_life::"
      << "tao_release (" << be_idt << be_idt_nl
      << node->full_name () << "_ptr p" << be_uidt_nl
      << ")" << be_uidt_nl
      << "{" << be_idt_nl
      << "CORBA::release (p);" << be_uidt_nl
      << "}" << be_nl << be_nl;

  *os << node->full_name () <<  "_ptr" << be_nl;

  if (parent != 0 && parent->node_type () != AST_Decl::NT_root)
    {
      *os << parent->name () << "::";
    }

  *os << "tao_" << node->local_name () << "_life::"
      << "tao_nil (" << be_idt << be_idt_nl
      << "void" << be_uidt_nl
      << ")" << be_uidt_nl
      << "{" << be_idt_nl
      << "return " << node->full_name ()
      << "::_nil ();" << be_uidt_nl
      << "}" << be_nl << be_nl;

  *os << "CORBA::Boolean" << be_nl;

  if (parent != 0 && parent->node_type () != AST_Decl::NT_root)
    {
      *os << parent->name () << "::";
    }

  *os << "tao_" << node->local_name () << "_life::"
      << "tao_marshal (" << be_idt << be_idt_nl
      << node->name () << "_ptr p," << be_nl
      << "TAO_OutputCDR &cdr" << be_uidt_nl
      << ")" << be_uidt_nl
      << "{" << be_idt_nl
      << "return p->marshal (cdr);" << be_uidt_nl
      << "}" << be_nl << be_nl;

  *os << node->full_name () << "_ptr" << be_nl;

  if (parent != 0 && parent->node_type () != AST_Decl::NT_root)
    {
      *os << parent->name () << "::";
    }

  *os << "tao_" << node->local_name () << "_cast::"
      << "tao_narrow (" << be_idt << be_idt_nl
      << "CORBA::Object_ptr p" << be_nl
      << "ACE_ENV_ARG_DECL" << be_uidt_nl
      << ")" << be_uidt_nl
      << "{" << be_idt_nl
      << "return " << node->full_name ()
      << "::_narrow (p ACE_ENV_ARG_PARAMETER);"
      << be_uidt_nl
      << "}" << be_nl << be_nl;

  *os << "CORBA::Object_ptr" << be_nl;

  if (parent != 0 && parent->node_type () != AST_Decl::NT_root)
    {
      *os << parent->name () << "::";
    }

  *os << "tao_" << node->local_name () << "_cast::"
      << "tao_upcast (" << be_idt << be_idt_nl
      << "void *src" << be_uidt_nl
      << ")" << be_uidt_nl
      << "{" << be_idt_nl
      << node->full_name () << " **tmp =" << be_idt_nl
      << "ACE_static_cast (" << node->full_name ()
      << " **, src);" << be_uidt_nl
      << "return *tmp;" << be_uidt_nl
      << "}" << be_nl << be_nl;

  *os << "CORBA::Boolean" << be_nl
      << "tao_" << node->flat_name () << "_marshal (" << be_idt << be_idt_nl
      << node->name () << "_ptr p," << be_nl
      << "TAO_OutputCDR &strm" << be_uidt_nl
      << ")" << be_uidt_nl
      << "{" << be_idt_nl
      << "return p->marshal (strm);" << be_uidt_nl
      << "}";


  be_visitor_context ctx (*this->ctx_);

  // Interceptor classes.  The interceptors helper classes must be
  // defined before the interface operations because they are used in
  // the implementation of said operations.

  ctx.state (TAO_CodeGen::TAO_INTERFACE_INTERCEPTORS_CS);
  be_visitor_interface_interceptors_cs ii_visitor (&ctx);

  if (node->accept (&ii_visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_component_cs::"
                         "visit_component - "
                         "codegen for interceptors classes failed\n"),
                        -1);
    }

  ctx = *this->ctx_;
  ctx.state (TAO_CodeGen::TAO_INTERFACE_REMOTE_PROXY_IMPL_CS);
  be_visitor_interface_remote_proxy_impl_cs irpi_visitor (&ctx);

  if (node->accept (&irpi_visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_component_cs::"
                         "visit_component - "
                         "codegen for Base Proxy Broker class failed\n"),
                        -1);
    }

  ctx = *this->ctx_;
  be_visitor_interface_remote_proxy_broker_cs irpb_visitor (&ctx);

  if (node->accept (&irpb_visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_component_cs::"
                         "visit_component - "
                         "codegen for Base Proxy Broker class failed\n"),
                        -1);
    }

  *os << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__ << be_nl << be_nl;

   // Generate the destructor and default constructor.
  *os << node->name () << "::~" << node->local_name ()
      << " (void)" << be_nl;
  *os << "{}" << be_nl << be_nl;

  *os << node->name () << "::" << node->local_name ()
      << " (int collocated)" << be_nl
      << "{" << be_idt_nl
      << "this->" << node->flat_name ()
      << "_setup_collocation (collocated);" << be_uidt_nl
      << be_uidt << "}" << be_nl << be_nl;

  // Collocation setup method.
  *os << "void" << be_nl
      << node->name () << "::" << node->flat_name ()
      << "_setup_collocation (int collocated)" << be_nl
      << "{" << be_idt_nl
      << "if (collocated)" << be_idt_nl
      << "this->the" << node->base_proxy_broker_name ()
      << "_ =" << be_idt_nl
      << "::" << node->flat_client_enclosing_scope ()
      << node->base_proxy_broker_name ()
      << "_Factory_function_pointer (this);"
      << be_uidt << be_uidt_nl
      << "else" << be_idt_nl
      << "this->the" << node->base_proxy_broker_name ()
      << "_ =" << be_idt_nl
      << "::" << node->full_remote_proxy_broker_name ()
      << "::the" << node->remote_proxy_broker_name ()
      << " ();" << be_uidt << be_uidt;

  AST_Component *base = node->base_component ();

  if (base != 0)
    {
      *os << be_nl
          << "this->" << base->flat_name ()
          << "_setup_collocation" << " (collocated);";
    }

  *os << be_uidt_nl << "}" << be_nl << be_nl;

  if (be_global->any_support ())
    {
      *os << "void " << be_nl
          << node->name ()
          << "::_tao_any_destructor (void *_tao_void_pointer)" << be_nl
          << "{" << be_idt_nl
          << node->local_name () << " *tmp = ACE_static_cast ("
          << node->local_name () << " *, _tao_void_pointer);" << be_nl
          << "CORBA::release (tmp);" << be_uidt_nl
          << "}" << be_nl << be_nl;
    }

  // The _narrow method.
  *os << node->full_name () << "_ptr" << be_nl << node->full_name ()
      << "::_narrow (" << be_idt << be_idt_nl
      << "CORBA::Object_ptr obj" << be_nl
      << "ACE_ENV_ARG_DECL" << be_uidt_nl
      << ")" << be_uidt_nl
      << "{" << be_idt_nl
      << "if (CORBA::is_nil (obj))" << be_idt_nl
      << "{" << be_idt_nl
      << "return " << bt->nested_type_name (this->ctx_->scope ())
      << "::_nil ();" << be_uidt_nl
      << "}" << be_uidt_nl << be_nl
      << "CORBA::Boolean is_a =" << be_idt_nl
      << "obj->_is_a (" << be_idt << be_idt_nl
      << "\"" << node->repoID () << "\"" << be_nl
      << "ACE_ENV_ARG_PARAMETER" << be_uidt_nl
      << ");" << be_uidt << be_uidt_nl
      << "ACE_CHECK_RETURN (" << bt->nested_type_name (this->ctx_->scope ())
      << "::_nil ());" << be_nl << be_nl
      << "if (is_a == 0)" << be_idt_nl
      << "{" << be_idt_nl
      << "return " << bt->nested_type_name (this->ctx_->scope ())
      << "::_nil ();" << be_uidt_nl
      << "}" << be_uidt_nl << be_nl
      << "return " << bt->nested_type_name (this->ctx_->scope ())
      << "::_unchecked_narrow (obj ACE_ENV_ARG_PARAMETER);" << be_uidt_nl
      << "}" << be_nl << be_nl;

  this->gen_unchecked_narrow (node,
                              bt,
                              os);

  // The _duplicate method
  *os << node->full_name () << "_ptr" << be_nl
      << node->full_name () << "::_duplicate ("
      << bt->nested_type_name (this->ctx_->scope ())
      << "_ptr obj)" << be_nl
      << "{" << be_idt_nl
      << "if (! CORBA::is_nil (obj))" << be_idt_nl
      << "{" << be_idt_nl
      << "obj->_add_ref ();" << be_uidt_nl
      << "}" << be_uidt_nl << be_nl
      << "return obj;" << be_uidt_nl
      << "}" << be_nl << be_nl;

  // The is_a method.
  *os << "CORBA::Boolean" << be_nl
      << node->full_name () << "::_is_a (" << be_idt << be_idt_nl
      << "const char *value" << be_nl
      << "ACE_ENV_ARG_DECL" << be_uidt_nl
      << ")" << be_uidt_nl
      << "{" << be_idt_nl
      << "if (" << be_idt << be_idt_nl;

  AST_Component *ancestor = node;

  while (ancestor != 0)
    {
      *os << "!ACE_OS::strcmp (" << be_idt << be_idt_nl
          << "(char *)value," << be_nl
          << "\"" << ancestor->repoID () << "\"" << be_uidt_nl
          << ") ||" << be_uidt_nl;

      ancestor = ancestor->base_component ();
    }

  *os << "!ACE_OS::strcmp (" << be_idt << be_idt_nl
      << "(char *)value," << be_nl
      << "\"IDL:omg.org/Components/CCMObject:1.0\"" << be_uidt_nl
      << ")" << be_uidt << be_uidt_nl
      << " )" << be_nl
      << "{" << be_idt_nl
      << "return 1; // success using local knowledge" << be_uidt_nl
      << "}" << be_uidt_nl
      << "else" << be_idt_nl
      << "{" << be_idt_nl
      << "return this->Object::_is_a (" << be_idt << be_idt_nl
      << "value" << be_nl
      << "ACE_ENV_ARG_PARAMETER" << be_uidt_nl
      << ");" << be_uidt << be_uidt_nl
      << "}" << be_uidt << be_uidt_nl
      << "}" << be_nl << be_nl;

  // Generating _tao_QueryInterface method.
  *os << "void *" << node->full_name ()
      << "::_tao_QueryInterface (ptr_arith_t type)" << be_nl
      << "{" << be_idt_nl
      << "void *retv = 0;" << be_nl << be_nl
      << "if ";

  if (node->traverse_inheritance_graph (
          be_interface::queryinterface_helper,
          os
        ) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_interface_cs::"
                         "visit_interface - "
                         "_tao_QueryInterface method codegen failed\n"),
                        -1);
    }

  *os << "(type == ACE_reinterpret_cast ("
      << be_idt << be_idt << be_idt << be_idt << be_idt << be_idt_nl
      << " ptr_arith_t," << be_nl;

  if (node->is_abstract ())
    {
      *os << " &CORBA::AbstractBase";
    }
  else
    {
      *os << " &CORBA::Object";
    }

  *os << "::_tao_class_id)" << be_uidt_nl
      << " )" << be_uidt << be_uidt << be_uidt << be_uidt_nl
      << "{" << be_idt_nl
      << "retv =" << be_idt_nl
      << "ACE_reinterpret_cast (" << be_idt << be_idt_nl
      << "void *," << be_nl
      << "ACE_static_cast (";

  if (node->is_abstract ())
    {
      *os << "CORBA::AbstractBase_ptr";
    }
  else
    {
      *os << "CORBA::Object_ptr";
    }

  *os << ", this)" << be_uidt_nl
      << ");" << be_uidt << be_uidt << be_uidt_nl
      << "}" << be_uidt_nl << be_nl;

  *os << "if (retv != 0)" << be_idt_nl
      << "{" << be_idt_nl
      << "this->_add_ref ();" << be_uidt_nl
      << "}" << be_uidt_nl << be_nl
      << "return retv;" << be_uidt_nl
      << "}" << be_nl << be_nl;

  *os << "const char*" << be_nl
      << node->full_name ()
      << "::_interface_repository_id (void) const"
      << be_nl
      << "{" << be_idt_nl
      << "return \"" << node->repoID ()
      << "\";" << be_uidt_nl
      << "}";

  *os << be_nl << be_nl
      << "CORBA::Boolean" << be_nl
      << node->name () << "::marshal (TAO_OutputCDR &cdr)" << be_nl
      << "{" << be_idt_nl
      << "return (cdr << this);" << be_uidt_nl
      << "}";

  // Generate code for the elements of the component.
  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_component_cs::"
                         "visit_component - "
                         "codegen for scope failed\n"),
                        -1);
    }

  // Smart Proxy classes.
  if (be_global->gen_smart_proxies ())
    {
      ctx.state (TAO_CodeGen::TAO_INTERFACE_SMART_PROXY_CS);
      be_visitor_interface_smart_proxy_cs isp_visitor (&ctx);

      if (node->accept (&isp_visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "be_visitor_component_cs::"
                             "visit_component - "
                             "codegen for smart proxy classes failed\n"),
                            -1);
        }
    }

  if (be_global->tc_support ())
    {
      ctx.sub_state (TAO_CodeGen::TAO_TC_DEFN_TYPECODE);
      be_visitor_typecode_defn tc_visitor (&ctx);

      if (node->accept (&tc_visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_component_cs::"
                             "visit_component - "
                             "TypeCode definition failed\n"),
                            -1);
        }
    }

  return 0;
}

void
be_visitor_component_cs::gen_unchecked_narrow (be_component *node,
                                               be_type *bt,
                                               TAO_OutStream *os)
{
  *os << node->full_name () << "_ptr "  << be_nl
      << node->full_name () << "::_unchecked_narrow ("
      << be_idt << be_idt_nl
      << "CORBA::Object_ptr obj" << be_nl
      << "ACE_ENV_ARG_DECL_NOT_USED" << be_uidt_nl
      << ")" << be_uidt_nl
      << "{" << be_idt_nl
      << "if (CORBA::is_nil (obj))" << be_idt_nl
      << "{" << be_idt_nl
      << "return " << bt->nested_type_name (this->ctx_->scope ())
      << "::_nil ();" << be_uidt_nl
      << "}" << be_uidt_nl << be_nl;

  // Declare the default proxy.
  *os << bt->nested_type_name (this->ctx_->scope ())
      << "_ptr default_proxy = "
      << bt->nested_type_name (this->ctx_->scope ())
      <<"::_nil ();" << be_nl << be_nl;

  // Code for lzay evaluation..
  *os << "// Code for lazily evaluated IOR's" << be_nl;

  *os << "if (!obj->is_evaluated ())" << be_idt_nl
      << "{" << be_idt_nl
      << "ACE_NEW_RETURN (" << be_idt << be_idt_nl
      << "default_proxy," << be_nl
      << "::" <<  bt->name ()
      << " (" << be_idt << be_idt_nl
      << "obj->steal_ior ()," << be_nl
      << "obj->orb_core ()" << be_uidt_nl << ")," << be_uidt_nl
      << bt->nested_type_name (this->ctx_->scope ())
      << "::_nil ()" << be_uidt_nl << ");" << be_uidt_nl << be_nl
      << "return default_proxy;" << be_uidt_nl
      << "}" << be_uidt_nl << be_nl;

  *os << "TAO_Stub* stub = obj->_stubobj ();" << be_nl << be_nl
      << "if (stub != 0)" << be_idt_nl
      << "{" << be_idt_nl
      << "stub->_incr_refcnt ();" << be_uidt_nl
      << "}" << be_uidt_nl << be_nl;

  // If the policy dictates that the proxy be collocated, use the
  // function to create one.
  *os << "if (" << be_idt << be_idt_nl
      << "!CORBA::is_nil (stub->servant_orb_var ().ptr ()) &&" << be_nl
      << "stub->servant_orb_var ()->orb_core ()->optimize_collocation_objects () &&"
      << be_nl
      << "obj->_is_collocated () &&" << be_nl
      << node->flat_client_enclosing_scope () << node->base_proxy_broker_name ()
      << "_Factory_function_pointer != 0" << be_uidt_nl << ")"
      << be_nl << "{"
      << be_idt_nl
      << "ACE_NEW_RETURN (" << be_idt << be_idt_nl
      << "default_proxy," << be_nl
      << "::" <<  bt->name ()
      << " (" << be_idt << be_idt_nl
      << "stub," << be_nl
      << "1," << be_nl
      << "obj->_servant ()" << be_uidt_nl << ")," << be_uidt_nl
      <<  bt->nested_type_name (this->ctx_->scope ())
      << "::_nil ()" << be_uidt_nl << ");"
      << be_uidt << be_uidt_nl
      << "}" << be_uidt_nl << be_nl;

  // The default proxy will either be returned else be transformed to
  // a smart one!
  *os << "if (CORBA::is_nil (default_proxy))" << be_idt_nl
      << "{" << be_idt_nl
      << "ACE_NEW_RETURN (" << be_idt << be_idt_nl
      << "default_proxy," << be_nl
      << "::" << bt->name () << " (" << be_idt << be_idt_nl
      << "stub," << be_nl
      << "0," << be_nl
      << "obj->_servant ()" << be_uidt_nl
      << ")," << be_uidt_nl
      << bt->nested_type_name (this->ctx_->scope ())
      << "::_nil ()" << be_uidt_nl
      << ");" << be_uidt << be_uidt_nl
      << "}" << be_uidt_nl << be_nl;

  if (be_global->gen_smart_proxies ())
    {
      *os << "return TAO_" << node->flat_name ()
          << "_PROXY_FACTORY_ADAPTER::instance ()->create_proxy (default_proxy);"
          << be_uidt_nl;
    }
  else
    {
      *os << "return default_proxy;" << be_uidt_nl;
    }

  *os << "}" << be_nl << be_nl;
}
