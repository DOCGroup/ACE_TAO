//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    rettype_post_upcall_ss.h
//
// = DESCRIPTION
//    Visitor for generating code for IDL operations. This generates the code
//    for post processing of the return type variable after the upcall is
//    made.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#ifndef _BE_VISITOR_OPERATION_RETTYPE_POST_UPCALL_SS_H_
#define _BE_VISITOR_OPERATION_RETTYPE_POST_UPCALL_SS_H_

// ***********************************************************************
// Operation visitor for post upcall processing of return types
// ***********************************************************************

class be_visitor_operation_rettype_post_upcall_ss : public be_visitor_decl
{
  //
  // = TITLE
  //   be_visitor_operation_rettype_post_upcall
  //
  // = DESCRIPTION
  //   This is a visitor for post processing after an upcall
  //
  //
public:
  be_visitor_operation_rettype_post_upcall_ss (be_visitor_context *ctx);
  // constructor

  int visit_array (be_array *node);
  // visit an array node

  int visit_typedef (be_typedef *node);
  // visit a typedef node
};

#endif /* _BE_VISITOR_OPERATION_RETTYPE_POST_UPCALL_SS_H_ */
