//
// $Id$
//
/* -*- c++ -*- */
// ============================================================================
//
// = LIBRARY
//    TAO_IDL_BE
//
// = FILENAME
//    connector_dds_exs.h
//
// = DESCRIPTION
//    Concrete visitor for the DDS Connector node.
//    This provides for code generation in the exec impl source.
//
// = AUTHOR
//    Jeff Parsons
//
// ============================================================================

#ifndef _BE_CONNECTOR_CONNECTOR_DDS_EXS_H_
#define _BE_CONNECTOR_CONNECTOR_DDS_EXS_H_

class be_visitor_connector_dds_exs
  : public be_visitor_component_scope
{
  //
  // = TITLE
  //   be_visitor_connector_dds_exs
  //
  // = DESCRIPTION
  //   This is a concrete visitor to generate
  //   the executor implementation source for connectors.
  //
  //
public:
  be_visitor_connector_dds_exs (be_visitor_context *ctx);
  
  ~be_visitor_connector_dds_exs (void);
  
  virtual int visit_connector (be_connector *node);
};

#endif /* _BE_CONNECTOR_CONNECTOR_DDS_EXS_H_ */

