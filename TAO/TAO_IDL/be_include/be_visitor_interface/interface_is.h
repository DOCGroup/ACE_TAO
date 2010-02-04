
//=============================================================================
/**
 *  @file    interface_is.h
 *
 *  $Id$
 *
 *  Concrete visitor for the Interface node.
 *  This provides for code generation in the implementation skeleton
 *
 *
 *  @author Yamuna Krishnamurthy (yamuna@cs.wustl.edu)
 */
//=============================================================================


#ifndef _BE_INTERFACE_INTERFACE_IS_H_
#define _BE_INTERFACE_INTERFACE_IS_H_

/**
 * @class be_visitor_interface_is
 *
 * @brief be_visitor_interface_is
 *
 * This is a concrete visitor to generate the implementation skeletons for interface
 */
class be_visitor_interface_is : public be_visitor_interface
{
public:
  /// constructor
  be_visitor_interface_is (be_visitor_context *ctx);

  /// destructor
  ~be_visitor_interface_is (void);

  /// set the right context and make a visitor
  virtual int visit_interface (be_interface *node);


  static int method_helper (be_interface *derived,
                            be_interface *node,
                            TAO_OutStream *os);
};

#endif /* _BE_INTERFACE_INTERFACE_IS_H_ */
