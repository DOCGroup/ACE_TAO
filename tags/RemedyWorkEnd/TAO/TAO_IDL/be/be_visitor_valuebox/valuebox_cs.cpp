
//=============================================================================
/**
 *  @file    valuebox_cs.cpp
 *
 *  $Id$
 *
 *  Visitor generating code for valueboxes in the client stub file
 *
 *
 *  @author Gary Maxey
 */
//=============================================================================


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

  *os << be_nl_2 << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__;

  if (node->is_defined ())
    {
      *os << be_nl_2
          << "void" << be_nl
          << "TAO::Value_Traits<" << node->name  () << ">::add_ref ("
          << be_idt << be_idt_nl
          << node->name () << " * p" << be_uidt_nl
          << ")" << be_uidt_nl
          << "{" << be_idt_nl
          << "::CORBA::add_ref (p);" << be_uidt_nl
          << "}";

      *os << be_nl_2
          << "void" << be_nl
          << "TAO::Value_Traits<" << node->name () << ">::remove_ref ("
          << be_idt << be_idt_nl
          << node->name () << " * p" << be_uidt_nl
          << ")" << be_uidt_nl
          << "{" << be_idt_nl
          << "::CORBA::remove_ref (p);" << be_uidt_nl
          << "}";

      *os << be_nl_2
          << "void" << be_nl
          << "TAO::Value_Traits<" << node->name () << ">::release ("
          << be_idt << be_idt_nl
          << node->name () << " * p" << be_uidt_nl
          << ")" << be_uidt_nl
          << "{" << be_idt_nl
          << "::CORBA::remove_ref (p);" << be_uidt_nl
          << "}";
    }

  // The _downcast method.
  *os << be_nl_2
      << node->name () << " *" << be_nl
      << node->name () << "::_downcast ( ::CORBA::ValueBase *v)" << be_nl
      << "{" << be_idt_nl
      << "return dynamic_cast< ::" << node->name () << " * > (v);"
      << be_uidt_nl << "}" << be_nl_2;

  // _copy_value method
  *os << "::CORBA::ValueBase *" << be_nl
      << node->name () << "::_copy_value (void)" << be_nl
      << "{" << be_idt_nl
      << "::CORBA::ValueBase *result = 0;" << be_nl
      << "ACE_NEW_RETURN (" << be_idt_nl
      << "result," << be_nl
      << node->local_name () << " (*this)," << be_nl
      << "0);" << be_nl << be_uidt_nl
      << "return result;" << be_uidt_nl
      << "}" << be_nl_2;

  // The _tao_obv_repository_id method.
  *os << "const char *" << be_nl
      << node->name () << "::_tao_obv_repository_id (void) const"
      << be_nl << "{" << be_idt_nl
      << "return this->_tao_obv_static_repository_id ();" << be_uidt_nl
      << "}" << be_nl_2;

  *os << "void" << be_nl
      << node->name ()
      << "::_tao_obv_truncatable_repo_ids (Repository_Id_List& ids) const"
      << be_nl
      << "{" << be_idt_nl
      << "ids.push_back (this->_tao_obv_static_repository_id ());"
      << be_uidt_nl
      << "}" << be_nl_2;

  // _tao_match_formal_type method.  Generated because ValueBase interface
  // requires it. Since value boxes do not support inheritence, this can
  // simply return true.
  *os << "::CORBA::Boolean " << be_nl
      << node->name ()
      << "::_tao_match_formal_type (ptrdiff_t ) const" << be_nl
      << "{" << be_idt_nl
      << "return true;" << be_uidt_nl
      << "}" << be_nl_2;


  if (be_global->any_support ())
    {
      *os << "void" << be_nl
          << node->name ()
          << "::_tao_any_destructor (void *_tao_void_pointer)" << be_nl
          << "{" << be_idt_nl
          << node->local_name () << " *_tao_tmp_pointer =" << be_idt_nl
          << "static_cast<" << be_idt
          << node->local_name () << " *> ("
          << "_tao_void_pointer);" << be_uidt << be_uidt_nl
          << "::CORBA::remove_ref (_tao_tmp_pointer);" << be_uidt_nl
          << "}" << be_nl_2;
    }

  // Switch streams to the *A.cpp file if we are using this option.
  if (be_global->gen_anyop_files ())
    {
      os = tao_cg->anyop_source ();
    }

  if (be_global->tc_support ())
    {
      *os << "// TAO extension - the virtual _type method." << be_nl;
      *os << "::CORBA::TypeCode_ptr " << node->name ()
          << "::_tao_type (void) const" << be_nl;
      *os << "{" << be_idt_nl;
      *os << "return ::" << node->tc_name () << ";" << be_uidt_nl;
      *os << "}" << be_nl_2;
    }

  // Make sure we are generating to *C.cpp regardless of the above.
  os = tao_cg->client_stubs ();

  AST_Type * at = node->boxed_type()->unaliased_type();
  be_type *bt = be_type::narrow_from_decl (at);

  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuebox_cs::"
                         "visit_valuebox - "
                         "Bad type\n"),
                        -1);
    }

  bool is_array = false;
  const char * unmarshal_arg;
  be_predefined_type *bpt = be_predefined_type::narrow_from_decl (bt);

  if (bpt != 0)
    {
      switch (bpt->pt())
        {
        case AST_PredefinedType::PT_boolean:
          unmarshal_arg =
            "::ACE_InputCDR::to_boolean (vb_object->_pd_value)";
          break;

        case AST_PredefinedType::PT_char:
          unmarshal_arg =
            "::ACE_InputCDR::to_char (vb_object->_pd_value)";
          break;

        case AST_PredefinedType::PT_wchar:
          unmarshal_arg =
            "::ACE_InputCDR::to_wchar (vb_object->_pd_value)";
          break;

        case AST_PredefinedType::PT_octet:
          unmarshal_arg =
            "::ACE_InputCDR::to_octet (vb_object->_pd_value)";
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
  *os << "::CORBA::Boolean " << be_nl
      << node->name () << "::_tao_unmarshal (" << be_idt << be_idt_nl
      << "TAO_InputCDR &strm," << be_nl
      << node->local_name () << " *&vb_object" << be_uidt_nl
      << ")" << be_uidt_nl
      << "{" << be_idt_nl
      << "::CORBA::Boolean is_null_object = false;" << be_nl
      << "::CORBA::Boolean is_indirected = false;" << be_nl
      << "TAO_InputCDR indrected_strm ((size_t) 0);" << be_nl
      << "if ( ::CORBA::ValueBase::_tao_validate_box_type (" << be_idt
      << be_idt << be_idt_nl
      << "strm, indrected_strm, " << be_nl
      << node->local_name () << "::_tao_obv_static_repository_id (),"
      << be_nl
      << "is_null_object, is_indirected"
      << be_uidt_nl
      << ") == false)" << be_uidt_nl
      << "{" << be_idt_nl
      << "return false;" << be_uidt_nl
      << "}" << be_uidt_nl << be_nl
      << "vb_object = 0;" << be_nl
      << "if (is_null_object)"  << be_idt_nl
      << "{" << be_idt_nl
      << "return true;" << be_uidt_nl
      << "}" << be_uidt_nl << be_nl
      << "if (is_indirected)"  << be_idt_nl
      << "{" << be_idt_nl
      << "return " << node->name () << "::_tao_unmarshal (" << be_idt
      << be_idt << be_idt_nl
      << " indrected_strm, vb_object);"
      << be_uidt << be_uidt << be_uidt << be_uidt_nl
      << "}" << be_uidt_nl << be_nl
      << "ACE_NEW_RETURN (" << be_idt_nl
      << "vb_object," << be_nl
      << node->local_name () << "," << be_nl
      << "false);" << be_uidt_nl << be_nl;

  if (is_array)
    {
      *os << at->full_name()
          << "_forany temp (vb_object->_boxed_inout ());" << be_nl;
    }

  *os << "return (strm >> ";

  be_string *str = be_string::narrow_from_decl (bt);
  if (str != 0 &&
      str->max_size ()->ev ()->u.ulval != 0)
    {
      if (str->width () == (long) sizeof (char))
        {
          *os << "::ACE_InputCDR::to_string "
              << "(vb_object->_pd_value, "
              << str->max_size ()->ev ()->u.ulval << ")";
        }
      else
        {
          *os << "::ACE_InputCDR::to_wstring "
              << "(vb_object->_pd_value, "
              << str->max_size ()->ev ()->u.ulval << ")";
        }
    }
  else
    {
      *os << unmarshal_arg;
    }

  *os << ");" << be_uidt_nl
      << "}" << be_nl_2;

  // _tao_unmarshal_v method.  Generated because ValueBase interface
  // requires it.  But there is nothing for it to do in the valuebox
  // case.
  *os << "::CORBA::Boolean " << be_nl
      << node->name ()
      << "::_tao_unmarshal_v (TAO_InputCDR &)" << be_nl
      << "{" << be_idt_nl
      << "return true;" << be_uidt_nl
      << "}" << be_nl_2;

  // Emit the type specific elements.  The visit_* methods in this
  // module do that work.
  if (bt->accept (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         " (%N:%l) be_visitor_valuebox_cs::visit_valuebox - "
                         "type-specific valuebox code generation failed\n"),
                        -1);
    }

  // Indicate that code is already generated for this node.
  node->cli_stub_gen (true);

  return 0;
}

