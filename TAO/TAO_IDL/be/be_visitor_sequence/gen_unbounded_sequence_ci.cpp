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
                                         TAO_CodeGen::TAO_SEQUENCE_CI);

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
  ctx.state (TAO_CodeGen::TAO_SEQUENCE_BASE_CI);
  be_visitor_sequence_base visitor (&ctx);

  *os << be_nl << "// TAO_IDL - Generated from "
      << __FILE__ << ":" << __LINE__ << be_nl << be_nl;

  os->gen_ifdef_AHETI();
  os->gen_ifdef_macro (class_name);
  os->indent ();

  // Static operations
  // allocbuf
  *os << "// = Static operations." << be_nl
      << "ACE_INLINE ";
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
      << "// Allocate storage for the sequence." << be_nl
      << "{" << be_idt_nl;

  bt->accept (&visitor);

  *os << " *retval = 0;" << be_nl
      << "ACE_NEW_RETURN (retval, ";

  bt->accept (&visitor);

  *os << "[size], 0);" << be_nl
      << "return retval;" << be_uidt_nl
      << "}" << be_nl
      << be_nl;

  *os << "ACE_INLINE void "
      << full_class_name << "::freebuf (";

  bt->accept (&visitor);

  *os << " *buffer)" << be_nl
      << "// Free the sequence." << be_nl
      << "{" << be_idt_nl
      << "delete [] buffer;" << be_uidt_nl
      << "}" << be_nl
      << be_nl;

  // constructor
  *os << "ACE_INLINE" << be_nl
      << full_class_name << "::" << class_name 
      << " (void) // Default constructor." << be_nl
      << "{" << be_nl
      << "}" << be_nl
      << be_nl;

  // constructor
  *os << "ACE_INLINE" << be_nl
      << full_class_name << "::" << class_name << " (CORBA::ULong maximum) "
      << "// Constructor using a maximum length value." << be_idt_nl
      << ": TAO_Unbounded_Base_Sequence (maximum, "
      << class_name << "::allocbuf (maximum))"
      << be_uidt_nl
      << "{" << be_nl
      << "}" << be_nl
      << be_nl;

  // constructor
  *os << "ACE_INLINE" << be_nl
      << full_class_name << "::" << class_name 
      << " (CORBA::ULong maximum," << be_idt_nl
      << "CORBA::ULong length," << be_nl;

  bt->accept (&visitor);

  *os << " *data," << be_nl
      << "CORBA::Boolean release)" << be_uidt_nl
      << ": TAO_Unbounded_Base_Sequence (maximum, length, data, release)" 
      << be_nl
      << "{" << be_nl
      << "}" << be_nl
      << be_nl;

  // constructor
  *os << "ACE_INLINE" << be_nl
      << full_class_name << "::" << class_name << " (const "
      << class_name << " &rhs)" << be_nl
      << "// Copy constructor." << be_idt_nl;
  *os << ": TAO_Unbounded_Base_Sequence (rhs)" << be_uidt_nl
      << "{" << be_idt_nl
      << "if (rhs.buffer_ != 0)" << be_nl
      << "{" << be_idt_nl;

  bt->accept (&visitor);

  *os <<" *tmp1 = " << class_name << "::allocbuf (this->maximum_);" << be_nl;

  bt->accept (&visitor);

  *os << " * const tmp2 = ACE_reinterpret_cast (";

  bt->accept (&visitor);

  *os << " * ACE_CAST_CONST, rhs.buffer_);" << be_nl
      << be_nl
      << "for (CORBA::ULong i = 0; i < this->length_; ++i)" << be_idt_nl;

  if (pt->node_type () == AST_Decl::NT_array)
    {
      bt->accept (&visitor);
      *os << "_var::copy (tmp1[i], tmp2[i]);" << be_uidt_nl;
    }
  else
    {
      *os << "tmp1[i] = tmp2[i];" << be_uidt_nl;
    }

  *os << be_nl
      << "this->buffer_ = tmp1;" << be_uidt_nl
      << "}" << be_nl
      << "else" << be_nl
      << "{" << be_idt_nl
      << "this->buffer_ = 0;" << be_uidt_nl
      << "}" << be_uidt_nl
      << "}" << be_nl
      << be_nl;

  // operator =
  *os << "ACE_INLINE " << full_class_name << " &" << be_nl
      << full_class_name << "::operator= (const " << class_name << " &rhs)" << be_nl
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

  bt->accept (&visitor);

  *os <<" *tmp = ACE_reinterpret_cast (";

  bt->accept (&visitor);

  *os << " *, this->buffer_);" << be_nl
      << class_name << "::freebuf (tmp);" << be_nl
      << "this->buffer_ = " << class_name
      << "::allocbuf (rhs.maximum_);" << be_uidt_nl
      << "}" << be_uidt_nl
      << "}" << be_nl
      << "else" << be_idt_nl
      << "this->buffer_ = " << class_name
      << "::allocbuf (rhs.maximum_);" << be_uidt_nl
      << be_nl
      << "TAO_Unbounded_Base_Sequence::operator= (rhs);" << be_nl
      << be_nl;

  bt->accept (&visitor);

  *os <<" *tmp1 = ACE_reinterpret_cast (";

  bt->accept (&visitor);

  *os << " *, this->buffer_);" << be_nl;

  bt->accept (&visitor);

  *os <<" * const tmp2 = ACE_reinterpret_cast (";

  bt->accept (&visitor);

  *os << " * ACE_CAST_CONST, rhs.buffer_);" << be_nl
      << be_nl
      << "for (CORBA::ULong i = 0; i < this->length_; ++i)" << be_idt_nl;

  if (pt->node_type () == AST_Decl::NT_array)
    {
      bt->accept (&visitor);
      *os << "_var::copy (tmp1[i], tmp2[i]);" << be_uidt_nl;
    }
  else
    {
      *os << "tmp1[i] = tmp2[i];" << be_uidt_nl;
    }

  *os << be_nl
      << "return *this;" << be_uidt_nl
      << "}" << be_nl
      << be_nl;

  // Accessors
  *os << "// = Accessors." << be_nl;
  *os << "ACE_INLINE ";

  bt->accept (&visitor);

  *os <<" &" << be_nl
      << full_class_name << "::operator[] (CORBA::ULong i)" << be_nl
      << "// operator []" << be_nl
      << "{" << be_idt_nl
      << "ACE_ASSERT (i < this->maximum_);" << be_nl;

  bt->accept (&visitor);

  *os <<"* tmp = ACE_reinterpret_cast(";

  bt->accept (&visitor);

  *os << "*,this->buffer_);" << be_nl
      << "return tmp[i];" << be_uidt_nl
      << "}" << be_nl
      << be_nl;

  // operator[]
  *os << "ACE_INLINE const ";

  bt->accept (&visitor);

  *os << " &" << be_nl
      << full_class_name << "::operator[] (CORBA::ULong i) const" << be_nl
      << "// operator []" << be_nl
      << "{" << be_idt_nl
      << "ACE_ASSERT (i < this->maximum_);" << be_nl;

  bt->accept (&visitor);

  *os <<" * const tmp = ACE_reinterpret_cast (";

  bt->accept (&visitor);

  *os << "* ACE_CAST_CONST, this->buffer_);" << be_nl
      << "return tmp[i];" << be_uidt_nl
      << "}" << be_nl
      << be_nl;

  // Implement the TAO_Base_Sequence methods (see Sequence.h)
  *os << "// Implement the TAO_Base_Sequence methods (see Sequence.h)" << be_nl
      << be_nl;
  *os << "ACE_INLINE ";

  bt->accept (&visitor);

  *os << " *" << be_nl
      << full_class_name << "::get_buffer (CORBA::Boolean orphan)" << be_nl
      << "{" << be_idt_nl;

  bt->accept (&visitor);

  *os <<" *result = 0;" << be_nl
      << "if (orphan == 0)" << be_nl
      << "{" << be_idt_nl
      << "// We retain ownership." << be_nl
      << "if (this->buffer_ == 0)" << be_nl
      << "{" << be_idt_nl
      << "result = " << class_name << "::allocbuf (this->length_);" << be_nl
      << "this->buffer_ = result;" << be_nl
      << "this->release_ = 1;" << be_uidt_nl
      << "}" << be_nl
      << "else" << be_nl
      << "{" << be_idt_nl
      << "result = ACE_reinterpret_cast (";

  bt->accept (&visitor);

  *os << "*, this->buffer_);" << be_uidt_nl
      << "}" << be_uidt_nl
      << "}" << be_nl
      << "else // if (orphan == 1)" << be_nl
      << "{" << be_idt_nl
      << "if (this->release_ != 0)" << be_nl
      << "{" << be_idt_nl
      << "// We set the state back to default and relinquish" << be_nl
      << "// ownership." << be_nl
      << "result = ACE_reinterpret_cast(";

  bt->accept (&visitor);

  *os << "*,this->buffer_);" << be_nl
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
  *os << "ACE_INLINE const ";

  bt->accept (&visitor);

  *os << " *" << be_nl
      << full_class_name << "::get_buffer (void) const" << be_nl
      << "{" << be_idt_nl
      << "return ACE_reinterpret_cast(const ";

  bt->accept (&visitor);

  *os << " * ACE_CAST_CONST, this->buffer_);" << be_uidt_nl
      << "}" << be_nl
      << be_nl;

  // replace
  *os << "ACE_INLINE void" << be_nl
      << full_class_name << "::replace (CORBA::ULong max," << be_nl
      << "CORBA::ULong length," << be_nl;

  bt->accept (&visitor);

  *os <<" *data," << be_nl
      << "CORBA::Boolean release)" << be_nl
      << "{" << be_idt_nl
      << "this->maximum_ = max;" << be_nl
      << "this->length_ = length;" << be_nl
      << "if (this->buffer_ && this->release_ == 1)" << be_nl
      << "{" << be_idt_nl;

  bt->accept (&visitor);

  *os <<" *tmp = ACE_reinterpret_cast(";

  bt->accept (&visitor);

  *os << "*,this->buffer_);" << be_nl
      << class_name << "::freebuf (tmp);" << be_uidt_nl
      << "}" << be_nl
      << "this->buffer_ = data;" << be_nl
      << "this->release_ = release;" << be_uidt_nl
      << "}" << be_nl;

  os->gen_endif (); // endif macro

  // generate #endif for AHETI
  os->gen_endif_AHETI();

  return 0;
}
