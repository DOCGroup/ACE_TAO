//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    gen_bounded_wstr_sequence_cs.cpp
//
// = DESCRIPTION
//    Visitor generating code for bounded wstring sequence in the client source
//
// = AUTHOR
//    Jeff Parsons <parsons@cs.wustl.edu>
//
// ============================================================================

#include	"be.h"

#include "be_visitor_sequence.h"


int
be_visitor_sequence_cs::gen_bounded_wstr_sequence (be_sequence *node)
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
                         "visit_sequence - "
                         "Bad element type\n"), -1);
    }

  const char * class_name = node->instance_name ();

  static char full_class_name [NAMEBUFSIZE];
  ACE_OS::memset (full_class_name, '\0', NAMEBUFSIZE);

  if (node->is_nested ())
    {
      ACE_OS::sprintf (full_class_name, "%s::%s",
                       be_scope::narrow_from_scope (node->defined_in ())->decl ()->full_name (),
                       class_name);
    }
  else
    {
      ACE_OS::sprintf (full_class_name, "%s",
                       class_name);
    }


  // get the visitor for the type of the sequence
  be_visitor_context ctx (*this->ctx_);
  ctx.state (TAO_CodeGen::TAO_SEQUENCE_BASE_CS);
  be_visitor *visitor = tao_cg->make_visitor (&ctx);

  // !! branching in either compile time template instantiation
  // or manual template instatiation
  os->gen_ifdef_AHETI();

  os->gen_ifdef_macro (class_name);

  os->indent ();

  // allocate_buffer
  *os << "void" << be_nl
      << full_class_name << "::_allocate_buffer (CORBA::ULong /* length */)"
      << be_nl
      << "{" << be_idt_nl
      << "// For this class memory is never reallocated so the implementation"
      << be_nl
      << "// is *really* simple." << be_nl
      << "this->buffer_ = " << class_name << "::allocbuf ("
      << node->max_size () << ");" << be_uidt_nl
      << "}" << be_nl
      << be_nl;

  // deallocate_buffer
  *os << "void" << be_nl
      << full_class_name << "::_deallocate_buffer (void)" << be_nl
      << "{" << be_idt_nl
      << "if (this->buffer_ == 0 || this->release_ == 0)" << be_idt_nl
      << "return;" << be_uidt_nl
      << "CORBA::WChar **tmp =" << be_idt_nl
      << "ACE_reinterpret_cast (CORBA::WChar **, this->buffer_);"
      << be_uidt_nl
      << class_name << "::freebuf (tmp);" << be_nl
      << "this->buffer_ = 0;" << be_uidt_nl
      << "}" << be_nl
      << be_nl;

  // destructor
  *os << full_class_name << "::~" << class_name << " (void)" << be_nl
      << "{" << be_idt_nl
      << "this->_deallocate_buffer ();" << be_uidt_nl
      << "}" << be_nl
      << be_nl;

  // shrink_buffer
  *os << "void" << be_nl
      << full_class_name << "::_shrink_buffer (CORBA::ULong nl, CORBA::ULong ol)" << be_nl
      << "{" << be_idt_nl
      << "CORBA::WChar **tmp = ACE_reinterpret_cast (CORBA::WChar **,this->buffer_);" << be_nl
      << be_nl
      << "for (CORBA::ULong i = nl; i < ol; ++i)" << be_nl
      << "{" << be_idt_nl
      << "CORBA::wstring_free (tmp[i]);" << be_nl
      << "tmp[i] = 0;" << be_uidt_nl
      << "}" << be_uidt_nl
      << "}" << be_nl;

  os->gen_endif (); // endif macro

  // generate #endif for AHETI
  os->gen_endif_AHETI();


  delete visitor;
  return 0;
}
