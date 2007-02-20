/* -*- c++ -*- */
// $Id$

#include "idl3_to_idl2_visitor.h"
#include "be_sunsoft.h"
#include "be_extern.h"

#include "ast_argument.h"
#include "ast_array.h"
#include "ast_attribute.h"
#include "ast_component_fwd.h"
#include "ast_enum.h"
#include "ast_enum_val.h"
#include "ast_eventtype.h"
#include "ast_eventtype_fwd.h"
#include "ast_exception.h"
#include "ast_factory.h"
#include "ast_field.h"
#include "ast_home.h"
#include "ast_operation.h"
#include "ast_root.h"
#include "ast_sequence.h"
#include "ast_string.h"
#include "ast_structure_fwd.h"
#include "ast_union.h"
#include "ast_union_branch.h"
#include "ast_union_fwd.h"
#include "ast_union_label.h"
#include "ast_valuebox.h"
#include "ast_valuetype_fwd.h"
#include "ast_native.h"
#include "utl_exceptlist.h"
#include "utl_identifier.h"
#include "utl_idlist.h"
#include "utl_string.h"
#include "global_extern.h"
#include "nr_extern.h"
#include "ace/OS_NS_stdio.h"

idl3_to_idl2_visitor::idl3_to_idl2_visitor (void)
  : os (0),
    disc_type_ (0)
{
}

idl3_to_idl2_visitor::~idl3_to_idl2_visitor (void)
{
  delete this->os;
  this->os = 0;
}

int
idl3_to_idl2_visitor::visit_decl (AST_Decl *)
{
  return 0;
}

int
idl3_to_idl2_visitor::visit_scope (UTL_Scope *node)
{
  for (UTL_ScopeActiveIterator si (node, UTL_Scope::IK_decls);
       !si.is_done ();
       si.next ())
    {
      AST_Decl *d = si.item ();

      if (d->node_type () == AST_Decl::NT_pre_defined)
        {
          continue;
        }

      // Want to skip the uses_xxxConnection structs added by uses
      // multiple ports.
      // @@@ (JP) This will go away when the visitor is finished, since
      // those uses_xxxConnection structs will not be added to the AST.
      if (ScopeAsDecl (node)->node_type () == AST_Decl::NT_component
          && d->node_type () != AST_Decl::NT_attr)
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
idl3_to_idl2_visitor::visit_type (AST_Type *)
{
  return 0;
}

int
idl3_to_idl2_visitor::visit_predefined_type (AST_PredefinedType *)
{
  return 0;
}

int
idl3_to_idl2_visitor::visit_module (AST_Module *node)
{
  if (node->imported ())
    {
      return 0;
    }

  *os << be_nl << be_nl;

  *os << "module " << node->local_name () << be_nl
      << "{" << be_idt;

  this->check_id_and_version (node);
  this->check_prefix (node);

  if (this->visit_scope (node) != 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                          "idl3_to_idl2_visitor::visit_module - "
                          "codegen for scope failed\n"),
                        -1);
    }

  *os << be_uidt_nl
      << "};";

  return 0;
}

int
idl3_to_idl2_visitor::visit_interface (AST_Interface *node)
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

  *os << "interface " << node->local_name ();

  AST_Interface **parents = node->inherits ();

  for (long i = 0; i < node->n_inherits (); ++i)
    {
      if (i == 0)
        {
          *os << " : ";
        }
      else
        {
          *os << ", ";
        }

      *os << parents[i]->name ();
    }

  *os << be_nl
      << "{" << be_idt;

  this->check_id_and_version (node);
  this->check_prefix (node);

  if (this->visit_scope (node) != 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                          "idl3_to_idl2_visitor::visit_interface - "
                          "codegen for scope failed\n"),
                        -1);
    }

  *os << be_uidt_nl
      << "};";

  return 0;
}

int
idl3_to_idl2_visitor::visit_interface_fwd (AST_InterfaceFwd *node)
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

  *os << "interface " << node->local_name () << ";";

  return 0;
}

