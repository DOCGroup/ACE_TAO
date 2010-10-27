// -*- C++ -*-

//=============================================================================
/**
 *  @file  objref_typecode.cpp
 *
 *  $Id$
 *
 *  Object reference TypeCode generation visitor.
 *
 *  @author  Ossama Othman <ossama@dre.vanderbilt.edu>
 */
//=============================================================================


TAO::be_visitor_objref_typecode::be_visitor_objref_typecode (
  be_visitor_context * ctx)
  : be_visitor_typecode_defn (ctx)
{
}

int
TAO::be_visitor_objref_typecode::visit_interface (be_interface * node)
{
  if (!node->is_defined ())
    return this->gen_forward_declared_typecode (node);

  static char const abstract_interface[] = "abstract_interface";
  static char const component[]          = "component";
  static char const home[]               = "home";
  static char const local_interface[]    = "local_interface";
  static char const objref[]             = "objref";

  char const * kind = 0;

  if (dynamic_cast<be_component *> (node))
    kind = component;
  else if (dynamic_cast<be_home *> (node))
    kind = home;
  else if (node->is_abstract ())
    kind = abstract_interface;
  else if (node->is_local ())
    kind = local_interface;
  else
    kind = objref;

  return this->visit_i (kind,
                        node->flat_name (),
                        node->repoID (),
                        node->original_local_name ()->get_string (),
                        node);
}

int
TAO::be_visitor_objref_typecode::visit_native (be_native * /* node */)
{
  return 0;
//   return this->visit_i ("native",
//                         ,
//                         ,
//                         ,
//                         node);
}

int
TAO::be_visitor_objref_typecode::visit_i (char const * kind,
                                          char const * flat_name,
                                          char const * repository_id,
                                          char const * original_local_name,
                                          be_type * node)
{
  TAO_OutStream & os = *this->ctx_->stream ();

  os << be_nl_2
     << "// TAO_IDL - Generated from" << be_nl
     << "// " << __FILE__ << ":" << __LINE__ << be_nl_2;

  // Generate the TypeCode instantiation.
  os
    << "static TAO::TypeCode::Objref<char const *," << be_nl
    << "                             TAO::Null_RefCount_Policy>"
    << be_idt_nl
    << "_tao_tc_" << flat_name << " (" << be_idt_nl
    << "::CORBA::tk_" << kind << "," << be_nl
    << "\"" << repository_id << "\"," << be_nl
    << "\"" << original_local_name << "\");" << be_uidt_nl
    << be_uidt_nl;

  if (this->gen_typecode_ptr (node) != 0)
    return -1;

  return 0;
}
