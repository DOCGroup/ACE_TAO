
//=============================================================================
/**
 *  @file    interface_svts.h
 *
 *  $Id$
 *
 *  Concrete visitor for the Interface node.
 *  This provides for code generation in the CIAO
 *  servant sourc e
 *
 *
 *  @author Marcel Smit
 */
//=============================================================================


#ifndef _BE_INTERFACE_INTERFACE_SVTS_H_
#define _BE_INTERFACE_INTERFACE_SVTS_H_

/**
 * @class be_visitor_interface_svts
 *
 * @brief be_visitor_interface_svts
 *
 * This is a concrete visitor to generate the
 * CIAO servant source for interface
 */
class be_visitor_interface_svts : public be_visitor_interface
{
public:
  be_visitor_interface_svts (be_visitor_context *ctx);

  ~be_visitor_interface_svts (void);

  virtual int visit_interface (be_interface *node);
  virtual int visit_operation (be_operation *node);
  virtual int visit_attribute (be_attribute *node);
};

#endif /* _BE_INTERFACE_INTERFACE_SVTS_H_ */
