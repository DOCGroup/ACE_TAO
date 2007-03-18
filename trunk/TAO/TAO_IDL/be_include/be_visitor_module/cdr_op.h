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
//    cdr_op.h
//
// = DESCRIPTION
//    Concrete visitor for the Module class
//    This one provides visitors to generate the CDR operators for elements in
//    the Module's scope.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#ifndef _BE_VISITOR_MODULE_CDR_OP_H_
#define _BE_VISITOR_MODULE_CDR_OP_H_

class be_visitor_module_cdr_op : public be_visitor_module
{
  //
  // = TITLE
  //   be_visitor_module_cdr_op
  //
  // = DESCRIPTION
  //   This is a concrete visitor for module that generates the CDR operator
  //   declarations and implementations
  //

public:
  be_visitor_module_cdr_op (be_visitor_context *ctx);
  // constructor

  ~be_visitor_module_cdr_op (void);
  // destructor

  virtual int visit_module (be_module *node);
  // visit module
};

#endif /* _BE_VISITOR_MODULE_CDR_OP_H_ */
