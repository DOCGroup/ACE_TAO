// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    interface_ci.cpp
//
// = DESCRIPTION
//    Visitor generating code for Interfaces in the client inline file
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

ACE_RCSID (be_visitor_interface,
           interface_ci,
           "$Id$")

// **************************************************
// Interface visitor for client inline
// **************************************************

be_visitor_interface_ci::be_visitor_interface_ci (be_visitor_context *ctx)
  : be_visitor_interface (ctx)
{
}

be_visitor_interface_ci::~be_visitor_interface_ci (void)
{
}

int
be_visitor_interface_ci::visit_interface (be_interface *node)
{
  if (node->imported ())
    {
      return 0;
    }

  // Generate inline methods for elements of our scope.
  // This will always be done here for any non-imported node,
  // whether the _var and _out code has been generated already
  // at a forward declaration or not.
  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_interface_ci::"
                         "visit_interface - "
                         "codegen for scope failed\n"), -1);
    }

  // A forward declared interface may have set this flag.
  if (node->cli_inline_gen ())
    {
      return 0;
    }

  TAO_OutStream *os = this->ctx_->stream ();

  *os << be_nl << be_nl << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__;

  os->gen_ifdef_macro (node->flat_name (), "");

  if (node->is_abstract ())
    {
      *os << be_nl << be_nl
          << "ACE_INLINE" << be_nl
          << node->name () << "::" << node->local_name ()
          << " (void)" << be_nl
          << "{}" << be_nl << be_nl;

      *os << "ACE_INLINE" << be_nl
          << node->name () << "::" << node->local_name ()
          << " (const " << node->local_name () << " &rhs)" << be_idt_nl
          << ": ACE_NESTED_CLASS (CORBA, AbstractBase) (rhs)" << be_uidt_nl
          << "{}" << be_nl << be_nl;

      *os << "ACE_INLINE" << be_nl
          << node->name () << "::" << node->local_name ()
          << " (" << be_idt << be_idt_nl
          << "TAO_Stub *objref," << be_nl
          << "CORBA::Boolean _tao_collocated," << be_nl
          << "TAO_Abstract_ServantBase *servant," << be_nl
          << "TAO_ORB_Core *orb_core" << be_uidt_nl
          << ")" << be_nl
          << ": ACE_NESTED_CLASS (CORBA, AbstractBase) (" 
          << be_idt << be_idt << be_idt_nl
          << "objref," << be_nl
          << "_tao_collocated," << be_nl
          << "servant" << be_uidt_nl
          << ")" << be_uidt << be_uidt << be_uidt_nl
          << "{}";
    }
  else
    {
      // Generate the constructor from stub and servant.
      node->gen_stub_ctor (os);
    }

  if (be_global->any_support ())
    {
      if (! node->is_abstract ())
        {
          *os << be_nl << be_nl
              << "template<>" << be_nl
              << "ACE_INLINE" << be_nl
              << "CORBA::Boolean" << be_nl
              << "TAO::Any_Impl_T<" << node->name () << ">::to_object ("
              << be_idt <<  be_idt_nl
              << "CORBA::Object_ptr &_tao_elem" << be_uidt_nl
              << ") const" << be_uidt_nl
              << "{" << be_idt_nl
              << "_tao_elem = CORBA::Object::_duplicate (this->value_);" << be_nl
              << "return 1;" << be_uidt_nl
              << "}";
        }

      if (node->is_abstract () || node->has_mixed_parentage ())
        {
          *os << be_nl << be_nl
              << "template<>" << be_nl
              << "ACE_INLINE" << be_nl
              << "CORBA::Boolean" << be_nl
              << "TAO::Any_Impl_T<" << node->name ()
              << ">::to_abstract_base ("
              << be_idt <<  be_idt_nl
              << "CORBA::AbstractBase_ptr &_tao_elem" << be_uidt_nl
              << ") const" << be_uidt_nl
              << "{" << be_idt_nl
              << "_tao_elem = CORBA::AbstractBase::_duplicate (this->value_);"
              << be_nl
              << "return 1;" << be_uidt_nl
              << "}";
        }

      // Since we don't generate CDR stream operators for types that
      // explicitly contain a local interface (at some level), we
      // must override these Any template class methods to avoid
      // calling the non-existent operators. The zero return value
      // will eventually cause CORBA::MARSHAL to be raised if this
      // type is inserted into an Any and then marshaled.
      if (node->is_local ())
        {
          *os << be_nl << be_nl
              << "template<>" << be_nl
              << "ACE_INLINE" << be_nl
              << "CORBA::Boolean" << be_nl
              << "TAO::Any_Impl_T<" << node->name ()
              << ">::marshal_value (TAO_OutputCDR &)" << be_nl
              << "{" << be_idt_nl
              << "return 0;" << be_uidt_nl
              << "}";

          *os << be_nl << be_nl
              << "template<>" << be_nl
              << "ACE_INLINE" << be_nl
              << "CORBA::Boolean" << be_nl
              << "TAO::Any_Impl_T<" << node->name ()
              << ">::demarshal_value (TAO_InputCDR &)" << be_nl
              << "{" << be_idt_nl
              << "return 0;" << be_uidt_nl
              << "}";
        }
    }

  if (!node->is_local () && !node->is_abstract ())
    {
      *os << be_nl << be_nl
          << "ACE_INLINE" << be_nl;
      *os << node->name () << "::"
          << node->local_name () << " ("
          << be_idt << be_idt_nl
          << "IOP::IOR *ior," << be_nl
          << "TAO_ORB_Core *oc" << be_uidt_nl
          << ")" << be_nl;
      *os << ": ACE_NESTED_CLASS (CORBA, Object) (ior, oc)," << be_idt_nl
          << "the"<< node->base_proxy_broker_name () << "_ (0)" << be_uidt_nl;

      *os << be_uidt_nl
          << "{" << be_nl
          << "}" ;
    }

  os->gen_endif ();
  node->cli_inline_gen (I_TRUE);
  return 0;
}
