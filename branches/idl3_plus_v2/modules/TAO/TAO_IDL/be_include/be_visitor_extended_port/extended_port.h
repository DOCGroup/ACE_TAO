//
// $Id$
//
/* -*- c++ -*- */
// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    extended_port.h
//
// = DESCRIPTION
//    Base class visitor for the extended port node.
//
// = AUTHOR
//    Jeff Parsons
//
// ============================================================================

#ifndef _BE_VISITOR_EXTENDED_PORT_EXTENEDED_PORT_H_
#define _BE_VISITOR_EXTENDED_PORT_EXTENEDED_PORT_H_

class be_visitor_extended_port
  : public be_visitor_scope
{
  //
  // = TITLE
  //   be_visitor_extended_port
  //
  // = DESCRIPTION
  //   This is a base class visitor for other extended port
  //   visitors.
  //
  //
protected:
  be_visitor_extended_port (be_visitor_context *ctx);
  
  ~be_visitor_extended_port (void);
  
protected:
  AST_Template_Common::T_ARGLIST *template_args_;
  be_porttype *port_type_;
};

#endif /* _BE_VISITOR_EXTENDED_PORT_EXTENEDED_PORT_H_ */


