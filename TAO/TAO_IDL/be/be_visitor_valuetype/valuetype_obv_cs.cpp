

//=============================================================================
/**
 *  @file    valuetype_obv_cs.cpp
 *
 *  $Id$
 *
 *  Visitor generating code for Valuetypes
 *  OBV_ class implementation
 *  (see C++ mapping OMG 20.17)
 *
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

be_visitor_valuetype_obv_cs::~be_visitor_valuetype_obv_cs (void)
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

  *os << be_nl_2 << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__ << be_nl_2;

  // Default constructor.
  *os << node->full_obv_skel_name () << "::";

  if (! node->is_nested ())
    {
      *os << "OBV_";
    }

  *os << node->local_name () << " (void)" << be_nl;
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

  *os << node->local_name () << " (void)" << be_nl
      << "{}";


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
              << "::_add_ref (void)" << be_nl
              << "{" << be_idt_nl
              << "this->::CORBA::DefaultValueRefCountBase::_add_ref ();"
              << be_uidt_nl
              << "}" << be_nl;

          *os << be_nl << "void" << be_nl
              << node->full_obv_skel_name ()
              << "::_remove_ref (void)" << be_nl
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
  visitor.in_obv_space_ = 1;

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
  if (parent != 0)
    {
      be_valuetype *be_parent =
        be_valuetype::narrow_from_decl (parent);
      this->gen_obv_init_base_constructor_args (be_parent, index);
    }

  for (UTL_ScopeActiveIterator si (node, UTL_Scope::IK_decls);
       !si.is_done ();
       si.next())
    {
      // be_attribute inherits from be_field
      // so we have to also screen out attributes
      be_field *f = be_field::narrow_from_decl (si.item ());
      be_attribute *attr =
        be_attribute::narrow_from_decl (si.item ());

      if (f == 0 || attr != 0)
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
  if (parent != 0)
    {
      be_valuetype *be_parent = be_valuetype::narrow_from_decl (parent);
      this->gen_obv_init_constructor_inits (be_parent);
    }

  for (UTL_ScopeActiveIterator si (node, UTL_Scope::IK_decls);
       !si.is_done ();
       si.next())
    {
      // be_attribute inherits from be_field
      // so we have to also screen out attributes
      be_field *f = be_field::narrow_from_decl (si.item ());
      be_attribute *attr =
        be_attribute::narrow_from_decl (si.item ());

      if (f == 0 || attr != 0)
        {
          continue;
        }

      *os << be_nl
          << f->local_name () << " (_tao_init_" << f->local_name ()
          << ");";
    }
}
