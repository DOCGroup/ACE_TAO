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
//    valuebox.h
//
// = DESCRIPTION
//    Concrete visitor for the Valuebox node.
//
// = AUTHOR
//    Gary Maxey
//
// ============================================================================

#ifndef _BE_VISITOR_VALUEBOX_H_
#define _BE_VISITOR_VALUEBOX_H_

class be_visitor_valuebox : public be_visitor_decl
{
  //
  // = TITLE
  //   be_visitor_valuebox
  //
  // = DESCRIPTION
  //   This is a concrete visitor to generate code for valueboxes.
  //
  //
public:
  be_visitor_valuebox (be_visitor_context *ctx);
  // constructor

  ~be_visitor_valuebox (void);
  // destructor

  virtual int visit_valuebox (be_valuebox *node);
  // visit attribute. We provide code for this method in the derived class
};

#endif /* _BE_VISITOR_VALUEBOX_H_ */
