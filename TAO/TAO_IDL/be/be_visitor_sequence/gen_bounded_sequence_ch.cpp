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
//    Modifications by Aniruddha Gokhale
//
// ============================================================================

ACE_RCSID (be_visitor_sequence, 
           gen_bounded_sequence_ch, 
           "$Id$")

int
be_visitor_sequence_ch::gen_bounded_sequence (be_sequence *node)
{
  TAO_OutStream *os = this->ctx_->stream ();
  be_type *bt = be_type::narrow_from_decl (node->base_type ());

  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_sequence_ch::"
                         "visit_sequence - "
                         "Bad element type\n"), -1);
    }

  // If we contain an anonymous sequence,
  // generate code for that sequence here.
  if (bt->node_type () == AST_Decl::NT_sequence)
    {
      int status = 
        this->gen_anonymous_base_type (bt,
                                       TAO_CodeGen::TAO_SEQUENCE_CH);

      if (status == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_sequence_ch::"
                             "gen_bounded_sequence - "
                             "gen_anonymous_base_type failed\n"),
                            -1);
        }
    }

  const char * class_name = node->instance_name ();

  be_visitor_context ctx (*this->ctx_);
  ctx.state (TAO_CodeGen::TAO_SEQUENCE_BASE_CH);
  be_visitor_sequence_base visitor (&ctx);

  *os << be_nl << "// TAO_IDL - Generated from "
      << __FILE__ << ":" << __LINE__ << be_nl << be_nl;

  // !! branching in either compile time template instantiation
  // or manual template instatiation
  os->gen_ifdef_AHETI();
  os->gen_ifdef_macro (class_name);

  *os << "class TAO_EXPORT_MACRO " << class_name << be_idt_nl
      << ": public TAO_Bounded_Base_Sequence" << be_uidt_nl
      << "{" << be_nl
      << "public:" << be_idt_nl
      << "// = Initialization and termination methods." << be_nl;

  // default Constructor
  *os << class_name << " (void);" << be_nl;

  // constructor
  *os << class_name << " (" << be_idt << be_idt_nl
      << "CORBA::ULong length," << be_nl;

  bt->accept (&visitor);

  *os <<" *data," << be_nl
      << "CORBA::Boolean release = 0" << be_uidt_nl
      << ");" << be_uidt_nl;

  // constructor
  *os << class_name << " (" << be_idt << be_idt_nl
      << "const " << class_name << " &rhs" << be_uidt_nl
      << ");" << be_uidt_nl;

  // operator=
  *os << class_name << " &operator= (" << be_idt << be_idt_nl
      << "const " << class_name << " &rhs" << be_uidt_nl
      << ");" << be_uidt_nl;

  // destructor
  *os << "virtual ~" << class_name << " (void);" << be_nl << be_nl;

  // Accessors
  // operator[]
  *os << "// = Accessors." << be_nl;

  bt->accept (&visitor);

  *os <<" &operator[] (CORBA::ULong i);" << be_nl;

  // operator[]
  *os << "const ";

  bt->accept (&visitor);

  *os << " &operator[] (CORBA::ULong i) const;" << be_nl << be_nl;

  // Static Operations
  *os << "// = Static operations." << be_nl
      << "static ";

  bt->accept (&visitor);

  *os << " *allocbuf (CORBA::ULong);" << be_nl;

  // free_buf
  *os << "static void freebuf (";

  bt->accept (&visitor);

  *os << " *buffer);" << be_nl << be_nl;

  // Implement the TAO_Base_Sequence methods (see Sequence.h)
  *os << "// Implement the TAO_Base_Sequence methods (see Sequence.h)" 
      << be_nl;

  // allocate_buffer
  *os << "virtual void _allocate_buffer (CORBA::ULong length);" << be_nl;

  // deallocate_buffer
  *os << "virtual void _deallocate_buffer (void);" << be_nl;

  // get_buffer
  bt->accept (&visitor);
  *os << " *get_buffer (CORBA::Boolean orphan = 0);" << be_nl;

  // get_buffer
  *os << "const ";

  bt->accept (&visitor);

  *os << " *get_buffer (void) const;" << be_nl;

  // replace
  *os << "void replace (" << be_idt << be_idt_nl
      << "CORBA::ULong max," << be_nl
      << "CORBA::ULong length," << be_nl;

  bt->accept (&visitor);

  *os <<" *data," << be_nl
      << "CORBA::Boolean release = 0" << be_uidt_nl
      << ");" << be_uidt << be_uidt_nl;

  *os << "};" << be_nl;

  os->gen_endif (); // endif macro

  // generate #endif for AHETI
  os->gen_endif_AHETI();

  return 0;
}
