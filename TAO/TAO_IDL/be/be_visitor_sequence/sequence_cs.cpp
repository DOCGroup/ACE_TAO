//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    sequence_cs.cpp
//
// = DESCRIPTION
//    Visitor generating code for Sequences in the client stubs file
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#include	"idl.h"
#include	"idl_extern.h"
#include	"be.h"

#include "be_visitor_sequence.h"

ACE_RCSID(be_visitor_sequence, sequence_cs, "$Id$")


// ************************************************************
// Root visitor for client stub class
// ************************************************************

be_visitor_sequence_cs::be_visitor_sequence_cs (be_visitor_context *ctx)
  : be_visitor_decl (ctx)
{
}

be_visitor_sequence_cs::~be_visitor_sequence_cs (void)
{
}

// XXXASG - this method is same as that in the _ch visitor. So we need some
// abstraction for this

int
be_visitor_sequence_cs::gen_base_sequence_class (be_sequence *node)
{
  TAO_OutStream *os = this->ctx_->stream ();
  be_type *bt;


  // retrieve the base type since we may need to do some code
  // generation for the base type.
  bt = be_type::narrow_from_decl (node->base_type ());
  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_sequence_cs::"
                         "gen_base_sequence_class - "
                         "Bad element type\n"), -1);
    }

  os->gen_ifdef_AHETI();

  // this is the instantiation branch
  *os << node->instance_name ();

  os->gen_else_AHETI();

  // generate the appropriate sequence type
  switch (node->managed_type ())
    {
    case be_sequence::MNG_OBJREF: // sequence of objrefs
      if (node->unbounded ())
        *os << "TAO_Unbounded_Object_Sequence<";
      else
        *os << "TAO_Bounded_Object_Sequence<";
      break;
    case be_sequence::MNG_STRING: // sequence of strings
      if (node->unbounded ())
        *os << "TAO_Unbounded_String_Sequence";
      else
        *os << "TAO_Bounded_String_Sequence";
      break;
    default: // not a managed type
      if (node->unbounded ())
        *os << "TAO_Unbounded_Sequence<";
      else
        *os << "TAO_Bounded_Sequence<";
      break;
    }

  be_visitor_context ctx (*this->ctx_);
  ctx.state (TAO_CodeGen::TAO_SEQUENCE_BASE_CS);
  be_visitor *visitor = tao_cg->make_visitor (&ctx);

  if (bt->accept (visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_sequence_cs::"
                         "visit_sequence - "
                         "base type visit failed\n"),
                        -1);
    }
  delete visitor;

  // find out if the sequence is of a managed type and if it is bounded or not
  if (node->managed_type () == be_sequence::MNG_STRING)
    {
      if (!node->unbounded ())
        {
          *os << "<" << node->max_size () << ">";
        }
    }
  else
    {
      if (node->unbounded ())
        {
          *os << ">";
        }
      else
        {
          *os << ", " << node->max_size () << ">";
        }
    }

  os->gen_endif_AHETI();

  return 0;
}

int be_visitor_sequence_cs::visit_sequence (be_sequence *node)
{
  // generate the constructors
  be_type *bt;       // type node
  TAO_OutStream *os = this->ctx_->stream ();

  if (node->cli_stub_gen () || node->imported ())
    return 0;

  // generate the ifdefined macro for the sequence type
  os->gen_ifdef_macro (node->flatname ());

  os->indent (); // start with the current indentation level

  // retrieve the base type since we may need to do some code
  // generation for the base type.
  bt = be_type::narrow_from_decl (node->base_type ());
  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_sequence_cs::"
                         "visit_sequence - "
                         "Bad element type\n"), -1);
    }

  *os << "// *************************************************************"
      << be_nl
      << "// " << node->name () << be_nl
      << "// *************************************************************"
      << be_nl << be_nl;

  // default constructor
  *os << node->name () << "::" << node->local_name () << " (void)" << be_nl
      << "{}" << be_nl;

  // for unbounded sequences, we have a different set of constructors
  if (node->unbounded ())
    {
      *os << node->name () << "::" << node->local_name ()
          << " (CORBA::ULong max) // uses max size" << be_nl
          << "  : ";

      // pass it to the base constructor
      if (this->gen_base_sequence_class (node) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_sequence_cs::"
                             "visit_sequence - "
                            "codegen for base sequence class\n"), -1);
        }


      *os << " (max)" << be_nl
          << "{}" << be_nl;
    }

  // constructor with the buffer
  *os << node->name () << "::" << node->local_name () << " (";
  if (node->unbounded ())
    {
      *os << "CORBA::ULong max, ";  // unbounded seq takes this extra parameter
    }
  *os << "CORBA::ULong length, ";
  // generate the base type for the buffer
  be_visitor_context ctx (*this->ctx_);
  ctx.state (TAO_CodeGen::TAO_SEQUENCE_BUFFER_TYPE_CS);
  be_visitor *visitor = tao_cg->make_visitor (&ctx);
  if (!visitor)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_sequence_cs::"
                         "visit_sequence - "
                         "Bad visitor\n"), -1);
    }

  if (bt->accept (visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_sequence_cs::"
                         "visit_sequence - "
                         "base type visit failed\n"),
                        -1);
    }
  delete visitor;
  *os << " *buffer, CORBA::Boolean release)" << be_nl
      << "  : ";
  // pass it to the base constructor
  if (this->gen_base_sequence_class (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_sequence_cs::"
                         "visit_sequence - "
                         "codegen for base sequence class\n"), -1);
    }
  *os << " (";
  if (node->unbounded ())
    {
      *os << "max, ";
    }
  *os << "length, buffer, release)" << be_nl
      << "{}" << be_nl;

  // copy constructor
  *os << node->name () << "::" << node->local_name ()
      << " (const " << node->local_name ()
      << " &seq) // copy ctor" << be_nl
      << "  : ";
  // pass it to the base constructor
  if (this->gen_base_sequence_class (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_sequence_cs::"
                         "visit_sequence - "
                         "codegen for base sequence class\n"), -1);
    }
  *os << " (seq)" << be_nl
      << "{}" << be_nl;

  // destructor
  *os << node->name () << "::~" << node->local_name ()
      << " (void) // dtor" << be_nl
      << "{}\n\n";

  if (!this->ctx_->tdef ())
    {
      // by using a visitor to declare and define the TypeCode, we have the
      // added advantage to conditionally not generate any code. This will be
      // based on the command line options. This is still TO-DO
      ctx = *this->ctx_;
      ctx.state (TAO_CodeGen::TAO_TYPECODE_DEFN);
      visitor = tao_cg->make_visitor (&ctx);
      if (!visitor || (node->accept (visitor) == -1))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_sequence_cs::"
                             "visit_sequence - "
                             "TypeCode definition failed\n"
                             ), -1);
        }
    }

  os->gen_endif ();

  return 0;
}
