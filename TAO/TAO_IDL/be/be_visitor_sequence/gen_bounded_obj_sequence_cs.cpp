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

ACE_RCSID (be_visitor_sequence, 
           gen_bounded_obj_sequence_cs, 
           "$Id$")

int
be_visitor_sequence_cs::gen_bounded_obj_sequence (be_sequence *node)
{
  TAO_OutStream *os = this->ctx_->stream ();
  be_type *bt = be_type::narrow_from_decl (node->base_type ());

  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_sequence_cs::"
                         "visit_sequence - "
                         "Bad element type\n"),
                        -1);
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

  const char *name =
    be_decl::narrow_from_decl (pt)->full_name ();

  idl_bool bt_is_defined;

  // Special cases.
  if (ACE_OS::strcmp (name, "CORBA::Object") == 0
      || ACE_OS::strcmp (name, "CORBA::TypeCode") == 0)
    {
      bt_is_defined = 1;
    }
  else if (pt->node_type () == AST_Decl::NT_interface_fwd)
    {
      AST_InterfaceFwd *ifbt = AST_InterfaceFwd::narrow_from_decl (pt);
      bt_is_defined = ifbt->full_definition ()->is_defined ();
    }
  else
    {
      AST_Interface *ibt = AST_Interface::narrow_from_decl (pt);
      bt_is_defined = ibt->is_defined ();
    }

  int is_valuetype = 0;
  be_interface *bf = be_interface::narrow_from_decl (pt);

  if (bf != 0)
    {
      is_valuetype = bf->is_valuetype ();
    }
  else
    {
      be_interface_fwd *bff = be_interface_fwd::narrow_from_decl (pt);

      if (bff != 0)
        {
          is_valuetype = bff->is_valuetype ();
        }
    }

  const char * class_name = node->instance_name ();

  static char full_class_name [NAMEBUFSIZE];
  ACE_OS::memset (full_class_name,
                  '\0',
                  NAMEBUFSIZE);

  if (node->is_nested ())
    {
      be_scope *parent = be_scope::narrow_from_scope (node->defined_in ());

      ACE_OS::sprintf (full_class_name,
                       "%s::%s",
                       parent->decl ()->full_name (),
                       class_name);
    }
  else
    {
      ACE_OS::sprintf (full_class_name,
                       "%s",
                       class_name);
    }

  be_visitor_context ctx (*this->ctx_);
  ctx.state (TAO_CodeGen::TAO_SEQUENCE_BASE_CS);
  be_visitor_sequence_base visitor (&ctx);

  *os << be_nl << "// TAO_IDL - Generated from "
      << __FILE__ << ":" << __LINE__ << be_nl << be_nl;

  // Branching in either compile time template instantiation
  // or manual template instatiation.
  os->gen_ifdef_AHETI();
  os->gen_ifdef_macro (class_name);
  os->indent ();

  // First generate the static methods since they are used by others. Since
  // they are inlined, their definition needs to come before their use else
  // some compilers give lots of warnings.

  // Allocate_buffer.
  *os << "// The Base_Sequence functions, please see tao/sequence.h"
      << be_nl
      << "void " << be_nl
      << full_class_name << "::_allocate_buffer (CORBA::ULong length)"
      << be_nl
      << "{" << be_idt_nl
      << "// For this class memory is never reallocated so the implementation"
      << be_nl
      << "// is *really* simple." << be_nl
      << "this->buffer_ = " << class_name << "::allocbuf (length);"
      << be_uidt_nl
      << "}" << be_nl
      << be_nl;

  // deallocate_buffer.
  *os << "void" << be_nl
      << full_class_name << "::_deallocate_buffer (void)" << be_nl
      << "{" << be_idt_nl
      << "if (this->buffer_ == 0 || this->release_ == 0)" << be_idt_nl
      << "return;" << be_uidt_nl;

  bt->accept (&visitor);

  *os <<" **tmp = ACE_reinterpret_cast (";

  bt->accept (&visitor);

  *os << " **, this->buffer_);" << be_nl
      << class_name << "::freebuf (tmp);" << be_nl
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
      << full_class_name
      << "::_shrink_buffer (CORBA::ULong nl, CORBA::ULong ol)"
      << be_nl
      << "{" << be_idt_nl;

  bt->accept (&visitor);

  *os <<" **tmp = ACE_reinterpret_cast (";

  bt->accept (&visitor);

  *os << " **, this->buffer_);" << be_nl
      << be_nl
      << "for (CORBA::ULong i = nl; i < ol; ++i)" << be_nl
      << "{" << be_idt_nl;

  if (is_valuetype)
    {
      *os << "if (tmp[i] != 0)" << be_idt_nl
          << "tmp[i]->_remove_ref ();" << be_uidt_nl
          << "tmp[i] = 0;";
    }
  else if (bt_is_defined)
    {
      *os << "CORBA::release (tmp[i]);" << be_nl
          << "tmp[i] = ";

      bt->accept (&visitor);

      *os << "::_nil ();";
    }
  else
    {
      *os << "tao_" << pt->flat_name () << "_release (tmp[i]);" << be_nl
          << "tmp[i] = "
          << "tao_" << pt->flat_name () << "_nil ();";
    }

  *os << be_uidt_nl
      << "}" << be_uidt_nl
      << "}\n" << be_nl;

  be_predefined_type *prim = be_predefined_type::narrow_from_decl (pt);

  if ((pt->node_type () != AST_Decl::NT_pre_defined)
      || (prim
          && (prim->pt () == AST_PredefinedType::PT_pseudo)
          && (!ACE_OS::strcmp (prim->local_name ()->get_string (), "Object"))))
    {
      // Pseudo objects do not require these methods.
      *os << "void" << be_nl
          << full_class_name << "::_downcast (" << be_idt << be_idt_nl
                << "void* target," << be_nl
                << "CORBA_Object *src" << be_nl
                << "ACE_ENV_ARG_DECL" << be_uidt_nl
                << ")" << be_uidt_nl
                << "{" << be_idt_nl;

      bt->accept (&visitor);

      *os << " **tmp = ACE_static_cast (";

      bt->accept (&visitor);

      *os << "**, target);" << be_nl
                << "*tmp = ";

      if (bt_is_defined)
        {
          bt->accept (&visitor);

          *os << "::_narrow (src ACE_ENV_ARG_PARAMETER);";
        }
      else
        {
          *os << "tao_" << pt->flat_name ()
              << "_narrow (src ACE_ENV_ARG_PARAMETER);";
        }

      *os << be_nl
          << "ACE_CHECK;" << be_uidt_nl
          << "}\n" << be_nl;

      *os << "CORBA_Object*" << be_nl
          << full_class_name << "::_upcast (void *src) const" <<  be_nl
          << "{" << be_idt_nl;

      if (bt_is_defined)
        {
          bt->accept (&visitor);

          *os << " **tmp = ACE_static_cast (";

          bt->accept (&visitor);

          *os << "**, src);" << be_nl
              << "return *tmp;";
        }
      else
        {
          *os << "return tao_" << pt->flat_name () << "_upcast (src);";
        }

      *os << be_uidt_nl
          << "}" << be_nl;
    }

  os->gen_endif ();

  // Generate #endif for AHETI.
  os->gen_endif_AHETI();

  return 0;
}
