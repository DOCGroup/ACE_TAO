//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    gen_unbounded_obj_sequence_ch.cpp
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

ACE_RCSID(be_visitor_sequence, gen_unbounded_obj_sequence_ch, "$Id$")


int
be_visitor_sequence_ch::gen_unbounded_obj_sequence (be_sequence *node)
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

  const char * object_manager = node->object_manager_name ();

  // !! branching in either compile time template instantiation
  // or manual template instatiation
  os->gen_ifdef_AHETI();

  os->gen_ifdef_macro (class_name);

  os->indent ();
  
  //forward declaration of the object manager;
  *os << "class " << object_manager << ";" << be_nl << be_nl;

  *os << "class " << class_name << " : public TAO_Unbounded_Base_Sequence" << be_nl
      << "{" << be_nl
      << "public:" << be_idt_nl
      << "// = Initialization and termination methods." << be_nl
      << be_nl;

  // constructor
  *os << class_name << " (void);" << be_nl;

  // constructor
  *os << class_name << " (CORBA::ULong maximum);" << be_nl;

  // constructor
  *os << class_name << " (CORBA::ULong maximum," << be_idt_nl
      << "CORBA::ULong length," << be_nl;
  pt->accept (visitor); 
  *os <<"* *value," << be_nl
      << "CORBA::Boolean release = 0);" << be_uidt_nl;

  // constructor
  *os << class_name << "(const " << class_name << " &rhs);" << be_nl;

  // destructor
  *os << "virtual ~" << class_name << " (void);" << be_nl;

  // operator=
  *os << class_name << " &operator= (const " << class_name << " &rhs);" << be_nl;

  // operator[]
  *os << object_manager << " operator[] (CORBA::ULong index) const;" << be_nl;

  // allocbuf
  *os << "static "; 
  pt->accept (visitor); 
  *os << " **allocbuf (CORBA::ULong nelems);" << be_nl;

  // freebuf
  *os << "static void freebuf (";
  pt->accept (visitor);
  *os << " **buffer);" << be_nl;

  // allocate_buffer
  *os << "// The Base_Sequence functions, please see tao/Sequence.h" << be_nl
      << "virtual void _allocate_buffer (CORBA::ULong length);" << be_nl;

  // deallocate_buffer
  *os << "virtual void _deallocate_buffer (void);" << be_nl;

  // get_buffer
  pt->accept(visitor); 
  *os << "* *get_buffer (CORBA::Boolean orphan = 0);" << be_nl;

  // get_buffer
  *os << "const "; 
  pt->accept (visitor); 
  *os << "* *get_buffer (void) const;" << be_nl;

  // shrink_buffer
  *os << "virtual void _shrink_buffer (CORBA::ULong nl, CORBA::ULong ol);" << be_nl;
  
  be_predefined_type *prim = be_predefined_type::narrow_from_decl (pt);
  if ((pt->node_type () != AST_Decl::NT_pre_defined) ||
      (prim && (prim->pt () == AST_PredefinedType::PT_pseudo) &&
       (!ACE_OS::strcmp (prim->local_name ()->get_string (), "Object"))))
    {
      // Pseudo objects do not require this methods.
      *os << "virtual void _downcast (" << be_idt << be_idt_nl
	  << "void* target," << be_nl
	  << "CORBA_Object *src," << be_nl
	  << "CORBA_Environment &ACE_TRY_ENV = "  << be_idt_nl
	  << "CORBA::Environment::default_environment ()"
	  << be_uidt << be_uidt_nl
	  << ");" << be_uidt_nl;

      *os << "virtual CORBA_Object* _upcast (void *src) const;" <<  be_nl;
    }
  *os << be_uidt_nl << "};\n"; 

  os->gen_endif (); // endif macro

  // generate #endif for AHETI
  os->gen_endif_AHETI();

  // Due to a bug with g++2.7.2.3 we have to generate the object manager class
  // after the sequence class and instead forward declare it above

  this->gen_object_manager (node);
  // Generate the code for the object manager

  delete visitor;
  return 0;
}

