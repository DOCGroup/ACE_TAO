//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    sequence_cs.cpp
//
// = DESCRIPTION
//    Visitor generating code for Sequences in the client stubs file
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

ACE_RCSID (be_visitor_sequence, 
           sequence_cs, 
           "$Id$")

// ************************************************************
// Root visitor for client stub class
// ************************************************************

be_visitor_sequence_cs::be_visitor_sequence_cs (be_visitor_context *ctx)
  : be_visitor_decl (ctx)
{
}

be_visitor_sequence_cs::~be_visitor_sequence_cs (void)
{
}

int be_visitor_sequence_cs::visit_sequence (be_sequence *node)
{
  if (node->cli_stub_gen () || node->imported ())
    {
      return 0;
    }

  TAO_OutStream *os = this->ctx_->stream ();
  be_type *bt = be_type::narrow_from_decl (node->base_type ());
  AST_Type::SIZE_TYPE st = bt->size_type ();

  *os << be_nl << be_nl << "// TAO_IDL - Generated from " << be_nl
      << "// "__FILE__ << ":" << __LINE__;

  // default constructor
  *os << be_nl << be_nl
      << node->name () << "::" << node->local_name () << " (void)" << be_nl
      << "{}";

  // for unbounded sequences, we have a different set of constructors
  if (node->unbounded ())
    {
      *os << be_nl << be_nl
          << node->name () << "::" << node->local_name ()
          << " (CORBA::ULong max)" << be_nl
          << "  : " << be_idt << be_idt;

      // Pass it to the base constructor.
      if (node->gen_base_class_name (os) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_sequence_cs::"
                             "visit_sequence - "
                            "codegen for base sequence class\n"), 
                           -1);
        }


      *os << be_nl << "(max)" << be_uidt << be_uidt_nl
          << "{}";
    }

  // constructor with the buffer
  *os << be_nl << be_nl 
      << node->name () << "::" << node->local_name () << " (" 
      << be_idt << be_idt_nl;

  if (node->unbounded ())
    {
      // Unbounded seq takes this extra parameter.
      *os << "CORBA::ULong max," << be_nl;
    }

  *os << "CORBA::ULong length," << be_nl;

  // generate the base type for the buffer
  be_visitor_context ctx (*this->ctx_);
  be_visitor_sequence_buffer_type bt_visitor (&ctx);

  if (bt->accept (&bt_visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_sequence_cs::"
                         "visit_sequence - "
                         "base type visit failed\n"),
                        -1);
    }

  *os << "* buffer," << be_nl
      << "CORBA::Boolean release" << be_uidt_nl
      << ")" << be_uidt_nl
      << "  : " << be_idt << be_idt;

  // Pass it to the base constructor.
  if (node->gen_base_class_name (os) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_sequence_cs::"
                         "visit_sequence - "
                         "codegen for base sequence class\n"), 
                        -1);
    }

  *os << be_nl << "(";

  if (node->unbounded ())
    {
      *os << "max, ";
    }

  *os << "length, buffer, release)" << be_uidt << be_uidt_nl
      << "{}";

  // Copy constructor.
  *os << be_nl << be_nl 
      << node->name () << "::" << node->local_name ()
      << " (const " << node->local_name ()
      << " &seq)" << be_nl
      << "  : " << be_idt << be_idt;

  // Pass it to the base constructor.
  if (node->gen_base_class_name (os) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_sequence_cs::"
                         "visit_sequence - "
                         "codegen for base sequence class\n"), 
                        -1);
    }

  *os << be_nl << "(seq)" << be_uidt << be_uidt_nl
      << "{}";

  // Destructor.
  *os << be_nl << be_nl
      << node->name () << "::~" << node->local_name ()
      << " (void)" << be_nl
      << "{}" << be_nl << be_nl;


  if (be_global->any_support ())
    {
      *os << "void "
          << node->name () << "::_tao_any_destructor (void *_tao_void_pointer)" 
          << be_nl
          << "{" << be_idt_nl
          << node->local_name () << " *tmp = ACE_static_cast ("
          << node->local_name () << "*, _tao_void_pointer);" << be_nl
          << "delete tmp;" << be_uidt_nl
          << "}";
    }

  if (this->ctx_->tdef () != 0)
    {
      this->gen_varout_tmplinst (node,
                                 bt);
    }

  node->cli_stub_gen (1);
  return 0;
}

