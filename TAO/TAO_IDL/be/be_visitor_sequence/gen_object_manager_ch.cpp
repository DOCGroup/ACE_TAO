//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    gen_object_manager_ch.cpp
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

ACE_RCSID(be_visitor_sequence, gen_object_manager_ch, "$Id$")


int
be_visitor_sequence_ch::gen_object_manager (be_sequence *node)
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
  // we might want to use this later
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
  // create the name for the object manager

  // !! branching in either compile time template instantiation
  // or manual template instatiation
  os->gen_ifdef_AHETI();

  os->gen_ifdef_macro (object_manager);

  os->indent ();

  *os << "class " << object_manager << be_nl
      << "{" << be_idt_nl
      << "friend class " << class_name << ";" << be_uidt_nl
      << "public:" << be_idt_nl
      << be_nl;

  // constructor
  *os << "// = Initialization and termination methods." << be_nl
      << object_manager << " (const " << object_manager << " &rhs);" << be_nl;

  // constructor
  *os << object_manager << " ("; 
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
  *os << "** buffer, CORBA::Boolean release);" << be_nl;

  // destructor
  *os << "~" << object_manager << " (void);" << be_nl;

  // operator=
  *os << object_manager << " &operator= (const " << object_manager << " &rhs);" << be_nl;

  // operator=
  *os << object_manager << " &operator= ("; 
  pt->accept (visitor); 
  *os << " *p);" << be_nl;

  // operator -> @@Bala
  pt->accept (visitor); 
  *os << "* operator-> (void) const;" << be_nl;

  //  cast operator
  *os << "operator const "; 
  pt->accept (visitor); 
  *os << " *() const; // Cast (read-only)." << be_nl;

  // cast operator
  *os << "operator "; 
  pt->accept (visitor); 
  *os << " *&(); // Cast." << be_nl;
  
  // @@Bala
  // in method
  //*os << "const "; 
  pt->accept (visitor); 
  *os << " *in (void) const; // in " << be_nl;
                                           
  // inout method
  pt->accept (visitor); 
  *os << " *&inout (void); // inout " << be_nl;
                                           
  // out method
  pt->accept (visitor);
  *os << " *&out (void); // out " << be_nl;
                                           
  // retn method
  pt->accept (visitor); 
  *os << " *_retn (void); // retn " << be_uidt_nl;
                                           
  // members
  *os << "private:" << be_idt_nl;
  pt->accept(visitor); 
  *os <<" **ptr_;" << be_nl
      << "// data member, notice that it is a pointer, to implement the" << be_nl
      << "// reference behavior for assignment." << be_nl
      << be_nl
      << "CORBA::Boolean release_;" << be_nl
      << "// release flag based on parent's flag" << be_uidt_nl
      << "};" << be_nl;
  
  os->gen_endif (); // endif macro

  // generate #endif for AHETI
  os->gen_endif_AHETI();

  delete visitor;
  return 0;
}

