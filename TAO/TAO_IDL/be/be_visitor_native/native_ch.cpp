//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    enum_ch.cpp
//
// = DESCRIPTION
//    Visitor generating code for Native in the client header
//
// = AUTHOR
//    Johnny Willemsen
//
// ============================================================================

#include "be_visitor_typecode/typecode_decl.h"

ACE_RCSID (be_visitor_native,
           native_ch,
           "$Id$")


// ********************************************************************
// Visitor implementation for the Native type
// This one for the client header file
// ********************************************************************

be_visitor_native_ch::be_visitor_native_ch (be_visitor_context *ctx)
  : be_visitor_scope (ctx)
{
}

be_visitor_native_ch::~be_visitor_native_ch (void)
{
}

// Visit the native_ch node and its scope.
int
be_visitor_native_ch::visit_native (be_native *node)
{
  if (node->cli_hdr_gen () || node->imported ())
    {
      return 0;
    }

  TAO_OutStream *os = this->ctx_->stream ();

  *os << be_nl << be_nl << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__ << be_nl << be_nl;

  const char *node_name = node->full_name ();
  if (ACE_OS::strcmp (node_name, "PortableServer::ServantLocator::Cookie") == 0)
    *os << "typedef void *Cookie;" << be_nl;

  node->cli_hdr_gen (true);
  return 0;
}

