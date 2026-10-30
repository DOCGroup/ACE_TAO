/* -*- c++ -*- */

//=============================================================================
/**
 *  @file    interface_ih.h
 *
 *  Concrete visitor for the Interface node.
 *  This provides for code generation in the implementation header
 *
 *  @author Yamuna Krishnamurthy (yamuna@cs.wustl.edu)
 */
//=============================================================================


#ifndef _BE_INTERFACE_INTERFACE_IH_H_
#define _BE_INTERFACE_INTERFACE_IH_H_

/**
 * @class be_visitor_interface_ih
 *
 * @brief be_visitor_interface_ih
 *
 * This is a concrete visitor to generate the implementation header for interface
 */
class be_visitor_interface_ih : public be_visitor_interface
{
public:
  /// constructor
  explicit be_visitor_interface_ih (be_visitor_context *ctx);

  /// destructor
  ~be_visitor_interface_ih () override;
  be_visitor_interface_ih (be_visitor_interface_ih const &) = delete;
  be_visitor_interface_ih (be_visitor_interface_ih &&) = delete;
  be_visitor_interface_ih &operator= (be_visitor_interface_ih const &) = delete;
  be_visitor_interface_ih &operator= (be_visitor_interface_ih &&) = delete;

  /// set the right context and make a visitor
  int visit_interface (be_interface *node) override;

  static int method_helper (be_interface *derived,
                            be_interface *node,
                            TAO_OutStream *os);
};

#endif /* _BE_INTERFACE_INTERFACE_IH_H_ */
