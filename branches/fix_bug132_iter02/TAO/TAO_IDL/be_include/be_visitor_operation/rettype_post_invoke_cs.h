//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    rettype_post_invoke_cs.h
//
// = DESCRIPTION
//    Visitor for generating code for IDL operations. This generates the code
//    for post processing of the return type variable after the docall is
//    made.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#ifndef _BE_VISITOR_OPERATION_RETTYPE_POST_INVOKE_CS_H_
#define _BE_VISITOR_OPERATION_RETTYPE_POST_INVOKE_CS_H_

// ************************************************************
// Operation visitor for postprocessing after invocation
// ************************************************************

class be_visitor_operation_rettype_post_invoke_cs : public be_visitor_decl
{
  //
  // = TITLE
  //   be_visitor_operation_rettype_post_docall
  //
  // = DESCRIPTION
  //   This is a visitor for post processing after the invocation in
  //   compiled marshaling
  //
  //
public:
  be_visitor_operation_rettype_post_invoke_cs (be_visitor_context *ctx);
  // constructor

  int visit_array (be_array *node);
  // visit an array node

  int visit_typedef (be_typedef *node);
  // visit a typedef node
};

#endif /* _BE_VISITOR_OPERATION_RETTYPE_POST_INVOKE_CS_H_ */
