// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    root_is.h
//
// = DESCRIPTION
//    Concrete visitor for the Root class
//    This one provides code generation for elements of the Root node in the
//    implementation skeletons.
//
// = AUTHOR
//   Yamuna Krishnamurthy (yamuna@cs.wustl.edu)
//
// ============================================================================

#ifndef _BE_VISITOR_ROOT_ROOT_IS_H_
#define _BE_VISITOR_ROOT_ROOT_IS_H_

class be_visitor_root_is : public be_visitor_root
{
  //
  // = TITLE
  //   be_visitor_root_is
  //
  // = DESCRIPTION
  //   This is a concrete visitor to generate the implementation skeletons for root
  //
  //
public:
  be_visitor_root_is (be_visitor_context *ctx);
  // constructor

  ~be_visitor_root_is (void);
  // destructor

  virtual int init (void);
  // set the right context and make a visitor
};

#endif /* _BE_VISITOR_ROOT_ROOT_IS_H_ */
