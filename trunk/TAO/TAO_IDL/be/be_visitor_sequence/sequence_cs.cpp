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
  be_type *bt = be_type::narrow_from_decl (node->base_type ());

  if (node->imported ())
    {
      bt->seen_in_sequence (I_TRUE);
      return 0;
    }

  if (node->cli_stub_gen ())
    {
      return 0;
    }

  AST_Decl::NodeType nt = bt->node_type ();

  // If our base type is an anonymous sequence, generate code for it here.
  if (nt == AST_Decl::NT_sequence)
    {
      if (bt->accept (this) != 0)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_sequence_cs::"
                             "visit_sequence - "
                            "codegen for anonymous base type failed\n"),
                           -1);
        }

    }

  TAO_OutStream *os = this->ctx_->stream ();

  *os << be_nl << be_nl << "// TAO_IDL - Generated from " << be_nl
      << "// "__FILE__ << ":" << __LINE__;

  os->gen_ifdef_macro (node->flat_name ());

  // default constructor
  *os << be_nl << be_nl
      << node->name () << "::" << node->local_name () << " (void)" << be_nl
      << "{}";

  // for unbounded sequences, we have a different set of constructors
  if (node->unbounded ())
    {
      *os << be_nl << be_nl
          << node->name () << "::" << node->local_name () << " ("
          << be_idt << be_idt_nl
          << "CORBA::ULong max" << be_uidt_nl
          << ")" << be_nl
          << ": " << be_idt;

      // Pass it to the base constructor.
      if (node->gen_base_class_name (os, idl_global->root ()) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_sequence_cs::"
                             "visit_sequence - "
                            "codegen for base sequence class failed\n"),
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

  *os << " * buffer," << be_nl
      << "CORBA::Boolean release" << be_uidt_nl
      << ")" << be_uidt_nl
      << "  : " << be_idt << be_idt;

  // Pass it to the base constructor.
  if (node->gen_base_class_name (os, idl_global->root ()) == -1)
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
      << " (" << be_idt << be_idt_nl
      << "const " << node->local_name ()
      << " &seq" << be_uidt_nl
      << ")" << be_uidt_nl
      << "  : " << be_idt << be_idt;

  // Pass it to the base constructor.
  if (node->gen_base_class_name (os, idl_global->root ()) == -1)
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
      << "{}";


  if (be_global->any_support ())
    {
      *os << be_nl << be_nl
          << "void "
          << node->name () << "::_tao_any_destructor (" << be_idt << be_idt_nl
          << "void * _tao_void_pointer" << be_uidt_nl
          << ")" << be_uidt_nl
          << "{" << be_idt_nl
          << node->local_name () << " * tmp =" << be_idt_nl
          << "ACE_static_cast (" << be_idt << be_idt_nl
          << node->local_name () << " *," << be_nl
          << "_tao_void_pointer" << be_uidt_nl
          << ");" << be_uidt << be_uidt_nl
          << "delete tmp;" << be_uidt_nl
          << "}";
    }

  if (!bt->seen_in_sequence ())
    {
      // This is a no-op unless our element is a managed type.
      this->gen_managed_type_tmplinst (node, bt);
    }

  if (this->ctx_->tdef () != 0)
    {
      this->gen_varout_tmplinst (node,
                                 bt);

      if (nt == AST_Decl::NT_typedef)
        {
          be_typedef *td = be_typedef::narrow_from_decl (bt);
          nt = td->base_node_type ();
        }
    }
      
  // basic IDL types are in TAO. Sequences of (w)strings in TAO are
  // specializations and so are not template classes.
  if ((nt != AST_Decl::NT_pre_defined
       && nt != AST_Decl::NT_string
       && nt != AST_Decl::NT_wstring)
      || ! node->unbounded ())
    {
      if (this->gen_base_class_tmplinst (node, bt) == -1)
        {
          return -1;
        }
    }

  os->gen_endif ();

  bt->seen_in_sequence (I_TRUE);
  node->cli_stub_gen (I_TRUE);
  return 0;
}

