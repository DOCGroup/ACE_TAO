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
//    extended_port_svh.h
//
// = DESCRIPTION
//    Concrete visitor for the extended port node.
//    This provides for code generation in the servant header
//
// = AUTHOR
//    Jeff Parsons
//
// ============================================================================

#ifndef _BE_VISITOR_EXTENDED_PORT_EXTENEDED_PORT_SVH_H_
#define _BE_VISITOR_EXTENDED_PORT_EXTENEDED_PORT_SVH_H_

class be_visitor_extended_port_facet_svh
  : public be_visitor_extended_port
{
  //
  // = TITLE
  //   be_visitor_extended_port_facet_svh
  //
  // = DESCRIPTION
  //   This is a concrete visitor to generate the facet
  //   classes for an extended port.
  //
  //
public:
  be_visitor_extended_port_facet_svh (be_visitor_context *ctx);
  
  ~be_visitor_extended_port_facet_svh (void);
  
  virtual int visit_extended_port (be_extended_port *node);
  virtual int visit_porttype (be_porttype *node);
  virtual int visit_provides (be_provides *node);
};

#endif /* _BE_VISITOR_EXTENDED_PORT_EXTENEDED_PORT_SVH_H_ */