int
be_visitor_sequence_cs::gen_varout_tmplinst (be_sequence *node,
                                             be_type *bt)
{
  TAO_OutStream *os = this->ctx_->stream ();

  os->gen_ifdef_AHETI ();

  switch (node->managed_type ())
    {
    case be_sequence::MNG_OBJREF:
      *os << be_nl << be_nl
          << "template class" << be_idt_nl
          << "TAO_MngSeq_Var_T<" << be_idt << be_idt_nl
          << node->name () << "," << be_nl
          << "TAO_Object_Manager<" << be_idt << be_idt_nl
          << bt->name () << "," << be_nl
          << bt->name () << "_var," << be_nl
          << bt->fwd_helper_name () << "_life" << be_uidt_nl
          << ">" << be_uidt << be_uidt_nl
          << ">;" << be_uidt << be_uidt;

      *os << be_nl << be_nl
          << "template class" << be_idt_nl
          << "TAO_Seq_Out_T<" << be_idt << be_idt_nl
          << node->name () << "," << be_nl
          << node->name () << "_var," << be_nl
          << "TAO_Object_Manager<" << be_idt << be_idt_nl
          << bt->name () << "," << be_nl
          << bt->name () << "_var," << be_nl
          << bt->fwd_helper_name () << "_life" << be_uidt_nl
          << ">" << be_uidt << be_uidt_nl
          << ">;" << be_uidt << be_uidt;

      break;
    case be_sequence::MNG_ABSTRACT:
      *os << be_nl << be_nl
          << "template class" << be_idt_nl
          << "TAO_MngSeq_Var_T<" << be_idt << be_idt_nl
          << node->name () << "," << be_nl
          << "TAO_Abstract_Manager<" << be_idt << be_idt_nl
          << bt->name () << "," << be_nl
          << bt->name () << "_var," << be_nl
          << bt->fwd_helper_name () << "_life" << be_uidt_nl
          << ">" << be_uidt << be_uidt_nl
          << ">;" << be_uidt << be_uidt;

      *os << be_nl << be_nl
          << "template class" << be_idt_nl
          << "TAO_Seq_Out_T<" << be_idt << be_idt_nl
          << node->name () << "," << be_nl
          << node->name () << "_var," << be_nl
          << "TAO_Abstract_Manager<" << be_idt << be_idt_nl
          << bt->name () << "," << be_nl
          << bt->name () << "_var," << be_nl
          << bt->fwd_helper_name () << "_life" << be_uidt_nl
          << ">" << be_uidt << be_uidt_nl
          << ">;" << be_uidt << be_uidt;

      break;
    case be_sequence::MNG_PSEUDO:
      *os << be_nl << be_nl
          << "template class" << be_idt_nl
          << "TAO_MngSeq_Var_T<" << be_idt << be_idt_nl
          << node->name () << "," << be_nl
          << "TAO_Pseudo_Object_Manager<" << be_idt << be_idt_nl
          << bt->name () << "," << be_nl
          << bt->name () << "_var" << be_uidt_nl
          << ">" << be_uidt << be_uidt_nl
          << ">;" << be_uidt << be_uidt;

      *os << be_nl << be_nl
          << "template class" << be_idt_nl
          << "TAO_Seq_Out_T<" << be_idt << be_idt_nl
          << node->name () << "," << be_nl
          << node->name () << "_var," << be_nl
          << "TAO_Pseudo_Object_Manager<" << be_idt << be_idt_nl
          << bt->name () << "," << be_nl
          << bt->name () << "_var" << be_uidt_nl
          << ">" << be_uidt << be_uidt_nl
          << ">;" << be_uidt << be_uidt;

      break;
    case be_sequence::MNG_VALUE:
      *os << be_nl << be_nl
          << "template class" << be_idt_nl
          << "TAO_MngSeq_Var_T<" << be_idt << be_idt_nl
          << node->name () << "," << be_nl
          << "TAO_Valuetype_Manager<" << be_idt << be_idt_nl
          << bt->name () << "," << be_nl
          << bt->name () << "_var," << be_nl
          << bt->fwd_helper_name () << "_life" << be_uidt_nl
          << ">" << be_uidt << be_uidt_nl
          << ">;" << be_uidt << be_uidt;

      *os << be_nl << be_nl
          << "template class" << be_idt_nl
          << "TAO_Seq_Out_T<" << be_idt << be_idt_nl
          << node->name () << "," << be_nl
          << node->name () << "_var," << be_nl
          << "TAO_Valuetype_Manager<" << be_idt << be_idt_nl
          << bt->name () << "," << be_nl
          << bt->name () << "_var," << be_nl
          << bt->fwd_helper_name () << "_life" << be_uidt_nl
          << ">" << be_uidt << be_uidt_nl
          << ">;" << be_uidt << be_uidt;

      break;
    case be_sequence::MNG_STRING:
      *os << be_nl << be_nl
          << "template class" << be_idt_nl
          << "TAO_MngSeq_Var_T<" << be_idt << be_idt_nl
          << node->name () << "," << be_nl
          <<"TAO_SeqElem_String_Manager" << be_uidt_nl
          << ">;" << be_uidt << be_uidt;

      *os << be_nl << be_nl
          << "template class" << be_idt_nl
          << "TAO_Seq_Out_T<" << be_idt << be_idt_nl
          << node->name () << "," << be_nl
          << node->name () << "_var," << be_nl
          << "TAO_SeqElem_String_Manager" << be_uidt_nl
          << ">;" << be_uidt << be_uidt;

      break;
    case be_sequence::MNG_WSTRING:
      *os << be_nl << be_nl
          << "template class" << be_idt_nl
          << "TAO_MngSeq_Var_T<" << be_idt << be_idt_nl
          << node->name () << "," << be_nl
          <<"TAO_SeqElem_WString_Manager" << be_uidt_nl
          << ">;" << be_uidt << be_uidt;

      *os << be_nl << be_nl
          << "template class" << be_idt_nl
          << "TAO_Seq_Out_T<" << be_idt << be_idt_nl
          << node->name () << "," << be_nl
          << node->name () << "_var," << be_nl
          << "TAO_SeqElem_WString_Manager" << be_uidt_nl
          << ">;" << be_uidt << be_uidt;

      break;
    default: // not a managed type
      {
        AST_Type::SIZE_TYPE st = bt->size_type ();

        *os << be_nl << be_nl
            << "template class" << be_idt_nl
            << (st == AST_Type::FIXED ? "TAO_FixedSeq_Var_T<" 
                                      : "TAO_VarSeq_Var_T<")
            << be_idt << be_idt_nl
            << node->local_name () << "," << be_nl
            << bt->name () << be_uidt_nl
            << ">;" << be_uidt << be_uidt;

        *os << be_nl << be_nl
            << "template class" << be_idt_nl
            << "TAO_Seq_Out_T<" << be_idt << be_idt_nl
            << node->local_name () << "," << be_nl
            << node->local_name () << "_var," << be_nl
            << bt->name () << be_uidt_nl
            << ">;" << be_uidt << be_uidt;
      }

      break;
    }

  os->gen_elif_AHETI ();
     
  switch (node->managed_type ())
    {
    case be_sequence::MNG_OBJREF:
      *os << be_nl << be_nl
          << "# pragma instantiate \\" << be_idt << be_idt_nl
          << "TAO_MngSeq_Var_T< \\" << be_idt << be_idt_nl
          << node->name () << ", \\" << be_nl
          << "TAO_Object_Manager< \\" << be_idt << be_idt_nl
          << bt->name () << ", \\" << be_nl
          << bt->name () << "_var, \\" << be_nl
          << bt->fwd_helper_name () << "_life \\" << be_uidt_nl
          << "> \\" << be_uidt << be_uidt_nl
          << ">" << be_uidt << be_uidt << be_uidt;

      *os << be_nl << be_nl
          << "# pragma instantiate \\" << be_idt << be_idt_nl
          << "TAO_Seq_Out_T< \\" << be_idt << be_idt_nl
          << node->name () << ", \\" << be_nl
          << node->name () << "_var, \\" << be_nl
          << "TAO_Object_Manager< \\" << be_idt << be_idt_nl
          << bt->name () << ", \\" << be_nl
          << bt->name () << "_var, \\" << be_nl
          << bt->fwd_helper_name () << "_life \\" << be_uidt_nl
          << "> \\" << be_uidt << be_uidt_nl
          << ">" << be_uidt << be_uidt;

      break;
    case be_sequence::MNG_ABSTRACT:
      *os << be_nl << be_nl
          << "# pragma instantiate \\" << be_idt_nl
          << "TAO_MngSeq_Var_T< \\" << be_idt << be_idt_nl
          << node->name () << ", \\" << be_nl
          << "TAO_Abstract_Manager< \\" << be_idt << be_idt_nl
          << bt->name () << ", \\" << be_nl
          << bt->name () << "_var, \\" << be_nl
          << bt->fwd_helper_name () << "_life \\" << be_uidt_nl
          << "> \\" << be_uidt << be_uidt_nl
          << ">" << be_uidt << be_uidt;

      *os << be_nl << be_nl
          << "# pragma instantiate \\" << be_idt_nl
          << "TAO_Seq_Out_T< \\" << be_idt << be_idt_nl
          << node->name () << ", \\" << be_nl
          << node->name () << "_var, \\" << be_nl
          << "TAO_Abstract_Manager< \\" << be_idt << be_idt_nl
          << bt->name () << ", \\" << be_nl
          << bt->name () << "_var, \\" << be_nl
          << bt->fwd_helper_name () << "_life \\" << be_uidt_nl
          << "> \\" << be_uidt << be_uidt_nl
          << ">" << be_uidt << be_uidt;

      break;
    case be_sequence::MNG_PSEUDO:
      *os << be_nl << be_nl
          << "# pragma instantiate \\" << be_idt_nl
          << "TAO_MngSeq_Var_T< \\" << be_idt << be_idt_nl
          << node->name () << ", \\" << be_nl
          << "TAO_Pseudo_Object_Manager< \\" << be_idt << be_idt_nl
          << bt->name () << ", \\" << be_nl
          << bt->name () << "_var \\" << be_uidt_nl
          << "> \\" << be_uidt << be_uidt_nl
          << ">" << be_uidt << be_uidt;

      *os << be_nl << be_nl
          << "# pragma instantiate \\" << be_idt_nl
          << "TAO_Seq_Out_T< \\" << be_idt << be_idt_nl
          << node->name () << ", \\" << be_nl
          << node->name () << "_var, \\" << be_nl
          << "TAO_Pseudo_Object_Manager< \\" << be_idt << be_idt_nl
          << bt->name () << ", \\" << be_nl
          << bt->name () << "_var \\" << be_uidt_nl
          << "> \\" << be_uidt << be_uidt_nl
          << ">" << be_uidt << be_uidt;

      break;
    case be_sequence::MNG_VALUE:
      *os << be_nl << be_nl
          << "# pragma instantiate \\" << be_idt_nl
          << "TAO_MngSeq_Var_T< \\" << be_idt << be_idt_nl
          << node->name () << ", \\" << be_nl
          << "TAO_Valuetype_Manager< \\" << be_idt << be_idt_nl
          << bt->name () << ", \\" << be_nl
          << bt->name () << "_var, \\" << be_nl
          << bt->fwd_helper_name () << "_life \\" << be_uidt_nl
          << "> \\" << be_uidt << be_uidt_nl
          << ">" << be_uidt << be_uidt;

      *os << be_nl << be_nl
          << "# pragma instantiate \\" << be_idt_nl
          << "TAO_Seq_Out_T< \\" << be_idt << be_idt_nl
          << node->name () << ", \\" << be_nl
          << node->name () << "_var, \\" << be_nl
          << "TAO_Valuetype_Manager< \\" << be_idt << be_idt_nl
          << bt->name () << ", \\" << be_nl
          << bt->name () << "_var, \\" << be_nl
          << bt->fwd_helper_name () << "_life \\" << be_uidt_nl
          << "> \\" << be_uidt << be_uidt_nl
          << ">" << be_uidt << be_uidt;

      break;
    case be_sequence::MNG_STRING:
      *os << be_nl << be_nl
          << "# pragma instantiate \\" << be_idt_nl
          << "TAO_MngSeq_Var_T< \\" << be_idt << be_idt_nl
          << node->name () << ", \\" << be_nl
          <<"TAO_SeqElem_String_Manager \\" << be_uidt_nl
          << ">" << be_uidt << be_uidt;

      *os << be_nl << be_nl
          << "# pragma instantiate \\" << be_idt_nl
          << "TAO_Seq_Out_T< \\" << be_idt << be_idt_nl
          << node->name () << ", \\" << be_nl
          << node->name () << "_var, \\" << be_nl
          << "TAO_SeqElem_String_Manager \\" << be_uidt_nl
          << ">" << be_uidt << be_uidt;

      break;
    case be_sequence::MNG_WSTRING:
      *os << be_nl << be_nl
          << "# pragma instantiate \\" << be_idt_nl
          << "TAO_MngSeq_Var_T< \\" << be_idt << be_idt_nl
          << node->name () << ", \\" << be_nl
          <<"TAO_SeqElem_WString_Manager \\" << be_uidt_nl
          << ">" << be_uidt << be_uidt;

      *os << be_nl << be_nl
          << "# pragma instantiate \\" << be_idt_nl
          << "TAO_Seq_Out_T< \\" << be_idt << be_idt_nl
          << node->name () << ", \\" << be_nl
          << node->name () << "_var, \\" << be_nl
          << "TAO_SeqElem_WString_Manager \\" << be_uidt_nl
          << ">" << be_uidt << be_uidt;

      break;
    default: // not a managed type
      {
        AST_Type::SIZE_TYPE st = bt->size_type ();

        *os << be_nl << be_nl
            << "# pragma instantiate \\" << be_idt_nl
            << (st == AST_Type::FIXED ? "TAO_FixedSeq_Var_T< \\" 
                                      : "TAO_VarSeq_Var_T< \\")
            << be_idt << be_idt_nl
            << node->local_name () << ", \\" << be_nl
            << bt->name () << " \\" << be_uidt_nl
            << ">" << be_uidt << be_uidt;

        *os << be_nl << be_nl
            << "# pragma instantiate \\" << be_idt_nl
            << "TAO_Seq_Out_T< \\" << be_idt << be_idt_nl
            << node->local_name () << ", \\" << be_nl
            << node->local_name () << "_var, \\" << be_nl
            << bt->name () << " \\" << be_uidt_nl
            << ">" << be_uidt << be_uidt;
      }

      break;
    }

  os->gen_endif_AHETI ();

  return 0;
}

