//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    gen_bounded_obj_sequence_cs.cpp
//
// = DESCRIPTION
//    Visitor generating code for Sequence in the client impl
//
// = AUTHOR
//    Michael Kircher
//
//    Modifications by Aniruddha Gokhale
// ============================================================================

#include	"be.h"

#include "be_visitor_sequence.h"

ACE_RCSID(be_visitor_sequence, gen_bounded_obj_sequence_cs, "$Id$")


int
be_visitor_sequence_cs::gen_bounded_obj_sequence (be_sequence *node)
{
  TAO_OutStream *os = this->ctx_->stream ();
  be_type *bt;

  // retrieve the base type since we may need to do some code
  // generation for the base type.
  bt = be_type::narrow_from_decl (node->base_type ());
  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_sequence_cs::"
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
  ctx.state (TAO_CodeGen::TAO_SEQUENCE_BASE_CS);
  be_visitor *visitor = tao_cg->make_visitor (&ctx);

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

  // first generate the static methods since they are used by others. Sinc
  // ethey are inlined, their definition needs to come before their use else
  // some compilers give lost of warnings.

  // allocate_buffer
  *os << "// The Base_Sequence functions, please see tao/sequence.h" << be_nl
      << "void " << be_nl
      << full_class_name << "::_allocate_buffer (CORBA::ULong length)" << be_nl
      << "{" << be_idt_nl
      << "// For this class memory is never reallocated so the implementation" << be_nl
      << "// is *really* simple." << be_nl
      << "this->buffer_ = " << full_class_name << "::allocbuf (length);" << be_uidt_nl
      << "}" << be_nl
      << be_nl;

  // deallocate_buffer
  *os << "void" << be_nl
      << full_class_name << "::_deallocate_buffer (void)" << be_nl
      << "{" << be_idt_nl
      << "if (this->buffer_ == 0 || this->release_ == 0)" << be_idt_nl
      << "return;" << be_uidt_nl;
  pt->accept(visitor); 
  *os <<" **tmp = ACE_reinterpret_cast ("; 
  pt->accept (visitor); 
  *os << " **, this->buffer_);" << be_nl
      << full_class_name << "::freebuf (tmp);" << be_nl
      << "this->buffer_ = 0;" << be_uidt_nl
      << "}" << be_nl
      << be_nl;
  
  // destructor
  *os << full_class_name << "::~" << class_name << " (void)" << be_nl
      << "// destructor" << be_nl
      << "{" << be_idt_nl
      << "this->_deallocate_buffer ();" << be_uidt_nl
      << "}" << be_nl
      << be_nl;

  // _shrink_buffer
  *os << "void" << be_nl
      << full_class_name << "::_shrink_buffer (CORBA::ULong nl, CORBA::ULong ol)" << be_nl
      << "{" << be_idt_nl;
  pt->accept(visitor);
  *os <<" **tmp = ACE_reinterpret_cast ("; 
  pt->accept (visitor);
  *os << " **, this->buffer_);" << be_nl
      << be_nl
      << "for (CORBA::ULong i = nl; i < ol; ++i)" << be_nl
      << "{" << be_idt_nl
      << "CORBA::release (tmp[i]);" << be_nl
      << "tmp[i] = ";
  pt->accept (visitor);
  *os << "::_nil ();" << be_uidt_nl
      << "}" << be_uidt_nl
      << "}\n" << be_nl;

  be_predefined_type *prim = be_predefined_type::narrow_from_decl (pt);
  if ((pt->node_type () != AST_Decl::NT_pre_defined) ||
      (prim && (prim->pt () == AST_PredefinedType::PT_pseudo) &&
       (!ACE_OS::strcmp (prim->local_name ()->get_string (), "Object"))))
    {
      // Pseudo objects do not require this methods.
      *os << "void" << be_nl
          << full_class_name << "::_downcast (" << be_idt << be_idt_nl
	  << "void* target," << be_nl
	  << "CORBA_Object *src," << be_nl
	  << "CORBA::Environment &ACE_TRY_ENV" << be_uidt_nl
	  << ")" << be_uidt_nl
	  << "{" << be_idt_nl;
      pt->accept (visitor);
      *os << " **tmp = ACE_static_cast (";
      pt->accept (visitor);
      *os << "**, target);" << be_nl
	  << "*tmp = ";
      pt->accept (visitor);
      *os << "::_narrow (src, ACE_TRY_ENV);" << be_uidt_nl
	  << "}\n" << be_nl;

      *os << "CORBA_Object*" << be_nl
          << full_class_name << "::_upcast (void *src) const" <<  be_nl
	  << "{" << be_idt_nl;
      pt->accept (visitor);
      *os << " **tmp = ACE_static_cast (";
      pt->accept (visitor);
      *os << "**, src);" << be_nl
	  << "return *tmp;" << be_uidt_nl
	  << "}" << be_nl;
    }

  os->gen_endif ();

  // generate #endif for AHETI
  os->gen_endif_AHETI();

  delete visitor;
  return 0;
}

