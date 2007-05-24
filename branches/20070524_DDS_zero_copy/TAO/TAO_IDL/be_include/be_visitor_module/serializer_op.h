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
//    serializer_op.h
//
// = DESCRIPTION
//    Concrete visitor for the Module class
//    This one provides visitors to generate the 
//    TAO::DCPS::Serializer operators for elements in
//    the Module's scope.
//
// = AUTHOR
//    Scott Harris <harris_s@ociweb.com> based on code by Aniruddha Gokhale
//
// ============================================================================

#ifndef _BE_VISITOR_MODULE_serializer_op_H_
#define _BE_VISITOR_MODULE_serializer_op_H_

class be_visitor_module_serializer_op : public be_visitor_module
{
  //
  // = TITLE
  //   be_visitor_module_serializer_op
  //
  // = DESCRIPTION
  //   This is a concrete visitor for module that generates the Serializer operator
  //   declarations and implementations
  //

public:
  be_visitor_module_serializer_op (be_visitor_context *ctx);
  // constructor

  ~be_visitor_module_serializer_op (void);
  // destructor

  virtual int visit_module (be_module *node);
  // visit module
};

#endif /* _BE_VISITOR_MODULE_serializer_op_H_ */
