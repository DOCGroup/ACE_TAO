//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    cdr_op_cs.cpp
//
// = DESCRIPTION
//    Visitor for code generation of Arrays for the CDR operators in the client
//    stubs.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#include	"idl.h"
#include	"idl_extern.h"
#include	"be.h"

#include "be_visitor_array.h"

ACE_RCSID(be_visitor_array, cdr_op_cs, "$Id$")


// ***************************************************************************
// Array visitor for generating CDR operator declarations in the client
// stubs file
// ***************************************************************************

be_visitor_array_cdr_op_cs::be_visitor_array_cdr_op_cs
(be_visitor_context *ctx)
  : be_visitor_decl (ctx)
{
}

be_visitor_array_cdr_op_cs::~be_visitor_array_cdr_op_cs (void)
{
}

int
be_visitor_array_cdr_op_cs::visit_array (be_array *node)
{
  if (this->ctx_->alias ())
    {
      // we are here because the base type of the array node is itself an array
      // i.e., this is a case of array of array
      return this->visit_node (node);
    }
  else
    {
      TAO_OutStream *os = this->ctx_->stream ();

      be_type *bt; // base type of the array

      if (node->cli_stub_cdr_op_gen () || node->imported ())
        return 0;

      // retrieve the type
      bt = be_type::narrow_from_decl (node->base_type ());
      if (!bt)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "be_visitor_array_cdr_op_cs::"
                             "visit_array - "
                             "Bad base type\n"),
                            -1);
        }

      // for anonymous arrays, the type name has a _ prepended. We compute the
      // fullname with or without the underscore and use it later on.
      char fname [NAMEBUFSIZE];  // to hold the full and
      
      // save the node's local name and full name in a buffer for quick use later
      // on 
      ACE_OS::memset (fname, '\0', NAMEBUFSIZE);
      if (this->ctx_->tdef ())
        {
          // typedefed node
          ACE_OS::sprintf (fname, "%s", node->fullname ());
        }
      else
        {
          // for anonymous arrays ...
          // we have to generate a name for us that has an underscope prepended to
          // our local name. This needs to be inserted after the parents's name
          
          if (node->is_nested ())
            {
              be_decl *parent = be_scope::narrow_from_scope (node->defined_in ())->decl ();
              ACE_OS::sprintf (fname, "%s::_%s", parent->fullname (), 
                               node->local_name ()->get_string ());
            }
          else
            {
              ACE_OS::sprintf (fname, "_%s", node->fullname ());
            }
        }

      // generate the CDR << and >> operator defns

      // save the array node for further use
      this->ctx_->node (node);

      //  set the sub state as generating code for the output operator
      this->ctx_->sub_state (TAO_CodeGen::TAO_CDR_OUTPUT);
      *os << "ACE_INLINE CORBA::Boolean operator<< (TAO_OutputCDR &strm, "
          << "const " << fname << "_forany &_tao_array)" << be_nl
          << "{" << be_idt_nl;

      if (bt->accept (this) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "be_visitor_array_cdr_op_cs::"
                             "visit_array - "
                             "Base type codegen failed\n"),
                            -1);
        }
      *os << "}\n\n";

      //  set the sub state as generating code for the input operator
      os->indent ();
      this->ctx_->sub_state (TAO_CodeGen::TAO_CDR_INPUT);
      *os << "ACE_INLINE CORBA::Boolean operator>> (TAO_InputCDR &strm, "
          << fname << "_forany &_tao_array)" << be_nl
          << "{" << be_idt_nl;
      if (bt->accept (this) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "be_visitor_array_cdr_op_cs::"
                             "visit_array - "
                             "Base type codegen failed\n"),
                            -1);
        }
      *os << "}\n\n";

      node->cli_stub_cdr_op_gen (1);
    }
  return 0;
}

// handle all the base types

int
be_visitor_array_cdr_op_cs::visit_enum (be_enum *node)
{
  return this->visit_node (node);
}

int
be_visitor_array_cdr_op_cs::visit_interface (be_interface *node)
{
  return this->visit_node (node);
}

int
be_visitor_array_cdr_op_cs::visit_interface_fwd (be_interface_fwd *node)
{
  return this->visit_node (node);
}