int
be_visitor_valuebox_cs::visit_array (be_array * node)
{
  TAO_OutStream & os = *this->ctx_->stream ();

  // Retrieve the node being visited by this be_visitor_valuebox_cs.
  be_decl * const vb_node = this->ctx_->node ();

  os << be_nl_2 << "// TAO_IDL - Generated from" << be_nl
     << "// " << __FILE__ << ":" << __LINE__ << be_nl_2;

  this->emit_destructor ();

  // _tao_marshal_v method
  os << "::CORBA::Boolean " << be_nl
     << vb_node->name ()
     << "::_tao_marshal_v (TAO_OutputCDR & strm) const" << be_nl
     << "{" << be_idt_nl
     << node->name () << "_forany temp (this->_pd_value.ptr ());"
     << be_nl
     << "return (strm << temp);" << be_uidt_nl
     << "}" << be_nl_2;

  return 0;
}

int
be_visitor_valuebox_cs::visit_enum (be_enum *)
{
  TAO_OutStream & os = *this->ctx_->stream ();

  // Retrieve the node being visited by this be_visitor_valuebox_cs.
  be_decl * const vb_node = this->ctx_->node ();

  os << be_nl_2 << "// TAO_IDL - Generated from" << be_nl
     << "// " << __FILE__ << ":" << __LINE__ << be_nl_2;

  this->emit_destructor ();

  static char const marshal_arg[] = "this->_pd_value";

  // _tao_marshal_v method
  os << "::CORBA::Boolean " << be_nl
     << vb_node->name ()
     << "::_tao_marshal_v (TAO_OutputCDR & strm) const" << be_nl
     << "{" << be_idt_nl
     << "return (strm << " << marshal_arg << ");" << be_uidt_nl
     << "}" << be_nl_2;

  return 0;
}


