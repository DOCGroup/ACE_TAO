//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    cdr_op_ci.cpp
//
// = DESCRIPTION
//    Visitor for code generation of Arrays for the CDR operators in the client
//    stubs.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

ACE_RCSID (be_visitor_array, 
           cdr_op_ci, 
           "$Id$")

#include "be_visitor_sequence/cdr_op_ci.h"

// ***************************************************************************
// Array visitor for generating CDR operator declarations in the client
// stubs file
// ***************************************************************************

be_visitor_array_cdr_op_ci::be_visitor_array_cdr_op_ci (
    be_visitor_context *ctx
  )
  : be_visitor_decl (ctx)
{
}

be_visitor_array_cdr_op_ci::~be_visitor_array_cdr_op_ci (void)
{
}

int
be_visitor_array_cdr_op_ci::visit_array (be_array *node)
{
  if (this->ctx_->alias ())
    {
      // We are here because we are visiting base type 
      // of the array node which is itself an
      // array, i.e., this is a case of array of array.
      return this->visit_node (node);
    }

  if (node->cli_inline_cdr_op_gen () 
      || node->imported ()
      || node->is_local ())
    {
      return 0;
    }

  TAO_OutStream *os = this->ctx_->stream ();
  be_type *bt = be_type::narrow_from_decl (node->base_type ());

  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_array_cdr_op_ci::"
                         "visit_array - "
                         "Bad base type\n"),
                        -1);
    }

  // If we contain an anonymous sequence,
  // generate code for the sequence here.

  AST_Decl::NodeType nt = bt->node_type ();

  // If the node is an array of anonymous sequence, we need to
  // generate the sequence's cdr operator declaration here.
  if (nt == AST_Decl::NT_sequence && bt->anonymous ())
    {
      be_visitor_sequence_cdr_op_ci visitor (this->ctx_);

      if (bt->accept (&visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "be_visitor_array_cdr_op_ci::"
                             "visit_array - "
                             "accept on anonymous base type failed\n"),
                            -1);
        }
    }

  // If the array is an anonymous member and if its element type
  // is a declaration (not a reference), we must generate code for
  // the declaration.
  if (this->ctx_->alias () == 0 // Not a typedef.
      && bt->is_child (this->ctx_->scope ()))
    {
      int status = 0;
      be_visitor_context ctx (*this->ctx_);

      switch (nt)
      {
        case AST_Decl::NT_enum:
          {
            be_visitor_enum_cdr_op_ci ec_visitor (&ctx);
            status = bt->accept (&ec_visitor);
            break;
          }
        case AST_Decl::NT_struct:
          {
            be_visitor_structure_cdr_op_ci sc_visitor (&ctx);
            status = bt->accept (&sc_visitor);
            break;
          }
        case AST_Decl::NT_union:
          {
            be_visitor_union_cdr_op_ci uc_visitor (&ctx);
            status = bt->accept (&uc_visitor);
            break;
          }
        default:
          break;
      }

      if (status == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_array_ch::"
                             "visit_array - "
                             "array base type codegen failed\n"),
                            -1);
        }
    }

  // For anonymous arrays, the type name has a _ prepended. We compute the
  // full_name with or without the underscore and use it later on.
  char fname [NAMEBUFSIZE];  // to hold the full and

  // Save the node's local name and full name in a buffer for quick use later
  // on.
  ACE_OS::memset (fname, 
                  '\0', 
                  NAMEBUFSIZE);

  if (this->ctx_->tdef ())
    {
      ACE_OS::sprintf (fname, "%s", node->full_name ());
    }
  else
    {
      // For anonymous arrays ...
      // We have to generate a name for us that has an underscope prepended
      // to our local name. This needs to be inserted after the parents's 
      // name.

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

  // Generate the CDR << and >> operator defns.

  // Save the array node for further use.
  this->ctx_->node (node);

  *os << be_nl << be_nl << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__ << be_nl << be_nl;

  //  Set the sub state as generating code for the output operator.
  this->ctx_->sub_state (TAO_CodeGen::TAO_CDR_OUTPUT);
  *os << "ACE_INLINE" << be_nl
      << "CORBA::Boolean operator<< (" << be_idt << be_idt_nl
      << "TAO_OutputCDR &strm," << be_nl
      << "const " << fname << "_forany &_tao_array" << be_uidt_nl
      << ")" << be_uidt_nl
      << "{" << be_idt_nl;

  if (bt->accept (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_array_cdr_op_ci::"
                         "visit_array - "
                         "Base type codegen failed\n"),
                        -1);
    }

  *os << "}" << be_nl << be_nl;

  this->ctx_->sub_state (TAO_CodeGen::TAO_CDR_INPUT);
  *os << "ACE_INLINE" << be_nl
      << "CORBA::Boolean operator>> (" << be_idt << be_idt_nl
      << "TAO_InputCDR &strm," << be_nl
      << fname << "_forany &_tao_array" << be_uidt_nl
      << ")" << be_uidt_nl
      << "{" << be_idt_nl;

  if (bt->accept (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_array_cdr_op_ci::"
                         "visit_array - "
                         "Base type codegen failed\n"),
                        -1);
    }

  *os << "}";

  node->cli_inline_cdr_op_gen (1);
  return 0;
}

