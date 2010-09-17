//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    union_branch/serializer_op_cs.cpp
//
// = DESCRIPTION
//    Visitor generating code for Union_Branch in the client stubs file.
//
// = AUTHOR
//    Scott Harris <harris_s@ociweb.com> based on code by Aniruddha Gokhale
//
// ============================================================================

// **********************************************
//  Visitor for union_branch in the client stubs file.
// **********************************************

be_visitor_union_branch_serializer_op_cs::be_visitor_union_branch_serializer_op_cs (
    be_visitor_context *ctx
  )
  : be_visitor_decl (ctx)
{
}

be_visitor_union_branch_serializer_op_cs::~be_visitor_union_branch_serializer_op_cs (void)
{
}

int
be_visitor_union_branch_serializer_op_cs::visit_union_branch (be_union_branch *node)
{
  be_type *bt = be_type::narrow_from_decl (node->field_type ());

  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_branch_serializer_op_cs::"
                         "visit_union_branch - "
                         "Bad union_branch type\n"),
                        -1);
    }

  this->ctx_->node (node);

  if (bt->accept (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_branch_serializer_op_cs::"
                         "visit_union_branch - "
                         "codegen for union_branch type failed\n"),
                        -1);
    }

  return 0;
}

int
be_visitor_union_branch_serializer_op_cs::visit_array (be_array *node)
{
  // If not a typedef and we are defined in the use scope, we must be defined.
  if (!this->ctx_->alias ()
      && node->is_child (this->ctx_->scope ()->decl ()))
    {
      be_visitor_context ctx (*this->ctx_);
      ctx.node (node);
      be_visitor_array_serializer_op_cs visitor (&ctx);

      if (node->accept (&visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_union_branch_serializer_op_cs::"
                             "visit_array - "
                             "codegen failed\n"),
                            -1);
        }
    }

  TAO_OutStream *os = this->ctx_->stream ();

  // Retrieve the union_branch node.
  be_union_branch *f =
    be_union_branch::narrow_from_decl (this->ctx_->node ());

  if (f == 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_branch_serializer_op_cs::"
                         "visit_array - "
                         "cannot retrieve union_branch node\n"),
                        -1);
    }

  // For anonymous arrays, the type name has a _ prepended. We compute the
  // fullname with or without the underscore and use it later on.
  char fname [NAMEBUFSIZE];  // to hold the full and

  // Save the node's local name and full name in a buffer for quick
  // use later on.
  ACE_OS::memset (fname,
                  '\0',
                  NAMEBUFSIZE);

  if (!this->ctx_->alias () // not a typedef
      && node->is_child (this->ctx_->scope ()->decl ()))
    {
      // For anonymous arrays ...
      // we have to generate a name for us that has an underscore
      // prepended to our local name. This needs to be inserted after
      // the parents's name.
      if (node->is_nested ())
        {
          be_decl *parent =
            be_scope::narrow_from_scope (node->defined_in ())->decl ();
          ACE_OS::sprintf (fname,
                           "%s::_%s",
                           parent->full_name (),
                           node->local_name ()->get_string ());
        }
      else
        {
          ACE_OS::sprintf (fname,
                           "_%s",
                           node->full_name ());
        }
    }
  else
    {
      // Typedefed node.
      ACE_OS::sprintf (fname, "%s", node->full_name ());
    }

  // Check what is the code generations substate. Are we generating code for
  // the in/out operators for our parent or for us?
  switch (this->ctx_->sub_state ())
    {
    case TAO_CodeGen::TAO_IS_BOUNDED_SIZE:
      {
        //SHH - TBD - could avoid the counter by enclosing in {}
        //     But this code is currently dead anyway because
        //     unions always return false for _dcps_is_bounded and a large
        //     number for _dcsp_max_marshaled_size.
        //     This note applies to multiple locations in this file.
        static int tmp_counter = 0;
        char tmp_val_name[30];
        ACE_OS::sprintf(tmp_val_name, "tmp_array_val%d", tmp_counter++);
        *os << fname << "_forany " << tmp_val_name << " (" << be_idt << be_idt_nl
            << "_tao_union."
            << f->local_name () << " ()" << be_uidt_nl
            <<");" << be_uidt_nl;
        *os << "is_bounded = is_bounded && "
            << "_tao_is_bounded_size (" << tmp_val_name << ");"
            << be_nl;
      }
      break;
    case TAO_CodeGen::TAO_MAX_MARSHALED_SIZE:
      {
        static int tmp_counter = 0;
        char tmp_size_name[30];
        char tmp_val_name[30];
        ACE_OS::sprintf(tmp_size_name, "tmp_array_size%d", tmp_counter);
        ACE_OS::sprintf(tmp_val_name, "tmp_array_val%d", tmp_counter++);
        *os << f->field_type ()->name() << "_forany " << tmp_val_name
            << "; // is type of " << f->local_name () << be_nl;
        *os << "size_t " << tmp_size_name
            << " = _dcps_max_marshaled_size (" << tmp_val_name << ");" << be_nl
            << "if (" << tmp_size_name << " > max_size) " << be_idt_nl
            << "max_size = " << tmp_size_name << ";" << be_uidt_nl << be_nl;
      }
      break;
    case TAO_CodeGen::TAO_FIND_SIZE:
      *os << fname << "_forany _tao_union_tmp (" << be_idt << be_idt_nl
          << "_tao_union."
          << f->local_name () << " ()" << be_uidt_nl
          <<");" << be_uidt_nl;
      *os << "result += _dcps_find_size (_tao_union_tmp);";
      break;
    case TAO_CodeGen::TAO_CDR_INPUT:
      *os << fname << " _tao_union_tmp;" << be_nl
          << fname << "_forany _tao_union_helper ("
          << be_idt << be_idt_nl
          << "_tao_union_tmp" << be_uidt_nl
          << ");" << be_uidt_nl
          << "result = strm >> _tao_union_helper;" << be_nl << be_nl
          << "if (result)" << be_idt_nl
          << "{" << be_idt_nl
          << "_tao_union." << f->local_name ()
          << " (_tao_union_tmp);" << be_nl
          << "_tao_union._d (_tao_discriminant);" << be_uidt_nl
          << "}" << be_uidt;

      return 0;

    case TAO_CodeGen::TAO_CDR_OUTPUT:
      *os << fname << "_forany _tao_union_tmp (" << be_idt << be_idt_nl
          << "_tao_union."
          << f->local_name () << " ()" << be_uidt_nl
          <<");" << be_uidt_nl
          << "result = strm << _tao_union_tmp;";
      return 0;

    case TAO_CodeGen::TAO_CDR_SCOPE:
      // This is done in serializer_op_cs.cpp  and hacked into *.i.
      break;
    default:
      // Error.
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_branch_serializer_op_cs::"
                         "visit_array - "
                         "bad sub state\n"),
                        -1);
    }

  return 0;
}

