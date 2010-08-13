
//=============================================================================
/**
 *  @file    interface_svh.h
 *
 *  $Id$
 *
 *  Concrete visitor for the Interface node.
 *  This provides for code generation in the CIAO
 *  servant header
 *
 *
 *  @author Jeff Parsons <j.parsons@vanderbilt.edu>
 */
//=============================================================================


#ifndef _BE_INTERFACE_INTERFACE_SVH_H_
#define _BE_INTERFACE_INTERFACE_SVH_H_

/**
 * @class be_visitor_interface_svh
 *
 * @brief be_visitor_interface_svh
 *
 * This is a concrete visitor to generate the
 * CIAO servant header for interface
 */
class be_visitor_interface_svh : public be_visitor_interface
{
public:
  be_visitor_interface_svh (be_visitor_context *ctx);

  ~be_visitor_interface_svh (void);

  virtual int visit_interface (be_interface *node);
  virtual int visit_operation (be_operation *node);
  virtual int visit_attribute (be_attribute *node);
};

#endif /* _BE_INTERFACE_INTERFACE_SVH_H_ */
