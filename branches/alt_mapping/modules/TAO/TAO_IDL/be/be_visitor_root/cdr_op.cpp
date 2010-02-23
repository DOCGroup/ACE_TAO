//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    cdr_op.cpp
//
// = DESCRIPTION
//    Visitor generating code for the CDR operators for types defined
//    in Root's scope.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

ACE_RCSID (be_visitor_root, 
           cdr_op, 
           "$Id$")

// ***************************************************************************
// Root visitor for generating CDR operator declarations in the client header
// and stub
// ***************************************************************************

be_visitor_root_cdr_op::be_visitor_root_cdr_op (be_visitor_context *ctx)
  : be_visitor_root (ctx)
{
}

be_visitor_root_cdr_op::~be_visitor_root_cdr_op (void)
{
}

int
be_visitor_root_cdr_op::visit_root (be_root *node)
{
  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_root_cdr_op::visit_root - "
                         "codegen for scope failed\n"), 
                        -1);
    }

  TAO_OutStream *os = this->ctx_->stream ();
  TAO_CodeGen::CG_STATE cg_state = this->ctx_->state ();
  
  if (cg_state == TAO_CodeGen::TAO_ROOT_CDR_OP_CH)
    {
      *os << be_nl << be_nl
          << "TAO_BEGIN_VERSIONED_NAMESPACE_DECL"
          << be_nl << be_nl
          << "::CORBA::Boolean operator<< (TAO_OutputCDR &strm, const std::string &_tao_string);"
          << be_nl
          << "::CORBA::Boolean operator>> (TAO_InputCDR &strm, std::string &_tao_string);";
          
      *os << be_nl << be_nl
          << "TAO_END_VERSIONED_NAMESPACE_DECL";
          
      *os << be_nl << be_nl
          << "TAO_BEGIN_VERSIONED_NAMESPACE_DECL";
          
      *os << be_nl << be_nl
          << "::CORBA::Boolean operator<< (TAO_OutputCDR &strm, const Param_Test::UB_Long_Seq &_tao_vector);"
          << be_nl
          << "::CORBA::Boolean operator>> (TAO_InputCDR &strm, Param_Test::UB_Long_Seq &_tao_vector);";
          
      *os << be_nl << be_nl
          << "TAO_END_VERSIONED_NAMESPACE_DECL";
    }
  else if (cg_state == TAO_CodeGen::TAO_ROOT_CDR_OP_CS)
    {
      *os << be_nl << be_nl
          << "TAO_BEGIN_VERSIONED_NAMESPACE_DECL";
          
      *os << be_nl << be_nl
          << "::CORBA::Boolean operator<< (" << be_idt_nl
          << "TAO_OutputCDR &strm," << be_nl
          << "const std::string &_tao_string)" << be_uidt_nl
          << "{" << be_idt_nl
          << "return strm << _tao_string.c_str ();" << be_uidt_nl
          << "}" << be_nl << be_nl
          << "::CORBA::Boolean operator>> (" << be_idt_nl
          << "TAO_InputCDR &strm," << be_nl
          << "std::string &_tao_string)" << be_uidt_nl
          << "{" << be_idt_nl
          << "char *buf = 0;" << be_nl
          << "strm >> buf;" << be_nl
          << "_tao_string.assign (buf);" << be_nl
          << "ACE::strdelete (buf);" << be_nl
          << "return true;" << be_uidt_nl
          << "}";
          
      *os << be_nl << be_nl
          << "TAO_END_VERSIONED_NAMESPACE_DECL";
          
      *os << be_nl << be_nl
          << "TAO_BEGIN_VERSIONED_NAMESPACE_DECL";
          
      *os << be_nl << be_nl
          << "::CORBA::Boolean operator<< (" << be_idt_nl
          << "TAO_OutputCDR &strm," << be_nl
          << "const Param_Test::UB_Long_Seq &_tao_vector)"
          << be_uidt_nl
          << "{" << be_idt_nl
          << "::CORBA::ULong length = _tao_vector.size ();"
          << be_nl
          << "strm << length;" << be_nl << be_nl
          << "for ( ::CORBA::ULong i = 0UL; i < length; ++i)"
          << be_idt_nl
          << "{" << be_idt_nl
          << "strm << _tao_vector[i];" << be_uidt_nl
          << "}" << be_uidt_nl << be_nl
          << "return true;" << be_uidt_nl
          << "}";
          
      *os << be_nl << be_nl
          << "::CORBA::Boolean operator>> (" << be_idt_nl
          << "TAO_InputCDR &strm," << be_nl
          << "Param_Test::UB_Long_Seq &_tao_vector)" << be_uidt_nl
          << "{" << be_idt_nl
          << "::CORBA::ULong length = 0UL;" << be_nl
          << "::CORBA::ULong tmp = 0UL;" << be_nl << be_nl
          << "for ( ::CORBA::ULong i = 0UL; i < length; ++i)"
          << be_idt_nl
          << "{" << be_idt_nl
          << "strm >> tmp;" << be_nl
          << "_tao_vector[i] = tmp;" << be_uidt_nl
          << "}" << be_uidt_nl << be_nl
          << "return true;" << be_uidt_nl
          << "}";

      *os << be_nl << be_nl
          << "TAO_BEGIN_VERSIONED_NAMESPACE_DECL";       
    }

  return 0;
}
