//=============================================================================
/**
 *  @file    valuetype_obv_cs.cpp
 *
 *  Visitor generating code for Valuetypes
 *  OBV_ class implementation
 *  (see C++ mapping OMG 20.17)
 *
 *  @author Torsten Kuepper  <kuepper2@lfa.uni-wuppertal.de>
 *  @author derived from interface_ch.cpp from Aniruddha Gokhale
 */
//=============================================================================

#include "valuetype.h"
#include "be_union_branch.h"

#include <string>
#include <functional>
#include <vector>

// ******************************************************
// Valuetype visitor for OBV_ class implementation
// ******************************************************

be_visitor_valuetype_obv_cs::be_visitor_valuetype_obv_cs (
    be_visitor_context *ctx
  )
  : be_visitor_valuetype (ctx)
{
}

be_visitor_valuetype_obv_cs::~be_visitor_valuetype_obv_cs () = default;

// OBV_ class must be in OBV_ namespace
int
be_visitor_valuetype_obv_cs::visit_valuetype (be_valuetype *node)
{
  // only visit non-abstract non-imported valuetype
  if (node->is_abstract () || node->imported ())
    {
      return 0;
    }

  TAO_OutStream *os = this->ctx_->stream ();

  TAO_INSERT_COMMENT (os);

  // Initializing constructor.
  if (node->has_member ())
    {
      *os << node->full_obv_skel_name () << "::";

      if (! node->is_nested ())
        {
          *os << "OBV_";
        }

      *os << node->local_name () << " (" << be_idt << be_idt;

      unsigned long index = 0;
      this->gen_obv_init_constructor_args (node, index);

      *os << ")" << be_uidt << be_uidt << be_uidt_nl
          << "{" << be_idt;

      this->gen_obv_init_constructor_inits (node);

      *os << be_uidt_nl
          << "}" << be_nl_2;
    }

  *os << node->full_obv_skel_name () << "::~";
  if (! node->is_nested ())
    {
      *os << "OBV_";
    }
  *os << node->local_name () << " ()" << be_nl
      << "{}";

  // A generated _copy_value() is needed for every valuetype without
  // operations, including derived valuetypes and factory-based values.
  if (!node->have_operation () && !node->opt_accessor ())
    {
      *os << be_nl_2
          << "::CORBA::ValueBase *" << be_nl
          << node->full_obv_skel_name () << "::_copy_value ()" << be_nl
          << "{" << be_idt_nl
          << "::TAO::Valuetype_Copy_Context _tao_copy_context;" << be_nl
          << "if (::CORBA::ValueBase * const _tao_existing = "
          << "::TAO::Valuetype_Copy_Context::find (this))" << be_idt_nl
          << "{" << be_idt_nl
          << "_tao_existing->_add_ref ();" << be_nl
          << "return _tao_existing;" << be_uidt_nl
          << "}" << be_uidt_nl
          << "::CORBA::ValueBase *ret_val {};" << be_nl
          << "ACE_NEW_THROW_EX (" << be_idt_nl
          << "ret_val," << be_nl;
      if (! node->is_nested ())
        {
          *os << "OBV_";
        }
      *os << node->local_name () << " (";
      if (node->has_member ())
        {
          unsigned long index = 0ul;
          *os << be_idt;
          gen_obv_call_base_constructor_args (node, index);
          *os << be_uidt_nl;
        }
      *os << ")," << be_nl
          << "::CORBA::NO_MEMORY ());" << be_uidt
          << be_nl
          << "::CORBA::ValueBase_var _tao_copy (ret_val);" << be_nl
          << "::TAO::Valuetype_Copy_Context::remember (this, _tao_copy.in ());"
          << be_nl << node->full_obv_skel_name ()
          << " * const _tao_obv_copy = dynamic_cast<"
          << node->full_obv_skel_name () << " *> (_tao_copy.in ());";

      // Clone nested valuetype references only after registering this value.
      // The context makes self-references and shared references terminate and
      // preserves their topology in the copied graph.
      std::vector<be_valuetype *> hierarchy;
      for (be_valuetype *current = node; current != nullptr;)
        {
          hierarchy.push_back (current);
          AST_Type *parent = current->inherits_concrete ();
          current = dynamic_cast<be_valuetype *> (parent);
        }
      for (auto current = hierarchy.rbegin (); current != hierarchy.rend (); ++current)
        {
          for (UTL_ScopeActiveIterator si (*current, UTL_Scope::IK_decls);
               !si.is_done ();
               si.next ())
            {
              be_field *field = dynamic_cast<be_field *> (si.item ());
              if (field == nullptr || dynamic_cast<be_attribute *> (si.item ()) != nullptr)
                {
                  continue;
                }

              be_type *type = dynamic_cast<be_type *> (field->field_type ());
              while (be_typedef *alias = dynamic_cast<be_typedef *> (type))
                {
                  type = alias->primitive_base_type ();
                }

              std::string const target = "_tao_obv_copy->"
                + field->local_name ()->get_string () + " ()";
              std::function<void (be_type *, std::string const &,
                                  std::string const &, bool, size_t)> emit_copy;
              emit_copy = [&] (be_type *copy_type,
                               std::string const &read,
                               std::string const &write,
                               bool setter,
                               size_t depth)
                {
                  while (be_typedef *alias = dynamic_cast<be_typedef *> (copy_type))
                    {
                      copy_type = alias->primitive_base_type ();
                    }

                  if (dynamic_cast<be_valuetype_fwd *> (copy_type) != nullptr
                      || dynamic_cast<be_valuetype *> (copy_type) != nullptr
                      || dynamic_cast<be_valuebox *> (copy_type) != nullptr)
                    {
                      std::string const nested = "_tao_nested_"
                        + std::to_string (depth);
                      *os << be_nl << "if (" << read << " != nullptr)"
                          << be_idt_nl << "{" << be_idt_nl
                          << "::CORBA::ValueBase_var " << nested << " ("
                          << read << "->_copy_value ());" << be_nl
                          << write << (setter ? " (" : " = ")
                          << copy_type->full_name () << "::_downcast ("
                          << nested << (setter ? ".in ()));" : "._retn ());")
                          << be_uidt_nl << "}" << be_uidt;
                      return;
                    }

                  if (be_sequence *sequence = dynamic_cast<be_sequence *> (copy_type))
                    {
                      std::string const index = "_tao_i_" + std::to_string (depth);
                      *os << be_nl << "for (::CORBA::ULong " << index << " {};"
                          << " " << index << " < " << read << ".length (); ++"
                          << index << ")" << be_idt_nl << "{" << be_idt;
                      std::string const element = read + "[" + index + "]";
                      emit_copy (dynamic_cast<be_type *> (sequence->base_type ()),
                                 element, element, false, depth + 1);
                      *os << be_uidt_nl << "}" << be_uidt;
                      return;
                    }

                  if (be_structure *structure = dynamic_cast<be_structure *> (copy_type))
                    {
                      for (UTL_ScopeActiveIterator member_it (structure, UTL_Scope::IK_decls);
                           !member_it.is_done ();
                           member_it.next ())
                        {
                          be_field *member = dynamic_cast<be_field *> (member_it.item ());
                          if (member == nullptr)
                            {
                              continue;
                            }
                          std::string const member_expr = read + "."
                            + member->local_name ()->get_string ();
                          emit_copy (dynamic_cast<be_type *> (member->field_type ()),
                                     member_expr, member_expr, false, depth + 1);
                        }
                      return;
                    }

                  if (be_union *union_type = dynamic_cast<be_union *> (copy_type))
                    {
                      *os << be_nl << "switch (" << read << "._d ())"
                          << be_idt_nl << "{" << be_idt;
                      for (UTL_ScopeActiveIterator branch_it (union_type, UTL_Scope::IK_decls);
                           !branch_it.is_done ();
                           branch_it.next ())
                        {
                          be_union_branch *branch =
                            dynamic_cast<be_union_branch *> (branch_it.item ());
                          if (branch == nullptr)
                            {
                              continue;
                            }
                          for (size_t label = 0;
                               label < branch->label_list_length ();
                               ++label)
                            {
                              if (branch->label (label)->label_kind ()
                                  == AST_UnionLabel::UL_default)
                                {
                                  *os << be_nl << "default:";
                                }
                              else
                                {
                                  *os << be_nl << "case ";
                                  branch->gen_label_value (os, label);
                                  *os << ":";
                                }
                            }
                          *os << be_idt_nl << "{" << be_idt;
                          std::string const branch_name =
                            branch->local_name ()->get_string ();
                          std::string const branch_expr = read + "."
                            + branch_name + " ()";
                          be_type *branch_type =
                            dynamic_cast<be_type *> (branch->field_type ());
                          while (be_typedef *alias =
                                   dynamic_cast<be_typedef *> (branch_type))
                            {
                              branch_type = alias->primitive_base_type ();
                            }
                          if (dynamic_cast<be_valuetype_fwd *> (branch_type) != nullptr
                              || dynamic_cast<be_valuetype *> (branch_type) != nullptr
                              || dynamic_cast<be_valuebox *> (branch_type) != nullptr)
                            {
                              emit_copy (branch_type, branch_expr,
                                         read + "." + branch_name, true, depth + 1);
                            }
                          else
                            {
                              std::string const branch_copy = "_tao_branch_"
                                + std::to_string (depth);
                              *os << be_nl << "auto " << branch_copy << " = "
                                  << branch_expr << ";";
                              emit_copy (branch_type, branch_copy, branch_copy,
                                         false, depth + 1);
                              *os << be_nl << read << "." << branch_name << " ("
                                  << branch_copy << ");";
                            }
                          *os << be_nl << "break;" << be_uidt_nl << "}"
                              << be_uidt;
                        }
                      *os << be_uidt_nl << "}" << be_uidt;
                      return;
                    }

                  if (be_array *array = dynamic_cast<be_array *> (copy_type))
                    {
                      std::string indexed = read;
                      for (size_t dim = 0; dim < array->n_dims (); ++dim)
                        {
                          std::string const index = "_tao_i_"
                            + std::to_string (depth) + "_" + std::to_string (dim);
                          size_t const bound = array->dims ()[dim]->ev ()->u.ulval;
                          *os << be_nl << "for (::CORBA::ULong " << index << " {};"
                              << " " << index << " < " << bound << "U; ++"
                              << index << ")" << be_idt_nl << "{" << be_idt;
                          indexed += "[" + index + "]";
                        }
                      emit_copy (dynamic_cast<be_type *> (array->base_type ()),
                                 indexed, indexed, false, depth + array->n_dims ());
                      for (size_t dim = 0; dim < array->n_dims (); ++dim)
                        {
                          *os << be_uidt_nl << "}" << be_uidt;
                        }
                    }
                };

              bool const is_value =
                dynamic_cast<be_valuetype_fwd *> (type) != nullptr
                || dynamic_cast<be_valuetype *> (type) != nullptr
                || dynamic_cast<be_valuebox *> (type) != nullptr;
              emit_copy (type, target,
                         is_value ? "_tao_obv_copy->" + field->local_name ()->get_string ()
                                  : target,
                         is_value, 0);
            }
        }
      *os << be_nl << "return _tao_copy._retn ();" << be_uidt_nl << "}";
    }

  // OBV_ class has no accessors or modifiers if we are optimizing
  // or the valuetype is abstract.
  if (!node->opt_accessor ())
    {
      if (be_global->cdr_support ())
        {
          *os << be_nl_2 << "::CORBA::Boolean" << be_nl
              << node->full_obv_skel_name()
              << "::_tao_marshal__" << node->flat_name()
              << " (TAO_OutputCDR &strm, TAO_ChunkInfo& ci) const" << be_nl
              << "{" << be_idt_nl
              << "return _tao_marshal_state (strm, ci);" << be_uidt_nl
              << "}" << be_nl_2;

          *os << "::CORBA::Boolean" << be_nl
              << node->full_obv_skel_name()
              << "::_tao_unmarshal__" << node->flat_name()
              << " (TAO_InputCDR &strm, TAO_ChunkInfo& ci)" << be_nl
              << "{" << be_idt_nl
              << "return _tao_unmarshal_state (strm, ci);" << be_uidt_nl
              << "}";
        }

      if (this->visit_scope (node) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_valuetype_obv_cs::"
                             "visit_valuetype - "
                             "visit_scope failed\n"),
                            -1);
        }

      // If we inherit from CORBA::Object and/or CORBA::AbstractBase
      // (in addition to CORBA::ValueBase) we have to add these
      // to avoid ambiguity.
      if (node->has_supported_interface ())
        {
          *os << be_nl_2 << "void" << be_nl
              << node->full_obv_skel_name ()
              << "::_add_ref ()" << be_nl
              << "{" << be_idt_nl
              << "this->::CORBA::DefaultValueRefCountBase::_add_ref ();"
              << be_uidt_nl
              << "}" << be_nl;

          *os << be_nl << "void" << be_nl
              << node->full_obv_skel_name ()
              << "::_remove_ref ()" << be_nl
              << "{" << be_idt_nl
              << "this->::CORBA::DefaultValueRefCountBase::_remove_ref ();"
              << be_uidt_nl
              << "}";
        }
    }

  return 0;
}

