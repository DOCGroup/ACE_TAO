//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    gen_bounded_str_sequence_ci.cpp
//
// = DESCRIPTION
//    Visitor generating code for Sequence in the client inline
//
// = AUTHOR
//    Michael Kircher
//
//    Modifications by Aniruddha Gokhale
//
// ============================================================================

#include	"be.h"

#include "be_visitor_sequence.h"

ACE_RCSID(be_visitor_sequence, gen_bounded_str_sequence_ci, "$Id$")


int
be_visitor_sequence_ci::gen_bounded_str_sequence (be_sequence *node)
{
  TAO_OutStream *os = this->ctx_->stream ();
  be_type *bt;

  // retrieve the base type since we may need to do some code
  // generation for the base type.
  bt = be_type::narrow_from_decl (node->base_type ());
  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_sequence_ci::"
                         "visit_sequence - "
                         "Bad element type\n"), -1);
    }

  const char * class_name = node->instance_name ();

  static char full_class_name [NAMEBUFSIZE];
  ACE_OS::memset (full_class_name, '\0', NAMEBUFSIZE);

  if (node->is_nested ())
    {
      ACE_OS::sprintf (full_class_name, "%s::%s",
                       be_scope::narrow_from_scope (node->defined_in ())->decl ()->fullname (),
                       class_name);
    }
  else
    {
      ACE_OS::sprintf (full_class_name, "%s",
                       class_name);
    }


  // get the visitor for the type of the sequence
  be_visitor_context ctx (*this->ctx_);
  ctx.state (TAO_CodeGen::TAO_SEQUENCE_BASE_CI);
  be_visitor *visitor = tao_cg->make_visitor (&ctx);
  
  // !! branching in either compile time template instantiation
  // or manual template instatiation
  os->gen_ifdef_AHETI();

  os->gen_ifdef_macro (class_name);

  os->indent ();

#if 0 /* Why is this here? ASG */
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

  // the accept is here the first time used and if an 
  // error occurs, it will occur here. Later no check
  // for errors will be done.
  if (pt->accept (visitor) == -1)
  {
     ACE_ERROR_RETURN ((LM_ERROR,
                        "(%N:%l) be_visitor_sequence_ci::"
                        "visit_sequence - "
                        "base type visit failed\n"),
                        -1);
  }
