// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    gen_unbounded_sequence_ch.cpp
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

//#include	"idl.h"
//#include	"idl_extern.h"
#include	"be.h"

#include "be_visitor_sequence.h"

ACE_RCSID(be_visitor_sequence, gen_unbounded_sequence_ch, "$Id$")


int
be_visitor_sequence_ch::gen_unbounded_sequence (be_sequence *node)
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

  // !! branching in either compile time template instantiation
  // or manual template instatiation
  os->gen_ifdef_AHETI();

  os->gen_ifdef_macro (class_name);

  os->indent ();

  *os << "class " << class_name << " : public TAO_Unbounded_Base_Sequence" << be_nl
      << "{" << be_nl
      << "public:" << be_idt_nl
      << "// = Initialization and termination methods." << be_nl
      << be_nl;
  // constructor
  *os << class_name << " (void); // Default constructor." << be_nl;

  // constructor
  *os << class_name << " (CORBA::ULong maximum); " << be_nl;

  // constructor
  *os << class_name << " (CORBA::ULong maximum," << be_idt_nl
      << "CORBA::ULong length," << be_nl;
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
  *os << " *data," << be_nl
      << "CORBA::Boolean release = 0);" << be_uidt_nl;

  // constructor
  *os << class_name << " (const " << class_name << " &rhs);" << be_nl;

  // operator =
  *os << class_name << " &operator= (const " << class_name << " &rhs);" << be_nl;

  // destructor
  *os << "virtual ~" << class_name << " (void); // Dtor." << be_nl;

  // Accessors
  *os << "// = Accessors." << be_nl;
  pt->accept (visitor);
  *os <<" &operator[] (CORBA::ULong i);" << be_nl;

  // operator[]
  *os << "const ";
  pt->accept (visitor);
  *os << " &operator[] (CORBA::ULong i) const;" << be_nl;

  // Static operations
  *os << "// = Static operations." << be_nl
      << "static ";
  pt->accept (visitor);
  *os << " *allocbuf (CORBA::ULong size);" << be_nl;

  *os << "static void freebuf (";
  pt->accept (visitor);
  *os << " *buffer);" << be_nl;

  // allocate_buffer
  *os << "virtual void _allocate_buffer (CORBA::ULong length);" << be_nl;

  // deallocate_buffer
  *os << "virtual void _deallocate_buffer (void);" << be_nl;

  // Implement the TAO_Base_Sequence methods (see Sequence.h)
  *os << "// Implement the TAO_Base_Sequence methods (see Sequence.h)" << be_nl
      << be_nl;
  pt->accept(visitor);
  *os << " *get_buffer (CORBA::Boolean orphan = 0);" << be_nl;

  // get_buffer
  *os << "const ";
  pt->accept (visitor);
  *os << " *get_buffer (void) const;" << be_nl;

  // replace
  *os << "void replace (CORBA::ULong max," << be_idt_nl
      << "CORBA::ULong length," << be_nl;
  pt->accept(visitor);
  *os <<" *data," << be_nl
      << "CORBA::Boolean release);" << be_uidt << be_uidt_nl;

  *os << "};\n";

  os->gen_endif (); // endif macro

  // generate #endif for AHETI
  os->gen_endif_AHETI();

  delete visitor;
  return 0;
}