int
be_visitor_union_branch_serializer_op_cs::visit_enum (be_enum *node)
{
  TAO_OutStream *os = this->ctx_->stream ();

  // Retrieve the union_branch node
  be_union_branch *f =
    be_union_branch::narrow_from_decl (this->ctx_->node ());

  if (f == 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_branch_serializer_op_cs::"
                         "visit_array - "
                         "cannot retrieve union_branch node\n"),
                        -1);
    }

  switch (this->ctx_->sub_state ())
    {
    case TAO_CodeGen::TAO_IS_BOUNDED_SIZE:
      *os << "// " << f->local_name ()
          << " is of an enum type; hence is bounded" << be_nl;
      return 0; // all done
      break;
    case TAO_CodeGen::TAO_FIND_SIZE:
      // enums are serialized as CORBA::ULong
      *os << "result +=  _dcps_max_marshaled_size_ulong (); /* enum */"
          << be_nl;
      return 0; // all done
      break;
    case TAO_CodeGen::TAO_MAX_MARSHALED_SIZE:
      // enums are serialized as CORBA::ULong
      *os << "if (_dcps_max_marshaled_size_ulong () > max_size)" << be_idt_nl
          << "max_size = _dcps_max_marshaled_size_ulong (); /* enum */"
          << be_uidt_nl << be_nl;
      return 0; // all done
      break;
    default:
      // do code in the rest of this method.
      break;
    }

  // Check what is the code generations substate. Are we generating code for
  // the in/out operators for our parent or for us?
  switch (this->ctx_->sub_state ())
    {
    case TAO_CodeGen::TAO_CDR_INPUT:
      *os << node->name () << " _tao_union_tmp"
          << ";" << be_nl
          << "result = strm >> _tao_union_tmp;" << be_nl
          << "if (result)" << be_idt_nl
          << "{" << be_idt_nl
          << "_tao_union." << f->local_name ()
          << " (_tao_union_tmp);" << be_nl
          << "_tao_union._d (_tao_discriminant);" << be_uidt_nl
          << "}" << be_uidt;

      return 0;

    case TAO_CodeGen::TAO_CDR_OUTPUT:
      *os << "result = strm << _tao_union." << f->local_name () << " ();";
      return 0;

    case TAO_CodeGen::TAO_CDR_SCOPE:
      // Proceed further.
      break;
    default:
      // Error.
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_branch_serializer_op_cs::"
                         "visit_enum - "
                         "bad sub state\n"),
                        -1);
    }

  // Not a typedef and node is defined inside the union. Otherwise the
  // Serializer operator is generated elsewhere.
  if (node->node_type () != AST_Decl::NT_typedef
      && node->is_child (this->ctx_->scope ()->decl ()))
    {
      // Instantiate a visitor context with a copy of our context. This info
      // will be modified based on what type of node we are visiting.
      be_visitor_context ctx (*this->ctx_);
      ctx.node (node);
      be_visitor_enum_serializer_op_cs visitor (&ctx);

      if (visitor.visit_enum (node) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_union_branch_serializer_op_cs::"
                             "visit_enum - "
                             "codegen failed\n"),
                            -1);
        }
    }

  return 0;
}

