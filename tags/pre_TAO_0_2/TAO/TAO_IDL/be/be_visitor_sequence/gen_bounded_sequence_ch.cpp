// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    gen_bounded_sequence_ch.cpp
//
// = DESCRIPTION
//    Visitor generating code for Sequence in the client header
//
// = AUTHOR
//    Michael Kircher
//
// ============================================================================

//#include      "idl.h"
//#include      "idl_extern.h"
#include        "be.h"

#include "be_visitor_sequence.h"


int
be_visitor_sequence_ch::gen_bounded_sequence (be_sequence *node)
{
  TAO_OutStream *os = this->ctx_->stream ();
  be_type *bt;

  // retrieve the base type since we may need to do some code
  // generation for the base type.
  bt = be_type::narrow_from_decl (node->base_type ());
  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_sequence_ch::"
                         "visit_sequence - "
                         "Bad element type\n"), -1);
    }

  // generate the class name
  be_type  *pt; // base types

  if (bt->node_type () == AST_Decl::NT_typedef)
  {
    // get the primitive base type of this typedef node
    be_typedef *t = be_typedef::narrow_from_decl (bt);
    pt = t->primitive_base_type ();
  }
  else
    pt = bt;


  const char * class_name = node->instance_name ();


  be_visitor_context ctx (*this->ctx_);
  ctx.state (TAO_CodeGen::TAO_SEQUENCE_BASE_CH);
  be_visitor *visitor = tao_cg->make_visitor (&ctx);

  os->gen_ifdef_macro (class_name);

  os->indent ();

  *os << "class " << class_name << " : public TAO_Bounded_Base_Sequence" << be_nl
      << "{" << be_nl
      << "public:" << be_idt_nl
      << "// = Initialization and termination methods." << be_nl
      << be_nl;

  // Constructor
  *os << class_name << " (void)" << be_nl
      << "// Default constructor." << be_nl
      << "  : TAO_Bounded_Base_Sequence (" << node->max_size () << ", 0, 0, CORBA::B_FALSE)" << be_nl
      << "{" << be_nl
      << "  this->_allocate_buffer (" << node->max_size () << ");" << be_nl 
      << "}" << be_nl
      << be_nl;

  // constructor
  *os << class_name << " (CORBA::ULong length," << be_idt_nl;
  pt->accept (visitor); *os <<" *data," << be_nl
            << "CORBA::Boolean release=0)" << be_uidt_nl
      << "// Constructor using the data and memory management flag." << be_nl
      << "  : TAO_Bounded_Base_Sequence (" << node->max_size () << ", length, data, release)" << be_nl
      << "{" << be_nl
      << "  this->_allocate_buffer (" << node->max_size () << ");" << be_nl 
      << "}" << be_nl
      << be_nl;

  // constructor
  *os << class_name << " (const " << class_name << " &rhs)" << be_nl
      << "// Copy constructor." << be_idt_nl
      << ": TAO_Bounded_Base_Sequence (rhs)" << be_uidt_nl
      << "{" << be_idt_nl;
  pt->accept(visitor); *os <<" *tmp1 = allocbuf (" << node->max_size () << ");" << be_nl
      << be_nl;
  pt->accept(visitor); *os <<" * const tmp2 = ACE_reinterpret_cast (";
  pt->accept (visitor); *os << " * ACE_CAST_CONST, rhs.buffer_);" << be_nl
      << be_nl
      << "for (CORBA::ULong i = 0; i < this->length_; ++i)" << be_idt_nl
      << "tmp1[i] = tmp2[i];" << be_uidt_nl
      << be_nl
      << "this->buffer_ = tmp1;" << be_uidt_nl
      << "}" << be_nl
      << be_nl;

  // operator=
  *os << class_name << " &operator= (const " << class_name << " &rhs)" << be_nl
      << "// Assignment operator.  " << be_nl
      << "{" << be_idt_nl
      << "if (this == &rhs)" << be_idt_nl
      << "return *this;" << be_uidt_nl
      << be_nl
      << "if (this->release_)" << be_nl
      << "{" << be_nl
      << "}" << be_nl
      << "else" << be_idt_nl
      << "this->buffer_ = allocbuf (rhs.maximum_);" << be_nl
      << be_nl
      <<"TAO_Bounded_Base_Sequence::operator= (rhs);" << be_nl
      << be_nl;
  pt->accept(visitor); *os <<"* tmp1 = ACE_reinterpret_cast (";
  pt->accept(visitor); *os << " *, this->buffer_);" << be_nl;
  pt->accept(visitor); *os <<"* const tmp2 = ACE_reinterpret_cast (";
      pt->accept (visitor); *os << " * ACE_CAST_CONST, rhs.buffer_);" << be_nl
      << be_nl
      << "for (CORBA::ULong i = 0; i < this->length_; ++i)" << be_idt_nl
      << "tmp1[i] = tmp2[i];" << be_uidt_nl
      << be_nl
      << "return *this;" << be_uidt_nl << be_uidt_nl
      << "}" << be_nl
      << be_nl;

  // destructor
  *os << "~" << class_name << " (void) // Dtor." << be_nl
      << "{" << be_idt_nl
      << "this->_deallocate_buffer ();" << be_uidt_nl
      << "}" << be_nl
      << be_nl;

  // Accessors
  *os << "// = Accessors." << be_nl;
  pt->accept(visitor); *os <<" &operator[] (CORBA::ULong i)// operator []" << be_nl
      << "{" << be_idt_nl
      << "ACE_ASSERT (i < this->maximum_);" << be_nl;
  pt->accept(visitor); *os <<" *tmp = ACE_reinterpret_cast (";
  pt->accept (visitor); *os << "*,this->buffer_);" << be_nl
      << "return tmp[i];" << be_uidt_nl
      << "}" << be_nl
      << be_nl
      << "const "; pt->accept (visitor); *os << " &operator[] (CORBA::ULong i) const// operator []" << be_nl
      << "{" << be_idt_nl
      << "ACE_ASSERT (i < this->maximum_);" << be_nl
      << "const "; pt->accept (visitor); *os << "* tmp = ACE_reinterpret_cast (const ";
  pt->accept (visitor); *os << "* ACE_CAST_CONST,this->buffer_);" << be_nl
      << "return tmp[i];" << be_uidt_nl
      << "}" << be_nl
      << be_nl;

  // Static Operations
  *os << "// = Static operations." << be_nl
      << "static "; pt->accept (visitor); *os << " *allocbuf (CORBA::ULong) // Allocate storage for the sequence." << be_nl
      << "{" << be_idt_nl
      << "return new "; pt->accept (visitor); *os << "[" << node->max_size () << "];" << be_uidt_nl
      << "}" << be_nl
      << be_nl;

  // free_buf
  *os << "static void freebuf ("; pt->accept (visitor); *os << " *buffer) // Free the sequence." << be_nl
      << "{" << be_idt_nl
      << "delete [] buffer;" << be_uidt_nl
      << "}" << be_nl
      << be_nl;

  // allocate_buffer
  *os << "virtual void _allocate_buffer (CORBA::ULong length)" << be_nl
      << "// allocate a buffer of the requested length. The buffer is allocated for the" << be_nl
      << "// right type" << be_nl
      << "{" << be_idt_nl
      << "this->buffer_ = allocbuf (" << node->max_size () << ");" << be_uidt_nl
      << "}" << be_nl
      << be_nl;

  // deallocate_buffer
  *os << "virtual void _deallocate_buffer (void)" << be_nl
      << "// deallocate the buffer" << be_nl
      << "{" << be_idt_nl
      << "if (this->buffer_ == 0 || this->release_ == 0)" << be_idt_nl
      << "return;" << be_uidt_nl;
  pt->accept(visitor); *os <<" *tmp = ACE_reinterpret_cast (";
  pt->accept (visitor); *os << " *, this->buffer_);" << be_nl
      << "freebuf (tmp);" << be_nl
      << "this->buffer_ = 0;" << be_uidt_nl
      << "}" << be_nl
      << be_nl;

  // get_buffer
  pt->accept(visitor); *os <<" *get_buffer (CORBA::Boolean orphan = CORBA::B_FALSE)" << be_nl
      << "{" << be_idt_nl;
  pt->accept(visitor); *os <<" *result = 0;" << be_nl
      << "if (orphan == CORBA::B_FALSE)" << be_nl
      << "{" << be_idt_nl
      << "// We retain ownership." << be_nl
      << "if (this->buffer_ == 0)" << be_nl
            << "{" << be_idt_nl
      << "result = allocbuf (this->maximum_);" << be_nl
      << "this->buffer_ = result;" << be_uidt_nl
            << "}" << be_nl
      << "else" << be_nl
            << "{" << be_idt_nl
      << "result = ACE_reinterpret_cast (";
  pt->accept (visitor);
  *os << "*, this->buffer_);" << be_uidt_nl
      << "}" << be_uidt_nl
      << "}" << be_nl
      << "else // if (orphan == CORBA::B_TRUE)" << be_nl
      << "{" << be_idt_nl
      << "if (this->release_ != CORBA::B_FALSE)" << be_nl
      << "{" << be_idt_nl
      << "// We set the state back to default and relinquish" << be_nl
      << "// ownership." << be_nl
      << "result = ACE_reinterpret_cast("; pt->accept (visitor); *os << "*,this->buffer_);" << be_nl
      << "this->maximum_ = 0;" << be_nl
      << "this->length_ = 0;" << be_nl
      << "this->buffer_ = 0;" << be_nl
      << "this->release_ = CORBA::B_FALSE;" << be_uidt_nl
      << "}" << be_uidt_nl
      << "}" << be_nl
      << "return result;" << be_uidt_nl
      << "}" << be_nl
      << be_nl;

  // get_buffer
  *os << "const "; pt->accept (visitor); *os << " *get_buffer (void) const" << be_nl
      << "{" << be_idt_nl
      << "return ACE_reinterpret_cast(const ";
  pt->accept (visitor);
  *os << " * ACE_CAST_CONST, this->buffer_);" << be_uidt_nl
      << "}" << be_nl
      << be_nl;

  // replace
  *os << "void replace (CORBA::ULong max," << be_idt_nl
      << "CORBA::ULong length," << be_nl;
  pt->accept(visitor); *os <<" *data," << be_nl
      << "CORBA::Boolean release = CORBA::B_FALSE)" << be_uidt_nl
      << "{" << be_idt_nl
      << "this->maximum_ = max;" << be_nl
      << "this->length_ = length;" << be_nl
      << "if (this->buffer_ && this->release_ == CORBA::B_TRUE)" << be_nl
      << "{" << be_idt_nl;
  pt->accept(visitor); *os <<"* tmp = ACE_reinterpret_cast(";
  pt->accept (visitor); *os << "* ACE_CAST_CONST, this->buffer_);" << be_nl
      << "freebuf (tmp);" << be_uidt_nl
      << "}" << be_nl
      << "this->buffer_ = data;" << be_nl
      << "this->release_ = release;" << be_uidt_nl
      << "}" << be_nl
      << be_uidt_nl
      << "};" << be_nl;

  os->gen_endif (); // endif macro


  delete visitor;
  return 0;
}
