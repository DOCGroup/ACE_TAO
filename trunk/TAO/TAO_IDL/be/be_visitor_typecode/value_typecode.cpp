// -*- C++ -*-

//=============================================================================
/**
 *  @file  value_typecode.cpp
 *
 *  $Id$
 *
 *  Structure TypeCode generation visitor.
 *
 *  @author  Ossama Othman <ossama@dre.vanderbilt.edu>
 */
//=============================================================================


#include <string>


TAO::be_visitor_value_typecode::be_visitor_value_typecode (
  be_visitor_context * ctx)
  : be_visitor_typecode_defn (ctx)
{
}

int
TAO::be_visitor_value_typecode::visit_valuetype (be_valuetype * node)
{
  TAO_OutStream & os = *this->ctx_->stream ();

  os << be_nl << be_nl
     << "// TAO_IDL - Generated from" << be_nl
     << "// " << __FILE__ << ":" << __LINE__ << be_nl << be_nl;

  size_t const count =
    node->data_members_count ();

  /*
  if (count == 1 &&
      count == node->nmembers ()  // Verify no operations.
      && node->n_inherits () == 0)
    {
      // Generate a value box TypeCode.  It is more compact than a
      // valuetype TypeCode.

      UTL_ScopeActiveIterator si (node, UTL_Scope::IK_decls);

      AST_Decl * const d = si.item ();

      ACE_ASSERT (d);

      AST_Field * const field = AST_Field::narrow_from_decl (d);

      ACE_ASSERT (field);

      be_type * const member_type =
        be_type::narrow_from_decl (field->field_type ());

      // Generate the TypeCode instantiation.
      os
        << "static TAO::TypeCode::Value_Box<char const *," << be_nl
        << "                                TAO::Null_RefCount_Policy> const"
        << be_idt_nl
        << "_tao_tc_" << node->flat_name () << " (" << be_idt_nl
        << "\"" << node->repoID () << "\"," << be_nl
        << "\"" << node->original_local_name () << "\"," << be_nl
        << "&"  << member_type->tc_name () << ");" << be_uidt_nl
        << be_uidt_nl;
    }
  else
  */
    {
      std::string const fields_name (std::string ("_tao_fields_")
                                     + node->flat_name ());

      // Generate array containing value field characteristics.
      os << "static TAO::TypeCode::Value_Field<char const *> ";

      if (count == 0)
        {
          os << " * const " << fields_name.c_str () << " = 0;" << be_nl;
        }
      else
        {
          os << fields_name.c_str () << "[] =" << be_idt_nl
             << "{" << be_idt_nl;

          if (this->visit_members (node) != 0)
            return -1;

          os << be_uidt_nl
             << "};" << be_uidt_nl;
        }

      // Generate the TypeCode instantiation.

      os << be_nl
         << "static TAO::TypeCode::Value<char const *," << be_nl
         << "                            TAO::TypeCode::Value_Field<char const *> const *," << be_nl
         << "                            CORBA::tk_"
         << (dynamic_cast<be_eventtype *> (node) ? "event" : "value") << "," << be_nl
         << "                            TAO::Null_RefCount_Policy> const"
         << be_idt_nl
         << "_tao_tc_" << node->flat_name () << " (" << be_idt_nl
         << "\"" << node->repoID () << "\"," << be_nl
         << "\"" << node->original_local_name () << "\"," << be_nl;

      // ValueModifier
      //
      // TAO doesn't support CUSTOM or TRUNCATABLE valuetypes.  Go
      // with VM_NONE or VM_ABSTRACT.
      os << "CORBA::"
         << (node->is_abstract () ? "VM_ABSTRACT" : "VM_NONE") << "," << be_nl;

      // Concrete base type.
      AST_ValueType * const concrete_base =
        node->inherits_concrete ();

      if (concrete_base)
        {
          be_type * const base_type =
            be_type::narrow_from_decl (concrete_base);

          ACE_ASSERT (base_type);

          os << "&" << base_type->tc_name () << "," << be_nl;
        }
      else
        {
          // No concrete base.
          os << "&CORBA::tk_null," << be_nl;
        }

      // Fields
      os << "_tao_fields_" << node->flat_name () << "," << be_nl
         << count << ");" << be_uidt_nl
         << be_uidt_nl;
    }

  return
    this->gen_typecode_ptr (be_type::narrow_from_decl (node));
}

int
TAO::be_visitor_value_typecode::visit_members (be_valuetype * node)
{
  TAO_OutStream & os = *this->ctx_->stream ();

  size_t const count =
    node->data_members_count ();

  size_t i = 0;

  for (UTL_ScopeActiveIterator si (node, UTL_Scope::IK_decls);
       !si.is_done ();
       si.next(), ++i)
    {
      AST_Decl * const d = si.item ();

      if (!d)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_value_typecode::visit_members - "
                             "bad node in this scope\n"), 0);
        }

      AST_Field * const field = AST_Field::narrow_from_decl (d);

      if (!field)
        {
          continue;
        }

      be_decl * const member_decl =
        be_decl::narrow_from_decl (field);

      be_type * const member_type =
        be_type::narrow_from_decl (field->field_type ());

      os << "{ "
         << "\"" << member_decl->original_local_name () << "\", "
         << "&"  << member_type->tc_name () << ", ";

      AST_Field::Visibility const vis = field->visibility ();

      switch (vis)
        {
        case AST_Field::vis_PUBLIC:
          os << "CORBA::PUBLIC_MEMBER";
          break;

        case AST_Field::vis_PRIVATE:
          os << "CORBA::PRIVATE_MEMBER";
          break;

        default:
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_value_typecode::visit_members - "
                             "Unknown valuetype member visibility.\n"),
                            -1);
        };

      os << " }";


      if (i < count - 1)
        os << ",";

      os << be_nl;
    }

  return 0;
}