int
be_visitor_union_branch_serializer_op_cs::visit_interface (be_interface *node)
{
  TAO_OutStream *os = this->ctx_->stream ();

  // Retrieve the union_branch node.
  be_union_branch *f =
    be_union_branch::narrow_from_decl (this->ctx_->node ());

  if (f == 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_branch_serializer_op_cs::"
                         "visit_interface - "
                         "cannot retrieve union_branch node\n"),
                        -1);
    }

  // Check what is the code generations substate. Are we generating code for
  // the in/out operators for our parent or for us?
  switch (this->ctx_->sub_state ())
    {
    case TAO_CodeGen::TAO_CDR_INPUT:
      if (node->is_local ())
        {
          *os << "result = 0;";
        }
      else
        {
          *os << node->name () << "_var _tao_union_tmp;" << be_nl
              << "result = strm >> _tao_union_tmp.inout ();" << be_nl << be_nl
              << "if (result)" << be_idt_nl
              << "{" << be_idt_nl
              << "_tao_union."
              << f->local_name () << " (_tao_union_tmp.in ());" << be_nl
              << "_tao_union._d (_tao_discriminant);" << be_uidt_nl
              << "}" << be_uidt;
        }

      break;

    case TAO_CodeGen::TAO_CDR_OUTPUT:
      if (node->is_defined ())
        {
          if (f->is_abstract ())
            {
              *os << "(strm << _tao_union."
                  << f->local_name () << " ());";
            }
          else
            {
              *os << "::CORBA::Object::marshal (" << be_idt << be_idt_nl
                  << "_tao_union." << f->local_name () << " ()," << be_nl
                  << "strm" << be_uidt_nl
                  << ");" << be_uidt;
            }
        }
      else
        {
          *os << "result =" << be_idt_nl
              << "TAO::Objref_Traits<" << node->name () << ">::marshal ("
              << be_idt << be_idt_nl
              << "_tao_union." << f->local_name () << " ()," << be_nl
              << "strm" << be_uidt_nl
              << ");" << be_uidt << be_uidt;
        }

      break;

    case TAO_CodeGen::TAO_CDR_SCOPE:
      // Nothing to be done because an interface cannot be declared inside a
      // union.
      break;

    default:
      // Error.
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_branch_serializer_op_cs::"
                         "visit_interface - "
                         "bad sub state\n"),
                        -1);
    }

  return 0;
}

int
be_visitor_union_branch_serializer_op_cs::visit_interface_fwd (be_interface_fwd *node)
{
  TAO_OutStream *os = this->ctx_->stream ();

  // Retrieve the union_branch node.
  be_union_branch *f =
    be_union_branch::narrow_from_decl (this->ctx_->node ());

  if (f == 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_branch_serializer_op_cs::"
                         "visit_interface_fwd - "
                         "cannot retrieve union_branch node\n"),
                        -1);
    }

  // Check what is the code generations substate. Are we generating code for
  // the in/out operators for our parent or for us?
  switch (this->ctx_->sub_state ())
    {
    case TAO_CodeGen::TAO_CDR_INPUT:
      if (node->is_local ())
        {
          *os << "result = 0;";
        }
      else
        {
          *os << node->name () << "_var _tao_union_tmp;" << be_nl
              << "result = strm >> _tao_union_tmp.inout ();" << be_nl << be_nl
              << "if (result)" << be_idt_nl
              << "{" << be_idt_nl
              << "_tao_union."
              << f->local_name () << " (_tao_union_tmp.in ());" << be_nl
              << "_tao_union._d (_tao_discriminant);" << be_uidt_nl
              << "}" << be_uidt;
        }

      break;

    case TAO_CodeGen::TAO_CDR_OUTPUT:
      *os << "result = _tao_union."
          << f->local_name () << " ()->marshal (strm);";
      break;

    case TAO_CodeGen::TAO_CDR_SCOPE:
      // Nothing to be done because an interface cannot be forward declared
      // inside a union.
      break;

    default:
      // Error.
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_branch_serializer_op_cs::"
                         "visit_interface_fwd - "
                         "bad sub state\n"),
                        -1);
    }

  return 0;
}

int
be_visitor_union_branch_serializer_op_cs::visit_valuetype (be_valuetype *node)
{
  TAO_OutStream *os = this->ctx_->stream ();

  // Retrieve the union_branch node.
  be_union_branch *f =
    be_union_branch::narrow_from_decl (this->ctx_->node ());

  if (f == 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_branch_serializer_op_cs::"
                         "visit_valuetype - "
                         "cannot retrieve union_branch node\n"),
                        -1);
    }

  // Check what is the code generations substate. Are we generating code for
  // the in/out operators for our parent or for us?
  switch (this->ctx_->sub_state ())
    {
    case TAO_CodeGen::TAO_CDR_INPUT:
      *os << node->name () << "_var _tao_union_tmp;" << be_nl
          << "result = strm >> _tao_union_tmp.inout ();" << be_nl << be_nl
          << "if (result)" << be_idt_nl
          << "{" << be_idt_nl
          << "_tao_union."
          << f->local_name () << " (_tao_union_tmp.in ());" << be_nl
          << "_tao_union._d (_tao_discriminant);" << be_uidt_nl
          << "}" << be_uidt;

      break;

    case TAO_CodeGen::TAO_CDR_OUTPUT:
      *os << "result = strm << _tao_union."
          << f->local_name () << " ();";
      break;

    case TAO_CodeGen::TAO_CDR_SCOPE:
      // Nothing to be done because an interface cannot be declared inside a
      // union.
      break;

    default:
      // Error.
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_branch_serializer_op_cs::"
                         "visit_valuetype - "
                         "bad sub state\n"),
                        -1);
    }

  return 0;
}

