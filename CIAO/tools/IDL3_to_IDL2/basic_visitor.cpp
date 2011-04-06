/* -*- c++ -*- */
// $Id$

#include "basic_visitor.h"
#include "identifier_helper.h"
#include "be_sunsoft.h"
#include "be_extern.h"

#include "ast_argument.h"
#include "ast_array.h"
#include "ast_attribute.h"
#include "ast_enum.h"
#include "ast_enum_val.h"
#include "ast_exception.h"
#include "ast_factory.h"
#include "ast_field.h"
#include "ast_operation.h"
#include "ast_root.h"
#include "ast_sequence.h"
#include "ast_string.h"
#include "ast_structure_fwd.h"
#include "ast_typedef.h"
#include "ast_union.h"
#include "ast_union_branch.h"
#include "ast_union_fwd.h"
#include "ast_union_label.h"
#include "ast_valuebox.h"
#include "ast_valuetype.h"
#include "ast_valuetype_fwd.h"
#include "ast_component.h"
#include "ast_native.h"

#include "utl_exceptlist.h"
#include "utl_idlist.h"
#include "utl_identifier.h"

#include "nr_extern.h"

basic_visitor::basic_visitor (void)
  : os (0),
    disc_type_ (0)
{
}

basic_visitor::~basic_visitor (void)
{
  delete this->os;
  this->os = 0;
}

int
basic_visitor::visit_decl (AST_Decl *)
{
  return 0;
}

int
basic_visitor::visit_scope (UTL_Scope *node)
{
  for (UTL_ScopeActiveIterator si (node, UTL_Scope::IK_decls);
       !si.is_done ();
       si.next ())
    {
      AST_Decl *d = si.item ();

      if (this->scope_skip_type (d))
        {
          continue;
        }

      AST_Decl::NodeType nt = d->node_type ();

      /// We want to skip the uses_xxxConnection structs added by uses
      /// multiple ports. We have no CCM preprocessing visitor as
      /// with tao_idl, so the get_connections() operation gets
      /// created before we see the xxxXonnections return type
      /// it need. So we just generate the struct and sequence
      /// typedef literally as well, just before generating the
      /// operation.
      if (AST_Component::narrow_from_scope (node) != 0
          && (nt == AST_Decl::NT_struct
              || nt == AST_Decl::NT_sequence
              || nt == AST_Decl::NT_typedef))
        {
          continue;
        }

      if (d->ast_accept (this) != 0)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "idl3_to_idl2_visitor::visit_scope - "
                             "codegen for scope failed\n"),
                            -1);
        }
    }

  return 0;
}

int
basic_visitor::visit_type (AST_Type *)
{
  return 0;
}

int
basic_visitor::visit_predefined_type (AST_PredefinedType *)
{
  return 0;
}

int
basic_visitor::visit_interface_fwd (AST_InterfaceFwd *node)
{
  if (node->imported ())
    {
      return 0;
    }

  *os << be_nl << be_nl;

  if (node->is_local ())
    {
      *os << "local ";
    }

  if (node->is_abstract ())
    {
      *os << "abstract ";
    }

  *os << "interface "
      << IdentifierHelper::try_escape (node->original_local_name ()).c_str ()
      << ";";

  return 0;
}

int
basic_visitor::visit_valuebox (AST_ValueBox *node)
{
  if (node->imported ())
    {
      return 0;
    }

  *os << be_nl << be_nl
      << "valuetype "
      << IdentifierHelper::try_escape (node->original_local_name ()).c_str ();

  AST_Type *bt = node->boxed_type ();

  // Keep output statements separate because of side effects.

  if (bt->node_type () == AST_Decl::NT_array)
    {
      this->gen_anonymous_array (bt, node);
    }
  else
    {
      *os << this->type_name (bt);
    }

  *os << ";";

  this->check_id_and_version (node);

  return 0;
}