// Handle all the base types.

int
be_visitor_array_cdr_op_ci::visit_enum (be_enum *node)
{
  return this->visit_node (node);
}

int
be_visitor_array_cdr_op_ci::visit_interface (be_interface *node)
{
  return this->visit_node (node);
}

int
be_visitor_array_cdr_op_ci::visit_interface_fwd (be_interface_fwd *node)
{
  return this->visit_node (node);
}

int
be_visitor_array_cdr_op_ci::visit_valuetype (be_valuetype *node)
{
  return this->visit_node (node);
}

int
be_visitor_array_cdr_op_ci::visit_valuetype_fwd (be_valuetype_fwd *node)
{
  return this->visit_node (node);
}

int
be_visitor_array_cdr_op_ci::visit_predefined_type (be_predefined_type *node)
{
  TAO_OutStream *os = this->ctx_->stream ();

  switch (node->pt ())
    {
    case AST_PredefinedType::PT_pseudo:
    case AST_PredefinedType::PT_object:
    case AST_PredefinedType::PT_any:
      // Let the helper handle this.
      return this->visit_node (node);
    case AST_PredefinedType::PT_void:
      // error
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_array_cdr_op_ci::"
                         "visit_predefined_type - "
                         "Bad primitive type\n"), 
                        -1);
    default:
      // All other primitive types. Handle them as shown below.
      break;
    };

  // We get here if the "type" of individual elements of the array is a
  // primitive type. In this case, we treat the array as a single dimensional
  // array (even though it was multi-dimensional), and pass the total length
  // of the array as a cross product of the dimensions.

  unsigned long i;

  // Grab the array node.
  be_array *array = this->ctx_->be_node_as_array ();

  if (!node)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_array_cdr_op_ci::"
                         "visit_predefined_type - "
                         "bad array node\n"),
                        -1);
    }

  // We generate optimized code based on an optimized interface available from
  // the CDR class. These optimizations are applicable only to primitive
  // types.
  *os << " return strm.";

  // Based on our substate, we may be reading from a stream or writing into a
  // stream.
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
                         "(%N:%l) be_visitor_array_cdr_op_ci::"
                         "visit_predefined_type - "
                         "bad sub state\n"),
                        -1);
    }

  // Determine what kind of array are we reading/writing.
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
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_array_cdr_op_ci::"
                         "visit_predefined_type - "
                         "bad primitive type for optimized code gen\n"),
                        -1);
    }

  // Handle special case to avoid compiler errors.
  switch (this->ctx_->sub_state ())
    {
    case TAO_CodeGen::TAO_CDR_INPUT:
      *os << " ((";
      break;
    case TAO_CodeGen::TAO_CDR_OUTPUT:
      *os << " ((const ";
      break;
    default:
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_array_cdr_op_ci::"
                         "visit_predefined_type - "
                         "bad substate in context\n"),
                        -1);
    }

  switch (node->pt ())
    {
    case AST_PredefinedType::PT_long:
      *os << "ACE_CDR::Long *)";
      break;
    case AST_PredefinedType::PT_ulong:
      *os << "ACE_CDR::ULong *)";
      break;
    case AST_PredefinedType::PT_short:
      *os << "ACE_CDR::Short *)";
      break;
    case AST_PredefinedType::PT_ushort:
      *os << "ACE_CDR::UShort *)";
      break;
    case AST_PredefinedType::PT_octet:
      *os << "ACE_CDR::Octet *)";
      break;
    case AST_PredefinedType::PT_char:
      *os << "ACE_CDR::Char *)";
      break;
    case AST_PredefinedType::PT_wchar:
      *os << "ACE_CDR::WChar *)";
      break;
    case AST_PredefinedType::PT_float:
      *os << "ACE_CDR::Float *)";
      break;
    case AST_PredefinedType::PT_double:
      *os << "ACE_CDR::Double *)";
      break;
    case AST_PredefinedType::PT_longlong:
      *os << "ACE_CDR::LongLong *)";
      break;
    case AST_PredefinedType::PT_ulonglong:
      *os << "ACE_CDR::ULongLong *)";
      break;
    case AST_PredefinedType::PT_longdouble:
      *os << "ACE_CDR::LongDouble *)";
      break;
    case AST_PredefinedType::PT_boolean:
      *os << "ACE_CDR::Boolean *)";
      break;
    default:
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_array_cdr_op_ci::"
                         "visit_predefined_type - "
                         "bad primitive type for optimized code gen\n"),
                        -1);
    }

  // Handle special case to avoid compiler errors.
  switch (this->ctx_->sub_state ())
    {
    case TAO_CodeGen::TAO_CDR_INPUT:
      *os << " _tao_array.out (), ";
      break;
    case TAO_CodeGen::TAO_CDR_OUTPUT:
      *os << "_tao_array.in (), ";
      break;
    default:
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_array_cdr_op_ci::"
                         "visit_predefined_type - "
                         "bad substate in context\n"),
                        -1);
    }

  unsigned long ndims = array->n_dims ();

  // Generate a product of all the dimensions. This will be the total length
  // of the "unfolded" single dimensional array.
  for (i = 0; i < ndims; ++i)
    {
      // Retrieve the ith dimension value.
      AST_Expression *expr = array->dims ()[i];

      if ((expr == 0) || ((expr != 0) && (expr->ev () == 0)))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_array_cdr_op_ci::"
                             "visit_predefined_type - "
                             "bad array dimension\n"),
                            -1);
        }

      if (i != 0)
        {
          // Do not generate the multiplication operator the first time in.
          *os << "*";
        }

      if (expr->ev ()->et == AST_Expression::EV_ulong)
        {
          // Generate a loop for each dimension.
          *os << expr->ev ()->u.ulval;
        }
      else
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_array_cdr_op_ci::"
                             "visit_predefined_type - "
                             "bad array dimension value\n"),
                            -1);
        }
    }

  *os << ");" << be_uidt_nl;

  return 0;
}

