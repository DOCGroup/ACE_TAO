// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    cdr_op_cs.cpp
//
// = DESCRIPTION
//    Visitor for code generation of Sequences for the CDR operators
//    in the client stubs.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

ACE_RCSID (be_visitor_sequence,
           cdr_op_cs,
           "$Id$")

// ***************************************************************************
// Sequence visitor for generating CDR operator declarations in the client
// stubs file.
// ***************************************************************************

be_visitor_sequence_cdr_op_cs::be_visitor_sequence_cdr_op_cs (
    be_visitor_context *ctx
  )
  : be_visitor_decl (ctx)
{
}

be_visitor_sequence_cdr_op_cs::~be_visitor_sequence_cdr_op_cs (void)
{
}

int
be_visitor_sequence_cdr_op_cs::visit_sequence (be_sequence *node)
{
  if (this->ctx_->alias ())
    {
      // We are here because the base type of the sequence node is
      // itself a sequence i.e., this is a case of sequence of
      // typedef'd sequence. For the case of sequence of
      // anonymous sequence, see comment below.
      return this->visit_node (node);
    }

  if (node->cli_stub_cdr_op_gen ()
      || node->imported ())
    {
      return 0;
    }

  TAO_OutStream *os = this->ctx_->stream ();

  be_type *bt =
    be_type::narrow_from_decl (node->base_type ());

  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_sequence_cdr_op_cs::"
                         "visit_sequence - "
                         "Bad base type\n"),
                        -1);
    }

  // Generate the CDR << and >> operator defns.

  // Save the sequence node for further use.
  this->ctx_->node (node);

  // If our element type is an anonymous sequence, generate code for it here.
  if (bt->node_type () == AST_Decl::NT_sequence)
    {
      int status = 
          this->gen_anonymous_base_type (
              bt,
              TAO_CodeGen::TAO_ROOT_CDR_OP_CS
            );

      if (status == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_array_cdr_op_cs::"
                             "visit_sequence - "
                             "gen_anonymous_base_type failed\n"),
                            -1);
        }
/*
      if (bt->accept (this) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "be_visitor_sequence_cdr_op_cs::"
                             "visit_sequence - "
                             "Base type codegen failed\n"),
                            -1);
        }
*/
    }

  *os << be_nl << be_nl << "// TAO_IDL - Generated from" << be_nl
    << "// " << __FILE__ << ":" << __LINE__ << be_nl << be_nl;

  *os << "#if !defined _TAO_CDR_OP_"
      << node->flat_name () << "_CPP_" << be_nl
      << "#define _TAO_CDR_OP_" << node->flat_name () << "_CPP_"
      << be_nl << be_nl;

  //  Set the sub state as generating code for the output operator.
  this->ctx_->sub_state (TAO_CodeGen::TAO_CDR_OUTPUT);

  *os << "CORBA::Boolean operator<< (" << be_idt << be_idt_nl
      << "TAO_OutputCDR &strm," << be_nl
      << "const " << node->name ()
      << " &_tao_sequence" << be_uidt_nl
      << ")" << be_uidt_nl
      << "{" << be_idt_nl;

  // First encode the sequence length.
  *os << "CORBA::ULong _tao_seq_len = _tao_sequence.length ();"
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
                             "be_visitor_sequence_cdr_op_cs::"
                             "visit_sequence - "
                             "Base type codegen failed\n"),
                            -1);
        }
    }

  *os << "}" << be_uidt_nl << be_nl
      << "return 0;" << be_uidt_nl
      << "}" << be_nl << be_nl;

  //  Set the sub state as generating code for the input operator.
  this->ctx_->sub_state(TAO_CodeGen::TAO_CDR_INPUT);

  *os << "CORBA::Boolean operator>> (" << be_idt << be_idt_nl
      << "TAO_InputCDR &";
      
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
      *os << "CORBA::ULong _tao_seq_len;" << be_nl << be_nl;
      *os << "if (strm >> _tao_seq_len)" << be_idt_nl
          << "{" << be_idt_nl;

      // Add a sanity check for the length of a sequence.
      *os << "// Add a check to the length of the sequence" << be_nl;
      *os << "// to make sure it does not exceed the length" << be_nl;
      *os << "// of the stream. (See bug 58.)" << be_nl;
      *os << "if (_tao_seq_len > strm.length ())" << be_idt_nl
          << "{" << be_idt_nl;
      *os << "return 0;" << be_uidt_nl
          << "}" << be_uidt_nl << be_nl;

      // Now check if the length does not exceed the maximum. We do this only
      // for bounded sequences
      AST_Expression *expr = node->max_size ();

      if (expr == 0 || (expr != 0 && expr->ev () == 0))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_sequence_cdr_op_cs::"
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
                             "(%N:%l) be_visitor_sequence_cdr_op_cs::"
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
                                 "be_visitor_sequence_cdr_op_cs::"
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
      << "#endif /* _TAO_CDR_OP_"
      << node->flat_name () << "_CPP_ */";

  node->cli_stub_cdr_op_gen (1);
  return 0;
}

