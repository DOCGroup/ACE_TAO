// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    gen_unbounded_sequence_cs.cpp
//
// = DESCRIPTION
//    Visitor generating code for Sequence in the client impl
//
// = AUTHOR
//    Michael Kircher
//
//    Modifications by Aniruddha Gokhale
//
// ============================================================================

#include "be.h"
#include "be_visitor_sequence.h"

ACE_RCSID (be_visitor_sequence, 
           gen_unbounded_sequence_cs, 
           "$Id$")

int
be_visitor_sequence_cs::gen_unbounded_sequence (be_sequence *node)
{
  TAO_OutStream *os = this->ctx_->stream ();
  be_type *bt = be_type::narrow_from_decl (node->base_type ());

  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_sequence_cs::"
                         "visit_sequence - "
                         "Bad element type\n"), -1);
    }

  // If we contain an anonymous sequence,
  // generate code for that sequence here.
  if (bt->node_type () == AST_Decl::NT_sequence)
    {
      int status =
        this->gen_anonymous_base_type (bt,
                                         TAO_CodeGen::TAO_SEQUENCE_CS);

      if (status == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_sequence_cs::"
                             "gen_unbounded_sequence - "
                             "gen_anonymous_base_type failed\n"),
                            -1);
        }
    }

  // Generate the class name.
  be_type  *pt;

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

  static char full_class_name [NAMEBUFSIZE];
  ACE_OS::memset (full_class_name, 
                  '\0', 
                  NAMEBUFSIZE);

  if (node->is_nested ())
    {
      be_decl *tmp = 
        be_scope::narrow_from_scope (node->defined_in ())->decl ();
      
      ACE_OS::sprintf (full_class_name, 
                       "%s::%s",
                       tmp->full_name (),
                       class_name);
    }
  else
    {
      ACE_OS::sprintf (full_class_name, 
                       "%s",
                       class_name);
    }

  // Get the visitor for the type of the sequence.
  be_visitor_context ctx (*this->ctx_);
  ctx.state (TAO_CodeGen::TAO_SEQUENCE_BASE_CS);
  be_visitor_sequence_base visitor (&ctx);

  *os << be_nl << "// TAO_IDL - Generated from "
      << __FILE__ << ":" << __LINE__ << be_nl << be_nl;

  os->gen_ifdef_AHETI();
  os->gen_ifdef_macro (class_name);
  os->indent ();

  // allocate_buffer
  *os << "void" << be_nl
      << full_class_name << "::_allocate_buffer (CORBA::ULong length)" << be_nl
      << "{" << be_idt_nl;

  bt->accept (&visitor);

  *os << "* tmp = 0;" << be_nl
      << "tmp = " << class_name << "::allocbuf (length);" << be_nl
      << be_nl
      << "if (this->buffer_ != 0)" << be_nl
      << "{" << be_idt_nl;

  bt->accept (&visitor);

  *os <<" *old = ACE_reinterpret_cast (";

  bt->accept (&visitor);

  *os << " *,this->buffer_);" << be_nl
      << be_nl
      << "for (CORBA::ULong i = 0; i < this->length_; ++i)" << be_idt_nl;

  if (pt->node_type () == AST_Decl::NT_array)
    {
      bt->accept (&visitor);

      *os << "_var::copy (tmp[i], old[i]);" << be_uidt_nl;
    }
  else
    {
      *os << "tmp[i] = old[i];" << be_uidt_nl;
    }

  *os << be_nl
      << "if (this->release_)" << be_idt_nl
      << class_name << "::freebuf (old);" << be_uidt_nl << be_uidt_nl
      << "}" << be_nl
      << "this->buffer_ = tmp;" << be_uidt_nl
      << "}" << be_nl
      << be_nl;

  // deallocate_buffer
  *os << "void" << be_nl
      << full_class_name << "::_deallocate_buffer (void)" << be_nl
      << "{" << be_idt_nl
      << "if (this->buffer_ == 0 || this->release_ == 0)" << be_idt_nl
      << "return;" << be_uidt_nl
      << be_nl;

  bt->accept (&visitor);

  *os <<" *tmp = ACE_reinterpret_cast (";

  bt->accept (&visitor);

  *os << " *,this->buffer_);" << be_nl
      << be_nl
      << class_name << "::freebuf (tmp);" << be_nl
      << "this->buffer_ = 0;" << be_uidt_nl
      << "} " << be_nl
      << be_nl;

  // destructor
  *os << full_class_name << "::~" << class_name << " (void) // Dtor." << be_nl
      << "{" << be_idt_nl
      << "this->_deallocate_buffer ();" << be_uidt_nl
      << "}" << be_nl
      << be_nl;

  os->gen_endif (); // endif macro

  // generate #endif for AHETI
  os->gen_endif_AHETI();

  return 0;
}
