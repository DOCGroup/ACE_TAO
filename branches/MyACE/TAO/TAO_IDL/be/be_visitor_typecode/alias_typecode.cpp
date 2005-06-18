// -*- C++ -*-

//=============================================================================
/**
 *  @file  alias_typecode.cpp
 *
 *  $Id$
 *
 *  Alias (typedef) TypeCode generation visitor.
 *
 *  @author  Ossama Othman <ossama@dre.vanderbilt.edu>
 */
//=============================================================================


TAO::be_visitor_alias_typecode::be_visitor_alias_typecode (
  be_visitor_context * ctx)
  : be_visitor_typecode_defn (ctx)
{
}

int
TAO::be_visitor_alias_typecode::visit_typedef (be_typedef * node)
{
  be_type * const base = be_type::narrow_from_decl (node->base_type ());

  TAO_OutStream & os = *this->ctx_->stream ();

  os << be_nl << be_nl
     << "// TAO_IDL - Generated from" << be_nl
     << "// " << __FILE__ << ":" << __LINE__ << be_nl << be_nl;

  this->ctx_->sub_state (TAO_CodeGen::TAO_TC_DEFN_TYPECODE_NESTED);

  // Generate typecode for the base type, being careful to avoid doing
  // so for a typedef since that could recursively cause multiple base
  // type TypeCode definitions to be generated.
  if (!base || (base->node_type () != AST_Decl::NT_typedef
                && base->accept (this) == -1))
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("(%N:%l) be_visitor_alias_typecode")
                         ACE_TEXT ("::visit_typedef) - ")
                         ACE_TEXT ("failed to generate base typecode\n")),
                        -1);
    }

  // Generate the alias TypeCode instantiation.
  os
    << "static TAO::TypeCode::Alias<char const *," << be_nl
    << "                            CORBA::TypeCode_ptr const *," << be_nl
    << "                            TAO::Null_RefCount_Policy>"
    << be_idt_nl
    << "_tao_tc_" << node->flat_name () << " (" << be_idt_nl
    << "CORBA::tk_alias," << be_nl
    << "\"" << node->repoID () << "\"," << be_nl
    << "\"" << node->original_local_name () << "\"," << be_nl
    << "&";

  if (this->gen_base_typecode_name (base) != 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("(%N:%l) be_visitor_typecode_defn::")
                       ACE_TEXT ("visit_sequence - problem generating base ")
                       ACE_TEXT ("TypeCode name.\n")),
                       -1);


  os << ");" << be_uidt_nl
     << be_uidt_nl;

  return this->gen_typecode_ptr (node);
}
