//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    extended_port.cpp
//
// = DESCRIPTION
//    Base class visitor generating code for extended ports.
//
// = AUTHOR
//    Jeff Parsons
//
// ============================================================================

// ******************************************************
// Base class extended port visitor
// ******************************************************

be_visitor_extended_port::be_visitor_extended_port (
      be_visitor_context *ctx)
  : be_visitor_decl (ctx),
    template_args_ (0),
    port_type_ (0)
{
}

be_visitor_extended_port::~be_visitor_extended_port (void)
{
}

