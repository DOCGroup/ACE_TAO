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
//    Visitor for code generation of Sequences for the CDR operators
//    in the client stubs.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#include	"idl.h"
#include	"idl_extern.h"
#include	"be.h"

#include "be_visitor_sequence.h"

ACE_RCSID(be_visitor_sequence, cdr_op_cs, "$Id$")


// ***************************************************************************
// Sequence visitor for generating CDR operator declarations in the client
// stubs file
// ***************************************************************************

be_visitor_sequence_cdr_op_cs::be_visitor_sequence_cdr_op_cs
(be_visitor_context *ctx)
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
      // we are here because the base type of the sequence node is itself a sequence
      // i.e., this is a case of sequence of sequence
      return this->visit_node (node);
    }
  else
    {
      TAO_OutStream *os = this->ctx_->stream ();

      be_type *bt; // base type of the sequence

      if (node->cli_stub_cdr_op_gen () || node->imported ())
        return 0;

      // retrieve the type
      bt = be_type::narrow_from_decl (node->base_type ());
      if (!bt)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "be_visitor_sequence_cdr_op_cs::"
                             "visit_sequence - "
                             "Bad base type\n"),
                            -1);
        }

      // generate the CDR << and >> operator defns

      // save the sequence node for further use
      this->ctx_->node (node);

      //  set the sub state as generating code for the output operator
      this->ctx_->sub_state (TAO_CodeGen::TAO_CDR_OUTPUT);
      *os << "ACE_INLINE CORBA::Boolean operator<< (" << be_idt << be_idt_nl
	  << "TAO_OutputCDR &strm," << be_nl
          << "const " << node->name ()
	  << " &_tao_sequence" << be_uidt_nl
	  << ")" << be_uidt_nl
          << "{" << be_idt_nl;

      // first encode the sequence length
      *os << "if (strm << _tao_sequence.length ())" << be_nl
          << "{" << be_idt_nl;
      // now encode the sequence elements
      *os << "// encode all elements" << be_nl;
      if (bt->accept (this) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "be_visitor_sequence_cdr_op_cs::"
                             "visit_sequence - "
                             "Base type codegen failed\n"),
                            -1);
        }
      *os << "}" << be_nl
          << "return 0; // error" << be_uidt_nl
          << "}\n\n";

      //  set the sub state as generating code for the input operator
      os->indent ();
      this->ctx_->sub_state(TAO_CodeGen::TAO_CDR_INPUT);
      *os << "ACE_INLINE CORBA::Boolean operator>> (TAO_InputCDR &strm, "
          << node->name () << " &_tao_sequence)" << be_nl
          << "{" << be_idt_nl;
      // first retrieve the length and adjust the sequence length accordingly
      *os << "CORBA::ULong _tao_seq_len;" << be_nl;
      *os << "if (strm >> _tao_seq_len)" << be_nl
          << "{" << be_idt_nl;
      // now check if the length does not exceed the maximum. We do this only
      // for bounded sequences

      AST_Expression *expr = node->max_size ();
      // dimension value
      if ((expr == NULL) || ((expr != NULL) && (expr->ev () == NULL)))
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
              // we are dealing with a bounded sequence. Check if we are within
              // bounds 
              *os << "if (_tao_seq_len <= _tao_sequence.maximum ())" << be_nl
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
      *os << "// set the length of the sequence" << be_nl
          << "_tao_sequence.length (_tao_seq_len);" << be_nl;
      *os << "// retrieve all the elements" << be_nl;
      if (bt->accept (this) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "be_visitor_sequence_cdr_op_cs::"
                             "visit_sequence - "
                             "Base type codegen failed\n"),
                            -1);
        }
      if (expr->ev ()->u.ulval > 0)
        {
          // we are dealing with a bounded sequence. 
          *os << "}" << be_uidt_nl;
        }
      *os << "}" << be_nl
          << "return 0; // error" << be_uidt_nl
          << "}\n\n";

      node->cli_stub_cdr_op_gen (1);
    }
  return 0;
}

