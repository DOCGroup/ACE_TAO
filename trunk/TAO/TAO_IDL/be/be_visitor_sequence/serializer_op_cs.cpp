// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    serializer_op_cs.cpp
//
// = DESCRIPTION
//    Visitor for code generation of Sequences for the
//    TAO::DCPS::Serializer operators in the client stubs.
//
// = AUTHOR
//    Scott Harris <harris_s@ociweb.com> based on code by Aniruddha Gokhale
//
// ============================================================================

// ***************************************************************************
// Sequence visitor for generating Serializer operator declarations in the client
// stubs file.
// ***************************************************************************

be_visitor_sequence_serializer_op_cs::be_visitor_sequence_serializer_op_cs (
    be_visitor_context *ctx
  )
  : be_visitor_decl (ctx)
{
}

be_visitor_sequence_serializer_op_cs::~be_visitor_sequence_serializer_op_cs (void)
{
}

int
be_visitor_sequence_serializer_op_cs::visit_sequence (be_sequence *node)
{
  if (this->ctx_->alias ())
    {
      // We are here because the base type of the sequence node is
      // itself a sequence i.e., this is a case of sequence of
      // typedef'd sequence. For the case of sequence of
      // anonymous sequence, see comment below.
      return this->visit_node (node);
    }

  if (node->cli_stub_serializer_op_gen ()
      || node->imported ()
      || node->is_local ())
    {
      return 0;
    }

  TAO_OutStream *os = this->ctx_->stream ();

  be_type *bt =
    be_type::narrow_from_decl (node->base_type ());

  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_sequence_serializer_op_cs::"
                         "visit_sequence - "
                         "Bad base type\n"),
                        -1);
    }

  // Generate the Serializer << and >> operator defns.

  // Save the sequence node for further use.
  this->ctx_->node (node);

  // If our element type is an anonymous sequence, generate code for it here.
  if (bt->node_type () == AST_Decl::NT_sequence)
    {
      int status =
          this->gen_anonymous_base_type (
              bt,
              TAO_CodeGen::TAO_ROOT_SERIALIZER_OP_CS
            );

      if (status == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_sequence_serializer_op_cs::"
                             "visit_sequence - "
                             "gen_anonymous_base_type failed\n"),
                            -1);
        }
    }


  if (be_global->gen_dcps_type_support_only ())
    {
      *os << be_nl << be_nl
          << "#endif /* end of disabling TAO specific code */"
          << be_nl << be_nl;
    }

  *os << be_nl << be_nl << "// TAO_IDL - Generated from" << be_nl
    << "// " << __FILE__ << ":" << __LINE__ << be_nl << be_nl;

  *os << "#if !defined _TAO_SERIALIZER_OP_"
      << node->flat_name () << "_CPP_" << be_nl
      << "#define _TAO_SERIALIZER_OP_" << node->flat_name () << "_CPP_"
      << be_nl << be_nl;

  // --- _tao_is_bounded_size ---
  this->ctx_->sub_state (TAO_CodeGen::TAO_IS_BOUNDED_SIZE);

  *os << "::CORBA::Boolean _tao_is_bounded_size (" << be_idt << be_idt_nl
      << "const " << node->name ()
      << " &_tao_sequence" << be_uidt_nl
      << ")" << be_uidt_nl
      << "{" << be_idt_nl;

  if (bt->node_type () == AST_Decl::NT_sequence)
    {
      this->visit_node (node);
    }
  else
    {
      if (bt->accept (this) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "be_visitor_sequence_serializer_op_cs::"
                             "visit_sequence - "
                             "Base type codegen failed\n"),
                            -1);
        }
    }

  *os << be_uidt_nl
      << "}" << be_nl << be_nl;

  // --- _dcps_max_marshaled_size ---

  this->ctx_->sub_state (TAO_CodeGen::TAO_MAX_MARSHALED_SIZE);

  *os << "size_t _dcps_max_marshaled_size (" << be_idt << be_idt_nl
      << "const " << node->name ()
      << " &_tao_sequence" << be_uidt_nl
      << ")" << be_uidt_nl
      << "{" << be_idt_nl;

  if (bt->node_type () == AST_Decl::NT_sequence)
    {
      this->visit_node (node);
    }
  else
    {
      if (bt->accept (this) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "be_visitor_sequence_serializer_op_cs::"
                             "visit_sequence - "
                             "Base type codegen failed\n"),
                            -1);
        }
    }

  *os << be_uidt_nl
      << "}" << be_nl << be_nl;

  // --- _dcps_find_size ---
  this->ctx_->sub_state (TAO_CodeGen::TAO_FIND_SIZE);

  *os << "size_t _dcps_find_size (" << be_idt << be_idt_nl
      << "const " << node->name ()
      << " &_tao_sequence" << be_uidt_nl
      << ")" << be_uidt_nl
      << "{" << be_idt_nl;

  if (bt->node_type () == AST_Decl::NT_sequence)
    {
      this->visit_node (node);
    }
  else
    {
      if (bt->accept (this) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "be_visitor_sequence_serializer_op_cs::"
                             "visit_sequence - "
                             "Base type codegen failed\n"),
                            -1);
        }
    }

  *os << be_uidt_nl
      << "}" << be_nl << be_nl;


  // --- operator<< ---

  //  Set the sub state as generating code for the output operator.
  this->ctx_->sub_state (TAO_CodeGen::TAO_CDR_OUTPUT);

  *os << "::CORBA::Boolean operator<< (" << be_idt << be_idt_nl
      << "TAO::DCPS::Serializer &strm," << be_nl
      << "const " << node->name ()
      << " &_tao_sequence" << be_uidt_nl
      << ")" << be_uidt_nl
      << "{" << be_idt_nl;

  // First encode the sequence length.
  *os << "const ::CORBA::ULong _tao_seq_len = _tao_sequence.length ();"
      << be_nl << be_nl;
  *os << "if (strm << _tao_seq_len)" << be_idt_nl
      << "{" << be_idt_nl;

  // Now encode the sequence elements.
  *os << "// Encode all elements." << be_nl;


  if (bt->node_type () == AST_Decl::NT_sequence)
    {
      this->visit_node (node);
    }
  else
    {
      if (bt->accept (this) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "be_visitor_sequence_serializer_op_cs::"
                             "visit_sequence - "
                             "Base type codegen failed\n"),
                            -1);
        }
    }

  *os << "}" << be_uidt_nl << be_nl
      << "return 0;" << be_uidt_nl
      << "}" << be_nl << be_nl;

  // --- operator>> ---

  //  Set the sub state as generating code for the input operator.
  this->ctx_->sub_state(TAO_CodeGen::TAO_CDR_INPUT);

  *os << "::CORBA::Boolean operator>> (" << be_idt << be_idt_nl
      << "TAO::DCPS::Serializer &";

  if (! bt->is_local ())
    {
      *os << "strm";
    }

  *os << "," << be_nl
      << node->name () << " &";

  if (! bt->is_local ())
    {
      *os << "_tao_sequence";
    }

  *os << be_uidt_nl
      << ")" << be_uidt_nl
      << "{" << be_idt_nl;

  if (! bt->is_local ())
    {
      // First retrieve the length and adjust the sequence length accordingly.
      *os << "::CORBA::ULong _tao_seq_len;" << be_nl << be_nl;
      *os << "if (strm >> _tao_seq_len)" << be_idt_nl
          << "{" << be_idt_nl;

      // Now check if the length does not exceed the maximum. We do this only
      // for bounded sequences
      AST_Expression *expr = node->max_size ();

      if (expr == 0 || (expr != 0 && expr->ev () == 0))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_sequence_serializer_op_cs::"
                             "visit_sequence - "
                             "bad sequence dimension\n"),
                            -1);
        }

      if (expr->ev ()->et == AST_Expression::EV_ulong)
        {
          if (expr->ev ()->u.ulval > 0)
            {
              // We are dealing with a bounded sequence. Check if we are within
              // bounds.
              *os << "if (_tao_seq_len <= _tao_sequence.maximum ())" << be_idt_nl
                  << "{" << be_idt_nl;
            }
        }
      else
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_sequence_serializer_op_cs::"
                             "visit_sequence - "
                             "bad sequence dimension value\n"),
                            -1);
        }

      *os << "// Set the length of the sequence." << be_nl
          << "_tao_sequence.length (_tao_seq_len);" << be_nl << be_nl;

      // Now we do a check for the sequence length to be non zero.
      // If length is 0 we return true.
      *os << "// If length is 0 we return true." << be_nl;
      *os << "if (0 >= _tao_seq_len) " << be_idt_nl
          << "{" << be_idt_nl;
      *os << "return 1;" << be_uidt_nl
          << "}" << be_uidt_nl << be_nl;

      *os << "// Retrieve all the elements." << be_nl;

      if (bt->node_type () == AST_Decl::NT_sequence)
        {
          this->visit_node (node);
        }
      else
        {
          if (bt->accept (this) == -1)
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "be_visitor_sequence_serializer_op_cs::"
                                 "visit_sequence - "
                                 "Base type codegen failed\n"),
                                -1);
            }
        }

      if (expr->ev ()->u.ulval > 0)
        {
          // We are dealing with a bounded sequence.
          *os << be_nl << "}" << be_uidt << be_uidt;
        }

      *os << be_nl << "}" << be_uidt_nl << be_nl;
    }

  *os << "return 0;" << be_uidt_nl
      << "}";

  *os << be_nl << be_nl
      << "#endif /* _TAO_SERIALIZER_OP_"
      << node->flat_name () << "_CPP_ */";

  node->cli_stub_serializer_op_gen (1);
  return 0;
}