int
idl3_to_idl2_visitor::visit_valuebox (AST_ValueBox *node)
{
  if (node->imported ())
    {
      return 0;
    }

  *os << be_nl << be_nl
      << "valuetype " << node->local_name ();

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
idl3_to_idl2_visitor::visit_valuetype (AST_ValueType *node)
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

  *os << "valuetype " << node->local_name ();

  AST_Decl::NodeType nt = node->node_type ();
  AST_Interface **parents = node->inherits ();
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

      *os << parents[i]->name ();
    }

  if (nt == AST_Decl::NT_eventtype)
    {
      *os << (ninherits == 0 ? " : " : ", ")
          << "Components::EventBase";
    }

  AST_Interface **supports = node->supports ();

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

      *os << supports[i]->name ();
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
idl3_to_idl2_visitor::visit_valuetype_fwd (AST_ValueTypeFwd *node)
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

  *os << "valuetype " << node->local_name () << ";";

  return 0;
}

int
idl3_to_idl2_visitor::visit_component (AST_Component *node)
{
  if (node->imported ())
    {
      return 0;
    }

  *os << be_nl << be_nl
      << "interface " << node->local_name ();

  AST_Component *base = node->base_component ();
  long nsupports = node->n_supports ();

  *os << " : "
      << (base != 0 ? base->full_name () : "Components::CCMObject");

  for (long i = 0; i < nsupports; ++i)
    {
      *os << ", " << node->supports ()[i]->name ();
    }

  *os << be_nl
      << "{" << be_idt;

  this->check_id_and_version (node);

  if (this->visit_scope (node) != 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                          "idl3_to_idl2_visitor::visit_component - "
                          "codegen for scope failed\n"),
                        -1);
    }

  this->gen_provides (node);
  this->gen_uses (node);
  this->gen_publishes (node);
  this->gen_emits (node);
  this->gen_consumes (node);

  *os << be_uidt_nl
      << "};";

  return 0;
}

int
idl3_to_idl2_visitor::visit_component_fwd (AST_ComponentFwd *node)
{
  if (node->imported ())
    {
      return 0;
    }

  *os << be_nl << be_nl;

  *os << "component " << node->local_name () << ";";

  return 0;
}

int
idl3_to_idl2_visitor::visit_eventtype (AST_EventType *node)
{
  if (node->imported ())
    {
      return 0;
    }

  if (this->visit_valuetype (node) != 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                          "idl3_to_idl2_visitor::visit_eventtype - "
                          "codegen for valuetype failed\n"),
                        -1);
    }

  *os << be_nl << be_nl
      << "interface " << node->local_name () << "Consumer : ";

  AST_Interface *parent = 0;
  AST_Decl::NodeType nt = AST_Decl::NT_native;

  if (node->n_inherits () > 0)
    {
      parent = node->inherits ()[0];
      AST_Type *ut = parent->unaliased_type ();
      nt = ut->node_type ();
    }

  if (node->n_inherits () == 0 || nt == AST_Decl::NT_valuetype)
    {
      *os << "Components::EventConsumerBase";
    }
  else
    {
      *os << node->inherits ()[0]->name () << "Consumer";
    }

  *os << be_nl
      << "{" << be_idt_nl
      << "void push_" << node->local_name () << " (in "
      << node->local_name () << " the_"
      << node->local_name () << ");" << be_uidt_nl
      << "};";

  return 0;
}

int
idl3_to_idl2_visitor::visit_eventtype_fwd (AST_EventTypeFwd *node)
{
  if (node->imported ())
    {
      return 0;
    }

  if (this->visit_valuetype_fwd (node) != 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                          "idl3_to_idl2_visitor::visit_eventtype_fwd - "
                          "codegen for valuetype_fwd failed\n"),
                        -1);
    }

  *os << be_nl
      << "interface " << node->local_name () << "Consumer;";

  return 0;
}