void
be_visitor_sequence_cs::gen_managed_type_tmplinst (be_sequence *node,
                                                   be_type *bt)
{
  TAO_OutStream *os = this->ctx_->stream ();

  switch (node->managed_type ())
    {
      case be_sequence::MNG_OBJREF:
        os->gen_ifdef_AHETI ();

        *os << be_nl << be_nl
            << "template class" << be_idt_nl
            << "TAO_Object_Manager<" << be_idt << be_idt_nl
            << bt->name () << "," << be_nl
            << bt->name () << "_var," << be_nl
            << bt->fwd_helper_name () << "_life" << be_uidt_nl
            << ">;" << be_uidt << be_uidt;

        os->gen_elif_AHETI ();

        *os << be_nl << be_nl
            << "# pragma instantiate \\" << be_idt << be_idt_nl
            << "TAO_Object_Manager< \\" << be_idt << be_idt_nl
            << bt->name () << ", \\" << be_nl
            << bt->name () << "_var, \\" << be_nl
            << bt->fwd_helper_name () << "_life \\" << be_uidt_nl
            << ">" << be_uidt << be_uidt << be_uidt;

        os->gen_endif_AHETI ();

        break;
      case be_sequence::MNG_ABSTRACT:
        os->gen_ifdef_AHETI ();

        *os << be_nl << be_nl
            << "template class" << be_idt_nl
            << "TAO_Abstract_Manager<" << be_idt << be_idt_nl
            << bt->name () << "," << be_nl
            << bt->name () << "_var," << be_nl
            << bt->fwd_helper_name () << "_life" << be_uidt_nl
            << ">;" << be_uidt << be_uidt;

        os->gen_elif_AHETI ();

        *os << be_nl << be_nl
            << "# pragma instantiate \\" << be_idt << be_idt_nl
            << "TAO_Abstract_Manager< \\" << be_idt << be_idt_nl
            << bt->name () << ", \\" << be_nl
            << bt->name () << "_var, \\" << be_nl
            << bt->fwd_helper_name () << "_life \\" << be_uidt_nl
            << ">" << be_uidt << be_uidt << be_uidt;

        os->gen_endif_AHETI ();

        break;
      case be_sequence::MNG_VALUE:
        os->gen_ifdef_AHETI ();

        *os << be_nl << be_nl
            << "template class" << be_idt_nl
            << "TAO_Valuetype_Manager<" << be_idt << be_idt_nl
            << bt->name () << "," << be_nl
            << bt->name () << "_var," << be_nl
            << bt->fwd_helper_name () << "_life" << be_uidt_nl
            << ">;" << be_uidt << be_uidt;

        os->gen_elif_AHETI ();

        *os << be_nl << be_nl
            << "# pragma instantiate \\" << be_idt << be_idt_nl
            << "TAO_Valuetype_Manager< \\" << be_idt << be_idt_nl
            << bt->name () << ", \\" << be_nl
            << bt->name () << "_var, \\" << be_nl
            << bt->fwd_helper_name () << "_life \\" << be_uidt_nl
            << ">" << be_uidt << be_uidt << be_uidt;

        os->gen_endif_AHETI ();

        break;
      case be_sequence::MNG_PSEUDO:
        os->gen_ifdef_AHETI ();

        *os << be_nl << be_nl
            << "template class" << be_idt_nl
            << "TAO_Pseudo_Object_Manager<" << be_idt << be_idt_nl
            << bt->name () << "," << be_nl
            << bt->name () << "_var" << be_uidt_nl
            << ">;" << be_uidt << be_uidt;

        os->gen_elif_AHETI ();

        *os << be_nl << be_nl
            << "# pragma instantiate \\" << be_idt << be_idt_nl
            << "TAO_Pseudo_Object_Manager< \\" << be_idt << be_idt_nl
            << bt->name () << ", \\" << be_nl
            << bt->name () << "_var \\" << be_uidt_nl
            << ">" << be_uidt << be_uidt << be_uidt;

        os->gen_endif_AHETI ();

        break;
      default:
        //  String and Wstring managed types are not template classes.
        break;
    }
}

