// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    interface_is.h
//
// = DESCRIPTION
//    Concrete visitor for the Interface node.
//    This provides for code generation in the implementation skeleton
//
// = AUTHOR
//   Yamuna Krishnamurthy (yamuna@cs.wustl.edu)
//
// ============================================================================

#ifndef _BE_INTERFACE_INTERFACE_IS_H_
#define _BE_INTERFACE_INTERFACE_IS_H_

class be_visitor_interface_is : public be_visitor_interface
{
  //
  // = TITLE
  //   be_visitor_interface_is
  //
  // = DESCRIPTION
  //   This is a concrete visitor to generate the implementation skeletons for interface
  //
  //
public:
  be_visitor_interface_is (be_visitor_context *ctx);
  // constructor

  ~be_visitor_interface_is (void);
  // destructor

  virtual int visit_interface (be_interface *node);
  // set the right context and make a visitor


  static int method_helper (be_interface *derived,
                            be_interface *node,
                            TAO_OutStream *os);
};

#endif /* _BE_INTERFACE_INTERFACE_IS_H_ */
