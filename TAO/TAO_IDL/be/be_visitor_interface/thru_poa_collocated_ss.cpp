//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    thru_poa_collocated_ss.cpp
//
// = DESCRIPTION
//    Visitor generating code for thru_poa_collocated classes for an Interface in the
//    server skeleton file.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

ACE_RCSID (be_visitor_interface, 
           thru_poa_collocated_ss, 
           "$Id$")


// ************************************************************
//  be_visitor_interface_collacted_ss
// ************************************************************

be_visitor_interface_thru_poa_collocated_ss::
be_visitor_interface_thru_poa_collocated_ss (be_visitor_context *ctx)
  : be_visitor_interface (ctx)
{
}

be_visitor_interface_thru_poa_collocated_ss::
~be_visitor_interface_thru_poa_collocated_ss (void)
{
}

int be_visitor_interface_thru_poa_collocated_ss::visit_interface (
    be_interface *node
  )
{
  TAO_OutStream *os = this->ctx_->stream ();

  this->ctx_->node (node);

  os->indent ();
  *os << node->full_coll_name (be_interface::THRU_POA) << "::"
      << node->local_coll_name (be_interface::THRU_POA) << " (" << be_idt_nl
      << "TAO_Stub *stub" << be_uidt_nl
      << ")" << be_idt_nl;

  *os << ":";

  // @@ We should call the constructor for all base classes, since we
  // are using multiple inheritance.

  if (node->traverse_inheritance_graph (
          be_visitor_interface_thru_poa_collocated_ss::collocated_ctor_helper,
          os
        )
      == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_interface_thru_poa_collocated_ss::"
                         "visit_interface - "
                         "codegen for base class ctor init failed\n"),
                        -1);
    }

  *os << "  CORBA_Object (stub, 1)" << be_uidt_nl;

  *os << "{" << be_nl;
  *os << "}" << be_nl << be_nl;

  // Generate _is_a implementation.
  *os << "CORBA::Boolean " 
      << node->full_coll_name (be_interface::THRU_POA) << "::"
      << "_is_a"  << be_idt
      << "(" << be_idt_nl
      << "const CORBA::Char *logical_type_id" << be_nl
      << "ACE_ENV_ARG_DECL" << be_uidt_nl
      << ")" << be_uidt_nl << be_nl
      << "{" << be_idt_nl;
  *os <<"TAO_Object_Adapter::Servant_Upcall servant_upcall ("
      << be_idt << be_idt_nl
      << "this->_stubobj ()->"
      << "servant_orb_var ()->orb_core ()"
      << be_uidt_nl
      << ");" << be_uidt_nl
      << "CORBA::Object_var forward_to;" << be_nl
      << "servant_upcall.prepare_for_upcall (" << be_idt << be_idt_nl
      << "this->_object_key ()," << be_nl
      << "\"" << "_is_a" << "\"," << be_nl
      << "forward_to.out ()" << be_nl
      << "ACE_ENV_ARG_PARAMETER" << be_uidt_nl
      << ");" << be_uidt_nl
      << "ACE_CHECK_RETURN (0);" << be_nl;
  *os << "return ACE_reinterpret_cast (" << be_idt << be_idt_nl
      << node->full_skel_name () << "_ptr," << be_nl
      << "servant_upcall.servant ()->_downcast (" << be_idt << be_idt_nl
      << "\"" << node->repoID ()  << "\"" << be_uidt_nl
      << ")" << be_uidt << be_uidt_nl
      << ")->_is_a (logical_type_id ACE_ENV_ARG_PARAMETER);" 
      << be_uidt << be_uidt_nl
      << "}" << be_uidt_nl << be_nl << be_nl;

  // Generate _non_existent implementation.
  *os << "CORBA::Boolean " 
      << node->full_coll_name (be_interface::THRU_POA) << "::"
      << "_non_existent"  << be_idt
      << "(" << be_idt_nl
      << "ACE_ENV_SINGLE_ARG_DECL" << be_uidt_nl
      << ")" << be_uidt_nl << be_nl
      << "{" << be_idt_nl;
  *os <<"TAO_Object_Adapter::Servant_Upcall servant_upcall ("
      << be_idt << be_idt_nl
      << "this->_stubobj ()->"
      << "servant_orb_var ()->orb_core ()"
      << be_uidt_nl
      << ");" << be_uidt_nl
      << "CORBA::Object_var forward_to;" << be_nl
      << "servant_upcall.prepare_for_upcall (" << be_idt << be_idt_nl
      << "this->_object_key ()," << be_nl
      << "\"" << "_non_existent" << "\"," << be_nl
      << "forward_to.out ()" << be_nl
      << "ACE_ENV_ARG_PARAMETER" << be_uidt_nl
      << ");" << be_uidt_nl
      << "ACE_CHECK_RETURN (0);" << be_nl;
  *os << "return ACE_reinterpret_cast (" << be_idt << be_idt_nl
      << node->full_skel_name () << "_ptr," << be_nl
      << "servant_upcall.servant ()->_downcast (" << be_idt << be_idt_nl
      << "\"" << node->repoID ()  << "\"" << be_uidt_nl
      << ")" << be_uidt << be_uidt_nl
      << ")->_non_existent (ACE_ENV_SINGLE_ARG_PARAMETER);" 
      << be_uidt << be_uidt_nl
      << "}" << be_uidt_nl << be_nl << be_nl;

  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_interface_thru_poa_collocated_ss::"
                         "visit_scope - "
                         "codegen for scope failed\n"),
                        -1);
    }

  return 0;
}

int
be_visitor_interface_thru_poa_collocated_ss::collocated_ctor_helper (
    be_interface *derived,
    be_interface *base,
    TAO_OutStream *os
  )
{
  if (derived == base)
    {
      // We are the same. Don't do anything, otherwise we will end up calling
      // ourself.
      return 0;
    }

  if (base->is_nested ())
    {
      be_decl *scope;
      scope = be_scope::narrow_from_scope (base->defined_in ())->decl ();

      *os << "  ACE_NESTED_CLASS (POA_" << scope->full_name () << ","
          << base->local_coll_name (be_interface::THRU_POA) << ") (stub)," 
          << be_nl;
    }
  else
    {
      *os << "  " << base->full_coll_name (be_interface::THRU_POA) 
          << " (stub)," << be_nl;
    }

  return 0;
}