int
be_visitor_sequence_cs::gen_varout_tmplinst (be_sequence *node,
                                             be_type *bt)
{
  TAO_OutStream *os = this->ctx_->stream ();

  os->gen_ifdef_AHETI ();

  /// @@NOTE: This method contains too much of duplicated code. Needs
  /// to be refactored. -- Bala
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

        // @@NOTE: Too much of code repetition. Aint there a way to
        // refactor these things?
        *os << be_nl << be_nl
            << "template class" << be_idt_nl
            << "TAO_Seq_Var_Base_T<" << be_idt << be_idt_nl
            << node->name () << "," << be_nl
            << "TAO_Object_Manager<" << be_idt << be_idt_nl
            << bt->name () << "," << be_nl
            << bt->name () << "_var," << be_nl
            << bt->fwd_helper_name () << "_life" << be_uidt_nl
            << ">" << be_uidt << be_uidt_nl
            << ">;" << be_uidt << be_uidt;

        *os << be_nl << be_nl
            << "template class" << be_idt_nl
            << "TAO_MngSeq_Out_T<" << be_idt << be_idt_nl
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

        // @@Please see note above
        *os << be_nl << be_nl
            << "template class" << be_idt_nl
            << "TAO_Seq_Var_Base_T<" << be_idt << be_idt_nl
            << node->name () << "," << be_nl
            << "TAO_Abstract_Manager<" << be_idt << be_idt_nl
            << bt->name () << "," << be_nl
            << bt->name () << "_var," << be_nl
            << bt->fwd_helper_name () << "_life" << be_uidt_nl
            << ">" << be_uidt << be_uidt_nl
            << ">;" << be_uidt << be_uidt;

        *os << be_nl << be_nl
            << "template class" << be_idt_nl
            << "TAO_MngSeq_Out_T<" << be_idt << be_idt_nl
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
            << "TAO_Seq_Var_Base_T<" << be_idt << be_idt_nl
            << node->name () << "," << be_nl
            << "TAO_Pseudo_Object_Manager<" << be_idt << be_idt_nl
            << bt->name () << "," << be_nl
            << bt->name () << "_var" << be_uidt_nl
            << ">" << be_uidt << be_uidt_nl
            << ">;" << be_uidt << be_uidt;

        *os << be_nl << be_nl
            << "template class" << be_idt_nl
            << "TAO_MngSeq_Out_T<" << be_idt << be_idt_nl
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

        //@@Please see note above..
        *os << be_nl << be_nl
            << "template class" << be_idt_nl
            << "TAO_Seq_Var_Base_T<" << be_idt << be_idt_nl
            << node->name () << "," << be_nl
            << "TAO_Valuetype_Manager<" << be_idt << be_idt_nl
            << bt->name () << "," << be_nl
            << bt->name () << "_var," << be_nl
            << bt->fwd_helper_name () << "_life" << be_uidt_nl
            << ">" << be_uidt << be_uidt_nl
            << ">;" << be_uidt << be_uidt;

        *os << be_nl << be_nl
            << "template class" << be_idt_nl
            << "TAO_MngSeq_Out_T<" << be_idt << be_idt_nl
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
            << "TAO_Seq_Var_Base_T<" << be_idt << be_idt_nl
            << node->name () << "," << be_nl
            <<"TAO_SeqElem_String_Manager" << be_uidt_nl
            << ">;" << be_uidt << be_uidt;

        *os << be_nl << be_nl
            << "template class" << be_idt_nl
            << "TAO_MngSeq_Out_T<" << be_idt << be_idt_nl
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
            << "TAO_Seq_Var_Base_T<" << be_idt << be_idt_nl
            << node->name () << "," << be_nl
            <<"TAO_SeqElem_WString_Manager" << be_uidt_nl
            << ">;" << be_uidt << be_uidt;

        *os << be_nl << be_nl
            << "template class" << be_idt_nl
            << "TAO_MngSeq_Out_T<" << be_idt << be_idt_nl
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
              << node->name () << "," << be_nl
              << bt->name () << be_uidt_nl
              << ">;" << be_uidt << be_uidt;

          // @@ Please see not above..
          *os << be_nl << be_nl
              << "template class" << be_idt_nl
              << "TAO_Seq_Var_Base_T<" << be_idt << be_idt_nl
              << node->name () << "," << be_nl
              << bt->name () << be_uidt_nl
              << ">;" << be_uidt << be_uidt;

          *os << be_nl << be_nl
              << "template class" << be_idt_nl
              << "TAO_Seq_Out_T<" << be_idt << be_idt_nl
              << node->name () << "," << be_nl
              << node->name () << "_var," << be_nl
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
            << "TAO_Seq_Var_Base_T< \\" << be_idt << be_idt_nl
            << node->name () << ", \\" << be_nl
            << "TAO_Object_Manager< \\" << be_idt << be_idt_nl
            << bt->name () << ", \\" << be_nl
            << bt->name () << "_var, \\" << be_nl
            << bt->fwd_helper_name () << "_life \\" << be_uidt_nl
            << "> \\" << be_uidt << be_uidt_nl
            << ">" << be_uidt << be_uidt << be_uidt;

        *os << be_nl << be_nl
            << "# pragma instantiate \\" << be_idt << be_idt_nl
            << "TAO_MngSeq_Out_T< \\" << be_idt << be_idt_nl
            << node->name () << ", \\" << be_nl
            << node->name () << "_var, \\" << be_nl
            << "TAO_Object_Manager< \\" << be_idt << be_idt_nl
            << bt->name () << ", \\" << be_nl
            << bt->name () << "_var, \\" << be_nl
            << bt->fwd_helper_name () << "_life \\" << be_uidt_nl
            << "> \\" << be_uidt << be_uidt_nl
            << ">" << be_uidt << be_uidt << be_uidt;

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
            << "TAO_Seq_Var_Base_T< \\" << be_idt << be_idt_nl
            << node->name () << ", \\" << be_nl
            << "TAO_Abstract_Manager< \\" << be_idt << be_idt_nl
            << bt->name () << ", \\" << be_nl
            << bt->name () << "_var, \\" << be_nl
            << bt->fwd_helper_name () << "_life \\" << be_uidt_nl
            << "> \\" << be_uidt << be_uidt_nl
            << ">" << be_uidt << be_uidt;

        *os << be_nl << be_nl
            << "# pragma instantiate \\" << be_idt_nl
            << "TAO_MngSeq_Out_T< \\" << be_idt << be_idt_nl
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
            << "TAO_Seq_Var_Base_T< \\" << be_idt << be_idt_nl
            << node->name () << ", \\" << be_nl
            << "TAO_Pseudo_Object_Manager< \\" << be_idt << be_idt_nl
            << bt->name () << ", \\" << be_nl
            << bt->name () << "_var \\" << be_uidt_nl
            << "> \\" << be_uidt << be_uidt_nl
            << ">" << be_uidt << be_uidt;

        *os << be_nl << be_nl
            << "# pragma instantiate \\" << be_idt_nl
            << "TAO_MngSeq_Out_T< \\" << be_idt << be_idt_nl
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
            << "TAO_Seq_Var_Base_T< \\" << be_idt << be_idt_nl
            << node->name () << ", \\" << be_nl
            << "TAO_Valuetype_Manager< \\" << be_idt << be_idt_nl
            << bt->name () << ", \\" << be_nl
            << bt->name () << "_var, \\" << be_nl
            << bt->fwd_helper_name () << "_life \\" << be_uidt_nl
            << "> \\" << be_uidt << be_uidt_nl
            << ">" << be_uidt << be_uidt;

        *os << be_nl << be_nl
            << "# pragma instantiate \\" << be_idt_nl
            << "TAO_MngSeq_Out_T< \\" << be_idt << be_idt_nl
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
            << "TAO_Seq_Var_Base_T< \\" << be_idt << be_idt_nl
            << node->name () << ", \\" << be_nl
            <<"TAO_SeqElem_String_Manager \\" << be_uidt_nl
            << ">" << be_uidt << be_uidt;

        *os << be_nl << be_nl
            << "# pragma instantiate \\" << be_idt_nl
            << "TAO_MngSeq_Out_T< \\" << be_idt << be_idt_nl
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
            << "TAO_Seq_Var_Base_T< \\" << be_idt << be_idt_nl
            << node->name () << ", \\" << be_nl
            <<"TAO_SeqElem_WString_Manager \\" << be_uidt_nl
            << ">" << be_uidt << be_uidt;

        *os << be_nl << be_nl
            << "# pragma instantiate \\" << be_idt_nl
            << "TAO_MngSeq_Out_T< \\" << be_idt << be_idt_nl
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
              << "TAO_Seq_Var_Base_T< \\" << be_idt << be_idt_nl
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

int
be_visitor_sequence_cs::gen_base_class_tmplinst (be_sequence *node,
                                                 be_type *elem)
{
  TAO_OutStream *os = this->ctx_->stream ();

  os->gen_ifdef_macro (node->instance_name ());
  os->gen_ifdef_AHETI ();

  *os << be_nl << be_nl
      <<"template class ";

  // Pass it to the base constructor.
  if (node->gen_base_class_name (os, idl_global->root ()) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_sequence_cs::"
                         "visit_sequence - "
                         "codegen for base sequence class failed\n"),
                        -1);
    }

  *os << ";" << be_uidt;

  os->gen_elif_AHETI ();

  *os << be_nl << be_nl
      << "#pragma instantiate ";

  // Pass it to the base constructor.
  if (node->gen_base_class_name (os, idl_global->root ()) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_sequence_cs::"
                         "visit_sequence - "
                         "codegen for base sequence class failed\n"),
                        -1);
    }

  *os << be_uidt;

  os->gen_endif_AHETI ();
  os->gen_endif ();

  return 0;
}