int
be_visitor_sequence_serializer_op_cs::visit_array (be_array *node)
{
  return this->visit_node (node);
}

int
be_visitor_sequence_serializer_op_cs::visit_enum (be_enum *node)
{
  TAO_OutStream *os = this->ctx_->stream ();

  // We get here if the "type" of individual elements of the sequence is a
  // primitive type. In this case, we treat the sequence as a single
  // dimensional sequence (even of it was multi-dimensional), and pass
  // the total length of the sequence as a cross product of the
  // dimensions.

  be_sequence *sequence =
    be_sequence::narrow_from_decl (this->ctx_->node ());

  if (sequence == 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_sequence_serializer_op_cs::"
                         "visit_enum - "
                         "bad sequence node\n"),
                        -1);
    }

  switch (this->ctx_->sub_state ())
    {
    case TAO_CodeGen::TAO_IS_BOUNDED_SIZE:
        *os << "ACE_UNUSED_ARG(_tao_sequence);" << be_nl;
        if (sequence->unbounded ())
          *os << "return false; /* unbounded enum sequence */";
        else
          *os << "return true; /* bounded enum sequence */";
        break;
    case TAO_CodeGen::TAO_FIND_SIZE:
      *os << "//enum types are fixed size so OK to call max_marshaled_size"
          << be_nl;
    case TAO_CodeGen::TAO_MAX_MARSHALED_SIZE:
      *os << "//sequence of enum = ulong for length + ulong for enum * length"
          << be_nl;
      *os << "return _dcps_max_marshaled_size_ulong () + "
          << "_dcps_max_marshaled_size_ulong () * "
          << be_idt << be_idt_nl;
      switch (this->ctx_->sub_state ())
        {
        case TAO_CodeGen::TAO_FIND_SIZE:
          *os << "_tao_sequence.length();" << be_uidt << be_uidt;
          break;
        case TAO_CodeGen::TAO_MAX_MARSHALED_SIZE:
          *os << "_tao_sequence.maximum();" << be_uidt << be_uidt;
          break;
        default:
          ACE_ERROR_RETURN ((LM_ERROR,
                            "(%N:%l) be_visitor_array_serializer_op_cs::"
                            "visit_enum - "
                            "bad sub_state for seq length\n"),
                            -1);
        }
      break;
    default:
      return this->visit_node (node);
    }
  return 0;
}

