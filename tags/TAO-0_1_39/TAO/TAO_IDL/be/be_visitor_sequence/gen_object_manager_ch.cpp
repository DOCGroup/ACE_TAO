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
// ============================================================================


#include	"be.h"

#include "be_visitor_sequence.h"


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

  os->gen_ifdef_macro (object_manager);

  os->indent ();

  *os << "class " << object_manager << be_nl
      << "{" << be_idt_nl
      << "friend class " << class_name << ";" << be_uidt_nl
      << "public:" << be_idt_nl
      << be_nl;

  // constructor
  *os << "// = Initialization and termination methods." << be_nl
      << object_manager << " (const " << object_manager << " &rhs)" << be_idt_nl
      << ": ptr_ (rhs.ptr_)," << be_nl
      << "release_ (rhs.release_)" << be_uidt_nl
      << "{" << be_nl
      << "}" << be_nl
      << be_nl;

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
  *os << "** buffer, CORBA::Boolean release)" << be_idt_nl
      << ": ptr_ (buffer)," << be_nl
      << "release_ (release)" << be_uidt_nl
      << "{" << be_nl
      << "}" << be_nl
      << be_nl;

  // destructor
  *os << "~" << object_manager << " (void)" << be_nl
      << "{" << be_nl
      << "}" << be_nl
      << be_nl;

  // operator=
  *os << object_manager << " &operator= (const " << object_manager << " &rhs)" << be_nl
      << "{" << be_idt_nl
      << "if (this == &rhs)" << be_idt_nl
      << "return *this;" << be_uidt_nl
      << be_nl
      << "if (this->release_)" << be_nl
      << "{" << be_idt_nl
      << "CORBA::release (*this->ptr_);" << be_nl
      << "*this->ptr_ = "; pt->accept (visitor); *os << "::_duplicate (*rhs.ptr_);" << be_uidt_nl
      << "}" << be_nl
      << "else" << be_idt_nl
      << "*this->ptr_ = *rhs.ptr_;" << be_uidt_nl
      << be_nl
      << "return *this;" << be_uidt_nl
      << "}" << be_nl
      << be_nl;

  // operator=
  *os << object_manager << " &operator= ("; pt->accept (visitor); *os << " *p)" << be_nl
      << "// Assignment from "; pt->accept (visitor); *os << " *." << be_nl
      << "{" << be_idt_nl
      << "if (this->release_)" << be_nl
      << "{" << be_idt_nl
      << "CORBA::release (*this->ptr_);" << be_nl
      << "*this->ptr_ = p;" << be_uidt_nl
      << "}" << be_nl
      << "else" << be_idt_nl
      << "*this->ptr_ = p;" << be_uidt_nl
      << be_nl
      << "return *this;" << be_uidt_nl
      << "}" << be_nl
      << be_nl;

  //  cast operator
  *os << "operator const "; pt->accept (visitor); *os << " *() const // Cast (read-only)." << be_nl
      << "{" << be_idt_nl
      << "return *this->ptr_;" << be_uidt_nl
      << "}" << be_nl
      << be_nl;

  // cast operator
  *os << "operator "; pt->accept (visitor); *os << " *&() // Cast." << be_nl
      << "{" << be_idt_nl
      << "return *this->ptr_;" << be_uidt_nl
      << "}" << be_nl
      << be_uidt_nl;

  // members
  *os << "private:" << be_idt_nl;
  pt->accept(visitor); *os <<" **ptr_;" << be_nl
      << "// data member, notice that it is a pointer, to implement the" << be_nl
      << "// reference behavior for assignment." << be_nl
      << be_nl
      << "CORBA::Boolean release_;" << be_nl
      << "// release flag based on parent's flag" << be_uidt_nl
      << "};" << be_nl;

  os->gen_endif (); // endif macro

  delete visitor;
  return 0;
}

