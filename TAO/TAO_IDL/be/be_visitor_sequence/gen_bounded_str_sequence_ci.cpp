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

ACE_RCSID (be_visitor_sequence, 
           gen_bounded_str_sequence_ci, 
           "$Id$")

int
be_visitor_sequence_ci::gen_bounded_str_sequence (be_sequence *node)
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

  *os << be_nl << be_nl << "// TAO_IDL - Generated from " << be_nl
      << "// " << __FILE__ << ":" << __LINE__;

  os->gen_ifdef_AHETI();
  os->gen_ifdef_macro (class_name);

  // first generate the static methods since they are used by others. Since
  // they are inlined, their definition needs to come before their use else
  // some compilers (e.g., g++) produce lots of warnings.

  // allocbuf
  *os << be_nl << be_nl 
      << "ACE_INLINE" << be_nl
      << "char **" << be_nl
      << full_class_name << "::allocbuf (CORBA::ULong /* length */)" << be_nl
      << "{" << be_idt_nl
      << "char **buf = 0;" << be_nl
      << "ACE_NEW_RETURN (buf, char *[" << node->max_size () << "], 0);" 
      << be_nl << be_nl
      << "for (CORBA::ULong i = 0; i < " << node->max_size () << "; ++i)" 
      << be_idt_nl
      << "{" << be_idt_nl
      << "buf[i] = 0;" << be_uidt_nl
      << "}" << be_uidt_nl << be_nl
      << "return buf;" << be_uidt_nl
      << "}" << be_nl
      << be_nl;

  // freebuf
  *os << "ACE_INLINE" << be_nl
      << "void " << be_nl
      << full_class_name << "::freebuf (char **buffer)" << be_nl
      << "{" << be_idt_nl
      << "for (CORBA::ULong i = 0; i < " << node->max_size () << "; ++i)" 
      << be_idt_nl
      << "{" << be_idt_nl
      << "if (buffer[i] != 0)" << be_idt_nl
      << "{" << be_idt_nl
      << "CORBA::string_free (buffer[i]);" << be_nl
      << "buffer[i] = 0;" << be_uidt_nl
      << "}" << be_uidt << be_uidt_nl
      << "}" << be_uidt_nl << be_nl
      << "delete [] buffer;" << be_uidt_nl
      << "}" << be_nl << be_nl;

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
      << full_class_name << "::" << class_name << " (" << be_idt << be_idt_nl
      << "CORBA::ULong length," << be_nl
      << "char* *value," << be_nl
      << "CORBA::Boolean release" << be_uidt_nl
      << ")" << be_nl
      << ": TAO_Bounded_Base_Sequence (" << node->max_size () 
      << ", length, value, release)" << be_uidt_nl
      << "{" << be_nl
      << "}" << be_nl << be_nl;

  // constructor
  *os << "ACE_INLINE" << be_nl
      << full_class_name << "::" << class_name << " (" << be_idt << be_idt_nl
      << "const "
      << class_name << " &rhs" << be_uidt_nl
      << ")" << be_nl
      << ": TAO_Bounded_Base_Sequence (rhs)" << be_uidt_nl
      << "{" << be_idt_nl
      << "if (rhs.buffer_ != 0)" << be_idt_nl
      << "{" << be_idt_nl
      << "char **tmp1 ="<< be_idt_nl
      << class_name << "::allocbuf (this->maximum_);" << be_uidt_nl
      << "char ** const tmp2 =" << be_idt_nl
      << "ACE_reinterpret_cast (char ** ACE_CAST_CONST, rhs.buffer_);" 
      << be_uidt_nl << be_nl
      << "for (CORBA::ULong i=0; i < rhs.length_; i++)" << be_idt_nl
      << "{" << be_idt_nl
      << "tmp1[i] = CORBA::string_dup (tmp2[i]);" << be_uidt_nl
      << "}" << be_uidt_nl << be_nl
      << "this->buffer_ = tmp1;" << be_uidt_nl
      << "}" << be_uidt_nl
      << "else" << be_idt_nl
      << "{" << be_idt_nl
      << "this->buffer_ = 0;" << be_uidt_nl
      << "}" << be_uidt << be_uidt_nl
      << "}" << be_nl << be_nl;

  // operator=
  *os << "ACE_INLINE" << be_nl
      << full_class_name << "& " << be_nl
      << full_class_name << "::operator= (" << be_idt << be_idt_nl
      << "const "
      << class_name << " &rhs" << be_uidt_nl
      << ")" << be_uidt_nl
      << "{" << be_idt_nl
      << "if (this == &rhs)" << be_idt_nl
      << "{" << be_idt_nl
      << "return *this;" << be_uidt_nl
      << "}" << be_uidt_nl << be_nl
      << "if (this->release_ && this->buffer_ != 0)" << be_idt_nl
      << "{ " << be_idt_nl
      << "char **tmp =" << be_idt_nl
      << "ACE_reinterpret_cast (char **, this->buffer_);" 
      << be_uidt_nl << be_nl
      << "for (CORBA::ULong i = 0; i < this->length_; ++i)" << be_idt_nl
      << "{" << be_idt_nl
      << "CORBA::string_free (tmp[i]);" << be_nl
      << "tmp[i] = 0;" << be_uidt_nl
      << "}" << be_uidt << be_uidt_nl
      << "}" << be_uidt_nl
      << "else" << be_idt_nl
      << "{" << be_idt_nl
      << "this->buffer_ =" << be_idt_nl
      << class_name << "::allocbuf (rhs.maximum_);" 
      << be_uidt << be_uidt_nl 
      << "}" << be_uidt_nl << be_nl
      << "TAO_Bounded_Base_Sequence::operator= (rhs);" << be_nl << be_nl
      << "char **tmp1 =" << be_idt_nl
      << "ACE_reinterpret_cast (char **, this->buffer_);" 
      << be_uidt_nl << be_nl
      << "char ** const tmp2 =" << be_idt_nl
      << "ACE_reinterpret_cast (char ** ACE_CAST_CONST, rhs.buffer_);" 
      << be_uidt_nl << be_nl
      << "for (CORBA::ULong i = 0; i < rhs.length_; i++)" << be_idt_nl
      << "{" << be_idt_nl
      << "tmp1[i] = CORBA::string_dup (tmp2[i]);" << be_uidt_nl
      << "}" << be_uidt_nl << be_nl
      << "return *this;" << be_uidt_nl
      << "}" << be_nl << be_nl;

  // operator[]
  *os << "ACE_INLINE" << be_nl
      << "TAO_SeqElem_String_Manager " << be_nl
      << full_class_name << "::operator[] (CORBA::ULong index) const" << be_nl
      << "{" << be_idt_nl
      << "ACE_ASSERT (index < this->maximum_);" << be_nl
      << "char **const tmp =" << be_idt_nl
      << "ACE_reinterpret_cast (char ** ACE_CAST_CONST, this->buffer_);" 
      << be_uidt_nl
      << "return TAO_SeqElem_String_Manager (tmp + index, this->release_);" 
      << be_uidt_nl
      << "}" << be_nl << be_nl;

  // get_buffer
  *os << "ACE_INLINE" << be_nl
      << "char**" << be_nl
      << full_class_name << "::get_buffer (CORBA::Boolean orphan)" << be_nl
      << "{" << be_idt_nl
      << "char **result = 0;" << be_nl << be_nl
      << "if (orphan == 0)" << be_idt_nl
      << "{" << be_idt_nl
      << "// We retain ownership. " << be_nl
      << "if (this->buffer_ == 0)" << be_idt_nl
      << "{" << be_idt_nl
      << "result =" << be_idt_nl
      << class_name << "::allocbuf (this->maximum_);" << be_uidt_nl
      << "this->buffer_ = result;" << be_nl
      << "this->release_ = 1;" << be_uidt_nl
      << "}" << be_uidt_nl
      << "else" << be_idt_nl
      << "{" << be_idt_nl
      << "result =" << be_idt_nl
      << "ACE_reinterpret_cast (char **, this->buffer_);" 
      << be_uidt << be_uidt_nl
      << "}" << be_uidt << be_uidt_nl
      << "}" << be_uidt_nl
      << "else // if (orphan == 1)" << be_idt_nl
      << "{" << be_idt_nl
      << "if (this->release_ != 0)" << be_idt_nl
      << "{" << be_idt_nl
      << "// We set state back to default and relinquish ownership." << be_nl
      << "result =" << be_idt_nl
      << "ACE_reinterpret_cast (char **, this->buffer_);" << be_uidt_nl
      << "this->maximum_ = 0;" << be_nl
      << "this->length_ = 0;" << be_nl
      << "this->buffer_ = 0;" << be_nl
      << "this->release_ = 0;" << be_uidt_nl
      << "}" << be_uidt << be_uidt_nl
      << "}" << be_uidt_nl << be_nl
      << "return result;" << be_uidt_nl
      << "}" << be_nl << be_nl;

  // get_buffer
  *os << "ACE_INLINE" << be_nl
      << "const char**" << be_nl
      << full_class_name << "::get_buffer (void) const" << be_nl
      << "{" << be_idt_nl
      << "return ACE_reinterpret_cast (const char ** ACE_CAST_CONST, this->buffer_);" << be_uidt_nl
      << "}";

  os->gen_endif (); // endif macro

  // generate #endif for AHETI
  os->gen_endif_AHETI();

  return 0;
}