int
be_visitor_sequence_serializer_op_cs::visit_interface (be_interface *node)
{
  return this->visit_node (node);
}

int
be_visitor_sequence_serializer_op_cs::visit_interface_fwd (be_interface_fwd *node)
{
  return this->visit_node (node);
}

int
be_visitor_sequence_serializer_op_cs::visit_component (be_component *node)
{
  return this->visit_node (node);
}

int
be_visitor_sequence_serializer_op_cs::visit_component_fwd (be_component_fwd *node)
{
  return this->visit_node (node);
}

int
be_visitor_sequence_serializer_op_cs::visit_home (be_home *node)
{
  return this->visit_node (node);
}

int
be_visitor_sequence_serializer_op_cs::visit_valuetype (be_valuetype *node)
{
  return this->visit_node (node);
}

int
be_visitor_sequence_serializer_op_cs::visit_valuetype_fwd (be_valuetype_fwd *node)
{
  return this->visit_node (node);
}

int
be_visitor_sequence_serializer_op_cs::visit_eventtype (be_eventtype *node)
{
  return this->visit_node (node);
}

int
be_visitor_sequence_serializer_op_cs::visit_eventtype_fwd (be_eventtype_fwd *node)
{
  return this->visit_node (node);
}

int
be_visitor_sequence_serializer_op_cs::visit_predefined_type (
    be_predefined_type *node
  )
{
  TAO_OutStream *os = this->ctx_->stream ();

  switch (node->pt ())
    {
    case AST_PredefinedType::PT_object:
    case AST_PredefinedType::PT_abstract:
    case AST_PredefinedType::PT_value:
    case AST_PredefinedType::PT_pseudo:
    case AST_PredefinedType::PT_any:
      return this->visit_node (node);
    case AST_PredefinedType::PT_void:
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_sequence_serializer_op_cs::"
                         "visit_predefined_type - "
                         "Bad primitive type\n"),
                        -1);
    default:
      break;
    };

  // We get here if the "type" of individual elements of the sequence is a
  // primitive type. In this case, we treat the sequence as a single
  // dimensional sequence (even of it was multi-dimensional), and pass
  // the total length of the sequence as a cross product of the
  // dimensions.

  be_sequence *sequence =
    be_sequence::narrow_from_decl (this->ctx_->node ());

  if (sequence == 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_sequence_serializer_op_cs::"
                         "visit_predefined_type - "
                         "bad sequence node\n"),
                        -1);
    }

  switch (this->ctx_->sub_state ())
    {
    case TAO_CodeGen::TAO_IS_BOUNDED_SIZE:
        *os << "ACE_UNUSED_ARG(_tao_sequence);" << be_nl;
        if (sequence->unbounded ())
          *os << "return false; /* seq<predef'd> */";
        else
          *os << "return true; /* seq<predef'd,N> */";
        break;
    case TAO_CodeGen::TAO_FIND_SIZE:
      *os << "//primitive types are fixed size so OK to call max_marshaled_size"
          << be_nl;
    case TAO_CodeGen::TAO_MAX_MARSHALED_SIZE:
      switch (node->pt ())
        {
        case AST_PredefinedType::PT_octet:
          *os << "return _dcps_max_marshaled_size_ulong () + "
              << "_dcps_max_marshaled_size_octet ()"
              << be_idt << be_idt_nl;
          break;
        case AST_PredefinedType::PT_char:
          *os << "return _dcps_max_marshaled_size_ulong () + "
              << "_dcps_max_marshaled_size_char ()"
              << be_idt << be_idt_nl;
          break;
        case AST_PredefinedType::PT_wchar:
          *os << "return _dcps_max_marshaled_size_ulong () + "
              << "_dcps_max_marshaled_size_wchar ()"
              << be_idt << be_idt_nl;
          break;
        case AST_PredefinedType::PT_long:
        case AST_PredefinedType::PT_ulong:
        case AST_PredefinedType::PT_short:
        case AST_PredefinedType::PT_ushort:
        case AST_PredefinedType::PT_float:
        case AST_PredefinedType::PT_double:
        case AST_PredefinedType::PT_longlong:
        case AST_PredefinedType::PT_ulonglong:
        case AST_PredefinedType::PT_longdouble:
          *os << "//sequence is of a fixed sized predefined type = "
              << "sizeof(ULong) + type size * length" << be_nl;
          *os << node->name () << " a_base_value;" << be_nl;
          *os << "return _dcps_max_marshaled_size_ulong () + "
              << "_dcps_max_marshaled_size (a_base_value)"
              << be_idt << be_idt_nl;
          break;
          break;
        case AST_PredefinedType::PT_boolean:
          *os << "return _dcps_max_marshaled_size_ulong () + "
              << "_dcps_max_marshaled_size_boolean ()"
              << be_idt << be_idt_nl;
          break;
        default:
          ACE_ERROR_RETURN ((LM_ERROR,
                            "(%N:%l) be_visitor_array_serializer_op_cs::"
                            "visit_predefined_type - "
                            "bad primitive type for _dcps_max_marshaled_size code gen\n"),
                            -1);
        }

      switch (this->ctx_->sub_state ())
        {
        case TAO_CodeGen::TAO_FIND_SIZE:
          *os << " * _tao_sequence.length();" << be_uidt << be_uidt;
          break;
        case TAO_CodeGen::TAO_MAX_MARSHALED_SIZE:
          *os << " * _tao_sequence.maximum();" << be_uidt << be_uidt;
          break;
        default:
          ACE_ERROR_RETURN ((LM_ERROR,
                            "(%N:%l) be_visitor_array_serializer_op_cs::"
                            "visit_predefined_type - "
                            "bad sub_state for seq length\n"),
                            -1);
        }
      break;
      case TAO_CodeGen::TAO_CDR_INPUT:
      case TAO_CodeGen::TAO_CDR_OUTPUT:
        {
  /*
   * Avoiding the work to make TAO::DCPS::Serializer handle the
   * TAO_NO_COPY_OCTET_SEQUENCES optimization.
   * TBD - support this optimization -- see corresponding CDR code.
   */
  *os << "return strm.";

  // Based on our substate, we may be reading from a stream or writing into a
  // stream/
  switch (this->ctx_->sub_state ())
    {
    case TAO_CodeGen::TAO_CDR_INPUT:
      *os << "read_";
      break;
    case TAO_CodeGen::TAO_CDR_OUTPUT:
      *os << "write_";
      break;
    default:
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_sequence_serializer_op_cs::"
                         "visit_predefined_type - "
                         "bad sub state\n"),
                        -1);
    }

  // Determine what kind of sequence are we reading/writing.
  switch (node->pt ())
    {
    case AST_PredefinedType::PT_long:
      *os << "long_array";
      break;
    case AST_PredefinedType::PT_ulong:
      *os << "ulong_array";
      break;
    case AST_PredefinedType::PT_short:
      *os << "short_array";
      break;
    case AST_PredefinedType::PT_ushort:
      *os << "ushort_array";
      break;
    case AST_PredefinedType::PT_octet:
      *os << "octet_array";
      break;
    case AST_PredefinedType::PT_char:
      *os << "char_array";
      break;
    case AST_PredefinedType::PT_wchar:
      *os << "wchar_array";
      break;
    case AST_PredefinedType::PT_float:
      *os << "float_array";
      break;
    case AST_PredefinedType::PT_double:
      *os << "double_array";
      break;
    case AST_PredefinedType::PT_longlong:
      *os << "longlong_array";
      break;
    case AST_PredefinedType::PT_ulonglong:
      *os << "ulonglong_array";
      break;
    case AST_PredefinedType::PT_longdouble:
      *os << "longdouble_array";
      break;
    case AST_PredefinedType::PT_boolean:
      *os << "boolean_array";
      break;
    default:
      // error
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_sequence_serializer_op_cs::"
                         "visit_predefined_type - "
                         "bad primitive type for optimized code gen\n"),
                        -1);
    }

  // Handle special case to avoid compiler errors.
  switch (node->pt ())
    {
    case AST_PredefinedType::PT_char:
      switch (this->ctx_->sub_state ())
        {
        case TAO_CodeGen::TAO_CDR_INPUT:
          *os << " ((char *)_tao_sequence.get_buffer (), ";
          break;
        case TAO_CodeGen::TAO_CDR_OUTPUT:
          *os << " ((const char *)_tao_sequence.get_buffer (), ";
          break;
        default:
          // error
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_sequence_serializer_op_cs"
                             "::visit_predefined_type - "
                             "bad codegen substate\n"),
                            -1);
        }
      break;
    default:
      *os << " (_tao_sequence.get_buffer (), ";
      break;
    }

  *os << "_tao_sequence.length ());" << be_uidt_nl;

      } // end TAO_CDR_INPUT/OUTPUT case
      break;
    default:
      ACE_ERROR_RETURN ((LM_ERROR,
                          "(%N:%l) be_visitor_sequence_serializer_op_cs::"
                          "visit_predefined_type - outter "
                          "bad sub state\n"),
                        -1);
    } // end outter switch

  return 0;
}