int
be_visitor_valuetype_obv_cs::visit_eventtype (be_eventtype *node)
{
  return this->visit_valuetype (node);
}

int
be_visitor_valuetype_obv_cs::visit_field (be_field *node)
{
  be_visitor_context ctx = (*this->ctx_);
  be_visitor_valuetype_field_cs visitor (&ctx);
  visitor.in_obv_space_ = true;

  if (visitor.visit_field (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuetype_obv_cs::"
                         "visit_field - "
                         "visit_field failed\n"
                         ), -1);
    }

  return 0;
}

void
be_visitor_valuetype_obv_cs::gen_obv_init_base_constructor_args (
    be_valuetype *node,
    unsigned long &index)
{
  TAO_OutStream *os = this->ctx_->stream ();
  AST_Type *parent = node->inherits_concrete ();

  // Generate for inherited members first.
  if (parent != nullptr)
    {
      be_valuetype *be_parent =
        dynamic_cast<be_valuetype*> (parent);
      this->gen_obv_init_base_constructor_args (be_parent, index);
    }

  for (UTL_ScopeActiveIterator si (node, UTL_Scope::IK_decls);
       !si.is_done ();
       si.next())
    {
      // be_attribute inherits from be_field
      // so we have to also screen out attributes
      be_field *f = dynamic_cast<be_field*> (si.item ());
      be_attribute *attr =
        dynamic_cast<be_attribute*> (si.item ());

      if (f == nullptr || attr != nullptr)
        {
          continue;
        }

      *os << (index++ != 0 ? "," : "") << be_nl
          << "_tao_init_" << f->local_name ();
    }
}

