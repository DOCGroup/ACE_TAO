//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    gen_object_manager_ci.cpp
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

#include	"be.h"

#include "be_visitor_sequence.h"

ACE_RCSID(be_visitor_sequence, gen_object_manager_ci, "$Id$")


int
be_visitor_sequence_ci::gen_object_manager (be_sequence *node)
{
  TAO_OutStream *os = this->ctx_->stream ();
  be_type *bt;

  // retrieve the base type since we may need to do some code
  // generation for the base type.
  bt = be_type::narrow_from_decl (node->base_type ());
  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_sequence_ci::"
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
    

  be_visitor_context ctx (*this->ctx_);
  ctx.state (TAO_CodeGen::TAO_SEQUENCE_BASE_CI);
  be_visitor *visitor = tao_cg->make_visitor (&ctx);
  
  const char * object_manager = node->object_manager_name ();  

  static char full_object_manager [NAMEBUFSIZE];
  ACE_OS::memset (full_object_manager, '\0', NAMEBUFSIZE);

  if (node->is_nested ())
    {
      ACE_OS::sprintf (full_object_manager, "%s::%s",
                       be_scope::narrow_from_scope (node->defined_in ())->decl ()->fullname (),
                       object_manager);
    }
  else
    {
      ACE_OS::sprintf (full_object_manager, "%s",
                       object_manager);
    }

  // create the name for the object manager

  // !! branching in either compile time template instantiation
  // or manual template instatiation
  os->gen_ifdef_AHETI();

  os->gen_ifdef_macro (object_manager);

  os->indent ();

  // constructor
  *os << "// = Initialization and termination methods." << be_nl
      << "ACE_INLINE" << be_nl
      << full_object_manager << "::" << object_manager << " (const " 
      << full_object_manager << " &rhs)" << be_idt_nl
      << ": ptr_ (rhs.ptr_)," << be_nl
      << "release_ (rhs.release_)" << be_uidt_nl
      << "{" << be_nl
      << "}" << be_nl
      << be_nl;

  // constructor
  *os << "ACE_INLINE" << be_nl
      << full_object_manager << "::" << object_manager << " ("; 
  // the accept is here the first time used and if an 
  // error occurs, it will occur here. Later no check
  // for errors will be done.
  if (pt->accept (visitor) == -1)
  {
     ACE_ERROR_RETURN ((LM_ERROR,
                        "(%N:%l) be_visitor_sequence_ci::"
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
  *os << "ACE_INLINE" << be_nl
      << full_object_manager << "::~" << object_manager << " (void)" << be_nl
      << "{" << be_nl
      << "}" << be_nl
      << be_nl;

  // operator=
  *os << "ACE_INLINE " << full_object_manager << " &" << be_nl
      << full_object_manager << "::operator= (const " << full_object_manager 
      << " &rhs)" << be_nl
      << "{" << be_idt_nl
      << "if (this == &rhs)" << be_idt_nl
      << "return *this;" << be_uidt_nl
      << be_nl
      << "if (this->release_)" << be_nl
      << "{" << be_idt_nl
      << "CORBA::release (*this->ptr_);" << be_nl
      << "*this->ptr_ = "; 
  pt->accept (visitor); 
  *os << "::_duplicate (*rhs.ptr_);" << be_uidt_nl
      << "}" << be_nl
      << "else" << be_idt_nl
      << "*this->ptr_ = *rhs.ptr_;" << be_uidt_nl
      << be_nl
      << "return *this;" << be_uidt_nl
      << "}" << be_nl
      << be_nl;
  
  // operator=
  *os << "ACE_INLINE " << full_object_manager << " &" << be_nl
      << full_object_manager << "::operator= ("; 
  pt->accept (visitor); 
  *os << " *p)" << be_nl
      << "// Assignment from "; 
  pt->accept (visitor); 
  *os << " *." << be_nl
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
  
  // operator -> @@Bala
  *os << "ACE_INLINE " << be_nl;
    pt->accept (visitor);
  *os << "*"<< be_nl
      << full_object_manager <<"::operator-> (void) const " << be_nl
      << "{" << be_idt_nl
      << "return *this->ptr_;" << be_uidt_nl
      << "}" << be_nl
      << be_nl;

  //  cast operator
  *os << "ACE_INLINE " << be_nl
      << full_object_manager << "::operator const "; 
  pt->accept (visitor); 
  *os << " *() const // Cast (read-only)." << be_nl
      << "{" << be_idt_nl
      << "return *this->ptr_;" << be_uidt_nl
      << "}" << be_nl
      << be_nl;

  // cast operator
  *os << "ACE_INLINE" << be_nl
      << full_object_manager << "::operator "; 
  pt->accept (visitor); 
  *os << " *&() // Cast." << be_nl
      << "{" << be_idt_nl
      << "return *this->ptr_;" << be_uidt_nl
      << "}" << be_nl
      << be_nl;
  
  // in method @@Bala
  *os << "ACE_INLINE "; 
  pt->accept (visitor); 
  *os << " *" << be_nl
      << full_object_manager << "::in (void) const // in " 
      << be_nl
      << "{" << be_idt_nl
      << "return *this->ptr_;" << be_uidt_nl
      << "}" << be_nl
      << be_nl;
                                           
  // inout method
  *os << "ACE_INLINE ";
  pt->accept (visitor); 
  *os << " *&" << be_nl
      << full_object_manager << "::inout (void) // inout " 
      << be_nl
      << "{" << be_idt_nl
      << "return *this->ptr_;" << be_uidt_nl
      << "}" << be_nl
      << be_nl;
                                           
  // out method
  *os << "ACE_INLINE ";
  pt->accept (visitor);
  *os << " *&" << be_nl
      << full_object_manager << "::out (void) // out " 
      << be_nl
      << "{" << be_idt_nl
      << "CORBA::release (*this->ptr_);" << be_nl
      << "*this->ptr_ = "; 
  pt->accept (visitor); 
  *os << "::_nil ();"
      << be_nl
      << "return *this->ptr_;" << be_uidt_nl
      << "}" << be_nl
      << be_nl;
                                           
  // retn method
  *os << "ACE_INLINE ";
  pt->accept (visitor); 
  *os << " *" << be_nl
      << full_object_manager << "::_retn (void) // retn " 
      << be_nl
      << "{" << be_idt_nl;
  pt->accept (visitor);
  *os << " *temp = *this->ptr_;" << be_nl
      << "*this->ptr_ = "; 
  pt->accept (visitor); 
  *os << "::_nil ();"
      << be_nl
      << "return temp;" << be_uidt_nl
      << "}" << be_nl;
  
  
  os->gen_endif (); // endif macro
  
  // generate #endif for AHETI
  os->gen_endif_AHETI();
  
  delete visitor;
  return 0;
}



