//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    gen_bounded_wstr_sequence_ci.cpp
//
// = DESCRIPTION
//    Visitor generating code for bounded wstring sequence in the client inline
//
// = AUTHOR
//    Jeff Parsons <parsons@cs.wustl.edu>
//
// ============================================================================

#include "be.h"
#include "be_visitor_sequence.h"

ACE_RCSID (be_visitor_sequence, 
           gen_bounded_wstr_sequence_ci, 
           "$Id$")

int
be_visitor_sequence_ci::gen_bounded_wstr_sequence (be_sequence *node)
{
  TAO_OutStream *os = this->ctx_->stream ();
  const char * class_name = node->instance_name ();

  static char full_class_name [NAMEBUFSIZE];
  ACE_OS::memset (full_class_name, 
                  '\0', 
                  NAMEBUFSIZE);

  if (node->is_nested ())
    {
      be_decl *tmp =
        be_scope::narrow_from_scope (node->defined_in ())->decl ();

      ACE_OS::sprintf (full_class_name, 
                       "%s::%s",
                       tmp->full_name (),
                       class_name);
    }
  else
    {
      ACE_OS::sprintf (full_class_name, 
                       "%s",
                       class_name);
    }

  *os << be_nl << "// TAO_IDL - Generated from "
      << __FILE__ << ":" << __LINE__ << be_nl << be_nl;

  os->gen_ifdef_AHETI();
  os->gen_ifdef_macro (class_name);
  os->indent ();

  // First generate the static methods since they are used by others. Since
  // they are inlined, their definition needs to come before their use else
  // some compilers (e.g., g++) produce lots of warnings.

  // allocbuf
  *os << "ACE_INLINE CORBA::WChar **" << be_nl
      << full_class_name << "::allocbuf (CORBA::ULong /* length */)" << be_nl
      << "{" << be_idt_nl
      << "CORBA::WChar **buf = 0;" << be_nl
      << "ACE_NEW_RETURN (buf, CORBA::WChar *[" << node->max_size () << "], 0);" << be_nl
      << "for (CORBA::ULong i = 0; i < " << node->max_size () << "; i++)" << be_idt_nl
      << "buf[i] = 0;" << be_uidt_nl
      << "return buf;" << be_uidt_nl
      << "}" << be_nl
      << be_nl;

  // freebuf
  *os << "ACE_INLINE void " << be_nl
      << full_class_name << "::freebuf (CORBA::WChar **buffer)" << be_nl
      << "{" << be_idt_nl
      << "for (CORBA::ULong i = 0; i < " << node->max_size () << "; ++i)" << be_nl
      << "{" << be_idt_nl
      << "if (buffer[i] != 0)" << be_nl
      << "{" << be_idt_nl
      << "CORBA::wstring_free (buffer[i]);" << be_nl
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
      << ", 0)" << be_nl
      << "{" << be_nl
      << "}" << be_nl
      << be_nl;

  // constructor
  *os << "ACE_INLINE" << be_nl
      << full_class_name << "::" << class_name << " (CORBA::ULong length," << be_idt_nl
      << "CORBA::WChar* *value," << be_nl
      << "CORBA::Boolean release)" << be_uidt_nl
      << "  : TAO_Bounded_Base_Sequence (" << node->max_size ()
      << ", length, value, release)" << be_nl
      << "{" << be_nl
      << "}" << be_nl
      << be_nl;

  // constructor
  *os << "ACE_INLINE" << be_nl
      << full_class_name << "::" << class_name << " (const "
      << class_name << " &rhs)" << be_idt_nl
      << ": TAO_Bounded_Base_Sequence (rhs)" << be_uidt_nl
      << "{" << be_idt_nl
      << "if (rhs.buffer_ != 0)" << be_nl
      << "{" << be_idt_nl
      << "CORBA::WChar **tmp1 = " << class_name
      << "::allocbuf (this->maximum_);" << be_nl
      << "CORBA::WChar ** const tmp2 =" << be_idt_nl
      << "ACE_reinterpret_cast (CORBA::WChar ** ACE_CAST_CONST, rhs.buffer_);"
      << be_uidt_nl
      << be_nl
      << "for (CORBA::ULong i=0; i < rhs.length_; i++)" << be_idt_nl
      << "tmp1[i] = CORBA::wstring_dup (tmp2[i]);" << be_uidt_nl
      << be_nl
      << "this->buffer_ = tmp1;" << be_uidt_nl
      << "}" << be_nl
      << "else" << be_nl
      << "{" << be_idt_nl
      << "this->buffer_ = 0;" << be_uidt_nl
      << "}" << be_uidt_nl
      << "}" << be_nl
      << be_nl;

  // operator=
  *os << "ACE_INLINE " << full_class_name << "& " << be_nl
      << full_class_name << "::operator= (const " << class_name
      << " &rhs)" << be_nl
      << "{" << be_idt_nl
      << "if (this == &rhs)" << be_idt_nl
      << "return *this;" << be_uidt_nl
      << be_nl
      << "if (this->release_ && this->buffer_ != 0)" << be_nl
      << "{ " << be_idt_nl
      << "CORBA::WChar **tmp =" << be_idt_nl
      << "ACE_reinterpret_cast (CORBA::WChar **, this->buffer_);"
      << be_uidt_nl
      << be_nl
      << "for (CORBA::ULong i = 0; i < this->length_; ++i)" << be_nl
      << "{" << be_idt_nl
      << "CORBA::wstring_free (tmp[i]);" << be_nl
      << "tmp[i] = 0;" << be_uidt_nl
      << "}" << be_uidt_nl
      << "}" << be_nl
      << "else" << be_idt_nl
      << "this->buffer_ = " << class_name
      << "::allocbuf (rhs.maximum_);" << be_uidt_nl
      << be_nl
      << "TAO_Bounded_Base_Sequence::operator= (rhs);" << be_nl
      << be_nl
      << "CORBA::WChar **tmp1 =" << be_idt_nl
      << "ACE_reinterpret_cast (CORBA::WChar **, this->buffer_);"
      << be_uidt_nl
      << "CORBA::WChar ** const tmp2 =" << be_idt_nl
      << "ACE_reinterpret_cast (CORBA::WChar ** ACE_CAST_CONST, rhs.buffer_);"
      << be_uidt_nl
      << be_nl
      << "for (CORBA::ULong i = 0; i < rhs.length_; i++)" << be_idt_nl
      << "tmp1[i] = CORBA::wstring_dup (tmp2[i]);" << be_uidt_nl
      << "return *this;" << be_uidt_nl
      << "}" << be_nl
      << be_nl;

  // operator[]
  *os << "ACE_INLINE TAO_SeqElem_WString_Manager " << be_nl
      << full_class_name << "::operator[] (CORBA::ULong index) const" << be_nl
      << "// read-write accessor" << be_nl
      << "{" << be_idt_nl
      << "ACE_ASSERT (index < this->maximum_);" << be_nl
      << "CORBA::WChar **const tmp = ACE_reinterpret_cast (CORBA::WChar ** ACE_CAST_CONST, this->buffer_);" << be_nl
      << "return TAO_SeqElem_WString_Manager (tmp + index, this->release_);" << be_uidt_nl
      << "}" << be_nl
      << be_nl;

  // get_buffer
  *os << "ACE_INLINE CORBA::WChar**" << be_nl
      << full_class_name << "::get_buffer (CORBA::Boolean orphan)" << be_nl
      << "{" << be_idt_nl
      << "CORBA::WChar **result = 0;" << be_nl
      << "if (orphan == 0)" << be_nl
      << "{" << be_idt_nl
      << "// We retain ownership. " << be_nl
      << "if (this->buffer_ == 0)" << be_nl
      << "{" << be_idt_nl
      << "result = " << class_name << "::allocbuf (this->maximum_);"
      << be_nl
      << "this->buffer_ = result;" << be_nl
      << "this->release_ = 1;" << be_uidt_nl
      << "}" << be_nl
      << "else" << be_nl
      << "{" << be_idt_nl
      << "result = ACE_reinterpret_cast (CORBA::WChar **, this->buffer_);"
      << be_uidt_nl
      << "}" << be_uidt_nl
      << "}" << be_nl
      << "else // if (orphan == 1)" << be_nl
      << "{" << be_idt_nl
      << "if (this->release_ != 0)" << be_nl
      << "{" << be_idt_nl
      << "// We set state back to default and relinquish" << be_nl
      << "// ownership." << be_nl
      << "result = ACE_reinterpret_cast (CORBA::WChar **, this->buffer_);"
      << be_nl
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
  *os << "ACE_INLINE const CORBA::WChar**" << be_nl
      << full_class_name << "::get_buffer (void) const" << be_nl
      << "{" << be_idt_nl
      << "return ACE_reinterpret_cast (const CORBA::WChar ** ACE_CAST_CONST, this->buffer_);" << be_uidt_nl
      << "}" << be_nl
      << be_nl;

  os->gen_endif (); // endif macro

  // generate #endif for AHETI
  os->gen_endif_AHETI();

  return 0;
}