int
be_visitor_union_branch_serializer_op_cs::visit_valuetype_fwd (be_valuetype_fwd *node)
{
  TAO_OutStream *os = this->ctx_->stream ();

  // Retrieve the union_branch node.
  be_union_branch *f =
    be_union_branch::narrow_from_decl (this->ctx_->node ());

  if (f == 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_branch_serializer_op_cs::"
                         "visit_valuetype_fwd - "
                         "cannot retrieve union_branch node\n"),
                        -1);
    }

  // Check what is the code generations substate. Are we generating code for
  // the in/out operators for our parent or for us?
  switch (this->ctx_->sub_state ())
    {
    case TAO_CodeGen::TAO_CDR_INPUT:
      *os << node->name () << "_var _tao_union_tmp;" << be_nl
          << "result = strm >> _tao_union_tmp.inout ();" << be_nl << be_nl
          << "if (result)" << be_idt_nl
          << "{" << be_idt_nl
          << "_tao_union."
          << f->local_name () << " (_tao_union_tmp.in ());" << be_nl
          << "_tao_union._d (_tao_discriminant);" << be_uidt_nl
          << "}" << be_uidt;

      break;

    case TAO_CodeGen::TAO_CDR_OUTPUT:
      *os << "result  = strm << _tao_union."
          << f->local_name () << " ();";
      break;

    case TAO_CodeGen::TAO_CDR_SCOPE:
      // Nothing to be done because an interface cannot be forward declared
      // inside a union.
      break;

    default:
      // Error.
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_branch_serializer_op_cs::"
                         "visit_valuetype_fwd - "
                         "bad sub state\n"),
                        -1);
    }

  return 0;
}

