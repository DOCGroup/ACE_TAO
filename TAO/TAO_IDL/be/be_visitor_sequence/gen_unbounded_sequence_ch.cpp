// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    gen_unbounded_sequence_ch.cpp
//
// = DESCRIPTION
//    Visitor generating code for Sequence in the client header
//
// = AUTHOR
//    Michael Kircher
//
// ============================================================================

//#include	"idl.h"
//#include	"idl_extern.h"
#include	"be.h"

#include "be_visitor_sequence.h"


int
be_visitor_sequence_ch::gen_unbounded_sequence (be_sequence *node)
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


  // get the visitor for the type of the sequence
  be_visitor_context ctx (*this->ctx_);
  ctx.state (TAO_CodeGen::TAO_SEQUENCE_BASE_CH);
  be_visitor *visitor = tao_cg->make_visitor (&ctx);
  
  os->gen_ifdef_macro (class_name);

  os->indent ();

  *os << "class " << class_name << " : public TAO_Unbounded_Base_Sequence" << be_nl
      << "{" << be_nl
      << "public:" << be_idt_nl
      << "// = Initialization and termination methods." << be_nl
      << be_nl;
  // constructor
  *os << class_name << " (void) // Default constructor." << be_nl
      << "{" << be_nl
      << "}" << be_nl
      << be_nl;

  // constructor
  *os << class_name << " (CORBA::ULong maximum) // Constructor using a maximum length value." << be_idt_nl
      << ": TAO_Unbounded_Base_Sequence (maximum, allocbuf (maximum))" << be_uidt_nl
      << "{" << be_nl
      << "}" << be_nl
      << be_nl;

  // constructor
  *os << class_name << " (CORBA::ULong maximum," << be_idt_nl
      << "CORBA::ULong length," << be_nl;
  // the accept is here the first time used and if an 
  // error occurs, it will occur here. Later no check
  // for errors will be done.
  if (pt->accept (visitor) == -1)
  {
     ACE_ERROR_RETURN ((LM_ERROR,
                        "(%N:%l) be_visitor_sequence_ch::"
                        "visit_sequence - "
                        "base type visit failed\n"),
                        -1);
  }
  *os << " *data," << be_nl
	    << "CORBA::Boolean release = 0)" << be_uidt_nl
      << ": TAO_Unbounded_Base_Sequence (maximum, length, data, release)" << be_nl
      << "{" << be_nl
      << "}" << be_nl
      << be_nl;

  // constructor
  *os << class_name << " (const " << class_name << " &rhs)" << be_nl
      << "// Copy constructor." << be_idt_nl;
  *os << ": TAO_Unbounded_Base_Sequence (rhs)" << be_uidt_nl
      << "{" << be_idt_nl;
  pt->accept(visitor); *os <<" *tmp1 = allocbuf (this->maximum_);" << be_nl;
  pt->accept(visitor);
  *os << " * const tmp2 = ACE_reinterpret_cast ("; 
  pt->accept (visitor); *os << " * ACE_CAST_CONST, rhs.buffer_);" << be_nl
      << be_nl
      << "for (CORBA::ULong i = 0; i < this->length_; ++i)" << be_idt_nl
      << "tmp1[i] = tmp2[i];" << be_uidt_nl
      << be_nl
      << "this->buffer_ = tmp1;" << be_uidt_nl
      << "}" << be_nl
      << be_nl;

  // operator = 
  *os << class_name << " &operator= (const " << class_name << " &rhs)" << be_nl
      << "// Assignment operator." << be_nl
      << "{" << be_idt_nl
      << "if (this == &rhs)" << be_idt_nl
      << "return *this;" << be_uidt_nl
      << be_nl
      << "if (this->release_)" << be_nl
      << "{" << be_idt_nl
      << "if (this->maximum_ < rhs.maximum_)" << be_nl
      << "{" << be_idt_nl
      << "// free the old buffer" << be_nl;
  pt->accept(visitor); *os <<" *tmp = ACE_reinterpret_cast ("; 
  pt->accept (visitor); *os << " *, this->buffer_);" << be_nl
      << "freebuf (tmp);" << be_nl
      << "this->buffer_ = allocbuf (rhs.maximum_);" << be_uidt_nl
      << "}" << be_uidt_nl
      << "}" << be_nl
      << "else" << be_idt_nl
      << "this->buffer_ = allocbuf (rhs.maximum_);" << be_uidt_nl
      << be_nl
      << "TAO_Unbounded_Base_Sequence::operator= (rhs);" << be_nl
      << be_nl;
  pt->accept(visitor); *os <<" *tmp1 = ACE_reinterpret_cast ("; 
  pt->accept (visitor); *os << " *, this->buffer_);" << be_nl;
  pt->accept(visitor); *os <<" * const tmp2 = ACE_reinterpret_cast ("; 
  pt->accept (visitor); *os << " * ACE_CAST_CONST, rhs.buffer_);" << be_nl
      << be_nl
      << "for (CORBA::ULong i = 0; i < this->length_; ++i)" << be_idt_nl
      << "tmp1[i] = tmp2[i];" << be_uidt_nl
      << be_nl
      << "return *this;" << be_uidt_nl
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
  pt->accept(visitor); *os <<" &operator[] (CORBA::ULong i)" << be_nl
      << "// operator []" << be_nl
      << "{" << be_idt_nl
      << "ACE_ASSERT (i < this->maximum_);" << be_nl;
  pt->accept(visitor); *os <<"* tmp = ACE_reinterpret_cast("; 
  pt->accept (visitor); *os << "*,this->buffer_);" << be_nl
      << "return tmp[i];" << be_uidt_nl
      << "}" << be_nl
      << be_nl;

  // operator[]
  *os << "const "; pt->accept (visitor); *os << " &operator[] (CORBA::ULong i) const" << be_nl
      << "// operator []" << be_nl
      << "{" << be_idt_nl
      << "ACE_ASSERT (i < this->maximum_);" << be_nl;
  pt->accept(visitor); *os <<" * const tmp = ACE_reinterpret_cast ("; 
  pt->accept (visitor); *os << "* ACE_CAST_CONST, this->buffer_);" << be_nl
      << "return tmp[i];" << be_uidt_nl
      << "}" << be_nl
      << be_nl;

  // Static operations
  *os << "// = Static operations." << be_nl
      << "static "; pt->accept (visitor); *os << " *allocbuf (CORBA::ULong size)" << be_nl
      << "// Allocate storage for the sequence." << be_nl
      << "{" << be_idt_nl
      << "return new "; pt->accept (visitor); *os << "[size];" << be_uidt_nl
      << "}" << be_nl
      << be_nl;
  *os << "static void freebuf ("; pt->accept (visitor); *os << " *buffer)" << be_nl
      << "// Free the sequence." << be_nl
      << "{" << be_idt_nl
      << "delete [] buffer;" << be_uidt_nl
      << "}" << be_nl
      << be_nl;

  // allocate_buffer
  *os << "virtual void _allocate_buffer (CORBA::ULong length)" << be_nl
      << "{" << be_idt_nl;
  pt->accept(visitor); *os <<"* tmp = allocbuf (length);" << be_nl
      << be_nl
      << "if (this->buffer_ != 0)" << be_nl
      << "{" << be_idt_nl;
  pt->accept(visitor); *os <<" *old = ACE_reinterpret_cast ("; 
  pt->accept (visitor); *os << " *,this->buffer_);" << be_nl
      << be_nl
      << "for (CORBA::ULong i = 0; i < this->length_; ++i)" << be_idt_nl
      << "tmp[i] = old[i];" << be_uidt_nl
      << be_nl
      << "if (this->release_)" << be_idt_nl
      << "freebuf (old);" << be_uidt_nl << be_uidt_nl
      << "}" << be_nl
      << "this->buffer_ = tmp;" << be_uidt_nl
      << "}" << be_nl
      << be_nl;

  // deallocate_buffer
  *os << "virtual void _deallocate_buffer (void)" << be_nl
      << "{" << be_idt_nl
      << "if (this->buffer_ == 0 || this->release_ == 0)" << be_idt_nl
      << "return;" << be_uidt_nl
      << be_nl;
  pt->accept(visitor); *os <<" *tmp = ACE_reinterpret_cast ("; 
  pt->accept (visitor); *os << " *,this->buffer_);" << be_nl
      << be_nl
      << "freebuf (tmp);" << be_nl
      << "this->buffer_ = 0;" << be_uidt_nl
      << "} " << be_nl
      << be_nl;
      
  // Implement the TAO_Base_Sequence methods (see Sequence.h)
  *os << "// Implement the TAO_Base_Sequence methods (see Sequence.h)" << be_nl
      << be_nl;
  pt->accept(visitor); *os <<" *get_buffer (CORBA::Boolean orphan = CORBA::B_FALSE)" << be_nl
      << "{" << be_idt_nl;
  pt->accept(visitor); *os <<" *result = 0;" << be_nl
      << "if (orphan == CORBA::B_FALSE)" << be_nl
      << "{" << be_idt_nl
      << "// We retain ownership." << be_nl
      << "if (this->buffer_ == 0)" << be_nl
	    << "{" << be_idt_nl
	    << "result = allocbuf (this->length_);" << be_nl
	    << "this->buffer_ = result;" << be_uidt_nl
	    << "}" << be_nl
      << "else" << be_nl
	    << "{" << be_idt_nl
	    << "result = ACE_reinterpret_cast ("; pt->accept (visitor); *os << "*, this->buffer_);" << be_uidt_nl
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
  *os << "void replace (CORBA::ULong max," << be_nl
      << "CORBA::ULong length," << be_nl;
  pt->accept(visitor); *os <<" *data," << be_nl
      << "CORBA::Boolean release)" << be_nl
      << "{" << be_idt_nl
      << "this->maximum_ = max;" << be_nl
      << "this->length_ = length;" << be_nl
      << "if (this->buffer_ && this->release_ == CORBA::B_TRUE)" << be_nl
      << "{" << be_idt_nl;
  pt->accept(visitor); *os <<" *tmp = ACE_reinterpret_cast("; pt->accept (visitor); *os << "*,this->buffer_);" << be_nl
      << "freebuf (tmp);" << be_uidt_nl
      << "}" << be_nl
      << "freebuf (("; pt->accept (visitor); *os << " *) this->buffer_);" << be_nl
      << "this->buffer_ = data;" << be_nl
      << "this->release_ = release;" << be_uidt_nl
      << "}" << be_nl
      << be_uidt_nl
      << "};" << be_nl;



  os->gen_endif (); // endif macro


  delete visitor;
  return 0;
}

