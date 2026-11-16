/* -*- c++ -*- */
//=============================================================================
/**
 *  @file    connector_dds_exs.h
 *
 *  Concrete visitor for the DDS Connector node.
 *  This provides for code generation in the exec impl source.
 *
 *  @author Jeff Parsons
 */
//=============================================================================

#ifndef _BE_CONNECTOR_CONNECTOR_DDS_EXS_H_
#define _BE_CONNECTOR_CONNECTOR_DDS_EXS_H_

/// Generates DDS connector executor implementation source.
class be_visitor_connector_dds_exs
  : public be_visitor_connector_dds_ex_base
{
public:
  explicit be_visitor_connector_dds_exs (be_visitor_context *ctx);

  ~be_visitor_connector_dds_exs () override;
  be_visitor_connector_dds_exs (be_visitor_connector_dds_exs const &) = delete;
  be_visitor_connector_dds_exs (be_visitor_connector_dds_exs &&) = delete;
  be_visitor_connector_dds_exs &operator= (be_visitor_connector_dds_exs const &) = delete;
  be_visitor_connector_dds_exs &operator= (be_visitor_connector_dds_exs &&) = delete;

  int visit_connector (be_connector *node) override;
};

#endif /* _BE_CONNECTOR_CONNECTOR_DDS_EXS_H_ */

