/* -*- c++ -*- */
//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    root_ex_idl.h
//
// = DESCRIPTION
//    Concrete visitor for the Root class
//    This one provides code generation for elements of the Root node in the
//    CIAO executor IDL.
//
// = AUTHOR
//    Jeff Parsons
//
// ============================================================================

#ifndef _BE_VISITOR_ROOT_ROOT_EX_IDL_H_
#define _BE_VISITOR_ROOT_ROOT_EX_IDL_H_

class be_visitor_root_ex_idl : public be_visitor_root
{
  //
  // = TITLE
  //   be_visitor_root_ex_idl
  //
  // = DESCRIPTION
  //   This is a concrete visitor to generate the CIAO executor
  //   IDL for root
  //
  //
public:
  be_visitor_root_ex_idl (be_visitor_context *ctx);

  ~be_visitor_root_ex_idl (void);

  virtual int init (void);
  // Set the right context and make a visitor.
};

#endif /* _BE_VISITOR_ROOT_ROOT_EX_IDL_H_ */