int
basic_visitor::visit_valuetype (AST_ValueType *node)
{
  if (node->imported ())
    {
      return 0;
    }

  *os << be_nl << be_nl;

  if (node->is_abstract ())
    {
      *os << "abstract ";
    }

  if (node->custom ())
    {
      *os << "custom ";
    }

  *os << "valuetype "
      << IdentifierHelper::try_escape (node->original_local_name ()).c_str ();

  AST_Decl::NodeType nt = node->node_type ();
  AST_Type **parents = node->inherits ();
  long ninherits = node->n_inherits ();

  long i = 0;
  for (i = 0; i < ninherits; ++i)
    {
      if (i == 0)
        {
          *os << " : ";
        }
      else
        {
          *os << ", ";
        }

      *os << IdentifierHelper::orig_sn (parents[i]->name ()).c_str ();
    }

  if (nt == AST_Decl::NT_eventtype)
    {
      *os << (ninherits == 0 ? " : " : ", ")
          << "Components::EventBase";
    }

  AST_Type **supports = node->supports ();

  for (i = 0; i < node->n_supports (); ++i)
    {
      if (i == 0)
        {
          *os << " supports ";
        }
      else
        {
          *os << ", ";
        }

      if (i == 0 && node->supports_concrete () != 0)
        {
          supports[i] = node->supports_concrete ();
        }

      AST_Type *supported = supports[i];

      *os << IdentifierHelper::orig_sn (supported->name ()).c_str ();
    }

  *os << be_nl
      << "{" << be_idt;

  this->check_id_and_version (node);
  this->check_prefix (node);

  if (this->visit_scope (node) != 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                          "idl3_to_idl2_visitor::visit_valuetype - "
                          "codegen for scope failed\n"),
                        -1);
    }

  *os << be_uidt_nl
      << "};";

  return 0;
}

int
basic_visitor::visit_valuetype_fwd (AST_ValueTypeFwd *node)
{
  if (node->imported ())
    {
      return 0;
    }

  *os << be_nl << be_nl;

  if (node->is_abstract ())
    {
      *os << "abstract ";
    }

  (void) node->node_type ();

  *os << "valuetype "
      << IdentifierHelper::try_escape (node->original_local_name ()).c_str ()
      << ";";

  return 0;
}

int
basic_visitor::visit_template_module (AST_Template_Module *)
{
  return 0;
}

int
basic_visitor::visit_template_module_inst (AST_Template_Module_Inst *)
{
  return 0;
}

int
basic_visitor::visit_template_module_ref (AST_Template_Module_Ref *)
{
  return 0;
}

int
basic_visitor::visit_porttype (AST_PortType *)
{
  return 0;
}

int
basic_visitor::visit_provides (AST_Provides *)
{
  return 0;
}

int
basic_visitor::visit_uses (AST_Uses *)
{
  return 0;
}

int
basic_visitor::visit_publishes (AST_Publishes *)
{
  return 0;
}

int
basic_visitor::visit_emits (AST_Emits *)
{
  return 0;
}

int
basic_visitor::visit_consumes (AST_Consumes *)
{
  return 0;
}

int
basic_visitor::visit_factory  (AST_Factory *node)
{
  *os << be_nl;

  *os << "factory "
      << IdentifierHelper::try_escape (node->original_local_name ()).c_str ()
      << " (";

  this->gen_params (node, node->argument_count ());

  *os << ")";

  this->gen_exception_list (node->exceptions ());

  *os << ";";

  this->check_id_and_version (node);

  return 0;
}

int
basic_visitor::visit_structure (AST_Structure *node)
{
  if (node->imported ())
    {
      return 0;
    }

  *os << be_nl << be_nl;

  *os << "struct "
      << IdentifierHelper::try_escape (node->original_local_name ()).c_str ()
      << be_nl
      << "{" << be_idt;

  this->check_id_and_version (node);

  if (this->visit_scope (node) != 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                          "idl3_to_idl2_visitor::visit_structure - "
                          "codegen for scope failed\n"),
                        -1);
    }

  *os << be_uidt_nl
      << "};";

  return 0;
}

