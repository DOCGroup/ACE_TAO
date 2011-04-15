/* -*- c++ -*- */
//=============================================================================
/**
 *  @file    interface_cs.h
 *
 *  $Id$
 *
 *  Concrete visitor for the Interface node.
 *  This one provides code generation for interfaces in the client stubs.
 *
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================

#ifndef _BE_INTERFACE_INTERFACE_CS_H_
#define _BE_INTERFACE_INTERFACE_CS_H_

/**
 * @class be_visitor_interface_cs
 *
 * @brief be_visitor_interface_cs
 *
 * This is a concrete visitor to generate the client stubs for interface
 */
class be_visitor_interface_cs : public be_visitor_interface
{
public:
  be_visitor_interface_cs (be_visitor_context *ctx);
  ~be_visitor_interface_cs (void);

  virtual int visit_interface (be_interface *node);
  virtual int visit_component (be_component *node);
  virtual int visit_connector (be_connector *node);
  virtual int visit_extended_port (be_extended_port *node);
  virtual int visit_mirror_port (be_mirror_port *node);

  /// Helper to generate the definitions for the operations
  /// of any abstract parents we may have.
  static int gen_abstract_ops_helper (be_interface *node,
                                      be_interface *base,
                                      TAO_OutStream *os);

private:
  /// Helper method to generate _narrow and _unchecked_narrow. The
  /// string <n> will contain the needed predicate that needs to be
  /// generated.
  bool gen_xxx_narrow (const char *n,
                       be_interface *node);
};

#endif /* _BE_INTERFACE_INTERFACE_CS_H_ */
