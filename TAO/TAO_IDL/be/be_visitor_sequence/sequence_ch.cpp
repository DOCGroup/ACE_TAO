//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    sequence_ch.cpp
//
// = DESCRIPTION
//    Visitor generating code for Sequence in the client header
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

ACE_RCSID (be_visitor_sequence,
           sequence_ch,
           "$Id$")

// Root visitor for client header.
be_visitor_sequence_ch::be_visitor_sequence_ch (be_visitor_context *ctx)
  : be_visitor_decl (ctx)
{
}

be_visitor_sequence_ch::~be_visitor_sequence_ch (void)
{
}

int be_visitor_sequence_ch::visit_sequence (be_sequence *node)
{
  if (node->defined_in () == 0)
    {
      // The node is a nested sequence, and has had no scope defined.
      node->set_defined_in (DeclAsScope (this->ctx_->scope ()));
    }

  // First create a name for ourselves.
  if (node->create_name (this->ctx_->tdef ()) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_sequence_ch::"
                         "visit_sequence - "
                         "failed creating name\n"),
                        -1);
    }

  if (node->cli_hdr_gen () || node->imported ())
    {
      return 0;
    }

  be_type *bt = 0;
  TAO_OutStream *os = this->ctx_->stream ();

  // Retrieve the base type since we may need to do some code
  // generation for the base type.
  bt = be_type::narrow_from_decl (node->base_type ());

  if (bt == 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_sequence_ch::"
                         "visit_sequence - "
                         "Bad element type\n"),
                        -1);
    }

  AST_Decl::NodeType nt = bt->node_type ();

  // If our base type is an anonymouse sequence, we must create a  name
  // and generate a class declaration for it as well.
  if (nt == AST_Decl::NT_sequence)
    {
      if (bt->accept (this) != 0)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_sequence_ch::"
                             "visit_sequence - "
                             "codegen for anonymous base type failed\n"),
                            -1);
        }
    }

  *os << be_nl << be_nl << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__;

  os->gen_ifdef_macro (node->flat_name ());

  if (this->ctx_->tdef () != 0)
    {
      *os << be_nl << be_nl
          << "class " << node->local_name () << ";";
    }

  if (this->ctx_->tdef () != 0)
    {
      this->gen_varout_typedefs (node,
                                 bt);
    }

  *os << be_nl << be_nl
      << "class " << be_global->stub_export_macro () << " "
      << node->local_name () << be_idt_nl
      << ": public" << be_idt << be_idt_nl;

  if (node->gen_base_class_name (os, this->ctx_->scope ()) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_sequence_ch::"
                         "visit_sequence - "
                         "Base class name generation failed\n"),
                        -1);
    }

  *os << be_uidt << be_uidt << be_uidt;

  *os << be_nl
      << "{" << be_nl
      << "public:" << be_idt;

  *os << be_nl
      << node->local_name () << " (void);";

  if (node->unbounded ())
    {
      *os << be_nl
          << node->local_name () << " (CORBA::ULong max);";
    }

  *os << be_nl
      << node->local_name () << " (" << be_idt << be_idt;

  if (node->unbounded ())
    {
      *os << be_nl
          << "CORBA::ULong max,";
    }

  *os << be_nl
      << "CORBA::ULong length," << be_nl;

  // Generate the base type for the buffer.
  be_visitor_context ctx (*this->ctx_);
  ctx.state (TAO_CodeGen::TAO_SEQUENCE_BUFFER_TYPE_CH);
  be_visitor_sequence_buffer_type bt_visitor (&ctx);

  if (bt->accept (&bt_visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_sequence_ch::"
                         "visit_sequence - "
                         "base type visit failed\n"),
                        -1);
    }

  *os << "* buffer, " << be_nl
      << "CORBA::Boolean release = 0" << be_uidt_nl
      << ");" << be_uidt_nl;
  *os << node->local_name () << " (const " << node->local_name ()
      << " &);" << be_nl;
  *os << "~" << node->local_name () << " (void);" << be_nl << be_nl;

  if (be_global->any_support ())
    {
      *os << "static void _tao_any_destructor (void *);" << be_nl << be_nl;
    }

  // Generate the _var_type typedef (only if we are not anonymous).
  if (this->ctx_->tdef () != 0)
    {
      *os << "typedef " << node->local_name () << "_var _var_type;";
    }

  // TAO provides extensions for octet sequences, first find out if
  // the base type is an octet (or an alias for octet).
  be_predefined_type *predef = 0;

  if (bt->base_node_type () == AST_Type::NT_pre_defined)
    {
      be_typedef* alias =
            be_typedef::narrow_from_decl (bt);

      if (alias == 0)
        {
          predef = be_predefined_type::narrow_from_decl (bt);
        }
      else
        {
          predef =
            be_predefined_type::narrow_from_decl (
                alias->primitive_base_type ()
              );
        }
    }

  // Now generate the extension...
  if (predef != 0 && predef->pt () == AST_PredefinedType::PT_octet
      && node->unbounded ())
    {
      *os << "\n\n#if (TAO_NO_COPY_OCTET_SEQUENCES == 1)" << be_nl
                << node->local_name () << " (" << be_idt << be_idt_nl
                << "CORBA::ULong length," << be_nl
                << "const ACE_Message_Block* mb" << be_uidt_nl
                << ")" << be_uidt_nl
                << "  : CORBA::OctetSeq"
                << " (length, mb) {}" << "\n"
                << "#endif /* TAO_NO_COPY_OCTET_SEQUENCE == 1 */";
    }

  *os << be_uidt_nl
      << "};";

  os->gen_endif ();

  node->cli_hdr_gen (1);
  return 0;
}