int
be_visitor_union_branch_serializer_op_cs::visit_predefined_type (be_predefined_type *node)
{
  TAO_OutStream *os = this->ctx_->stream ();

  // Retrieve the union_branch node.
  be_union_branch *f =
    be_union_branch::narrow_from_decl (this->ctx_->node ());

  if (f == 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_branch_serializer_op_cs::"
                         "visit_predefined_type - "
                         "cannot retrieve union_branch node\n"),
                        -1);
    }

  AST_PredefinedType::PredefinedType pt = node->pt ();

  // Check what is the code generations substate. Are we generating code for
  // the in/out operators for our parent or for us?
  switch (this->ctx_->sub_state ())
    {
    case TAO_CodeGen::TAO_MAX_MARSHALED_SIZE:
      {
      static int tmp_counter = 0;
      char tmp_name[10];
      ACE_OS::sprintf(tmp_name, "tmp%d", tmp_counter++);

      if (pt == AST_PredefinedType::PT_pseudo
          || pt == AST_PredefinedType::PT_object)
        {
          *os << "PT_pseudo and PT_object not supported by -Gdcps";
          *os << be_nl << be_nl << "// above error is from " << be_nl
              << "// " << __FILE__ << ":" << __LINE__ << be_nl << be_nl;
        }
      else if (pt == AST_PredefinedType::PT_char)
        {
          *os << "size_t " << tmp_name << " = _dcps_max_marshaled_size ("
              << "::ACE_OutputCDR::from_char (_tao_union."
              << f->local_name () << " ()));" << be_nl
              << "if (" << tmp_name << " > max_size) max_size = "
              << tmp_name << ";" << be_nl;
        }
      else if (pt == AST_PredefinedType::PT_wchar)
        {
          *os << "size_t " << tmp_name << " = _dcps_max_marshaled_size ("
              << "::ACE_OutputCDR::from_wchar (_tao_union."
              << f->local_name () << " ()));" << be_nl
              << "if (" << tmp_name << " > max_size) max_size = "
              << tmp_name << ";" << be_nl;
        }
      else if (pt == AST_PredefinedType::PT_octet)
        {
          *os << "size_t " << tmp_name << " = _dcps_max_marshaled_size ("
              << "::ACE_OutputCDR::from_octet (_tao_union."
              << f->local_name () << " ()));" << be_nl
              << "if (" << tmp_name << " > max_size) max_size = "
              << tmp_name << ";" << be_nl;
        }
      else if (pt == AST_PredefinedType::PT_boolean)
        {
          *os << "size_t " << tmp_name << " = _dcps_max_marshaled_size ("
              << "::ACE_OutputCDR::from_boolean (_tao_union."
              << f->local_name () << " ()));" << be_nl
              << "if (" << tmp_name << " > max_size) max_size = "
              << tmp_name << ";" << be_nl;
        }
      else
        {
          *os << "size_t " << tmp_name
              << " = _dcps_max_marshaled_size (_tao_union."
              << f->local_name () << " ());" << be_nl
              << "if (" << tmp_name << " > max_size)" << be_idt_nl
              << "max_size = "<< tmp_name << ";" << be_uidt_nl << be_nl;
        }
      }
      break;

    case TAO_CodeGen::TAO_IS_BOUNDED_SIZE:
      if (pt == AST_PredefinedType::PT_pseudo
          || pt == AST_PredefinedType::PT_object)
        {
          *os << "PT_pseudo and PT_object not supported by -Gdcps";
          *os << be_nl << be_nl << "// above error is from " << be_nl
              << "// " << __FILE__ << ":" << __LINE__ << be_nl << be_nl;
        }
      else
        {
          *os << "// " << f->local_name ()
              << " is of a predefined type; hence is bounded" << be_nl;
        }
      break;

    case TAO_CodeGen::TAO_FIND_SIZE:

      *os << "result += ";

      if (pt == AST_PredefinedType::PT_pseudo
          || pt == AST_PredefinedType::PT_object)
        {
          *os << "PT_pseudo and PT_object not supported by -Gdcps";
          *os << be_nl << be_nl << "// above error is from " << be_nl
              << "// " << __FILE__ << ":" << __LINE__ << be_nl << be_nl;
        }
      else if (pt == AST_PredefinedType::PT_char)
        {
          *os << "_dcps_max_marshaled_size (::ACE_OutputCDR::from_char (_tao_union."
              << f->local_name () << " ()));";
        }
      else if (pt == AST_PredefinedType::PT_wchar)
        {
          *os << "_dcps_max_marshaled_size (::ACE_OutputCDR::from_wchar (_tao_union."
              << f->local_name () << " ()));";
        }
      else if (pt == AST_PredefinedType::PT_octet)
        {
          *os << "_dcps_max_marshaled_size (::ACE_OutputCDR::from_octet (_tao_union."
              << f->local_name () << " ()));";
        }
      else if (pt == AST_PredefinedType::PT_boolean)
        {
          *os << "_dcps_max_marshaled_size (::ACE_OutputCDR::from_boolean (_tao_union."
              << f->local_name () << " ()));";
        }
      else
        {
          // is predefined so _find_size = _dcps_max_marshaled_size
          *os << "_dcps_max_marshaled_size (_tao_union." << f->local_name () << " ());";
        }

      break;

    case TAO_CodeGen::TAO_CDR_INPUT:
      if (pt == AST_PredefinedType::PT_object)
        {
          *os << "::CORBA::Object_var _tao_union_tmp;" << be_nl;

          *os << "result = strm >> _tao_union_tmp.out ();" << be_nl << be_nl
              << "if (result)" << be_idt_nl
              << "{" << be_idt_nl
              << "_tao_union." << f->local_name () << " (_tao_union_tmp.in ());";

        }
      else if (pt == AST_PredefinedType::PT_abstract)
        {
          *os << "::CORBA::AbstractBase_var _tao_union_tmp;" << be_nl;

          *os << "result = strm >> _tao_union_tmp.out ();" << be_nl << be_nl
              << "if (result)" << be_idt_nl
              << "{" << be_idt_nl
              << "_tao_union." << f->local_name () << " (_tao_union_tmp.in ());";

        }
      else if (pt == AST_PredefinedType::PT_pseudo)
        {
          *os << "::CORBA::TypeCode_var _tao_union_tmp;" << be_nl
              << "result = strm >> _tao_union_tmp.out ();" << be_nl << be_nl
              << "if (result)" << be_idt_nl
              << "{" << be_idt_nl
              << "_tao_union." << f->local_name () << " (_tao_union_tmp.in ());";

        }
      else if (pt == AST_PredefinedType::PT_value)
        {
          *os << "::CORBA::ValueBase * _tao_union_tmp;" << be_nl
              << "result = strm >> _tao_union_tmp;" << be_nl << be_nl
              << "if (result)" << be_idt_nl
              << "{" << be_idt_nl
              << "_tao_union." << f->local_name () << " (_tao_union_tmp);";

        }
      else if (pt == AST_PredefinedType::PT_char)
        {
          *os << "::CORBA::Char _tao_union_tmp;" << be_nl
              << "::ACE_InputCDR::to_char _tao_union_helper "
              << "(_tao_union_tmp);" << be_nl
              << "result = strm >> _tao_union_helper;" << be_nl << be_nl
              << "if (result)" << be_idt_nl
              << "{" << be_idt_nl
              << "_tao_union." << f->local_name ()
              << " (_tao_union_tmp);";
        }
      else if (pt == AST_PredefinedType::PT_wchar)
        {
          *os << "::CORBA::WChar _tao_union_tmp;" << be_nl
              << "::ACE_InputCDR::to_wchar _tao_union_helper "
              << "(_tao_union_tmp);" << be_nl
              << "result = strm >> _tao_union_helper;" << be_nl << be_nl
              << "if (result)" << be_idt_nl
              << "{" << be_idt_nl
              << "_tao_union." << f->local_name ()
              << " (_tao_union_tmp);";
        }
      else if (pt == AST_PredefinedType::PT_octet)
        {
          *os << "::CORBA::Octet _tao_union_tmp;" << be_nl
              << "::ACE_InputCDR::to_octet _tao_union_helper "
              << "(_tao_union_tmp);" << be_nl
              << "result = strm >> _tao_union_helper;" << be_nl << be_nl
              << "if (result)" << be_idt_nl
              << "{" << be_idt_nl
              << "_tao_union." << f->local_name ()
              << " (_tao_union_tmp);";
        }
      else if (pt == AST_PredefinedType::PT_boolean)
        {
          *os << "::CORBA::Boolean _tao_union_tmp;" << be_nl
              << "::ACE_InputCDR::to_boolean _tao_union_helper "
              << "(_tao_union_tmp);" << be_nl
              << "result = strm >> _tao_union_helper;" << be_nl << be_nl
              << "if (result)" << be_idt_nl
              << "{" << be_idt_nl
              << "_tao_union." << f->local_name ()
              << " (_tao_union_tmp);";
        }
      else
        {
          *os << node->name () << " _tao_union_tmp;" << be_nl
              << "result = strm >> _tao_union_tmp;" << be_nl << be_nl
              << "if (result)" << be_idt_nl
              << "{" << be_idt_nl
              << "_tao_union." << f->local_name ()
              << " (_tao_union_tmp);";
        }

      *os << be_nl;
      *os << "_tao_union._d (_tao_discriminant);" << be_uidt_nl;
      *os << "}" << be_uidt;

      break;

    case TAO_CodeGen::TAO_CDR_OUTPUT:

      *os << "result = ";

      if (pt == AST_PredefinedType::PT_pseudo
          || pt == AST_PredefinedType::PT_object)
        {
          *os << "strm << _tao_union." << f->local_name () << " ();";
        }
      else if (pt == AST_PredefinedType::PT_char)
        {
          *os << "strm << ::ACE_OutputCDR::from_char (_tao_union."
              << f->local_name () << " ());";
        }
      else if (pt == AST_PredefinedType::PT_wchar)
        {
          *os << "strm << ::ACE_OutputCDR::from_wchar (_tao_union."
              << f->local_name () << " ());";
        }
      else if (pt == AST_PredefinedType::PT_octet)
        {
          *os << "strm << ::ACE_OutputCDR::from_octet (_tao_union."
              << f->local_name () << " ());";
        }
      else if (pt == AST_PredefinedType::PT_boolean)
        {
          *os << "strm << ::ACE_OutputCDR::from_boolean (_tao_union."
              << f->local_name () << " ());";
        }
      else
        {
          *os << "strm << _tao_union." << f->local_name () << " ();";
        }

      break;

    case TAO_CodeGen::TAO_CDR_SCOPE:
      // Nothing to be done.
      break;
    default:
      // Error.
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_branch_serializer_op_cs::"
                         "visit_array - "
                         "bad sub state\n"),
                        -1);
    }

  return 0;
}