int
idl3_to_idl2_visitor::visit_home (AST_Home *node)
{
  if (node->imported ())
    {
      return 0;
    }

  ACE_CString explicit_name = node->local_name ()->get_string ();
  explicit_name += "Explicit";

  *os << be_nl << be_nl
      << "interface " << explicit_name.c_str () << " : ";

  AST_Home *base = node->base_home ();

  if (base == 0)
    {
      *os << "Components::CCMHome";
    }
  else
    {
      *os << base->name () << "Explicit";
    }

  *os << be_nl
      << "{" << be_idt;

  this->check_id_and_version (node);

  // Create a temporary interface node corresponding to the one we
  // just generated above for the home explicit interface.
  UTL_ScopedName *sn =
    this->create_scoped_name (0,
                              explicit_name.c_str (),
                              0,
                              ScopeAsDecl (node->defined_in ()));

  AST_Interface xplicit (sn,
                         0,
                         0,
                         0,
                         0,
                         false,
                         false);
  xplicit.set_defined_in (node->defined_in ());

  // Reset the home's decls to be defined in the explicit home interface.
  this->tranfer_scope_elements (node, xplicit);

  this->gen_factories (node, xplicit);
  this->gen_finders (node, xplicit);

  *os << be_uidt_nl
      << "};" << be_nl << be_nl;

  xplicit.destroy ();
  sn->destroy ();
  delete sn;
  sn = 0;

  AST_ValueType *key = node->primary_key ();

  // Generate the implicit home interface and its operations.
  *os << "interface " << node->local_name () << "Implicit"
      << (key == 0 ? " : Components::KeylessCCMHome" : "")
      << be_nl
      << "{" << be_idt_nl
      << node->managed_component ()->name () << " create (";

  if (key != 0)
    {
      *os << "in " << key->name () << " key";
    }

  *os << ")" << be_idt_nl
      << "raises (Components::CreateFailure";

  if (key != 0)
    {
      *os << ", Components::InvalidKey, Components::DuplicateKeyValue";
    }

  *os << ");" << be_uidt;

  if (key != 0)
    {
      *os << be_nl << be_nl
          << node->managed_component ()->name ()
          << " find_by_primary_key (in " << key->name ()
          << " key)" << be_idt_nl
          << "raises (Components::InvalidKey, Components::UnknownKeyValue, "
          << "Components::FinderFailure);" << be_uidt;

      *os << be_nl << be_nl
          << "void remove (in " << key->name () << " key)" << be_idt_nl
          << "raises (Components::InvalidKey, Components::UnknownKeyValue, "
          << "Components::RemoveFailure);" << be_uidt;

      *os << be_nl << be_nl
          << key->name () << " get_primary_key (in "
          << node->managed_component ()->name () << " comp);";
    }

  *os << be_uidt_nl
      << "};";

  // Create equivalent interface.
  *os << be_nl << be_nl
      << "interface " << node->local_name () << " : "
      << node->local_name () << "Explicit, "
      << node->local_name () << "Implicit" << be_nl
      << "{" << be_nl
      << "};";

  return 0;
}


int
idl3_to_idl2_visitor::visit_factory  (AST_Factory *node)
{
  if (node->imported ())
    {
      return 0;
    }

  *os << be_nl;

  *os << "factory " << node->local_name () << " (";

  this->gen_params (node, node->argument_count ());

  *os << ")";

  this->gen_exception_list (node->exceptions ());

  *os << ";";

  this->check_id_and_version (node);

  return 0;
}