int
be_visitor_sequence_cdr_op_cs::visit_array (be_array *node)
{
  return this->visit_node (node);
}

int
be_visitor_sequence_cdr_op_cs::visit_enum (be_enum *node)
{
  return this->visit_node (node);
}

int
be_visitor_sequence_cdr_op_cs::visit_interface (be_interface *node)
{
  return this->visit_node (node);
}

int
be_visitor_sequence_cdr_op_cs::visit_interface_fwd (be_interface_fwd *node)
{
  return this->visit_node (node);
}

int
be_visitor_sequence_cdr_op_cs::visit_valuetype (be_valuetype *node)
{
  return this->visit_node (node);
}

int
be_visitor_sequence_cdr_op_cs::visit_valuetype_fwd (be_valuetype_fwd *node)
{
  return this->visit_node (node);
}

int
be_visitor_sequence_cdr_op_cs::visit_predefined_type (
    be_predefined_type *node
  )
{
  TAO_OutStream *os = this->ctx_->stream ();

  switch (node->pt ())
    {
    case AST_PredefinedType::PT_object:
    case AST_PredefinedType::PT_value:
    case AST_PredefinedType::PT_pseudo:
    case AST_PredefinedType::PT_any:
      return this->visit_node (node);
    case AST_PredefinedType::PT_void:
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_sequence_cdr_op_cs::"
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

  be_sequence *sequence = this->ctx_->be_node_as_sequence ();

  if (!sequence)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_sequence_cdr_op_cs::"
                         "visit_predefined_type - "
                         "bad sequence node\n"),
                        -1);
    }

  // Handle octet sequences using the optimizations provided by the TAO ORB
  // Core. If these optimizations are not available, then use the normal form.

  if (node->pt () == AST_PredefinedType::PT_octet && sequence->unbounded ())
    {
      *os << "\n#if (TAO_NO_COPY_OCTET_SEQUENCES == 1)" << be_nl;

      switch (this->ctx_->sub_state ())
        {
        case TAO_CodeGen::TAO_CDR_INPUT:
          {
            *os << "if (ACE_BIT_DISABLED (strm.start ()->flags ()," << be_nl
                << "ACE_Message_Block::DONT_DELETE))" << be_nl
                << "{" << be_idt_nl
                <<  "TAO_ORB_Core* orb_core = strm.orb_core ();" << be_nl
                << "if (orb_core != 0 &&" << be_nl
                << "strm.orb_core ()->resource_factory ()->" << be_nl
                << "input_cdr_allocator_type_locked () == 1)" << be_nl
                << "{" << be_idt_nl
                << "TAO_Unbounded_Sequence<CORBA::Octet> *oseq = " << be_nl
                << "  ACE_static_cast(TAO_Unbounded_Sequence<CORBA::Octet>*, "
                << "&_tao_sequence);" << be_nl
                << "oseq->replace (_tao_seq_len, strm.start ());"
                << be_nl
                << "oseq->mb ()->wr_ptr (oseq->mb()->rd_ptr () + "
                << "_tao_seq_len);" << be_nl
                << "strm.skip_bytes (_tao_seq_len);" << be_nl
                << "return 1;" << be_uidt_nl
                << "}" << be_uidt_nl
                << "}" << be_nl
                << "return strm.read_octet_array ("
                << "_tao_sequence.get_buffer (), _tao_seq_len);";
          }
          break;
        case TAO_CodeGen::TAO_CDR_OUTPUT:
          {
            *os << "{" << be_idt_nl
                << "TAO_Unbounded_Sequence<CORBA::Octet> *oseq = " << be_nl
                << "  ACE_static_cast (TAO_Unbounded_Sequence<CORBA::Octet>*, "
                << "(" << sequence->name () << " *)&_tao_sequence);" << be_nl
                << "if (oseq->mb ())" << be_idt_nl
                << "return strm.write_octet_array_mb (oseq->mb ());"
                << be_uidt_nl
                << "else" << be_idt_nl
                << "return strm.write_octet_array ("
                << "_tao_sequence.get_buffer (), _tao_sequence.length ());"
                << be_uidt << be_uidt_nl
                << "}" << be_nl;
          }
          break;
        default:
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_sequence_cdr_op_cs::"
                             "visit_predefined_type - "
                             "bad sub state\n"),
                            -1);
        }

      *os << "\n#else /* TAO_NO_COPY_OCTET_SEQUENCES == 0 */" << be_nl;
    }

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
                         "(%N:%l) be_visitor_sequence_cdr_op_cs::"
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
                         "(%N:%l) be_visitor_sequence_cdr_op_cs::"
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
                             "(%N:%l) be_visitor_sequence_cdr_op_cs"
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

  if (node->pt () == AST_PredefinedType::PT_octet && sequence->unbounded ())
    {
      *os << "\n#endif /* TAO_NO_COPY_OCTET_SEQUENCES == 0 */" << be_nl;
    }

  return 0;
}