void
be_visitor_sequence_ch::gen_varout_typedefs (be_sequence *node,
                                             be_type *elem)
{
  TAO_OutStream *os = this->ctx_->stream ();
  be_decl *scope = this->ctx_->scope ();

  *os << be_nl << be_nl;

  switch (node->managed_type ())
    {
    case be_sequence::MNG_OBJREF:
      *os << "typedef" << be_idt_nl
          << "TAO_MngSeq_Var_T<" << be_idt << be_idt_nl
          << node->local_name () << "," << be_nl
          << "TAO_Object_Manager<" << be_idt << be_idt_nl
          << elem->nested_type_name (scope) << "," << be_nl;
      *os << elem->nested_type_name (scope, "_var") << "," << be_nl
          << elem->fwd_helper_name () << "_life" << be_uidt_nl
          << ">" << be_uidt << be_uidt_nl
          << ">" << be_uidt_nl
          << node->local_name () << "_var;" << be_uidt;

      *os << be_nl << be_nl
          << "typedef" << be_idt_nl
          << "TAO_MngSeq_Out_T<" << be_idt << be_idt_nl
          << node->local_name () << "," << be_nl
          << node->local_name () << "_var," << be_nl
          << "TAO_Object_Manager<" << be_idt << be_idt_nl
          << elem->nested_type_name (scope) << "," << be_nl;
      *os << elem->nested_type_name (scope, "_var") << "," << be_nl
          << elem->fwd_helper_name () << "_life" << be_uidt_nl
          << ">" << be_uidt << be_uidt_nl
          << ">" << be_uidt_nl
          << node->local_name () << "_out;" << be_uidt;

      break;
    case be_sequence::MNG_ABSTRACT:
      *os << "typedef" << be_idt_nl
          << "TAO_MngSeq_Var_T<" << be_idt << be_idt_nl
          << node->local_name () << "," << be_nl
          << "TAO_Abstract_Manager<" << be_idt << be_idt_nl
          << elem->nested_type_name (scope) << "," << be_nl;
      *os << elem->nested_type_name (scope, "_var") << "," << be_nl
          << elem->fwd_helper_name () << "_life" << be_uidt_nl
          << ">" << be_uidt << be_uidt_nl
          << ">" << be_uidt_nl
          << node->local_name () << "_var;" << be_uidt;

      *os << be_nl << be_nl
          << "typedef" << be_idt_nl
          << "TAO_MngSeq_Out_T<" << be_idt << be_idt_nl
          << node->local_name () << "," << be_nl
          << node->local_name () << "_var," << be_nl
          << "TAO_Abstract_Manager<" << be_idt << be_idt_nl
          << elem->nested_type_name (scope) << "," << be_nl;
      *os << elem->nested_type_name (scope, "_var") << "," << be_nl
          << elem->fwd_helper_name () << "_life" << be_uidt_nl
          << ">" << be_uidt << be_uidt_nl
          << ">" << be_uidt_nl
          << node->local_name () << "_out;" << be_uidt;

      break;
    case be_sequence::MNG_PSEUDO:
      *os << "typedef" << be_idt_nl
          << "TAO_MngSeq_Var_T<" << be_idt << be_idt_nl
          << node->local_name () << "," << be_nl
          << "TAO_Pseudo_Object_Manager<" << be_idt << be_idt_nl
          << elem->nested_type_name (scope) << "," << be_nl;
      *os << elem->nested_type_name (scope, "_var") << be_uidt_nl
          << ">" << be_uidt << be_uidt_nl
          << ">" << be_uidt_nl
          << node->local_name () << "_var;" << be_uidt;

      *os << be_nl << be_nl
          << "typedef" << be_idt_nl
          << "TAO_MngSeq_Out_T<" << be_idt << be_idt_nl
          << node->local_name () << "," << be_nl
          << node->local_name () << "_var," << be_nl
          << "TAO_Pseudo_Object_Manager<" << be_idt << be_idt_nl
          << elem->nested_type_name (scope) << "," << be_nl;
      *os << elem->nested_type_name (scope, "_var") << be_uidt_nl
          << ">" << be_uidt << be_uidt_nl
          << ">" << be_uidt_nl
          << node->local_name () << "_out;" << be_uidt;

      break;
    case be_sequence::MNG_VALUE:
      *os << "typedef" << be_idt_nl
          << "TAO_MngSeq_Var_T<" << be_idt << be_idt_nl
          << node->local_name () << "," << be_nl
          << "TAO_Valuetype_Manager<" << be_idt << be_idt_nl
          << elem->nested_type_name (scope) << "," << be_nl;
      *os << elem->nested_type_name (scope, "_var") << "," << be_nl
          << elem->fwd_helper_name () << "_life" << be_uidt_nl
          << ">" << be_uidt << be_uidt_nl
          << ">" << be_uidt_nl
          << node->local_name () << "_var;" << be_uidt;

      *os << be_nl << be_nl
          << "typedef" << be_idt_nl
          << "TAO_MngSeq_Out_T<" << be_idt << be_idt_nl
          << node->local_name () << "," << be_nl
          << node->local_name () << "_var," << be_nl
          << "TAO_Valuetype_Manager<" << be_idt << be_idt_nl
          << elem->nested_type_name (scope) << "," << be_nl;
      *os << elem->nested_type_name (scope, "_var") << "," << be_nl
          << elem->fwd_helper_name () << "_life" << be_uidt_nl
          << ">" << be_uidt << be_uidt_nl
          << ">" << be_uidt_nl
          << node->local_name () << "_out;" << be_uidt;

      break;
    case be_sequence::MNG_STRING:
      *os << "typedef" << be_idt_nl
          << "TAO_MngSeq_Var_T<" << be_idt << be_idt_nl
          << node->local_name () << "," << be_nl
          << "TAO_SeqElem_String_Manager" << be_uidt_nl
          << ">" << be_uidt_nl
          << node->local_name () << "_var;" << be_uidt;

      *os << be_nl << be_nl
          << "typedef" << be_idt_nl
          << "TAO_MngSeq_Out_T<" << be_idt << be_idt_nl
          << node->local_name () << "," << be_nl
          << node->local_name () << "_var," << be_nl
          << "TAO_SeqElem_String_Manager" << be_uidt_nl
          << ">" << be_uidt_nl
          << node->local_name () << "_out;" << be_uidt;

      break;
    case be_sequence::MNG_WSTRING:
      *os << "typedef" << be_idt_nl
          << "TAO_MngSeq_Var_T<" << be_idt << be_idt_nl
          << node->local_name () << "," << be_nl
          << "TAO_SeqElem_WString_Manager" << be_uidt_nl
          << ">" << be_uidt_nl
          << node->local_name () << "_var;" << be_uidt;

      *os << be_nl << be_nl
          << "typedef" << be_idt_nl
          << "TAO_MngSeq_Out_T<" << be_idt << be_idt_nl
          << node->local_name () << "," << be_nl
          << node->local_name () << "_var," << be_nl
          << "TAO_SeqElem_WString_Manager" << be_uidt_nl
          << ">" << be_uidt_nl
          << node->local_name () << "_out;" << be_uidt;

      break;
    default: // Not a managed type.
      {
        AST_Type::SIZE_TYPE st = elem->size_type ();
        be_typedef *td = be_typedef::narrow_from_decl (elem);
        AST_Decl::NodeType nt = elem->node_type ();

        if (td != 0)
          {
            nt = td->base_node_type ();
          }

        *os << "typedef" << be_idt_nl
            << (st == AST_Type::FIXED ? "TAO_FixedSeq_Var_T<"
                                      : "TAO_VarSeq_Var_T<")
            << be_idt << be_idt_nl
            << node->local_name () << "," << be_nl
            << elem->nested_type_name (scope);

        /*if (nt == AST_Decl::NT_array)
          {
            *os << "_slice *";
            }*/

        *os << be_uidt_nl
            << ">" << be_uidt_nl
            << node->local_name () << "_var;" << be_uidt;

        *os << be_nl << be_nl
            << "typedef" << be_idt_nl
            << "TAO_Seq_Out_T<" << be_idt << be_idt_nl
            << node->local_name () << "," << be_nl
            << node->local_name () << "_var," << be_nl
            << elem->nested_type_name (scope);

        /*if (nt == AST_Decl::NT_array)
          {
            *os << "_slice *";
            }*/

        *os << be_uidt_nl
            << ">" << be_uidt_nl
            << node->local_name () << "_out;" << be_uidt;
      }

      break;
    }
}
