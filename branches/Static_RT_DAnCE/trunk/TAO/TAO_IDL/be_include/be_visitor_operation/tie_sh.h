//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    tie_sh.h
//
// = DESCRIPTION
//    Visitor for generating code for IDL operations for the TIE class inside
//    the server header
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#ifndef _BE_VISITOR_OPERATION_TIE_SH_H_
#define _BE_VISITOR_OPERATION_TIE_SH_H_

// ***********************************************************************
// Operation visitor for operation declaration inside the TIE class
// ***********************************************************************

class be_visitor_operation_tie_sh : public be_visitor_scope
{
  //
  // = TITLE
  //   be_visitor_operation_tie_sh
  //
  // = DESCRIPTION
  //   This is a concrete visitor to generate the operaton inside a TIE class
  //
  //
public:
  be_visitor_operation_tie_sh (be_visitor_context *ctx);
  // constructor

  ~be_visitor_operation_tie_sh (void);
  // destructor

  virtual int visit_operation (be_operation *node);
  // set the right context and make a visitor
};

#endif /* _BE_VISITOR_OPERATION_TIE_SH_H_ */
