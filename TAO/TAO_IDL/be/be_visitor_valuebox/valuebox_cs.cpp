//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    valuebox_cs.cpp
//
// = DESCRIPTION
//    Visitor generating code for valueboxes in the client stub file
//
// = AUTHOR
//    Gary Maxey
//
// ============================================================================

ACE_RCSID (be_visitor_valuebox, 
           valuebox_cs, 
           "$Id: valuebox_cs.cpp Exp")

be_visitor_valuebox_cs::be_visitor_valuebox_cs (be_visitor_context *ctx)
  : be_visitor_valuebox (ctx)
{
}

be_visitor_valuebox_cs::~be_visitor_valuebox_cs (void)
{
}

int
be_visitor_valuebox_cs::visit_valuebox (be_valuebox *node)
{
  // Nothing to do if we are imported or code is already generated.
  if (node->cli_stub_gen () || node->imported ())
    { 
      return 0;
    }

  if (be_global->tc_support ())
    {
      be_visitor_context ctx (*this->ctx_);
      TAO::be_visitor_alias_typecode visitor (&ctx);

      if (node->accept (&visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_valuebox_cs::"
                             "visit_valuebox - "
                             "TypeCode definition failed\n"),
                            -1);
        }
    }

  TAO_OutStream *os = this->ctx_->stream ();

  this->ctx_->node (node); // save the node

  *os << be_nl << be_nl << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__;

  // The _downcast method.
  *os << be_nl << node->name () << " *" << be_nl
      << node->name () << "::_downcast (CORBA::ValueBase *v)" << be_nl
      << "{" << be_idt_nl
      << "if (v == 0)" << be_idt_nl
      << "{" << be_idt_nl
      << "return 0;" << be_uidt_nl
      << "}" << be_uidt_nl << be_nl
      << "return dynamic_cast< ::" << node->name () << " * > (v);"
      << be_uidt_nl << "}" << be_nl << be_nl;

  // _copy_value method
  *os << be_nl << "CORBA::ValueBase *" << be_nl
      << node->name () << "::_copy_value (void)" << be_nl
      << "{" << be_idt_nl
      << "CORBA::ValueBase *result = 0;" << be_nl
      << "ACE_NEW_RETURN (" << be_idt_nl
      << "result," << be_nl
      << node->local_name () << " (*this)," << be_nl
      << "0);" << be_nl << be_uidt_nl
      << "return result;" << be_uidt_nl
      << "}" << be_nl << be_nl;

  // The _tao_obv_repository_id method.
  *os << "const char *" << be_nl
      << node->name () << "::_tao_obv_repository_id (void) const"
      << be_nl << "{" << be_idt_nl
      << "return this->_tao_obv_static_repository_id ();" << be_uidt_nl
      << "}" << be_nl << be_nl;

  AST_Type * at = node->boxed_type()->unaliased_type();
  be_type *bt = be_type::narrow_from_decl (at);
  bool is_array = false;

  const char * unmarshal_arg;
  be_predefined_type *bpt = be_predefined_type::narrow_from_decl (bt);
  if (bpt != 0)
    {
      switch (bpt->pt())
        {
        case AST_PredefinedType::PT_boolean:
          unmarshal_arg = "CORBA::Any::to_boolean (vb_object->_pd_value)";
          break;

        case AST_PredefinedType::PT_char:
          unmarshal_arg = "CORBA::Any::to_char (vb_object->_pd_value)";
          break;

        case AST_PredefinedType::PT_wchar:
          unmarshal_arg = "CORBA::Any::to_wchar (vb_object->_pd_value)";
          break;

        case AST_PredefinedType::PT_octet:
          unmarshal_arg = "CORBA::Any::to_octet (vb_object->_pd_value)";
          break;

        case AST_PredefinedType::PT_any:
          // We need to help the ">>" operator for "any" because
          // a conversion operator is not available.
          unmarshal_arg = "vb_object->_pd_value.inout ()";
          break;

        default:
          unmarshal_arg = "vb_object->_pd_value";
        }
    }
  else if (be_array::narrow_from_decl (bt) != 0)
    {
      is_array = true;
      unmarshal_arg = "temp";
    }
  else
    {
      unmarshal_arg = "vb_object->_pd_value";
    }

  // The _tao_unmarshal method.
  *os << "CORBA::Boolean " << be_nl
      << node->name () << "::_tao_unmarshal (" << be_idt << be_idt_nl
      << "TAO_InputCDR &strm," << be_nl
      << node->local_name () << " *&vb_object" << be_uidt_nl
      << ")" << be_uidt_nl
      << "{" << be_idt_nl
      << "CORBA::Boolean is_null_object;" << be_nl
      << "if (CORBA::ValueBase::_tao_validate_box_type (" << be_idt
      << be_idt << be_idt_nl
      << "strm," << be_nl
      << node->local_name () << "::_tao_obv_static_repository_id ()," << be_nl
      << "is_null_object"
      << be_uidt_nl
      << ") == 0)" << be_uidt_nl
      << "{" << be_idt_nl
      << "return false;" << be_uidt_nl
      << "}" << be_uidt_nl << be_nl
      << "vb_object = 0;" << be_nl
      << "if (is_null_object)"  << be_idt_nl
      << "{" << be_idt_nl
      << "return true;" << be_uidt_nl
      << "}" << be_uidt_nl << be_nl
      << "ACE_NEW_RETURN (" << be_idt_nl
      << "vb_object," << be_nl
      << node->local_name () << "," << be_nl
      << "false);" << be_nl << be_uidt_nl;
  if (is_array)
    {
      *os << at->full_name() << "_forany temp (vb_object->_boxed_inout ());"
          << be_nl;
    }
  *os << "return (strm >> " << unmarshal_arg << ");" << be_uidt_nl
      << "}" << be_nl << be_nl;



  // Emit the type specific elements.  The visit_* methods in this
  // module do that work.
  if (!bt || (bt->accept (this) == -1))
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         " (%N:%l) be_visitor_valuebox_cs::visit_valuebox - "
                         "type-specific valuebox code generation failed\n"),
                        -1);
    }

  // Indicate that code is already generated for this node.
  node->cli_stub_gen (I_TRUE);

  return 0;
}

