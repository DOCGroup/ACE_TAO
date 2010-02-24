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
//    connector_dds_exh.h
//
// = DESCRIPTION
//    Concrete visitor for the DDS Connector node.
//    This provides for code generation in the exec impl header
//
// = AUTHOR
//    Jeff Parsons
//
// ============================================================================

#ifndef _BE_CONNECTOR_CONNECTOR_DDS_EXH_H_
#define _BE_CONNECTOR_CONNECTOR_DDS_EXH_H_

class be_visitor_context;

class be_visitor_connector_dds_exh
  : public be_visitor_component_scope
{
  //
  // = TITLE
  //   be_visitor_connector_dds_exh
  //
  // = DESCRIPTION
  //   This is a concrete visitor to generate
  //   the executor implementation header for DDS connectors.
  //
  //
public:
  be_visitor_connector_dds_exh (be_visitor_context *ctx);
  
  ~be_visitor_connector_dds_exh (void);
  
  virtual int visit_connector (be_connector *node);
};

#endif /* _BE_CONNECTOR_CONNECTOR_DDS_EXH_H_ */

