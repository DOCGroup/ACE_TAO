
//=============================================================================
/**
 *  @file    operation_cs.h
 *
 *  $Id$
 *
 *  Visitor for generating code for IDL operations in client stubs
 *
 *
 *  @author Aniruddha Gokhale & Angelo Corsaro
 */
//=============================================================================


#ifndef _BE_VISITOR_OPERATION_OPERATION_CS_H_
#define _BE_VISITOR_OPERATION_OPERATION_CS_H_

// ************************************************************
// Operation visitor for client stubs
// ************************************************************

/**
 * @class be_visitor_operation_cs
 *
 * @brief be_visitor_operation_cs
 *
 * This is a concrete visitor to generate the client stubs for operation
 */
class be_visitor_operation_cs : public be_visitor_operation
{
public:
  /// constructor
  be_visitor_operation_cs (be_visitor_context *ctx);

  /// destructor
  ~be_visitor_operation_cs (void);

  /// visit operation.
  virtual int visit_operation (be_operation *node);

  /// visit argument to generate ParamData entries
  virtual int visit_argument (be_argument *node);

  // =helper
  /// stuff to output after every member of the scope is handled
  virtual int post_process (be_decl *);
};

#endif /* _BE_VISITOR_OPERATION_OPERATION_CS_H_ */
