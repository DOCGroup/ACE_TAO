//
// $Id$
//
/* -*- c++ -*- */
// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    any_op_cs.h
//
// = DESCRIPTION
//    Concrete visitor for the valuebox node.
//    This one provides code generation for the Any operators
//
// = AUTHOR
//    Gary Maxey
//
// ============================================================================

#ifndef _BE_VALUEBOX_ANY_OP_CS_H_
#define _BE_VALUEBOX_ANY_OP_CS_H_

class be_visitor_valuebox_any_op_cs : public be_visitor_valuebox
{
  //
  // = TITLE
  //   be_visitor_valuebox_any_op_cs
  //
  // = DESCRIPTION
  //   This is a concrete visitor for valuebox that generates the Any
  //   operator implementations
  //

public:
  be_visitor_valuebox_any_op_cs (be_visitor_context *ctx);
  // constructor

  ~be_visitor_valuebox_any_op_cs (void);
  // destructor

  virtual int visit_valuebox (be_valuebox *node);
  // visit valuebox
};

#endif /* _BE_VALUEBOX_ANY_OP_CS_H_ */
