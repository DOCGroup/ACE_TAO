// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    gen_bounded_sequence_cs.cpp
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
           gen_bounded_sequence_cs, 
           "$Id$")

int
be_visitor_sequence_cs::gen_bounded_sequence (be_sequence *node)
{
  TAO_OutStream *os = this->ctx_->stream ();
  be_type *bt = be_type::narrow_from_decl (node->base_type ());

  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_sequence_cs::"
                         "visit_sequence - "
                         "Bad element type\n"), -1);
    }

  // If we contain an anonymous sequence,
  // generate code for that sequence here.
  if (bt->node_type () == AST_Decl::NT_sequence)
    {
      int status =
        this->gen_anonymous_base_type (bt,
                                       TAO_CodeGen::TAO_SEQUENCE_CS);

      if (status == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_sequence_cs::"
                             "gen_bounded_sequence - "
                             "gen_anonymous_base_type failed\n"),
                            -1);
        }
    }

  const char * class_name = node->instance_name ();
  static char full_class_name [NAMEBUFSIZE];

  ACE_OS::memset (full_class_name, 
                  '\0', 
                  NAMEBUFSIZE);

  be_decl *tmp =
    be_scope::narrow_from_scope (node->defined_in ())->decl ();

  if (node->is_nested ())
    {
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


  be_visitor_context ctx (*this->ctx_);
  ctx.state (TAO_CodeGen::TAO_SEQUENCE_BASE_CS);
  be_visitor_sequence_base visitor (&ctx);

  *os << be_nl << be_nl << "// TAO_IDL - Generated from " << be_nl
      << "// " << __FILE__ << ":" << __LINE__;

  os->gen_ifdef_AHETI();
  os->gen_ifdef_macro (class_name);

  // allocate_buffer
  *os << be_nl << be_nl << "void " << be_nl
      << full_class_name << "::_allocate_buffer (CORBA::ULong /* length */)" 
      << be_nl
      << "// allocate a buffer of the requested length. "
      << "The buffer is allocated for the" << be_nl
      << "// right type" << be_nl
      << "{" << be_idt_nl
      << "this->buffer_ = " << class_name << "::allocbuf ("
      << node->max_size () << ");" << be_uidt_nl
      << "}" << be_nl
      << be_nl;

  // deallocate_buffer
  *os << "void " << be_nl
      << full_class_name << "::_deallocate_buffer (void)" << be_nl
      << "// deallocate the buffer" << be_nl
      << "{" << be_idt_nl
      << "if (this->buffer_ == 0 || this->release_ == 0)" << be_idt_nl
      << "{" << be_idt_nl
      << "return;" << be_uidt_nl
      << "}" <<  be_uidt_nl << be_nl;

  bt->accept (&visitor);

  *os <<" *tmp = ACE_reinterpret_cast (";

  bt->accept (&visitor);

  *os << " *, this->buffer_);" << be_nl
      << class_name << "::freebuf (tmp);" << be_nl
      << "this->buffer_ = 0;" << be_uidt_nl
      << "}" << be_nl
      << be_nl;

  // destructor
  *os << full_class_name << "::~" << class_name 
      << " (void) // Dtor." << be_nl
      << "{" << be_idt_nl
      << "this->_deallocate_buffer ();" << be_uidt_nl
      << "}" << be_nl
      << be_nl;

  os->gen_endif (); // endif macro

  // generate #endif for AHETI
  os->gen_endif_AHETI();

  return 0;
}
