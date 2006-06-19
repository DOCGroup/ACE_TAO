// -*- C++ -*-
//
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    operation_ss.h
//
// = DESCRIPTION
//    Visitor for generating code for IDL operations in server skeletons
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#ifndef _BE_VISITOR_OPERATION_OPERATION_SS_H_
#define _BE_VISITOR_OPERATION_OPERATION_SS_H_

// ************************************************************
// Operation visitor for server skeletons
// ************************************************************

class be_visitor_operation_ss : public be_visitor_operation
{
  //
  // = TITLE
  //   be_visitor_operation_ss
  //
  // = DESCRIPTION
  //   This is a concrete visitor to generate the server skeletons for operation
  //
  //
public:
  be_visitor_operation_ss (be_visitor_context *ctx);
  // constructor

  ~be_visitor_operation_ss (void);
  // destructor

  virtual int visit_operation (be_operation *node);
  // visit operation. We provide code for this method in the derived class

  virtual int visit_argument (be_argument *node);
  // visit argument to generate ParamData entries

  // template methods

  virtual int gen_pre_skel_info (be_operation *node);
  // generate any pre skeleton code info

  /// Generate the skeleton operation body.
  int gen_skel_operation_body (be_operation * node,
                               be_type * return_type);

  /// Generate the skeleton operation argument list.
  void gen_skel_body_arglist (be_operation * node,
                              TAO_OutStream * os);
};

#endif /* _BE_VISITOR_OPERATION_OPERATION_SS_H_ */