int
basic_visitor::visit_structure_fwd (AST_StructureFwd *node)
{
  if (node->imported ())
    {
      return 0;
    }

  *os << be_nl << be_nl;

  *os << "struct "
      << IdentifierHelper::try_escape (node->original_local_name ()).c_str ()
      << ";";

  return 0;
}

int
basic_visitor::visit_exception (AST_Exception *node)
{
  if (node->imported ())
    {
      return 0;
    }

  *os << be_nl << be_nl;

  *os << "exception "
      << IdentifierHelper::try_escape (node->original_local_name ()).c_str ()
      << be_nl
      << "{" << be_idt;

  this->check_id_and_version (node);

  if (this->visit_scope (node) != 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                          "idl3_to_idl2_visitor::visit_exception - "
                          "codegen for scope failed\n"),
                        -1);
    }

  *os << be_uidt_nl
      << "};";

  return 0;
}

int
basic_visitor::visit_expression (AST_Expression *)
{
  return 0;
}

int
basic_visitor::visit_enum (AST_Enum *node)
{
  if (node->imported ())
    {
      return 0;
    }

  *os << be_nl << be_nl;

  *os << "enum "
      << IdentifierHelper::try_escape (node->original_local_name ()).c_str ()
      << be_nl
      << "{" << be_idt;

  for (UTL_ScopeActiveIterator i (node, UTL_Scope::IK_decls);
       !i.is_done ();)
    {
      *os << be_nl;

      AST_EnumVal *ev = AST_EnumVal::narrow_from_decl (i.item ());

      *os << IdentifierHelper::try_escape (ev->original_local_name ()).c_str ();

      // Advance here so the check below will work.
      i.next ();

      if (!i.is_done ())
        {
          *os << ",";
        }
    }

  *os << be_uidt_nl
      << "};";

  this->check_id_and_version (node);

  return 0;
}

int
basic_visitor::visit_operation (AST_Operation *node)
{
  this->gen_operation (node);
  this->check_id_and_version (node);

  return 0;
}

int
basic_visitor::visit_field (AST_Field *node)
{
  AST_Field::Visibility v = node->visibility ();

  *os << be_nl
      << (v == AST_Field::vis_PUBLIC
          ? "public "
          : (v == AST_Field::vis_PRIVATE ? "private " : ""));

  AST_Type *ft = node->field_type ();

  if (ft->node_type () == AST_Decl::NT_array)
    {
      this->gen_anonymous_array (ft, node);
    }
  else
    {
      // Keep these statements separate because of side effects.
      *os << this->type_name (ft);
      *os << " "
          << IdentifierHelper::try_escape (node->original_local_name ()).c_str ();
    }

  *os << ";";

  return 0;
}

int
basic_visitor::visit_argument (AST_Argument *node)
{
  *os << be_nl;

  switch (node->direction ())
    {
      case AST_Argument::dir_IN:
        *os << "in ";
        break;
      case AST_Argument::dir_INOUT:
        *os << "inout ";
        break;
      case AST_Argument::dir_OUT:
        *os << "out ";
        break;
      default:
        return -1;
    }

  *os << this->type_name (node->field_type ())
      << " "
      << IdentifierHelper::try_escape (node->original_local_name ()).c_str ();

  return 0;
}

int
basic_visitor::visit_attribute (AST_Attribute *node)
{
  this->gen_attribute (node);
  this->check_id_and_version (node);

  return 0;
}

int
basic_visitor::visit_union (AST_Union *node)
{
  if (node->imported ())
    {
      return 0;
    }

  this->disc_type_ = node->disc_type ()->unaliased_type ();

  *os << be_nl << be_nl
      << "union "
      << IdentifierHelper::try_escape (node->original_local_name ()).c_str ()
      << " switch (";

  *os << this->type_name (node->disc_type ())
      << ")" << be_nl
      << "{" << be_idt;

  this->check_id_and_version (node);

  if (this->visit_scope (node) != 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                          "idl3_to_idl2_visitor::visit_union - "
                          "codegen for scope failed\n"),
                        -1);
    }

  *os << be_uidt_nl
      << "};";

  return 0;
}