int
be_visitor_array_cdr_op_cs::visit_predefined_type (be_predefined_type *node)
{
  TAO_OutStream *os = this->ctx_->stream ();

  switch (node->pt ())
    {
    case AST_PredefinedType::PT_pseudo:
    case AST_PredefinedType::PT_any:
      // let the helper handle this
      return this->visit_node (node);
    case AST_PredefinedType::PT_void:
      // error
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_array_cdr_op_cs::"
                         "visit_predefined_type - "
                         "Bad primitive type\n"
                         ), -1);
    default:
      // all other primitive types. Handle them as shown below
      break;
    };

  // we get here if the "type" of individual elements of the array is a
  // primitive type. In this case, we treat the array as a single dimensional
  // array (even though it was multi-dimensional), and pass the total length of
  // the array as a cross product of the dimensions

  // index
  unsigned long i;

  // grab the array node
  be_array *array = this->ctx_->be_node_as_array ();

  if (!node)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_array_cdr_op_cs::"
                         "visit_predefined_type - "
                         "bad array node\n"),
                        -1);
    }

  // we generate optimized code based on an optimized interface available from
  // the CDR class. These optimizations are applicable only to primitive
  // types. 
  *os << " return strm."; 

  // based on our substate, we may be reading from a stream or writing into a
  // stream 
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
                         "(%N:%l) be_visitor_array_cdr_op_cs::"
                         "visit_predefined_type - "
                         "bad sub state\n"),
                        -1);
    }

  // determine what kind of array are we reading/writing
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
                         "(%N:%l) be_visitor_array_cdr_op_cs::"
                         "visit_predefined_type - "
                         "bad primitive type for optimized code gen\n"),
                        -1);
    }

  // handle special case to avoid compiler errors
  switch (node->pt ())
    {
    case AST_PredefinedType::PT_char:
      switch (this->ctx_->sub_state ())
        {
        case TAO_CodeGen::TAO_CDR_INPUT:
          *os << " ((char *)_tao_array.inout (), ";
          break;
        case TAO_CodeGen::TAO_CDR_OUTPUT:
          *os << " ((const char *)_tao_array.in (), ";
          break;
        }
      break;
    default:
      switch (this->ctx_->sub_state ())
        {
        case TAO_CodeGen::TAO_CDR_INPUT:
          *os << " (_tao_array.inout (), ";
          break;
        case TAO_CodeGen::TAO_CDR_OUTPUT:
          *os << " (_tao_array.in (), ";
          break;
        }
      break;
    }
  // generate a product of all the dimensions. This will be the total length of
  // the "unfolded" single dimensional array.
  for (i = 0; i < array->n_dims (); i++)
    {
      // retrieve the ith dimension value
      AST_Expression *expr = array->dims ()[i];
      // dimension value
      if ((expr == NULL) || ((expr != NULL) && (expr->ev () == NULL)))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_array_cdr_op_cs::"
                             "visit_predefined_type - "
                             "bad array dimension\n"),
                            -1);
        }
      if (i != 0)
        // do not generate the multiplication operator the first time in
        *os << "*";
      if (expr->ev ()->et == AST_Expression::EV_ulong)
        {
          // generate a loop for each dimension
          *os << expr->ev ()->u.ulval;
        }
      else
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_array_cdr_op_cs::"
                             "visit_predefined_type - "
                             "bad array dimension value\n"),
                            -1);
        }
    }
  *os << ");" << be_uidt_nl;

  return 0;
}

int
be_visitor_array_cdr_op_cs::visit_sequence (be_sequence *node)
{
  return this->visit_node (node);
}

int
be_visitor_array_cdr_op_cs::visit_string (be_string *node)
{
  return this->visit_node (node);
}

int
be_visitor_array_cdr_op_cs::visit_structure (be_structure *node)
{
  return this->visit_node (node);
}

int
be_visitor_array_cdr_op_cs::visit_union (be_union *node)
{
  return this->visit_node (node);
}

int
be_visitor_array_cdr_op_cs::visit_typedef (be_typedef *node)
{
  this->ctx_->alias (node); // save the typedef node for use in code generation
                            // as we visit the base type

  // the node to be visited in the base primitve type that gets typedefed
  be_type *bt = node->primitive_base_type ();
  if (!bt || (bt->accept (this) == -1))
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_array_cdr_op_cs::"
                         "visit_typedef - "
                         "Bad primitive type\n"
                         ), -1);
    }

  this->ctx_->alias (0);
  return 0;
}

