/* -*- c++ -*- */
//=============================================================================
/**
 *  @file    connector_dds_exh.h
 *
 *  $Id$
 *
 *  Concrete visitor for the DDS Connector node.
 *  This provides for code generation in the exec impl header
 *
 *
 *  @author Jeff Parsons
 */
//=============================================================================


#ifndef _BE_CONNECTOR_CONNECTOR_DDS_EXH_H_
#define _BE_CONNECTOR_CONNECTOR_DDS_EXH_H_

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
  be_visitor_connector_dds_exh (be_visitor_context *ctx);

  ~be_visitor_connector_dds_exh (void);

  virtual int visit_connector (be_connector *node);

  /// Used to generate the traits structures.
  virtual int visit_provides (be_provides *node);
  virtual int visit_uses (be_uses *node);

private:
  /// Generate application-specific instantiations of
  /// middleware templates containing traits constructs.
  void gen_dds_traits (void);
  void gen_connector_traits (void);

  /// Common code triggered by provides and uses ports.
  void gen_interface_connector_trait (be_interface *iface,
                                      bool for_facet);

  /// Have to keep a list of port interfaces so their
  /// dds
  ACE_Unbounded_Queue<be_interface *> port_ifaces_;
};

#endif /* _BE_CONNECTOR_CONNECTOR_DDS_EXH_H_ */