int
idl3_to_idl2_visitor::visit_structure (AST_Structure *node)
{
  if (node->imported ())
    {
      return 0;
    }

  *os << be_nl << be_nl;

  *os << "struct " << node->local_name () << be_nl
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
idl3_to_idl2_visitor::visit_structure_fwd (AST_StructureFwd *node)
{
  if (node->imported ())
    {
      return 0;
    }

  *os << be_nl << be_nl;

  *os << "struct " << node->local_name () << ";";

  return 0;
}

int
idl3_to_idl2_visitor::visit_exception (AST_Exception *node)
{
  if (node->imported ())
    {
      return 0;
    }

  *os << be_nl << be_nl;

  *os << "exception " << node->local_name () << be_nl
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
idl3_to_idl2_visitor::visit_expression (AST_Expression *)
{
  return 0;
}

int
idl3_to_idl2_visitor::visit_enum (AST_Enum *node)
{
  if (node->imported ())
    {
      return 0;
    }

  *os << be_nl << be_nl;

  *os << "enum " << node->local_name () << be_nl
      << "{" << be_idt;

  for (UTL_ScopeActiveIterator i (node, UTL_Scope::IK_decls);
       !i.is_done ();)
    {
      *os << be_nl;

      AST_EnumVal *ev = AST_EnumVal::narrow_from_decl (i.item ());
      *os << ev->local_name ();

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
idl3_to_idl2_visitor::visit_operation (AST_Operation *node)
{
  *os << be_nl << be_nl;

  if (node->flags () == AST_Operation::OP_oneway)
    {
      *os << "oneway ";
    }

  *os << this->type_name (node->return_type ());
  *os << " " << node->local_name () << " (";

  this->gen_params (node, node->argument_count ());

  *os << ")";

  this->gen_exception_list (node->exceptions ());

  *os << ";";

  this->check_id_and_version (node);

  return 0;
}

int
idl3_to_idl2_visitor::visit_field (AST_Field *node)
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
      *os << " " << node->local_name ();
    }

  *os << ";";

  return 0;
}

int
idl3_to_idl2_visitor::visit_argument (AST_Argument *node)
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
      << " " << node->local_name ();

  return 0;
}

int
idl3_to_idl2_visitor::visit_attribute (AST_Attribute *node)
{
  bool rd_only = node->readonly ();

  // Keep output statements separate because of side effects.
  // No need to check for anonymous array - anonymous types not
  // accepted by parser for attributes.
  *os << be_nl << be_nl
      << (rd_only ? "readonly " : "") << "attribute ";
  *os << this->type_name (node->field_type ());
  *os << " " << node->local_name ();

  this->gen_exception_list (node->get_get_exceptions (),
                            rd_only ? "" : "get");

  this->gen_exception_list (node->get_set_exceptions (),
                            "set");

  *os << ";";

  this->check_id_and_version (node);

  return 0;
}

int
idl3_to_idl2_visitor::visit_union (AST_Union *node)
{
  if (node->imported ())
    {
      return 0;
    }

  this->disc_type_ = node->disc_type ()->unaliased_type ();

  *os << be_nl << be_nl;

  *os << "union " << node->local_name () << " switch ("
      << node->disc_type ()->name () << ")" << be_nl
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
idl3_to_idl2_visitor::visit_union_fwd (AST_UnionFwd *node)
{
  if (node->imported ())
    {
      return 0;
    }

  *os << be_nl << be_nl
      << "union " << node->local_name () << ";";

  return 0;
}

int
idl3_to_idl2_visitor::visit_union_branch (AST_UnionBranch *node)
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
      *os << " " << node->local_name ();
    }

  *os << ";";

  return 0;
}

int
idl3_to_idl2_visitor::visit_union_label (AST_UnionLabel *node)
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
idl3_to_idl2_visitor::visit_constant (AST_Constant *node)
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
        *os << node->enum_full_name ();
        break;
      default:
        break;
    }

  *os <<  " "
      << node->local_name () << " = " << node->constant_value () << ";";

  this->check_id_and_version (node);

  return 0;
}

int
idl3_to_idl2_visitor::visit_enum_val (AST_EnumVal *)
{
  return 0;
}

int
idl3_to_idl2_visitor::visit_array (AST_Array *node)
{
  *os << node->base_type ()->name ();

  for (unsigned long i = 0; i < node->n_dims (); ++i)
    {
      *os << "[" << node->dims ()[i] << "]";
    }

  return 0;
}

int
idl3_to_idl2_visitor::visit_sequence (AST_Sequence *node)
{
  // Keep output statements separate because of side effects.
  *os << "sequence<";
  *os << this->type_name (node->base_type ());

  if (!node->unbounded ())
    {
      *os << ", " << node->max_size ()->ev ()->u.ulval;
    }

  *os << "> ";

  return 0;
}

int
idl3_to_idl2_visitor::visit_string (AST_String *node)
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
idl3_to_idl2_visitor::visit_typedef (AST_Typedef *node)
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
      *os << " " << node->local_name ();
   }

  *os << ";";

  this->check_id_and_version (node);

  return 0;
}

