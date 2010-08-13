// -*- C++ -*-

//=============================================================================
/**
 *  @file    operation_ss.h
 *
 *  $Id$
 *
 *  Visitor for generating code for IDL operations in server skeletons
 *
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================


#ifndef _BE_VISITOR_OPERATION_OPERATION_SS_H_
#define _BE_VISITOR_OPERATION_OPERATION_SS_H_

// ************************************************************
// Operation visitor for server skeletons
// ************************************************************

/**
 * @class be_visitor_operation_ss
 *
 * @brief be_visitor_operation_ss
 *
 * This is a concrete visitor to generate the server skeletons for operation
 */
class be_visitor_operation_ss : public be_visitor_operation
{
public:
  /// constructor
  be_visitor_operation_ss (be_visitor_context *ctx);

  /// destructor
  ~be_visitor_operation_ss (void);

  /// visit operation. We provide code for this method in the derived class
  virtual int visit_operation (be_operation *node);

  /// visit argument to generate ParamData entries
  virtual int visit_argument (be_argument *node);

  // template methods

  /// generate any pre skeleton code info
  virtual int gen_pre_skel_info (be_operation *node);

  /// Generate the skeleton operation body.
  int gen_skel_operation_body (be_operation * node,
                               be_type * return_type);

  /// Generate the skeleton operation argument list.
  void gen_skel_body_arglist (be_operation * node,
                              TAO_OutStream * os);
};

#endif /* _BE_VISITOR_OPERATION_OPERATION_SS_H_ */
