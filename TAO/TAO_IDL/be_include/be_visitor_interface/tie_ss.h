/* -*- c++ -*- */
//=============================================================================
/**
 *  @file    tie_ss.h
 *
 *  Concrete visitor for the Interface node.
 *  This one provides the code generation for TIE classes
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================


#ifndef _BE_INTERFACE_TIE_SS_H_
#define _BE_INTERFACE_TIE_SS_H_

/**
 * @class be_visitor_interface_tie_ss
 *
 * @brief be_visitor_interface_tie_ss
 *
 * This is a concrete visitor to generate the server skeleton for operations
 * of the TIE class
 */
class be_visitor_interface_tie_ss : public be_visitor_interface
{
public:
  /// constructor
  explicit be_visitor_interface_tie_ss (be_visitor_context *ctx);

  /// destructor
  ~be_visitor_interface_tie_ss () override;
  be_visitor_interface_tie_ss (be_visitor_interface_tie_ss const &) = delete;
  be_visitor_interface_tie_ss (be_visitor_interface_tie_ss &&) = delete;
  be_visitor_interface_tie_ss &operator= (be_visitor_interface_tie_ss const &) = delete;
  be_visitor_interface_tie_ss &operator= (be_visitor_interface_tie_ss &&) = delete;

  /// set the right context and make a visitor
  int visit_interface (be_interface *node) override;

  /// set the right context and make a visitor
  int visit_component (be_component *node) override;

  /// helper method to generate method declaration in the TIE classes.
  static int method_helper (be_interface *,
                            be_interface *,
                            TAO_OutStream *os);
};

#endif /* _BE_INTERFACE_TIE_SS_H_ */