int
idl3_to_idl2_visitor::visit_root (AST_Root *node)
{
  ACE_NEW_RETURN (this->os,
                  TAO_SunSoft_OutStream,
                  -1);

  ACE_CString fn (idl_global->stripped_filename ()->get_string ());
  fn = fn.substr (0, fn.rfind ('.'));
  fn += "_IDL2.idl";

  const char *path = be_global->output_dir ();
  ACE_CString target_name;

  if (path != 0)
    {
      target_name = path;
      target_name += "/";
    }

  target_name += fn;

  if (this->os->open (target_name.c_str ()) != 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Failed to open file %s for writing.\n",
                  target_name.c_str ()));

      return -1;
    }

  *os << be_nl;

  os->gen_ifndef_string (fn.c_str (), "_TAO_IDL_", "_IDL_");

  ACE_CString raw_filename;
  ACE_CString filename;

  for (size_t i = 0; i < idl_global->n_included_idl_files (); ++i)
    {
      if (i == 0)
        {
          *os << be_nl;
        }

      raw_filename = idl_global->included_idl_files ()[i];

      if (raw_filename.find (".pidl") != ACE_CString::npos
          || raw_filename == "orb.idl"
          || raw_filename == "Components.idl")
        {
          filename = raw_filename;
        }
      else
        {
          filename =
            raw_filename.substr (0, raw_filename.rfind ('.')) + "_IDL2.idl";
        }

      *os << be_nl
          << "#include \"" << filename.c_str () << "\"";
    }

  const char *pfix = node->prefix ();

  if (ACE_OS::strcmp (pfix, "") != 0)
    {
      *os << be_nl << be_nl
          << "#pragma prefix \"" << pfix << "\"";
    }

  if (this->visit_scope (node) != 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                          "idl3_to_idl2_visitor::visit_root - "
                          "codegen for scope failed\n"),
                        -1);
    }

  *os << be_nl << be_nl
      << "#endif /* ifndef */" << be_nl << be_nl;

  return 0;
}

int
idl3_to_idl2_visitor::visit_native (AST_Native *node)
{
  if (node->imported ())
    {
      return 0;
    }

  *os << be_nl << be_nl;

  *os << "native " << node->local_name () << ";";

  return 0;
}

void
idl3_to_idl2_visitor::check_prefix (AST_Decl *d)
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
          << "typeprefix " << d->local_name ()
          << " \"" << the_prefix << "\";";
    }
}

void
idl3_to_idl2_visitor::check_id_and_version (AST_Decl *d)
{
  if (d->typeid_set ())
    {
      *os << be_nl
          << "typeid " << d->local_name ()
          << " \"" << d->repoID () << "\";";

      return;
    }

  const char *the_version = d->version ();
  AST_Decl *p = ScopeAsDecl (d->defined_in ());

  if (ACE_OS::strcmp (the_version, p->version ()) != 0)
    {
      *os << "\n"
          << "#pragma version " << d->local_name ()
          << " " << the_version;
    }
}

const char *
idl3_to_idl2_visitor::type_name (AST_Type *t)
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
        return t->full_name ();
    }
}

void
idl3_to_idl2_visitor::gen_anonymous_array (AST_Type *a,
                                           AST_Decl *wrapper)
{
  AST_Array *array = AST_Array::narrow_from_decl (a);
  AST_Type *bt = array->base_type ();

  *os << this->type_name (bt);
  *os << " " << wrapper->local_name ();

  for (unsigned long i = 0; i < array->n_dims (); ++i)
    {
      *os << "[" << array->dims ()[i]->ev ()->u.ulval << "]";
    }
}