int
be_visitor_union_branch_serializer_op_cs::visit_sequence (be_sequence *node)
{
  // If the sequence is defined in this scope, generate its Serializer stream
  // operators heree.
  if (node->node_type () != AST_Decl::NT_typedef
      && node->is_child (this->ctx_->scope ()->decl ()))
    {
      // Anonymous sequence
      be_visitor_context ctx (*this->ctx_);
      ctx.node (node);
      be_visitor_sequence_serializer_op_cs visitor (&ctx);

      if (node->accept (&visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_union_branch_serializer_op_cs::"
                             "visit_sequence - "
                             "codegen failed\n"),
                            -1);
        }
    }

  // Now generate the Serializer stream operators for the sequence as a
  // union branch.

  TAO_OutStream *os = this->ctx_->stream ();

  // Retrieve the union_branch node.
  be_union_branch *f =
    be_union_branch::narrow_from_decl (this->ctx_->node ());

  if (f == 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_branch_serializer_op_cs::"
                         "visit_sequence - "
                         "cannot retrieve union_branch node\n"
                         ), -1);
    }

  // Check what is the code generations substate. Are we generating code for
  // the in/out operators for our parent or for us?
  switch (this->ctx_->sub_state ())
    {
    case TAO_CodeGen::TAO_IS_BOUNDED_SIZE:
      {
        static int tmp_counter = 0;
        char tmp_val_name[30];
        ACE_OS::sprintf(tmp_val_name, "tmp_seq_val%d", tmp_counter++);
        *os << f->field_type ()->name() << " " << tmp_val_name
            << "; // is type of " << f->local_name () << be_nl;
        *os << "is_bounded = is_bounded && "
            << "_tao_is_bounded_size (" << tmp_val_name << ");"
            << be_nl;
      }
      break;
    case TAO_CodeGen::TAO_MAX_MARSHALED_SIZE:
      {
        static int tmp_counter = 0;
        char tmp_size_name[30];
        char tmp_val_name[30];
        ACE_OS::sprintf(tmp_size_name, "tmp_seq_size%d", tmp_counter);
        ACE_OS::sprintf(tmp_val_name,  "tmp_seq_val%d", tmp_counter++);
        *os << f->field_type ()->name() << " " << tmp_val_name
            << "; // is type of " << f->local_name () << be_nl;
        *os << "size_t " << tmp_size_name
            << " = _dcps_max_marshaled_size (" << tmp_val_name << ");" << be_nl
            << "if (" << tmp_size_name << " > max_size) " << be_idt_nl
            << "max_size = " << tmp_size_name << ";" << be_uidt_nl << be_nl;
      }
      break;
    case TAO_CodeGen::TAO_FIND_SIZE:
      //SHH - TBD - make sure this is a shallow copy.
      //     But this code is currently dead anyway because
      //     unions always return false for _dcps_is_bounded and a large
      //     number for _dcsp_max_marshaled_size.
      //     This note applies to multiple locations in this file.
      *os << f->field_type ()->name() << " tmp (_tao_union."
          << f->local_name () << " ());" << be_nl;
      *os << "result += _dcps_find_size (tmp);" << be_nl;
      break;
    case TAO_CodeGen::TAO_CDR_INPUT:
      {
        // If the typedef'd sequence is included from another
        // file, node->name() won't work. The following works
        // for all typedefs, external or not.
        be_typedef *td = this->ctx_->alias ();

        if (td)
          {
            *os << td->name ();
          }
        else
          {
            *os << node->name ();
          }

        *os << " _tao_union_tmp;" << be_nl
            << "result = strm >> _tao_union_tmp;" << be_nl << be_nl
            << "if (result)" << be_idt_nl
            << "{" << be_idt_nl
            << "_tao_union."
            << f->local_name () << " (_tao_union_tmp);" << be_nl
            << "_tao_union._d (_tao_discriminant);" << be_uidt_nl
            << "}" << be_uidt;

        return 0;
      }

    case TAO_CodeGen::TAO_CDR_OUTPUT:
      *os << "result = strm << _tao_union."
          << f->local_name () << " ();";
      return 0;

    case TAO_CodeGen::TAO_CDR_SCOPE:
      // Proceed further.
      break;
    default:
      // Error
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_branch_serializer_op_cs::"
                         "visit_sequence - "
                         "bad sub state\n"),
                        -1);
    }

  return 0;
}

