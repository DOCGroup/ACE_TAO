//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    exceptlist_cs.h
//
// = DESCRIPTION
//    Visitor for generating code for IDL operations. This generates the tables
//    for the exceptions used by the stubs.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#ifndef _BE_VISITOR_OPERATION_EXCEPTIONLIST_CS_H_
#define _BE_VISITOR_OPERATION_EXCEPTIONLIST_CS_H_

// ******************************************************************************
// Visitor for generating the exception list
// ******************************************************************************

class be_visitor_operation_exceptlist_cs : public be_visitor_decl
{
  //
  // = TITLE
  //   be_visitor_operaion_exceptlist_cs
  //
  // = DESCRIPTION
  //   This is a visitor to generate operation exception list to be provided to
  //   the do_static_call
  //
  //
public:
  be_visitor_operation_exceptlist_cs (be_visitor_context *ctx);
  // constructor

  ~be_visitor_operation_exceptlist_cs (void);
  // destructor

  int visit_operation (be_operation *node);
  // visit the operation

};

#endif /* _BE_VISITOR_OPERATION_EXCEPTIONLIST_CS_H_ */