void
idl3_to_idl2_visitor::gen_label_value (AST_UnionLabel *node)
{
  AST_Expression *val = node->label_val ();
  AST_Expression::AST_ExprValue *ev = val->ev ();

  if (this->disc_type_->node_type () == AST_Decl::NT_enum)
    {
      UTL_Scope *s = this->disc_type_->defined_in ();

      if (s == 0)
        {
          *os << val->n ();
        }
      else
        {
          *os << ScopeAsDecl (s)->name () << "::"
              << val->n ()->last_component ();
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
        this->os->print ("%ld", ev->u.llval);
#endif /* ! defined (ACE_LACKS_LONGLONG_T) */
        break;
      case AST_Expression::EV_ulonglong:
#if ! defined (ACE_LACKS_LONGLONG_T)
        *os << "ACE_UINT64_LITERAL (";
        this->os->print (ACE_UINT64_FORMAT_SPECIFIER, ev->u.ullval);
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
        *os << (ev->u.bval ? "true" : "false");
        break;
      case AST_Expression::EV_enum:
        *os << val->n ();
        break;
      default:
        break;
    }
}

void
idl3_to_idl2_visitor::gen_provides (AST_Component *node)
{
  ACE_Unbounded_Queue<AST_Component::port_description> &s =
    node->provides ();
  AST_Component::port_description *pd = 0;

  for (ACE_Unbounded_Queue_Iterator<AST_Component::port_description> iter (s);
       ! iter.done ();
       iter.advance ())
    {
      iter.next (pd);

      *os << be_nl << be_nl
          << pd->impl->name () << " provide_" << pd->id << " ();";
    }
}

void
idl3_to_idl2_visitor::gen_uses (AST_Component *node)
{
  ACE_Unbounded_Queue<AST_Component::port_description> &s =
    node->uses ();
  AST_Component::port_description *pd = 0;

  for (ACE_Unbounded_Queue_Iterator<AST_Component::port_description> iter (s);
       ! iter.done ();
       iter.advance ())
    {
      iter.next (pd);

      *os << be_nl << be_nl;

      if (pd->is_multiple)
        {
          *os << "struct " << pd->id << "Connection" << be_nl
              << "{" << be_idt_nl
              << pd->impl->name () << " objref;" << be_nl
              << "Components::Cookie ck;" << be_uidt_nl
              << "};" << be_nl << be_nl
              << "typedef sequence<" << pd->id << "Connection> "
              << pd->id << "Connections;"
              << be_nl << be_nl
              << "Components::Cookie connect_" << pd->id << " (in "
              << pd->impl->name () << " connection)" << be_idt_nl
              << "raises (Components::ExceededConnectionLimit, "
              << "Components::InvalidConnection);" << be_uidt_nl << be_nl
              << pd->impl->name () << " disconnect_" << pd->id
              << " (in Components::Cookie ck)" << be_idt_nl
              << "raises (Components::InvalidConnection);"
              << be_uidt_nl << be_nl
              << pd->id << "Connections get_connections_" << pd->id
              << " ();";
        }
      else
        {
          *os << "void connect_" << pd->id << " (in "
              << pd->impl->name () << " conxn)" << be_idt_nl
              << "raises (Components::AlreadyConnected, "
              << "Components::InvalidConnection);" << be_uidt_nl << be_nl
              << pd->impl->name () << " disconnect_" << pd->id
              << " ()" << be_idt_nl
              << "raises (Components::NoConnection);" << be_uidt_nl << be_nl
              << pd->impl->name () << " get_connection_" << pd->id
              << " ();";
        }
    }
}

void
idl3_to_idl2_visitor::gen_publishes (AST_Component *node)
{
  ACE_Unbounded_Queue<AST_Component::port_description> &s =
    node->publishes ();
  AST_Component::port_description *pd = 0;

  for (ACE_Unbounded_Queue_Iterator<AST_Component::port_description> iter (s);
       ! iter.done ();
       iter.advance ())
    {
      iter.next (pd);

      *os << be_nl << be_nl
          << "Components::Cookie subscribe_" << pd->id
          << " (in " << pd->impl->name () <<"Consumer consumer)"
          << be_idt_nl
          << "raises (Components::ExceededConnectionLimit);"
          << be_uidt_nl << be_nl
          << pd->impl->name () << "Consumer unsubscribe_" << pd->id
          << " (in Components::Cookie ck)" << be_idt_nl
          << "raises (Components::InvalidConnection);" << be_uidt;
    }
}

void
idl3_to_idl2_visitor::gen_emits (AST_Component *node)
{
  ACE_Unbounded_Queue<AST_Component::port_description> &s =
    node->emits ();
  AST_Component::port_description *pd = 0;

  for (ACE_Unbounded_Queue_Iterator<AST_Component::port_description> iter (s);
       ! iter.done ();
       iter.advance ())
    {
      iter.next (pd);

      *os << be_nl << be_nl
          << "void connect_" << pd->id
          << " (in " << pd->impl->name ()
          << "Consumer consumer)" << be_idt_nl
          << "raises (Components::AlreadyConnected);"
          << be_uidt_nl << be_nl
          << pd->impl->name () << "Consumer disconnect_" << pd->id
          << " ()" << be_idt_nl
          << "raises (Components::NoConnection);" << be_uidt;
    }
}

void
idl3_to_idl2_visitor::gen_consumes (AST_Component *node)
{
  ACE_Unbounded_Queue<AST_Component::port_description> &s =
    node->consumes ();
  AST_Component::port_description *pd = 0;

  for (ACE_Unbounded_Queue_Iterator<AST_Component::port_description> iter (s);
       ! iter.done ();
       iter.advance ())
    {
      iter.next (pd);

      *os << be_nl << be_nl
          << pd->impl->name () << "Consumer get_consumer_" << pd->id
          << " ();";
    }
}

UTL_ScopedName *
idl3_to_idl2_visitor::create_scoped_name (const char *prefix,
                                          const char *local_name,
                                          const char *suffix,
                                          AST_Decl *parent)
{
  ACE_CString local_string (prefix,
                            0,
                            0);
  local_string += local_name;
  local_string += suffix;
  Identifier *local_id = 0;
  ACE_NEW_RETURN (local_id,
                  Identifier (local_string.fast_rep ()),
                  0);
  UTL_ScopedName *last_segment = 0;
  ACE_NEW_RETURN (last_segment,
                  UTL_ScopedName (local_id,
                                  0),
                  0);
  UTL_ScopedName *full_name =
    static_cast<UTL_ScopedName *> (parent->name ()->copy ());
  full_name->nconc (last_segment);
  return full_name;
}

void
idl3_to_idl2_visitor::tranfer_scope_elements (AST_Home *src,
                                              AST_Interface &dst)
{
  // Transfer the elements of the home's scope to the temporary
  // explicit home interface.
  for (UTL_ScopeActiveIterator src_iter (src, UTL_Scope::IK_decls);
       ! src_iter.is_done ();
       src_iter.next ())
    {
      AST_Decl *d = src_iter.item ();
      d->set_defined_in (&dst);
      UTL_ScopedName *new_name =
        this->create_scoped_name (0,
                                  d->local_name ()->get_string (),
                                  0,
                                  &dst);
      d->set_name (new_name);
      dst.add_to_scope (d);
    }

  // Visit the transferred scope elements normally to generate the IDL.
  // This way referenced items will have the interface's name in the
  // scoped name instead of the home's name.
  for (UTL_ScopeActiveIterator dst_iter (&dst, UTL_Scope::IK_decls);
       ! dst_iter.is_done ();
       dst_iter.next ())
    {
      if (dst_iter.item ()->ast_accept (this) != 0)
        {
          ACE_ERROR ((LM_ERROR,
                      "idl3_to_idl2_visitor::tranfer_scope_elements - "
                      "codegen for destination scope failed\n"));
        }
    }
}

void
idl3_to_idl2_visitor::gen_factories (AST_Home *node,
                                     AST_Interface &)
{
  AST_Operation **item = 0;

  for (ACE_Unbounded_Queue_Iterator<AST_Operation *> i (node->factories ());
       ! i.done ();
       i.advance ())
    {
      i.next (item);

      *os << be_nl << be_nl
          << node->managed_component ()->name () << " "
          << (*item)->local_name () << " (";

      this->gen_params (*item, (*item)->argument_count ());

      *os << ")";

      UTL_ExceptList *exceps = (*item)->exceptions ();

      if (exceps != 0 && exceps->length () > 0)
        {
          this->gen_exception_list (exceps, "", false);
        }
      else
        {
          *os << be_idt_nl
              << "raises (";
        }

      *os << "Components::CreateFailure);" << be_uidt;
    }
}

void
idl3_to_idl2_visitor::gen_finders (AST_Home *node,
                                   AST_Interface &)
{
  AST_Operation **item = 0;

  for (ACE_Unbounded_Queue_Iterator<AST_Operation *> i (node->finders ());
       ! i.done ();
       i.advance ())
    {
      i.next (item);

      *os << be_nl << be_nl
          << node->managed_component ()->name () << " "
          << (*item)->local_name () << " (";

      this->gen_params (*item, (*item)->argument_count ());

      *os << ")";

      UTL_ExceptList *exceps = (*item)->exceptions ();

      if (exceps != 0 && exceps->length () > 0)
        {
          this->gen_exception_list (exceps, "", false);
        }
      else
        {
          *os << be_idt_nl
              << "raises (";
        }

      *os << "Components::FinderFailure);" << be_uidt;
    }
}

void
idl3_to_idl2_visitor::gen_params (UTL_Scope *s, int arg_count)
{
  if (arg_count > 0)
    {
      *os << be_idt << be_idt;

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

      *os << be_uidt_nl << be_uidt;
    }
}

void
idl3_to_idl2_visitor::gen_exception_list (UTL_ExceptList *exceptions,
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
          *os << ei.item ()->name ();

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