int
be_visitor_union_branch_serializer_op_cs::visit_string (be_string *node)
{
  TAO_OutStream *os = this->ctx_->stream ();

  // Retrieve the union_branch node.
  be_union_branch *f =
    be_union_branch::narrow_from_decl (this->ctx_->node ());

  if (f == 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_branch_serializer_op_cs::"
                         "visit_string - "
                         "cannot retrieve union_branch node\n"),
                        -1);
    }

  // Check what is the code generations substate. Are we generating code for
  // the in/out operators for our parent or for us?
  switch (this->ctx_->sub_state ())
    {
    case TAO_CodeGen::TAO_IS_BOUNDED_SIZE:
       if (0 == node->max_size ()->ev ()->u.ulval)
         *os << "is_bounded = is_bounded && false; /* unbounded string */"
             << be_nl;
       else
         *os << "// bounded string " << f->local_name ()
             << be_nl;
      break;
    case TAO_CodeGen::TAO_MAX_MARSHALED_SIZE:
      {
        static int tmp_counter = 0;
        char tmp_size_name[30];
        ACE_OS::sprintf(tmp_size_name, "tmp_string_size%d", tmp_counter++);

        char buff[15];
        ACE_OS::sprintf(buff, "%ld", node->max_size ()->ev ()->u.ulval);
        *os << "size_t " << tmp_size_name
            << " = _dcps_max_marshaled_size_ulong () + " << buff << ";" << be_nl
            << "if (" << tmp_size_name << ")" << be_idt_nl
            << "max_size = " << tmp_size_name << ";" << be_uidt_nl;
      }
      break;
    case TAO_CodeGen::TAO_FIND_SIZE:
      *os << "result += _dcps_max_marshaled_size_ulong() + " << be_idt_nl
          << "(_tao_union." << f->local_name () << " () ? ACE_OS::strlen(_tao_union."
          << f->local_name () << " ()) : 0);" << be_uidt_nl;
      break;
    case TAO_CodeGen::TAO_CDR_INPUT:
      if (node->width () == (long) sizeof (char))
        {
          *os << "::CORBA::String_var _tao_union_tmp;" << be_nl;
        }
      else
        {
          *os << "::CORBA::WString_var _tao_union_tmp;" << be_nl;
        }

      *os << "result = strm >> _tao_union_tmp.out ();" << be_nl << be_nl
          << "if (result)" << be_idt_nl
          << "{" << be_idt_nl
          << "_tao_union."
          << f->local_name () << " (_tao_union_tmp);" << be_nl
          << "_tao_union._d (_tao_discriminant);" << be_uidt_nl
          << "}" << be_uidt;

      break;

    case TAO_CodeGen::TAO_CDR_OUTPUT:
      *os << "result = strm << _tao_union."
          << f->local_name () << " ();";
      break;

    case TAO_CodeGen::TAO_CDR_SCOPE:
      // Nothing to be done.
      break;
    default:
      // Error.
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_branch_serializer_op_cs::"
                         "visit_array - "
                         "bad sub state\n"),
                        -1);
    }

  return 0;
}

int
be_visitor_union_branch_serializer_op_cs::visit_structure (be_structure *node)
{
  // If the structure is defined in this scope. generate its Serializer
  // stream operators here.
  if (node->node_type () != AST_Decl::NT_typedef
      && node->is_child (this->ctx_->scope ()->decl ()))
    {
      be_visitor_context ctx (*this->ctx_);
      ctx.node (node);
      be_visitor_structure_serializer_op_cs visitor (&ctx);

      if (node->accept (&visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_union_branch_serializer_op_cs::"
                             "visit_struct - "
                             "codegen failed\n"),
                            -1);
        }
    }

  // How generate the Serializer stream operators for the structure as
  // a union branch.

  TAO_OutStream *os = this->ctx_->stream ();

  // Retrieve the union_branch node.
  be_union_branch *f =
    be_union_branch::narrow_from_decl (this->ctx_->node ());

  if (f == 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_branch_serializer_op_cs::"
                         "visit_structure - "
                         "cannot retrieve union_branch node\n"),
                        -1);
    }

  // Check what is the code generations substate. Are we generating code for
  // the in/out operators for our parent or for us?
  switch (this->ctx_->sub_state ())
    {
    case TAO_CodeGen::TAO_MAX_MARSHALED_SIZE:
      {
        static int tmp_counter = 0;
        char tmp_size_name[30];
        char tmp_val_name[30];
        ACE_OS::sprintf(tmp_size_name, "tmp_struct_size%d", tmp_counter);
        ACE_OS::sprintf(tmp_val_name, "tmp_struct_val%d", tmp_counter++);

        *os << f->field_type ()->name() << " " << tmp_val_name
            << "; // is type of " << f->local_name () << be_nl;
        *os << "size_t " << tmp_size_name
            << " = _dcps_max_marshaled_size (" << tmp_val_name << ");" << be_nl
            << "if (" << tmp_size_name << " > max_size) " << be_idt_nl
            << "max_size = " << tmp_size_name << ";" << be_uidt_nl << be_nl;
      }
      break;

    case TAO_CodeGen::TAO_IS_BOUNDED_SIZE:
      {
        static int tmp_counter = 0;
        char tmp_size_name[30];
        char tmp_val_name[30];
        ACE_OS::sprintf(tmp_size_name, "tmp_struct_size%d", tmp_counter);
        ACE_OS::sprintf(tmp_val_name, "tmp_struct_val%d", tmp_counter++);

        *os << f->field_type ()->name() << " " << tmp_val_name
            << "; // is type of " << f->local_name () << be_nl;
        *os << "is_bounded = is_bounded && _tao_is_bounded_size ("
            << tmp_val_name << ");" << be_nl;
      }
      break;

    case TAO_CodeGen::TAO_FIND_SIZE:
      *os << "result += _dcps_find_size (_tao_union."
          << f->local_name () << " ());" << be_nl;
      break;

    case TAO_CodeGen::TAO_CDR_INPUT:
      *os << node->name () << " _tao_union_tmp;" << be_nl
          << "result = strm >> _tao_union_tmp;" << be_nl << be_nl
          << "if (result)" << be_idt_nl
          << "{" << be_idt_nl
          << "_tao_union." << f->local_name ()
          << " (_tao_union_tmp);" << be_nl
          << "_tao_union._d (_tao_discriminant);" << be_uidt_nl
          << "}" << be_uidt;

      return 0;

    case TAO_CodeGen::TAO_CDR_OUTPUT:
      *os << "result = strm << _tao_union."
          << f->local_name () << " ();";
      return 0;

    case TAO_CodeGen::TAO_CDR_SCOPE:
      // Proceed further.
      break;
    default:
      // Error.
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_branch_serializer_op_cs::"
                         "visit_structure - "
                         "bad sub state\n"),
                        -1);
    }

  return 0;
}