int
basic_visitor::visit_union_fwd (AST_UnionFwd *node)
{
  if (node->imported ())
    {
      return 0;
    }

  *os << be_nl << be_nl
      << "union "
      << IdentifierHelper::try_escape (node->original_local_name ()).c_str ()
      << ";";

  return 0;
}

int
basic_visitor::visit_union_branch (AST_UnionBranch *node)
{
  for (unsigned long i = 0; i < node->label_list_length (); ++i)
    {
      if (this->visit_union_label (node->label (i)) != 0)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                              "idl3_to_idl2_visitor::visit_union_branch - "
                              "codegen for label failed\n"),
                            -1);
        }
    }

  AST_Type *ft = node->field_type ();

  if (ft->node_type () == AST_Decl::NT_array)
    {
      this->gen_anonymous_array (ft, node);
    }
  else
    {
      *os << this->type_name (ft);
      *os << " "
          << IdentifierHelper::try_escape (node->original_local_name ()).c_str ();
    }

  *os << ";";

  return 0;
}

int
basic_visitor::visit_union_label (AST_UnionLabel *node)
{
  *os << be_nl;

  if (node->label_kind () == AST_UnionLabel::UL_default)
    {
      *os << "default: ";
    }
  else
    {
      *os << "case ";
      this->gen_label_value (node);
      *os << ": ";
    }

  return 0;
}

int
basic_visitor::visit_constant (AST_Constant *node)
{
  if (node->imported ())
    {
      return 0;
    }

  *os << be_nl << be_nl;

  *os << "const ";

  switch (node->et ())
    {
      case AST_Expression::EV_short:
        *os << "short";
        break;
      case AST_Expression::EV_ushort:
        *os << "unsigned short";
        break;
      case AST_Expression::EV_long:
        *os << "long";
        break;
      case AST_Expression::EV_ulong:
        *os << "unsigned long";
        break;
      case AST_Expression::EV_longlong:
        *os << "long long";
        break;
      case AST_Expression::EV_ulonglong:
        *os << "unsigned long long";
        break;
      case AST_Expression::EV_char:
        *os << "char";
        break;
      case AST_Expression::EV_wchar:
        *os << "wchar";
        break;
      case AST_Expression::EV_bool:
        *os << "boolean";
        break;
      case AST_Expression::EV_octet:
        *os << "octet";
        break;
      case AST_Expression::EV_float:
        *os << "float";
        break;
      case AST_Expression::EV_double:
        *os << "double";
        break;
      case AST_Expression::EV_longdouble:
        *os << "long double";
        break;
      case AST_Expression::EV_string:
        *os << "string";
        break;
      case AST_Expression::EV_wstring:
        *os << "wstring";
        break;
      case AST_Expression::EV_enum:
        *os << IdentifierHelper::orig_sn (node->enum_full_name ()).c_str ();
        break;
      default:
        break;
    }

  *os << " "
      << IdentifierHelper::try_escape (node->original_local_name ()).c_str ()
      << " = " << node->constant_value () << ";";

  this->check_id_and_version (node);

  return 0;
}

int
basic_visitor::visit_enum_val (AST_EnumVal *)
{
  return 0;
}

int
basic_visitor::visit_array (AST_Array *node)
{
  *os << IdentifierHelper::orig_sn (node->base_type ()->name ()).c_str ();

  for (unsigned long i = 0; i < node->n_dims (); ++i)
    {
      *os << "[" << node->dims ()[i] << "]";
    }

  return 0;
}

