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
//    Modifications by Aniruddha Gokhale
// ============================================================================

#include	"be.h"

#include "be_visitor_sequence.h"

ACE_RCSID(be_visitor_sequence, gen_bounded_str_sequence_ch, "$Id$")


int
be_visitor_sequence_ch::gen_bounded_str_sequence (be_sequence *node)
{
  TAO_OutStream *os = this->ctx_->stream ();
  be_type *bt;

  // Retrieve the base type since we may need to do some code
  // generation for the base type.
  bt = be_type::narrow_from_decl (node->base_type ());

  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_sequence_ch::"
                         "visit_sequence - "
                         "Bad element type\n"), -1);
    }

  // Generate the class name.
  be_type *pt;

  if (bt->node_type () == AST_Decl::NT_typedef)
    {
      // Get the primitive base type of this typedef node.
      be_typedef *t = be_typedef::narrow_from_decl (bt);
      pt = t->primitive_base_type ();
    }
  else
    {
      pt = bt;
    }

  const char * class_name = node->instance_name ();

  // get the visitor for the type of the sequence
  be_visitor_context ctx (*this->ctx_);
  ctx.state (TAO_CodeGen::TAO_SEQUENCE_BASE_CH);
  be_visitor *visitor = tao_cg->make_visitor (&ctx);

  // !! branching in either compile time template instantiation
  // or manual template instatiation
  os->gen_ifdef_AHETI();

  os->gen_ifdef_macro (class_name);

  // The accept is used here the first time and if an
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

  *os << "class TAO_EXPORT_MACRO " << class_name << be_idt_nl
      << ": public TAO_Bounded_Base_Sequence" << be_uidt_nl
      << "{" << be_nl
      << "public:" << be_idt_nl
      << "// = Initialization and termination methods." << be_nl;

  // constructor
  *os << class_name << " (void);" << be_nl;

  // constructor
  *os << class_name << " (" << be_idt << be_idt_nl
      << "CORBA::ULong length," << be_nl
      << "char* *value," << be_nl
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
  *os << "// = Accessors." << be_nl;

  // operator[]
  *os << "TAO_SeqElem_String_Manager operator[] (CORBA::ULong index) const;" 
      << be_nl << be_nl;

  // Static operations
  *os << "// = Static operations." << be_nl;

  // allocbuf
  *os << "static char **allocbuf (CORBA::ULong length);" << be_nl;

  // freebuf
  *os << "static void freebuf (char **buffer);" << be_nl << be_nl;

  // Implement the TAO_Base_Sequence methods (see Sequence.h)
  *os << "// Implement the TAO_Base_Sequence methods (see Sequence.h)" << be_nl;

  // allocate_buffer
  *os << "virtual void _allocate_buffer (CORBA::ULong length);" << be_nl;

  // deallocate_buffer
  *os << "virtual void _deallocate_buffer (void);" << be_nl;

  // get_buffer
  *os << "char* *get_buffer (CORBA::Boolean orphan = 0);" << be_nl;

  // get_buffer
  *os << "const char* *get_buffer (void) const;" << be_nl;

  // shrink_buffer
  *os << "virtual void _shrink_buffer (" << be_idt << be_idt_nl
      << "CORBA::ULong nl," << be_nl
      << "CORBA::ULong ol" << be_uidt_nl
      << ");" << be_uidt << be_uidt_nl;

  *os << "};" << be_nl;

  os->gen_endif (); // endif macro

  // generate #endif for AHETI
  os->gen_endif_AHETI();


  delete visitor;
  return 0;
}
