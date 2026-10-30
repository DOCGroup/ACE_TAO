/* -*- c++ -*- */
//=============================================================================
/**
 *  @file    connector_dds_exh.h
 *
 *  Concrete visitor for the DDS Connector node.
 *  This provides for code generation in the exec impl header
 *
 *  @author Jeff Parsons
 */
//=============================================================================


#ifndef _BE_CONNECTOR_CONNECTOR_DDS_EXH_H_
#define _BE_CONNECTOR_CONNECTOR_DDS_EXH_H_

#include <vector>

class be_visitor_context;

class be_visitor_connector_dds_exh
  : public be_visitor_connector_dds_ex_base
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
  explicit be_visitor_connector_dds_exh (be_visitor_context *ctx);

  ~be_visitor_connector_dds_exh () override;
  be_visitor_connector_dds_exh (be_visitor_connector_dds_exh const &) = delete;
  be_visitor_connector_dds_exh (be_visitor_connector_dds_exh &&) = delete;
  be_visitor_connector_dds_exh &operator= (be_visitor_connector_dds_exh const &) = delete;
  be_visitor_connector_dds_exh &operator= (be_visitor_connector_dds_exh &&) = delete;

  int visit_connector (be_connector *node) override;

  /// Override which also calls the base class version.
  int visit_mirror_port (be_mirror_port *node) override;

  /// Used to generate the traits structures.
  int visit_provides (be_provides *node) override;
  int visit_uses (be_uses *node) override;
  int visit_attribute (be_attribute *node) override;

private:
  /// Generate application-specific instantiations of
  /// middleware templates containing traits constructs.
  void gen_dds_traits (AST_Decl *datatype);
  void gen_connector_traits ();

  /// Common code triggered by provides and uses ports.
  void gen_interface_connector_trait (be_interface *iface,
                                      be_field *port_elem,
                                      bool for_facet);

  /// Have to keep a list of port interfaces so their
  /// dds
  std::vector<be_interface *> port_ifaces_;
};

#endif /* _BE_CONNECTOR_CONNECTOR_DDS_EXH_H_ */