int
basic_visitor::visit_sequence (AST_Sequence *node)
{
  // Keep output statements separate because of side effects.
  *os << "sequence< ";
  *os << this->type_name (node->base_type ());

  if (!node->unbounded ())
    {
      *os << ", " << node->max_size ()->ev ()->u.ulval;
    }

  *os << "> ";

  return 0;
}

int
basic_visitor::visit_string (AST_String *node)
{
  *os << (node->width () > 1 ? "w" : "") << "string";

  unsigned long bound = node->max_size ()->ev ()->u.ulval;

  if (bound > 0)
    {
      *os << "<" << bound << ">";
    }

  return 0;
}

int
basic_visitor::visit_typedef (AST_Typedef *node)
{
  if (node->imported ())
    {
      return 0;
    }

  *os << be_nl << be_nl
      << "typedef ";

  AST_Type *bt = node->base_type ();

  // Keep output statements separate because of side effects.

  if (bt->node_type () == AST_Decl::NT_array)
    {
      this->gen_anonymous_array (bt, node);
    }
  else
    {
      *os << this->type_name (bt);
      *os << " "
          << IdentifierHelper::try_escape (node->original_local_name ()).c_str ();
   }

  *os << ";";

  this->check_id_and_version (node);

  return 0;
}

int
basic_visitor::visit_native (AST_Native *node)
{
  if (node->imported ())
    {
      return 0;
    }

  *os << be_nl << be_nl;

  *os << "native "
      << IdentifierHelper::try_escape (node->original_local_name ()).c_str ()
      << ";";

  return 0;
}

int
basic_visitor::visit_param_holder (AST_Param_Holder *)
{
  return 0;
}

//========================================================

void
basic_visitor::check_prefix (AST_Decl *d)
{
  if (d->typeid_set ())
    {
      return;
    }

  const char *the_prefix = d->prefix ();
  AST_Decl *p = ScopeAsDecl (d->defined_in ());

  if (ACE_OS::strcmp (the_prefix, p->prefix ()) != 0)
    {
      *os << be_nl
          << "typeprefix "
          << IdentifierHelper::try_escape (d->original_local_name ()).c_str ()
          << " \"" << the_prefix << "\";";
    }
}

void
basic_visitor::check_id_and_version (AST_Decl *d)
{
  if (d->typeid_set ())
    {
      *os << be_nl
          << "typeid "
          << IdentifierHelper::try_escape (d->original_local_name ()).c_str ()
          << " \"" << d->repoID () << "\";";

      return;
    }

  const char *the_version = d->version ();
  AST_Decl *p = ScopeAsDecl (d->defined_in ());

  if (ACE_OS::strcmp (the_version, p->version ()) != 0)
    {
      *os << "\n"
          << "#pragma version "
          << IdentifierHelper::try_escape (d->original_local_name ()).c_str ()
          << " " << the_version;
    }
}

const char *
basic_visitor::type_name (AST_Type *t)
{
  AST_PredefinedType *pdt = 0;

  switch (t->node_type ())
    {
      case AST_Decl::NT_wstring:
      case AST_Decl::NT_string:
      case AST_Decl::NT_sequence:
        // This causes side effects so output statements
        // sending us here should not be concatenated.
        (void) t->ast_accept (this);
        return "";
      case AST_Decl::NT_pre_defined:
        pdt = AST_PredefinedType::narrow_from_decl (t);

        switch (pdt->pt ())
          {
            case AST_PredefinedType::PT_pseudo:
              return t->full_name ();
            case AST_PredefinedType::PT_object:
              return "Object";
            case AST_PredefinedType::PT_any:
              return "any";
            case AST_PredefinedType::PT_long:
              return "long";
            case AST_PredefinedType::PT_ulong:
              return "unsigned long";
            case AST_PredefinedType::PT_longlong:
              return "long long";
            case AST_PredefinedType::PT_ulonglong:
              return "unsigned long long";
            case AST_PredefinedType::PT_short:
              return "short";
            case AST_PredefinedType::PT_ushort:
              return "unsigned short";
            case AST_PredefinedType::PT_float:
              return "float";
            case AST_PredefinedType::PT_double:
              return "double";
            case AST_PredefinedType::PT_longdouble:
              return "long double";
            case AST_PredefinedType::PT_char:
              return "char";
            case AST_PredefinedType::PT_wchar:
              return "wchar";
            case AST_PredefinedType::PT_boolean:
              return "boolean";
            case AST_PredefinedType::PT_octet:
              return "octet";
            case AST_PredefinedType::PT_void:
              return "void";
            default:
              break;
          }
      default:
        this->tmp_retval_ = IdentifierHelper::orig_sn (t->name ()).c_str ();
        return this->tmp_retval_.c_str ();
    }
}

