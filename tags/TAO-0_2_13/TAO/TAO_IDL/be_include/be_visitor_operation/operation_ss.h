//
// $Id$
//

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

#if !defined (_BE_VISITOR_OPERATION_OPERATION_SS_H_)
#define _BE_VISITOR_OPERATION_OPERATION_SS_H_

// ************************************************************
// Operation visitor for server skeletons
// ************************************************************

class be_visitor_operation_ss : public be_visitor_scope
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

  virtual int post_process (void);
  // stuff to output after every member of the scope is handled
};

#endif /* _BE_VISITOR_OPERATION_OPERATION_SS_H_ */
