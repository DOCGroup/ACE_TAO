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

  // Generate the TypeCode instantiation.
  os
    << "static TAO::TypeCode::Alias<const char *," << be_nl
    << "                            TAO::Null_RefCount_Policy> const"
    << be_idt_nl
    << "_tao_tc_" << node->flat_name () << " (" << be_idt_nl
    << "\"" << node->repoID () << "\"," << be_nl
    << "\"" << node->original_local_name () << "\"," << be_nl
    << "&" << base->tc_name () << ");" << be_uidt_nl
    << be_uidt_nl;

  return this->gen_typecode_ptr (node);
}