// helper
int
be_visitor_array_cdr_op_cs::visit_node (be_type *bt)
{
  TAO_OutStream *os = this->ctx_->stream ();
  unsigned long i;
  be_array *node = this->ctx_->be_node_as_array ();

  if (!node)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_array_cdr_op_cs::"
                         "visit_node - "
                         "bad array node\n"),
                        -1);
    }

  // initialize a boolean variable
  *os << "CORBA::Boolean _tao_marshal_flag = 1;" << be_nl;

  // we get here if the "type" of individual elements of the array is not a
  // primitive type. In this case, we are left with no other alternative but to
  // encode/decode element by element

  // generate nested loops for as many dimensions as there are
  for (i = 0; i < node->n_dims (); i++)
    {
      // retrieve the ith dimension value
      AST_Expression *expr = node->dims ()[i];
      // dimension value
      if ((expr == NULL) || ((expr != NULL) && (expr->ev () == NULL)))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_array_cdr_op_cs::"
                             "visit_node - "
                             "bad array dimension\n"),
                            -1);
        }
      if (expr->ev ()->et == AST_Expression::EV_ulong)
        {
          // generate a loop for each dimension
          *os << "for (CORBA::ULong i" << i << " = 0; i" << i << " < "
              << expr->ev ()->u.ulval << " && _tao_marshal_flag; i" << i 
              << "++)" << be_idt_nl;
        }
      else
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_array_cdr_op_cs::"
                             "visit_node - "
                             "bad array dimension value\n"),
                            -1);
        }
    }

  // if the type was a string, an obj ref, or a pseudo-obj, we are dealing with
  // the _var type and hence we must use the appropriate in () and out () methods
  switch (this->ctx_->sub_state ())
    {
    case TAO_CodeGen::TAO_CDR_INPUT:
      *os << "_tao_marshal_flag = (strm >> ";
      // handle the array of array case in which case, we need to pass the
      // forany type
      if (bt->node_type () == AST_Decl::NT_array)
        {
          *os << bt->name () << "_forany ((" << bt->name () 
              << "_slice *) ";
        }
      *os << "_tao_array "; 
      for (i = 0; i < node->n_dims (); i++)
        {
          *os << "[i" << i << "]";
        }
      switch (bt->node_type ())
        {
          // the following have a _var type and must be handled in a special way
        case AST_Decl::NT_string:
        case AST_Decl::NT_interface:
        case AST_Decl::NT_interface_fwd:
          *os << ".out ()";
          break;
        case AST_Decl::NT_pre_defined:
          {
            // we need to separately handle this case of pseudo objects because
            // they have a _var type
            be_predefined_type *pt = be_predefined_type::narrow_from_decl (bt);
            if (!pt)
              {
                ACE_ERROR_RETURN ((LM_ERROR,
                                   "(%N:%l) be_visitor_array_cdr_op_cs::"
                                   "visit_node - "
                                   "bad predefined type node\n"),
                                  -1);
              }
            if (pt->pt () == AST_PredefinedType::PT_pseudo)
              {
                *os << ".out ()";
              }
          }
        }
      if (bt->node_type () == AST_Decl::NT_array)
        {
          *os << ")";
        }
      *os << ");";
      break;
    case TAO_CodeGen::TAO_CDR_OUTPUT:
      *os << "_tao_marshal_flag = (strm << ";
      // handle the array of array case in which case, we need to pass the
      // forany type
      if (bt->node_type () == AST_Decl::NT_array)
        {
          *os << bt->name () << "_forany ((" << bt->name () 
              << "_slice *) ";
        }
      *os << "_tao_array "; 
      for (i = 0; i < node->n_dims (); i++)
        {
          *os << "[i" << i << "]";
        }
      switch (bt->node_type ())
        {
          // the follwoing three have a _var type and must be handled in a
          // special way
        case AST_Decl::NT_string:
        case AST_Decl::NT_interface:
        case AST_Decl::NT_interface_fwd:
          *os << ".in ()";
          break;
        case AST_Decl::NT_pre_defined:
          {
            be_predefined_type *pt = be_predefined_type::narrow_from_decl (bt);
            if (!pt)
              {
                ACE_ERROR_RETURN ((LM_ERROR,
                                   "(%N:%l) be_visitor_array_cdr_op_cs::"
                                   "visit_node - "
                                   "bad predefined type node\n"),
                                  -1);
              }
            if (pt->pt () == AST_PredefinedType::PT_pseudo)
              {
                *os << ".in ()";
              }
          }
        }
      if (bt->node_type () == AST_Decl::NT_array)
        {
          // array of array case
          *os << ")";
        }
      *os << ");";
      break;
    default:
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_array_cdr_op_cs::"
                         "visit_node - "
                         "bad sub state\n"),
                        -1);
    }

  for (i = 0; i < node->n_dims (); i++)
    {
      // decrement indentation as many times as the number of dimensions
      *os << be_uidt;
    }
  *os << be_nl;
  *os << "return _tao_marshal_flag;" << be_uidt_nl;

  return 0;
}
