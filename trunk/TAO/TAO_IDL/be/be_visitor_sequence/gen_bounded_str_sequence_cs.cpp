//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    gen_bounded_str_sequence_cs.cpp
//
// = DESCRIPTION
//    Visitor generating code for Sequence in the client impl
//
// = AUTHOR
//    Michael Kircher
//
//    Modifications by Aniruddha Gokhale
//
// ============================================================================

ACE_RCSID (be_visitor_sequence, 
           gen_bounded_str_sequence_cs, 
           "$Id$")

int
be_visitor_sequence_cs::gen_bounded_str_sequence (be_sequence *node)
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
      << "char **tmp = ACE_reinterpret_cast (char **, this->buffer_);"
      << be_nl
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
      << "char **tmp = ACE_reinterpret_cast (char **,this->buffer_);" << be_nl
      << be_nl
      << "for (CORBA::ULong i = nl; i < ol; ++i)" << be_nl
      << "{" << be_idt_nl
      << "CORBA::string_free (tmp[i]);" << be_nl
      << "tmp[i] = 0;" << be_uidt_nl
      << "}" << be_uidt_nl
      << "}" << be_nl;

  os->gen_endif (); // endif macro

  // generate #endif for AHETI
  os->gen_endif_AHETI();

  return 0;
}