int
be_visitor_array_cdr_op_ci::visit_sequence (be_sequence *node)
{
  return this->visit_node (node);
}

int
be_visitor_array_cdr_op_ci::visit_string (be_string *node)
{
  return this->visit_node (node);
}

int
be_visitor_array_cdr_op_ci::visit_structure (be_structure *node)
{
  return this->visit_node (node);
}

int
be_visitor_array_cdr_op_ci::visit_union (be_union *node)
{
  return this->visit_node (node);
}

int
be_visitor_array_cdr_op_ci::visit_typedef (be_typedef *node)
{
  this->ctx_->alias (node);

  // The node to be visited in the base primitve type that gets typedefed.
  be_type *bt = node->primitive_base_type ();

  if (!bt || (bt->accept (this) == -1))
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_array_cdr_op_ci::"
                         "visit_typedef - "
                         "Bad primitive type\n"), 
                        -1);
    }

  this->ctx_->alias (0);
  return 0;
}

// Helper.
int
be_visitor_array_cdr_op_ci::visit_node (be_type *bt)
{
  TAO_OutStream *os = this->ctx_->stream ();
  unsigned long i;
  be_array *node = this->ctx_->be_node_as_array ();
  AST_Decl::NodeType nt = bt->node_type ();

  if (!node)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_array_cdr_op_ci::"
                         "visit_node - "
                         "bad array node\n"),
                        -1);
    }

  // Initialize a boolean variable.
  *os << "CORBA::Boolean _tao_marshal_flag = 1;" << be_nl;

  unsigned long ndims = node->n_dims ();

  // We get here if the "type" of individual elements of the array is not a
  // primitive type. In this case, we are left with no other alternative but to
  // encode/decode element by element.

  // generate nested loops for as many dimensions as there are
  for (i = 0; i < ndims; ++i)
    {
      // Retrieve the ith dimension value.
      AST_Expression *expr = node->dims ()[i];

      if ((expr == 0) || ((expr != 0) && (expr->ev () == 0)))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_array_cdr_op_ci::"
                             "visit_node - "
                             "bad array dimension\n"),
                            -1);
        }

      if (expr->ev ()->et == AST_Expression::EV_ulong)
        {
          // Generate a loop for each dimension.
          *os << be_nl << "for (CORBA::ULong i" << i 
              << " = 0; i" << i << " < "
              << expr->ev ()->u.ulval << " && _tao_marshal_flag; i" << i
              << "++)" << be_idt_nl
              << "{" << be_idt;
        }
      else
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_array_cdr_op_ci::"
                             "visit_node - "
                             "bad array dimension value\n"),
                            -1);
        }
    }

  // @@ (JP) Need to factor out some of this into method call(s).
  switch (this->ctx_->sub_state ())
    {
    case TAO_CodeGen::TAO_CDR_INPUT:
      *os << be_nl;

      // Handle the array of array case, where we need to pass the
      // forany type.
      if (nt == AST_Decl::NT_array)
        {
          *os << bt->name () << "_forany tmp ("
              << bt->name () << "_alloc ());" << be_nl;
          *os << "_tao_marshal_flag = (strm >> tmp);" << be_nl;
          *os << bt->name () << "_copy (_tao_array";

          for (i = 0; i < ndims; ++i)
            {
              *os << "[i" << i << "]";
            }

          *os << ", tmp.in ());" << be_nl;
          *os << bt->name () << "_free (tmp.inout ());";
        }
      else
        {
          *os << "_tao_marshal_flag = (strm >> ";
          *os << "_tao_array ";

          for (i = 0; i < ndims; ++i)
            {
              *os << "[i" << i << "]";
            }

          switch (bt->node_type ())
            {
              // The following have a _var type and must be
              // handled in a special way.
              case AST_Decl::NT_string:
              case AST_Decl::NT_wstring:
              case AST_Decl::NT_valuetype:
              case AST_Decl::NT_valuetype_fwd:
                *os << ".out ()";
                break;
              case AST_Decl::NT_pre_defined:
                {
                  // We need to separately handle this case of pseudo
                  // objects because they have a _var type.
                  be_predefined_type *pt =
                    be_predefined_type::narrow_from_decl (bt);

                  if (!pt)
                    {
                      ACE_ERROR_RETURN ((LM_ERROR,
                                         "(%N:%l) be_visitor_array_cdr_op_ci::"
                                         "visit_node - "
                                         "bad predefined type node\n"),
                                        -1);
                    }

                  AST_PredefinedType::PredefinedType pdt = pt->pt ();

                  if (pdt == AST_PredefinedType::PT_pseudo
                      || pdt == AST_PredefinedType::PT_object)
                    {
                      *os << ".out ()";
                    }
                }
              default:
                break;
            }

          *os << ");";
        }

      break;
    case TAO_CodeGen::TAO_CDR_OUTPUT:
      *os << be_nl;

      // Handle the array of array case, where we need to pass the
      // forany type.
      if (nt == AST_Decl::NT_array)
        {
          *os << bt->name () << "_var tmp_var ("
              << bt->name () << "_dup (_tao_array";

          for (i = 0; i < ndims; ++i)
            {
              *os << "[i" << i << "]";
            }

          *os << "));" << be_nl;
          *os << bt->name () << "_forany tmp (tmp_var.inout ());" << be_nl;
          *os << "_tao_marshal_flag = (strm << tmp);";
        }
      else if (nt == AST_Decl::NT_interface 
               || nt == AST_Decl::NT_interface_fwd)
        {
          *os << "_tao_marshal_flag = " << be_idt_nl;

          if (bt->is_defined ())
            {    
              *os << "_tao_array";

              for (i = 0; i < ndims; ++i)
                {
                  *os << "[i" << i << "]";
                }

              *os << ".in ()->marshal (strm);" << be_uidt;
            }
          else
            {
              
              AST_Decl *parent = 
                ScopeAsDecl (bt->defined_in ());

              if (parent != 0 && parent->node_type () != AST_Decl::NT_root)
                {
                  *os << parent->name () << "::";
                }

              *os << "tao_" << bt->local_name () << "_life::"
                  << "tao_marshal (_tao_array";
              
              for (i = 0; i < ndims; ++i)
                {
                  *os << "[i" << i << "]";
                }

              *os << ".in (), strm);" << be_uidt;
            }
        }
      else
        {
          *os << "_tao_marshal_flag = (strm << ";
          *os << "_tao_array ";

          for (i = 0; i < ndims; ++i)
            {
              *os << "[i" << i << "]";
            }

          switch (bt->node_type ())
            {
              // The following have a _var type and must be
              // handled in a special way.
              case AST_Decl::NT_string:
              case AST_Decl::NT_wstring:
              case AST_Decl::NT_valuetype:
              case AST_Decl::NT_valuetype_fwd:
                *os << ".in ()";
                break;
              case AST_Decl::NT_pre_defined:
                {
                  // We need to separately handle this case of pseudo
                  // objects because they have a _var type.
                  be_predefined_type *pt =
                    be_predefined_type::narrow_from_decl (bt);

                  if (!pt)
                    {
                      ACE_ERROR_RETURN ((LM_ERROR,
                                         "(%N:%l) be_visitor_array_cdr_op_ci::"
                                         "visit_node - "
                                         "bad predefined type node\n"),
                                        -1);
                    }

                  AST_PredefinedType::PredefinedType pdt = pt->pt ();

                  if (pdt == AST_PredefinedType::PT_pseudo
                      || pdt == AST_PredefinedType::PT_object)
                    {
                      *os << ".in ()";
                    }
                }
              default:
                break;
            }

          *os << ");";
        }

      break;
    default:
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_array_cdr_op_ci::"
                         "visit_node - "
                         "bad sub state\n"),
                        -1);
    }

  for (i = 0; i < ndims; ++i)
    {
      // Decrement indentation as many times as the number of dimensions.
      *os << be_uidt_nl << "}" << be_uidt;
    }

  *os << be_nl << be_nl << "return _tao_marshal_flag;" << be_uidt_nl;

  return 0;
}
