// $id: any_op_cs.cpp,v 1.12 1998/10/30 19:46:55 levine Exp $

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    any_op_cs.cpp
//
// = DESCRIPTION
//    Visitor generating code for Any operators for Interface in the stubs
//    file.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

ACE_RCSID (be_visitor_interface, 
           any_op_cs, 
           "$Id$")

// ***************************************************************************
// Interface visitor for generating Any operator declarations in the client
// stubs file
// ***************************************************************************

be_visitor_interface_any_op_cs::be_visitor_interface_any_op_cs
(be_visitor_context *ctx)
  : be_visitor_interface (ctx)
{
}

be_visitor_interface_any_op_cs::~be_visitor_interface_any_op_cs (void)
{
}

int
be_visitor_interface_any_op_cs::visit_interface (be_interface *node)
{
  if (node->cli_stub_any_op_gen ()
      || node->imported ())
    {
      return 0;
    }

  TAO_OutStream *os = this->ctx_->stream ();

  *os << be_nl << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__ << be_nl << be_nl;

  if (!node->is_local ())
    {
      // Generate the Any <<= and >>= operator declarations
      // Any <<= and >>= operators.
      *os << "void" << be_nl
          << "operator<<= (" << be_idt << be_idt_nl
          << "CORBA::Any &_tao_any," << be_nl
          << node->full_name () << "_ptr _tao_elem" << be_uidt_nl
          << ")" << be_uidt_nl
          << "{" << be_idt_nl
          << "TAO_OutputCDR stream;" << be_nl
          << "if (stream << _tao_elem)" << be_idt_nl
          << "{" << be_idt_nl
          << "_tao_any._tao_replace (" << be_idt << be_idt_nl
          << node->tc_name () << ", " << be_nl
          << "TAO_ENCAP_BYTE_ORDER," << be_nl
          << "stream.begin ()," << be_nl
          << "1," << be_nl
          << node->full_name () << "::_duplicate (_tao_elem)," << be_nl
          << node->name () << "::_tao_any_destructor" << be_uidt_nl
          << ");" << be_uidt << be_uidt_nl
          << "}" << be_uidt << be_uidt_nl
          << "}" << be_nl << be_nl;

      *os << "CORBA::Boolean" << be_nl
          << "operator>>= (" << be_idt << be_idt_nl
          << "const CORBA::Any &_tao_any," << be_nl
          << node->full_name () << "_ptr &_tao_elem" << be_uidt_nl
          << ")" << be_uidt_nl
          << "{" << be_idt_nl
          << "ACE_TRY_NEW_ENV" << be_nl
          << "{" << be_idt_nl
          << "_tao_elem = " << node->full_name () << "::_nil ();" << be_nl
          << "CORBA::TypeCode_var type = _tao_any.type ();" << be_nl << be_nl
          << "CORBA::Boolean result =" << be_idt_nl
          << "type->equivalent (" << be_idt << be_idt_nl
          << node->tc_name () << be_nl
          << "ACE_ENV_ARG_PARAMETER" << be_uidt_nl
          << ");" << be_uidt << be_uidt_nl
          << "ACE_TRY_CHECK;" << be_nl << be_nl
          << "if (!result)" << be_idt_nl
          << "{" << be_idt_nl
          << "return 0; // not equivalent" << be_uidt_nl 
          << "}" << be_uidt_nl << be_nl
          << "TAO_InputCDR stream (" << be_idt << be_idt_nl
          << "_tao_any._tao_get_cdr ()," << be_nl
          << "_tao_any._tao_byte_order ()" << be_uidt_nl
          << ");" << be_uidt_nl << be_nl
          << "if (stream >> _tao_elem)" << be_idt_nl
          << "{" << be_idt_nl
          << "((CORBA::Any *)&_tao_any)->_tao_replace (" << be_idt << be_idt_nl
          << node->tc_name () << "," << be_nl
          << "1," << be_nl
          << "_tao_elem," << be_nl
          << node->name () << "::_tao_any_destructor" << be_uidt_nl
          << ");" << be_uidt_nl << be_nl
          << "return 1;" << be_uidt_nl
          << "}" << be_uidt << be_uidt_nl
          << "}" << be_nl
          << "ACE_CATCHANY" << be_nl
          << "{" << be_idt_nl
          << "_tao_elem = " << node->full_name () << "::_nil ();" << be_nl
          << "return 0;" << be_uidt_nl
          << "}" << be_nl
          << "ACE_ENDTRY;" << be_nl
          << "_tao_elem = " << node->full_name () << "::_nil ();" << be_nl
          << "return 0;" << be_uidt_nl
          << "}" << be_nl << be_nl;
    }

  *os << "#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)  || \\"
      << be_idt_nl
      << "  defined (ACE_HAS_GNU_REPO)" << be_nl;

  if (node->is_abstract ())
    {
      *os << "template class TAO_Abstract_Manager<";
    }
  else
    {
      *os << "template class TAO_Object_Manager<";
    }

  *os << node->full_name () << ","
      << node->full_name () << "_var>;" << be_uidt_nl
      << "#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)" << be_nl;

  if (node->is_abstract ())
    {
      *os << "# pragma instantiate TAO_Abstract_Manager<";
    }
  else
    {
      *os << "# pragma instantiate TAO_Object_Manager<";
    }

  *os << node->full_name () << ","
      << node->full_name () << "_var>" << be_uidt_nl
      << "#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */\n\n";

  // All we have to do is to visit the scope and generate code.
  if (!node->is_local ())
    {
      if (this->visit_scope (node) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_interface::visit_interface - "
                             "codegen for scope failed\n"), -1);
        }
    }

  node->cli_stub_any_op_gen (1);
  return 0;
}