int
be_visitor_union_branch_serializer_op_cs::visit_structure_fwd (
  be_structure_fwd *node)
{
  be_structure *s =
    be_structure::narrow_from_decl (node->full_definition ());

  return this->visit_structure (s);
}

int
be_visitor_union_branch_serializer_op_cs::visit_typedef (be_typedef *node)
{
  this->ctx_->alias (node);

  // The node to be visited in the base primitve type that gets typedefed.
  be_type *bt = node->primitive_base_type ();

  if (!bt || (bt->accept (this) == -1))
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_branch_public_ci::"
                         "visit_typedef - "
                         "Bad primitive type\n"),
                        -1);
    }

  this->ctx_->alias (0);
  return 0;
}

int
be_visitor_union_branch_serializer_op_cs::visit_union (be_union *node)
{
  // If the union is defined in this scope, generate its Serializer
  // stream operators here.
  if (node->node_type () != AST_Decl::NT_typedef
      && node->is_child (this->ctx_->scope ()->decl ()))
    {
      be_visitor_context ctx (*this->ctx_);
      ctx.node (node);
      be_visitor_union_serializer_op_cs visitor (&ctx);

      if (node->accept (&visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_union_branch_serializer_op_cs::"
                             "visit_union - "
                             "codegen failed\n"),
                            -1);
        }
    }

  // Now generate the Serializer stream operators for the union as
  // a union branch.

  TAO_OutStream *os = this->ctx_->stream ();

  // Retrieve the union_branch node.
  be_union_branch *f =
    be_union_branch::narrow_from_decl (this->ctx_->node ());

  if (f == 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_branch_serializer_op_cs::"
                         "visit_union - "
                         "cannot retrieve union_branch node\n"),
                        -1);
    }

  // Check what is the code generations substate. Are we generating code for
  // the in/out operators for our parent or for us?
  switch (this->ctx_->sub_state ())
    {
    case TAO_CodeGen::TAO_CDR_INPUT:
      *os << node->name () << " _tao_union_tmp"
          << ";" << be_nl
          << "result = strm >> _tao_union_tmp;" << be_nl << be_nl
          << "if (result)" << be_idt_nl
          << "{" << be_idt_nl
          << "_tao_union." << f->local_name ()
          << " (_tao_union_tmp);" << be_nl
          << "_tao_union._d (_tao_discriminant);" << be_uidt_nl
          << "}" << be_uidt_nl;

      return 0;

    case TAO_CodeGen::TAO_CDR_OUTPUT:
      *os << "result = strm << _tao_union."
          << f->local_name () << " ();";
      return 0;

    case TAO_CodeGen::TAO_CDR_SCOPE:
      // Proceed further.
      break;
    default:
      // Error.
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_branch_serializer_op_cs::"
                         "visit_union - "
                         "bad sub state\n"),
                        -1);
    }

  return 0;
}

int
be_visitor_union_branch_serializer_op_cs::explicit_default (void)
{
  be_union *bu =
    be_union::narrow_from_decl (this->ctx_->scope ()->decl ());
  int def_index = bu->default_index ();

  if (def_index != -1)
    {
      be_union_branch *ub =
        be_union_branch::narrow_from_decl (this->ctx_->node ());

      int i = 0;

      // Instantiate a scope iterator.
      for (UTL_ScopeActiveIterator si (bu, UTL_Scope::IK_decls);
           !si.is_done ();
           si.next ())
        {
          be_union_branch *bub = 0; // union branch node

          AST_Decl *d = si.item ();

          if (!d->imported ())
            {
              bub = be_union_branch::narrow_from_decl (d);
            }

          if (bub == ub)
            {
              return (i == def_index);
            }
          else
            {
              ++i;
            }
        }
    }

  return 0;
}

int
be_visitor_union_branch_serializer_op_cs::visit_union_fwd (
  be_union_fwd *node)
{
  be_union *u =
    be_union::narrow_from_decl (node->full_definition ());

  return this->visit_union (u);
}