int
be_visitor_valuebox_cs::visit_interface (be_interface *)
{
  return this->emit_for_predef_enum ("this->_pd_value");
}

int
be_visitor_valuebox_cs::visit_predefined_type (be_predefined_type * node)
{
  char const * marshal_arg;

  switch (node->pt ())
    {
    case AST_PredefinedType::PT_boolean:
      marshal_arg =
        "::ACE_OutputCDR::from_boolean (this->_pd_value)";
      break;

    case AST_PredefinedType::PT_char:
      marshal_arg =
        "::ACE_OutputCDR::from_char (this->_pd_value)";
      break;

    case AST_PredefinedType::PT_wchar:
      marshal_arg =
        "::ACE_OutputCDR::from_wchar (this->_pd_value)";
      break;

    case AST_PredefinedType::PT_octet:
      marshal_arg =
        "::ACE_OutputCDR::from_octet (this->_pd_value)";
      break;

    case AST_PredefinedType::PT_any:
      marshal_arg = "this->_pd_value.in ()";
      break;

    default:
      marshal_arg = "this->_pd_value";
      break;
    }

  return this->emit_for_predef_enum (marshal_arg);
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
                         "(%N:%l) be_visitor_valuebox_cs::"
                         "visit_sequence - "
                         "Bad element type\n"),
                        -1);
    }
  be_visitor_context ctx (*this->ctx_);
  ctx.state (TAO_CodeGen::TAO_SEQUENCE_BUFFER_TYPE_CH);
  be_visitor_sequence_buffer_type bt_visitor (&ctx);

  *os << be_nl_2 << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__ << be_nl_2;

  if (node->unbounded ())
    {
      // Public constructor with one argument of type ULong
      *os << vb_node->name () << "::" << vb_node->local_name ()
          << " ( ::CORBA::ULong max)" << be_nl
          << "{" << be_idt_nl
          << node->full_name () << "* p;" << be_nl
          << "ACE_NEW (" << be_idt_nl
          << "p," << be_nl
          << node->full_name () << " (max));" << be_uidt_nl
          << "this->_pd_value = p;" << be_uidt_nl
          << "}" << be_nl_2;
    }

  // Public constructor for sequence with supplied buffer
  *os << vb_node->name () << "::" << vb_node->local_name ()
      << " (" << be_idt;

  if (node->unbounded ())
    {
      *os << be_nl << "::CORBA::ULong max,";
    }

  *os << be_nl << "::CORBA::ULong length, " << be_nl;


  if (bt->accept (&bt_visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuebox_cs::"
                         "visit_valuebox - "
                         "base type visit failed\n"),
                        -1);
    }

  *os << " * buf," << be_nl
      << "::CORBA::Boolean release)" << be_uidt_nl
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
      << "}" << be_nl_2;

  // end: Public constructor for sequence with supplied buffer

  this->emit_destructor ();

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
      << vb_node->name () << "::operator[] ( ::CORBA::ULong index)"
      << be_nl
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
      << "}" << be_nl_2;

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
      << "::operator[] ( ::CORBA::ULong index) const" << be_nl
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

  *os << "}" << be_nl_2;

  // _tao_marshal_v method
  *os << "::CORBA::Boolean " << be_nl
      << vb_node->name ()
      << "::_tao_marshal_v (TAO_OutputCDR & strm) const" << be_nl
      << "{" << be_idt_nl
      << "return (strm << this->_pd_value.in ());" << be_uidt_nl
      << "}" << be_nl_2;

  return 0;
}