void
basic_visitor::gen_anonymous_array (AST_Type *a,
                                    AST_Decl *wrapper)
{
  AST_Array *array = AST_Array::narrow_from_decl (a);
  AST_Type *bt = array->base_type ();

  *os << this->type_name (bt);
  *os << " "
      << IdentifierHelper::try_escape (wrapper->original_local_name ()).c_str ();

  for (unsigned long i = 0; i < array->n_dims (); ++i)
    {
      *os << "[" << array->dims ()[i]->ev ()->u.ulval << "]";
    }
}

void
basic_visitor::gen_params (UTL_Scope *s, int arg_count)
{
  if (arg_count > 0)
    {
      *os << be_idt;

      for (UTL_ScopeActiveIterator si (s, UTL_Scope::IK_decls);
           !si.is_done ();)
        {
          if (si.item ()->ast_accept (this) != 0)
            {
              ACE_ERROR ((LM_ERROR,
                          "idl3_to_idl2_visitor::gen_params - "
                          "codegen for parameters failed\n"));
            }

          si.next ();

          if (!si.is_done ())
            {
              *os << ",";
            }
        }

      *os << be_uidt;
    }
}

void
basic_visitor::gen_exception_list (UTL_ExceptList *exceptions,
                                   const char *prefix,
                                   bool closed)
{
  if (exceptions != 0 && exceptions->length () > 0)
    {
      *os << be_idt_nl
          << prefix << "raises (";

      for (UTL_ExceptlistActiveIterator ei (exceptions);
           !ei.is_done ();)
        {
          *os << IdentifierHelper::orig_sn (ei.item ()->name ()).c_str ();

          ei.next ();

          if (!ei.is_done () || !closed)
            {
              *os << ", ";
            }
        }

      if (closed)
        {
          *os << ")" << be_uidt;
        }
    }
}

void
basic_visitor::gen_operation (AST_Operation *node)
{
   *os << be_nl << be_nl;

   if (node->flags () == AST_Operation::OP_oneway)
     {
        *os << "oneway ";
     }

   *os << this->type_name (node->return_type ());

   *os << " "
       << IdentifierHelper::try_escape (node->original_local_name ()).c_str ()
       << " (";

   this->gen_params (node, node->argument_count ());

   *os << ")";

   this->gen_exception_list (node->exceptions ());

   *os << ";";
}

void
basic_visitor::gen_attribute (AST_Attribute *node)
{
  bool rd_only = node->readonly ();

  // Keep output statements separate because of side effects.
  // No need to check for anonymous array - anonymous types not
  // accepted by parser for attributes.
  *os << be_nl << be_nl
     << (rd_only ? "readonly " : "") << "attribute ";
  *os << this->type_name (node->field_type ());
  *os << " " << this->port_prefix_.c_str ()
     << IdentifierHelper::try_escape (node->original_local_name ()).c_str ();

  this->gen_exception_list (node->get_get_exceptions (),
                           rd_only ? "" : "get");

  this->gen_exception_list (node->get_set_exceptions (),
                           "set");

  *os << ";";
}

