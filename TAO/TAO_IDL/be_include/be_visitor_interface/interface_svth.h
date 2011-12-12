
//=============================================================================
/**
 *  @file    interface_svth.h
 *
 *  $Id$
 *
 *  Concrete visitor for the Interface node.
 *  This provides for code generation in the CIAO
 *  servant header
 *
 *
 *  @author Marcel Smit
 */
//=============================================================================


#ifndef _BE_INTERFACE_INTERFACE_SVTH_H_
#define _BE_INTERFACE_INTERFACE_SVTH_H_

/**
 * @class be_visitor_interface_svth
 *
 * @brief be_visitor_interface_svth
 *
 * This is a concrete visitor to generate the
 * CIAO servant header for interface
 */
class be_visitor_interface_svth : public be_visitor_interface
{
public:
  be_visitor_interface_svth (be_visitor_context *ctx);

  ~be_visitor_interface_svth (void);

  virtual int visit_interface (be_interface *node);
  virtual int visit_operation (be_operation *node);
  virtual int visit_attribute (be_attribute *node);
};

#endif /* _BE_INTERFACE_INTERFACE_SVTH_H_ */
