
//=============================================================================
/**
 *  @file    interface_svs.h
 *
 *  $Id$
 *
 *  Concrete visitor for the Interface node.
 *  This provides for code generation in the CIAO
 *  servant sourc e
 *
 *
 *  @author Jeff Parsons <j.parsons@vanderbilt.edu>
 */
//=============================================================================


#ifndef _BE_INTERFACE_INTERFACE_SVS_H_
#define _BE_INTERFACE_INTERFACE_SVS_H_

/**
 * @class be_visitor_interface_svs
 *
 * @brief be_visitor_interface_svs
 *
 * This is a concrete visitor to generate the
 * CIAO servant source for interface
 */
class be_visitor_interface_svs : public be_visitor_interface
{
public:
  be_visitor_interface_svs (be_visitor_context *ctx);

  ~be_visitor_interface_svs (void);

  virtual int visit_interface (be_interface *node);
  virtual int visit_operation (be_operation *node);
  virtual int visit_attribute (be_attribute *node);
};

#endif /* _BE_INTERFACE_INTERFACE_SVS_H_ */