void
basic_visitor::gen_label_value (AST_UnionLabel *node)
{
  AST_Expression *val = node->label_val ();
  AST_Expression::AST_ExprValue *ev = val->ev ();

  if (this->disc_type_->node_type () == AST_Decl::NT_enum)
    {
      UTL_Scope *s = this->disc_type_->defined_in ();

      if (s == 0)
        {
          *os << IdentifierHelper::orig_sn (val->n ()).c_str ();
        }
      else
        {
          *os << IdentifierHelper::orig_sn (ScopeAsDecl (s)->name ()).c_str ()
              << "::";

          Identifier *id =
            IdentifierHelper::original_local_name (val->n ()->last_component ());

          *os << IdentifierHelper::try_escape (id).c_str ();

          id->destroy ();
          delete id;
          id = 0;
        }

      return;
    }

  switch (ev->et)
    {
      case AST_Expression::EV_short:
        *os << ev->u.sval;
        break;
      case AST_Expression::EV_ushort:
        *os << ev->u.usval;
        break;
      case AST_Expression::EV_long:
        *os << ev->u.lval;
        break;
      case AST_Expression::EV_ulong:
        *os << ev->u.ulval;
        break;
      case AST_Expression::EV_longlong:
#if ! defined (ACE_LACKS_LONGLONG_T)
        this->os->print ("%lld", ev->u.llval);
#endif /* ! defined (ACE_LACKS_LONGLONG_T) */
        break;
      case AST_Expression::EV_ulonglong:
#if ! defined (ACE_LACKS_LONGLONG_T)
        *os << "ACE_UINT64_LITERAL (";
        this->os->print (ACE_UINT64_FORMAT_SPECIFIER_ASCII, ev->u.ullval);
        *os << ")";
#endif /* ! defined (ACE_LACKS_LONGLONG_T) */
        break;
      case AST_Expression::EV_char:
        *os << ev->u.cval;
        break;
      case AST_Expression::EV_wchar:
        *os << ev->u.wcval;
        break;
      case AST_Expression::EV_bool:
        *os << (ev->u.bval ? "TRUE" : "FALSE");
        break;
      case AST_Expression::EV_enum:
        *os << IdentifierHelper::orig_sn (val->n ()).c_str ();
        break;
      default:
        break;
    }
}

bool
basic_visitor::scope_skip_type (AST_Decl *d)
{
  return (d->node_type () == AST_Decl::NT_pre_defined);
}

bool
basic_visitor::can_skip_module (AST_Module *m)
{
  for (UTL_ScopeActiveIterator si (m, UTL_Scope::IK_decls);
       !si.is_done ();
       si.next ())
    {
      AST_Decl *d = si.item ();
      AST_Decl::NodeType nt = d->node_type ();

      switch (nt)
        {
          case AST_Decl::NT_interface:
          case AST_Decl::NT_interface_fwd:
          case AST_Decl::NT_component:
          case AST_Decl::NT_component_fwd:
          case AST_Decl::NT_eventtype:
          case AST_Decl::NT_eventtype_fwd:
          case AST_Decl::NT_home:
            if (d->is_abstract () || d->is_local ())
              {
                break;
              }

            return false;
          case AST_Decl::NT_module:
            if (!this->can_skip_module (AST_Module::narrow_from_decl (d)))
              {
                return false;
              }

            break;
          default:
            break;
        }
    }

  return true;
}

bool
basic_visitor::match_excluded_file (const char *raw_filename)
{
  ACE_CString::size_type p = 0;

  // If this included IDL file matches one of the 'excluded' files,
  // generate the include without tacking on the suffix.
  while (p != ACE_CString::npos)
    {
      ACE_CString::size_type cursor = p;
      p = be_global->excluded_filenames ().find (' ', cursor);

      ACE_CString one_filename =
        be_global->excluded_filenames ().substr (cursor, p - cursor);

      if (one_filename == raw_filename)
        {
          return true;
        }

      // Skip the whitespace.
      if (p != ACE_CString::npos)
        {
          while (be_global->excluded_filenames ()[p] == ' ')
            {
              p++;
            }
        }
    }

  return false;
}
