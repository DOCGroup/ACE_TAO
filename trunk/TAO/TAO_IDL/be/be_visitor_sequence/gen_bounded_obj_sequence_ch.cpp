//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    gen_bounded_obj_sequence_ch.cpp
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

#include	"be.h"

#include "be_visitor_sequence.h"

ACE_RCSID(be_visitor_sequence, gen_bounded_obj_sequence_ch, "$Id$")


int
be_visitor_sequence_ch::gen_bounded_obj_sequence (be_sequence *node)
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

  be_visitor_context ctx (*this->ctx_);
  ctx.state (TAO_CodeGen::TAO_SEQUENCE_BASE_CH);
  be_visitor *visitor = tao_cg->make_visitor (&ctx);

  // !! branching in either compile time template instantiation
  // or manual template instantiation
  os->gen_ifdef_AHETI();

  os->gen_ifdef_macro (class_name);

  os->indent ();

  *os << "class " << class_name
      << " : public TAO_Bounded_Base_Sequence" << be_nl
      << "{" << be_nl
      << "public:" << be_idt_nl
      << "// = Initialization and termination methods." << be_nl
      << be_nl;

  // default constructor
  *os << class_name << " (void);" << be_nl;

  // constructor
  *os << class_name << " (CORBA::ULong length," << be_idt_nl;
  // the accept is here the first time used and if an
  // error occurs, it will occur here. Later no check
  // for errors will be done.
  if (bt->accept (visitor) == -1)
  {
     ACE_ERROR_RETURN ((LM_ERROR,
                        "(%N:%l) be_visitor_sequence_ch::"
                        "visit_sequence - "
                        "base type visit failed\n"),
                        -1);
  }
  *os <<"* *value," << be_nl
      << "CORBA::Boolean release = 0);" << be_uidt_nl;

  // constructor
  *os << class_name << " (const " << class_name << " &rhs);" << be_nl
      << "// Copy constructor." << be_nl;

  // destructor
  *os << "virtual ~" << class_name << " (void);" << be_nl
      << "// destructor" << be_nl;

  // operator=
  *os << class_name << " &operator= (const " << class_name << " &rhs);" << be_nl
      << "// Assignment from another Bounded sequence." << be_nl;

  be_predefined_type *prim = be_predefined_type::narrow_from_decl (pt);
  int is_pseudo_object =
    pt->node_type () == AST_Decl::NT_pre_defined
    && prim && prim->pt () == AST_PredefinedType::PT_pseudo
    && ACE_OS::strcmp (prim->local_name ()->get_string (),
                       "Object") != 0;

  // operator[]
  if (is_pseudo_object)
    {
      *os << "TAO_Pseudo_Object_Manager<";
    }
  else
    {
      *os << "TAO_Object_Manager<";
    }

  *os << bt->name () << ","
      << bt->name () << "_var>"
      << " operator[] (CORBA::ULong index) const;"
      << "// Read-write accessor." << be_nl;

  // allocbuf
  *os << "static ";
  bt->accept (visitor);
  *os << " **allocbuf (CORBA::ULong length); "
      << "// Allocate storage for a sequence.." << be_nl;

  // freebuf
  *os << "static void freebuf (";
  bt->accept (visitor);
  *os << " **buffer);" << be_nl;

  // allocate_buffer
  *os << "// The Base_Sequence functions, please see tao/sequence.h" << be_nl
      << "virtual void _allocate_buffer (CORBA::ULong length);" << be_nl;

  // deallocate_buffer
  *os << "virtual void _deallocate_buffer (void);" << be_nl;

  // get_buffer
  bt->accept(visitor);
  *os << "* *get_buffer (CORBA::Boolean orphan = 0);" << be_nl;

  // get_buffer
  *os << "const ";
  bt->accept (visitor);
  *os << "* *get_buffer (void) const;" << be_nl;

  // _shrink_buffer
  *os << "virtual void _shrink_buffer (CORBA::ULong nl, CORBA::ULong ol);" << be_nl;

  if (!is_pseudo_object)
    {
      // Pseudo objects do not require this methods.
      *os << "virtual void _downcast (" << be_idt << be_idt_nl
	  << "void* target," << be_nl
	  << "CORBA_Object *src," << be_nl
	  << "CORBA_Environment &ACE_TRY_ENV = "  << be_idt_nl
	  << "TAO_default_environment ()"
	  << be_uidt << be_uidt_nl
	  << ");" << be_uidt_nl;

      *os << "virtual CORBA_Object* _upcast (void *src) const;" <<  be_nl;
    }
  *os << be_uidt_nl << "};\n";

  os->gen_endif ();

  // generate #endif for AHETI
  os->gen_endif_AHETI ();

  delete visitor;
  return 0;
}
