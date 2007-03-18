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
//    any_op.h
//
// = DESCRIPTION
//    Concrete visitor for the Module class
//    This one provides visitors to generate the Any operators for elements in
//    the Module's scope.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#ifndef _BE_VISITOR_MODULE_ANY_OP_H_
#define _BE_VISITOR_MODULE_ANY_OP_H_

class be_visitor_module_any_op : public be_visitor_module
{
  //
  // = TITLE
  //   be_visitor_module_any_op
  //
  // = DESCRIPTION
  //   This is a concrete visitor for module that generates the Any operator
  //   declarations and implementations
  //

public:
  be_visitor_module_any_op (be_visitor_context *ctx);
  // constructor

  ~be_visitor_module_any_op (void);
  // destructor

  virtual int visit_module (be_module *node);
  // visit module
};

#endif /* _BE_VISITOR_MODULE_ANY_OP_H_ */