int
be_visitor_valuebox_cs::visit_array (be_array *)
{
  return 0;
}

int
be_visitor_valuebox_cs::visit_enum (be_enum *)
{
  return 0;
}


int
be_visitor_valuebox_cs::visit_interface (be_interface *)
{
  return 0;
}

int
be_visitor_valuebox_cs::visit_predefined_type (be_predefined_type *)
{
  return 0;
}

int
be_visitor_valuebox_cs::visit_sequence (be_sequence *node)
{
  TAO_OutStream *os = this->ctx_->stream ();

  // Retrieve the node being visited by this be_visitor_valuebox_cs.
  be_decl * vb_node = this->ctx_->node ();

  if (node->anonymous ())
    { // Our sequence is anonymous so we must generate a declaration
      // for it.
      be_visitor_context ctx (*this->ctx_);
      ctx.node (node);

      // First generate the sequence definition
      be_visitor_sequence_cs visitor (&ctx);

      if (node->accept (&visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_valuebox_cs::"
                             "visit_sequence - "
                             "codegen failed\n"),
                            -1);
        }
    }

  // Retrieve the base type since we will need to do some code
  // generation for it.
  be_type *bt = be_type::narrow_from_decl (node->base_type ());
  if (bt == 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuebox_ch::"
                         "visit_sequence - "
                         "Bad element type\n"),
                        -1);
    }
  be_visitor_context ctx (*this->ctx_);
  ctx.state (TAO_CodeGen::TAO_SEQUENCE_BUFFER_TYPE_CH);
  be_visitor_sequence_buffer_type bt_visitor (&ctx);

  *os << be_nl << be_nl << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__ << be_nl << be_nl;

  if (node->unbounded ())
    {
      // Public constructor with one argument of type ULong
      *os << vb_node->name () << "::" << vb_node->local_name ()
          << " (CORBA::ULong max)" << be_nl
          << "{" << be_idt_nl
          << node->full_name () << "* p;" << be_nl
          << "ACE_NEW (" << be_idt_nl
          << "p," << be_nl
          << node->full_name () << " (max));" << be_uidt_nl
          << "this->_pd_value = p;" << be_uidt_nl
          << "}" << be_nl << be_nl;
    }

  // Public constructor for sequence with supplied buffer
  *os << vb_node->name () << "::" << vb_node->local_name ()
      << " (" << be_idt;

  if (node->unbounded ())
    {
      *os << be_nl << "CORBA::ULong max,";
    }

  *os << be_nl << "CORBA::ULong length, " << be_nl;


  if (bt->accept (&bt_visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                       "(%N:%l) be_visitor_valuebox_cs::"
                       "visit_valuebox - "
                       "base type visit failed\n"),
                       -1);
    }

  *os << " * buf," << be_nl
      << "CORBA::Boolean release)" << be_uidt_nl
      << "{" << be_idt_nl
      << node->full_name () << "* p;" << be_nl
      << "ACE_NEW (" << be_idt_nl
      << "p," << be_nl
      << node->full_name () << " (";

  if (node->unbounded ())
    {
      *os << "max, ";
    }

  *os << "length, buf, release));" << be_uidt_nl
      << "this->_pd_value = p;" << be_uidt_nl
      << "}" << be_nl << be_nl;

  // end: Public constructor for sequence with supplied buffer

  // Accessor: non const
  if (bt->accept (&bt_visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                       "(%N:%l) be_visitor_valuebox_cs::"
                       "visit_valuebox - "
                       "base type visit failed\n"),
                       -1);
    }

  *os << " & " << be_nl
      << vb_node->name () << "::operator[] (CORBA::ULong index)" << be_nl
      << "{" << be_idt_nl
      << "return (";

  if (bt->accept (&bt_visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                       "(%N:%l) be_visitor_valuebox_cs::"
                       "visit_valuebox - "
                       "base type visit failed\n"),
                       -1);
    }

  *os << "&) this->_pd_value->operator[] (index);" << be_uidt_nl
      << "}" << be_nl << be_nl;

  // Accessor: const
  *os << "const ";
  if (bt->accept (&bt_visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuebox_cs::"
                         "visit_sequence - "
                         "base type visit failed\n"),
                        -1);
    }
  *os << " &" << be_nl;
  *os << vb_node->name ()
      << "::operator[] (CORBA::ULong index) const" << be_nl
      << "{" << be_idt_nl;

  switch (bt->node_type())
    {
    case AST_Decl::NT_wstring:
      *os << "TAO_SeqElem_WString_Manager mgr = this->_pd_value->operator[] "
             "(index); " << be_nl
          << "return mgr._retn ();" << be_uidt_nl;
      break;

    case AST_Decl::NT_string:
      *os << "TAO_SeqElem_String_Manager mgr = this->_pd_value->operator[] "
             "(index); " << be_nl
          << "return mgr._retn ();" << be_uidt_nl;
      break;

    default:
      *os << "return (";

      if (bt->accept (&bt_visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_valuebox_cs::"
                             "visit_sequence - "
                             "base type visit failed\n"),
                            -1);
        }

      *os << "&) this->_pd_value->operator[] (index);" << be_uidt_nl;

    }

  *os << "}" << be_nl << be_nl;

  return 0;
}

int
be_visitor_valuebox_cs::visit_string (be_string *)
{
  return 0;
}

int
be_visitor_valuebox_cs::visit_structure (be_structure *)
{
  return 0;
}

int
be_visitor_valuebox_cs::visit_typedef (be_typedef *node)
{
  // Make a decision based on the primitive base type.
  be_type *bt = node->primitive_base_type ();

  if (!bt || (bt->accept (this) == -1))
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuebox_cs::"
                         "visit_typedef - "
                         "Bad primitive type\n"),
                        -1);
    }

  return 0;
}

int
be_visitor_valuebox_cs::visit_union (be_union *)
{
  return 0;
}