int
be_visitor_sequence_serializer_op_cs::visit_string (be_string *node)
{
  return this->visit_node (node);
}

int
be_visitor_sequence_serializer_op_cs::visit_structure (be_structure *node)
{
  return this->visit_node (node);
}

int
be_visitor_sequence_serializer_op_cs::visit_union (be_union *node)
{
  return this->visit_node (node);
}

int
be_visitor_sequence_serializer_op_cs::visit_typedef (be_typedef *node)
{
  this->ctx_->alias (node);
  be_type *bt = node->primitive_base_type ();

  if (!bt || (bt->accept (this) == -1))
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_sequence_serializer_op_cs::"
                         "visit_typedef - "
                         "Bad primitive type\n"),
                        -1);
    }

  this->ctx_->alias (0);
  return 0;
}

int
be_visitor_sequence_serializer_op_cs::visit_node (be_type *bt)
{
  TAO_OutStream *os = this->ctx_->stream ();
  be_sequence *node =
    be_sequence::narrow_from_decl (this->ctx_->node ());

  if (node == 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_sequence_serializer_op_cs::"
                         "visit_node - "
                         "bad sequence node\n"),
                        -1);
    }

  be_visitor_context ctx (*this->ctx_);
  be_visitor_sequence_base visitor (&ctx);

  // We get here if the "type" of individual elements of the sequence is not a
  // primitive type. In this case, we are left with no other alternative but
  // to encode/decode element by element.

  AST_Expression *expr = node->max_size ();

  if (expr == 0 || (expr != 0 && expr->ev () == 0))
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_sequence_serializer_op_cs::"
                         "visit_node - "
                         "bad sequence dimension\n"),
                        -1);
    }

  AST_Decl::NodeType nt = bt->node_type ();
  be_typedef *td = 0;

  if (nt == AST_Decl::NT_typedef)
    {
      td = be_typedef::narrow_from_decl (bt);
      nt = td->base_node_type ();
    }

