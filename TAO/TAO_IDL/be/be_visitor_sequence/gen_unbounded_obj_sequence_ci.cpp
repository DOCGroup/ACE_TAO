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
// ============================================================================

#include "be.h"
#include "be_visitor_sequence.h"

ACE_RCSID (be_visitor_sequence, 
           gen_unbounded_obj_sequence_ci, 
           "$Id$")

int
be_visitor_sequence_ci::gen_unbounded_obj_sequence (be_sequence *node)
{
  TAO_OutStream *os = this->ctx_->stream ();

  // Retrieve the base type since we may need to do some code
  // generation for the base type.
  be_type *bt = be_type::narrow_from_decl (node->base_type ());

  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_sequence_ci::"
                         "visit_sequence - "
                         "Bad element type\n"),
                        -1);
    }

  // Generate the class name;
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

  const char *class_name = node->instance_name ();

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
  ctx.state (TAO_CodeGen::TAO_SEQUENCE_BASE_CI);
  be_visitor_sequence_base visitor (&ctx);

  *os << be_nl << "// TAO_IDL - Generated from "
      << __FILE__ << ":" << __LINE__ << be_nl << be_nl;

  os->gen_ifdef_AHETI();
  os->gen_ifdef_macro (class_name);
  os->indent ();

  // allocbuf.
  *os << "ACE_INLINE ";

  // The accept is here the first time used and if an
  // error occurs, it will occur here. Later no check
  // for errors will be done.
  if (bt->accept (&visitor) == -1)
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

  bt->accept (&visitor);

  *os <<" **buf = 0;" << be_nl
      << be_nl
      << "ACE_NEW_RETURN (buf, "
      ;
  bt->accept (&visitor);

  *os << "*[nelems], 0);" << be_nl
      << be_nl
      << "for (CORBA::ULong i = 0; i < nelems; i++)" << be_idt_nl
      << "{" << be_idt_nl
      << "buf[i] = ";

  int is_valuetype = 0;
  {
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
  }

  if (is_valuetype)
    {
      *os << "0;";
    }
  else if (bt_is_defined)
    {
      bt->accept (&visitor);

      *os << "::_nil ();";
    }
  else
    {
      *os << "tao_" << pt->flat_name () << "_nil ();";
    }

  *os << be_uidt_nl
      << "}" << be_uidt_nl << be_nl
      << "return buf;" << be_uidt_nl
      << "}" << be_nl << be_nl;

  // Freebuf.
  *os << "ACE_INLINE void " << be_nl
      << full_class_name << "::freebuf (";

  bt->accept (&visitor);

  *os << " **buffer)" << be_nl
      << "{" << be_idt_nl
      << "if (buffer == 0)" << be_idt_nl
      << "return;" << be_uidt_nl
      << "delete[] buffer;" << be_uidt_nl
      << "}" << be_nl
      << be_nl;

  // Constructor.
  *os << "ACE_INLINE" << be_nl
      << full_class_name << "::" << class_name << " (void)" << be_nl
      << "{" << be_nl
      << "}" << be_nl
      << be_nl;

  // Constructor.
  *os << "ACE_INLINE" << be_nl
      << full_class_name << "::" << class_name
      << " (CORBA::ULong maximum)" << be_idt_nl
      << ": TAO_Unbounded_Base_Sequence (maximum, "
      << class_name << "::allocbuf (maximum))" << be_uidt_nl
      << "{" << be_nl
      << "}" << be_nl
      << be_nl;

  // Constructor.
  *os << "ACE_INLINE" << be_nl
      << full_class_name << "::" << class_name
      << " (CORBA::ULong maximum," << be_idt_nl
      << "CORBA::ULong length," << be_nl;

  bt->accept (&visitor);

  *os <<"* *value," << be_nl
      << "CORBA::Boolean release)" << be_uidt_nl
      << ": TAO_Unbounded_Base_Sequence (maximum, length, value, release)"
      << be_nl
      << "{" << be_nl
      << "}" << be_nl
      << be_nl;

  // Constructor.
  *os << "ACE_INLINE" << be_nl
      << full_class_name << "::" << class_name << "(const "
      << class_name << " &rhs)" << be_idt_nl
      << ": TAO_Unbounded_Base_Sequence (rhs)" << be_uidt_nl
      << "{" << be_idt_nl
      << "if (rhs.buffer_ != 0)" << be_nl
      << "{" << be_idt_nl;

  bt->accept (&visitor);

  *os <<" **tmp1 = " << class_name << "::allocbuf (this->maximum_);"
      << be_nl;

  bt->accept (&visitor);

  *os <<" ** const tmp2 = ACE_reinterpret_cast (";

  bt->accept (&visitor);

  *os << " ** ACE_CAST_CONST, rhs.buffer_);" << be_nl
      << be_nl
      << "for (CORBA::ULong i = 0; i < rhs.length_; ++i)" << be_idt_nl
      << "{" << be_idt_nl;

  if (is_valuetype)
    {
      *os << "if (tmp2[i] != 0)" << be_idt_nl
          << "tmp2[i]->_add_ref ();" << be_uidt_nl
          << "tmp1[i] = tmp2 [i];";
    }
  else
    {
      *os << "tmp1[i] = ";

      if (bt_is_defined)
        {
          bt->accept (&visitor);

          *os << "::_duplicate (tmp2[i]);";
        }
      else
        {
          *os << "tao_" << pt->flat_name () << "_duplicate (tmp2[i]);";
        }
    }

  *os << be_uidt_nl
      << "}" << be_uidt_nl << be_nl
      << "this->buffer_ = tmp1;" << be_uidt_nl
      << "}" << be_nl
      << "else" << be_nl
      << "{" << be_idt_nl
      << "this->buffer_ = 0;" << be_uidt_nl
      << "}" << be_uidt_nl
      << "}" << be_nl
      << be_nl;

  // Operator =.
  *os << "ACE_INLINE " << full_class_name << " &" << be_nl
      << full_class_name << "::operator= (const "
      << class_name << " &rhs)" << be_nl
      << "{" << be_idt_nl
      << "if (this == &rhs)" << be_idt_nl
      << "return *this;" << be_uidt_nl
      << be_nl
      << "if (this->release_)" << be_nl
      << "{" << be_idt_nl;

  bt->accept (&visitor);

  *os <<" **tmp = ACE_reinterpret_cast (";

  bt->accept (&visitor);

  *os << " **, this->buffer_);" << be_nl
      << be_nl
      << "for (CORBA::ULong i = 0; i < this->length_; ++i)" << be_nl
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
      << "}" << be_nl
      << "if (this->maximum_ < rhs.maximum_)" << be_nl
      << "{" << be_idt_nl
      << class_name << "::freebuf (tmp);" << be_nl
      << "this->buffer_ = " << class_name
      << "::allocbuf (rhs.maximum_);" << be_uidt_nl
      << "}" << be_uidt_nl
      << "}" << be_nl
      << "else" << be_idt_nl
      << "this->buffer_ = " << class_name
      << "::allocbuf (rhs.maximum_);" << be_uidt_nl
      << be_nl
      << "TAO_Unbounded_Base_Sequence::operator= (rhs);" << be_nl
      << be_nl;

  bt->accept (&visitor);

  *os <<" **tmp1 = ACE_reinterpret_cast (";

  bt->accept (&visitor);

  *os << " **, this->buffer_);" << be_nl;

  bt->accept (&visitor);

  *os <<" ** const tmp2 = ACE_reinterpret_cast (";

  bt->accept (&visitor);

  *os << " ** ACE_CAST_CONST, rhs.buffer_);" << be_nl
      << be_nl
      << "for (CORBA::ULong i = 0; i < rhs.length_; ++i)" << be_idt_nl
      << "{" << be_idt_nl;

  if (is_valuetype)
    {
      *os << "if (tmp2[i] != 0)" << be_idt_nl
          << "tmp2[i]->_add_ref ();" << be_uidt_nl
          << "tmp1[i] = tmp2 [i];";
    }
  else
    {
      *os << "tmp1[i] = ";

      if (bt_is_defined)
        {
          bt->accept (&visitor);

          *os << "::_duplicate (tmp2[i]);";
        }
      else
        {
          *os << "tao_" << pt->flat_name () << "_duplicate (tmp2[i]);";
        }
    }

  *os << be_uidt_nl
      << "}" << be_uidt_nl << be_nl
      << "return *this;" << be_uidt_nl
      << "}" << be_nl
      << be_nl;

  // Operator[].
  be_predefined_type *prim = be_predefined_type::narrow_from_decl (pt);
  int is_pseudo_object =
    pt->node_type () == AST_Decl::NT_pre_defined
    && prim && prim->pt () == AST_PredefinedType::PT_pseudo
    && ACE_OS::strcmp (prim->local_name ()->get_string (),
                       "Object") != 0;
  if (is_pseudo_object)
    {
      *os << "ACE_INLINE TAO_Pseudo_Object_Manager<";
    }
  else
    {
      if (is_valuetype)
        {
          *os << "ACE_INLINE TAO_Valuetype_Manager<";
        }
      else
        {
          *os << "ACE_INLINE TAO_Object_Manager<";
        }
    }

  *os << bt->name () << ","
      << bt->name () << "_var>" << be_nl
      << full_class_name << "::operator[] (CORBA::ULong index) const"
      << be_nl
      << "// read-write accessor" << be_nl
      << "{" << be_idt_nl
      << "ACE_ASSERT (index < this->maximum_);" << be_nl;

  bt->accept (&visitor);

  *os <<" ** const tmp = ACE_reinterpret_cast (";

  bt->accept (&visitor);

  *os << " ** ACE_CAST_CONST, this->buffer_);" << be_nl;

  if (is_pseudo_object)
    {
      *os << "return TAO_Pseudo_Object_Manager<";
    }
  else
    {
      if (is_valuetype)
        {
          *os << "return TAO_Valuetype_Manager<";
        }
      else
        {
          *os << "return TAO_Object_Manager<";
        }
    }

  *os << bt->name () << ","
      << bt->name () << "_var>"
      << " (tmp + index, this->release_);" << be_uidt_nl
      << "}" << be_nl
      << be_nl;

  // get_buffer
  *os << "ACE_INLINE ";

  bt->accept (&visitor);

  *os << "* *" << be_nl
      << full_class_name << "::get_buffer (CORBA::Boolean orphan)" << be_nl
      << "{" << be_idt_nl;

  bt->accept (&visitor);

  *os << " **result = 0;" << be_nl
      << "if (orphan == 0)" << be_nl
      << "{" << be_idt_nl
      << "// We retain ownership." << be_nl
      << "if (this->buffer_ == 0)" << be_nl
      << "{" << be_idt_nl
      << "result = " << class_name << "::allocbuf (this->maximum_);"
      << be_nl
      << "this->buffer_ = result;" << be_nl
      << "this->release_ = 1;" << be_uidt_nl
      << "}" << be_nl
      << "else" << be_nl
      << "{" << be_idt_nl
      << "result = ACE_reinterpret_cast (";

  bt->accept (&visitor);

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

  bt->accept (&visitor);

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

  // get_buffer.
  *os << "ACE_INLINE ";
  *os << "const ";

  bt->accept (&visitor);

  *os << "* *" << be_nl
      << full_class_name << "::get_buffer (void) const" << be_nl
      << "{" << be_idt_nl
      << "return ACE_reinterpret_cast(const ";

  bt->accept (&visitor);

  *os << " ** ACE_CAST_CONST, this->buffer_);" << be_uidt_nl
      << "}" << be_nl
      << be_nl;

  os->gen_endif ();

  // Generate #endif for AHETI.
  os->gen_endif_AHETI();

  return 0;
}
