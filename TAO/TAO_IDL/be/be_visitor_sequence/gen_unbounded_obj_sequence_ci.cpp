//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    gen_unbounded_obj_sequence_ci.cpp
//
// = DESCRIPTION
//    Visitor generating code for Sequence in the client inline.
//
// = AUTHOR
//    Michael Kircher
//
//    Modifications by Aniruddha Gokhale
//
// ============================================================================


#include	"be.h"

#include "be_visitor_sequence.h"

ACE_RCSID(be_visitor_sequence, gen_unbounded_obj_sequence_ci, "$Id$")


int
be_visitor_sequence_ci::gen_unbounded_obj_sequence (be_sequence *node)
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

  static char full_class_name [NAMEBUFSIZE];
  ACE_OS::memset (full_class_name, '\0', NAMEBUFSIZE);

  if (node->is_nested ())
    {
      ACE_OS::sprintf (full_class_name, "%s::%s",
                       be_scope::narrow_from_scope (node->defined_in ())->decl ()->fullname (),
                       class_name);
    }
  else
    {
      ACE_OS::sprintf (full_class_name, "%s",
                       class_name);
    }

  be_visitor_context ctx (*this->ctx_);
  ctx.state (TAO_CodeGen::TAO_SEQUENCE_BASE_CI);
  be_visitor *visitor = tao_cg->make_visitor (&ctx);

  this->gen_object_manager (node);

  static char object_manager [NAMEBUFSIZE];
  ACE_OS::memset (object_manager, '\0', NAMEBUFSIZE);

  if (node->is_nested ())
    {
      ACE_OS::sprintf (object_manager, "%s::%s",
                       be_scope::narrow_from_scope (node->defined_in ())->decl ()->fullname (),
                       node->object_manager_name ());
    }
  else
    {
      ACE_OS::sprintf (object_manager, "%s",
                       node->object_manager_name ());
    }

  // !! branching in either compile time template instantiation
  // or manual template instatiation
  os->gen_ifdef_AHETI();

  os->gen_ifdef_macro (class_name);

  os->indent ();
  
  // allocbuf
  *os << "ACE_INLINE "; 
  // the accept is here the first time used and if an 
  // error occurs, it will occur here. Later no check
  // for errors will be done.
  if (pt->accept (visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_sequence_ci::"
                         "gen_unbounded_obj_sequence - "
                         "base type visit failed\n"),
                        -1);
    }
  *os << " **" << be_nl
      << full_class_name << "::allocbuf (CORBA::ULong nelems)" << be_nl
      << "{" << be_idt_nl;
  pt->accept(visitor); 
  *os <<" **buf;" << be_nl
      << be_nl
      << "ACE_NEW_RETURN (buf, "; 
  pt->accept (visitor); 
  *os << "*[nelems], 0);" << be_nl
      << be_nl
      << "for (CORBA::ULong i = 0; i < nelems; i++)" << be_idt_nl
      << "buf[i] = "; 
  pt->accept (visitor); 
  *os << "::_nil ();" << be_uidt_nl
      << be_nl
      << "return buf;" << be_uidt_nl
      << "}" << be_nl
      << be_nl;

  // freebuf
  *os << "ACE_INLINE void " << be_nl
      << full_class_name << "::freebuf ("; 
  pt->accept (visitor); 
  *os << " **buffer)" << be_nl
      << "{" << be_idt_nl
      << "if (buffer == 0)" << be_idt_nl
      << "return;" << be_uidt_nl
      << "delete[] buffer;" << be_uidt_nl
      << "}" << be_nl
      << be_nl;

  // constructor
  *os << "ACE_INLINE" << be_nl
      << full_class_name << "::" << class_name << " (void)" << be_nl
      << "{" << be_nl
      << "}" << be_nl
      << be_nl;

  // constructor
  *os << "ACE_INLINE" << be_nl
      << full_class_name << "::" << class_name << " (CORBA::ULong maximum)" << be_idt_nl
      << ": TAO_Unbounded_Base_Sequence (maximum, "
      << full_class_name << "::allocbuf (maximum))" << be_uidt_nl
      << "{" << be_nl
      << "}" << be_nl
      << be_nl;

  // constructor
  *os << "ACE_INLINE" << be_nl
      << full_class_name << "::" << class_name << " (CORBA::ULong maximum," << be_idt_nl
      << "CORBA::ULong length," << be_nl;
  pt->accept (visitor); 
  *os <<"* *value," << be_nl
      << "CORBA::Boolean release)" << be_uidt_nl
      << ": TAO_Unbounded_Base_Sequence (maximum, length, value, release)" << be_nl
      << "{" << be_nl
      << "}" << be_nl
      << be_nl;

  // constructor
  *os << "ACE_INLINE" << be_nl
      << full_class_name << "::" << class_name << "(const " 
      << full_class_name << " &rhs)" << be_idt_nl
      << ": TAO_Unbounded_Base_Sequence (rhs)" << be_uidt_nl
      << "{" << be_idt_nl;
  pt->accept(visitor); 
  *os <<" **tmp1 = " << full_class_name << "::allocbuf (this->maximum_);" << be_nl;
  pt->accept(visitor); 
  *os <<" ** const tmp2 = ACE_reinterpret_cast ("; 
  pt->accept (visitor); 
  *os << " ** ACE_CAST_CONST, rhs.buffer_);" << be_nl
      << be_nl
      << "for (CORBA::ULong i = 0; i < rhs.length_; ++i)" << be_idt_nl
      << "tmp1[i] = "; 
  pt->accept (visitor); *os << "::_duplicate (tmp2[i]);" << be_uidt_nl
      << be_nl
      << "this->buffer_ = tmp1;" << be_uidt_nl
      << "}" << be_nl
      << be_nl;
      
  // operator=
  *os << "ACE_INLINE " << full_class_name << " &" << be_nl
      << full_class_name << "::operator= (const " << full_class_name << " &rhs)" << be_nl
      << "{" << be_idt_nl
      << "if (this == &rhs)" << be_idt_nl
      << "return *this;" << be_uidt_nl
      << be_nl
      << "if (this->release_)" << be_nl
      << "{" << be_idt_nl;
  pt->accept(visitor); 
  *os <<" **tmp = ACE_reinterpret_cast (";
  pt->accept (visitor); 
  *os << " **, this->buffer_);" << be_nl
      << be_nl
      << "for (CORBA::ULong i = 0; i < this->length_; ++i)" << be_nl
      << "{" << be_idt_nl
      << "CORBA::release (tmp[i]);" << be_nl
      << "tmp[i] = "; 
  pt->accept (visitor); 
  *os << "::_nil ();" << be_uidt_nl
      << "}" << be_nl
      << "if (this->maximum_ < rhs.maximum_)" << be_nl
      << "{" << be_idt_nl
      << full_class_name << "::freebuf (tmp);" << be_nl
      << "this->buffer_ = " << full_class_name 
      << "::allocbuf (rhs.maximum_);" << be_uidt_nl
      << "}" << be_uidt_nl
      << "}" << be_nl
      << "else" << be_idt_nl
      << "this->buffer_ = " << full_class_name 
      << "::allocbuf (rhs.maximum_);" << be_uidt_nl
      << be_nl
      << "TAO_Unbounded_Base_Sequence::operator= (rhs);" << be_nl
      << be_nl;
  pt->accept(visitor); 
  *os <<" **tmp1 = ACE_reinterpret_cast (";
  pt->accept (visitor); 
  *os << " **, this->buffer_);" << be_nl;
  pt->accept(visitor); 
  *os <<" ** const tmp2 = ACE_reinterpret_cast (";
  pt->accept (visitor); 
  *os << " ** ACE_CAST_CONST, rhs.buffer_);" << be_nl
      << be_nl
      << "for (CORBA::ULong i = 0; i < rhs.length_; ++i)" << be_idt_nl
      << "tmp1[i] = "; 
  pt->accept (visitor); 
  *os << "::_duplicate (tmp2[i]);" << be_uidt_nl
      << be_nl
      << "return *this;" << be_uidt_nl
      << "}" << be_nl
      << be_nl;
  
  // operator[]
  *os << "ACE_INLINE " << object_manager << be_nl
      << full_class_name << "::operator[] (CORBA::ULong index) const" << be_nl
      << "// read-write accessor" << be_nl
      << "{" << be_idt_nl
      << "ACE_ASSERT (index < this->maximum_);" << be_nl;
  pt->accept(visitor); 
  *os <<" ** const tmp = ACE_reinterpret_cast (";
  pt->accept (visitor); 
  *os << " ** ACE_CAST_CONST, this->buffer_);" << be_nl
      << "return " << object_manager << " (tmp + index, this->release_);" << be_uidt_nl
      << "}" << be_nl
      << be_nl;
  
  // get_buffer
  *os << "ACE_INLINE ";
  pt->accept(visitor); 
  *os << "* *" << be_nl
      << full_class_name << "::get_buffer (CORBA::Boolean orphan)" << be_nl
      << "{" << be_idt_nl;
  pt->accept(visitor); 
  *os << " **result = 0;" << be_nl
      << "if (orphan == 0)" << be_nl
      << "{" << be_idt_nl
      << "// We retain ownership." << be_nl
      << "if (this->buffer_ == 0)" << be_nl
      << "{" << be_idt_nl
      << "result = " << full_class_name << "::allocbuf (this->maximum_);" << be_nl
      << "this->buffer_ = result;" << be_uidt_nl
      << "}" << be_nl
      << "else" << be_nl
      << "{" << be_idt_nl
      << "result = ACE_reinterpret_cast (";
  pt->accept (visitor);
  *os << "**, this->buffer_);" << be_uidt_nl
      << "}" << be_uidt_nl
      << "}" << be_nl
      << "else // if (orphan == 1)" << be_nl
      << "{" << be_idt_nl
      << "if (this->release_ != 0)" << be_nl
      << "{" << be_idt_nl
      << "// We set the state back to default and relinquish" << be_nl
      << "// ownership." << be_nl
      << "result = ACE_reinterpret_cast("; 
  pt->accept (visitor); 
  *os << "**,this->buffer_);" << be_nl
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
  *os << "ACE_INLINE ";
  *os << "const "; 
  pt->accept (visitor); 
  *os << "* *" << be_nl
      << full_class_name << "::get_buffer (void) const" << be_nl
      << "{" << be_idt_nl
      << "return ACE_reinterpret_cast(const ";
  pt->accept (visitor);
  *os << " ** ACE_CAST_CONST, this->buffer_);" << be_uidt_nl
      << "}" << be_nl
      << be_nl;

  os->gen_endif (); // endif macro

  // generate #endif for AHETI
  os->gen_endif_AHETI();

  delete visitor;
  return 0;
}

