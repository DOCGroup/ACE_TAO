
//=============================================================================
/**
 *  @file    valuetype_ss.cpp
 *
 *  $Id$
 *
 *  Visitor generating code for Interfaces in the server skeletons file.
 *
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================

#include "nr_extern.h"

// ************************************************************
// Interface visitor for server skeletons.
// ************************************************************

be_visitor_valuetype_ss::be_visitor_valuetype_ss (be_visitor_context *ctx)
  : be_visitor_valuetype (ctx)
{
}

be_visitor_valuetype_ss::~be_visitor_valuetype_ss (void)
{
}

int
be_visitor_valuetype_ss::visit_valuetype (be_valuetype *node)
{
  if (node->srv_skel_gen () || node->imported () || node->is_abstract ())
    {
      return 0;
    }

  AST_Type *concrete = node->supports_concrete ();

  // We generate a skeleton class only if the valuetype supports a
  // non-abstract interface.
  if (concrete == 0)
    {
      return 0;
    }

  // Generate the normal skeleton as usual.

  TAO_OutStream *os = this->ctx_->stream ();

  os->indent ();

  ACE_CString full_skel_name_holder =
    this->generate_full_skel_name (node);

  const char *full_skel_name = full_skel_name_holder.c_str ();

  ACE_CString flat_name_holder =
    this->generate_flat_name (node);

  *os << be_nl << "// TAO_IDL - Generated from " << be_nl
      << "// " << __FILE__ << ":" << __LINE__ << be_nl_2;

  // Find if we are at the top scope or inside some module,
  // pre-compute the prefix that must be added to the local name in
  // each case.
  const char *local_name_prefix = "";

  if (!node->is_nested ())
    {
      local_name_prefix = "POA_";
    }

  ACE_CString node_local_name_holder =
    this->generate_local_name (node);

  const char *node_local_name = node_local_name_holder.c_str ();

  *os << full_skel_name << "::"
      << local_name_prefix << node_local_name
      << " (void)" << be_nl
      << "{}" << be_nl_2;

// @@@ (JP) I'm commenting out the copy constructor for now. The
// declaration in the skeleton header file has been made private. These
// valuetypes (only if a concrete interface is supported) inherit
// from the stub-side valuetype, which has a private umimplemented
// copy constructor. This makes it impossible to call all the base
// class copy constructors, which some compilers require. If there
// is no fallout from this change, this code will be removed, if
// there is a problem, we'll have to reevaluate the approach.
/*
  *os << full_skel_name << "::"
      << local_name_prefix << node_local_name << " ("
      << "const " << local_name_prefix << node_local_name << "& rhs)";

  *os << be_idt_nl
      << ": TAO_Abstract_ServantBase (rhs)," << be_nl
      << "  TAO_ServantBase (rhs)," << be_idt_nl;

  if (concrete->is_nested ())
    {
      AST_Decl *scope = ScopeAsDecl (concrete->defined_in ());

      *os << "POA_" << scope->name () << "::"
          << concrete->local_name () << " (rhs)," << be_nl;
    }
  else
    {
      be_interface *bd = be_interface::narrow_from_decl (concrete);
      *os << bd->full_skel_name () << " (rhs)," << be_nl;
    }

  *os << "ValueBase (rhs)" << be_uidt << be_uidt_nl
      << "{}" << be_nl_2;
*/

  *os << full_skel_name << "::~"
      << local_name_prefix << node_local_name
      << " (void)" << be_nl
      << "{}";

  return 0;
}

int
be_visitor_valuetype_ss::visit_eventtype (be_eventtype *node)
{
  return this->visit_valuetype (node);
}

ACE_CString
be_visitor_valuetype_ss::generate_flat_name (be_valuetype *node)
{
  return ACE_CString (node->flat_name ());
}

ACE_CString
be_visitor_valuetype_ss::generate_local_name (be_valuetype *node)
{
  return ACE_CString (node->local_name ());
}

ACE_CString
be_visitor_valuetype_ss::generate_full_skel_name (be_valuetype *node)
{
  return ACE_CString (node->full_skel_name ());
}