int
be_visitor_sequence_cdr_op_cs::visit_string (be_string *node)
{
  return this->visit_node (node);
}

int
be_visitor_sequence_cdr_op_cs::visit_structure (be_structure *node)
{
  return this->visit_node (node);
}

int
be_visitor_sequence_cdr_op_cs::visit_union (be_union *node)
{
  return this->visit_node (node);
}

int
be_visitor_sequence_cdr_op_cs::visit_typedef (be_typedef *node)
{
  this->ctx_->alias (node);
  be_type *bt = node->primitive_base_type ();

  if (!bt || (bt->accept (this) == -1))
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_sequence_cdr_op_cs::"
                         "visit_typedef - "
                         "Bad primitive type\n"),
                        -1);
    }

  this->ctx_->alias (0);
  return 0;
}

int
be_visitor_sequence_cdr_op_cs::visit_node (be_type *bt)
{
  TAO_OutStream *os = this->ctx_->stream ();
  be_sequence *node = this->ctx_->be_node_as_sequence ();

  if (!node)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_sequence_cdr_op_cs::"
                         "visit_node - "
                         "bad sequence node\n"),
                        -1);
    }

  be_visitor_context ctx (*this->ctx_);
  be_visitor_sequence_base visitor (&ctx);

  // Initialize a boolean variable.
  *os << "CORBA::Boolean _tao_marshal_flag = 1;" << be_nl << be_nl;

  // We get here if the "type" of individual elements of the sequence is not a
  // primitive type. In this case, we are left with no other alternative but
  // to encode/decode element by element.

  AST_Expression *expr = node->max_size ();

  if (expr == 0 || (expr != 0 && expr->ev () == 0))
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_sequence_cdr_op_cs::"
                         "visit_node - "
                         "bad sequence dimension\n"),
                        -1);
    }

  if (expr->ev ()->et == AST_Expression::EV_ulong)
    {
      *os << "for (CORBA::ULong i = 0; i < _tao_seq_len"
          << " && _tao_marshal_flag; "
          << "++i)" << be_idt_nl
          << "{" << be_idt_nl;
    }
  else
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_sequence_cdr_op_cs::"
                         "visit_node - "
                         "bad sequence dimension value\n"),
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
    case TAO_CodeGen::TAO_CDR_INPUT:
      switch (nt)
        {
        case AST_Decl::NT_array:
          if (bt->accept (&visitor) == -1)
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "(%N:%l) be_visitor_sequence_cdr_op_cs::"
                                 "visit_node - "
                                 "base type visit failed\n"),
                                -1);
            }

          *os << "_forany tmp (";

          if (bt->accept (&visitor) == -1)
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "(%N:%l) be_visitor_sequence_cdr_op_cs::"
                                 "visit_node - "
                                 "base type visit failed\n"),
                                -1);
            }

          *os << "_alloc ());" << be_nl;
          *os << "_tao_marshal_flag = (strm >> tmp);" << be_nl;

          if (bt->accept (&visitor) == -1)
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "(%N:%l) be_visitor_sequence_cdr_op_cs::"
                                 "visit_node - "
                                 "base type visit failed\n"),
                                -1);
            }

          *os << "_copy (_tao_sequence[i], tmp.in ());" << be_nl;

          if (bt->accept (&visitor) == -1)
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "(%N:%l) be_visitor_sequence_cdr_op_cs::"
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
                                   "(%N:%l) be_visitor_sequence_cdr_op_cs::"
                                   "visit_node - "
                                   "bad string node\n"),
                                  -1);
              }

            if (str->max_size ()->ev ()->u.ulval == 0)
              {
                // Unbounded.
                *os << "_tao_marshal_flag = (strm >> _tao_sequence[i].out ());"
                    << be_uidt_nl;
              }
            else
              {
                if (str->width () == (long) sizeof (char))
                  {
                    *os << "_tao_marshal_flag = (strm >> "
                        << "CORBA::Any::to_string (_tao_sequence[i].out (), ";
                  }
                else
                  {
                    *os << "_tao_marshal_flag = (strm >> "
                        << "CORBA::Any::to_wstring (_tao_sequence[i].out (), ";
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
          *os << "_tao_marshal_flag = (strm >> _tao_sequence[i].out ());";

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
                                   "(%N:%l) be_visitor_sequence_cdr_op_cs::"
                                   "visit_node - "
                                   "bad predefined type node\n"),
                                  -1);
              }

            switch (pt->pt ())
              {
                case AST_PredefinedType::PT_pseudo:
                case AST_PredefinedType::PT_object:
                case AST_PredefinedType::PT_value:
                  *os << "_tao_marshal_flag = (strm >> _tao_sequence[i].out ());";
                  break;
                case AST_PredefinedType::PT_char:
                  *os << "_tao_marshal_flag =" << be_idt_nl
                      << "(strm >> CORBA::Any::to_char (_tao_sequence[i]));" 
                      << be_uidt;
                  break;
                case AST_PredefinedType::PT_wchar:
                  *os << "_tao_marshal_flag =" << be_idt_nl
                      << "(strm >> CORBA::Any::to_wchar (_tao_sequence[i]));" 
                      << be_uidt;
                  break;
                case AST_PredefinedType::PT_boolean:
                  *os << "_tao_marshal_flag =" << be_idt_nl 
                      << "(strm >> CORBA::Any::to_boolean (_tao_sequence[i]));" 
                      << be_uidt;
                  break;
                case AST_PredefinedType::PT_octet:
                  *os << "_tao_marshal_flag =" << be_idt_nl 
                      << "(strm >> CORBA::Any::to_octet (_tao_sequence[i]));" 
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
                                 "(%N:%l) be_visitor_sequence_cdr_op_cs::"
                                 "visit_node - "
                                 "base type visit failed\n"),
                                -1);
            }

          *os << "_var tmp_var (" << be_idt << be_idt_nl;

          if (bt->accept (&visitor) == -1)
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "(%N:%l) be_visitor_sequence_cdr_op_cs::"
                                 "visit_node - "
                                 "base type visit failed\n"),
                                -1);
            }

          *os << "_dup (\n";

          // Even though the following arg is declared const in the
          // function signature, MSVC++ 5.0 needs the const cast,
          // and many other compilers just won't do it.
          *os << "#if defined (_MSC_VER) && (_MSC_VER <= 1100)";

          os->indent ();

          *os << be_idt << be_idt_nl;
          *os << "ACE_const_cast (const " << this->ctx_->node ()->name ()
              << ", _tao_sequence)[i]\n";
          *os << "#else";

          os->indent ();

          *os << be_nl;
          *os << "_tao_sequence[i]\n";
          *os << "#endif /* defined (_MSC_VER) && (_MSC_VER <= 1100) */";

          os->indent ();

          *os << be_uidt_nl;
          *os << ")" << be_uidt << be_uidt_nl;
          *os << ");" << be_uidt_nl;

          if (bt->accept (&visitor) == -1)
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "(%N:%l) be_visitor_sequence_cdr_op_cs::"
                                 "visit_node - "
                                 "base type visit failed\n"),
                                -1);
            }

          *os << "_forany tmp (tmp_var.inout ());" << be_nl;
          *os << "_tao_marshal_flag = (strm << tmp);";

          break;
        case AST_Decl::NT_interface:
        case AST_Decl::NT_interface_fwd:
          *os << "_tao_marshal_flag =" << be_idt_nl;

          if (bt->is_defined ())
            {
              *os << "_tao_sequence[i].in ()->marshal (strm);" << be_uidt;
            }
          else
            {
              AST_Decl *parent = ScopeAsDecl (bt->defined_in ());

              if (parent != 0 && parent->node_type () != AST_Decl::NT_root)
                {
                  *os << parent->name () << "::";
                }

              *os << "tao_" << bt->local_name () << "_life::tao_marshal ("
                  << "_tao_sequence[i].in (), strm);" << be_uidt;
            }

          break;
        case AST_Decl::NT_string:
        case AST_Decl::NT_wstring:
        case AST_Decl::NT_valuetype:
        case AST_Decl::NT_valuetype_fwd:
          *os << "_tao_marshal_flag = (strm << _tao_sequence[i].in ());";

          break;
        case AST_Decl::NT_pre_defined:
          {
            be_predefined_type *pt =
              be_predefined_type::narrow_from_decl (
                  td == 0 ? bt : td->primitive_base_type ()
                );

            if (!pt)
              {
                ACE_ERROR_RETURN ((LM_ERROR,
                                   "(%N:%l) be_visitor_sequence_cdr_op_cs::"
                                   "visit_node - "
                                   "bad predefined type node\n"),
                                  -1);
              }

            switch (pt->pt ())
              {
                case AST_PredefinedType::PT_pseudo:
                case AST_PredefinedType::PT_object:
                case AST_PredefinedType::PT_value:
                  *os << "_tao_marshal_flag = (strm << _tao_sequence[i].in ());";
                  break;
                case AST_PredefinedType::PT_char:
                  *os << "_tao_marshal_flag =" << be_idt_nl
                      << "(strm << CORBA::Any::from_char (_tao_sequence[i]));" 
                      << be_uidt;
                  break;
                case AST_PredefinedType::PT_wchar:
                  *os << "_tao_marshal_flag =" << be_idt_nl
                      << "(strm << CORBA::Any::from_wchar (_tao_sequence[i]));" 
                      << be_uidt;
                  break;
                case AST_PredefinedType::PT_boolean:
                  *os << "_tao_marshal_flag =" << be_idt_nl 
                      << "(strm << CORBA::Any::from_boolean (_tao_sequence[i]));" 
                      << be_uidt;
                  break;
                case AST_PredefinedType::PT_octet:
                  *os << "_tao_marshal_flag =" << be_idt_nl 
                      << "(strm << CORBA::Any::from_octet (_tao_sequence[i]));" 
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
                         "(%N:%l) be_visitor_sequence_cdr_op_cs::"
                         "visit_node - "
                         "bad sub state\n"),
                        -1);
    }

  *os << be_uidt_nl << be_nl;
  *os << "return _tao_marshal_flag;" << be_uidt_nl;
 
  return 0;
}
