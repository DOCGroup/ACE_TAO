// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    root_ih.h
//
// = DESCRIPTION
//    Concrete visitor for the Root class
//    This one provides code generation for elements of the Root node in the
//    implementation header.
//
// = AUTHOR
//   Yamuna Krishnamurthy
//
// ============================================================================

#ifndef _BE_VISITOR_ROOT_ROOT_IH_H_
#define _BE_VISITOR_ROOT_ROOT_IH_H_



class be_visitor_root_ih : public be_visitor_root
{
  //
  // = TITLE
  //   be_visitor_root_ih
  //
  // = DESCRIPTION
  //   This is a concrete visitor to generate the implementation header for root
  //
  //
public:
  be_visitor_root_ih (be_visitor_context *ctx);
  // constructor

  ~be_visitor_root_ih (void);
  // destructor

  virtual int init (void);
  // set the right context and make a visitor
};

#endif /* _BE_VISITOR_ROOT_ROOT_IH_H_ */
