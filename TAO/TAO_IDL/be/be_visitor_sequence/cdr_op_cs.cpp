// $Id$

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
    }

  *os << be_nl << be_nl << "// TAO_IDL - Generated from" << be_nl
    << "// " << __FILE__ << ":" << __LINE__ << be_nl;

  *os << "#if !defined _TAO_CDR_OP_"
      << node->flat_name () << "_CPP_" << be_nl
      << "#define _TAO_CDR_OP_" << node->flat_name () << "_CPP_"
      << be_nl;

  bool alt = be_global->alt_mapping ();

  *os << be_global->core_versioning_begin () << be_nl;

  //  Set the sub state as generating code for the output operator.
  this->ctx_->sub_state (TAO_CodeGen::TAO_CDR_OUTPUT);

  if (alt)
    {
      *os << "::CORBA::Boolean operator<< (" << be_idt_nl
          << "TAO_OutputCDR &strm," << be_nl
          << "const std::vector<" << bt->full_name ()
          << "> &_tao_vector)"
          << be_uidt_nl
          << "{" << be_idt_nl
          << "::CORBA::ULong length = _tao_vector.size ();"
          << be_nl
          << "strm << length;" << be_nl << be_nl
          << "for ( ::CORBA::ULong i = 0UL; i < length; ++i)"
          << be_idt_nl
          << "{" << be_idt_nl
          << "if (! (strm << _tao_vector[i]))" << be_idt_nl
          << "{" << be_idt_nl
          << "return false;" << be_uidt_nl
          << "}" << be_uidt << be_uidt_nl
          << "}" << be_uidt_nl << be_nl
          << "return true;" << be_uidt_nl
          << "}" << be_nl << be_nl;
    }
  else
    {
      *os << "::CORBA::Boolean operator<< ("
          << be_idt << be_idt_nl
          << "TAO_OutputCDR &strm," << be_nl
          << "const " << node->name () << " &_tao_sequence"
          << be_uidt_nl
          << ")" << be_uidt_nl
          << "{" << be_idt_nl
          << "return TAO::marshal_sequence(strm, _tao_sequence);"
          << be_uidt_nl
          << "}" << be_nl << be_nl;
        }

  //  Set the sub state as generating code for the input operator.
  this->ctx_->sub_state(TAO_CodeGen::TAO_CDR_INPUT);

  if (alt)
    {
      *os << "::CORBA::Boolean operator>> (" << be_idt_nl
          << "TAO_InputCDR &strm," << be_nl
          << "std::vector<" << bt->full_name ()
          << "> &_tao_vector)" << be_uidt_nl
          << "{" << be_idt_nl
          << "::CORBA::ULong length = 0UL;" << be_nl
          << bt->full_name ();

      if (bt->size_type () == AST_Type::VARIABLE)
        {
          *os << " *";
        }

      *os << " tmp;" << be_nl << be_nl
          << "if (! (strm >> length))" << be_idt_nl
          << "{" << be_idt_nl
          << "return false;" << be_uidt_nl
          << "}" << be_uidt_nl << be_nl
          << "_tao_vector.resize (length);" << be_nl << be_nl
          << "for ( ::CORBA::ULong i = 0UL; i < length; ++i)"
          << be_idt_nl
          << "{" << be_idt_nl
          << "if (! (strm >> tmp))" << be_idt_nl
          << "{" << be_idt_nl
          << "return false;" << be_uidt_nl
          << "}" << be_uidt_nl << be_nl
          << "_tao_vector[i] = tmp;" << be_uidt_nl
          << "}" << be_uidt_nl << be_nl
          << "return true;" << be_uidt_nl
          << "}" << be_nl;
    }
  else
    {
      *os << "::CORBA::Boolean operator>> ("
          << be_idt << be_idt_nl
          << "TAO_InputCDR &strm"
          << "," << be_nl
          << node->name ()
          << " &_tao_sequence"
          << be_uidt_nl
          << ")" << be_uidt_nl
          << "{" << be_idt_nl
          << "return TAO::demarshal_sequence(strm, _tao_sequence);"
          << be_uidt_nl
          << "}" << be_nl;
    }

  if (be_global->gen_ostream_operators ())
    {
      node->gen_ostream_operator (os, false);
    }

  *os << be_nl << be_global->core_versioning_end ();

  *os << be_nl
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
be_visitor_sequence_cdr_op_cs::visit_component (be_component *node)
{
  return this->visit_node (node);
}

int
be_visitor_sequence_cdr_op_cs::visit_component_fwd (be_component_fwd *node)
{
  return this->visit_node (node);
}

int
be_visitor_sequence_cdr_op_cs::visit_home (be_home *node)
{
  return this->visit_node (node);
}

int
be_visitor_sequence_cdr_op_cs::visit_valuebox (be_valuebox *node)
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
be_visitor_sequence_cdr_op_cs::visit_eventtype (be_eventtype *node)
{
  return this->visit_node (node);
}

int
be_visitor_sequence_cdr_op_cs::visit_eventtype_fwd (be_eventtype_fwd *node)
{
  return this->visit_node (node);
}

int
be_visitor_sequence_cdr_op_cs::visit_predefined_type (
    be_predefined_type *node
  )
{
  return this->visit_node (node);
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
  return this->visit_node (node);
}

int
be_visitor_sequence_cdr_op_cs::visit_node (be_type *)
{
  return 0;
}
