//
// $Id$
//

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
      << "// " << __FILE__ << ":" << __LINE__;

  if (node->is_defined ())
    {
      *os << be_nl << be_nl
          << "// Traits specializations for " << node->name () << ".";

      *os << be_nl << be_nl
          << node->name () << "_ptr" << be_nl
          << "TAO::Objref_Traits<" << node->name () << ">::duplicate ("
          << be_idt << be_idt_nl
          << node->name () << "_ptr p" << be_uidt_nl
          << ")" << be_uidt_nl
          << "{" << be_idt_nl
          << "return " << node->name () << "::_duplicate (p);" << be_uidt_nl
          << "}";

      *os << be_nl << be_nl
          << "void" << be_nl
          << "TAO::Objref_Traits<" << node->name () << ">::release ("
          << be_idt << be_idt_nl
          << node->name () << "_ptr p" << be_uidt_nl
          << ")" << be_uidt_nl
          << "{" << be_idt_nl
          << "::CORBA::release (p);" << be_uidt_nl
          << "}";

      *os << be_nl << be_nl
          << node->name () << "_ptr" << be_nl
          << "TAO::Objref_Traits<" << node->name () << ">::nil (void)"
          << be_nl
          << "{" << be_idt_nl
          << "return " << node->name () << "::_nil ();" << be_uidt_nl
          << "}";

      *os << be_nl << be_nl
          << "::CORBA::Boolean" << be_nl
          << "TAO::Objref_Traits<" << node->name () << ">::marshal ("
          << be_idt << be_idt_nl
          << "const " << node->name () << "_ptr p," << be_nl
          << "TAO_OutputCDR & cdr" << be_uidt_nl
          << ")" << be_uidt_nl
          << "{" << be_idt_nl
          << "return cdr << p;" << be_uidt_nl
          << "}";
    }

  if (node->has_mixed_parentage ())
    {
      *os << be_nl << be_nl
          << "void" << be_nl
          << "CORBA::release ("
          << node->name ()
          << "_ptr p)" << be_nl
          << "{" << be_idt_nl
          << "::CORBA::AbstractBase_ptr abs = p;" << be_nl
          << "::CORBA::release (abs);" << be_uidt_nl
          << "}";

      *os << be_nl << be_nl
          << "::CORBA::Boolean" << be_nl
          << "CORBA::is_nil ("
          << node->name ()
          << "_ptr p)" << be_nl
          << "{" << be_idt_nl
          << "::CORBA::Object_ptr obj = p;" << be_nl
          << "return ::CORBA::is_nil (obj);" << be_uidt_nl
          << "}";
    }

  if (be_global->gen_direct_collocation() || be_global->gen_thru_poa_collocation ())
    {
      // Generate the proxy broker factory function pointer definition.
      *os << be_nl << be_nl
          << "// Function pointer for collocation factory initialization."
          << be_nl
          << "TAO::Collocation_Proxy_Broker * " << be_nl
          << "(*" << node->flat_client_enclosing_scope ()
          << node->base_proxy_broker_name ()
          << "_Factory_function_pointer) ("
          << be_idt << be_idt_nl
          << "::CORBA::Object_ptr obj" << be_uidt_nl
          << ") = 0;" << be_uidt;
    }

   // Generate the destructor and default constructor.
  *os << be_nl << be_nl
      << node->name () << "::~" << node->local_name ()
      << " (void)" << be_nl;
  *os << "{}" << be_nl << be_nl;

  *os << node->name () << "::" << node->local_name ()
      << " (void)" << be_nl
      << "{" << be_idt_nl;

  if (be_global->gen_direct_collocation() || be_global->gen_thru_poa_collocation ())
    {
      *os << "this->" << node->flat_name ()
          << "_setup_collocation ();" << be_uidt_nl;
    }

  *os << be_uidt << "}" << be_nl << be_nl;

  if (be_global->gen_direct_collocation() || be_global->gen_thru_poa_collocation ())
    {
      // Collocation setup method.
      *os << "void" << be_nl
          << node->name () << "::" << node->flat_name ()
          << "_setup_collocation (void)" << be_nl
          << "{" << be_idt_nl
          << "if ("<< "::" << node->flat_client_enclosing_scope ()
          << node->base_proxy_broker_name ()
          << "_Factory_function_pointer)" << be_idt_nl
          << "this->the" << node->base_proxy_broker_name ()
          << "_ =" << be_idt_nl
          << "::" << node->flat_client_enclosing_scope ()
          << node->base_proxy_broker_name ()
          << "_Factory_function_pointer (this);"
          << be_uidt << be_uidt_nl;

      AST_Component *base = node->base_component ();

      if (base != 0)
        {
          *os << be_nl
              << "this->" << base->flat_name ()
              << "_setup_collocation" << " ();";
        }

      *os << be_uidt_nl << "}" << be_nl << be_nl;
    }

  if (be_global->any_support ())
    {
      *os << "void " << be_nl
          << node->name ()
          << "::_tao_any_destructor (void *_tao_void_pointer)" << be_nl
          << "{" << be_idt_nl
          << node->local_name () << " *_tao_tmp_pointer = static_cast<"
          << node->local_name () << " *> (_tao_void_pointer);" << be_nl
          << "::CORBA::release (_tao_tmp_pointer);" << be_uidt_nl
          << "}" << be_nl << be_nl;
    }

  if (node->has_mixed_parentage ())
    {
      *os << "void" << be_nl
          << node->name () << "::_add_ref (void)" << be_nl
          << "{" << be_idt_nl
          << "this->::CORBA::Object::_add_ref ();"
          << be_uidt_nl
          << "}" << be_nl << be_nl;
    }

  // The _narrow method.
  *os << node->full_name () << "_ptr" << be_nl << node->full_name ()
      << "::_narrow (::CORBA::Object_ptr _tao_objref)" << be_nl
      << "{" << be_idt_nl;

  *os << "return" << be_idt_nl
      << "TAO::Narrow_Utils<"
      << node->local_name () << ">::narrow (";

  *os << be_idt << be_idt_nl
      << "_tao_objref," << be_nl
      << "\"" << node->repoID () << "\"," << be_nl;

  if (be_global->gen_direct_collocation() || be_global->gen_thru_poa_collocation ())
    {
      *os << node->flat_client_enclosing_scope ()
          << node->base_proxy_broker_name ()
          << "_Factory_function_pointer" << be_uidt_nl;
    }
  else
    {
      *os << "0" << be_uidt_nl;
    }

  *os << ");" << be_uidt << be_uidt << be_uidt_nl
      << "}" << be_nl << be_nl;

  // The _duplicate method
  *os << node->full_name () << "_ptr" << be_nl
      << node->full_name () << "::_duplicate ("
      << bt->nested_type_name (this->ctx_->scope ())
      << "_ptr obj)" << be_nl
      << "{" << be_idt_nl
      << "if (! ::CORBA::is_nil (obj))" << be_idt_nl
      << "{" << be_idt_nl
      << "obj->_add_ref ();" << be_uidt_nl
      << "}" << be_uidt_nl << be_nl
      << "return obj;" << be_uidt_nl
      << "}" << be_nl << be_nl;

  // The is_a method.
  *os << "::CORBA::Boolean" << be_nl
      << node->full_name () << "::_is_a (const char *value)" << be_nl
      << "{" << be_idt_nl
      << "if (" << be_idt << be_idt_nl;

  AST_Component *ancestor = node;

  while (ancestor != 0)
    {
      *os << "!ACE_OS::strcmp (" << be_idt << be_idt_nl
          << "value," << be_nl
          << "\"" << ancestor->repoID () << "\"" << be_uidt_nl
          << ") ||" << be_uidt_nl;

      ancestor = ancestor->base_component ();
    }

  *os << "!ACE_OS::strcmp (" << be_idt << be_idt_nl
      << "value," << be_nl
      << "\"IDL:omg.org/Components/CCMObject:1.0\"" << be_uidt_nl
      << ")" << be_uidt << be_uidt_nl
      << " )" << be_nl
      << "{" << be_idt_nl
      << "return true; // success using local knowledge" << be_uidt_nl
      << "}" << be_uidt_nl
      << "else" << be_idt_nl
      << "{" << be_idt_nl
      << "return this->::CORBA::Object::_is_a (value);" << be_uidt_nl
      << "}" << be_uidt << be_uidt_nl
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
      << "::CORBA::Boolean" << be_nl
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

  be_visitor_context ctx (*this->ctx_);

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
      //       ctx.sub_state (TAO_CodeGen::TAO_TC_DEFN_TYPECODE);
      TAO::be_visitor_objref_typecode tc_visitor (&ctx);

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