int
be_visitor_valuebox_cs::visit_string (be_string *str)
{
  TAO_OutStream & os = *this->ctx_->stream ();

  // Retrieve the node being visited by this be_visitor_valuebox_cs.
  be_decl * const vb_node = this->ctx_->node ();

  os << be_nl_2 << "// TAO_IDL - Generated from" << be_nl
     << "// " << __FILE__ << ":" << __LINE__ << be_nl_2;

  this->emit_destructor ();

  // _tao_marshal_v method
  os << "::CORBA::Boolean " << be_nl
     << vb_node->name ()
     << "::_tao_marshal_v (TAO_OutputCDR & strm) const" << be_nl
     << "{" << be_idt_nl
     << "return (strm << ";

  if (str->max_size ()->ev ()->u.ulval != 0)
    {
      if (str->width () == (long) sizeof (char))
        {
          os << "::ACE_OutputCDR::from_string "
             << "(this->_pd_value, "
             << str->max_size ()->ev ()->u.ulval << ")";
        }
      else
        {
          os << "::ACE_OutputCDR::from_wstring "
             << "(this->_pd_value, "
             << str->max_size ()->ev ()->u.ulval << ")";
        }
    }
  else
    {
      os << "this->_pd_value";
    }

  os << ");" << be_uidt_nl
     << "}" << be_nl_2;

  return 0;
}

int
be_visitor_valuebox_cs::visit_structure (be_structure *)
{
  TAO_OutStream & os = *this->ctx_->stream ();

  // Retrieve the node being visited by this be_visitor_valuebox_cs.
  be_decl * const vb_node = this->ctx_->node ();

  os << be_nl_2 << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__ << be_nl_2;

  this->emit_destructor ();

  // _tao_marshal_v method
  os << "::CORBA::Boolean " << be_nl
     << vb_node->name ()
     << "::_tao_marshal_v (TAO_OutputCDR & strm) const" << be_nl
     << "{" << be_idt_nl
     << "return (strm << this->_pd_value.in ());" << be_uidt_nl
     << "}" << be_nl_2;

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
  TAO_OutStream & os = *this->ctx_->stream ();

  // Retrieve the node being visited by this be_visitor_valuebox_cs.
  be_decl * const vb_node = this->ctx_->node ();

  os << be_nl_2 << "// TAO_IDL - Generated from" << be_nl
     << "// " << __FILE__ << ":" << __LINE__ << be_nl_2;

  this->emit_destructor ();

  // _tao_marshal_v method
  os << "::CORBA::Boolean " << be_nl
     << vb_node->name ()
     << "::_tao_marshal_v (TAO_OutputCDR & strm) const" << be_nl
     << "{" << be_idt_nl
     << "return (strm << this->_pd_value.in ());" << be_uidt_nl
     << "}" << be_nl_2;

  return 0;
}

void
be_visitor_valuebox_cs::emit_destructor (void)
{
  TAO_OutStream & os = *this->ctx_->stream ();

  // Retrieve the node being visited by this be_visitor_valuebox_cs.
  be_decl * const vb_node = this->ctx_->node ();

  // Protected destructor
  os << vb_node->name () << "::~" << vb_node->local_name () << " (void)"
     << be_nl << "{" << be_nl << "}" << be_nl_2;
}

int
be_visitor_valuebox_cs::emit_for_predef_enum (char const * marshal_arg)
{
  TAO_OutStream & os = *this->ctx_->stream ();

  // Retrieve the node being visited by this be_visitor_valuebox_cs.
  be_decl * const vb_node = this->ctx_->node ();

  os << be_nl_2 << "// TAO_IDL - Generated from" << be_nl
     << "// " << __FILE__ << ":" << __LINE__ << be_nl_2;

  this->emit_destructor ();

  // _tao_marshal_v method
  os << "::CORBA::Boolean " << be_nl
     << vb_node->name ()
     << "::_tao_marshal_v (TAO_OutputCDR & strm) const" << be_nl
     << "{" << be_idt_nl
     << "return (strm << " << marshal_arg << ");" << be_uidt_nl
     << "}" << be_nl_2;

  return 0;
}
