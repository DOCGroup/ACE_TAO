
//=============================================================================
/**
 *  @file    operation_cs.h
 *
 *  Visitor for generating code for IDL operations in client stubs
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
  ~be_visitor_operation_cs ();

  /// visit operation.
  int visit_operation (be_operation *node) override;

  /// visit argument to generate ParamData entries
  int visit_argument (be_argument *node) override;

  // =helper
  /// stuff to output after every member of the scope is handled
  int post_process (be_decl *) override;
};

#endif /* _BE_VISITOR_OPERATION_OPERATION_CS_H_ */
