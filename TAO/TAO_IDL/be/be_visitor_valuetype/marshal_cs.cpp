//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    marshal_cs.cpp
//
// = DESCRIPTION
//    Concrete visitor for valuetypes.
//    This one provides code generation for marshalling.
//
// = AUTHOR
//    Torsten Kuepper  <kuepper2@lfa.uni-wuppertal.de>
//
// ============================================================================


#include        "idl.h"
#include        "idl_extern.h"
#include        "be.h"

#include "be_visitor_valuetype.h"

ACE_RCSID(be_visitor_valuetype, marshal_cs, "$Id$")


be_visitor_valuetype_marshal_cs::be_visitor_valuetype_marshal_cs
(be_visitor_context *ctx)
  : be_visitor_scope (ctx)
{
}

be_visitor_valuetype_marshal_cs::~be_visitor_valuetype_marshal_cs (void)
{
}

int
be_visitor_valuetype_marshal_cs::visit_valuetype (be_valuetype *node)
{
  TAO_OutStream *os = this->ctx_->stream ();

  this->ctx_->sub_state(TAO_CodeGen::TAO_CDR_OUTPUT);

  *os << "CORBA::Boolean" << be_nl;
  this->class_name (node, os);
  *os << "::_tao_marshal_state (TAO_OutputCDR &strm)" << be_nl
      << "{" << be_idt_nl;
  be_valuetype *inh = node->statefull_inherit ();
  if (inh)
    {
      if (inh->opt_accessor ())
        {
          *os << "if (!";
          this->class_name (inh, os);
          *os << "::_tao_marshal_state (strm)) return 0;" << be_nl;
        }
      else // only can access base class via virtual function
        {
          *os << "if (!this->_tao_marshal__"
              <<       inh->flat_name ()
              << " (strm)) return 0;" << be_nl;
        }
    }

  { // array _forany
    be_visitor_context* new_ctx =
      new be_visitor_context (*this->ctx_);
    be_visitor_valuetype_field_cdr_decl field_decl (new_ctx);
    field_decl.visit_scope (node);
  }

  *os << "if (" << be_idt_nl;
  // all we have to do is to visit the scope and generate code
  this->gen_fields (node, *this->ctx_);

  *os << be_uidt_nl << ")"
      << be_idt_nl
      << "return 1;" << be_uidt_nl
      << "else" << be_idt_nl
      << "return 0;" << be_uidt_nl << be_uidt_nl
      << "}\n\n";

  // set the substate as generating code for the input operator
  this->ctx_->sub_state(TAO_CodeGen::TAO_CDR_INPUT);

  *os << "CORBA::Boolean" << be_nl;
  this->class_name (node, os);
  *os << "::_tao_unmarshal_state (TAO_InputCDR &strm)" << be_nl
      << "{" << be_idt_nl;
  inh = node->statefull_inherit ();
  if (inh)
    {
      if (inh->opt_accessor ())
        {
          *os << "if (!";
          this->class_name (inh, os);
          *os << "::_tao_unmarshal_state (strm)) return 0;" << be_nl;
        }
      else // only can access base class via virtual function
        {
          *os << "if (!this->_tao_unmarshal__"
              <<       inh->flat_name ()
              << " (strm)) return 0;" << be_nl;
        }
    }

  { // array _forany
    be_visitor_context* new_ctx =
      new be_visitor_context (*this->ctx_);
    be_visitor_valuetype_field_cdr_decl field_decl (new_ctx);
    field_decl.visit_scope (node);
  }

  *os << "if (" << be_idt_nl;
  // all we have to do is to visit the scope and generate code
  this->gen_fields (node, *this->ctx_);

  *os << be_uidt_nl << ")"
      << be_idt_nl
      << "return 1;" << be_uidt_nl
      << "else" << be_idt_nl
      << "return 0;" << be_uidt_nl << be_uidt_nl
      << "}\n\n";

  return 0;
}

// retrieve the fully scoped skeleton name
void
be_visitor_valuetype_marshal_cs::class_name (be_valuetype *node,
                                             TAO_OutStream *os)
{
  if (node->opt_accessor ())
    {
      be_decl *scope = be_scope::narrow_from_scope (node->defined_in ())->decl ();
      *os << "ACE_NESTED_CLASS ("
          << scope->name () << ","
          << node->local_name () << ")";
    }
  else
    *os << node->full_obv_skel_name ();
}

// ops for field marshal
int
be_visitor_valuetype_marshal_cs::gen_fields (be_valuetype *node,
                                             be_visitor_context &ctx)
{
  int n_processed = 0;

  TAO_OutStream *os = ctx.stream ();

  this->elem_number_ = 0;
  // initialize an iterator to iterate thru our scope
  for (UTL_ScopeActiveIterator si (node, UTL_Scope::IK_decls);
       !si.is_done ();
       si.next())
    {
      AST_Decl *d = si.item ();
      if (!d)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_scope::visit_scope - "
                             "bad node in this scope\n"), -1);
        }
      be_field *field = be_field::narrow_from_decl (d);
      if (field)
        {
          if (n_processed > 0)
            *os << " &&" << be_nl;
          ++n_processed;
          be_visitor_context* new_ctx =
            new be_visitor_context (ctx);
          be_visitor_valuetype_field_cdr_ci visitor (new_ctx);
          visitor.pre_ = node->field_pd_prefix ();
          visitor.post_ = node->field_pd_postfix ();
          if (visitor.visit_field (field) == -1)
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "(%N:%l) be_visitor_valuetype_marshal_cs::"
                                 "visit_valuetype - "
                                 "codegen for scope failed\n"), -1);
            }
        }
    }
  if (n_processed == 0)
    *os << "1";
  return 0;
}
