//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    valuetype_ss.cpp
//
// = DESCRIPTION
//    Visitor generating code for Interfaces in the server skeletons file.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#include "nr_extern.h"

ACE_RCSID (be_visitor_valuetype,
           valuetype_ss,
           "$Id$")


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

  AST_Interface *concrete = node->supports_concrete ();

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
      << "// " << __FILE__ << ":" << __LINE__ << be_nl << be_nl;

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
      << "{}" << be_nl << be_nl;

  *os << full_skel_name << "::"
      << local_name_prefix << node_local_name << " ("
      << "const " << local_name_prefix << node_local_name << "& rhs)";

  *os << be_idt_nl
      << ": ";

  if (concrete->is_nested ())
    {
      AST_Decl *scope = ScopeAsDecl (concrete->defined_in ());

      *os << "ACE_NESTED_CLASS (POA_" << scope->name () << ", "
          << concrete->local_name () << ") (rhs)";
    }
  else
    {
      be_interface *bd = be_interface::narrow_from_decl (concrete);
      *os << bd->full_skel_name () << " (rhs)";
    }

  *os << be_uidt_nl << "{}" << be_nl << be_nl;

  *os << full_skel_name << "::~"
      << local_name_prefix << node_local_name
      << " (void)" << be_nl
      << "{}" << be_nl << be_nl;

  return 0;
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
