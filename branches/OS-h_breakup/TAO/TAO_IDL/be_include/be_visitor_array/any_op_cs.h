//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    any_op_cs.h
//
// = DESCRIPTION
//    Visitors for generation of code for Arrays in client stubs. This one
//    generates the Any operators.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#ifndef _BE_VISITOR_ARRAY_ANY_OP_CS_H_
#define _BE_VISITOR_ARRAY_ANY_OP_CS_H_

// ************************************************************
// class be_visitor_array_cs
// ************************************************************

class be_visitor_array_any_op_cs : public be_visitor_decl
{
  //
  // = TITLE
  //   be_visitor_array_any_op_cs
  //
  // = DESCRIPTION
  //   This is a concrete visitor for array that generates the Any operator
  //   implementations
  //

public:
  be_visitor_array_any_op_cs (be_visitor_context *ctx);
  // constructor

  ~be_visitor_array_any_op_cs (void);
  // destructor

  virtual int visit_array (be_array *node);
  // visit array

};

#endif /* _BE_VISITOR_ARRAY_ANY_OP_CS_H_ */