#endif /* 0 */

  // first generate the static methods since they are used by others. Since
  // they are inlined, their definition needs to come before their use else
  // some compilers (e.g., g++) produce lots of warnings.

  // allocbuf
  *os << "ACE_INLINE char **" << be_nl
      << full_class_name << "::allocbuf (CORBA::ULong length)" << be_nl
      << "{" << be_idt_nl
      << "char **buf;" << be_nl
      << "ACE_NEW_RETURN (buf, char *[" << node->max_size () << "], 0);" << be_nl
      << "for (CORBA::ULong i = 0; i < " << node->max_size () << "; i++)" << be_idt_nl
      << "buf[i] = 0;" << be_uidt_nl
      << "return buf;" << be_uidt_nl
      << "}" << be_nl
      << be_nl;

  // freebuf
  *os << "ACE_INLINE void " << be_nl
      << full_class_name << "::freebuf (char **buffer)" << be_nl
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

  // constructor
  *os << "ACE_INLINE" << be_nl
      << full_class_name << "::" << class_name << " (void)" << be_nl
      << "  : TAO_Bounded_Base_Sequence (" << node->max_size () 
      << ", " << full_class_name << "::allocbuf(" << node->max_size () << "))" << be_nl
      << "{" << be_nl
      << "}" << be_nl
      << be_nl;

  // constructor
  *os << "ACE_INLINE" << be_nl
      << full_class_name << "::" << class_name << " (CORBA::ULong length," << be_idt_nl
      << "char* *value," << be_nl
      << "CORBA::Boolean release)" << be_uidt_nl
      << "  : TAO_Bounded_Base_Sequence (" << node->max_size () << ", length, value, release)" << be_nl
      << "{" << be_nl
      << "  this->_allocate_buffer (" << node->max_size () << ");" << be_nl
      << "}" << be_nl
      << be_nl;

  // constructor
  *os << "ACE_INLINE" << be_nl
      << full_class_name << "::" << class_name << " (const " << full_class_name << " &rhs)" << be_idt_nl
      << ": TAO_Bounded_Base_Sequence (rhs)" << be_uidt_nl
      << "{" << be_idt_nl
      << "char **tmp1 = " << full_class_name << "::allocbuf (this->maximum_);" << be_nl
      << "char ** const tmp2 = ACE_reinterpret_cast (char ** ACE_CAST_CONST, rhs.buffer_);" << be_nl
      << be_nl
      << "for (CORBA::ULong i=0; i < rhs.length_; i++)" << be_idt_nl
      << "tmp1[i] = CORBA::string_dup (tmp2[i]);" << be_uidt_nl
      << be_nl
      << "this->buffer_ = tmp1;" << be_uidt_nl
      << "}" << be_nl
      << be_nl;

  // operator=
  *os << "ACE_INLINE " << full_class_name << "& " << be_nl 
      << full_class_name << "::operator= (const " << full_class_name << " &rhs)" << be_nl
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
      << "this->buffer_ = " << full_class_name << "::allocbuf (rhs.maximum_);" << be_uidt_nl
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

  // operator[]
  *os << "ACE_INLINE TAO_SeqElem_String_Manager " << be_nl
      << full_class_name << "::operator[] (CORBA::ULong index) const" << be_nl
      << "// read-write accessor" << be_nl
      << "{" << be_idt_nl
      << "ACE_ASSERT (index < this->maximum_);" << be_nl
      << "char **const tmp = ACE_reinterpret_cast (char ** ACE_CAST_CONST, this->buffer_);" << be_nl
      << "return TAO_SeqElem_String_Manager (tmp + index, this->release_);" << be_uidt_nl
      << "}" << be_nl
      << be_nl;

  // get_buffer
  *os << "ACE_INLINE char**" << be_nl
      << full_class_name << "::get_buffer (CORBA::Boolean orphan)" << be_nl
      << "{" << be_idt_nl
      << "char **result = 0;" << be_nl
      << "if (orphan == 0)" << be_nl
      << "{" << be_idt_nl
      << "// We retain ownership. " << be_nl
      << "if (this->buffer_ == 0)" << be_nl
      << "{" << be_idt_nl
      << "result = " << full_class_name << "::allocbuf (this->maximum_);" << be_nl
      << "this->buffer_ = result;" << be_uidt_nl
      << "}" << be_nl
      << "else" << be_nl
      << "{" << be_idt_nl
      << "result = ACE_reinterpret_cast (char **, this->buffer_);" << be_uidt_nl
      << "}" << be_uidt_nl
      << "}" << be_nl
      << "else // if (orphan == 1)" << be_nl
      << "{" << be_idt_nl
      << "if (this->release_ != 0)" << be_nl
      << "{" << be_idt_nl
      << "// We set state back to default and relinquish" << be_nl
      << "// ownership." << be_nl
      << "result = ACE_reinterpret_cast (char **, this->buffer_);" << be_nl
      << "this->maximum_ = 0;" << be_nl
      << "this->length_ = 0;" << be_nl
      << "this->buffer_ = 0;" << be_nl
      << "this->release_ = 0;" << be_uidt_nl
      << "}" << be_uidt_nl
      << "}" << be_nl
      << "return result;" << be_uidt_nl
      << "}" << be_nl
      << be_nl;

  // get_buffer
  *os << "ACE_INLINE const char**" << be_nl
      << full_class_name << "::get_buffer (void) const" << be_nl
      << "{" << be_idt_nl
      << "return ACE_reinterpret_cast (const char ** ACE_CAST_CONST, this->buffer_);" << be_uidt_nl
      << "}" << be_nl
      << be_nl;

  os->gen_endif (); // endif macro

  // generate #endif for AHETI
  os->gen_endif_AHETI();


  delete visitor;
  return 0;
}