// handle all the base types

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
be_visitor_sequence_cdr_op_cs::visit_predefined_type (be_predefined_type *node)
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
                         "(%N:%l) be_visitor_sequence_cdr_op_cs::"
                         "visit_predefined_type - "
                         "Bad primitive type\n"
                         ), -1);
    default:
      // all other primitive types. Handle them as shown below
      break;
    };

  // we get here if the "type" of individual elements of the sequence is a
  // primitive type. In this case, we treat the sequence as a single
  // dimensional sequence (even of it was multi-dimensional), and pass
  // the total length of the sequence as a cross product of the
  // dimensions

  // grab the sequence node
  be_sequence *sequence = this->ctx_->be_node_as_sequence ();

  if (!sequence)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_sequence_cdr_op_cs::"
                         "visit_predefined_type - "
                         "bad sequence node\n"),
                        -1);
    }
  
  // handle octet sequences using the optimizations provided by the TAO ORB
  // Core. If these optimizations are not available, then use the normal form
  
  if (node->pt () == AST_PredefinedType::PT_octet)
    {
      *os << "\n#if defined (TAO_NO_COPY_OCTET_SEQUENCES)" << be_nl;
      switch (this->ctx_->sub_state ())
        {
        case TAO_CodeGen::TAO_CDR_INPUT:
          {
            *os << "if (ACE_BIT_DISABLED (strm.start ()->flags (),"
                << "ACE_Message_Block::DONT_DELETE))" << be_nl
                << "{" << be_idt_nl
                << "TAO_Unbounded_Sequence<CORBA::Octet> *oseq = " << be_nl
                << "  ACE_dynamic_cast(TAO_Unbounded_Sequence<CORBA::Octet>*, "
                << "&_tao_sequence);" << be_nl;
            *os << "oseq->replace (_tao_seq_len, strm.start ());" 
                << be_nl
                << "oseq->mb ()->wr_ptr (oseq->mb()->rd_ptr () + "
                << "_tao_seq_len);" << be_nl
                << "strm.skip_bytes (_tao_seq_len);" << be_nl
                << "return 1;" << be_uidt_nl
                << "}" << be_nl
                << "else" << be_idt_nl
                << "return strm.read_octet_array ("
                << "_tao_sequence.get_buffer (), _tao_seq_len);"
                << be_uidt_nl;
          }
          break;
        case TAO_CodeGen::TAO_CDR_OUTPUT:
          {
            *os << "{" << be_idt_nl
                << "TAO_Unbounded_Sequence<CORBA::Octet> *oseq = " << be_nl
                << "  ACE_dynamic_cast (TAO_Unbounded_Sequence<CORBA::Octet>*, "
                << "(" << sequence->name () << " *)&_tao_sequence);" << be_nl;
            *os << "if (oseq->mb ())" << be_idt_nl
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
      *os << "\n#else /* TAO_NO_COPY_OCTET_SEQUENCES */" << be_nl;
    }
  
  *os << "return strm.";
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
                         "(%N:%l) be_visitor_sequence_cdr_op_cs::"
                         "visit_predefined_type - "
                         "bad sub state\n"),
                        -1);
    }
  
  // determine what kind of sequence are we reading/writing
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
  
  // handle special case to avoid compiler errors
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
  
  if (node->pt () == AST_PredefinedType::PT_octet)
    *os << "\n#endif /* TAO_NO_COPY_OCTET_SEQUENCES */" << be_nl;
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
  this->ctx_->alias (node); // save the typedef node for use in code generation
                            // as we visit the base type

  // the node to be visited in the base primitve type that gets typedefed
  be_type *bt = node->primitive_base_type ();
  if (!bt || (bt->accept (this) == -1))
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_sequence_cdr_op_cs::"
                         "visit_typedef - "
                         "Bad primitive type\n"
                         ), -1);
    }

  this->ctx_->alias (0);
  return 0;
}

// helper
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

  // initialize a boolean variable
  *os << "CORBA::Boolean _tao_marshal_flag = 1;" << be_nl;

  // we get here if the "type" of individual elements of the sequence is not a
  // primitive type. In this case, we are left with no other alternative but to
  // encode/decode element by element

  AST_Expression *expr = node->max_size ();
  // dimension value
  if ((expr == NULL) || ((expr != NULL) && (expr->ev () == NULL)))
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_sequence_cdr_op_cs::"
                         "visit_node - "
                         "bad sequence dimension\n"),
                        -1);
    }
  if (expr->ev ()->et == AST_Expression::EV_ulong)
    {
      *os << "for (CORBA::ULong i = 0; i < _tao_sequence.length ()"
	  << " && _tao_marshal_flag; "
          << "i++)" << be_idt_nl;
    }
  else
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_sequence_cdr_op_cs::"
                         "visit_node - "
                         "bad sequence dimension value\n"),
                        -1);
    }

  // if the type was a string, an obj ref, or a pseudo-obj, we are dealing with
  // a manager type and hence we must use the appropriate in () and out ()
  // methods.
  //

  switch (this->ctx_->sub_state ())
    {
    case TAO_CodeGen::TAO_CDR_INPUT:
      *os << "_tao_marshal_flag = (strm >> ";
      switch (bt->node_type ())
        {
        case AST_Decl::NT_string:
          {
            be_string *str = be_string::narrow_from_decl (bt);

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
                // unbounded
                *os << "_tao_sequence[i].out ()"; 
              }
            else
              {
                *os << "CORBA::Any::to_string (_tao_sequence[i].out (), "
                    << str->max_size ()->ev ()->u.ulval - 1 << ")"; 
              }
          }
          break;
        case AST_Decl::NT_interface:
        case AST_Decl::NT_interface_fwd:
          *os << "_tao_sequence[i].out ()"; 
          break;
        case AST_Decl::NT_pre_defined:
          {
            // we need to separately handle this case of pseudo objects because
            // they have a _var type
            be_predefined_type *pt = be_predefined_type::narrow_from_decl (bt);
            if (!pt)
              {
                ACE_ERROR_RETURN ((LM_ERROR,
                                   "(%N:%l) be_visitor_sequence_cdr_op_cs::"
                                   "visit_node - "
                                   "bad predefined type node\n"),
                                  -1);
              }
            if (pt->pt () == AST_PredefinedType::PT_pseudo)
              {
                *os << "_tao_sequence[i].out ()"; 
              }
            else
              {
                *os << "_tao_sequence[i]"; 
              }
          }
          break;
        default:
          *os << "_tao_sequence[i]"; 
        }
      *os << ");";
      break;
    case TAO_CodeGen::TAO_CDR_OUTPUT:
      *os << "_tao_marshal_flag = (strm << _tao_sequence[i]"; 
      switch (bt->node_type ())
        {
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
                                   "(%N:%l) be_visitor_sequence_cdr_op_cs::"
                                   "visit_node - "
                                   "bad predefined type node\n"),
                                  -1);
              }
            if (pt->pt () == AST_PredefinedType::PT_pseudo)
              {
                *os << ".in ()";
              }
          }
        default:
          break;
        }
      *os << ");";
      break;
    default:
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_sequence_cdr_op_cs::"
                         "visit_node - "
                         "bad sub state\n"),
                        -1);
    }

  *os << be_uidt_nl;
  *os << "return _tao_marshal_flag;" << be_uidt_nl;

  return 0;
}
