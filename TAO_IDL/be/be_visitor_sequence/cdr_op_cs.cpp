
//=============================================================================
/**
 *  @file    cdr_op_cs.cpp
 *
 *  $Id$
 *
 *  Visitor for code generation of Sequences for the CDR operators
 *  in the client stubs.
 *
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================


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

  AST_Type *bt = node->base_type ()->unaliased_type ();
  AST_String *str = AST_String::narrow_from_decl (bt);
  
  // Generating sequences as typedefs of std::vector classes
  // means that unbounded sequences with the same element type
  // are not unique types to the C++ compiler. String sequences
  // are a common problem, so we check for an inclusion and skip
  // the operator definition if found.
  if (str != 0 && str->width () == 1 && idl_global->imported_string_seq_seen_)
    {
      return 0;
    }

  TAO_OutStream *os = this->ctx_->stream ();

  // Generate the CDR << and >> operator defns.

  // Save the sequence node for further use.
  this->ctx_->node (node);

  // If our element type is an anonymous sequence, generate code for it here.
  if (bt->node_type () == AST_Decl::NT_sequence)
    {
      int status =
          this->gen_anonymous_base_type (
              be_type::narrow_from_decl (bt),
              TAO_CodeGen::TAO_ROOT_CDR_OP_CS);

      if (status == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_array_cdr_op_cs::"
                             "visit_sequence - "
                             "gen_anonymous_base_type failed\n"),
                            -1);
        }
    }

  *os << be_nl_2 << "// TAO_IDL - Generated from" << be_nl
    << "// " << __FILE__ << ":" << __LINE__ << be_nl;

  bool alt = be_global->alt_mapping ();

  *os << be_global->core_versioning_begin () << be_nl;

  // The guard should be generated to prevent multiple declarations,
  // since a sequence of a given element type may be typedef'd
  // more than once.

  os->gen_ifdef_macro (bt->flat_name (), "seq_cdr_op_cs", false);

  *os << be_nl_2;

  AST_PredefinedType *pdt =
    AST_PredefinedType::narrow_from_decl (bt);
  AST_PredefinedType::PredefinedType pt =
    AST_PredefinedType::PT_abstract;
  
  if (pdt != 0)
    {
      pt = pdt->pt ();
    }
 
  //  Set the sub state as generating code for the output operator.
  this->ctx_->sub_state (TAO_CodeGen::TAO_CDR_OUTPUT);

  if (alt)
    {
      *os << "::CORBA::Boolean operator<< (" << be_idt_nl
          << "TAO_OutputCDR &strm," << be_nl
          << "const " << node->name () << " &_tao_sequence)"
          << be_uidt_nl
          << "{" << be_idt_nl
          << "::CORBA::ULong length = _tao_sequence.size ();"
          << be_nl
          << "strm << length;" << be_nl_2
          << "for ( ::CORBA::ULong i = 0UL; i < length; ++i)"
          << be_idt_nl
          << "{" << be_idt_nl;
          
      switch (pt)
        {
          case AST_PredefinedType::PT_boolean:
            *os << "ACE_OutputCDR::from_boolean tao_tmp (_tao_sequence[i]);"
                << be_nl << be_nl;
            break;
          case AST_PredefinedType::PT_char:
            *os << "ACE_OutputCDR::from_char tao_tmp (_tao_sequence[i]);"
                << be_nl << be_nl;
            break;
          case AST_PredefinedType::PT_octet:
            *os << "ACE_OutputCDR::from_octet tao_tmp (_tao_sequence[i]);"
                << be_nl << be_nl;
            break;
          case AST_PredefinedType::PT_wchar:
            *os << "ACE_OutputCDR::from_wchar tao_tmp (_tao_sequence[i]);"
                << be_nl << be_nl;
            break;
          default:
            break;
        }
          
      *os << "if (! (strm << ";
      
      switch (pt)
        {
          case AST_PredefinedType::PT_boolean:
          case AST_PredefinedType::PT_char:
          case AST_PredefinedType::PT_octet:
          case AST_PredefinedType::PT_wchar:
            *os << "tao_tmp))";
            break;
          default:
            *os << "_tao_sequence[i]))";
            break;
        }
          
      *os << be_idt_nl
          << "{" << be_idt_nl
          << "return false;" << be_uidt_nl
          << "}" << be_uidt << be_uidt_nl
          << "}" << be_uidt_nl << be_nl
          << "return true;" << be_uidt_nl
          << "}" << be_nl_2;
    }
  else
    {
      *os << "::CORBA::Boolean operator<< ("
          << be_idt << be_idt_nl
          << "TAO_OutputCDR &strm," << be_nl
          << "const " << node->name () << " &_tao_sequence)"
          << be_uidt
          << be_uidt_nl
          << "{" << be_idt_nl
          << "return TAO::marshal_sequence(strm, _tao_sequence);"
          << be_uidt_nl
          << "}" << be_nl_2;
        }

  //  Set the sub state as generating code for the input operator.
  this->ctx_->sub_state(TAO_CodeGen::TAO_CDR_INPUT);

  if (alt)
    {
      *os << "::CORBA::Boolean operator>> (" << be_idt_nl
          << "TAO_InputCDR &strm," << be_nl
          << node->name () << " &_tao_sequence)" << be_uidt_nl
          << "{" << be_idt_nl
          << "::CORBA::ULong length = 0UL;" << be_nl
          << bt->full_name ();

      AST_Interface *i = AST_Interface::narrow_from_decl (bt);
      AST_ValueType *v = AST_ValueType::narrow_from_decl (bt);
      bool ptr_type = false;
      
      if (i != 0)
        {
          ptr_type =  true;
          *os << (v == 0 ? "_ptr" : " *");
        }
        
      if (pt == AST_PredefinedType::PT_pseudo)
        {
          ACE_CString lname (bt->local_name ()->get_string ());
            
          if (lname == "TypeCode")
            {
              ptr_type = true;
              *os << "_ptr";
            }
        }
         
      *os << " tmp" << (ptr_type ? " = 0" : "");
      
      *os << ";" << be_nl << be_nl
          << "if (! (strm >> length))" << be_idt_nl
          << "{" << be_idt_nl
          << "return false;" << be_uidt_nl
          << "}" << be_uidt_nl << be_nl
          << "_tao_sequence.resize (length);" << be_nl << be_nl
          << "for ( ::CORBA::ULong i = 0UL; i < length; ++i)"
          << be_idt_nl
          << "{" << be_idt_nl;
                    
      switch (pt)
        {
          case AST_PredefinedType::PT_boolean:
            *os << "ACE_InputCDR::to_boolean tao_tmp (tmp);"
                << be_nl << be_nl;
            break;
          case AST_PredefinedType::PT_char:
            *os << "ACE_InputCDR::to_char tao_tmp (tmp);"
                << be_nl << be_nl;
            break;
          case AST_PredefinedType::PT_octet:
            *os << "ACE_InputCDR::to_octet tao_tmp (tmp);"
                << be_nl << be_nl;
            break;
          case AST_PredefinedType::PT_wchar:
            *os << "ACE_InputCDR::to_wchar tao_tmp (tmp);"
                << be_nl << be_nl;
            break;
          default:
            break;
        }
          
      *os << "if (! (strm >> ";
          
      switch (pt)
        {
          case AST_PredefinedType::PT_boolean:
          case AST_PredefinedType::PT_char:
          case AST_PredefinedType::PT_octet:
          case AST_PredefinedType::PT_wchar:
            *os << "tao_tmp))";
            break;
          default:
            *os << "tmp))";
            break;
        }
          
      *os << be_idt_nl
          << "{" << be_idt_nl
          << "return false;" << be_uidt_nl
          << "}" << be_uidt_nl << be_nl;
          
      *os << "_tao_sequence[i] = tmp;" << be_uidt_nl
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
          << " &_tao_sequence)"
          << be_uidt
          << be_uidt_nl
          << "{" << be_idt_nl
          << "return TAO::demarshal_sequence(strm, _tao_sequence);"
          << be_uidt_nl
          << "}" << be_nl;
    }

  if (be_global->gen_ostream_operators ())
    {
      node->gen_ostream_operator (os, false);
    }

  os->gen_endif ();

  *os << be_nl << be_global->core_versioning_end ();

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
