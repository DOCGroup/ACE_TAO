//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    gen_bounded_str_sequence_ch.cpp
//
// = DESCRIPTION
//    Visitor generating code for Sequence in the client header
//
// = AUTHOR
//    Michael Kircher
//
// ============================================================================

#include	"be.h"

#include "be_visitor_sequence.h"

ACE_RCSID(be_visitor_sequence, gen_bounded_str_sequence_ch, "$Id$")


int
be_visitor_sequence_ch::gen_bounded_str_sequence (be_sequence *node)
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


  *os << "class " << class_name << " : public TAO_Bounded_Base_Sequence" << be_nl
      << "{" << be_nl
      << "public:" << be_idt_nl;

  // constructor
  *os << class_name << " (void)" << be_nl
      << "  : TAO_Bounded_Base_Sequence (" << node->max_size () 
      << ", allocbuf(" << node->max_size () << "))" << be_nl
      << "{" << be_nl
      << "}" << be_nl
      << be_nl;

  // constructor
  *os << class_name << " (CORBA::ULong length," << be_idt_nl
      << "char* *value," << be_nl
      << "CORBA::Boolean release = 0)" << be_uidt_nl
      << "  : TAO_Bounded_Base_Sequence (" << node->max_size () << ", length, value, release)" << be_nl
      << "{" << be_nl
      << "  this->_allocate_buffer (" << node->max_size () << ");" << be_nl
      << "}" << be_nl
      << be_nl;

  // constructor
  *os << class_name << " (const " << class_name << " &rhs)" << be_idt_nl
      << ": TAO_Bounded_Base_Sequence (rhs)" << be_uidt_nl
      << "{" << be_idt_nl
      << "char **tmp1 = allocbuf (this->maximum_);" << be_nl
      << "char ** const tmp2 = ACE_reinterpret_cast (char ** ACE_CAST_CONST, rhs.buffer_);" << be_nl
      << be_nl
      << "for (CORBA::ULong i=0; i < rhs.length_; i++)" << be_idt_nl
      << "tmp1[i] = CORBA::string_dup (tmp2[i]);" << be_uidt_nl
      << be_nl
      << "this->buffer_ = tmp1;" << be_uidt_nl
      << "}" << be_nl
      << be_nl;

  // operator=
  *os << class_name << " &operator= (const " << class_name << " &rhs)" << be_nl
      << "{" << be_idt_nl
      << "if (this == &rhs)" << be_idt_nl
      << "return *this;" << be_uidt_nl
      << be_nl
      << "if (this->release_)" << be_nl
      << "{ " << be_idt_nl
      << "char **tmp = ACE_reinterpret_cast (char **, this->buffer_);" << be_nl
      << be_nl
      << "for (CORBA::ULong i = 0; i < this->length_; ++i)" << be_nl
      << "{" << be_idt_nl
      << "CORBA::string_free (tmp[i]);" << be_nl
      << "tmp[i] = 0;" << be_uidt_nl
      << "}" << be_uidt_nl
      << "}" << be_nl
      << "else" << be_idt_nl
      << "this->buffer_ = allocbuf (rhs.maximum_);" << be_uidt_nl
      << be_nl
      << "TAO_Bounded_Base_Sequence::operator= (rhs);" << be_nl
      << be_nl
      << "char **tmp1 = ACE_reinterpret_cast (char **, this->buffer_);" << be_nl
      << "char ** const tmp2 = ACE_reinterpret_cast (char ** ACE_CAST_CONST, rhs.buffer_);" << be_nl
      << be_nl
      << "for (CORBA::ULong i = 0; i < rhs.length_; i++)" << be_idt_nl
      << "tmp1[i] = CORBA::string_dup (tmp2[i]);" << be_uidt_nl
      << "return *this;" << be_uidt_nl
      << "}" << be_nl
      << be_nl;

  // destructor
  *os << "~" << class_name << " (void)" << be_nl
      << "{" << be_idt_nl
      << "this->_deallocate_buffer ();" << be_uidt_nl
      << "}" << be_nl
      << be_nl;

  // operator[]
  *os << "TAO_String_Manager operator[] (CORBA::ULong index) const" << be_nl
      << "// read-write accessor" << be_nl
      << "{" << be_idt_nl
      << "ACE_ASSERT (index < this->maximum_);" << be_nl
      << "char **const tmp = ACE_reinterpret_cast (char ** ACE_CAST_CONST, this->buffer_);" << be_nl
      << "return TAO_String_Manager (tmp + index, this->release_);" << be_uidt_nl
      << "}" << be_nl
      << be_nl;

  // allocbuf
  *os << "static char **allocbuf (CORBA::ULong length)" << be_nl
      << "{" << be_idt_nl
      << "char **buf;" << be_nl
      << "ACE_NEW_RETURN (buf, char *[" << node->max_size () << "], 0);" << be_nl
      << "for (CORBA::ULong i = 0; i < " << node->max_size () << "; i++)" << be_idt_nl
      << "buf[i] = 0;" << be_uidt_nl
      << "return buf;" << be_uidt_nl
      << "}" << be_nl
      << be_nl;

  // freebuf
  *os << "static void freebuf (char **buffer)" << be_nl
      << "{" << be_idt_nl
      << "for (CORBA::ULong i = 0; i < " << node->max_size () << "; ++i)" << be_nl
      << "{" << be_idt_nl
      << "if (buffer[i] != 0)" << be_nl
      << "{" << be_idt_nl
      << "CORBA::string_free (buffer[i]);" << be_nl
      << "buffer[i] = 0;" << be_uidt_nl
      << "}" << be_uidt_nl
      << "}" << be_nl
      << "delete [] buffer;" << be_uidt_nl
      << "}" << be_nl
      << be_nl;

  // allocate_buffer
  *os << "virtual void _allocate_buffer (CORBA::ULong length)" << be_nl
      << "{" << be_idt_nl
      << "// For this class memory is never reallocated so the implementation" << be_nl
      << "// is *really* simple." << be_nl
      << "this->buffer_ = allocbuf (" << node->max_size () << ");" << be_uidt_nl
      << "}" << be_nl
      << be_nl;

  // deallocate_bufffer
  *os << "virtual void _deallocate_buffer (void)" << be_nl
      << "{" << be_idt_nl
      << "if (this->buffer_ == 0 || this->release_ == 0)" << be_idt_nl
      << "return;" << be_uidt_nl
      << "char **tmp = ACE_reinterpret_cast (char **, this->buffer_);" << be_nl
      << "freebuf (tmp);" << be_nl
      << "this->buffer_ = 0;" << be_uidt_nl
      << "}" << be_nl
      << be_nl;

  // shrink_buffer
  *os << "virtual void _shrink_buffer (CORBA::ULong nl,CORBA::ULong ol)" << be_nl
      << "{" << be_idt_nl
      << "char **tmp = ACE_reinterpret_cast (char **,this->buffer_);" << be_nl
      << be_nl
      << "for (CORBA::ULong i = nl; i < ol; ++i)" << be_nl
      << "{" << be_idt_nl
      << "CORBA::string_free (tmp[i]);" << be_nl
      << "tmp[i] = 0;" << be_uidt_nl
      << "}" << be_uidt_nl
      << "}" << be_nl
      << be_uidt_nl
      << "};" << be_nl;



  os->gen_endif (); // endif macro


  delete visitor;
  return 0;
}