void
be_visitor_valuetype_obv_cs::gen_obv_init_constructor_inits (
    be_valuetype *node)
{
  TAO_OutStream *os = this->ctx_->stream ();
  AST_Type *parent = node->inherits_concrete ();

  // Generate for inherited members first.
  if (parent != nullptr)
    {
      be_valuetype *be_parent = dynamic_cast<be_valuetype*> (parent);
      this->gen_obv_init_constructor_inits (be_parent);
    }

  for (UTL_ScopeActiveIterator si (node, UTL_Scope::IK_decls);
       !si.is_done ();
       si.next())
    {
      // be_attribute inherits from be_field
      // so we have to also screen out attributes
      be_field *f = dynamic_cast<be_field*> (si.item ());
      be_attribute *attr =
        dynamic_cast<be_attribute*> (si.item ());

      if (f == nullptr || attr != nullptr)
        {
          continue;
        }

      *os << be_nl
          << f->local_name () << " (_tao_init_" << f->local_name ()
          << ");";
    }
}

void
be_visitor_valuetype_obv_cs::gen_obv_call_base_constructor_args (
  be_valuetype *node,
  unsigned long &index)
{
  TAO_OutStream *os = this->ctx_->stream ();

  // Generate for inherited members first.
  AST_Type *parent = node->inherits_concrete ();
  if (parent != nullptr)
    {
      be_valuetype *be_parent =
        dynamic_cast<be_valuetype*> (parent);
      this->gen_obv_call_base_constructor_args (be_parent, index);
    }

  // Copy the current state shallowly. Nested valuetype references are cloned
  // after the new object is registered in Valuetype_Copy_Context.
  for (UTL_ScopeActiveIterator si (node, UTL_Scope::IK_decls);
       !si.is_done ();
       si.next())
    {
      // be_attribute inherits from be_field
      // so we have to also screen out attributes
      be_field *f = dynamic_cast<be_field*> (si.item ());
      if (f && !dynamic_cast<be_attribute*> (si.item ()))
        {
          if (index++) // comma before 2nd onwards
            *os << ",";

          *os << be_nl << f->local_name () << " ()";
        }
    }
}
