// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    gen_unbounded_sequence_ci.cpp
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
           gen_unbounded_sequence_ci, 
           "$Id$")

int
be_visitor_sequence_ci::gen_unbounded_sequence (be_sequence *node)
{
  TAO_OutStream *os = this->ctx_->stream ();
  be_type *bt = be_type::narrow_from_decl (node->base_type ());

  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_sequence_ci::"
                         "visit_sequence - "
                         "Bad element type\n"), 
                        -1);
    }

  // If we contain an anonymous sequence,
  // generate code for that sequence here.
  if (bt->node_type () == AST_Decl::NT_sequence)
    {
      int status =
        this->gen_anonymous_base_type (bt,
                                         TAO_CodeGen::TAO_ROOT_CI);

      if (status == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_sequence_ci::"
                             "gen_unbounded_sequence - "
                             "gen_anonymous_base_type failed\n"),
                            -1);
        }
    }

  // Generate the class name
  be_type  *pt;

  if (bt->node_type () == AST_Decl::NT_typedef)
  {
    // Get the primitive base type of this typedef node.
    be_typedef *t = be_typedef::narrow_from_decl (bt);
    pt = t->primitive_base_type ();
  }
  else
    pt = bt;


  const char * class_name = node->instance_name ();

  static char full_class_name [NAMEBUFSIZE];
  ACE_OS::memset (full_class_name, '\0', NAMEBUFSIZE);

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

  // Get the visitor for the type of the sequence.
  be_visitor_context ctx (*this->ctx_);
  be_visitor_sequence_base visitor (&ctx);

  *os << be_nl << be_nl << "// TAO_IDL - Generated from " << be_nl
      << "// " << __FILE__ << ":" << __LINE__;

  os->gen_ifdef_AHETI();
  os->gen_ifdef_macro (class_name);

  // Static operations
  // allocbuf
  *os << be_nl << be_nl 
      << "ACE_INLINE" << be_nl;
  // the accept is here the first time used and if an
  // error occurs, it will occur here. Later no check
  // for errors will be done.
  if (bt->accept (&visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_sequence_ci::"
                         "gen_unbounded_sequence - "
                         "base type visit failed\n"),
                        -1);
    }

  *os << " *" << be_nl
      << full_class_name << "::allocbuf (CORBA::ULong size)" << be_nl
      << "{" << be_idt_nl;

  bt->accept (&visitor);

  *os << " *retval = 0;" << be_nl
      << "ACE_NEW_RETURN (retval, ";

  bt->accept (&visitor);

  *os << "[size], 0);" << be_nl
      << "return retval;" << be_uidt_nl
      << "}" << be_nl
      << be_nl;

  *os << "ACE_INLINE" << be_nl
      << "void "
      << full_class_name << "::freebuf (";

  bt->accept (&visitor);

  *os << " *buffer)" << be_nl
      << "{" << be_idt_nl
      << "delete [] buffer;" << be_uidt_nl
      << "}" << be_nl
      << be_nl;

  // constructor
  *os << "ACE_INLINE" << be_nl
      << full_class_name << "::" << class_name 
      << " (void)" << be_nl
      << "{" << be_nl
      << "}" << be_nl
      << be_nl;

  // constructor
  *os << "ACE_INLINE" << be_nl
      << full_class_name << "::" << class_name 
      << " (CORBA::ULong maximum) " << be_idt_nl
      << ": TAO_Unbounded_Base_Sequence (maximum, "
      << class_name << "::allocbuf (maximum))"
      << be_uidt_nl
      << "{" << be_nl
      << "}" << be_nl
      << be_nl;

  // constructor
  *os << "ACE_INLINE" << be_nl
      << full_class_name << "::" << class_name << " (" << be_idt << be_idt_nl
      << "CORBA::ULong maximum," << be_nl
      << "CORBA::ULong length," << be_nl;

  bt->accept (&visitor);

  *os << " *data," << be_nl
      << "CORBA::Boolean release" << be_uidt_nl
      << ")" << be_nl
      << ": TAO_Unbounded_Base_Sequence (maximum, length, data, release)" 
      << be_uidt_nl
      << "{" << be_nl
      << "}" << be_nl << be_nl;

  // constructor
  *os << "ACE_INLINE" << be_nl
      << full_class_name << "::" << class_name << " (" << be_idt << be_idt_nl
      << "const "
      << class_name << " &rhs" << be_uidt_nl
      << ")" << be_nl;
  *os << ": TAO_Unbounded_Base_Sequence (rhs)" << be_uidt_nl
      << "{" << be_idt_nl
      << "if (rhs.buffer_ != 0)" << be_idt_nl
      << "{" << be_idt_nl;

  bt->accept (&visitor);

  *os <<" *tmp1 =" << be_idt_nl
      << class_name << "::allocbuf (this->maximum_);" << be_uidt_nl;

  bt->accept (&visitor);

  *os << " * const tmp2 =" << be_idt_nl
      << "ACE_reinterpret_cast (";

  bt->accept (&visitor);

  *os << " * ACE_CAST_CONST, rhs.buffer_);" << be_uidt_nl << be_nl
      << "for (CORBA::ULong i = 0; i < this->length_; ++i)" << be_idt_nl
      << "{" << be_idt_nl;

  if (pt->node_type () == AST_Decl::NT_array)
    {
      bt->accept (&visitor);
      *os << "_var::copy (tmp1[i], tmp2[i]);";
    }
  else
    {
      *os << "tmp1[i] = tmp2[i];";
    }

  *os << be_uidt_nl
      << "}" << be_uidt_nl;

  *os << be_nl
      << "this->buffer_ = tmp1;" << be_uidt_nl
      << "}" << be_uidt_nl
      << "else" << be_idt_nl
      << "{" << be_idt_nl
      << "this->buffer_ = 0;" << be_uidt_nl
      << "}" << be_uidt << be_uidt_nl
      << "}" << be_nl
      << be_nl;

  // operator =
  *os << "ACE_INLINE" << be_nl
      << full_class_name << " &" << be_nl
      << full_class_name << "::operator= (" << be_idt << be_idt_nl
      << "const " 
      << class_name << " &rhs" << be_uidt_nl 
      << ")" << be_uidt_nl
      << "{" << be_idt_nl
      << "if (this == &rhs)" << be_idt_nl
      << "{" << be_idt_nl
      << "return *this;" << be_uidt_nl
      << "}" << be_uidt_nl << be_nl
      << "if (this->release_)" << be_idt_nl
      << "{" << be_idt_nl
      << "if (this->maximum_ < rhs.maximum_)" << be_idt_nl
      << "{" << be_idt_nl
      << "// Free the old buffer." << be_nl;

  bt->accept (&visitor);

  *os <<" *tmp =" << be_idt_nl
      << "ACE_reinterpret_cast (";

  bt->accept (&visitor);

  *os << " *, this->buffer_);" << be_uidt_nl << be_nl
      << class_name << "::freebuf (tmp);" << be_nl << be_nl
      << "this->buffer_ =" << be_idt_nl 
      << class_name
      << "::allocbuf (rhs.maximum_);" << be_uidt << be_uidt_nl
      << "}" << be_uidt << be_uidt_nl
      << "}" << be_uidt_nl
      << "else" << be_idt_nl
      << "{" << be_idt_nl
      << "this->buffer_ =" << be_idt_nl
      << class_name
      << "::allocbuf (rhs.maximum_);" << be_uidt << be_uidt_nl
      << "}" << be_uidt_nl << be_nl
      << "TAO_Unbounded_Base_Sequence::operator= (rhs);" << be_nl << be_nl;

  bt->accept (&visitor);

  *os <<" *tmp1 =" << be_idt_nl
      << "ACE_reinterpret_cast (";

  bt->accept (&visitor);

  *os << " *, this->buffer_);" << be_uidt_nl;

  bt->accept (&visitor);

  *os <<" * const tmp2 =" << be_idt_nl
      << "ACE_reinterpret_cast (";

  bt->accept (&visitor);

  *os << " * ACE_CAST_CONST, rhs.buffer_);" << be_uidt_nl << be_nl
      << "for (CORBA::ULong i = 0; i < this->length_; ++i)" << be_idt_nl
      << "{" << be_idt_nl;

  if (pt->node_type () == AST_Decl::NT_array)
    {
      bt->accept (&visitor);
      *os << "_var::copy (tmp1[i], tmp2[i]);";
    }
  else
    {
      *os << "tmp1[i] = tmp2[i];";
    }

  *os << be_uidt_nl
      << "}" << be_uidt_nl;

  *os << be_nl
      << "return *this;" << be_uidt_nl
      << "}" << be_nl << be_nl;

  // Accessors
  *os << "// = Accessors." << be_nl;
  *os << "ACE_INLINE" << be_nl;

  bt->accept (&visitor);

  *os <<" &" << be_nl
      << full_class_name << "::operator[] (CORBA::ULong i)" << be_nl
      << "{" << be_idt_nl
      << "ACE_ASSERT (i < this->maximum_);" << be_nl;

  bt->accept (&visitor);

  *os <<"* tmp =" << be_idt_nl
      << "ACE_reinterpret_cast (";

  bt->accept (&visitor);

  *os << "*, this->buffer_);" << be_uidt_nl
      << "return tmp[i];" << be_uidt_nl
      << "}" << be_nl << be_nl;

  // operator[]
  *os << "ACE_INLINE" << be_nl
      << "const ";

  bt->accept (&visitor);

  *os << " &" << be_nl
      << full_class_name << "::operator[] (CORBA::ULong i) const" << be_nl
      << "{" << be_idt_nl
      << "ACE_ASSERT (i < this->maximum_);" << be_nl;

  bt->accept (&visitor);

  *os <<" * const tmp =" << be_idt_nl
      << "ACE_reinterpret_cast (";

  bt->accept (&visitor);

  *os << "* ACE_CAST_CONST, this->buffer_);" << be_uidt_nl
      << "return tmp[i];" << be_uidt_nl
      << "}" << be_nl << be_nl;

  // Implement the TAO_Base_Sequence methods (see Sequence.h)
  *os << "// Implement the TAO_Base_Sequence methods (see Sequence.h)" << be_nl
      << be_nl;
  *os << "ACE_INLINE" << be_nl;

  bt->accept (&visitor);

  *os << " *" << be_nl
      << full_class_name << "::get_buffer (CORBA::Boolean orphan)" << be_nl
      << "{" << be_idt_nl;

  bt->accept (&visitor);

  *os <<" *result = 0;" << be_nl << be_nl
      << "if (orphan == 0)" << be_idt_nl
      << "{" << be_idt_nl
      << "// We retain ownership." << be_nl
      << "if (this->buffer_ == 0)" << be_idt_nl
      << "{" << be_idt_nl
      << "result =" << be_idt_nl
      << class_name << "::allocbuf (this->length_);" << be_uidt_nl
      << "this->buffer_ = result;" << be_nl
      << "this->release_ = 1;" << be_uidt_nl
      << "}" << be_uidt_nl
      << "else" << be_idt_nl
      << "{" << be_idt_nl
      << "result =" << be_idt_nl
      << "ACE_reinterpret_cast (";

  bt->accept (&visitor);

  *os << "*, this->buffer_);" << be_uidt << be_uidt_nl
      << "}" << be_uidt << be_uidt_nl
      << "}" << be_uidt_nl
      << "else // if (orphan == 1)" << be_idt_nl
      << "{" << be_idt_nl
      << "if (this->release_ != 0)" << be_idt_nl
      << "{" << be_idt_nl
      << "// We set the state back to default and relinquish ownership." 
      << be_nl
      << "result =" << be_idt_nl
      << "ACE_reinterpret_cast(";

  bt->accept (&visitor);

  *os << "*,this->buffer_);" << be_uidt_nl
      << "this->maximum_ = 0;" << be_nl
      << "this->length_ = 0;" << be_nl
      << "this->buffer_ = 0;" << be_nl
      << "this->release_ = 0;" << be_uidt_nl
      << "}" << be_uidt << be_uidt_nl
      << "}" << be_uidt_nl << be_nl
      << "return result;" << be_uidt_nl
      << "}" << be_nl << be_nl;

  // get_buffer
  *os << "ACE_INLINE" << be_nl;
  *os << "const ";

  bt->accept (&visitor);

  *os << " *" << be_nl
      << full_class_name << "::get_buffer (void) const" << be_nl
      << "{" << be_idt_nl
      << "return ACE_reinterpret_cast (const ";

  bt->accept (&visitor);

  *os << " * ACE_CAST_CONST, this->buffer_);" << be_uidt_nl
      << "}" << be_nl << be_nl;

  // replace
  *os << "ACE_INLINE" << be_nl
      << "void" << be_nl
      << full_class_name << "::replace (" << be_idt << be_idt_nl 
      << "CORBA::ULong max," << be_nl
      << "CORBA::ULong length," << be_nl;

  bt->accept (&visitor);

  *os <<" *data," << be_nl
      << "CORBA::Boolean release" << be_uidt_nl
      << ")" << be_uidt_nl
      << "{" << be_idt_nl
      << "this->maximum_ = max;" << be_nl
      << "this->length_ = length;" << be_nl << be_nl
      << "if (this->buffer_ && this->release_ == 1)" << be_idt_nl
      << "{" << be_idt_nl;

  bt->accept (&visitor);

  *os <<" *tmp =" << be_idt_nl
      << "ACE_reinterpret_cast (";

  bt->accept (&visitor);

  *os << "*, this->buffer_);" << be_uidt_nl
      << class_name << "::freebuf (tmp);" << be_uidt_nl
      << "}" << be_uidt_nl <<  be_nl
      << "this->buffer_ = data;" << be_nl
      << "this->release_ = release;" << be_uidt_nl
      << "}";

  os->gen_endif (); // endif macro

  // generate #endif for AHETI
  os->gen_endif_AHETI();

  return 0;
}
