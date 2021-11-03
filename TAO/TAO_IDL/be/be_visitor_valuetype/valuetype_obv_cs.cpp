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

// ******************************************************
// Valuetype visitor for OBV_ class implementation
// ******************************************************

be_visitor_valuetype_obv_cs::be_visitor_valuetype_obv_cs (
    be_visitor_context *ctx
  )
  : be_visitor_valuetype (ctx)
{
}

be_visitor_valuetype_obv_cs::~be_visitor_valuetype_obv_cs ()
{
}

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

  // Default constructor.
  *os << node->full_obv_skel_name () << "::";

  if (! node->is_nested ())
    {
      *os << "OBV_";
    }

  *os << node->local_name () << " ()" << be_nl;
  *os << ": require_truncation_ (false)" << be_nl
      << "{}" << be_nl_2;

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
          << ": require_truncation_ (false)" << be_nl
          << "{" << be_idt;

      this->gen_obv_init_constructor_inits (node);

      *os << be_uidt_nl
          << "}" << be_nl_2;
    }

  // Destructor.
  *os << node->full_obv_skel_name () << "::~";
  if (! node->is_nested ())
    {
      *os << "OBV_";
    }
  *os << node->local_name () << " ()" << be_nl
      << "{}";

  // Virtual _copy_value() only provided in OBV_* class when
  // ::CORBA::DefaultValueRefCountBase has been included.
  // The OBV_ class is concrete in this case and so requires
  // a _copy_value definition.
  // Otherwise, the end user must derive from this abstract
  // OBV_* class and it is up to them to provide the correct
  // implimentation of the _copy_value() there.
  if (this->obv_need_ref_counter (node))
    {
      *os << be_nl_2
          << "::CORBA::ValueBase *" << be_nl
          << node->full_obv_skel_name () << "::_copy_value ()" << be_nl
          << "{" << be_idt_nl
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
          << "::CORBA::NO_MEMORY ()" << be_uidt_nl
          << ");" << be_nl
          << "return ret_val;" << be_uidt_nl
          << "}";
    }

  // OBV_ class has no accessors or modifiers if we are optimizing
  // or the valuetype is abstract.
  if (!node->opt_accessor ())
    {
      *os << be_nl_2 << "::CORBA::Boolean" << be_nl
          << node->full_obv_skel_name ()
          << "::_tao_marshal__" << node->flat_name ()
          <<    " (TAO_OutputCDR &strm, TAO_ChunkInfo& ci) const" << be_nl
          << "{" << be_idt_nl
          << "return _tao_marshal_state (strm, ci);" << be_uidt_nl
          << "}" << be_nl_2;

      *os << "::CORBA::Boolean" << be_nl
          << node->full_obv_skel_name ()
          << "::_tao_unmarshal__" << node->flat_name ()
          << " (TAO_InputCDR &strm, TAO_ChunkInfo& ci)" << be_nl
          << "{" << be_idt_nl
          << "return _tao_unmarshal_state (strm, ci);" << be_uidt_nl
          << "}";

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
      if (node->n_supports () > 0)
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
    unsigned long &index
  )
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
    be_valuetype *node
  )
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
  unsigned long &index
  )
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

  // Now generate for each "derived" added members
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

          // output each accessor on new line
          *os << be_nl;

          // Check the member type for nested valuetypes
          be_type *t = dynamic_cast<be_type*> (f->field_type ());
          if (dynamic_cast<be_valuetype_fwd*> (t) ||
              dynamic_cast<be_valuetype*> (t) ||
              dynamic_cast<be_valuebox*> (t) )
            {
              // Nested valuetypes/boxes need to be deep copied also
              *os << "(" << f->local_name () << " () ?" << be_idt_nl
                  << t->full_name () << "::_downcast (" << f->local_name ()
                  << " ()->_copy_value ())" << be_nl
                  << ": 0)" << be_uidt;
            }
          else // Simple non-nested type
            *os << f->local_name () << " ()";
        }
    }
}