switch (this->ctx_->sub_state ())
    {
    case TAO_CodeGen::TAO_IS_BOUNDED_SIZE:
     if (node->unbounded ())
       {
         *os << "ACE_UNUSED_ARG(_tao_sequence);" << be_nl;
         *os << "return false; /* unbounded sequence */";
       }
     else

      switch (nt)
        {
        case AST_Decl::NT_enum:
          *os << "ACE_UNUSED_ARG(_tao_sequence);" << be_nl;
          *os << "return true; // bounded seqence of enums";
          break;
        case AST_Decl::NT_union:
          *os << "ACE_UNUSED_ARG(_tao_sequence);" << be_nl;
          *os << "// Always return false for a union because " << be_nl
              << "// _dcps_max_marshaled_size may produce an access violation."
              << be_nl
              << "return false; // seq of union " << bt->name () << be_uidt;
          break;
        case AST_Decl::NT_sequence:
        case AST_Decl::NT_struct:
          // its a type that should be defined in the generated code.
          *os << "ACE_UNUSED_ARG(_tao_sequence);" << be_nl;
          *os << bt->name () << " a_base_value;" << be_nl;
          *os << "return _tao_is_bounded_size (a_base_value);";
          break;
        case AST_Decl::NT_array:
          *os << "ACE_UNUSED_ARG(_tao_sequence);" << be_nl;
          *os << bt->name () << " a_base_value;" << be_nl;
          *os << bt->name () << "_forany tmp(a_base_value);" << be_nl;
          *os << "return _tao_is_bounded_size (tmp);" << be_uidt_nl;
          break;
        case AST_Decl::NT_string:
        case AST_Decl::NT_wstring:
          {
            be_string *str =
              be_string::narrow_from_decl (
                  td == 0 ? bt : td->primitive_base_type ()
                );

            if (!str)
              {
                ACE_ERROR_RETURN ((LM_ERROR,
                                   "(%N:%l) be_visitor_sequence_serializer_op_cs::"
                                   "visit_node - "
                                   "bad string node\n"),
                                  -1);
              }

            *os << "ACE_UNUSED_ARG(_tao_sequence);" << be_nl;
            if (str->max_size ()->ev ()->u.ulval == 0)
              {
                 // Unbounded string
                 *os << "return false; /* bounded seq of unbounded strings */";
              }
            else
              {
                 *os << "return true; /* bounded seq of bounded strings */";
              }
          }
          break;
        default:
          ACE_ERROR_RETURN ((LM_ERROR,
                              "(%N:%l) be_visitor_sequence_serializer_op_cs::"
                              "visit_node - is_bounded_size - "
                              "unsupported sequence base type\n"),
                            -1);
          break;
        }
      break;
    case TAO_CodeGen::TAO_MAX_MARSHALED_SIZE:
      switch (nt)
        {
        case AST_Decl::NT_enum:
          *os << "// sequence of enums" << be_nl
              << "return _dcps_max_marshaled_size_ulong () + " << be_idt_nl
              << "_tao_sequence.maximum() * " << be_nl
              << "_dcps_max_marshaled_size_ulong ();" << be_uidt;
          break;
        case AST_Decl::NT_union:
          *os << "ACE_UNUSED_ARG(_tao_sequence);" << be_nl;
          *os << "// do not try getting the _dcps_max_marshaled_size" << be_nl
              << "// doing so may produce an access violation" << be_nl
              << "return 100000; // seq of union " << bt->name () << be_uidt;
          break;
        case AST_Decl::NT_sequence:
        case AST_Decl::NT_struct:
          // its a type that should be defined in the generated code.
          *os << bt->name () << " a_base_value;" << be_nl
              << "return _dcps_max_marshaled_size_ulong () + " << be_idt_nl
              << "_tao_sequence.maximum() * " << be_nl
              << "_dcps_max_marshaled_size (a_base_value);" << be_uidt;
          break;
        case AST_Decl::NT_array:
          *os << bt->name () << " a_base_value;" << be_nl;
          *os << bt->name() << "_forany tmp(a_base_value);" << be_nl;
          *os << "return _dcps_max_marshaled_size_ulong () + " << be_idt_nl
              << "_tao_sequence.maximum() * " << be_nl;
          *os << "_dcps_max_marshaled_size (tmp);" << be_uidt_nl;

          break;
        case AST_Decl::NT_string:
        case AST_Decl::NT_wstring:
          {
            be_string *str =
              be_string::narrow_from_decl (
                  td == 0 ? bt : td->primitive_base_type ()
                );

            if (!str)
              {
                ACE_ERROR_RETURN ((LM_ERROR,
                                   "(%N:%l) be_visitor_sequence_serializer_op_cs::"
                                   "visit_node - "
                                   "bad string node\n"),
                                  -1);
              }

            u_long max_size = str->max_size ()->ev ()->u.ulval;
            if (max_size == 0)
                 *os << "// bounded seq of unbounded strings" << be_nl;
            else
                 *os << "// bounded seq of bounded strings" << be_nl;

            char buff[15];
            ACE_OS::sprintf(buff, "%ld", max_size);
            *os << "return _dcps_max_marshaled_size_ulong () + " << be_nl
                << " _tao_sequence.maximum() * ";
            *os << "(_dcps_max_marshaled_size_ulong () + " << buff << ");";

          }
          break;
        case AST_Decl::NT_interface:
          *os << "//DCPS - Sequence of interfaces is not supported" << be_nl;
          *os << "return 77777;" << be_nl;
          break;
        default:
          ACE_ERROR_RETURN ((LM_ERROR,
                              "(%N:%l) be_visitor_sequence_serializer_op_cs::"
                              "visit_node - is_bounded_size - "
                              "unsupported sequence base type\n"),
                            -1);
          break;
        }
      break;
    case TAO_CodeGen::TAO_FIND_SIZE:
      {
        *os << "//iterate over the sequence to find the current size" << be_nl
            << "size_t the_length = _dcps_max_marshaled_size_ulong ();" << be_nl
            << "for ( ::CORBA::ULong i = 0; i < _tao_sequence.length(); "
            << "++i)" << be_idt_nl
            << "{" << be_idt_nl;

      switch (nt)
        {
        case AST_Decl::NT_enum:
          *os << "the_length += _dcps_max_marshaled_size_ulong (); // enum" << be_uidt_nl;
          break;
        case AST_Decl::NT_union:
        case AST_Decl::NT_sequence:
        case AST_Decl::NT_struct:
          // its a type that should be defined in the generated code.
          *os << "the_length += _dcps_find_size (_tao_sequence[i]);" << be_uidt_nl;
          break;
        case AST_Decl::NT_array:
          if (bt->accept (&visitor) == -1)
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "(%N:%l) be_visitor_sequence_serializer_op_cs::"
                                 "visit_node - "
                                 "base type visit failed\n"),
                                -1);
            }

          *os << "_var tmp_var (" << be_idt << be_idt_nl;

          if (bt->accept (&visitor) == -1)
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "(%N:%l) be_visitor_sequence_serializer_op_cs::"
                                 "visit_node - "
                                 "base type visit failed\n"),
                                -1);
            }

          *os << "_dup (_tao_sequence[i])" << be_uidt_nl
              << ");" << be_uidt_nl;

          if (bt->accept (&visitor) == -1)
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "(%N:%l) be_visitor_sequence_serializer_op_cs::"
                                 "visit_node - "
                                 "base type visit failed\n"),
                                -1);
            }

          *os << "_forany tmp (tmp_var.inout ());" << be_nl;
          *os << "the_length += _dcps_find_size (tmp);" << be_uidt_nl;

          break;
        case AST_Decl::NT_string:
          *os << "the_length += _dcps_max_marshaled_size_ulong () " << be_nl
            << "+ (_tao_sequence[i] == 0 ? 0 : ACE_OS::strlen (_tao_sequence[i]));"
            << be_uidt_nl;
          break;
        case AST_Decl::NT_wstring:
          *os << "the_length += _dcps_max_marshaled_size_ulong () " << be_nl
            << "+ (_tao_sequence[i] == 0 ? 0 : ACE_OS::strlen (_tao_sequence[i]) * sizeof (CORBA::WChar));"
            << be_uidt_nl;
          break;
        case AST_Decl::NT_interface:
          *os << "//DCPS - Sequence of interfaces is not supported" << be_nl;
          break;
        default:
          ACE_ERROR_RETURN ((LM_ERROR,
                              "(%N:%l) be_visitor_sequence_serializer_op_cs::"
                              "visit_node - is_bounded_size - "
                              "unsupported sequence base type\n"),
                            -1);
          break;
        }


        *os << "}" << be_uidt_nl
            << "return the_length;";
      }
      break;
    case TAO_CodeGen::TAO_CDR_INPUT:
    case TAO_CodeGen::TAO_CDR_OUTPUT:
      {
    // Initialize a boolean variable.
  *os << "::CORBA::Boolean _tao_marshal_flag = true;" << be_nl << be_nl;


  if (expr->ev ()->et == AST_Expression::EV_ulong)
    {
      *os << "for ( ::CORBA::ULong i = 0; i < _tao_seq_len"
          << " && _tao_marshal_flag; "
          << "++i)" << be_idt_nl
          << "{" << be_idt_nl;
    }
  else
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_sequence_serializer_op_cs::"
                         "visit_node - "
                         "bad sequence dimension value\n"),
                        -1);
    }

  switch (this->ctx_->sub_state ())
    {
    case TAO_CodeGen::TAO_CDR_INPUT:
      switch (nt)
        {
        case AST_Decl::NT_array:
          if (bt->accept (&visitor) == -1)
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "(%N:%l) be_visitor_sequence_serializer_op_cs::"
                                 "visit_node - "
                                 "base type visit failed\n"),
                                -1);
            }

          *os << "_forany tmp (";

          if (bt->accept (&visitor) == -1)
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "(%N:%l) be_visitor_sequence_serializer_op_cs::"
                                 "visit_node - "
                                 "base type visit failed\n"),
                                -1);
            }

          *os << "_alloc ());" << be_nl;
          *os << "_tao_marshal_flag = (strm >> tmp);" << be_nl;

          if (bt->accept (&visitor) == -1)
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "(%N:%l) be_visitor_sequence_serializer_op_cs::"
                                 "visit_node - "
                                 "base type visit failed\n"),
                                -1);
            }

          *os << "_copy (_tao_sequence[i], tmp.in ());" << be_nl;

          if (bt->accept (&visitor) == -1)
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "(%N:%l) be_visitor_sequence_serializer_op_cs::"
                                 "visit_node - "
                                 "base type visit failed\n"),
                                -1);
            }

          *os << "_free (tmp.inout ());" << be_uidt_nl;

          break;
        case AST_Decl::NT_string:
        case AST_Decl::NT_wstring:
          {
            be_string *str =
              be_string::narrow_from_decl (
                  td == 0 ? bt : td->primitive_base_type ()
                );

            if (!str)
              {
                ACE_ERROR_RETURN ((LM_ERROR,
                                   "(%N:%l) be_visitor_sequence_serializer_op_cs::"
                                   "visit_node - "
                                   "bad string node\n"),
                                  -1);
              }

            if (str->max_size ()->ev ()->u.ulval == 0)
              {
                // Unbounded.
                *os << node->name ()
                    << "::value_type * buffer = _tao_sequence.get_buffer();"
                    << be_idt_nl;;
                *os << "_tao_marshal_flag = (strm >> buffer[i]);"
                    << be_uidt_nl;
              }
            else
              {
                if (str->width () == (long) sizeof (char))
                  {
                    *os << "_tao_marshal_flag = (strm >> "
                        << "::ACE_InputCDR::to_string (_tao_sequence[i].out (), ";
                  }
                else
                  {
                    *os << "_tao_marshal_flag = (strm >> "
                        << "::ACE_InputCDR::to_wstring (_tao_sequence[i].out (), ";
                  }

                *os << str->max_size ()->ev ()->u.ulval << "));"
                    << be_uidt_nl;
              }
          }
          break;
        case AST_Decl::NT_interface:
        case AST_Decl::NT_interface_fwd:
        case AST_Decl::NT_valuetype:
        case AST_Decl::NT_valuetype_fwd:
        case AST_Decl::NT_eventtype:
        case AST_Decl::NT_eventtype_fwd:
        case AST_Decl::NT_valuebox:
          *os << "//DCPS does not support interfaces and value types";

          break;
        case AST_Decl::NT_pre_defined:
          {
            // We need to separately handle this case of pseudo objects
            // because they have a _var type.
            be_predefined_type *pt =
              be_predefined_type::narrow_from_decl (
                  td == 0 ? bt : td->primitive_base_type ()
                );

            if (!pt)
              {
                ACE_ERROR_RETURN ((LM_ERROR,
                                   "(%N:%l) be_visitor_sequence_serializer_op_cs::"
                                   "visit_node - "
                                   "bad predefined type node\n"),
                                  -1);
              }

            switch (pt->pt ())
              {
                case AST_PredefinedType::PT_pseudo:
                case AST_PredefinedType::PT_abstract:
                case AST_PredefinedType::PT_object:
                case AST_PredefinedType::PT_value:
                  *os << "_tao_marshal_flag = (strm >> _tao_sequence[i].out ());";
                  break;
                case AST_PredefinedType::PT_char:
                  *os << "_tao_marshal_flag =" << be_idt_nl
                      << "(strm >> ::ACE_InputCDR::to_char (_tao_sequence[i]));"
                      << be_uidt;
                  break;
                case AST_PredefinedType::PT_wchar:
                  *os << "_tao_marshal_flag =" << be_idt_nl
                      << "(strm >> ::ACE_InputCDR::to_wchar (_tao_sequence[i]));"
                      << be_uidt;
                  break;
                case AST_PredefinedType::PT_boolean:
                  *os << "_tao_marshal_flag =" << be_idt_nl
                      << "(strm >> ::ACE_InputCDR::to_boolean (_tao_sequence[i]));"
                      << be_uidt;
                  break;
                case AST_PredefinedType::PT_octet:
                  *os << "_tao_marshal_flag =" << be_idt_nl
                      << "(strm >> ::ACE_InputCDR::to_octet (_tao_sequence[i]));"
                      << be_uidt;
                  break;
                default:
                  *os << "_tao_marshal_flag = (strm >> _tao_sequence[i]);";
                  break;
              }
          }

          break;
        default:
          *os << "_tao_marshal_flag = (strm >> _tao_sequence[i]);";

          break;
        }

      *os << be_uidt_nl
          << "}";

      break;
    case TAO_CodeGen::TAO_CDR_OUTPUT:
      switch (nt)
        {
        case AST_Decl::NT_array:
          if (bt->accept (&visitor) == -1)
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "(%N:%l) be_visitor_sequence_serializer_op_cs::"
                                 "visit_node - "
                                 "base type visit failed\n"),
                                -1);
            }

          *os << "_var tmp_var (" << be_idt << be_idt_nl;

          if (bt->accept (&visitor) == -1)
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "(%N:%l) be_visitor_sequence_serializer_op_cs::"
                                 "visit_node - "
                                 "base type visit failed\n"),
                                -1);
            }

          *os << "_dup (_tao_sequence[i])" << be_uidt_nl
              << ");" << be_uidt_nl;

          if (bt->accept (&visitor) == -1)
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "(%N:%l) be_visitor_sequence_serializer_op_cs::"
                                 "visit_node - "
                                 "base type visit failed\n"),
                                -1);
            }

          *os << "_forany tmp (tmp_var.inout ());" << be_nl;
          *os << "_tao_marshal_flag = (strm << tmp);";

          break;
        case AST_Decl::NT_interface:
        case AST_Decl::NT_interface_fwd:
          *os << "//DCPS does not support interfaces";

          break;
        case AST_Decl::NT_string:
        case AST_Decl::NT_wstring:
          *os << "_tao_marshal_flag = (strm << _tao_sequence[i]);";

          break;
        case AST_Decl::NT_valuetype:
        case AST_Decl::NT_valuetype_fwd:
        case AST_Decl::NT_eventtype:
        case AST_Decl::NT_eventtype_fwd:
        case AST_Decl::NT_valuebox:
          *os << "//DCPS does not support value types";

          break;
        case AST_Decl::NT_pre_defined:
          {
            //SHH harris_s@ociweb.com - I am not sure you can every get here
            //   predefined types are handled by be_visitor_sequence_serializer_op_cs::visit_predefined_type
            be_predefined_type *pt =
              be_predefined_type::narrow_from_decl (
                  td == 0 ? bt : td->primitive_base_type ()
                );

            if (!pt)
              {
                ACE_ERROR_RETURN ((LM_ERROR,
                                   "(%N:%l) be_visitor_sequence_serializer_op_cs::"
                                   "visit_node - "
                                   "bad predefined type node\n"),
                                  -1);
              }

            switch (pt->pt ())
              {
                case AST_PredefinedType::PT_pseudo:
                case AST_PredefinedType::PT_abstract:
                case AST_PredefinedType::PT_object:
                case AST_PredefinedType::PT_value:
                  *os << "_tao_marshal_flag = (strm << _tao_sequence[i].in ());";
                  break;
                case AST_PredefinedType::PT_char:
                  *os << "_tao_marshal_flag =" << be_idt_nl
                      << "(strm << ::ACE_OutputCDR::from_char (_tao_sequence[i]));"
                      << be_uidt;
                  break;
                case AST_PredefinedType::PT_wchar:
                  *os << "_tao_marshal_flag =" << be_idt_nl
                      << "(strm << ::ACE_OutputCDR::from_wchar (_tao_sequence[i]));"
                      << be_uidt;
                  break;
                case AST_PredefinedType::PT_boolean:
                  *os << "_tao_marshal_flag =" << be_idt_nl
                      << "(strm << ::ACE_OutputCDR::from_boolean (_tao_sequence[i]));"
                      << be_uidt;
                  break;
                case AST_PredefinedType::PT_octet:
                  *os << "_tao_marshal_flag =" << be_idt_nl
                      << "(strm << ::ACE_OutputCDR::from_octet (_tao_sequence[i]));"
                      << be_uidt;
                  break;
                default:
                  *os << "_tao_marshal_flag = (strm << _tao_sequence[i]);";
                  break;
              }
          }

          break;
        default:
          *os << "_tao_marshal_flag = (strm << _tao_sequence[i]);";

          break;
        }

      *os << be_uidt_nl
          << "}";

      break;
    default:
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_sequence_serializer_op_cs::"
                         "visit_node - "
                         "bad sub state\n"),
                        -1);
    }

  *os << be_uidt_nl << be_nl;
  *os << "return _tao_marshal_flag;" << be_uidt_nl;
  }
  break;
    default:
      // Error.
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_sequence_serializer_op_cs::"
                         "visit_node - "
                         "bad sub state\n"),
                        -1);
    break;
} //outer switch


  return 0;
}
